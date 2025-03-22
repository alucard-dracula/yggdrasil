//for_each_adapter.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_FOR_EACH_ADAPTER_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_FOR_EACH_ADAPTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Handler>
struct for_each_adapter
{
public:
	typedef Handler handler_type;
	typedef func::foo_t_info<handler_type> foo_t_info_type;
	typedef typename foo_t_info_type::result_type result_type;

private:
	typedef for_each_adapter this_type;

public:
	for_each_adapter(const handler_type& handler)
		: _handler(handler)
	{
	}

public:
	template<typename Iter> inline
	result_type operator()(Iter& iter) const
	{
		return (_handler)(*iter);
	}

private:
	const handler_type& _handler;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_FOR_EACH_ADAPTER_HPP__
