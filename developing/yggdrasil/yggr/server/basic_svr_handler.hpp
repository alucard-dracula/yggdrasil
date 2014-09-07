//basic_svr_handler.hpp

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

#ifndef __YGGR_NETWORK_BASIC_SERVER_HANDLER_HPP__
#define __YGGR_NETWORK_BASIC_SERVER_HANDLER_HPP__

#include <memory>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/ctrl_center.hpp>
#include <yggr/base/system_code.hpp>
#include <yggr/base/exception.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/network/network_handler.hpp>
#include <yggr/network/session_helper/tcp_async_acceptor_handler.hpp>
#include <yggr/network/session_helper/udp_async_acceptor_handler.hpp>
#include <yggr/network/type_traits/tags.hpp>

namespace yggr
{
namespace server
{

template<	typename ServerConfig,
			typename Service_Handler,
			typename SessionMgr,
			template<typename _ServicePool, typename _Acceptor,
						typename _SessionMgrWrap, typename _Error_Maker>
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

private:

	typedef typename server_config_type::error_maker_type error_maker_type;
	typedef typename server_config_type::error_type error_type;

private:

	typedef typename session_mgr_type::session_type session_type;
	typedef typename session_mgr_type::link_init_type link_init_type;
	typedef typename session_mgr_type::service_type service_type;
	typedef typename session_mgr_type::conn_ptr_type conn_ptr_type;
	typedef typename session_mgr_type::smap_iter_type session_iter_type;
	//typedef typename session_mgr_type::send_data_type send_data_type;
	//typedef typename session_mgr_type::recv_data_type recv_data_type;
	typedef typename session_mgr_type::adapter_mgr_type adapter_mgr_type;
	typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;
	typedef typename session_mgr_type::endpoint_type endpoint_type;
	typedef typename session_mgr_type::protocol_type protocol_type;

	typedef typename adapter_mgr_type::src_type source_type;
	typedef typename adapter_mgr_type::send_std_data_type send_std_data_type;
	typedef typename adapter_mgr_type::recv_rst_data_type recv_rst_data_type;
	typedef typename adapter_mgr_type::reg_def_type adapter_mgr_reg_def_type;

	typedef typename server_config_type::acceptor_type acceptor_type;
	typedef boost::shared_ptr<acceptor_type> acceptor_ptr_type;

	typedef safe_container::safe_wrap<session_mgr_type> session_mgr_wrap_type;
	typedef typename session_mgr_wrap_type::value_ptr_type session_mgr_ptr_type;

	typedef typename service_handler_type::io_service_pool_type io_service_pool_type;

	typedef Acceptor_Handler<io_service_pool_type,
								acceptor_type,
								session_mgr_wrap_type,
								error_maker_type
							> acceptor_handler_type;

public:

	typedef typename session_mgr_type::protocol_tag_type protocol_tag_type;
	typedef typename acceptor_handler_type::init_type link_handler_init_type;

	typedef typename server_config_type::tag_type tag_type;
	BOOST_MPL_ASSERT((boost::is_same
						<
							tag_type,
							typename session_mgr_type::tag_type
						>));

private:
	typedef basic_svr_handler this_type;

	friend class network::network_handler<this_type>;
public:
	//enum
	//{
	//	E_id = 0x0008001,

	//	E_compile_u32 = 0xffffffff
	//};

	typedef u32 id_type;

	typedef struct _init_t
	{
	public:
		_init_t(u16 nport, source_type& nsrc, const link_handler_init_type& lkh)
			: _port(nport), _src(nsrc), _lkh_init(lkh)
		{
		}

		~_init_t(void)
		{
		}

		_init_t(const _init_t& init)
			: _port(init._port), _src(init._src), _lkh_init(init._lkh_init)
		{
		}

		const u16& port(void) const
		{
			return _port;
		}

		source_type& session_mgr_init(void) const
		{
			return _src;
		}

		const link_handler_init_type& lkh_init(void) const
		{
			return _lkh_init;
		}

	private:
		u16 _port;
		source_type& _src;
		link_handler_init_type _lkh_init;
	} init_type;

public:

	template<typename Init>
	basic_svr_handler(service_handler_type& sh, const Init& init)
		: _service_handler(sh),
            _pacceptor(new acceptor_type(_service_handler.get_service(),
											endpoint_type(session_mgr_type::is_v4()?
												protocol_type::v4():protocol_type::v6(),
												init.port()))),
			_session_mgr_wrap(session_mgr_ptr_type(new session_mgr_type(init.session_mgr_init()))),
			_acceptor_handler(_service_handler.service_pool(), _pacceptor, _session_mgr_wrap, init.lkh_init())
	{
		session_mgr_ptr_type psmgr = _session_mgr_wrap.get_shared_ptr();
		if(!psmgr)
		{
			return;
		}

		//psmgr->init();
	}

	~basic_svr_handler(void)
	{
	}

	template<typename Real_Data>
	bool register_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return false;
		}

		return ptr->template register_adapter<Real_Data>(stat);
	}

	template<typename Real_Data, typename Container>
	bool register_network_protocol_of_container(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return false;
		}

		return ptr->template register_adapter_of_container<Real_Data, Container>(stat);
	}

	template<typename Container, typename Send_ID, typename Recv_ID>
	bool register_network_protocol(const Send_ID& send_id, const Recv_ID& recv_id,
									u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return false;
		}

		return ptr->template register_adapter<Container>(send_id, recv_id, stat);
	}

	template<typename Real_Data>
	bool unregister_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return false;
		}

		return ptr->template unregister_adapter<Real_Data>(stat);
	}

	template<typename ID>
	bool unregister_network_protocol(const ID& id, u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return false;
		}

		return ptr->unregister_adapter(id, stat);
	}

	void run(void)
	{
        listen();
	}

	session_mgr_ptr_type get_session_mgr(void) const
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

	size_type online_size(void) const
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		return ptr? ptr->size() : 0;
	}

	void clear_sessions(void)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			ptr->clear_sessions();
		}
	}

	void clear(void)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			ptr->clear();
		}
	}

private:
	template<typename Protocol_Tag, typename Nil_T = int>
	struct check_state_op;

	template<typename Nil_T>
	struct check_state_op<yggr::network::type_traits::tag_tcp, Nil_T>
	{
		void operator()(session_mgr_ptr_type ptr, acceptor_handler_type& ah) const
		{
			ptr->check_state();
		}
	};

	template<typename Nil_T>
	struct check_state_op<yggr::network::type_traits::tag_udp, Nil_T>
	{
		void operator()(session_mgr_ptr_type ptr, acceptor_handler_type& ah) const
		{
			ptr->check_state();
			ah.check_state();
		}
	};

public:
	void check_state(void)
	{
		typedef check_state_op<protocol_tag_type> op_type;
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			op_type op;
			op(ptr, _acceptor_handler);
		}
	}

	template<typename Key, typename NetInfo, typename Data, typename Handler>
	bool send_packet(const Key& key, const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return false;
		}

		session_mgr_type& smgr = *ptr;
		return smgr.send_packet(key, netinfo, data, handler);
	}

	template<typename Key, typename Pak, typename Handler>
	bool send_packet(const Key& key, Pak& pak, const Handler& handler)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return false;
		}

		session_mgr_type& smgr = *ptr;
		return smgr.send_packet(key, pak, handler);
	}

	template<typename Key, typename Pak>
	bool send_packet(const Key& key, Pak& pak)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return false;
		}

		session_mgr_type& smgr = *ptr;
		return smgr.send_packet(key, pak);
	}

	template<typename Key, typename Handler>
	bool access(const Key& key, const Handler& handler)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return false;
		}

		session_mgr_type& smgr = *ptr;
		return smgr.access(key, handler);
	}

	template<typename Key, typename Handler>
	bool access(const Key& key, const Handler& handler) const
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return false;
		}
		const session_mgr_type& smgr = *ptr;
		return smgr.access(key, handler);
	}

	template<typename Handler>
	typename Handler::result_type access(const Handler& handler)
	{
		typedef Handler handler_type;
		typedef typename handler_type::result_type ret_type;
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return ret_type();
		}

		session_mgr_type& smgr = *ptr;
		return smgr.access(handler);
	}

	template<typename Handler>
	typename Handler::result_type access(const Handler& handler) const
	{
		typedef Handler handler_type;
		typedef typename handler_type::result_type ret_type;

		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(!ptr)
		{
			return ret_type();
		}
		const session_mgr_type& smgr = *ptr;
		return smgr.access(handler);
	}

	template<typename Handler>
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

	template<typename Handler>
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

private:


	friend class yggr::system_controller::ctrl_center;
	template<typename Tag, typename Ctrl_Center>
	void register_controller(Ctrl_Center& cc)
	{
		session_mgr_ptr_type ptr = _session_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			cc.register_controller<Tag>(*ptr);
		}
	}

private:
	void listen(void)
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
