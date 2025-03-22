// noexcept_cfg.hpp

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

#ifndef __YGGR_BASE_NOEXCEPT_CFG_HPP__
#define __YGGR_BASE_NOEXCEPT_CFG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/base/yggr_cpp_ver_config.hpp>

#if	defined(BOOST_NO_CXX11_NOEXCEPT)
#	if !defined(YGGR_NO_CXX11_NOEXCEPT)  
#		define YGGR_NO_CXX11_NOEXCEPT
#	endif // YGGR_NO_CXX11_NOEXCEPT
#	define YGGR_NOEXCEPT
#	define YGGR_NOEXCEPT_OR_NOTHROW throw()
#	define YGGR_NOEXCEPT_IF(Predicate)
#	define YGGR_NOEXCEPT_EXPR(Expression) false
#else
#	define YGGR_NOEXCEPT noexcept
#	define YGGR_NOEXCEPT_OR_NOTHROW noexcept
#	define YGGR_NOEXCEPT_IF(Predicate) noexcept((Predicate))
#	define YGGR_NOEXCEPT_EXPR(Expression) noexcept((Expression))
#endif // BOOST_NO_CXX11_NOEXCEPT

#if (YGGR_CPP_VER_23 <= YGGR_CPP_VERSION)
#	define YGGR_CXX23_NOEXCEPT YGGR_NOEXCEPT
#	define YGGR_CXX23_NOEXCEPT_OR_NOTHROW YGGR_NOEXCEPT_OR_NOTHROW
#	define YGGR_CXX23_NOEXCEPT_IF YGGR_NOEXCEPT_IF
#	define YGGR_CXX23_NOEXCEPT_EXPR YGGR_NOEXCEPT_EXPR
#else
#	define YGGR_CXX23_NOEXCEPT
#	define YGGR_CXX23_NOEXCEPT_OR_NOTHROW
#	define YGGR_CXX23_NOEXCEPT_IF(Predicate) 
#	define YGGR_CXX23_NOEXCEPT_EXPR(Expression)
#endif // (YGGR_CPP_VER_23 <= YGGR_CPP_VERSION)

#if (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)
#	define YGGR_CXX20_NOEXCEPT YGGR_NOEXCEPT
#	define YGGR_CXX20_NOEXCEPT_OR_NOTHROW YGGR_NOEXCEPT_OR_NOTHROW
#	define YGGR_CXX20_NOEXCEPT_IF YGGR_NOEXCEPT_IF
#	define YGGR_CXX20_NOEXCEPT_EXPR YGGR_NOEXCEPT_EXPR
#else
#	define YGGR_CXX20_NOEXCEPT
#	define YGGR_CXX20_NOEXCEPT_OR_NOTHROW
#	define YGGR_CXX20_NOEXCEPT_IF(Predicate) 
#	define YGGR_CXX20_NOEXCEPT_EXPR(Expression)
#endif // (YGGR_CPP_VER_20 <= YGGR_CPP_VERSION)

#if (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
#	define YGGR_CXX17_NOEXCEPT YGGR_NOEXCEPT
#	define YGGR_CXX17_NOEXCEPT_OR_NOTHROW YGGR_NOEXCEPT_OR_NOTHROW
#	define YGGR_CXX17_NOEXCEPT_IF YGGR_NOEXCEPT_IF
#	define YGGR_CXX17_NOEXCEPT_EXPR YGGR_NOEXCEPT_EXPR
#else
#	define YGGR_CXX17_NOEXCEPT
#	define YGGR_CXX17_NOEXCEPT_OR_NOTHROW
#	define YGGR_CXX17_NOEXCEPT_IF(Predicate) 
#	define YGGR_CXX17_NOEXCEPT_EXPR(Expression)
#endif // (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)

#if (YGGR_CPP_VER_14 <= YGGR_CPP_VERSION)
#	define YGGR_CXX14_NOEXCEPT YGGR_NOEXCEPT
#	define YGGR_CXX14_NOEXCEPT_OR_NOTHROW YGGR_NOEXCEPT_OR_NOTHROW
#	define YGGR_CXX14_NOEXCEPT_IF YGGR_NOEXCEPT_IF
#	define YGGR_CXX14_NOEXCEPT_EXPR YGGR_NOEXCEPT_EXPR
#else
#	define YGGR_CXX14_NOEXCEPT
#	define YGGR_CXX14_NOEXCEPT_OR_NOTHROW
#	define YGGR_CXX14_NOEXCEPT_IF(Predicate) 
#	define YGGR_CXX14_NOEXCEPT_EXPR(Expression)
#endif // (YGGR_CPP_VER_14 <= YGGR_CPP_VERSION)

#if (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION)
#	define YGGR_CXX11_NOEXCEPT YGGR_NOEXCEPT
#	define YGGR_CXX11_NOEXCEPT_OR_NOTHROW YGGR_NOEXCEPT_OR_NOTHROW
#	define YGGR_CXX11_NOEXCEPT_IF YGGR_NOEXCEPT_IF
#	define YGGR_CXX11_NOEXCEPT_EXPR YGGR_NOEXCEPT_EXPR
#else
#	define YGGR_CXX11_NOEXCEPT
#	define YGGR_CXX11_NOEXCEPT_OR_NOTHROW
#	define YGGR_CXX11_NOEXCEPT_IF(Predicate) 
#	define YGGR_CXX11_NOEXCEPT_EXPR(Expression)
#endif // (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION)

#endif // __YGGR_BASE_NOEXCEPT_CFG_HPP__

