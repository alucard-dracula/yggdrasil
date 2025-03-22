// is_sizeof_char_1_byte.hpp

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

#ifndef __YGGR_ARCHIVE_DETAIL_IS_SIZEOF_CHAR_1_BYTE_HPP__
#define __YGGR_ARCHIVE_DETAIL_IS_SIZEOF_CHAR_1_BYTE_HPP__

#include <yggr/base/yggrdef.h>

// fix binary_archive "Schrodinger's template instantiation"
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace archive
{
namespace detail
{

template<typename Char, std::size_t N = sizeof(Char)>
struct is_sizeof_char_1_byte_helper
	: public boost::mpl::false_
{
};

template<typename Char>
struct is_sizeof_char_1_byte_helper<Char, 1>
	: public boost::mpl::true_
{
};

template<typename Char>
struct is_sizeof_char_1_byte
	: public is_sizeof_char_1_byte_helper<Char>
{
};

} // namespace detail
} // namespace archive
} // namespace yggr

#endif // __YGGR_ARCHIVE_DETAIL_IS_SIZEOF_CHAR_1_BYTE_HPP__

