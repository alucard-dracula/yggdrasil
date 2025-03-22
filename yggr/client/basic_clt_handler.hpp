//basic_clt_handler.hpp

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

#ifndef __YGGR_CLIENT_TCP_CLIENT_HANDLER_HPP__
#define __YGGR_CLIENT_TCP_CLIENT_HANDLER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/network/network_handler.hpp>
#include <yggr/network/session_helper/options_setter.hpp>
#include <yggr/network/session_helper/tcp_connect_handler.hpp>
#include <yggr/network/session_helper/udp_connect_handler.hpp>
#include <yggr/network/type_traits/tags.hpp>

#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/system_controller/system_code.hpp>

#include <yggr/bind/bind.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/nonable/nonable.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <boost/asio.hpp>
#include <boost/type_traits/is_same.hpp>

#include <memory>

namespace yggr
{
namespace client
{
namespace detail
{

template<typename Source, typename LinkHandleInit>
struct client_init
{
public:
	typedef Source source_type;
	typedef LinkHandleInit link_handler_init_type;

private:
	typedef client_init this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	client_init(source_type& nsrc, const link_handler_init_type& lkh)
		: _src(nsrc), _lkh_init(lkh)
	{
	}

	client_init(const this_type& right)
		: _src(right._src), _lkh_init(right._lkh_init)
	{
	}

	~client_init(void)
	{
	}

public:
	inline source_type& session_mgr_init(void) const
	{
		return _src;
	}

	inline link_handler_init_type lkh_init(void) const
	{
		return _lkh_init;
	}

private:
	source_type& _src;
	link_handler_init_type _lkh_init;
};

// check_state_op

template<typename SessionMgrPtr, typename ConectHandler, typename Protocol_Tag>
struct check_state_op;

template<typename SessionMgrPtr, typename ConectHandler>
struct check_state_op<SessionMgrPtr, ConectHandler, yggr::network::type_traits::tag_tcp>
{
public:
	typedef SessionMgrPtr session_mgr_ptr_type;
	typedef ConectHandler connect_handler_type;

private:
	typedef check_state_op this_type;

public:
	inline void operator()(session_mgr_ptr_type ptr, connect_handler_type&) const
	{
		ptr->check_state();
	}
};

template<typename SessionMgrPtr, typename ConectHandler>
struct check_state_op<SessionMgrPtr, ConectHandler, yggr::network::type_traits::tag_udp>
{
public:
	typedef SessionMgrPtr session_mgr_ptr_type;
	typedef ConectHandler connect_handler_type;

private:
	typedef check_state_op this_type;

public:
	inline void operator()(session_mgr_ptr_type ptr, connect_handler_type& ch) const
	{
		ptr->check_state();
		ch.check_state();
	}
};

} // namespace detail
} // namespace client
} // namespace yggr

namespace yggr
{
namespace client
{

template<
			typename ClientConfig,
			typename Service_Handler,
			typename SessionMgr,
			typename OptionsSetter = network::default_options_setter_type,
			template<typename _ServicePool,
						typename _SessionMgrWrap,
						typename _OptionsSetter,
						typename _Error_Maker>
				class Connect_Handler = network::session_helper::tcp_connect_handler
		>
class basic_clt_handler
	: private nonable::noncopyable,
		private nonable::nonmoveable
{

public:
	typedef ClientConfig client_config_type;
	typedef Service_Handler service_handler_type;
	typedef SessionMgr session_mgr_type;
	typedef OptionsSetter options_setter_type;

	typedef typename session_mgr_type::adapter_mgr_type adapter_mgr_type;
	typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;

private:
	typedef typename client_config_type::error_maker_type error_maker_type;
	typedef typename client_config_type::error_type error_type;

private:
	typedef typename session_mgr_type::session_type session_type;
	typedef typename session_mgr_type::link_init_type link_init_type;
	typedef typename session_mgr_type::service_type service_type;
	typedef typename session_mgr_type::conn_ptr_type conn_ptr_type;
	typedef typename session_mgr_type::smap_iter_type session_iter_type;
	typedef typename session_mgr_type::recv_data_type recv_data_type;

	typedef typename session_mgr_type::endpoint_type endpoint_type;
	typedef typename session_mgr_type::protocol_type protocol_type;

	typedef typename adapter_mgr_type::src_type source_type;
	typedef typename adapter_mgr_type::send_std_data_type send_std_data_type;
	typedef typename adapter_mgr_type::recv_rst_data_type recv_rst_data_type;

	typedef safe_container::safe_wrap<session_mgr_type> session_mgr_wrap_type;

public:
	typedef typename adapter_mgr_type::reg_def_type adapter_mgr_reg_def_type;
	typedef typename session_mgr_wrap_type::value_ptr_type session_mgr_ptr_type;

private:
	typedef typename service_handler_type::io_service_pool_type io_service_pool_type;

	typedef 
		Connect_Handler
		<
			io_service_pool_type,
			session_mgr_wrap_type,
			options_setter_type,
			error_maker_type
		> connect_handler_type;

public:
	typedef typename session_mgr_type::protocol_tag_type protocol_tag_type;
	typedef typename connect_handler_type::init_type link_handler_init_type;
	typedef typename client_config_type::tag_type tag_type;

private:
	BOOST_MPL_ASSERT((
		boost::is_same
		<
			tag_type,
			typename session_mgr_type::tag_type
		>));

private:
	typedef basic_clt_handler this_type;

private:
	template<typename OpHandler>
	friend class yggr::network::network_handler;

	friend class yggr::system_controller::ctrl_center;

public:
	typedef detail::client_init<source_type, link_handler_init_type> init_type;

protected:
	template<typename Tag, typename CtrlCenter> inline
	void register_controller(CtrlCenter& cc)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			cc.template register_controller<Tag>(*ptr);
		}
	}

public:
	template<typename Init>
	basic_clt_handler(service_handler_type& sh, const Init& init)
		: _service_handler(sh),
			_session_mgr_wrap(construct_shared<session_mgr_type>(yggr_nothrow_new session_mgr_type(init.session_mgr_init()))),
			_connect_handler(_service_handler.service_pool(), _session_mgr_wrap, init.lkh_init())
	{
	}

	~basic_clt_handler(void)
	{
	}

public:
	template<typename Address, typename Port> inline
	void connect(const Address& addr, const Port& port)
	{
		_connect_handler.connect(addr, port);
	}

	template<typename Address, typename Port, typename Time> inline
	void connect(const Address& addr, const Port& port, const Time& tm_duration)
	{
		_connect_handler.connect(addr, port, tm_duration);
	}

	template<typename Real_Data> inline
	bool register_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		
		return ptr && ptr->template register_adapter<Real_Data>(stat);
	}

	template<typename Real_Data, typename Container> inline
	bool register_network_protocol_of_container(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		
		return ptr && ptr->template register_adapter_of_container<Real_Data, Container>(stat);
	}

	template<typename Container, typename Send_ID, typename Recv_ID> inline
	bool register_network_protocol(const Send_ID& send_id, const Recv_ID& recv_id,
									u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		
		return ptr && ptr->template register_adapter<Container>(send_id, recv_id, stat);
	}

	template<typename Real_Data> inline
	bool unregister_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();

		return ptr && ptr->template unregister_adapter<Real_Data>(stat);
	}

	template<typename ID> inline
	bool unregister_network_protocol(const ID& id, 
										u32 tag = adapter_mgr_type::template default_tag<ID>::value,
										u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();

		return ptr && ptr->unregister_adapter(id, tag, stat);
	}

	inline void run(void)
	{
	}

	inline session_mgr_ptr_type get_session_mgr(void) const
	{
		return _session_mgr_wrap.get_shared_ptr();
	}

	inline void stop(void)
	{
		assert(!_service_handler.is_running());
		_connect_handler.close();
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			ptr->clear();
			_session_mgr_wrap.clear();
		}
	}

	inline size_type online_size(void) const
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		return ptr? ptr->size() : 0;
	}

	inline void clear_sessions(void)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			ptr->clear_sessions();
		}
	}

	inline void clear(void)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			ptr->clear();
		}
	}

public:
	inline void check_state(void)
	{
		typedef 
			detail::check_state_op
			<
				session_mgr_ptr_type, 
				connect_handler_type,
				protocol_tag_type
			> op_type;
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			op_type op;
			op(ptr, _connect_handler);
		}
	}

	template<typename Key, typename NetInfo, typename Data, typename Handler> inline
	bool send_packet(const Key& key, const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		
		return ptr && ptr->send_packet(key, netinfo, data, handler);
	}

	template<typename Key, typename Pak, typename Handler> inline
	bool send_packet(const Key& key, Pak& pak, const Handler& handler)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		
		return ptr && ptr->send_packet(key, pak, handler);
	}

	template<typename Key, typename Pak> inline
	bool send_packet(const Key& key, Pak& pak)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		
		return ptr && ptr->send_packet(key, pak);
	}

	template<typename Key, typename Handler> inline
	bool access(const Key& key, const Handler& handler)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		
		return ptr && ptr->access(key, handler);
	}

	template<typename Key, typename Handler> inline
	bool access(const Key& key, const Handler& handler) const
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		
		return ptr && ptr->access(key, handler);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type 
		access(const Handler& handler)
	{
		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;

		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		assert(ptr);
		session_mgr_type& smgr = *ptr;
		return smgr.access(handler);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type 
		access(const Handler& handler) const
	{
		typedef Handler handler_type;
		typedef typename func::foo_t_info<handler_type>::result_type ret_type;

		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		assert(ptr);
		const session_mgr_type& smgr = *ptr;
		return smgr.access(handler);
	}

	template<typename Handler> inline
	void access_of_all(const Handler& handler)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return;
		}

		session_mgr_type& smgr = *ptr;
		smgr.access(handler);
	}

	template<typename Handler> inline
	void access_of_all(const Handler& handler) const
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return;
		}
		const session_mgr_type& smgr = *ptr;
		smgr.access(handler);
	}

	inline adapter_mgr_ptr_type get_adapter_mgr_ptr(void) const
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		return ptr? ptr->get_adapter_mgr_ptr() : adapter_mgr_ptr_type();
	}

protected:
	service_handler_type& _service_handler;
	session_mgr_wrap_type _session_mgr_wrap;
	connect_handler_type _connect_handler;
};

} // namespace client
} // namespace yggr

#endif //__YGGR_CLIENT_TCP_CLIENT_HANDLER_HPP__
