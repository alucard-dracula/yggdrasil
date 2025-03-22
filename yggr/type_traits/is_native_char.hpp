//is_native_char

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

#ifndef __YGGR_TYPE_TRAITS_IS_NATIVE_CHAR__
#define __YGGR_TYPE_TRAITS_IS_NATIVE_CHAR__

#include <yggr/base/yggrdef.h>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>

#include <boost/type_traits/is_same.hpp>

#include <cstddef> //char16_t, char32_t

namespace yggr
{

//template<typename T>
//struct is_native_char
//	: public
//		boost::mpl::bool_
//		<
//			boost::mpl::or_
//			<
//				boost::is_same<T, char>
//#ifndef YGGR_NO_CWCHAR
//				,boost::is_same<T, wchar_t>
//#endif // YGGR_NO_CWCHAR
//
//#ifndef YGGR_NO_CHAR8_T
//				,boost::is_same<T, ::yggr::c8>
//#endif // YGGR_NO_CHAR8_T
//
//#ifndef YGGR_NO_CHAR16_T
//				,boost::is_same<T, ::yggr::c16>
//#endif // YGGR_NO_CHAR16_T
//
//#ifndef YGGR_NO_CHAR32_T
//				,boost::is_same<T, ::yggr::c32>
//#endif // YGGR_NO_CHAR32_T
//			>::type::value
//		>
//{
//};

template<typename T>
struct is_native_char
	: public
		boost::mpl::bool_
		<
			((boost::is_same<T, char>::value)

#ifndef YGGR_NO_CWCHAR
			|| (boost::is_same<T, wchar_t>::value)
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
			|| (boost::is_same<T, ::yggr::c8>::value)
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
			|| (boost::is_same<T, ::yggr::c16>::value)
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
			|| (boost::is_same<T, ::yggr::c32>::value)
#endif // YGGR_NO_CHAR32_T
			 )
		>
{
};

} // namespace yggr

#endif // __YGGR_TYPE_TRAITS_IS_NATIVE_CHAR__
