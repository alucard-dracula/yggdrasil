//udp_connect_hanbdler.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HELPER_UDP_CONNECT_HANDLER_HPP__
#define __YGGR_NETWORK_SESSION_HELPER_UDP_CONNECT_HANDLER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/container/data_range.hpp>

#include <yggr/network/session_helper/basic_udp_connect_handler.hpp>
#include <yggr/network/udp_helper/udp_socket_pool.hpp>
#include <yggr/network/udp_packet_wrap.hpp>
#include <yggr/network/udp_packets_collecter.hpp>
#include <yggr/network/udp_ctrl_data/udp_conn_checker.hpp>

#include <sstream>

namespace yggr
{
namespace network
{
namespace session_helper
{
namespace detail
{

struct udp_connect_handler_init
{
private:
	typedef udp_connect_handler_init this_type;

public:
	udp_connect_handler_init(const size_type& npool_size)
		: pool_size(npool_size)
	{
	}

	udp_connect_handler_init(const this_type& right)
		: pool_size(right.pool_size)
	{
	}

	~udp_connect_handler_init(void)
	{
	}

public:
	size_type pool_size;
};

} // namespace detail
} // namespace session_helper
} // namespace network
} // namespace yggr

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
struct udp_connect_handler
	: public
		basic_udp_connect_handler
		<
			typename SessionMgrWrap::value_type::conn_ptr_type,
			typename SessionMgrWrap::value_type::task_type::owner_info_type
		>
{
public:
	typedef ServicePool service_pool_type;
	typedef SessionMgrWrap session_mgr_wrap_type;
	typedef OptionsSetter options_setter_type;
	typedef Error_Maker error_maker_type;
	typedef typename error_maker_type::error_type error_type;

	typedef detail::udp_connect_handler_init init_type;

protected:
	typedef typename service_pool_type::service_type service_type;

	typedef typename session_mgr_wrap_type::value_type session_mgr_type;
	typedef typename session_mgr_wrap_type::value_ptr_type session_mgr_ptr_type;

	typedef typename session_mgr_type::conn_type conn_type;
	typedef typename session_mgr_type::conn_ptr_type conn_ptr_type;

	typedef typename session_mgr_type::start_mode_type start_mode_type;

	typedef
		basic_udp_connect_handler
		<
			conn_ptr_type,
			typename session_mgr_type::task_type::owner_info_type
		> base_type;

	typedef typename base_type::id_type id_type;
	typedef typename base_type::container_type container_type;

	typedef typename base_type::resolver_type resolver_type;
	typedef typename base_type::resolver_iter_type resolver_iter_type;
	typedef typename base_type::resolver_query_type resolver_query_type;

protected:
	typedef typename conn_type::socket_type socket_type;
	typedef typename conn_type::endpoint_type endpoint_type;
	typedef ::yggr::shared_ptr<endpoint_type> endpoint_ptr_type;

	typedef
		udp_helper::udp_socket_pool
		<
			socket_type,
			endpoint_type,
			service_type,
			conn_type::E_NOW_IP_VERSION
		> udp_socket_pool_type;

	typedef ::yggr::shared_ptr<udp_socket_pool_type> udp_socket_pool_ptr_type;

	typedef typename udp_socket_pool_type::socket_ptr_type socket_ptr_type;

	typedef typename session_mgr_type::data_buf_type data_buf_type;
	typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;

	typedef network::udp_packet_wrap<data_buf_type> udp_packet_wrap_type;
	typedef network::udp_packets_collecter<udp_packet_wrap_type> udp_packets_collecter_type;

	typedef typename udp_packets_collecter_type::packet_wrap_ptr_type packet_wrap_ptr_type;

protected:
	typedef typename session_mgr_type::recv_data_type recv_data_type;
	typedef typename recv_data_type::buf_type recv_buf_type;

	typedef recv_buf_type wrap_recv_buf;

	typedef wrap_recv_buf wrap_recv_buf_type;
	typedef safe_container::safe_buffered_object<wrap_recv_buf_type> recv_buf_queue_type;
	typedef typename recv_buf_queue_type::obj_ptr_type recv_buf_ptr_type;

private:
	typedef udp_connect_handler this_type;

#define _s_recv_buf_queue _s_recv_buf_queue()

public:
//	udp_connect_handler(service_pool_type& service_pool,
//							session_mgr_wrap_type& smgr_wrap,
//							const init_type& init)
//		: _service_pool(service_pool), _smgr_wrap(smgr_wrap),
//			_pudp_socket_pool(construct_shared<udp_socket_pool_type>(yggr_nothrow_new udp_socket_pool_type(init.pool_size)))
//	{
//#ifdef YGGR_AT_WINDOWS
//		// this code notify windows firewall only
//		typedef typename base_type::protocol_type now_protocol_type;
//		{
//			try
//			{
//				socket_type tmp(_service_pool.get_service(),
//									endpoint_type(
//										session_mgr_type::is_v4()?
//											now_protocol_type::v4()
//											: now_protocol_type::v6(),
//											65000) );
//			}
//			catch(...)
//			{
//			}
//		}
//#endif // YGGR_AT_WINDOWS
//	}


#if (defined(_MSC_VER) || (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION))
   	udp_connect_handler(service_pool_type& service_pool,
							session_mgr_wrap_type& smgr_wrap,
							const init_type& init)
		: _service_pool(service_pool), _smgr_wrap(smgr_wrap),
			_pudp_socket_pool(construct_shared<udp_socket_pool_type>(yggr_nothrow_new udp_socket_pool_type(init.pool_size)))
#else
	udp_connect_handler(const service_pool_type& service_pool,
						const session_mgr_wrap_type& smgr_wrap,
						const init_type& init)
	: _service_pool(const_cast<service_pool_type&>(service_pool)), _smgr_wrap(const_cast<session_mgr_wrap_type&>(smgr_wrap)),
		_pudp_socket_pool(construct_shared<udp_socket_pool_type>(yggr_nothrow_new udp_socket_pool_type(init.pool_size)))
#endif //
	{
#if defined(YGGR_AT_WINDOWS)
		// this code notify windows firewall only
		typedef typename base_type::protocol_type now_protocol_type;
		{
			try
			{
				socket_type tmp(_service_pool.get_service(),
									endpoint_type(
										session_mgr_type::is_v4()?
											now_protocol_type::v4()
											: now_protocol_type::v6(),
											65000) );
			}
			catch(...)
			{
			}
		}
#endif // YGGR_AT_WINDOWS
	}

	~udp_connect_handler(void)
	{
		assert(base_type::backup_empty());
		assert((_pudp_socket_pool && _pudp_socket_pool->empty()));
	}

public:
	template<typename Address, typename Port>
	void connect(const Address& addr, const Port& port)
	{
		assert(_pudp_socket_pool);
		socket_ptr_type psocket =
			_pudp_socket_pool->get(
				_service_pool.get_service(),
				this_type::pro_s_options_setter());

		assert(psocket);
		if(!psocket)
		{
			return;
		}

		conn_ptr_type pconn(
			this_type::pro_s_create_connect(
				*psocket,
				boost::bind(
					&this_type::pro_s_back_socket,
					_1, _pudp_socket_pool, psocket) ));

		if(!pconn)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc),
				yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		this_type::recv(psocket);

		base_type::s_connect(
			pconn,
			addr,
			port,
			boost::bind(&this_type::pro_s_handler_connect, _1, _2) );
	}

	template<typename Address, typename Port, typename Time>
	void connect(const Address& addr, const Port& port, const Time& tm_duration)
	{
		assert(_pudp_socket_pool);
		socket_ptr_type psocket =
			_pudp_socket_pool->get(
				_service_pool.get_service(),
				this_type::pro_s_options_setter());

		assert(psocket);
		if(!psocket)
		{
			return;
		}

		conn_ptr_type pconn(
			this_type::pro_s_create_connect(
				*psocket,
				boost::bind(
					&this_type::pro_s_back_socket,
					_1, _pudp_socket_pool, psocket) ));

		if(!pconn)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc),
				yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		this_type::recv(psocket);

		base_type::s_connect(
			pconn,
			addr,
			port,
			boost::bind(&this_type::pro_s_handler_connect, _1, _2),
			tm_duration );
	}

	inline void close(void)
	{
		assert(_pudp_socket_pool);
		base_type::clear_backup(boost::bind(&this_type::pro_s_handler_clear_backup, _1));
		_pudp_socket_pool->close();
	}

protected:
	/*
	*	return value bool is def stop_connect // true continue connect, false stop connect
	*/
	static bool pro_s_handler_connect(const boost::system::error_code& e, conn_ptr_type pconn)
	{
		if(!pconn)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_empty_conn),
				yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return false;
		}

		if(!e)
		{
			pconn->close();
			return false;
		}

		return true;
	}

	void recv(socket_ptr_type psocket)
	{
		if(!psocket)
		{
			return;
		}

		recv_buf_ptr_type precv_buf = _s_recv_buf_queue.drop();

		if(!precv_buf)
		{
			boost::system::error_code e(boost::asio::error::no_buffer_space);
			this_type::pro_s_throw_error(e, yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		precv_buf->resize(udp_packet_wrap_type::E_MAX_SINGLE_BUF_LENGTH);

		endpoint_ptr_type ped = ::yggr::make_shared<endpoint_type>();
		if(!ped)
		{
			return;
		}

		psocket->async_receive_from(
			boost::asio::buffer(
				&((*precv_buf)[0]),
				udp_packet_wrap_type::E_MAX_SINGLE_BUF_LENGTH),
				*ped,
				boost::bind(
					&this_type::handler_recv, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred,
					precv_buf, ped, psocket) );
	}

	void handler_recv(const boost::system::error_code& e,
						size_t recv_size,
						recv_buf_ptr_type pbuf,
						endpoint_ptr_type ped,
						socket_ptr_type psocket)
	{
		this_type::recv(psocket);

		if(e)
		{
			this_type::pro_s_throw_error(e, yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		packet_wrap_ptr_type pwobj = _collecter.collect(*pbuf);
		if(!pwobj)
		{
			return;
		}

		const udp_packet_wrap_type& pak_wrap = *pwobj;
		recv_data_type rd;
		pak_wrap >> rd;

		if(!rd.owner_id())
		{
			accept_complete(psocket, ped, rd);
			return;
		}

		this_type::recv_complete(ped, rd);
	}

	inline void accept_complete(socket_ptr_type psocket, endpoint_ptr_type ped, recv_data_type& data)
	{
		assert(data.owner_id() == 0);

		if(!psocket)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_access),
				yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		this_type::pro_insert_to_session(psocket, ped, data);
	}

	inline void recv_complete(endpoint_ptr_type ped, recv_data_type& data)
	{
		assert(data.owner_id() != 0);

		session_mgr_ptr_type psmgr = _smgr_wrap.get_shared_ptr();
		if(!psmgr)
		{
			return;
		}

		psmgr->send_to_adapter(data);
	}

	void pro_insert_to_session(socket_ptr_type psocket, endpoint_ptr_type ped, recv_data_type& data)
	{
		if(!psocket)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_access),
				yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		conn_ptr_type pconn(
			this_type::pro_s_create_connect(
				*psocket,
				boost::bind(
					&this_type::pro_s_back_socket,
					_1, _pudp_socket_pool, psocket) ));

		if(!pconn)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc),
				yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		pconn->connect(*ped);

		session_mgr_ptr_type psmgr = _smgr_wrap.get_shared_ptr();
		if(!psmgr)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_client),
				yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		start_mode_type start_mode(boost::move(data));
		psmgr->insert(pconn, start_mode);
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

	template<typename Handler> inline
	static conn_ptr_type pro_s_create_connect(socket_type& s, Handler handler)
	{
		try
		{
			return conn_ptr_type(yggr_nothrow_new conn_type(s, handler));
		}
		catch(...)
		{
			return conn_ptr_type();
		}
	}

protected:
	inline static void pro_s_back_socket(socket_type& s, udp_socket_pool_ptr_type psocket_pool, socket_ptr_type psocket)
	{
		assert(psocket_pool);
		assert(((psocket) && (boost::addressof(s) == boost::addressof(*psocket))));

		if(psocket_pool)
		{
			psocket_pool->back(psocket);
		}
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

#undef _s_recv_buf_queue

protected:
	service_pool_type& _service_pool;
	session_mgr_wrap_type& _smgr_wrap;
	udp_socket_pool_ptr_type _pudp_socket_pool;
	udp_packets_collecter_type _collecter;

	inline static recv_buf_queue_type& _s_recv_buf_queue(void)
	{
		static recv_buf_queue_type queue;
		return queue;
	}
};


} // namespace session_helper
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SESSION_HELPER_UDP_CONNECT_HANDLER_HPP__
