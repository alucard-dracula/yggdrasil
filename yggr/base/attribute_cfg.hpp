// attribute_cfg.hpp

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

#ifndef __YGGR_BASE_ATTRIBUTE_CFG_HPP__
#define __YGGR_BASE_ATTRIBUTE_CFG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <boost/detail/workaround.hpp>

#if defined(BOOST_ATTRIBUTE_UNUSED)
#	define YGGR_ATTRIBUTE_UNUSED
#endif // BOOST_ATTRIBUTE_UNUSED

#if !defined(BOOST_ATTRIBUTE_NODISCARD)
#	define YGGR_ATTRIBUTE_NODISCARD
#else
#	if BOOST_WORKAROUND(BOOST_GCC_VERSION, >=70000) && __cplusplus < 201103L
#		define YGGR_ATTRIBUTE_NODISCARD //https://github.com/boostorg/config/issues/336
#	else
#		define YGGR_ATTRIBUTE_NODISCARD BOOST_ATTRIBUTE_NODISCARD
#	endif // BOOST_WORKAROUND(BOOST_GCC_VERSION,>=70000) && __cplusplus < 201103L
#endif //  //BOOST_ATTRIBUTE_NODISCARD

#if !defined(BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS)
#	define YGGR_ATTRIBUTE_NO_UNIQUE_ADDRESS
#else
#	define YGGR_ATTRIBUTE_NO_UNIQUE_ADDRESS BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
#endif //  //BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS

#endif // __YGGR_BASE_ATTRIBUTE_CFG_HPP__

