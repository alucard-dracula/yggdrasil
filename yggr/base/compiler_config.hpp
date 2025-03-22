//compiler_config.hpp

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

#ifndef __YGGR_COMPILER_CONFIG_HPP__
#define __YGGR_COMPILER_CONFIG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

#if defined(_MSC_VER) && (!(_MSC_VER < 1700)) 
#	define _SCL_SECURE_NO_WARNINGS
#endif // _MSC_VER

// auto_ptr

#if !(YGGR_CPP_VERSION < YGGR_CPP_VER_17)
#	if !defined(YGGR_NO_AUTO_PTR)
#		define YGGR_NO_AUTO_PTR
#	endif // YGGR_NO_AUTO_PTR
#endif // #if !(YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#if defined(YGGR_NO_AUTO_PTR) || defined(BOOST_NO_AUTO_PTR)

#	if !defined(BOOST_NO_AUTO_PTR)
#	define BOOST_NO_AUTO_PTR
#	endif // BOOST_NO_AUTO_PTR

#	if !defined(YGGR_NO_AUTO_PTR)
#	define YGGR_NO_AUTO_PTR
#	endif // YGGR_NO_AUTO_PTR

#endif // YGGR_NO_AUTO_PTR

// char and string
#if defined(YGGR_NO_CWCHAR) || defined(BOOST_NO_CWCHAR)

#	if !defined(BOOST_NO_CWCHAR)
#		define BOOST_NO_CWCHAR
#	endif // BOOST_NO_CWCHAR

#	if !defined(YGGR_NO_CWCHAR)
#		define YGGR_NO_CWCHAR
#	endif // YGGR_NO_CWCHAR

#	if !defined(BOOST_NO_STD_WSTRING)
#		define BOOST_NO_STD_WSTRING
#	endif // BOOST_NO_STD_WSTRING

#	if !defined(YGGR_NO_WSTRING)
#		define YGGR_NO_WSTRING
#	endif // YGGR_NO_WSTRING

#endif // defined(YGGR_NO_CWCHAR) || defined(BOOST_NO_CWCHAR)

#if defined(YGGR_NO_WSTRING) || defined(BOOST_NO_STD_WSTRING)

#	if !defined(YGGR_NO_WSTRING)
#		define YGGR_NO_WSTRING
#	endif // YGGR_NO_WSTRING

#endif // defined(YGGR_NO_WSTRING) || defined(BOOST_NO_STD_WSTRING)

#if defined(BOOST_NO_CHAR8_T)
#	if !defined(YGGR_NO_CHAR8_T)
#		define YGGR_NO_CHAR8_T
#		define YGGR_NO_U8STRING_T
#	endif // YGGR_NO_CHAR8_T
#else
#	if defined(__APPLE_CC__) && (__APPLE_CC__ == 6000) && (YGGR_CPP_VERSION == YGGR_CPP_VER_20)
		// fix xcode 16.2, undefined symbol: typeinfo for char8_t
#		if !defined(YGGR_NO_CHAR8_T)
#			define YGGR_NO_CHAR8_T
#			define YGGR_NO_U8STRING_T
#		endif // YGGR_NO_CHAR8_T
#	else
#		if (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
#			define YGGR_NO_CHAR8_T
#			define YGGR_NO_U8STRING_T
#		endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
#	endif // #	if defined(__APPLE_CC__) && (__APPLE_CC__ == 6000) && (YGGR_CPP_VERSION == YGGR_CPP_VER20)
#endif // BOOST_NO_CHAR8_T

#if defined(BOOST_NO_CHAR16_T)
#	if !defined(YGGR_NO_CHAR16_T)
#		define YGGR_NO_CHAR16_T
#		define YGGR_NO_U16STRING_T
#	endif // YGGR_NO_CHAR16_T
#else
#	if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
#		define YGGR_NO_CHAR16_T
#		define YGGR_NO_U16STRING_T
#	endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
#endif // BOOST_NO_CHAR16_T

#if defined(BOOST_NO_CHAR32_T)
#	if !defined(YGGR_NO_CHAR32_T)
#		define YGGR_NO_CHAR32_T
#		define YGGR_NO_U32STRING_T
#	endif // YGGR_NO_CHAR16_T
#else
#	if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
#		define YGGR_NO_CHAR32_T
#		define YGGR_NO_U32STRING_T
#	endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
#endif // BOOST_NO_CHAR32_T

//container
#if defined(BOOST_NO_CXX11_HDR_ARRAY)
#	if !defined(YGGR_NO_CXX11_HDR_ARRAY)
#		define YGGR_NO_CXX11_HDR_ARRAY
#	endif // BOOST_NO_CXX11_HDR_ARRAY
#endif // BOOST_NO_CXX11_HDR_ARRAY

#if defined(BOOST_NO_CXX11_HDR_UNORDERED_SET)
#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
#		define YGGR_NO_CXX11_HDR_UNORDERED_SET
#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
#endif // BOOST_NO_CXX11_HDR_UNORDERED_SET

#if defined(BOOST_NO_CXX11_HDR_UNORDERED_MAP)
#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
#		define YGGR_NO_CXX11_HDR_UNORDERED_MAP
#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
#endif // BOOST_NO_CXX11_HDR_UNORDERED_MAP

#if defined(BOOST_NO_CXX11_STD_UNORDERED)
#	if !defined(YGGR_NO_CXX11_STD_UNORDERED)
#		define YGGR_NO_CXX11_STD_UNORDERED
#	endif // YGGR_NO_CXX11_STD_UNORDERED
#endif // BOOST_NO_CXX11_STD_UNORDERED

// comparer ability

#if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
#	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
#		define YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	if !defined(YGGR_NO_CXX14_VARIADIC_TEMPLATES)
#		define YGGR_NO_CXX14_VARIADIC_TEMPLATES
#	endif // YGGR_NO_CXX14_VARIADIC_TEMPLATES
#endif // BOOST_NO_VARIADIC_TEMPLATES

#if defined(BOOST_NO_CXX14_VARIADIC_TEMPLATES)
#	if !defined(YGGR_NO_CXX14_VARIADIC_TEMPLATES)
#		define YGGR_NO_CXX14_VARIADIC_TEMPLATES
#	endif // YGGR_NO_CXX14_VARIADIC_TEMPLATES
#endif // BOOST_NO_VARIADIC_TEMPLATES

#if defined(BOOST_NO_CXX11_DECLTYPE)
#	if !defined(YGGR_NO_CXX11_DECLTYPE)
#		define YGGR_NO_CXX11_DECLTYPE
#	endif // YGGR_NO_CXX11_DECLTYPE
#else
#	if defined(_MSC_VER) && (_MSC_VER <= 1600)
#		define YGGR_NO_CXX11_DECLTYPE
#	endif //  YGGR_NO_CXX11_DECLTYPE
#endif // BOOST_NO_CXX11_DECLTYPE

#if defined(BOOST_NO_CXX11_VARIADIC_MACROS)
#	if !defined(YGGR_NO_CXX11_VARIADIC_MACROS)
#		define YGGR_NO_CXX11_VARIADIC_MACROS
#	endif // YGGR_NO_CXX11_VARIADIC_MACROS
#endif //BOOST_NO_CXX11_VARIADIC_MACROS

#if defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
#	if !defined(YGGR_NO_CXX11_HDR_INITIALIZER_LIST)
#		define YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#	endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

#if defined(BOOST_NO_CXX11_SMART_PTR)
#	if !defined(YGGR_NO_CXX11_SMART_PTR)
#		define YGGR_NO_CXX11_SMART_PTR
#	endif // YGGR_NO_CXX11_SMART_PTR
#endif // BOOST_NO_CXX11_SMART_PTR

#if !defined(YGGR_NO_CXX11_STD_UNORDERED)
#	define YGGR_HAS_CXX11_STD_HASH
#endif // YGGR_NO_CXX11_STD_UNORDERED

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
#	if (defined(BOOST_NO_CXX11_HDR_TUPLE) \
		||  defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES) \
		|| (YGGR_CPP_VERSION < YGGR_CPP_VER_11) ) 
#		define YGGR_NO_CXX11_HDR_TUPLE
#	endif // (defined(BOOST_NO_CXX11_HDR_TUPLE)
#endif // YGGR_NO_CXX11_HDR_TUPLE

#endif // __YGGR_COMPILER_CONFIG_HPP__
