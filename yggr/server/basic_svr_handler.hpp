//basic_svr_handler.hpp

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

#ifndef __YGGR_NETWORK_BASIC_SERVER_HANDLER_HPP__
#define __YGGR_NETWORK_BASIC_SERVER_HANDLER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/bind/bind.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/system_controller/system_code.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/ppex/friend.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/network/network_handler.hpp>
#include <yggr/network/session_helper/tcp_async_acceptor_handler.hpp>
#include <yggr/network/session_helper/udp_async_acceptor_handler.hpp>
#include <yggr/network/session_helper/options_setter.hpp>
#include <yggr/network/type_traits/tags.hpp>

#include <boost/asio.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <memory>

namespace yggr
{
namespace server
{
namespace detail
{

// basic_svr_handler_init_t
template<typename Source, typename LinkHandlerInit>
struct basic_svr_handler_init_t
{
public:
	typedef Source source_type;
	typedef LinkHandlerInit link_handler_init_type;

private:
	typedef basic_svr_handler_init_t this_type;

public:
	basic_svr_handler_init_t(u16 nport, source_type& nsrc, const link_handler_init_type& lkh)
		: _port(nport), _src(nsrc), _lkh_init(lkh)
	{
	}

	basic_svr_handler_init_t(const this_type& right)
		: _port(right._port), _src(right._src), _lkh_init(right._lkh_init)
	{
	}

	~basic_svr_handler_init_t(void)
	{
	}

public:
	inline u16 port(void) const
	{
		return _port;
	}

	inline source_type& session_mgr_init(void) const
	{
		return _src;
	}

	inline const link_handler_init_type& lkh_init(void) const
	{
		return _lkh_init;
	}

private:
	u16 _port;
	source_type& _src;
	link_handler_init_type _lkh_init;
};

//basic_svr_handler_check_state_op
template<typename Protocol_Tag, typename SessionMgrPtr, typename AcceptorHandler>
struct basic_svr_handler_check_state_op;

template<typename SessionMgrPtr, typename AcceptorHandler>
struct basic_svr_handler_check_state_op<yggr::network::type_traits::tag_tcp, SessionMgrPtr, AcceptorHandler>
{
	typedef SessionMgrPtr session_mgr_ptr_type;
	typedef AcceptorHandler acceptor_handler_type;

	inline void operator()(session_mgr_ptr_type ptr, acceptor_handler_type&) const
	{
		ptr->check_state();
	}
};

template<typename SessionMgrPtr, typename AcceptorHandler>
struct basic_svr_handler_check_state_op<yggr::network::type_traits::tag_udp, SessionMgrPtr, AcceptorHandler>
{
	typedef SessionMgrPtr session_mgr_ptr_type;
	typedef AcceptorHandler acceptor_handler_type;

	inline void operator()(session_mgr_ptr_type ptr, acceptor_handler_type& ah) const
	{
		ptr->check_state();
		ah.check_state();
	}
};

} // namespace detail
} // namespace server
} // namespace yggr

namespace yggr
{
namespace server
{

template<	typename ServerConfig,
			typename Service_Handler,
			typename SessionMgr,
			typename OptionsSetter = network::default_options_setter_type,
			template<typename _ServicePool,
						typename _Acceptor,
						typename _SessionMgrWrap,
						typename _OptionsSetter,
						typename _Error_Maker>
			class Acceptor_Handler = network::session_helper::tcp_async_acceptor_handler
		>
class basic_svr_handler
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef ServerConfig server_config_type;
	typedef Service_Handler service_handler_type;
	typedef SessionMgr session_mgr_type;
	typedef OptionsSetter options_setter_type;
	typedef typename session_mgr_type::adapter_mgr_type adapter_mgr_type;
	typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;

private:
	typedef typename server_config_type::error_maker_type error_maker_type;
	typedef typename server_config_type::error_type error_type;

private:
	typedef typename session_mgr_type::session_type session_type;
	typedef typename session_mgr_type::link_init_type link_init_type;
	typedef typename session_mgr_type::service_type service_type;
	typedef typename session_mgr_type::conn_ptr_type conn_ptr_type;
	typedef typename session_mgr_type::smap_iter_type session_iter_type;

	typedef typename session_mgr_type::endpoint_type endpoint_type;
	typedef typename session_mgr_type::protocol_type protocol_type;

	typedef typename adapter_mgr_type::src_type source_type;
	typedef typename adapter_mgr_type::send_std_data_type send_std_data_type;
	typedef typename adapter_mgr_type::recv_rst_data_type recv_rst_data_type;
	typedef typename adapter_mgr_type::reg_def_type adapter_mgr_reg_def_type;

	typedef typename server_config_type::acceptor_type acceptor_type;
	typedef ::yggr::shared_ptr<acceptor_type> acceptor_ptr_type;

	typedef safe_container::safe_wrap<session_mgr_type> session_mgr_wrap_type;

public:
	typedef typename session_mgr_wrap_type::value_ptr_type session_mgr_ptr_type;

private:
	typedef typename service_handler_type::io_service_pool_type io_service_pool_type;

	typedef
		Acceptor_Handler
		<
			io_service_pool_type,
			acceptor_type,
			session_mgr_wrap_type,
			options_setter_type,
			error_maker_type
		> acceptor_handler_type;

public:

	typedef typename session_mgr_type::protocol_tag_type protocol_tag_type;
	typedef typename acceptor_handler_type::init_type link_handler_init_type;

	typedef typename server_config_type::tag_type tag_type;
	BOOST_MPL_ASSERT((
		boost::is_same
		<
			tag_type,
			typename session_mgr_type::tag_type
		>));

private:
	typedef basic_svr_handler this_type;
	YGGR_PP_FRIEND_CLASS(network::network_handler<this_type>);

public:
	typedef u32 id_type;
	typedef
		detail::basic_svr_handler_init_t
		<
			source_type,
			link_handler_init_type
		> init_type;

public:
	template<typename Init>
	basic_svr_handler(service_handler_type& sh, const Init& init)
		: _service_handler(sh),
            _pacceptor(construct_shared<acceptor_type>(
							yggr_nothrow_new acceptor_type(
								_service_handler.get_service(),
								endpoint_type(
									session_mgr_type::is_v4()? protocol_type::v4() : protocol_type::v6(),
									init.port()) ) ) ),
			_session_mgr_wrap(yggr_nothrow_new session_mgr_type(init.session_mgr_init())),
			_acceptor_handler(_service_handler.service_pool(), _pacceptor, _session_mgr_wrap, init.lkh_init())
	{
	}

	~basic_svr_handler(void)
	{
	}

public:

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
        this_type::listen();
	}

	inline session_mgr_ptr_type get_session_mgr(void) const
	{
		return _session_mgr_wrap.get_shared_ptr();
	}

	void stop(void)
	{
		assert(!_service_handler.is_running());
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			ptr->clear();
			_session_mgr_wrap.clear();
		}

		if(!_pacceptor)
		{
			return;
		}

		try
		{
			_pacceptor->cancel();
			_pacceptor->close();
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e);
		}

		_acceptor_handler.close();
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

private:
	typedef
		detail::basic_svr_handler_check_state_op
		<
			protocol_tag_type,
			session_mgr_ptr_type,
			acceptor_handler_type
		> check_state_op_type;

public:
	inline void check_state(void)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			check_state_op_type op;
			op(ptr, _acceptor_handler);
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
		typedef typename handler_type::result_type ret_type;
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
		typedef typename handler_type::result_type ret_type;

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
	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename CtrlCenter> inline
	void register_controller(CtrlCenter& cc)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			cc.template register_controller<Tag>(*ptr);
		}
	}

private:
	inline void listen(void)
	{
		_acceptor_handler.listen();
	}

protected:
	service_handler_type& _service_handler;
	acceptor_ptr_type _pacceptor;
	session_mgr_wrap_type _session_mgr_wrap;
	acceptor_handler_type _acceptor_handler;
};

} // namespace server
} // namespace yggr

#endif //__YGGR_NETWORK_BASIC_SERVER_HANDLER_HPP__
