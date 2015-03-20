//tcp_async_acceptor_handler.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#include <boost/asio.hpp>
//#include <boost/mpl/assert.hpp>
//#include <boost/type_traits/is_same.hpp>

#include <yggr/base/exception.hpp>
#include <yggr/base/system_code.hpp>
#include <yggr/nonable/nonable.hpp>

#include <yggr/ids/base_ids_def.hpp>
#include <yggr/ids/id_generator.hpp>

#include <yggr/network/type_traits/tags.hpp>
#include <yggr/safe_container/safe_backup_device.hpp>

namespace yggr
{
namespace network
{
namespace session_helper
{

template<typename ServicePool,
			typename Acceptor, 
			typename SessionMgrWrap,
			typename Error_Maker>
class tcp_async_acceptor_handler
	: public yggr::safe_container::safe_backup_device
				<
					yggr::ids::id64_type,
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
	typedef Error_Maker error_maker_type;
	typedef typename error_maker_type::error_type error_type;

	typedef boost::shared_ptr<acceptor_type> acceptor_ptr_type;

	typedef typename service_pool_type::service_type service_type;
	typedef typename session_mgr_wrap_type::value_type session_mgr_type;
	typedef typename session_mgr_wrap_type::value_ptr_type session_mgr_ptr_type;
	typedef typename session_mgr_type::session_type session_type;
	typedef typename session_mgr_type::conn_type conn_type;
	typedef typename session_mgr_type::conn_ptr_type conn_ptr_type;

	typedef yggr::safe_container::safe_backup_device
				<
					yggr::ids::id64_type,
					conn_ptr_type,
					ids::pointer_id_generator<yggr::ids::id64_type>
				> base_type;
	typedef typename base_type::key_type id_type;
	typedef typename base_type::container_type container_type;

//private:
//	typedef typename session_mgr_type::tag_type tag_type;
//	BOOST_MPL_ASSERT((
//						boost::is_same
//						<
//							tag_type,
//							yggr::network::type_traits::tag_server
//						>
//					));

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

	void listen(void)
	{
		if(!_pacceptor)
		{
			return;
		}

		service_type& s = _service_pool.get_service();
		conn_ptr_type ptr;
		id_type id = id_type();
		for(;!ptr;)
		{
			conn_ptr_type tptr(new conn_type(s));

			if(!tptr)
			{
				exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc));
				continue;
			}

			ptr.swap(tptr);
			id = base_type::backup(ptr);
			if(id == id_type())
			{
				conn_ptr_type tmp;
				tmp.swap(ptr);
				tmp->close();
			}
		}

		assert((ptr && id != id_type()));
		
		//id_type id = base_type::backup(ptr);
		_pacceptor->async_accept(ptr->get_link(),
									boost::bind( &this_type::handler_accept,
													this,
													boost::asio::placeholders::error,
													id
												)
									);

	}

	void close(void)
	{
		base_type::clear_backup(boost::bind(&this_type::handler_clear_backup, this, _1));
	}

private:

	void handler_accept(const boost::system::error_code& e, const id_type& id)
	{
		conn_ptr_type pconn = base_type::drop_backup(id);
		
		listen();

		if(!pconn)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc));
			return;
		}

		if(e)
		{
			pconn->close();
			exception::exception::throw_error(e);
		}
		else
		{
			session_mgr_ptr_type psmgr = _session_mgr_wrap.get_shared_ptr();
			if(!psmgr)
			{
				exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc));
				return;
			}

			psmgr->insert(pconn); //检查到这里
		}

#ifdef _DEBUG
		std::cout << "ACCEPT" << std::endl; // Debug
#endif //_DEBUG
	}

	void handler_clear_backup(container_type& cont) const
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

private:
	service_pool_type& _service_pool;
	//acceptor_type& _acceptor;
	acceptor_ptr_type _pacceptor;
	session_mgr_wrap_type& _session_mgr_wrap;
};

} // namespace session_helper
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SESSION_HELPER_TCP_ASYNC_ACCEPTOR_HANDLER_HPP__