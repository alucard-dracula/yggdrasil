//proxy_netdata_def.hpp

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

#ifndef __YGGR_PROXY_PROXY_MSG_PROXY_NETDATA_DEF_HPP__
#define __YGGR_PROXY_PROXY_MSG_PROXY_NETDATA_DEF_HPP__

#include <yggr/ppex/range_checker.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_msg
{

struct proxy_netdata_def
{
	YGGR_STATIC_CONSTANT(u32, E_id_start = 0x00008200);
	YGGR_STATIC_CONSTANT(u32, E_id_end = 0x00008300);
};

} // namespace proxy_msg
} // namespace proxy
} // namespace yggr

#define YGGR_PP_CHECK_PROXY_MSG_ID( __id__ ) \
	YGGR_PP_IS_IN_RANGE(__id__, \
							::yggr::proxy::proxy_msg::proxy_netdata_def::E_id_start, \
							::yggr::proxy::proxy_msg::proxy_netdata_def::E_id_end )

#endif //__YGGR_PROXY_PROXY_MSG_PROXY_NETDATA_DEF_HPP__