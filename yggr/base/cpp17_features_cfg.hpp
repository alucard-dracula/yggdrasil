// cpp17_features_cfg.hpp

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

#ifndef __YGGR_BASE_CPP17_FEATURES_CFG_HPP__
#define __YGGR_BASE_CPP17_FEATURES_CFG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

// YGGR_NO_CXX17_HDR_STRING_VIEW
#if !defined(YGGR_NO_CXX17_HDR_STRING_VIEW)
#	if !defined(BOOST_NO_CXX17_HDR_STRING_VIEW)
#		if !(defined(_YVALS) || defined(_CPPLIB_VER))  // msvc std lib already configured
#			if ((YGGR_CPP_VERSION < YGGR_CPP_VER_17) || !defined(__has_include))
#				define YGGR_NO_CXX17_HDR_STRING_VIEW
#			else
#				if !__has_include(<string_view>)
#					define YGGR_NO_CXX17_HDR_STRING_VIEW
#				endif //BOOST_NO_CXX17_HDR_STRING_VIEW
#			endif // (!defined(__has_include) || (__cplusplus < 201700))
#		endif // !defined(_YVALS) && !defined(_CPPLIB_VER)
#	else
#		define YGGR_NO_CXX17_HDR_STRING_VIEW
#	endif //BOOST_NO_CXX17_HDR_STRING_VIEW
#endif // YGGR_NO_CXX17_HDR_STRING_VIEW

#if !defined(YGGR_NO_CXX17_HDR_STRING_VIEW)
#	if !defined(BOOST_NO_CXX17_HDR_STRING_VIEW)
#		if !defined(_CPPLIB_VER) || (_CPPLIB_VER < 650) || !defined(BOOST_MSVC) || (BOOST_MSVC < 1910) || !defined(_HAS_CXX17) || (_HAS_CXX17 == 0)
#			define YGGR_NO_CXX17_HDR_STRING_VIEW
#		endif // !defined(_CPPLIB_VER) || (_CPPLIB_VER < 650) || !defined(BOOST_MSVC) || (BOOST_MSVC < 1910) || !defined(_HAS_CXX17) || (_HAS_CXX17 == 0)
#	else
#		define YGGR_NO_CXX17_HDR_STRING_VIEW
#	endif //BOOST_NO_CXX17_HDR_STRING_VIEW
#endif // YGGR_NO_CXX17_HDR_STRING_VIEW

#if !defined(YGGR_NO_CXX17_HDR_STRING_VIEW)
#	if !defined(BOOST_NO_CXX17_HDR_STRING_VIEW)
#		if (_LIBCPP_VERSION < 4000) || (__cplusplus <= 201402L)
#			define YGGR_NO_CXX17_HDR_STRING_VIEW
#		endif //(_LIBCPP_VERSION < 4000) || (__cplusplus <= 201402L)
#	else
#		define YGGR_NO_CXX17_HDR_STRING_VIEW
#	endif //BOOST_NO_CXX17_HDR_STRING_VIEW
#endif // YGGR_NO_CXX17_HDR_STRING_VIEW

#if !defined(YGGR_NO_CXX17_HDR_STRING_VIEW)
#	if !defined(BOOST_NO_CXX17_HDR_STRING_VIEW)
#		if (BOOST_LIBSTDCXX_VERSION < 70100) || (__cplusplus <= 201402L)
#			define BOOST_NO_CXX17_HDR_STRING_VIEW
#		endif // (BOOST_LIBSTDCXX_VERSION < 70100) || (__cplusplus <= 201402L)
#	else
#		define YGGR_NO_CXX17_HDR_STRING_VIEW
#	endif //BOOST_NO_CXX17_HDR_STRING_VIEW
#endif // YGGR_NO_CXX17_HDR_STRING_VIEW

#if !defined(BOOST_NO_CXX98_FUNCTION_BASE)
#	if defined(_CPPLIB_VER) && (_CPPLIB_VER >= 650)
#		if defined(_HAS_AUTO_PTR_ETC) && (_HAS_AUTO_PTR_ETC == 0)
#			define YGGR_NO_CXX98_FUNCTION_BASE
#		endif // (_HAS_AUTO_PTR_ETC) && (_HAS_AUTO_PTR_ETC == 0)
#	endif
#else
#	define YGGR_NO_CXX98_FUNCTION_BASE
#endif // BOOST_NO_CXX98_FUNCTION_BASE

// append other...

#endif // __YGGR_BASE_CPP17_FEATURES_CFG_HPP__
