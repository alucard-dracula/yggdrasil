//buffer_real_value_def.hpp

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

#ifndef __YGGR_ARCHIVE_BUFFER_REAL_VALUE_DEF_HPP__
#define __YGGR_ARCHIVE_BUFFER_REAL_VALUE_DEF_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/archive/buffer_real_value_type_traits.hpp>

#include <boost/mpl/if.hpp>

YGGR_ARCHIVE_BUFFER_REAL_VALUE_DEF(char, yggr::u8);

namespace yggr
{
namespace archive
{
namespace detail
{
	typedef
		boost::mpl::if_c
		<
			(sizeof(wchar_t) == 2),
			yggr::u16,
			yggr::u32 
		>::type wchar_value_type;

} //namespace detail
} // namespace archive
} // namespace yggr

YGGR_ARCHIVE_BUFFER_REAL_VALUE_DEF(wchar_t, yggr::archive::detail::wchar_value_type)

#endif // __YGGR_ARCHIVE_BUFFER_REAL_VALUE_DEF_HPP__