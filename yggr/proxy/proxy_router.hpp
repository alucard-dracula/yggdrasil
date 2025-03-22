//proxy_router.hpp

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

#ifndef __YGGR_PROXY_PROXY_ROUTER_HPP__
#define __YGGR_PROXY_PROXY_ROUTER_HPP__

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

namespace yggr
{
namespace proxy
{

template<typename Dispatcher>
class proxy_router
	: public Dispatcher
{

public:
	typedef Dispatcher base_type;
	typedef typename base_type::task_type task_type;
	typedef typename base_type::owner_info_type owner_info_type;
	typedef typename base_type::id_type dispatch_id_type;

private:
	typedef proxy_router this_type;

public:
	proxy_router(void)
	{
	}

	~proxy_router(void)
	{
	}

public:
	template<typename Runner, typename Handler> inline
	void route_to_proxy(const owner_info_type& owner,
						const task_type& proxy_data,
						Runner* prunner,
						const Handler& handler,
						u32 cal_type) const
	{
		base_type::send_to_proxy(owner, proxy_data, prunner, handler, cal_type);
	}

	template<typename Runner, typename Handler, typename Args> inline
	void route_to_proxy(const owner_info_type& owner,
						const task_type& proxy_data,
						Runner* prunner,
						const Handler& handler,
						u32 cal_type,
						Args& args) const
	{
		base_type::send_to_proxy(owner, proxy_data, prunner, handler, cal_type, args);
	}

	template<typename Runner, typename Handler> inline
	void route_to_back(const owner_info_type& owner,
						const task_type& proxy_data,
						Runner* prunner,
						const Handler& handler,
						u32 cal_type) const
	{
		base_type::send_to_back(owner, proxy_data, prunner, handler, cal_type);
	}

	template<typename Runner, typename Handler> inline
	void route(const owner_info_type& owner,
				const task_type& proxy_data,
				Runner* prunner,
				const Handler& handler,
				u32 cal_type) const
	{
		base_type::send(owner, proxy_data, prunner, handler, cal_type);
	}

};

} // namespace proxy
} // namespace yggr

#endif //__YGGR_PROXY_PROXY_ROUTER_HPP__