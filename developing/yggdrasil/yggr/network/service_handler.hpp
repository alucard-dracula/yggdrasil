//service_handler.hpp

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

#ifndef __YGGR_NETWORK_BASIC_NETWORK_HANDLER_HPP__
#define __YGGR_NETWORK_BASIC_NETWORK_HANDLER_HPP__

#include <vector>

#include <boost/asio.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/nonable.hpp>
#include <yggr/network/io_service_pool.hpp>

namespace yggr
{
namespace network
{

template<	typename Service,
			typename SignalSet = boost::asio::signal_set,
			typename Thread_Config = thread::boost_thread_config_type,
			template<typename _PTy> 
				class Allocator = std::allocator,
			template<typename _PTy, typename _PAx> 
				class Saver = std::vector,
			template<typename _Saver> 
				class Selector = network::normal_io_service_selector
		>
class service_handler
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef SignalSet signal_set_type;
	typedef Service service_type;
	typedef Thread_Config thread_config_type;

	typedef network::io_service_pool<service_type, 
										thread_config_type,
										Allocator, Saver,
										Selector> io_service_pool_type;

	typedef size_type init_type;

private:
	typedef service_handler this_type;

public:
	//service_handler(void)
	//	: _service_pool(1),
	//		_sig_set(_service_pool.get_service())
	//{
	//	//BOOST_MPL_ASSERT((boost::mpl::false_));
	//	assert(false);
	//}

	service_handler(const size_type& size)
		: _service_pool(size),
			_sig_set(_service_pool.get_service())
	{
	}

	~service_handler(void)
	{
		assert(!_service_pool.is_running());
	}

	io_service_pool_type& service_pool(void)
	{
		return _service_pool;
	}

	signal_set_type& signal_set(void)
	{
		return _sig_set;
	}

	service_type& get_service(void)
	{
		return _service_pool.get_service();
	}

	void add_signal(u32 sig)
	{
		_sig_set.add(sig);
	}

	void join(void)
	{
		_service_pool.join();
	}

	template<typename Handler>
	void set_signal_handler(const Handler& handler)
	{
		_sig_set.async_wait(handler);
	}

	bool is_running(void) const
	{
		return _service_pool.is_running();
	}

	void run(void)
	{
		_service_pool.run();
	}

	void stop(void)
	{
		_service_pool.stop();
	}
private:
	io_service_pool_type _service_pool; //must befor _acceptor
	signal_set_type _sig_set;
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_BASIC_NETWORK_HANDLER_HPP__