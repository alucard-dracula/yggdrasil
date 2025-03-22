//udp_async_acceptor_handler.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HELPER_UDP_ASYNC_ACCEPTOR_HANDLER_HPP__
#define __YGGR_NETWORK_SESSION_HELPER_UDP_ASYNC_ACCEPTOR_HANDLER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/bind/bind.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/system_controller/system_code.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/container/data_range.hpp>

#include <yggr/safe_container/safe_buffered_object.hpp>
#include <yggr/safe_container/safe_backup_device.hpp>

#include <yggr/network/hn_conv.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/udp_helper/udp_socket_pool.hpp>
#include <yggr/network/udp_packet_wrap.hpp>
#include <yggr/network/udp_packets_collecter.hpp>
#include <yggr/network/udp_ctrl_data/udp_conn_checker.hpp>

#include <yggr/ids/base_ids_def.hpp>

#include <boost/asio.hpp>
#include <cassert>

namespace yggr
{
namespace network
{
namespace session_helper
{

template<
			typename ServicePool,
			typename Acceptor,
			typename SessionMgrWrap,
			typename OptionsSetter,
			typename Error_Maker
		>
class udp_async_acceptor_handler
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef struct tag_init
	{
		tag_init(const size_type& npool_size)
			: pool_size(npool_size)
		{
		}

		tag_init(const tag_init& right)
			: pool_size(right.pool_size)
		{
		}

		~tag_init(void)
		{
		}

		size_type pool_size;
	} init_type;

protected:
	typedef ServicePool service_pool_type;
	typedef Acceptor acceptor_type;
	typedef SessionMgrWrap session_mgr_wrap_type;
	typedef OptionsSetter options_setter_type;
	typedef Error_Maker error_maker_type;
	typedef typename error_maker_type::error_type error_type;

	typedef typename service_pool_type::service_type service_type;

	typedef typename session_mgr_wrap_type::value_type session_mgr_type;
	typedef typename session_mgr_wrap_type::value_ptr_type session_mgr_ptr_type;
	typedef typename session_mgr_type::session_type session_type;
	typedef typename session_mgr_type::conn_type conn_type;
	typedef typename session_mgr_type::conn_ptr_type conn_ptr_type;

	typedef typename session_type::id_type id_type;

	typedef ::yggr::shared_ptr<acceptor_type> acceptor_ptr_type;

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

	BOOST_MPL_ASSERT((boost::is_same<acceptor_type, socket_type>));

	typedef typename session_mgr_type::data_buf_type data_buf_type;
	typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;

	typedef network::udp_packet_wrap<data_buf_type> udp_packet_wrap_type;
	typedef network::udp_packets_collecter<udp_packet_wrap_type> udp_packets_collecter_type;

	typedef typename udp_packets_collecter_type::packet_wrap_ptr_type packet_wrap_ptr_type;

protected:
	typedef typename session_mgr_type::recv_data_type recv_data_type;
	typedef typename session_mgr_type::send_data_type send_data_type;

	typedef typename recv_data_type::buf_type recv_buf_type;

	typedef recv_buf_type wrap_recv_buf;

	typedef wrap_recv_buf wrap_recv_buf_type;
	typedef safe_container::safe_buffered_object<wrap_recv_buf_type> recv_buf_queue_type;
	typedef typename recv_buf_queue_type::obj_ptr_type recv_buf_ptr_type;

	typedef ::yggr::shared_ptr<recv_data_type> recv_data_ptr_type;

	typedef yggr::network::udp_ctrl_data::udp_conn_checker<> udp_conn_checker_type;


private:
	typedef udp_async_acceptor_handler this_type;

#define _s_recv_buf_queue _s_recv_buf_queue()

public:
	udp_async_acceptor_handler(service_pool_type& pool,
								acceptor_ptr_type pacceptor,
								session_mgr_wrap_type& smgr_wrap,
								const init_type& init)
		: _service_pool(pool),
			_pacceptor(pacceptor),
			_session_mgr_wrap(smgr_wrap),
			_pudp_socket_pool(
				construct_shared<udp_socket_pool_type>(
				yggr_nothrow_new udp_socket_pool_type(init.pool_size)))
	{
		assert(_pudp_socket_pool);
	}

	udp_async_acceptor_handler(service_pool_type& pool,
								acceptor_ptr_type pacceptor,
								session_mgr_wrap_type& smgr_wrap,
								size_type pool_size)
		: _service_pool(pool),
			_pacceptor(pacceptor),
			_session_mgr_wrap(smgr_wrap),
			_pudp_socket_pool(
				construct_shared<udp_socket_pool_type>(
					yggr_nothrow_new udp_socket_pool_type(pool_size)))
	{
		assert(_pudp_socket_pool);
	}

	~udp_async_acceptor_handler(void)
	{
		assert((_pudp_socket_pool && _pudp_socket_pool->empty()));
	}

public:
	void listen(void)
	{
		if(!_pacceptor)
		{
			return;
		}

		this_type::recv(_pacceptor);
	}

	void close(void)
	{
		assert(_pudp_socket_pool);
		_pudp_socket_pool->close();
		_collecter.clear();
	}

	void check_state(void)
	{
		_collecter.check_state();
	}

protected:

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
			boost::asio::buffer(&((*precv_buf)[0]), udp_packet_wrap_type::E_MAX_SINGLE_BUF_LENGTH),
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

		if(!pbuf)
		{
			boost::system::error_code e(boost::asio::error::no_buffer_space);
			this_type::pro_s_throw_error(e, yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		const data_buf_type& buf_cref = *pbuf;

		packet_wrap_ptr_type pwobj = _collecter.collect(buf_cref);
		if(!pwobj)
		{
			return;
		}

		const udp_packet_wrap_type& pak_wrap = *pwobj;
		recv_data_type rd;
		pak_wrap >> rd;

		_collecter.back_buffer(pwobj);

		if(!rd.owner_id())
		{
			this_type::accept_complete(ped, rd);
		}
		else
		{
			this_type::recv_complete(ped, rd);
		}
	}

	void accept_complete(endpoint_ptr_type ped, recv_data_type& data)
	{
		assert(data.owner_id() == id_type());
		assert(_pudp_socket_pool);

		socket_ptr_type psocket =
			_pudp_socket_pool->get(
				_service_pool.get_service(),
				this_type::pro_s_options_setter());

		assert(psocket);
		if(!psocket)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_access),
				yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		this_type::pro_insert_to_session(psocket, ped, data);
		this_type::recv(psocket);
	}

	void recv_complete(endpoint_ptr_type ped, recv_data_type& data)
	{
		assert(data.owner_id() != id_type());

		session_mgr_ptr_type psmgr = _session_mgr_wrap.get_shared_ptr();
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

		udp_conn_checker_type checker;
		if(!( data.load(checker) && checker.check()))
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
			this_type::pro_s_throw_error(error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc));
			return;
		}

#ifdef _DEBUG
		std::cout << *ped  << " connectd" << std::endl;
#endif // _DEBUG

		pconn->connect(*ped);

		session_mgr_ptr_type psmgr = _session_mgr_wrap.get_shared_ptr();
		if(!psmgr)
		{
			this_type::pro_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_invalid_server),
				yggr::system_controller::system_code::E_UDP_BASE_NETWORK_SYSTEM);
			return;
		}

		psmgr->insert(pconn);
	}

protected:
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
	acceptor_ptr_type _pacceptor;
	session_mgr_wrap_type& _session_mgr_wrap;
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

#endif // __YGGR_NETWORK_SESSION_HELPER_UDP_ASYNC_ACCEPTOR_HANDLER_HPP__
