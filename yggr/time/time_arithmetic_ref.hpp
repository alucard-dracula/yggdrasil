//time_category.hpp

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

#ifndef __YGGR_TIME_TIME_ARITHMETIC_REF_HPP__
#define __YGGR_TIME_TIME_ARITHMETIC_REF_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/math/arithmetic_ref.hpp>

#include <boost/mpl/size_t.hpp>

namespace yggr
{
namespace time
{

struct tag_float_divides
{
};

typedef tag_float_divides tag_float_divides_type;


#if defined( __BORLANDC__ ) && BOOST_WORKAROUND( __BORLANDC__, BOOST_TESTED_AT(0x581) )
#	define YGGR_REF_CONST
#else
#	define YGGR_REF_CONST const
#endif // __BORLANDC__

template<typename T> inline
math::arith_reference_wrapper<tag_float_divides_type, f64, T> YGGR_REF_CONST
	time_float_divides_ref(T& t)
{
	return math::arith_ref<tag_float_divides_type, f64>(t);
}

template<typename R, typename T> inline
math::arith_reference_wrapper<tag_float_divides_type, R, T> YGGR_REF_CONST
	time_float_divides_ref(T& t)
{
	return math::arith_ref<tag_float_divides_type, R>(t);
}

template<typename T> inline
math::arith_reference_wrapper<tag_float_divides_type, f64, T const> YGGR_REF_CONST
	time_float_divides_cref(T const& t)
{
	return math::arith_cref<tag_float_divides_type, f64>(t);
}

template<typename R, typename T> inline
math::arith_reference_wrapper<tag_float_divides_type, R, T const> YGGR_REF_CONST
	time_float_divides_cref(T const& t)
{
	return math::arith_cref<tag_float_divides_type, R>(t);
}

#undef YGGR_REF_CONST


} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_TIME_ARITHMETIC_REF_HPP__
