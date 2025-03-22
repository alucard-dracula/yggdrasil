//operator_t.hpp

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

#ifndef __YGGR_SUPPORT_OPERATOR_T_HPP__
#define __YGGR_SUPPORT_OPERATOR_T_HPP__

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#if !defined( BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS ) && !defined( BOOST_NO_CXX11_NULLPTR )

#	define YGGR_OP_TYPE_EXPLICIT() explicit

#else

#	define YGGR_OP_TYPE_EXPLICIT()

#endif // !defined( BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS ) && !defined( BOOST_NO_CXX11_NULLPTR )

#	define YGGR_OP_BOOL_EXPLICIT YGGR_OP_TYPE_EXPLICIT

#endif // __YGGR_SUPPORT_OPERATOR_T_HPP__