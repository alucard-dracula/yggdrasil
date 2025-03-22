// if_constexpr_cfg.hpp

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

#ifndef __YGGR_BASE_IF_CONSTEXPR_CFG_HPP__
#define __YGGR_BASE_IF_CONSTEXPR_CFG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/base/yggr_cpp_ver_config.hpp>

#if (YGGR_CPP_VERSION > YGGR_CPP_VER_14) && !defined(BOOST_NO_CXX17_IF_CONSTEXPR)

#	define YGGR_IF_CONSTEXPR if constexpr
#	define YGGR_CPP17_IF_CONSTEXPR if constexpr

#else

#	define YGGR_IF_CONSTEXPR if
#	define YGGR_CPP17_IF_CONSTEXPR if

#	endif //BOOST_NO_CXX17_IF_CONSTEXPR

#if (YGGR_CPP_VERSION > YGGR_CPP_VER_17)
#	define YGGR_CPP20_IF_CONSTEXPR YGGR_IF_CONSTEXPR
#else
#	define YGGR_CPP20_IF_CONSTEXPR if
#endif // YGGR_CPP_VERSION > YGGR_CPP_VER_17

#if (YGGR_CPP_VERSION > YGGR_CPP_VER_20)
#	define YGGR_CPP23_IF_CONSTEXPR YGGR_IF_CONSTEXPR
#else
#	define YGGR_CPP23_IF_CONSTEXPR if
#endif // YGGR_CPP_VERSION > YGGR_CPP_VER_20

#endif // __YGGR_BASE_IF_CONSTEXPR_CFG_HPP__
