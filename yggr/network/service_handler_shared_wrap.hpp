//service_handler_shared_wrap.hpp

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

#ifndef __YGGR_NETWORK_SERVICE_HANDLER_SHARED_WRAP_HPP__
#define __YGGR_NETWORK_SERVICE_HANDLER_SHARED_WRAP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <cassert>

namespace yggr
{
namespace network
{

template<typename Service_Handler>
class service_handler_shared_wrap
{
public:
	typedef Service_Handler service_handler_type;
	typedef ::yggr::shared_ptr<service_handler_type> service_handler_ptr_type;
	typedef service_handler_ptr_type init_type;

	typedef typename service_handler_type::signal_set_type signal_set_type;
	typedef typename service_handler_type::service_type service_type;
	typedef typename service_handler_type::io_service_pool_type io_service_pool_type;
	
private:
	typedef service_handler_shared_wrap this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	service_handler_shared_wrap(const service_handler_ptr_type& psh)
		: _psh(psh)
	{
		assert(_psh);
	}

	service_handler_shared_wrap(BOOST_RV_REF(this_type) right)
		: _psh(boost::move(right._psh))
	{
		assert(_psh);
	}

	service_handler_shared_wrap(const this_type& right)
		: _psh(right._psh)
	{
		assert(_psh);
	}

	~service_handler_shared_wrap(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_psh, boost::move(right._psh));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_psh = right._psh;

		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		::yggr::swap(_psh, right._psh);
	}

public:
	inline io_service_pool_type& service_pool(void)
	{
		assert(_psh);
		return _psh->service_pool();
	}

	inline signal_set_type& signal_set(void)
	{
		assert(_psh);
		return _psh->signal_set();
	}

	inline service_type& get_service(void)
	{
		assert(_psh);
		return _psh->get_service();
	}

	inline bool add_signal(u32 sig)
	{
		assert(_psh);
		return _psh->add_signal(sig);
	}

	inline bool remove_signal(u32 sig)
	{
		assert(_psh);
		return _psh->remove_signal(sig);
	}

	inline void clear_signal(void)
	{
		assert(_psh);
		return _psh->clear_signal();
	}

	inline void join(void)
	{
		assert(_psh);
		_psh->join();
	}

	template<typename Handler> inline
	void set_signal_handler(const Handler& handler)
	{
		assert(_psh);
		_psh->set_signal_handler(handler);
	}

	inline bool cancel_signal_handler(void)
	{
		assert(_psh);
		return _psh->cancel_signal_handler();
	}

	inline bool is_running(void) const
	{
		assert(_psh);
		return _psh->is_running();
	}

	inline void run(void)
	{
		assert(_psh);
		_psh->run();
	}

	inline void stop(void)
	{
		assert(_psh);
		_psh->stop();
	}

private:
	service_handler_ptr_type _psh;
};

} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{
namespace swap_support
{

template<typename Service_Handler> 
void swap(service_handler_shared_wrap<Service_Handler>& l, 
			service_handler_shared_wrap<Service_Handler>& r) 
{ 
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::swap_support::swap;
} // namespace boost

#endif // __YGGR_NETWORK_SERVICE_HANDLER_SHARED_WRAP_HPP__
