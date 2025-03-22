//session.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HPP__
#define __YGGR_NETWORK_SESSION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/base/error_make.hpp>
#include <yggr/bind/bind.hpp>

#include <yggr/nonable/noncopyable.hpp>

#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/system_controller/system_code.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/tuple/tuple.hpp>

#include <yggr/utility/args_holder_nv.hpp>

#include <yggr/exception/exception.hpp>
#include <yggr/exception/exception_sort.hpp>

#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/address_converter.hpp>
#include <yggr/network/address_helper.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/friend.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <boost/asio.hpp>
#include <boost/ref.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sstream>

namespace yggr
{
namespace network
{
namespace detail
{

template<typename ErrorMaker, typename _Tag >
struct session_close_code;

template<typename ErrorMaker>
struct session_close_code<ErrorMaker, yggr::network::type_traits::tag_server>
	: public boost::mpl::integral_c<yggr::u32, ErrorMaker::E_need_remove_self>
{
};

template<typename ErrorMaker>
struct session_close_code<ErrorMaker, yggr::network::type_traits::tag_client>
	: public boost::mpl::integral_c<yggr::u32, ErrorMaker::E_need_reconnect_self>
{
};

} // namespace detail
} // namespace network
} // namespace yggr

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
			typename Mutex = void>
class session
	: public 
		boost::enable_shared_from_this
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
	typedef Mutex tpl_arg_mutex_type;

public:
	typedef Tag tag_type;
	typedef Conn conn_type;
	typedef Adapter_Mgr adapter_mgr_type;
	typedef PacketsChecker packets_checker_type;
	typedef PacketsCrypher packets_crypher_type;

	typedef typename conn_type::protocol_tag_type protocol_tag_type;
	typedef typename conn_type::service_type service_type;
	typedef typename conn_type::socket_type socket_type;
	typedef typename conn_type::endpoint_type endpoint_type;
	typedef typename conn_type::protocol_type protocol_type;
	typedef typename conn_type::deadline_timer_type deadline_timer_type;
	
	typedef typename conn_type::link_address_type link_address_type;

	typedef typename conn_type::link_type link_type;
	typedef typename conn_type::link_init_type link_init_type;
	
	typedef typename conn_type::data_buf_type data_buf_type;
	typedef typename conn_type::send_data_type send_data_type;
	typedef typename conn_type::recv_data_type recv_data_type;
	

private:
	typedef First_Action first_action_type;
	YGGR_PP_FRIEND_TYPENAME(first_action_type);

	typedef State_Fixer state_fixer_type;
	YGGR_PP_FRIEND_TYPENAME(state_fixer_type);
	YGGR_PP_FRIEND_DEDUCED_TYPENAME(state_fixer_type::fixer_type);

public:
	typedef typename state_fixer_type::def_type state_def_type;

private:
	typedef safe_container::safe_wrap<conn_type, tpl_arg_mutex_type> conn_wrap_type;
	typedef typename conn_wrap_type::error_type conn_wrap_error_type;

	typedef safe_container::safe_wrap<endpoint_type, tpl_arg_mutex_type> endpoint_wrap_type;
	typedef typename endpoint_wrap_type::value_ptr_type endpoint_ptr_type;
	typedef typename endpoint_wrap_type::error_type endpoint_wrap_error_type;
	typedef address_helper address_helper_type;

public:
	typedef typename conn_wrap_type::value_ptr_type conn_ptr_type;

	typedef typename adapter_mgr_type::task_type task_type; // udp_curer support
	typedef typename adapter_mgr_type::send_std_data_type adapter_send_std_data_type; //ipak
	typedef typename adapter_mgr_type::recv_rst_data_type adapter_recv_rst_data_type; // opak
	typedef typename adapter_mgr_type::src_type task_center_type;

public:
	typedef safe_container::safe_wrap<adapter_mgr_type, tpl_arg_mutex_type> adapter_mgr_wrap_type;
	typedef typename adapter_mgr_wrap_type::value_ptr_type adapter_mgr_ptr_type;
	typedef typename adapter_mgr_wrap_type::error_type adapter_mgr_wrap_error_type;

private:
	typedef typename packets_checker_type::error_type ex_error_type;

	typedef safe_container::safe_wrap<packets_checker_type, tpl_arg_mutex_type> packets_checker_wrap_type;
	typedef typename packets_checker_wrap_type::value_ptr_type packets_checker_ptr_type;
	typedef typename packets_checker_wrap_type::error_type packets_checker_error_type;

	typedef safe_container::safe_wrap<packets_crypher_type, tpl_arg_mutex_type> packets_crypher_wrap_type;
	typedef typename packets_crypher_wrap_type::value_ptr_type packets_crypher_ptr_type;
	typedef typename packets_crypher_wrap_type::error_type packets_crypher_wrap_error_type;

	typedef safe_container::safe_wrap<state_fixer_type, tpl_arg_mutex_type> state_fixer_wrap_type;
	typedef typename state_fixer_wrap_type::value_ptr_type state_fixer_ptr_type;
	typedef typename state_fixer_wrap_type::error_type state_fixer_wrap_error_type;

public:
	typedef typename packets_checker_type::id_type id_type;
	typedef typename packets_checker_type::owner_id_parser_type owner_id_parser_type;

private:
	typedef safe_container::safe_wrap<id_type, tpl_arg_mutex_type> id_wrap_type;
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

	BOOST_MPL_ASSERT((
		typename 
			boost::mpl::if_
			<
				typename 
					boost::is_same
					<
						tag_type,
						yggr::network::type_traits::tag_server
					>::type,
				boost::mpl::true_,
				typename 
					boost::is_same
					<
						tag_type,
						yggr::network::type_traits::tag_client
					>::type
			>));

public:
	typedef detail::session_close_code<error_maker_type, tag_type> close_code_type;

private:
	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

private:
	typedef session this_type;

public:
	session(const id_type& id,
				const conn_ptr_type& pconn,
				const adapter_mgr_ptr_type& adp_mgr_ptr,
				const packets_checker_ptr_type& pchecker,
				const packets_crypher_ptr_type& pcrypher,
				const endpoint_type& local_endpoint,
				const endpoint_type& localhost_endpoint,
				const endpoint_type& remote_endpoint)
		: _id_wrap(id),
			_conn_wrap(pconn),
			_adp_mgr_wrap(adp_mgr_ptr),
			_packets_checker_wrap(pchecker),
			_packets_crypher_wrap(pcrypher),
			_local_endpoint_wrap(local_endpoint),
			_localhost_endpoint_wrap(localhost_endpoint), 
			_remote_endpoint_wrap(remote_endpoint),
			_state_fixer_wrap(yggr_nothrow_new state_fixer_type())
	{
		assert(id);
		assert(!_conn_wrap.empty());
		if(_conn_wrap.empty())
		{
			this_type::prv_s_throw_error(error_maker_type::make_error(error_maker_type::E_invalid_conn_alloc));
		}
	}

	~session(void)
	{
#ifdef _DEBUG
		std::cout << "~session" << std::endl;
#endif // _DEBUG
	}

public:
	inline static bool is_v4(void)
	{
		return conn_type::is_v4();
	}

	inline static bool is_v6(void)
	{
		return conn_type::is_v6();
	}

public:
	id_type get_id(void) const
	{
		try
		{
			return _id_wrap.load();
		}
		catch(const id_wrap_error_type&)
		{
			return id_type();
		}
	}

	inline bool compare_eq(const id_type& id) const
	{
		return id == this_type::get_id();
	}

	template<typename OTag,
				typename OAdapter_Mgr,
				typename OPacketsChecker,
				typename OPacketsCrypher,
				typename OFirst_Action,
				typename OState_Fixer,
				typename OMutex> inline
	bool compare_eq(const
						session<
							OTag, 
							conn_type,
							OAdapter_Mgr,
							OPacketsChecker,
							OPacketsCrypher,
							OFirst_Action,
							OState_Fixer,
							OMutex >& right)
	{
		return this_type::get_id() == right.get_id();
	}

	bool compare_eq(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| (this_type::get_id() == right.get_id());
	}

public:
	inline bool is_open(void) const
	{
		return this_type::prv_is_open();
	}

	inline bool is_connected(void) const
	{
		return this_type::prv_is_connected();
	}

	void close(void)
	{
		this_type::stop_recv();
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

		id_type rm_id = _id_wrap.load();

		try
		{
			padp->template drop_task<typename task_center_type::rst_mark_type>(rm_id);
		}
		catch(const id_wrap_error_type&)
		{
			return;
		}
	}

	void start(void)
	{
		if(!this_type::prv_is_connected())
		{
			this_type::handler_close_level_error();
			return;
		}

		if(!this_type::hold_recv())
		{
			this_type::handler_close_level_error();
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
			return 
				ptr->hold_recv(
					_id_wrap.load(),
					boost::bind(&this_type::handler_adapter_mgr_recv, this->this_type::shared_from_this(), _1));
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
			return ptr->stop_recv(_id_wrap.load());
		}
		catch(const id_wrap_error_type&)
		{
			return false;
		}
	}

public:
	template<typename OptionSetter> inline
	bool set_option(const OptionSetter& ops_setter)
	{
		typedef OptionSetter ops_setter_type;
		return _conn_wrap.call_function(&conn_type::template set_option<ops_setter_type>, ops_setter);
	}

	template<typename OptionGetter> inline
	bool get_option(OptionGetter& ops_getter) const
	{
		typedef OptionGetter ops_getter_type;
		return _conn_wrap.call_function(&conn_type::template get_option<ops_getter_type>, ops_getter);
	}

public:
	template<typename Time, typename Handler> inline
	std::size_t expires_at_timer_wait(const Time& tm, const Handler& handler)
	{
		typedef Time time_type;
		typedef Handler handler_type;

		return
			_conn_wrap.call_function(
				&conn_type::template expires_at_timer_wait<time_type, handler_type>, 
				tm, handler);
	}

	template<typename Time, typename Handler> inline
	std::size_t expires_from_now_timer_wait(const Time& tm, const Handler& handler)
	{
		typedef Time time_type;
		typedef Handler handler_type;

		return
			_conn_wrap.call_function(
				&conn_type::template expires_from_now_timer_wait<time_type, handler_type>, 
				tm, handler);
	}

	inline std::size_t timer_cancel(void)
	{
		return _conn_wrap.call_function(&conn_type::timer_cancel);
	}

	inline std::size_t timer_finish(void)
	{
		return _conn_wrap.call_function(&conn_type::timer_finish);
	}

public:
	inline conn_ptr_type get_conn_ptr(void) const
	{
		return _conn_wrap.get_shared_ptr();
	}

public:
	inline endpoint_type local_endpoint(void) const
	{
		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();
		return ptr? *ptr : endpoint_type();
	}

	inline link_address_type local_address(void) const
	{
		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->address() : link_address_type();
	}

	inline yggr::u16 local_port(void) const
	{
		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->port() : 0;
	}

	template<typename Socket_Info> inline
	Socket_Info local_socket_info(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return Socket_Info();
		}

		addr_conv_type conv_addr(ptr->address());
		port_conv_type conv_port(ptr->port());

		return Socket_Info(conv_addr, conv_port);
	}

	inline ::yggr::string str_local_address(void) const
	{
		typedef ::yggr::string now_string_type;
		typedef address_converter<link_address_type> addr_conv_type;

		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return now_string_type();
		}

		addr_conv_type conv(ptr->address());
		return conv;
	}

	template<typename String> inline
	String str_local_address(void) const
	{
		typedef String now_string_type;
		typedef address_converter<link_address_type> addr_conv_type;

		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return now_string_type();
		}

		addr_conv_type conv(ptr->address());
		return conv;
	}

	inline ::yggr::string str_local_port(void) const
	{
		typedef ::yggr::string now_string_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();

		if(!ptr) 
		{
			return now_string_type();
		}

		port_conv_type conv(ptr->port());
		return conv;
	}

	template<typename String> inline
	String str_local_port(void) const
	{
		typedef String now_string_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();

		if(!ptr) 
		{
			return now_string_type();
		}

		port_conv_type conv(ptr->port());
		return conv;
	}

	inline ::yggr::string str_local_socket_info(void) const
	{
		typedef ::yggr::string now_string_type;
		typedef port_converter<endpoint_type> conv_type;

		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return now_string_type();
		}

		conv_type conv(*ptr);
		return conv;
	}

	template<typename String> inline
	String str_local_socket_info(void) const
	{
		typedef String now_string_type;
		typedef port_converter<endpoint_type> conv_type;

		endpoint_ptr_type ptr = _local_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return now_string_type();
		}

		conv_type conv(*ptr);
		return conv;
	}

	inline endpoint_type localhost_endpoint(void) const
	{
		endpoint_ptr_type ptr = _localhost_endpoint_wrap.get_shared_ptr();
		return ptr? *ptr : endpoint_type();
	}

	inline link_address_type localhost_address(void) const
	{
		endpoint_ptr_type ptr = _localhost_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->address() : link_address_type();
	}

	inline yggr::u16 localhost_port(void) const
	{
		endpoint_ptr_type ptr = _localhost_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->port() : 0;
	}

	template<typename Socket_Info> inline
	Socket_Info localhost_socket_info(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_ptr_type ptr = _localhost_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return Socket_Info();
		}

		addr_conv_type conv_addr(ptr->address());
		port_conv_type conv_port(ptr->port());

		return Socket_Info(conv_addr, conv_port);
	}

	inline ::yggr::string str_localhost_address(void) const
	{
		typedef ::yggr::string now_string_type;
		typedef address_converter<link_address_type> addr_conv_type;

		endpoint_ptr_type ptr = _localhost_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return now_string_type();
		}

		addr_conv_type conv(ptr->address());
		return conv;
	}

	template<typename String> inline
	String str_localhost_address(void) const
	{
		typedef String now_string_type;
		typedef address_converter<link_address_type> addr_conv_type;

		endpoint_ptr_type ptr = _localhost_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return now_string_type();
		}

		addr_conv_type conv(ptr->address());
		return conv;
	}

	inline ::yggr::string str_localhost_port(void) const
	{
		typedef ::yggr::string now_string_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_ptr_type ptr = _localhost_endpoint_wrap.get_shared_ptr();

		if(!ptr) 
		{
			return now_string_type();
		}

		port_conv_type conv(ptr->port());
		return conv;
	}

	template<typename String> inline
	String str_localhost_port(void) const
	{
		typedef ::yggr::string now_string_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_ptr_type ptr = _localhost_endpoint_wrap.get_shared_ptr();

		if(!ptr) 
		{
			return now_string_type();
		}

		port_conv_type conv(ptr->port());
		return conv;
	}

	inline ::yggr::string str_localhost_socket_info(void) const
	{
		typedef ::yggr::string now_string_type;
		typedef endpoint_converter<endpoint_type> conv_type;

		endpoint_ptr_type ptr = _localhost_endpoint_wrap.get_shared_ptr();
		if(!ptr)
		{
			return now_string_type();
		}

		conv_type conv(*ptr);
		return conv;
	}

	template<typename String> inline
	String str_localhost_socket_info(void) const
	{
		typedef String now_string_type;
		typedef endpoint_converter<endpoint_type> conv_type;

		endpoint_ptr_type ptr = _localhost_endpoint_wrap.get_shared_ptr();
		if(!ptr)
		{
			return now_string_type();
		}

		conv_type conv(*ptr);
		return conv;
	}

	inline endpoint_type remote_endpoint(void) const
	{
		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		return ptr? *ptr : endpoint_type();
	}

	inline link_address_type remote_address(void) const
	{
		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->address() : link_address_type();
	}

	inline yggr::u16 remote_port(void) const
	{
		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		return ptr? ptr->port() : 0;
	}

	template<typename Socket_Info> inline
	Socket_Info remote_socket_info(void) const
	{
		typedef address_converter<link_address_type> addr_conv_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return Socket_Info();
		}

		addr_conv_type conv_addr(ptr->address());
		port_conv_type conv_port(ptr->port());

		return Socket_Info(conv_addr, conv_port);
	}

	inline ::yggr::string str_remote_address(void) const
	{
		typedef ::yggr::string now_string_type;
		typedef address_converter<link_address_type> addr_conv_type;

		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return now_string_type();
		}

		addr_conv_type conv(ptr->address());
		return conv;
	}

	template<typename String> inline
	String str_remote_address(void) const
	{
		typedef String now_string_type;
		typedef address_converter<link_address_type> addr_conv_type;

		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return now_string_type();
		}

		addr_conv_type conv(ptr->address());
		return conv;
	}

	inline ::yggr::string str_remote_port(void) const
	{
		typedef ::yggr::string now_string_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return now_string_type();
		}

		port_conv_type conv(ptr->port());
		return conv;
	}

	template<typename String> inline
	String str_remote_port(void) const
	{
		typedef String now_string_type;
		typedef port_converter<u16> port_conv_type;

		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();

		if(!ptr)
		{
			return now_string_type();
		}

		port_conv_type conv(ptr->port());
		return conv;
	}

	inline ::yggr::string str_remote_socket_info(void) const
	{
		typedef ::yggr::string now_string_type;
		typedef endpoint_converter<endpoint_type> conv_type;

		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		if(!ptr)
		{
			return now_string_type();
		}

		conv_type conv(*ptr);
		return conv;
	}

	template<typename String> inline
	String str_remote_socket_info(void) const
	{
		typedef String now_string_type;
		typedef endpoint_converter<endpoint_type> conv_type;

		endpoint_ptr_type ptr = _remote_endpoint_wrap.get_shared_ptr();
		if(!ptr)
		{
			return now_string_type();
		}

		conv_type conv(*ptr);
		return conv;
	}

	template<typename Address, typename Port>
	bool update_remote_endpoint(const Address& addr, const Port& port)
	{
		address_helper_type::address_type now_addr(address_helper_type::address_from_string(addr));
		u16 now_port = 0;
		{
			std::stringstream ss;
			ss << port;
			ss >> now_port;
		}

		endpoint_type ep;
		ep.address(now_addr);
		ep.port(now_port);

		return this_type::update_remote_endpoint(ep);
	}

	inline bool update_remote_endpoint(const endpoint_type& ep)
	{
		return 
			_conn_wrap.using_handler(
				boost::bind(
					&this_type::handler_update_remote_endpoint, 
					this, _1, boost::cref(ep)));
	}

	template<typename NetInfo, typename Data> inline
	void send_packet(const NetInfo& netinfo, const Data& data)
	{
		if(!this_type::prv_is_connected())
		{
			this_type::handler_close_level_error();
			return;
		}

		send_data_type pak(netinfo, data);
		if(!this_type::prv_fix_send_opacket(pak))
		{
			return;
		}

		this_type::prv_send_packet_helper(
			pak,
			boost::bind(
				&this_type::handler_send,
				this_type::shared_from_this(),
				boost::asio::placeholders::error));
	}

	template<typename NetInfo, typename Data, typename Handler> inline
	void send_packet(const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv<handler_type> handler_holder_type;

		if(!this_type::prv_is_connected())
		{
			this_type::handler_close_level_error();
			return;
		}

		send_data_type pak(netinfo, data);
		if(!this_type::prv_fix_send_opacket(pak))
		{
			return;
		}

		handler_holder_type any_handler(handler);

		this_type::prv_send_packet_helper(
			pak,
			boost::bind(
				&this_type::template handler_send_of_handler<handler_type>,
				this_type::shared_from_this(),
				boost::asio::placeholders::error,
				any_handler));
	}

	template<typename Handler> inline
	void send_packet(send_data_type& pak, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv<handler_type> handler_holder_type;

		if(!this_type::prv_is_connected())
		{
			this_type::handler_close_level_error();
			return;
		}

		if(!this_type::prv_fix_send_opacket(pak))
		{
			return;
		}

		handler_holder_type any_handler(handler);

		this_type::prv_send_packet_helper(
			pak,
			boost::bind(
				&this_type::template handler_send_of_handler<handler_type>,
				this_type::shared_from_this(),
				boost::asio::placeholders::error,
				any_handler));
	}

	inline void send_packet(send_data_type& pak)
	{
		if(!this_type::prv_is_connected())
		{
			this_type::handler_close_level_error();
			return;
		}


		if(!this_type::prv_fix_send_opacket(pak))
		{
			return;
		}

		this_type::prv_send_packet_helper(
			pak,
			boost::bind(
				&this_type::handler_send,
				this_type::shared_from_this(),
				boost::asio::placeholders::error));
	}

	inline adapter_mgr_ptr_type get_adapter_mgr_ptr(void) const
	{
		return _adp_mgr_wrap.get_shared_ptr();
	}

	void send_to_adapter(recv_data_type& pak)
	{
		adapter_mgr_ptr_type padp = _adp_mgr_wrap.get_shared_ptr();
		if(!padp)
		{
			this_type::prv_s_throw_error(
				error_maker_type::make_error(error_maker_type::E_adapter_is_empty),
				static_cast<u32>(system_controller::system_code::E_SESSION_SYSTEM),
				this_type::get_id());
			this_type::handler_close_level_error();
			return;
		}


		if(!this_type::prv_fix_recv_ipacket(pak))
		{
			return;
		}

		state_fixer_ptr_type pfixer = _state_fixer_wrap.get_shared_ptr();
		if(!pfixer)
		{
			this_type::handler_close_level_error();
			return;
		}

		if(!pfixer->fix_packet(pak, this->this_type::shared_from_this()))
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
			this_type::handler_close_level_error();
			return;
		}

		pfixer->check(this->this_type::shared_from_this());
	}

	template<typename Handler> inline
	void check_state(const Handler& handler)
	{
		state_fixer_ptr_type pfixer = _state_fixer_wrap.get_shared_ptr();
		if(!pfixer)
		{
			this_type::handler_close_level_error();
			return;
		}

		pfixer->check(this->this_type::shared_from_this(), handler);
	}

private:
	template<typename Handler> inline
	void prv_send_packet_helper(send_data_type& pak, const Handler& handler)
	{
		typedef Handler handler_type;
		conn_ptr_type ptr = _conn_wrap.get_shared_ptr();
		if(!ptr)
		{
			this_type::handler_close_level_error();
		}
		else
		{
			ptr->send_packet(pak, handler);
		}
	}

	template<typename Handler> inline
	void prv_recv_packet_helper(const Handler& handler)
	{
		typedef Handler handler_type;

		conn_ptr_type ptr = _conn_wrap.get_shared_ptr();
		if(!ptr)
		{
			this_type::handler_close_level_error();
		}
		else
		{
			ptr->template recv_packet<recv_data_type>(handler);
		}
	}

	bool prv_fix_send_opacket(send_data_type& pak) const
	{
		id_type now_id = id_type();
		try
		{
			now_id = _id_wrap.load();
		}
		catch(const id_wrap_error_type&)
		{
			return false;
		}

		assert(now_id);

		packets_checker_ptr_type chk_ptr = _packets_checker_wrap.get_shared_ptr();
		packets_crypher_ptr_type cry_ptr = _packets_crypher_wrap.get_shared_ptr();
		if(!(chk_ptr && cry_ptr))
		{
			return false;
		}

		pak.update_time();
		ex_error_type ex_err((*chk_ptr)(pak));

		if(ex_err)
		{
			this_type::prv_s_throw_error(ex_err);
			return false;
		}

		return cry_ptr->template encrypt<send_data_type>(pak);
	}

	bool prv_fix_recv_ipacket(recv_data_type& pak) const
	{
		id_type now_id = id_type();
		try
		{
			now_id = _id_wrap.load();
		}
		catch(const id_wrap_error_type&)
		{
			return false;
		}

		assert(now_id);

		packets_checker_ptr_type chk_ptr = _packets_checker_wrap.get_shared_ptr();
		packets_crypher_ptr_type cry_ptr = _packets_crypher_wrap.get_shared_ptr();

		if(!(chk_ptr && cry_ptr))
		{
			return false;
		}

		ex_error_type ex_err((*chk_ptr)(pak));

		if(ex_err)
		{
			this_type::prv_s_throw_error(ex_err);
			this_type::handler_close_level_error();
			return false;
		}

		if(cry_ptr->decrypt(pak))
		{
			pak.restore_info();
			return true;
		}
		return false;
	}

	inline bool prv_is_open(void) const
	{
		conn_ptr_type ptr = _conn_wrap.get_shared_ptr();
		return ptr && ptr->is_open();
	}

	inline bool prv_is_connected(void) const
	{
		return this_type::prv_is_open() && this_type::get_id();
	}

	inline void handler_close_level_error(void) const
	{
		this_type::prv_s_throw_error(
			error_maker_type::make_error(close_code_type::value),
			static_cast<u32>(system_controller::system_code::E_SESSION_SYSTEM),
			this_type::get_id());
	}

	template<typename Err, typename Param> inline
	static void prv_s_throw_error(const Err& err, u32 sys_code, const Param& param)
	{
		exception::exception::throw_error(err, sys_code, param);
	}

	template<typename Err> inline
	static void prv_s_throw_error(const Err& err)
	{
		exception::exception::throw_error(err);
	}

private:

	template<typename Handler> inline
	void handler_send_of_handler(const boost::system::error_code& e, const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;

		this_type::handler_send(e);

		const handler_type* phandler = utility::args_holder_static_cast<handler_type>(any_handler);
		assert(phandler);
		
		if(phandler)
		{
			(*phandler)(e);
		}
	}

	void handler_send(const boost::system::error_code& e)
	{
		if(!e)
		{
			conn_ptr_type ptr = _conn_wrap.get_shared_ptr();

			if(!ptr)
			{
				this_type::prv_s_throw_error(e);
				this_type::handler_close_level_error();
				return;
			}

			ptr->next_send();
			return;
		}

		this_type::prv_s_throw_error(
			e, 
			static_cast<u32>(system_controller::system_code::E_SESSION_SYSTEM), 
			this_type::get_id());
	}

	void handler_recv(const boost::system::error_code& e, recv_data_type& pak)
	{
		if(!e)
		{
			this_type::send_to_adapter(pak);

			this_type::prv_recv_packet_helper(
				::yggr::make_tuple(
					boost::bind(
						&this_type::handler_recv,
						this_type::shared_from_this(), _1, _2 ),
					boost::bind(
						&this_type::handler_close_level_error,
						this_type::shared_from_this() ) ) );
			return;
		}

		this_type::prv_s_throw_error(
			e, 
			static_cast<u32>(system_controller::system_code::E_SESSION_SYSTEM), 
			this_type::get_id());
	}

	inline void handler_adapter_mgr_recv(adapter_recv_rst_data_type& pak)
	{
		this_type::send_packet(pak);
	}

	inline bool handler_update_remote_endpoint(typename conn_wrap_type::base_type& pconn, const endpoint_type& ep)
	{
		return 
			pconn 
			&& pconn->update_remote_endpoint(ep)?
				(_remote_endpoint_wrap = ep, true) 
				: false;
	}

private:
	id_wrap_type _id_wrap;
	conn_wrap_type _conn_wrap;
	adapter_mgr_wrap_type _adp_mgr_wrap;
	packets_checker_wrap_type _packets_checker_wrap;
	packets_crypher_wrap_type _packets_crypher_wrap;
	endpoint_wrap_type _local_endpoint_wrap;
	endpoint_wrap_type _localhost_endpoint_wrap;
	endpoint_wrap_type _remote_endpoint_wrap;
	state_fixer_wrap_type _state_fixer_wrap;
};


// non-member_function

// operator==
template <typename Tag,
			typename Conn,
			typename Adapter_Mgr,
			typename PacketsChecker,
			typename PacketsCrypher,
			typename First_Action,
			typename State_Fixer,
			typename Mutex> inline
bool operator==(const 
					typename
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
						>::id_type& l,
					const
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
						>& r)
{
	return r.compare_eq(l);
}

template <typename Tag,
			typename Conn,
			typename Adapter_Mgr,
			typename PacketsChecker,
			typename PacketsCrypher,
			typename First_Action,
			typename State_Fixer,
			typename Mutex> inline
bool operator==(const 
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
					>& l,
					const
						typename
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
							>::id_type& r)
{
	return l.compare_eq(r);
}

template <typename Conn,
			typename TagL,
			typename Adapter_MgrL,
			typename PacketsCheckerL,
			typename PacketsCrypherL,
			typename First_ActionL,
			typename State_FixerL,
			typename MutexL,
			typename TagR,
			typename Adapter_MgrR,
			typename PacketsCheckerR,
			typename PacketsCrypherR,
			typename First_ActionR,
			typename State_FixerR,
			typename MutexR > inline
bool operator==(const 
					session
					<
						TagL,
						Conn, 
						Adapter_MgrL, 
						PacketsCheckerL, 
						PacketsCrypherL, 
						First_ActionL, 
						State_FixerL, 
						MutexL
					>& l,
					const
						session
						<
							TagR,
							Conn, 
							Adapter_MgrR, 
							PacketsCheckerR, 
							PacketsCrypherR, 
							First_ActionR, 
							State_FixerR, 
							MutexR
						>& r)
{
	return l.compare_eq(r);
}

template <typename Tag,
			typename Conn,
			typename Adapter_Mgr,
			typename PacketsChecker,
			typename PacketsCrypher,
			typename First_Action,
			typename State_Fixer,
			typename Mutex> inline
bool operator==(const 
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
					>& l,
					const
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
						>& r)
{
	return l.compare_eq(r);
}

// operator !=

template <typename Tag,
			typename Conn,
			typename Adapter_Mgr,
			typename PacketsChecker,
			typename PacketsCrypher,
			typename First_Action,
			typename State_Fixer,
			typename Mutex> inline
bool operator!=(const 
					typename
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
						>::id_type& l,
					const
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
						>& r)
{
	return !r.compare_eq(l);
}

template <typename Tag,
			typename Conn,
			typename Adapter_Mgr,
			typename PacketsChecker,
			typename PacketsCrypher,
			typename First_Action,
			typename State_Fixer,
			typename Mutex> inline
bool operator!=(const 
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
					>& l,
					const
						typename
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
							>::id_type& r)
{
	return !l.compare_eq(r);
}

template <typename Conn,
			typename TagL,
			typename Adapter_MgrL,
			typename PacketsCheckerL,
			typename PacketsCrypherL,
			typename First_ActionL,
			typename State_FixerL,
			typename MutexL,
			typename TagR,
			typename Adapter_MgrR,
			typename PacketsCheckerR,
			typename PacketsCrypherR,
			typename First_ActionR,
			typename State_FixerR,
			typename MutexR > inline
bool operator!=(const 
					session
					<
						TagL,
						Conn, 
						Adapter_MgrL, 
						PacketsCheckerL, 
						PacketsCrypherL, 
						First_ActionL, 
						State_FixerL, 
						MutexL
					>& l,
					const
						session
						<
							TagR,
							Conn, 
							Adapter_MgrR, 
							PacketsCheckerR, 
							PacketsCrypherR, 
							First_ActionR, 
							State_FixerR, 
							MutexR
						>& r)
{
	return !l.compare_eq(r);
}

template <typename Tag,
			typename Conn,
			typename Adapter_Mgr,
			typename PacketsChecker,
			typename PacketsCrypher,
			typename First_Action,
			typename State_Fixer,
			typename Mutex> inline
bool operator!=(const 
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
					>& l,
					const
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
						>& r)
{
	return !l.compare_eq(r);
}


} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_SESSION_HPP__
