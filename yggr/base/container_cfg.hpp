// container_cfg.hpp

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

#ifndef __YGGR_BASE_CONTAINER_CFG_HPP__
#define __YGGR_BASE_CONTAINER_CFG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

#if defined(_MSC_VER) && (_MSC_VER < 1800)

#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
#		define YGGR_NO_CXX11_HDR_UNORDERED_SET
#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
#		define YGGR_NO_CXX11_HDR_UNORDERED_MAP
#	endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

#	if !defined(YGGR_NO_CXX11_STD_UNORDERED)
#		define YGGR_NO_CXX11_STD_UNORDERED
#	endif // YGGR_NO_CXX11_STD_UNORDERED

#endif // _MSC_VER

#if !(BOOST_VERSION < 106200)
// #define YGGR_CONTAINER_DISABLE_FORCEINLINE
// #define YGGR_CONTAINER_FORCEINLINE_IS_BOOST_FORCELINE
#	if !defined(YGGR_CONTAINER_DISABLE_FORCEINLINE)
#		if defined(YGGR_CONTAINER_DISABLE_FORCEINLINE)
#			define YGGR_CONTAINER_FORCEINLINE inline
#		elif	defined(YGGR_CONTAINER_FORCEINLINE_IS_BOOST_FORCELINE)
#			if defined(BOOST_FORCEINLINE)
#				define YGGR_CONTAINER_FORCEINLINE BOOST_FORCEINLINE
#			else
#				define YGGR_CONTAINER_FORCEINLINE
#			endif // BOOST_FORCEINLINE
#		elif defined(BOOST_MSVC) && (_MSC_VER <= 1900 || defined(_DEBUG))
			//"__forceinline" and MSVC seems to have some bugs in old versions and in debug mode
#			define YGGR_CONTAINER_FORCEINLINE inline
#		elif defined(BOOST_GCC) && ((__GNUC__ <= 5) || defined(__MINGW32__))
			//Older GCCs and MinGw have problems with forceinline
#			define YGGR_CONTAINER_FORCEINLINE inline
#		else
#			if defined(BOOST_FORCEINLINE)
#				define YGGR_CONTAINER_FORCEINLINE BOOST_FORCEINLINE
#			else
#				define YGGR_CONTAINER_FORCEINLINE
#			endif // BOOST_FORCEINLINE
#		endif // YGGR_CONTAINER_DISABLE_FORCEINLINE
#	endif //  YGGR_CONTAINER_FORCEINLINE

#	define YGGR_CONTAINER_CONSTRUCT_FORCEINLINE YGGR_CONTAINER_FORCEINLINE

//#define YGGR_CONTAINER_DISABLE_NOINLINE

#	if defined(YGGR_CONTAINER_DISABLE_NOINLINE)
#		define YGGR_CONTAINER_NOINLINE
#	else
#		if defined(BOOST_FORCEINLINE)
#			define YGGR_CONTAINER_NOINLINE BOOST_FORCEINLINE
#		else
#			define YGGR_CONTAINER_NOINLINE
#		endif // BOOST_FORCEINLINE
#	endif // __YGGR_BASE_CONTAINER_CFG_HPP__

#else

#	define YGGR_CONTAINER_FORCEINLINE
#	define YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
#	define YGGR_CONTAINER_NOINLINE

#endif // !(BOOST_VERSION < 106200)

#endif // __YGGR_BASE_CONTAINER_CFG_HPP__

