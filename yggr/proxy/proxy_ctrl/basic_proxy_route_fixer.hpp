// basic_proxy_route_fixer.hpp

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

#ifndef __YGGR_PROXY_PROXY_CTRL_BASIC_PROXY_ROUTE_FIXER_HPP__
#define __YGGR_PROXY_PROXY_CTRL_BASIC_PROXY_ROUTE_FIXER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/ref_count_info.h>
#include <yggr/func/foo_t_info.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <boost/smart_ptr/enable_shared_from_raw.hpp>
#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_ctrl
{

template<typename ProxyRepeater>
class basic_proxy_route_fixer
	: public boost::enable_shared_from_raw
{
public:
	typedef ProxyRepeater proxy_repeater_type;

	typedef yggr::safe_container::safe_wrap<proxy_repeater_type> proxy_repeater_wrap_type;
	typedef typename proxy_repeater_wrap_type::value_ptr_type proxy_repeater_ptr_type;

	typedef typename proxy_repeater_type::router_type proxy_router_type;
	typedef typename proxy_repeater_type::router_ptr_type proxy_router_ptr_type;

protected:
	typedef yggr::ref_count_info<> ref_count_info_type;
	typedef yggr::ref_count_info_locker<ref_count_info_type> ref_count_info_locker_type;

private:
	typedef basic_proxy_route_fixer this_type;

public:
	basic_proxy_route_fixer(const proxy_repeater_ptr_type& prepeater)
		:  _repeater_wrap(prepeater)
	{
	}

	~basic_proxy_route_fixer(void)
	{
	}

public:
	void join(void)
	{
		for(;!_count.empty(););
	}

protected:

	inline proxy_repeater_ptr_type get_proxy_repeater(void) const
	{
		ref_count_info_locker_type lk(_count);
		return _repeater_wrap.get_shared_ptr();
	}

protected:
	template<typename RealData, typename Fix_Handler> inline
	void register_repeater_data(const Fix_Handler& fix_handler)
	{
		typedef RealData real_data_type;

		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->template register_repeater_data<real_data_type>(fix_handler);
		}
	}

	template<typename RealData, typename Fix_Handler> inline
	void register_repeater_special_data(const Fix_Handler& fix_handler)
	{
		typedef RealData real_data_type;

		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->template register_repeater_special_data<real_data_type>(fix_handler);
		}
	}

	template<typename DataInfo, typename Fix_Handler> inline
	typename 
		boost::enable_if
		<
			typename func::foo_t_info<Fix_Handler>::is_callable_type,
			void
		>::type
		register_repeater_data(const DataInfo& info, const Fix_Handler& fix_handler)
	{
		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->register_repeater_data(info, fix_handler);
		}
	}

	template<typename DataInfo> inline
	void register_repeater_data(const DataInfo& info, u32 cal_t)
	{
		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->register_repeater_data(info, cal_t);
		}
	}


#ifdef YGGR_USE_SEH

	template<typename RealData, typename Fix_Handler, typename Seh_Handler> inline
	void register_repeater_data(const Fix_Handler& fix_handler, const Seh_Handler& seh_handler)
	{
		typedef RealData real_data_type;

		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->template register_repeater_data<real_data_type>(fix_handler, seh_handler);
		}
	}
	
	template<typename RealData, typename Fix_Handler, typename Seh_Handler> inline
	void register_repeater_special_data(const Fix_Handler& fix_handler, const Seh_Handler& seh_handler)
	{
		typedef RealData real_data_type;

		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->template register_repeater_special_data<real_data_type>(fix_handler, seh_handler);
		}
	}

	template<typename DataInfo, typename Fix_Handler, typename Seh_Handler> inline
	typename 
		boost::enable_if
		<
			typename func::foo_t_info<Fix_Handler>::is_callable_type,
			void
		>::type
		register_repeater_data(const DataInfo& info, const Fix_Handler& fix_handler, const Seh_Handler& seh_handler)
	{
		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->register_repeater_data(info, fix_handler, seh_handler);
		}
	}

	template<typename DataInfo, typename Seh_Handler> inline
	void register_repeater_data(const DataInfo& info, u32 cal_t, const Seh_Handler& seh_handler)
	{
		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->register_repeater_data(info, cal_t, seh_handler);
		}
	}

#endif // YGGR_USE_SEH

	template<typename RealData> inline
	void unregister_repeater_data(void)
	{
		typedef RealData real_data_type;

		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->template unregister_repeater_data<real_data_type>();
		}
	}

	template<typename RealData> inline
	void unregister_repeater_special_data(void)
	{
		typedef RealData real_data_type;

		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->template unregister_repeater_special_data<real_data_type>();
		}
	}

	template<typename DataInfo> inline
	void unregister_repeater_data(const DataInfo& info)
	{
		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->unregister_repeater_data(info);
		}
	}

protected:
	template<typename RealData> inline
	void append_special_data(void)
	{
		typedef RealData real_data_type;

		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->template append_special_data<real_data_type>();
		}
	}

	template<typename DataInfo> inline
	void append_special_data(const DataInfo& info)
	{
		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->append_special_data(info);
		}
	}

	template<typename RealData> inline
	void remove_special_data(void)
	{
		typedef RealData real_data_type;

		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->template remove_special_data<real_data_type>();
		}
	}

	template<typename DataInfo> inline
	void remove_special_data(const DataInfo& info)
	{
		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->remove_special_data(info);
		}
	}

protected:
	template<typename Owner_Info, typename Net_IPak, typename Runner, typename Handler> inline
	void route(const Owner_Info& owner,
				const Net_IPak& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type) const
	{
		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->route(owner, proxy_data, prunner, handler, cal_type);
		}
	}

	template<typename Owner_Info, typename Net_IPak, typename Runner, typename Handler, typename Args> inline
	void route(const Owner_Info& owner,
				const Net_IPak& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type,
				Args& args) const
	{
		ref_count_info_locker_type lk(_count);

		proxy_repeater_ptr_type prepeater = _repeater_wrap.get_shared_ptr();
		if(prepeater)
		{
			prepeater->route(owner, proxy_data, prunner, handler, cal_type, args);
		}
	}

protected:
	mutable ref_count_info_type _count;
	proxy_repeater_wrap_type _repeater_wrap;

};

} // namespace proxy_ctrl
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_CTRL_BASIC_PROXY_ROUTE_FIXER_HPP__


