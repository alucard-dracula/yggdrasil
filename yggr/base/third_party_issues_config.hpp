//third_party_issuses_config.hpp

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

#ifndef __YGGR_THIRD_PARTY_ISSUES_CONFIG_HPP__
#define __YGGR_THIRD_PARTY_ISSUES_CONFIG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! fix old version boost( <= 1.53) interprocess						!!!
!!! win32_api.hpp get_wmi_class_attribute IWbemClassObject_BIPC::Get	!!!
!!! wchar_variant size													!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

#ifndef YGGR_BOOST_OLD_VERSION_COMPATIBILITY_FIXED
#	define YGGR_BOOST_OLD_VERSION_COMPATIBILITY_FIXED
#endif // YGGR_BOOST_OLD_VERSION_COMPATIBILITY_FIXED

#if YGGR_CPP_VERSION < YGGR_CPP_VER_11
// cpp 98-03
#elif YGGR_CPP_VERSION < YGGR_CPP_VER_14
// cpp 11
#elif YGGR_CPP_VERSION < YGGR_CPP_VER_17
// cpp 14
#elif YGGR_CPP_VERSION < YGGR_CPP_VER_20
// cpp 17
#	if (BOOST_VERSION < 106400)
//#		error "yggr panic: boost 1.64 and older not support cpp17 and later, see boost/functional/hash.hpp and std::unary_function"
#	endif // BOOST_VERSION < 106400
#elif YGGR_CPP_VERSION < YGGR_CPP_VER_23
// cpp 20
#else

#endif // YGGR_CPP_VERSION < YGGR_CPP_VER_11

#endif // __YGGR_THIRD_PARTY_ISSUES_CONFIG_HPP__
