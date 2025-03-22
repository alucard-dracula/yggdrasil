//basic_repeater.hpp

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

#ifndef __YGGR_PROXY_DETAIL_BASIC_REPEATER_HPP__
#define __YGGR_PROXY_DETAIL_BASIC_REPEATER_HPP__

#include <yggr/bind/bind.hpp>

#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

namespace yggr
{
namespace proxy
{
namespace detail
{

template<typename Shared_Info_Ptr, typename Router>
struct basic_repeater
	: private nonable::noncopyable,
		private nonable::nonmoveable
		
{
public:
	typedef Shared_Info_Ptr shared_info_ptr_type;
	typedef Router router_type;

protected:
	typedef typename mplex::pointer_to_value_t<shared_info_ptr_type>::type shared_info_type;

	typedef safe_container::safe_wrap<shared_info_type> shared_info_wrap_type;
	typedef typename shared_info_wrap_type::error_type shared_info_wrap_error_type;

	typedef typename router_type::proxy_val_type proxy_id_type;
	typedef safe_container::safe_wrap<router_type> router_wrap_type;
	typedef typename router_wrap_type::value_ptr_type router_ptr_type;
	typedef typename router_wrap_type::error_type router_wrap_error_type;

private:
	typedef basic_repeater this_type;

protected:

	basic_repeater(const shared_info_ptr_type& psinfo, const router_ptr_type& proute)
		: _shared_info_wrap(psinfo), _router_wrap(proute)
	{
	}

	~basic_repeater(void)
	{
	}

public:
	template<typename Repeater_Container> inline
	void register_repeater_data(Repeater_Container& cont)
	{
		shared_info_ptr_type psinfo = _shared_info_wrap.get_shared_ptr();
		if(!psinfo)
		{
			return;
		}

		psinfo->register_calculator(cont);

	}

	template<typename Repeater_Container> inline
	void unregister_repeater_data(Repeater_Container& cont)
	{
		shared_info_ptr_type psinfo = _shared_info_wrap.get_shared_ptr();
		if(!psinfo)
		{
			return;
		}

		psinfo->unregister_calculator(cont);
	}

public:
	inline void clear(void)
	{
		try
		{
			_shared_info_wrap.call_function(&shared_info_type::clear);
		}
		catch(const shared_info_wrap_error_type&)
		{
		}
	}

	inline shared_info_ptr_type get_shared_info(void) const
	{
		return _shared_info_wrap.get_shared_ptr();
	}

	inline router_ptr_type get_router(void) const
	{
		return _router_wrap.get_shared_ptr();
	}

protected:
	shared_info_wrap_type _shared_info_wrap;
	router_wrap_type _router_wrap;
};

} // namespace detail
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_DETAIL_BASIC_REPEATER_HPP__