// yggr_cpp_ver_config.hpp

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

#ifndef __YGGR_BASE_YGGR_CPP_VER_CONFIG_HPP__
#define __YGGR_BASE_YGGR_CPP_VER_CONFIG_HPP__

#include <boost/config/pragma_message.hpp>

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

#if YGGR_CPP_VER_14 < YGGR_CPP_VERSION // cpp17
#	if BOOST_VERSION < 106600
#		error "boost 1.53.0 - boost 1.65.1 not support cpp17 and later"
#	endif //BOOST_VERSION < 106600
#endif // YGGR_CPP_VER_14 < YGGR_CPP_VERSION

#if YGGR_CPP_VER_17 < YGGR_CPP_VERSION // cpp20
#	if BOOST_VERSION < 108200
		BOOST_PRAGMA_MESSAGE("boost 1.66.0 - boost 1.81.0 not recommend using cpp20")
#	endif // BOOST_VERSION < 108200
#endif // #if YGGR_CPP_VER_14 < YGGR_CPP_VERSION

#if YGGR_CPP_VER_20 < YGGR_CPP_VERSION // cpp23
#	error "yggdrail has not been tested for cpp23 support"
#endif // YGGR_CPP_VER_20 < YGGR_CPP_VERSION // cpp23

#endif // __YGGR_BASE_YGGR_CPP_VER_CONFIG_HPP__
