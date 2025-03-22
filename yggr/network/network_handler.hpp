//network_handler.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_HANDLER_HPP__
#define __YGGR_NETWORK_NETWORK_HANDLER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>

#include <boost/atomic.hpp>

#include <cassert>

namespace yggr
{
namespace network
{

template<typename OP_Handler>
class network_handler
{
public:
	typedef OP_Handler op_handler_type;
	typedef typename op_handler_type::service_handler_type service_handler_type;

	typedef typename service_handler_type::init_type service_handler_init_type;
	typedef typename op_handler_type::init_type op_handler_init_type;
	typedef typename op_handler_type::adapter_mgr_reg_def_type adapter_mgr_reg_def_type;
	typedef typename op_handler_type::adapter_mgr_type adapter_mgr_type;
	typedef typename op_handler_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;

private:
	typedef network_handler this_type;

public:
	template<typename Service_Handler_Init, typename OP_Handler_Init>
	network_handler(const Service_Handler_Init& service_init,
						const OP_Handler_Init& svr_init)
		: _brun(false),
			_service_handler(service_init),
			_op_handler(_service_handler, const_cast<OP_Handler_Init&>(svr_init))
	{
	}

	~network_handler(void)
	{
		assert(!_brun.load());
	}

public:
	template<typename Real_Data> inline
	bool register_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		return _op_handler.template register_network_protocol<Real_Data>(stat);
	}

	template<typename Real_Data, typename Container> inline
	bool register_network_protocol_of_container(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		return _op_handler.template register_network_protocol_of_container<Real_Data, Container>(stat);
	}

	template<typename Container, typename Send_ID, typename Recv_ID> inline
	bool register_network_protocol(const Send_ID& send_id, const Recv_ID& recv_id,
									u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		return _op_handler.template register_network_protocol<Container>(send_id, recv_id, stat);
	}

	template<typename Real_Data> inline
	bool unregister_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		return _op_handler.template unregister_network_protocol<Real_Data>(stat);
	}

	template<typename ID> inline
	bool unregister_network_protocol(const ID& id, 
										u32 tag = adapter_mgr_type::template default_tag<ID>::value,
										u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		return _op_handler.unregister_network_protocol(id, tag, stat);
	}

	inline void add_signal(u32 sig)
	{
		_service_handler.add_signal(sig);
	}

	inline void join(void)
	{
		_service_handler.join();
	}

	void run(void)
	{
		bool now = false;
		bool next = true; 
		if(!_brun.compare_exchange_strong(now, next))
		{
			return;
		}

	    _service_handler.add_signal(SIGINT);
		_service_handler.add_signal(SIGTERM);
#   if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGBREAK);
#   endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

#if defined(YGGR_AT_ANDROID)
		_service_handler.add_signal(SIGSTOP);
#endif // #if defined(YGGR_AT_ANDROID)

		_service_handler.add_signal(SIGFPE);
		_service_handler.add_signal(SIGILL);
		_service_handler.add_signal(SIGABRT);
#	if defined(SIGQUIT)
		_service_handler.add_signal(SIGQUIT);
#	endif // defined(SIGQUIT)
#	if defined(SIGTSTP)
		_service_handler.add_signal(SIGTSTP);
#	endif // SIGTSTP
		_service_handler.set_signal_handler(boost::bind(&this_type::stop, this));

		_op_handler.run();
		_service_handler.run();

		assert(_brun.load() == _service_handler.is_running());
	}

	template<typename Handler>
	void run(const Handler& handler)
	{
		bool now = false;
		bool next = true; 
		if(!_brun.compare_exchange_strong(now, next))
		{
			return;
		}

	    _service_handler.add_signal(SIGINT);
		_service_handler.add_signal(SIGTERM);
#   if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGBREAK);
#   endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

#if defined(YGGR_AT_ANDROID)
		_service_handler.add_signal(SIGSTOP);
#endif // #if defined(YGGR_AT_ANDROID)

		_service_handler.add_signal(SIGFPE);
		_service_handler.add_signal(SIGILL);
		_service_handler.add_signal(SIGABRT);
#	if defined(SIGQUIT)
		_service_handler.add_signal(SIGQUIT);
#	endif // defined(SIGQUIT)
#	if defined(SIGTSTP)
		_service_handler.add_signal(SIGTSTP);
#	endif // SIGTSTP
		_service_handler.set_signal_handler(handler);

		_op_handler.run();
		_service_handler.run();

		assert(_brun.load() == _service_handler.is_running());
	}

	void stop(void)
	{
		bool now = true;
		bool next = false; 
		if(!_brun.compare_exchange_strong(now, next))
		{
			return;
		}

		_service_handler.stop();
		_op_handler.stop();

		_service_handler.remove_signal(SIGINT);
		_service_handler.remove_signal(SIGTERM);
#   if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.remove_signal(SIGBREAK);
#   endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

#if defined(YGGR_AT_ANDROID)
		_service_handler.remove_signal(SIGSTOP);
#endif // #if defined(YGGR_AT_ANDROID)

		_service_handler.remove_signal(SIGFPE);
		_service_handler.remove_signal(SIGILL);
		_service_handler.remove_signal(SIGABRT);
#	if defined(SIGQUIT)
		_service_handler.remove_signal(SIGQUIT);
#	endif // defined(SIGQUIT)
#if defined(SIGTSTP)
		_service_handler.remove_signal(SIGTSTP);
#endif // SIGTSTP
		_service_handler.cancel_signal_handler();

		assert(_brun.load() == _service_handler.is_running());
	}

	inline size_type online_size(void) const
	{
		return _op_handler.online_size();
	}

	inline void clear_sessions(void)
	{
		_op_handler.clear_sessions();
	}

	inline void clear(void)
	{
		_op_handler.clear();
	}

	inline service_handler_type& service_handler(void)
	{
		return _service_handler;
	}

	inline op_handler_type& op_handler(void)
	{
		return _op_handler;
	}

	inline void check_state(void)
	{
		_op_handler.check_state();
	}

	template<typename Key, typename NetInfo, typename Data, typename Handler> inline
	bool send_packet(const Key& key, const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		return _op_handler.send_packet(key, netinfo, data, handler);
	}

	template<typename Key, typename Pak, typename Handler> inline 
	bool send_packet(const Key& key, Pak& pak, const Handler& handler)
	{
		return _op_handler.send_packet(key, pak, handler);
	}

	template<typename Key, typename Pak> inline
	bool send_packet(const Key& key, Pak& pak)
	{
		return _op_handler.send_packet(key, pak);
	}

	template<typename Key, typename Handler> inline
	bool access(const Key& key, const Handler& handler)
	{
		return _op_handler.access(key, handler);
	}

	template<typename Key, typename Handler> inline
	bool access(const Key& key, const Handler& handler) const
	{
		return _op_handler.access(key, handler);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type 
		access(const Handler& handler)
	{
		return _op_handler.access(handler);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type 
		access(const Handler& handler) const
	{
		return _op_handler.access(handler);
	}

	inline adapter_mgr_ptr_type get_adapter_mgr_ptr(void) const
	{
		return _op_handler.get_adapter_mgr_ptr();
	}

private:

	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename CtrlCenter> inline
	void register_controller(CtrlCenter& cc)
	{
		_op_handler.template register_controller<Tag>(cc);
	}

private:
	boost::atomic<bool> _brun;
	service_handler_type _service_handler;
	op_handler_type _op_handler;
};

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_NETWORK_HANDLER_HPP__
