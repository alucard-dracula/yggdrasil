//moving_elem_genner.hpp

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

#ifndef __YGGR_MATH_MOVING_AVG_MOVING_ELEM_GENNER_HPP__
#define __YGGR_MATH_MOVING_AVG_MOVING_ELEM_GENNER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/math/moving_avg/genner_getter_typeid.hpp>

#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_signed.hpp> 
#include <boost/type_traits/is_floating_point.hpp>

#include <complex>

namespace yggr
{
namespace math
{
namespace moving_avg
{

template<typename Val>
struct identical_genner
{
public:
	typedef Val result_type;

	BOOST_MPL_ASSERT((
			boost::mpl::or_
			<
				boost::is_floating_point<result_type>,
				boost::is_signed<result_type>
			> ));

	YGGR_STATIC_CONSTANT(
		genner_getter_typeid_def::value_type, 
		E_typeid = genner_getter_typeid_def::E_identical);

public:
	template<typename T> inline
	result_type operator()(const T& t, ...) const
	{
		return result_type(t);
	}
};

template<typename Val>
struct distance
{
public:
	typedef Val result_type;

	BOOST_MPL_ASSERT((
			boost::mpl::or_
			<
				boost::is_floating_point<result_type>,
				boost::is_signed<result_type>
			> ));

	YGGR_STATIC_CONSTANT(
		genner_getter_typeid_def::value_type, 
		E_typeid = genner_getter_typeid_def::E_identical);

public:
	template<typename T1, typename T2> inline
	result_type operator()(const T1& t1, const T2& t2) const
	{
		return result_type(t1 - t2);
	}
};

template<typename Val>
struct abs_distance
{
public:
	typedef Val result_type;

	BOOST_MPL_ASSERT((
			boost::mpl::or_
			<
				boost::is_floating_point<result_type>,
				boost::is_signed<result_type>
			> ));

	YGGR_STATIC_CONSTANT(
		genner_getter_typeid_def::value_type, 
		E_typeid = genner_getter_typeid_def::E_identical);

public:
	template<typename T1, typename T2> inline
	result_type operator()(const T1& t1, const T2& t2) const
	{
		return result_type(std::abs(t1 - t2));
	}
};

template<typename Val>
struct pow2_distance
{
public:
	typedef Val result_type;

	BOOST_MPL_ASSERT((
			boost::mpl::or_
			<
				boost::is_floating_point<result_type>,
				boost::is_signed<result_type>
			> ));

	YGGR_STATIC_CONSTANT(
		genner_getter_typeid_def::value_type, 
		E_typeid = genner_getter_typeid_def::E_pow2);

public:
	template<typename T1, typename T2> inline
	result_type operator()(const T1& t1, const T2& t2) const
	{
		result_type tmp = result_type(t1 - t2);
		return tmp * tmp;
	}
};

} // namespace moving_avg
} // namespace math
} // namespace yggr

YGGR_MAKE_MOVING_AVG_GENNER_TYPEID_INNER(identical_genner)

YGGR_MAKE_MOVING_AVG_GENNER_TYPEID_INNER(distance)

YGGR_MAKE_MOVING_AVG_GENNER_TYPEID_INNER(abs_distance)

YGGR_MAKE_MOVING_AVG_GENNER_TYPEID_INNER(pow2_distance)


#endif //__YGGR_MATH_MOVING_AVG_MOVING_ELEM_GENNER_HPP__
