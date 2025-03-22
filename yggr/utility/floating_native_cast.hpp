//floating_native_cast.hpp

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

#ifndef __YGGR_UTILITY_FLOATING_NATIVE_CAST_HPP__
#define __YGGR_UTILITY_FLOATING_NATIVE_CAST_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/number_t.hpp>

#include <boost/ref.hpp>
#include <boost/enable_if.hpp>
#include <boost/type_traits/is_floating_point.hpp>

namespace yggr
{
namespace utility
{

template<typename T> inline
typename 
	boost::enable_if
	<
		is_floating_point<T>,
		const YGGR_PP_UINTEGER_T_TYPE(sizeof(T))&
	>::type
	floating_native_cast(const T& t)
{
	typedef YGGR_PP_UINTEGER_T_TYPE(sizeof(T)) ret_type;
	return *(reinterpret_cast<const ret_type*>(boost::addressof(t)));
}

template<typename T> inline
typename 
	boost::enable_if
	<
		is_floating_point<T>,
		YGGR_PP_UINTEGER_T_TYPE(sizeof(T))&
	>::type
	floating_native_cast(T& t)
{
	typedef YGGR_PP_UINTEGER_T_TYPE(sizeof(T)) ret_type;
	return *(reinterpret_cast<ret_type*>(boost::addressof(t)));
}


} // namespace utility
} // namespace yggr

#endif // __YGGR_UTILITY_FLOATING_NATIVE_CAST_HPP__