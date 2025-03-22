//tcp_connect_hanbdler.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HELPER_TCP_CONNECT_HANDLER_HPP__
#define __YGGR_NETWORK_SESSION_HELPER_TCP_CONNECT_HANDLER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/network/session_helper/basic_tcp_connect_handler.hpp>

namespace yggr
{
namespace network
{
namespace session_helper
{

template<typename ServicePool,
			typename SessionMgrWrap,
			typename OptionsSetter,
			typename Error_Maker>
struct tcp_connect_handler 
	: public 
		basic_tcp_connect_handler
		<
			typename SessionMgrWrap::value_type::conn_ptr_type
		>
{
public:
	typedef ServicePool service_pool_type;
	typedef SessionMgrWrap session_mgr_wrap_type;
	typedef OptionsSetter options_setter_type;
	typedef Error_Maker error_maker_type;
	typedef typename error_maker_type::error_type error_type;

	typedef struct {} init_type;
protected:

	typedef typename session_mgr_wrap_type::value_type session_mgr_type;
	typedef typename session_mgr_wrap_type::value_ptr_type session_mgr_ptr_type;

	typedef typename session_mgr_type::conn_type conn_type;
	typedef typename session_mgr_type::conn_ptr_type conn_ptr_type;

	typedef basic_tcp_connect_handler< conn_ptr_type > base_type;
	typedef typename base_type::id_type id_type;
	typedef typename base_type::container_type container_type;
	
private:
	typedef tcp_connect_handler this_type;

public:
	tcp_connect_handler(service_pool_type& service_pool, 
							session_mgr_wrap_type& smgr_wrap,
							const init_type& init)
		: _service_pool(service_pool), _smgr_wrap(smgr_wrap)
	{
	}

	~tcp_connect_handler(void)
	{
		assert(base_type::backup_empty());
	}

public:
	template<typename Address, typename Port>
	void connect(const Address& addr, const Port& port)
	{
		conn_ptr_type pconn;
		try
		{
			pconn = conn_ptr_type(yggr_nothrow_new conn_type(_service_pool.get_service()));
		}
		catch(...)
		{
			conn_ptr_type tmp;
			tmp.swap(pconn);
		}
		
		if(!pconn)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc),
				yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			return;
		}

		base_type::s_connect(
			pconn,
			addr, 
			port,
			boost::bind(&this_type::pro_handler_connect, this, _1, _2) );
	}

	template<typename Address, typename Port, typename Time>
	void connect(const Address& addr, const Port& port, const Time& tm_duration)
	{
		conn_ptr_type pconn;
		try
		{
			pconn = conn_ptr_type(yggr_nothrow_new conn_type(_service_pool.get_service()));
		}
		catch(...)
		{
			conn_ptr_type tmp;
			tmp.swap(pconn);
		}
		
		if(!pconn)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc),
				yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			return;
		}

		base_type::s_connect(
			pconn,
			addr, 
			port,
			boost::bind(&this_type::pro_handler_connect, this, _1, _2),
			tm_duration);
	}

	inline void close(void)
	{
		base_type::clear_backup(boost::bind(&this_type::pro_s_handler_clear_backup, _1));
	}

protected:

	/*
	* return value bool is def stop_connect // true continue connect, false stop connect
	*/
	bool pro_handler_connect(const boost::system::error_code& e, conn_ptr_type pconn)
	{
		if(!pconn)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_empty_conn), 
				yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			return false;
		}

		if(!e)
		{
			if(!this_type::pro_s_options_setter()(*pconn))
			{
				pconn->close();
				this_type::pro_s_throw_error(
					error_maker_type::make_error(error_maker_type::E_invalid_access),
					yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
			}

			session_mgr_ptr_type smgr_ptr = _smgr_wrap.get_shared_ptr();
			if(!smgr_ptr)
			{
				pconn->close();
				this_type::pro_s_throw_error(
					error_maker_type::make_error(error_maker_type::E_invalid_client),
					yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
				return false;
			}

			smgr_ptr->insert(pconn);
			return false;
		}

		this_type::pro_s_throw_error(e, yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
		return true;
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

protected:
	service_pool_type& _service_pool;
	session_mgr_wrap_type& _smgr_wrap;
};

} // namespace session_helper
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SESSION_HELPER_TCP_CONNECT_HANDLER_HPP__