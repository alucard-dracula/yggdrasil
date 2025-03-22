// conv_to_tuple.hpp

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

#ifndef __YGGR_MPLEX_CONV_TO_TUPLE_HPP__
#define __YGGR_MPLEX_CONV_TO_TUPLE_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

#	include <yggr/mplex/detail/conv_to_tuple_vtpl.hpp>

#else

#	include <yggr/mplex/detail/conv_to_tuple_vmacro.hpp>

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

namespace yggr
{
namespace mplex
{

template<typename Vector, typename Null_T = ::yggr::tuples::null_type>
struct conv_to_yggr_tuple
	: public conv_to_tuple< ::yggr::tuple, Vector, Null_T>
{
};

template<typename Vector, typename Null_T = boost::tuples::null_type>
struct conv_to_boost_tuple
	: public conv_to_tuple< ::boost::tuple, Vector, Null_T>
{
};


} // namespace mplex
} // namespace yggr




#endif // __YGGR_MPLEX_CONV_TO_TUPLE_HPP__
