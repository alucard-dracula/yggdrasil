//basic_t_sizeof_log2.hpp

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

#ifndef __YGGR_MPLEX_BASIC_T_SIZEOF_LOG2_HPP__
#define __YGGR_MPLEX_BASIC_T_SIZEOF_LOG2_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/is_enum.hpp>
#include <yggr/mplex/integer_log2.hpp>

#include <boost/type_traits/is_arithmetic.hpp>

namespace yggr
{
namespace mplex
{
namespace detail
{

typedef interger_log2_failed basic_t_sizeof_log2_failed;

template< typename T, 
			bool checkable = 
				boost::mpl::or_
				<
					boost::is_arithmetic<T>,
					::yggr::is_enum<T>
				>::type::value >
struct basic_t_sizeof_log2_detail;

template<typename T>
struct basic_t_sizeof_log2_detail<T, false>
	: public basic_t_sizeof_log2_failed
{
};

template<typename T>
struct basic_t_sizeof_log2_detail<T, true>
	: public integer_log2<sizeof(T)>::type
{
};

} // namespace detail

using detail::basic_t_sizeof_log2_failed;

template<typename T>
struct basic_t_sizeof_log2
	: public detail::basic_t_sizeof_log2_detail<T>::type
{
};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_BASIC_T_SIZEOF_LOG2_HPP__