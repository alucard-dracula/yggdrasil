//service_handler.hpp

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

#ifndef __YGGR_NETWORK_BASIC_NETWORK_HANDLER_HPP__
#define __YGGR_NETWORK_BASIC_NETWORK_HANDLER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/container/default_allocator.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/network/io_service_pool.hpp>

#include <boost/asio.hpp>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace network
{

template<	typename Service,
			typename SignalSet = boost::asio::signal_set,
			typename Thread_Config = thread::boost_thread_config_type,
			template<typename _PTy> 
				class Allocator = YGGR_CONTAINER_DEFAULT_ALLOCATOR(),
			template<typename _PTy, typename _PAx> 
				class Saver = container::vector,
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
	service_handler(const size_type& size)
		: _service_pool(size),
			_sig_set(_service_pool.get_service())
	{
	}

	~service_handler(void)
	{
		assert(!_service_pool.is_running());
	}

public:
	inline io_service_pool_type& service_pool(void)
	{
		return _service_pool;
	}

	inline signal_set_type& signal_set(void)
	{
		return _sig_set;
	}

	inline service_type& get_service(void)
	{
		return _service_pool.get_service();
	}

	inline bool add_signal(u32 sig)
	{
		boost::system::error_code e;
		_sig_set.add(sig, e);
		return !e;
	}

	inline bool remove_signal(u32 sig)
	{
		boost::system::error_code e;
		_sig_set.remove(sig, e);
		return !e;
	}

	inline void clear_signal(void)
	{
		_sig_set.clear();
	}

	inline void join(void)
	{
		_service_pool.join();
	}

	template<typename Handler> inline
	void set_signal_handler(const Handler& handler)
	{
		_sig_set.async_wait(handler);
	}

	inline bool cancel_signal_handler(void)
	{
		boost::system::error_code e;
		_sig_set.cancel(e);
		return !e;
	}

	inline bool is_running(void) const
	{
		return _service_pool.is_running();
	}

	inline void run(void)
	{
		_service_pool.run();
	}

	inline void stop(void)
	{
		_service_pool.stop();
	}
private:
	io_service_pool_type _service_pool; //must before _acceptor
	signal_set_type _sig_set;
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_BASIC_NETWORK_HANDLER_HPP__