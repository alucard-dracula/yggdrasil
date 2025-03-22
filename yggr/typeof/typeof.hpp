//typeof.hpp

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

#ifndef __YGGR_TYPEOF_TYPEOF_HPP__
#define __YGGR_TYPEOF_TYPEOF_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/config.hpp>

#if defined(YGGR_NO_CXX11_DECLTYPE)
#	include <boost/typeof/typeof.hpp>
#endif // YGGR_NO_CXX11_DECLTYPE

#include <boost/type_traits/remove_reference.hpp>

#ifdef YGGR_NO_CXX11_DECLTYPE

// BOOST_TYPEOF don't support void

#	define YGGR_TYPEOF BOOST_TYPEOF
#	define YGGR_TYPEOF_TPL BOOST_TYPEOF_TPL
#	define YGGR_TYPEOF_NESTED_TYPEDEF_TPL BOOST_TYPEOF_NESTED_TYPEDEF_TPL
#	define YGGR_TYPEOF_NESTED_TYPEDEF BOOST_TYPEOF_NESTED_TYPEDEF

#	define YGGR_TYPEOF_REGISTER_TYPE BOOST_TYPEOF_REGISTER_TYPE
#	define YGGR_TYPEOF_REGISTER_TEMPLATE BOOST_TYPEOF_REGISTER_TEMPLATE

#define YGGR_AUTO BOOST_AUTO
#define YGGR_AUTO_TPL BOOST_AUTO_TPL

#else

#	define YGGR_TYPEOF( __expr__ ) decltype( __expr__ )
#	define YGGR_TYPEOF_TPL YGGR_TYPEOF

#	define YGGR_TYPEOF_NESTED_TYPEDEF( __name__, __expr__ ) \
		struct __name__ { typedef YGGR_TYPEOF( __expr__ ) type; };

#	define YGGR_TYPEOF_NESTED_TYPEDEF_TPL YGGR_TYPEOF_NESTED_TYPEDEF

#	define YGGR_TYPEOF_REGISTER_TYPE( __x__ )
#	define YGGR_TYPEOF_REGISTER_TEMPLATE( __x__, __params__ )

#define YGGR_AUTO(Var, Expr) auto Var = Expr
#define YGGR_AUTO_TPL YGGR_AUTO

#endif // YGGR_NO_CXX11_DECLTYPE

namespace yggr
{

template<typename T>
typename boost::remove_reference<T>::type declval(void);

template<typename T>
typename boost::remove_reference<T>::type declval(T);


template<typename T>
T gen_value_t(T);

} // namespace yggr

#endif // __YGGR_TYPEOF_TYPEOF_HPP__