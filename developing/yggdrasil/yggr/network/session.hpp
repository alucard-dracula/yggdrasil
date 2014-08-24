//session.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HPP__
#define __YGGR_NETWORK_SESSION_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/exception.hpp>
#include <yggr/base/exception_sort.hpp>
#include <yggr/base/ctrl_center.hpp>
#include <yggr/base/system_code.hpp>
#include <yggr/safe_container/safe_queue.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/base/error_make.hpp>
#include <yggr/network/type_traits/tags.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/friend.hpp>

namespace yggr
{
namespace network
{

template <typename Tag,
			typename Conn,
			typename Adapter_Mgr,
			typename PacketsChecker,
			typename PacketsCrypher,
			typename First_Action,
			typename State_Fixer,
			typename Mutex = boost::mutex>
class session
	: public boost::enable_shared_from_this
				<
					session
					<
						Tag,
						Conn,
						Adapter_Mgr,
						PacketsChecker,
						PacketsCrypher,
						First_Action,
						State_Fixer,
						Mutex
					>
				>,
		private nonable::noncopyable
{

private:
	typedef Mutex mutex_type;

public:
	typedef Tag tag_type;
	typedef Conn conn_type;
	typedef Adapter_Mgr adapter_mgr_type;
	typedef PacketsChecker packets_checker_type;
	typedef PacketsCrypher packets_crypher_type;

	typedef typename conn_type::service_type service_type;
	typedef typename conn_type::socket_type socket_type;
	typedef typename conn_type::link_type link_type;
	typedef typename conn_type::link_init_type link_init_type;
	typedef typename conn_type::link_address_type link_address_type;
	typedef typename conn_type::send_data_type send_data_type;
	typedef typename conn_type::recv_data_type recv_data_type;
	typedef typename conn_type::data_buf_type data_buf_type;
	typedef typename conn_type::endpoint_type endpoint_type;
	typedef typename conn_type::protocol_type protocol_type;
	typedef typename conn_type::protocol_tag_type protocol_tag_type;

private:
	typedef First_Action first_action_type;
	YGGR_PP_FRIEND_TYPENAME(first_action_type);

	typedef State_Fixer state_fixer_type;
	YGGR_PP_FRIEND_TYPENAME(state_fixer_type);
	YGGR_PP_FRIEND_DEDUCED_TYPENAME(state_fixer_type::fixer_type);

public:
	typedef typename state_fixer_type::def_type state_def_type;

private:
	typedef safe_container::safe_wrap<conn_type, mutex_type> conn_wrap_type;
	typedef typename conn_wrap_type::error_type conn_wrap_error_type;

	typedef safe_container::safe_wrap<endpoint_type, mutex_type> endpoint_wrap_type;
	typedef typename endpoint_wrap_type::value_ptr_type endpoint_ptr_type;
	typedef typename endpoint_wrap_type::error_type endpoint_wrap_error_type;

public:
	typedef typename conn_wrap_type::value_ptr_type conn_ptr_type;

	typedef typename adapter_mgr_type::task_type task_type; // udp_curer support
	typedef typename adapter_mgr_type::send_std_data_type adapter_send_std_data_type; //ipak
	typedef typename adapter_mgr_type::recv_rst_data_type adapter_recv_rst_data_type; // opak
	typedef typename adapter_mgr_type::src_type task_center_type;

public:
	typedef safe_container::safe_wrap<adapter_mgr_type, mutex_type> adapter_mgr_wrap_type;
	typedef typename adapter_mgr_wrap_type::value_ptr_type adapter_mgr_ptr_type;
	typedef typename adapter_mgr_wrap_type::error_type adapter_mgr_wrap_error_type;

private:

	typedef typename packets_checker_type::error_type ex_error_type;

	typedef safe_container::safe_wrap<packets_checker_type, mutex_type> packets_checker_wrap_type;
	typedef typename packets_checker_wrap_type::value_ptr_type packets_checker_ptr_type;
	typedef typename packets_checker_wrap_type::error_type packets_checker_error_type;

	typedef safe_container::safe_wrap<packets_crypher_type, mutex_type> packets_crypher_wrap_type;
	typedef typename packets_crypher_wrap_type::value_ptr_type packets_crypher_ptr_type;
	typedef typename packets_crypher_wrap_type::error_type packets_crypher_wrap_error_type;

	typedef safe_container::safe_wrap<state_fixer_type, mutex_type> state_fixer_wrap_type;
	typedef typename state_fixer_wrap_type::value_ptr_type state_fixer_ptr_type;
	typedef typename state_fixer_wrap_type::error_type state_fixer_wrap_error_type;

public:
	typedef typename packets_checker_type::id_type id_type;
	typedef typename packets_checker_type::owner_id_parser_type owner_id_parser_type;

private:

	typedef safe_container::safe_wrap<id_type, mutex_type> id_wrap_type;
	typedef typename id_wrap_type::value_ptr_type id_ptr_type;
	typedef typename id_wrap_type::error_type id_wrap_error_type;

	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("session")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_10)
			ERROR_CODE_DEF(E_invalid_conn_alloc)
			ERROR_CODE_DEF(E_adapter_is_empty)
			ERROR_CODE_DEF(E_need_remove_self)
			ERROR_CODE_DEF(E_need_reconnect_self) // use to client only
			ERROR_CODE_KEY_VAL_DEF(E_splice_line, E_need_remove_self + 10)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_10)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_conn_alloc, "invalid conn alloc")
			ERROR_CODE_MSG(E_adapter_is_empty, "adapter is empty")
			ERROR_CODE_MSG(E_need_remove_self, "need remove self")
			ERROR_CODE_MSG(E_need_reconnect_self, "need reconnect self")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------

	BOOST_MPL_ASSERT((typename boost::mpl::if_
						<
							typename boost::is_same
							<
								tag_type,
								yggr::network::type_traits::tag_server
							>::type,
							boost::mpl::true_,
							typename boost::is_same
							<
								tag_type,
								yggr::network::type_traits::tag_client
							>::type
						>));

private:
	template<typename _Tag, typename Nil_T = int>
	struct close_code;

	template<typename Nil_T>
	struct close_code<yggr::network::type_traits::tag_server, Nil_T>
	{ enum { value = error_maker_type::E_need_remove_self }; };

	template<typename Nil_T>
	struct close_code<yggr::network::type_traits::tag_client, Nil_T>
	{ enum { value = error_maker_type::E_need_reconnect_self }; };

public:
	typedef close_code<tag_type> close_code_type;

private:
	typedef session this_type;

public:
	session(const id_type& id,
				const conn_ptr_type& pconn,
				const adapter_mgr_ptr_type& adp_mgr_ptr,
				const packets_checker_ptr_type& pchecker,
				const packets_crypher_ptr_type& pcrypher,
				const endpoint_type& local_endpoint,
				const endpoint_type& remote_endpoint)
		: _id_wrap(id),
			_conn_wrap(pconn),
			_adp_mgr_wrap(adp_mgr_ptr),
			_packets_checker_wrap(pchecker),
			_packets_crypher_wrap(pcrypher),
			_local_endpoint_wrap(local_endpoint),
			_remote_endpoint_wrap(remote_endpoint),
			_state_fixer_wrap(state_fixer_ptr_type(new state_fixer_type()))
	{
		assert(id);
		assert(!_conn_wrap.empty());
		if(_conn_wrap.empty())
		{
			prv_throw_error(error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc));
		}
	}

	~session(void)
	{
		std::cout << "~session" << std::endl;
	}

	static bool is_v4(void)
	{
		return conn_type::is_v4();
	}

	static bool is_v6(void)
	{
		return conn_type::is_v6();
	}

	id_type get_id(void) const
	{
		try
		{
			return _id_wrap.get_value();
		}
		catch(const id_wrap_error_type& e)
		{
			prv_throw_error(e);
		}
		return id_type();
	}

	bool operator==(const id_type& id) const
	{
		try
		{
			return id == _id_wrap.get_value();
		}
		catch(const id_wrap_error_type& e)
		{
			prv_throw_error(e);
		}

		return false;
	}

	bool operator!=(const id_type& id) const
	{
		return !this_type::operator==(id);
	}

	bool is_open(void) const
	{
		return prv_is_open();
	}

	bool is_connected(void) const
	{
		return prv_is_connected();
	}

	void close(void)
	{
		stop_recv();

		//conn_ptr_type ptr = _conn_wrap.get_shared_ptr();

		//if(ptr)
		//{
		//	ptr->close();
		//}
		// replace swap ok?
		conn_ptr_type ptr;
		_conn_wrap.swap(ptr);

		if(ptr)
		{
			ptr->close();
		}

		adapter_mgr_ptr_type padp = _adp_mgr_wrap.get_shared_ptr();
		if(!padp)
		{
			return;
		}

		try
		{
			padp->template drop_task<typename task_center_type::rst_mark_type>(_id_wrap.get_value());
		}
		catch(const id_wrap_error_type&)
		{
			return;
		}
	}

	void start(void)
	{
		if(!prv_is_connected())
		{
			handler_close_level_error();
			return;
		}

		if(!hold_recv())
		{
			handler_close_level_error();
			return;
		}

		first_action_type first_action;
		first_action(*this);
	}

	bool hold_recv(void)
	{
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		assert(ptr);
		if(!ptr)
		{
			return false;
		}

		try
		{
			return ptr->hold_recv(_id_wrap.get_value(),
									boost::bind(&this_type::handler_adapter_mgr_recv, this->shared_from_this(), _1));
		}
		catch(const id_wrap_error_type&)
		{
			return false;
		}
	}

	bool stop_recv(void)
	{
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		assert(ptr);
		if(!ptr)
		{
			return false;
		}

		try
		{
			return ptr->stop_recv(_id_wrap.get_value());
		}
		catch(const id_wrap_error_type&)
		{
			return false;
		}
	}

	conn_ptr_type get_conn_ptr(void) const
	{
		return _conn_wrap.get_shared_ptr();
	}

	endpoint_type local_endpoint(void) const
	{
		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();
		return ptr? *ptr : endpoint_type();
	}

	link_address_type local_address(void) const
	{
		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->address() : link_address_type();
	}

	yggr::u16 local_port(void) const
	{
		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->port() : 0;
	}

	template<typename Socket_Info>
	Socket_Info local_socket_info(void) const
	{
		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();
		return ptr? Socket_Info(ptr->address(), ptr->port()) : Socket_Info();
	}

	std::string str_local_address(void) const
	{
		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->address().to_string() : std::string();
	}

	std::string str_local_port(void) const
	{
		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();
		if(!ptr) return std::string();
		std::stringstream ss;
		ss << ptr->port();
		return ss.str();
	}

	std::string str_local_socket_info(void) const
	{
		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();
		if(!ptr) return std::string();
		std::stringstream ss;
		ss << ptr->address() << ":" << ptr->port();
		return ss.str();
	}

	endpoint_type remote_endpoint(void) const
	{
		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		return ptr? *ptr : endpoint_type();
	}

	link_address_type remote_address(void) const
	{
		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->address() : link_address_type();
	}

	yggr::u16 remote_port(void) const
	{
		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->port() : 0;
	}

	template<typename Socket_Info>
	Socket_Info remote_socket_info(void) const
	{
		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		return ptr? Socket_Info(ptr->address(), ptr->port()) : Socket_Info();
	}

	std::string str_remote_address(void) const
	{
		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->address().to_string() : std::string();
	}

	std::string str_remote_port(void) const
	{
		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		if(!ptr) return std::string();
		std::stringstream ss;
		ss << ptr->port();
		return ss.str();
	}

	std::string str_remote_socket_info(void) const
	{
		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		if(!ptr) return std::string();
		std::stringstream ss;
		ss << ptr->address() << ":" << ptr->port();
		return ss.str();
	}

	template<typename NetInfo, typename Data, typename Handler>
	void send_packet(const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef boost::tuple<handler_type> tuple_handler_type;
		if(!prv_is_connected())
		{
			handler_close_level_error();
			return;
		}

		send_data_type pak(netinfo, data);
		if(!prv_fix_send_opacket(pak))
		{
			return;
		}

		prv_send_packet_helper(pak,
								boost::bind(&this_type::handler_send_of_handler<tuple_handler_type>,
												this_type::shared_from_this(),
												boost::asio::placeholders::error,
												boost::make_tuple(handler)));
	}

	template<typename Handler>
	void send_packet(send_data_type& pak, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef boost::tuple<handler_type> tuple_handler_type;
		if(!prv_is_connected())
		{
			handler_close_level_error();
			return;
		}


		if(!prv_fix_send_opacket(pak))
		{
			return;
		}

		prv_send_packet_helper(pak,
								boost::bind(&this_type::handler_send_of_handler<tuple_handler_type>,
												this_type::shared_from_this(),
												boost::asio::placeholders::error,
												boost::make_tuple(handler)));
	}

	void send_packet(send_data_type& pak)
	{
		if(!prv_is_connected())
		{
			handler_close_level_error();
			return;
		}


		if(!prv_fix_send_opacket(pak))
		{
			return;
		}

		prv_send_packet_helper(pak,
								boost::bind(&this_type::handler_send,
												this_type::shared_from_this(),
												boost::asio::placeholders::error));
	}

	adapter_mgr_ptr_type get_adapter_mgr_ptr(void) const
	{
		return _adp_mgr_wrap.get_shared_ptr();
	}

	void send_to_adapter(recv_data_type& pak)
	{
		adapter_mgr_ptr_type padp = _adp_mgr_wrap.get_shared_ptr();
		if(!padp)
		{
			prv_throw_error(error_maker_type::make_error(error_maker_type::E_adapter_is_empty),
								(u32)system_controller::system_code::E_SESSION_SYSTEM,
								this_type::get_id());
			handler_close_level_error();
			return;
		}


		if(!prv_fix_recv_ipacket(pak))
		{
			return;
		}

		state_fixer_ptr_type pfixer = _state_fixer_wrap.get_shared_ptr();
		if(!pfixer)
		{
			handler_close_level_error();
			return;
		}

		if(!pfixer->fix_packet(pak, this->shared_from_this()))
		{
			return;
		}

		padp->send(pak);
	}

	void check_state(void)
	{
		state_fixer_ptr_type pfixer = _state_fixer_wrap.get_shared_ptr();
		if(!pfixer)
		{
			handler_close_level_error();
			return;
		}

		pfixer->check(this->shared_from_this());
	}

	template<typename Handler>
	void check_state(const Handler& handler)
	{
		state_fixer_ptr_type pfixer = _state_fixer_wrap.get_shared_ptr();
		if(!pfixer)
		{
			handler_close_level_error();
			return;
		}

		pfixer->check(this->shared_from_this(), handler);
	}

private:

	template<typename Handler>
	void prv_send_packet_helper(send_data_type& pak, const Handler& handler)
	{
		typedef Handler handler_type;
		conn_ptr_type ptr = _conn_wrap.get_shared_ptr();
		if(!ptr)
		{
			handler_close_level_error();
			return;
		}

		ptr->send_packet(pak, handler);
	}

	template<typename Handler>
	void prv_recv_packet_helper(const Handler& handler)
	{
		typedef Handler handler_type;

		conn_ptr_type ptr = _conn_wrap.get_shared_ptr();
		if(!ptr)
		{
			handler_close_level_error();
			return;
		}

		ptr->template recv_packet<recv_data_type>(handler);
	}

	bool prv_fix_send_opacket(send_data_type& pak) const
	{
		id_type now_id = id_type();
		try
		{
			now_id = _id_wrap.get_value();
		}
		catch(const id_wrap_error_type&)
		{
			return false;
		}

		assert(now_id);

		packets_checker_ptr_type chk_ptr = _packets_checker_wrap.get_shared_ptr();
		if(!chk_ptr)
		{
			return false;
		}

		ex_error_type ex_err((*chk_ptr)(pak));

		if(ex_err)
		{
			prv_throw_error(ex_err);
			return false;
		}

		packets_crypher_ptr_type cry_ptr = _packets_crypher_wrap.get_shared_ptr();

		if(!cry_ptr)
		{
			return false;
		}

		pak.update_time();
		return cry_ptr->template encrypt<send_data_type>(pak);
	}

	bool prv_fix_recv_ipacket(recv_data_type& pak) const
	{
		id_type now_id = id_type();
		try
		{
			now_id = _id_wrap.get_value();
		}
		catch(const id_wrap_error_type&)
		{
			return false;
		}

		assert(now_id);

		//cry_ptr->decrypt(pak.net_info()); //don't need this

		packets_checker_ptr_type chk_ptr = _packets_checker_wrap.get_shared_ptr();

		if(!chk_ptr)
		{
			return false;
		}

		ex_error_type ex_err((*chk_ptr)(pak));
		//assert(!ex_err); // gcc debug

		if(ex_err)
		{
			prv_throw_error(ex_err);
			handler_close_level_error();
			return false;
		}

		packets_crypher_ptr_type cry_ptr = _packets_crypher_wrap.get_shared_ptr();

		if(!cry_ptr)
		{
			return false;
		}

		if(cry_ptr->decrypt(pak))
		{
			pak.restore_info();
			return true;
		}
		return false;
	}

	bool prv_is_open(void) const
	{
		conn_ptr_type ptr = _conn_wrap.get_shared_ptr();
		return ptr? ptr->is_open() : false;
	}

	bool prv_is_connected(void) const
	{
		return prv_is_open() && this_type::get_id();
	}

	void handler_close_level_error(void) const
	{
		prv_throw_error(error_maker_type::make_error(close_code_type::value),
							(u32)system_controller::system_code::E_SESSION_SYSTEM,
							get_id());
	}

	template<typename Err, typename Param>
	void prv_throw_error(const Err& err, u32 sys_code, const Param& param) const
	{
		exception::exception::throw_error(err, sys_code, param);
	}

	template<typename Err>
	void prv_throw_error(const Err& err) const
	{
		exception::exception::throw_error(err);
	}

private:

	template<typename Handler>
	void handler_send_of_handler(const boost::system::error_code& e, const Handler& handler)
	{
		this_type::handler_send(e);
		boost::get<0>(handler)(e);

	}

	void handler_send(const boost::system::error_code& e)
	{
		if(!e)
		{
			conn_ptr_type ptr = _conn_wrap.get_shared_ptr();

			if(!ptr)
			{
				prv_throw_error(e);
				handler_close_level_error();
				return;
			}

			ptr->next_send();
			return;
		}

		prv_throw_error(e, (u32)system_controller::system_code::E_SESSION_SYSTEM, this_type::get_id());
	}

	void handler_recv(const boost::system::error_code& e, recv_data_type& pak)
	{
		if(!e)
		{
			this_type::send_to_adapter(pak);

			prv_recv_packet_helper(
					boost::make_tuple(
							boost::bind(
											&this_type::handler_recv,
											this_type::shared_from_this(), _1, _2
										),
							boost::bind(
											&this_type::handler_close_level_error,
											this_type::shared_from_this()
										)
									)
								);
			return;
		}

		prv_throw_error(e, (u32)system_controller::system_code::E_SESSION_SYSTEM, this_type::get_id());
	}

	void handler_adapter_mgr_recv(adapter_recv_rst_data_type& pak)
	{
		this_type::send_packet(pak);
	}

private:
	id_wrap_type _id_wrap;
	conn_wrap_type _conn_wrap;
	adapter_mgr_wrap_type _adp_mgr_wrap;
	packets_checker_wrap_type _packets_checker_wrap;
	packets_crypher_wrap_type _packets_crypher_wrap;
	endpoint_wrap_type _remote_endpoint_wrap;
	endpoint_wrap_type _local_endpoint_wrap;
	state_fixer_wrap_type _state_fixer_wrap;
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_SESSION_HPP__
