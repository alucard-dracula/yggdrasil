//tool_buf_size.hpp

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

#ifndef __YGGR_ENCRYPTION_TOOL_TOOL_BUF_SIZE_HPP__
#define __YGGR_ENCRYPTION_TOOL_TOOL_BUF_SIZE_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/mpl/integral_c.hpp>

namespace yggr
{
namespace encryption_tool
{

template<typename Tool>
struct tool_buf_size
	: public boost::mpl::integral_c<u32, 0>
{
};

} // naemspace encryption_tool
} // namespace yggr

#define YGGR_ENCRYPTION_TOOL_TOOL_BUF_SIZE(__type__, __size__) \
	namespace yggr { namespace encryption_tool { \
		template<> struct tool_buf_size< __type__ > \
			: public boost::mpl::integral_c< u32, static_cast<u32>( __size__ ) > { }; }}

#define YGGR_ENCRYPTION_TOOL_DEFED_TOOL_BUF_SIZE(__type__, __def__) \
	namespace yggr { namespace encryption_tool { \
		template<> struct tool_buf_size< __type__ > \
			: public boost::mpl::integral_c<u32, static_cast<u32>( __type__::__def__ ) > { }; }}

#endif // __YGGR_ENCRYPTION_TOOL_TOOL_BUF_SIZE_HPP__