//udp_async_acceptor_handler.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HELPER_UDP_ASYNC_ACCEPTOR_HANDLER_HPP__
#define __YGGR_NETWORK_SESSION_HELPER_UDP_ASYNC_ACCEPTOR_HANDLER_HPP__

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/mpl/assert.hpp>
//#include <boost/type_traits/is_same.hpp>

#include <yggr/base/exception.hpp>
#include <yggr/base/system_code.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/container/data_range.hpp>

#include <yggr/safe_container/safe_buffered_object.hpp>

#include <yggr/network/hn_conv.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/udp_helper/udp_socket_pool.hpp>
#include <yggr/network/udp_packet_wrap.hpp>
#include <yggr/network/udp_packets_collecter.hpp>
#include <yggr/network/udp_ctrl_data/udp_conn_checker.hpp>

#include <yggr/ids/base_ids_def.hpp>
#include <yggr/ids/id_generator.hpp>
#include <yggr/safe_container/safe_backup_device.hpp>

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
			typename Error_Maker
		>
class udp_async_acceptor_handler
	:  public yggr::safe_container::safe_backup_device
				<
					yggr::ids::id64_type,
					typename SessionMgrWrap::value_type::conn_ptr_type,
					ids::pointer_id_generator<yggr::ids::id64_type>
				>,
		private nonable::noncopyable,
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

private:
	typedef ServicePool service_pool_type;
	typedef Acceptor acceptor_type;
	typedef SessionMgrWrap session_mgr_wrap_type;
	typedef Error_Maker error_maker_type;
	typedef typename error_maker_type::error_type error_type;

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

	typedef boost::shared_ptr<acceptor_type> acceptor_ptr_type;

private:
	typedef typename conn_type::socket_type socket_type;
	typedef typename conn_type::endpoint_type endpoint_type;
	typedef boost::shared_ptr<endpoint_type> endpoint_ptr_type;

	typedef udp_helper::udp_socket_pool
			<
				socket_type,
				endpoint_type,
				service_type,
				conn_type::E_NOW_IP_VERSION
			> udp_socket_pool_type;

	typedef typename udp_socket_pool_type::socket_ptr_type socket_ptr_type;

	BOOST_MPL_ASSERT((boost::is_same<acceptor_type, socket_type>));
	
	typedef typename session_mgr_type::data_buf_type data_buf_type;
	typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;

	typedef network::udp_packet_wrap<data_buf_type> udp_packet_wrap_type;
	typedef network::udp_packets_collecter<udp_packet_wrap_type> udp_packets_collecter_type;
	
	typedef typename udp_packets_collecter_type::wrap_obj_ptr_type wrap_obj_ptr_type;

private:
	typedef typename session_mgr_type::recv_data_type recv_data_type;
	typedef typename session_mgr_type::send_data_type send_data_type;

	typedef typename recv_data_type::buf_type recv_buf_type;

	class wrap_recv_buf : public recv_buf_type
	{
	public:
		inline void splice_to(const wrap_recv_buf& right)
		{
		}

		inline operator recv_buf_type&(void)
		{
			return *this;
		}

		inline operator const recv_buf_type&(void) const
		{
			return *this;
		}
	};

	typedef wrap_recv_buf wrap_recv_buf_type;
	typedef safe_container::safe_buffered_object<wrap_recv_buf_type> recv_buf_queue_type;
	typedef typename recv_buf_queue_type::obj_ptr_type recv_buf_ptr_type;

	typedef boost::shared_ptr<recv_data_type> recv_data_ptr_type;

	typedef yggr::network::udp_ctrl_data::udp_conn_checker<> udp_conn_checker_type;

	typedef container::data_range<u16> ports_type;
	typedef ports_type::error_type ports_error_type;

private:
	typedef udp_async_acceptor_handler this_type;

public:

	udp_async_acceptor_handler(service_pool_type& pool,
									acceptor_ptr_type pacceptor,
									session_mgr_wrap_type& smgr_wrap,
									const init_type& init)
		: _service_pool(pool), 
			_pacceptor(pacceptor), 
			_session_mgr_wrap(smgr_wrap),
			_udp_socket_pool(init.pool_size)
	{
	}

	~udp_async_acceptor_handler(void)
	{
		assert(base_type::backup_empty());
		assert(_udp_socket_pool.empty());
	}

	void listen(void)
	{
		if(!_pacceptor)
		{
			return;
		}

		recv(_pacceptor);
	}

	void close(void)
	{
		base_type::clear_backup(boost::bind(&this_type::handler_clear_backup, this, _1));
		_udp_socket_pool.close();
		_collecter.clear();
	}

	void check_state(void)
	{
		_collecter.check_state();
	}

private:

	void recv(socket_ptr_type psocket)
	{
		if(!psocket)
		{
			return;
		}

		recv_buf_ptr_type precv_buf = _s_recv_buf_queue.drop();

		if(!precv_buf)
		{
			boost::system::error_code err(boost::asio::error::no_buffer_space);
			prv_throw_error(err);
			return;
		}

		precv_buf->resize(udp_packet_wrap_type::E_MAX_SINGLE_BUF_LENGTH);
		endpoint_ptr_type ped(new endpoint_type());

		psocket->async_receive_from(boost::asio::buffer(&((*precv_buf)[0]), udp_packet_wrap_type::E_MAX_SINGLE_BUF_LENGTH),
										*ped,
										boost::bind(&this_type::handler_recv, this,
														boost::asio::placeholders::error,
														boost::asio::placeholders::bytes_transferred,
														precv_buf, ped, psocket)
									);
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
			prv_throw_error(e);
			return;
		}

		wrap_obj_ptr_type pwobj = _collecter.collect(*pbuf);
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
			accept_complete(ped, rd);
			return;
		}

		recv_complete(ped, rd);
	}

	void accept_complete(endpoint_ptr_type ped, recv_data_type& data)
	{
		assert(data.owner_id() == 0);

		socket_ptr_type psocket(_udp_socket_pool.get(_service_pool.get_service()));
		assert(psocket);
		if(!psocket)
		{
			prv_throw_error(error_maker_type::make_error(error_maker_type::E_invalid_access));
			return;
		}

		recv(psocket);
		prv_insert_to_session(psocket, ped, data);
	}

	void recv_complete(endpoint_ptr_type ped, recv_data_type& data)
	{
		assert(data.owner_id() != 0);

		session_mgr_ptr_type psmgr = _session_mgr_wrap.get_shared_ptr();
		if(!psmgr)
		{
			return;
		}

		psmgr->send_to_adapter(data);
	}

	void prv_insert_to_session(socket_ptr_type psocket, endpoint_ptr_type ped, recv_data_type& data)
	{
		if(!psocket)
		{
			prv_throw_error(error_maker_type::make_error(error_maker_type::E_invalid_access));
			return;
		}

		udp_conn_checker_type checker;
		if(!( data.load(checker) && checker.check()))
		{
			prv_throw_error(error_maker_type::make_error(error_maker_type::E_invalid_access));
			return;
		}

		conn_ptr_type pconn(new conn_type(*psocket));
		if(!pconn)
		{
			prv_throw_error(error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc));
			return;
		}

		pconn->connect(*ped);

		session_mgr_ptr_type psmgr = _session_mgr_wrap.get_shared_ptr();
		if(!psmgr)
		{
			prv_throw_error(error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc));
			return;
		}

		psmgr->insert(pconn);
	}

	template<typename Error>
	void prv_throw_error(const Error& e) const
	{
		exception::exception::throw_error(e);
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
	acceptor_ptr_type _pacceptor;
	session_mgr_wrap_type& _session_mgr_wrap;
	udp_socket_pool_type _udp_socket_pool;
	udp_packets_collecter_type _collecter;
	
	static recv_buf_queue_type _s_recv_buf_queue;
};

template<typename ServicePool, typename Acceptor, typename SessionMgrWrap, typename Error_Maker>
    typename udp_async_acceptor_handler<ServicePool, Acceptor, SessionMgrWrap, Error_Maker>::recv_buf_queue_type
        udp_async_acceptor_handler<ServicePool, Acceptor, SessionMgrWrap, Error_Maker>::_s_recv_buf_queue;

} // namespace session_helper
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SESSION_HELPER_UDP_ASYNC_ACCEPTOR_HANDLER_HPP__