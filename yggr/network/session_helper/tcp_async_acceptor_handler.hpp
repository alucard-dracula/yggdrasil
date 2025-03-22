//tcp_async_acceptor_handler.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_NETWORK_SESSION_HELPER_TCP_ASYNC_ACCEPTOR_HANDLER_HPP__
#define __YGGR_NETWORK_SESSION_HELPER_TCP_ASYNC_ACCEPTOR_HANDLER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/system_controller/system_code.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/nonable/nonable.hpp>

#include <yggr/ids/base_ids_def.hpp>

#include <yggr/network/type_traits/tags.hpp>
#include <yggr/safe_container/safe_backup_device.hpp>

#include <boost/asio.hpp>

namespace yggr
{
namespace network
{
namespace session_helper
{

template<typename ServicePool,
			typename Acceptor, 
			typename SessionMgrWrap,
			typename OptionsSetter,
			typename Error_Maker>
class tcp_async_acceptor_handler
	: public 
		yggr::safe_container::safe_backup_device
		<
			ids::id64_type,
			typename SessionMgrWrap::value_type::conn_ptr_type,
			ids::pointer_id_generator<yggr::ids::id64_type>
		>,
		private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef struct {} init_type;

private:
	typedef ServicePool service_pool_type;
	typedef Acceptor acceptor_type;
	typedef SessionMgrWrap session_mgr_wrap_type;
	typedef OptionsSetter options_setter_type;
	typedef Error_Maker error_maker_type;
	typedef typename error_maker_type::error_type error_type;

	typedef ::yggr::shared_ptr<acceptor_type> acceptor_ptr_type;

	typedef typename service_pool_type::service_type service_type;
	typedef typename session_mgr_wrap_type::value_type session_mgr_type;
	typedef typename session_mgr_wrap_type::value_ptr_type session_mgr_ptr_type;
	typedef typename session_mgr_type::session_type session_type;
	typedef typename session_mgr_type::conn_type conn_type;
	typedef typename session_mgr_type::conn_ptr_type conn_ptr_type;

	typedef 
		yggr::safe_container::safe_backup_device
		<
			ids::id64_type,
			conn_ptr_type,
			ids::pointer_id_generator<yggr::ids::id64_type>
		> base_type;

	typedef typename base_type::key_type id_type;
	typedef typename base_type::container_type container_type;

private:
	typedef tcp_async_acceptor_handler this_type;

public:
	tcp_async_acceptor_handler(service_pool_type& pool,
								acceptor_ptr_type pacceptor,
								session_mgr_wrap_type& smgr_wrap,
								const init_type& init)
		: _service_pool(pool), 
			_pacceptor(pacceptor), 
			_session_mgr_wrap(smgr_wrap)
	{
	}

	~tcp_async_acceptor_handler(void)
	{
		assert(base_type::backup_empty());
	}

public:
	void listen(void)
	{
		if(!_pacceptor)
		{
			return;
		}

		service_type& s = _service_pool.get_service();
		conn_ptr_type ptr;
		std::pair<id_type, bool> id_pair(id_type(), false);
		for(;!ptr;)
		{
			conn_ptr_type tptr;
			try
			{
				tptr = conn_ptr_type(yggr_nothrow_new conn_type(s));
			}
			catch(...)
			{
				conn_ptr_type tmp;
				tmp.swap(tptr);
			}

			if(!tptr)
			{
				this_type::pro_s_throw_error(
					error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc),
					yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
				continue;
			}

			ptr.swap(tptr);
			if(!((id_pair = base_type::backup(ptr)).second))
			{
				conn_ptr_type tmp;
				tmp.swap(ptr);
				tmp->close();
			}
		}

		assert((ptr && id_pair.second && id_pair.first != id_type()));

		_pacceptor->async_accept(
			ptr->get_link(),
			boost::bind( 
				&this_type::pro_handler_accept,
				this,
				boost::asio::placeholders::error,
				id_pair.first ) );
	}

	inline void close(void)
	{
		base_type::clear_backup(boost::bind(&this_type::pro_s_handler_clear_backup, _1));
	}

private:
	void pro_handler_accept(const boost::system::error_code& e, const id_type& id)
	{
		conn_ptr_type pconn = base_type::drop_backup(id);
		
		listen();

		if(!pconn)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_empty_conn),
				yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			return;
		}

		if(e)
		{
			pconn->close();
			this_type::pro_s_throw_error(e, yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
		}
		else
		{
			if(!this_type::pro_s_options_setter()(*pconn))
			{
				this_type::pro_s_throw_error(
					error_maker_type::make_error(error_maker_type::E_invalid_access),
					yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
				pconn->close();
				return;
			}

			session_mgr_ptr_type psmgr = _session_mgr_wrap.get_shared_ptr();
			if(!psmgr)
			{
				this_type::pro_s_throw_error(
					error_maker_type::make_error(error_maker_type::E_invalid_server),
					yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
				return;
			}

			psmgr->insert(pconn);
		}

#ifdef _DEBUG
		std::cout << "ACCEPT" << std::endl; // Debug
#endif //_DEBUG
	}

	static void pro_s_handler_clear_backup(container_type& cont)
	{
		typedef typename container_type::iterator iter_type;

		for(iter_type i = cont.begin(), isize = cont.end(); i != isize; ++i)
		{
			if(i->second)
			{
				i->second->close();
			}
		}

		cont.clear();
	}

protected:
	inline static const options_setter_type& pro_s_options_setter(void)
	{
		static const options_setter_type setter;
		return setter;
	}

protected:
	template<typename Error> inline
	static void pro_s_throw_error(const Error& e)
	{
		exception::exception::throw_error(e);
	}

	template<typename Error> inline
	static void pro_s_throw_error(const Error& e, u32 sys_code)
	{
		exception::exception::throw_error(e, sys_code);
	}

private:
	service_pool_type& _service_pool;
	acceptor_ptr_type _pacceptor;
	session_mgr_wrap_type& _session_mgr_wrap;
};

} // namespace session_helper
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SESSION_HELPER_TCP_ASYNC_ACCEPTOR_HANDLER_HPP__