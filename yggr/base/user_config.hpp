// user_config.hpp

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

#ifndef __YGGR_BASE_USER_CONFIG_HPP__
#define __YGGR_BASE_USER_CONFIG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

#if (defined(YGGR_NO_CWCHAR)) && (!defined(YGGR_NO_WSTRING))
#	define YGGR_NO_WSTRING
#endif // YGGR_NO_CWCHAR

//#if defined(__MINGW32__)
//
//#	if !defined(YGGR_NO_CHAR8_T)
//#		define YGGR_NO_CHAR8_T
//#	endif // YGGR_NO_CHAR8_T
//
//#	if !defined(YGGR_NO_CHAR16_T)
//#		define YGGR_NO_CHAR16_T
//#	endif // YGGR_NO_CHAR16_T
//
//#	if !defined(YGGR_NO_CHAR32_T)
//#		define YGGR_NO_CHAR32_T
//#	endif // YGGR_NO_CHAR32_T
//
//#endif // __MINGW32__

// BOOST_HAS_ICU defined
//#if !defined(__MINGW32__)
////#	define BOOST_HAS_ICU
//#endif // __MINGW32__

// fix boost::unordered_map try_emplace hint version succeed not move key
# define YGGR_FIX_BOOST_UNORDERED_TRY_EMPLACE_HINT_NOT_MOVE_KEY 

// apppend you configure

//#ifndef YGGR_USE_UTF8_EX_MODE
//#	define YGGR_USE_UTF8_EX_MODE
//#endif // YGGR_USE_UTF8_EX_MODE

//#ifndef YGGR_HEX_CONV_DEFAULT_MODE
//#	define YGGR_HEX_CONV_DEFAULT_MODE 0
//#endif // YGGR_HEX_CONV_DEFAULT_MODE

//#if !defined(YGGR_LUA_THREAD_SAFE)
//#	define YGGR_LUA_THREAD_SAFE
//#endif // YGGR_LUA_THREAD_SAFE

#endif // __YGGR_BASE_USER_CONFIG_HPP__

