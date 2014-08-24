//network_handler.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_HANDLER_HPP__
#define __YGGR_NETWORK_NETWORK_HANDLER_HPP__

namespace yggr
{
namespace network
{

template<typename OP_Handler>
class network_handler
{
public:
	//typedef Service_Handler service_handler_type;
	typedef OP_Handler op_handler_type;
	typedef typename op_handler_type::service_handler_type service_handler_type;

	typedef typename service_handler_type::init_type service_handler_init_type;
	typedef typename op_handler_type::init_type op_handler_init_type;
	typedef typename op_handler_type::adapter_mgr_reg_def_type adapter_mgr_reg_def_type;
private:

	typedef network_handler this_type;

public:
	template<typename Service_Handler_Init, typename OP_Handler_Init>
	network_handler(const Service_Handler_Init& service_init,
						OP_Handler_Init& svr_init)
		: _service_handler(service_init),
			_op_handler(_service_handler, svr_init)
	{
	}

	~network_handler(void)
	{
	}

	template<typename Real_Data>
	bool register_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		return _op_handler.template register_network_protocol<Real_Data>(stat);
	}

	template<typename Real_Data, typename Container>
	bool register_network_protocol_of_container(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		return _op_handler.template register_network_protocol_of_container<Real_Data, Container>(stat);
	}

	template<typename Container, typename Send_ID, typename Recv_ID>
	bool register_network_protocol(const Send_ID& send_id, const Recv_ID& recv_id,
									u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		return _op_handler.template register_network_protocol<Container>(send_id, recv_id, stat);
	}

	template<typename Real_Data>
	bool unregister_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		return _op_handler.template unregister_network_protocol<Real_Data>(stat);
	}

	template<typename ID>
	bool unregister_network_protocol(const ID& id, u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		return _op_handler.unregister_network_protocol(id, stat);
	}

	void add_signal(u32 sig)
	{
		_service_handler.add_signal(sig);
	}

	void join(void)
	{
		_service_handler.join();
	}

	void run(void)
	{
	    _service_handler.add_signal(SIGINT);
		_service_handler.add_signal(SIGTERM);
#   if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGBREAK);
#   endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGFPE);
		_service_handler.add_signal(SIGILL);
		_service_handler.add_signal(SIGABRT);
#	if defined(SIGQUIT)
		_service_handler.add_signal(SIGQUIT);
#	endif // defined(SIGQUIT)
		_service_handler.set_signal_handler(boost::bind(&this_type::stop, this));

		_op_handler.run();
		_service_handler.run();
	}

	template<typename Handler>
	void run(const Handler& handler)
	{
	    _service_handler.add_signal(SIGINT);
		_service_handler.add_signal(SIGTERM);
#   if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGBREAK);
#   endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGFPE);
		_service_handler.add_signal(SIGILL);
		_service_handler.add_signal(SIGABRT);
#	if defined(SIGQUIT)
		_service_handler.add_signal(SIGQUIT);
#	endif // defined(SIGQUIT)
		_service_handler.set_signal_handler(handler);

		_op_handler.run();
		_service_handler.run();
	}

	void stop(void)
	{
		_service_handler.stop();
		_op_handler.stop();
	}

	size_type online_size(void) const
	{
		return _op_handler.online_size();
	}

	void clear_sessions(void)
	{
		_op_handler.clear_sessions();
	}

	void clear(void)
	{
		_op_handler.clear();
	}

	service_handler_type& service_handler(void)
	{
		return _service_handler;
	}

	op_handler_type& op_handler(void)
	{
		return _op_handler;
	}

	void check_state(void)
	{
		_op_handler.check_state();
	}

	template<typename Key, typename NetInfo, typename Data, typename Handler>
	bool send_packet(const Key& key, const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		return _op_handler.send_packet(key, netinfo, data, handler);
	}

	template<typename Key, typename Pak, typename Handler>
	bool send_packet(const Key& key, Pak& pak, const Handler& handler)
	{
		return _op_handler.send_packet(key, pak, handler);
	}

	template<typename Key, typename Pak>
	bool send_packet(const Key& key, Pak& pak)
	{
		return _op_handler.send_packet(key, pak);
	}

	template<typename Key, typename Handler>
	bool access(const Key& key, const Handler& handler)
	{
		return _op_handler.access(key, handler);
	}

	template<typename Key, typename Handler>
	bool access(const Key& key, const Handler& handler) const
	{
		return _op_handler.access(key, handler);
	}

private:

	friend class yggr::system_controller::ctrl_center;
	template<typename Tag, typename Ctrl_Center>
	void register_controller(Ctrl_Center& cc)
	{
		_op_handler.template register_controller<Tag>(cc);
	}

private:
	service_handler_type _service_handler;
	op_handler_type _op_handler;
};

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_NETWORK_HANDLER_HPP__
