//pfto.hpp

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

#ifndef __YGGR_SERIALIZATION_PFTO_HPP__
#define __YGGR_SERIALIZATION_PFTO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/symbols.hpp>

#if BOOST_VERSION < 105900
#	include <boost/serialization/pfto.hpp>
#endif // BOOST_VERSION < 105900

#if BOOST_VERSION < 105900

#	define YGGR_PFTO_TYPE( __type__ ) YGGR_PP_SYMBOL_COMMA() BOOST_PFTO __type__
#	define YGGR_NON_PFTO_TYPE( __type__ ) YGGR_PP_SYMBOL_COMMA() __type__

#	define YGGR_PFTO_TYPE_VAR( __type__, __var__) YGGR_PP_SYMBOL_COMMA() BOOST_PFTO __type__ __var__
#	define YGGR_NON_PFTO_TYPE_VAR( __type__, __var__) YGGR_PP_SYMBOL_COMMA() __type__ __var__

#	define YGGR_PFTO_VAR( __var__ ) YGGR_PP_SYMBOL_COMMA() __var__

#else

#	define YGGR_PFTO_TYPE( __type__ )
#	define YGGR_NON_PFTO_TYPE( __type__ )

#	define YGGR_PFTO_TYPE_VAR( __type__, __var__)
#	define YGGR_NON_PFTO_TYPE_VAR( __type__, __var__)

#	define YGGR_PFTO_VAR( __var__ )

#endif // YGGR_PFTO_TYPE

namespace yggr
{
namespace serialization
{
} // namespace serialization
} // namespace yggr

#endif // __YGGR_SERIALIZATION_PFTO_HPP__
