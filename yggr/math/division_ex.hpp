// division_ex.hpp

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

#ifndef __YGGR_MATH_DIVISION_EX_HPP__
#define __YGGR_MATH_DIVISION_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/type_traits/operator_result.hpp>

#include <cmath>

namespace yggr
{
namespace math
{

namespace detail
{

template<typename T1, typename T2>
struct is_using_float_div_ver
	: public
		boost::is_floating_point
		<
			typename divides_operator_result<T1, T2>::type
		>
{
};

// floor

template<typename T1, typename T2,
			bool is_floating_point = is_using_float_div_ver<T1, T2>::value >
struct division_ex_floor;

template<typename T1, typename T2>
struct division_ex_floor<T1, T2, true>
{
public:
	typedef T1 l_type;
	typedef T2 r_type;
	typedef typename divides_operator_result<T1, T2>::type result_type;

private:
	typedef division_ex_floor this_type;

public:
	inline result_type operator()(const l_type& l, const r_type& r) const
	{
		return result_type(std::floor(l / r));
	}
};

template<typename T1, typename T2>
struct division_ex_floor<T1, T2, false>
{
public:
	typedef T1 l_type;
	typedef T2 r_type;
	typedef typename divides_operator_result<T1, T2>::type result_type;

private:
	typedef division_ex_floor this_type;

public:
	inline result_type operator()(const l_type& l, const r_type& r) const
	{
		return result_type(l / r);
	}
};

// round

template<typename T1, typename T2,
			bool is_floating_point = is_using_float_div_ver<T1, T2>::value >
struct division_ex_round;

template<typename T1, typename T2>
struct division_ex_round<T1, T2, true>
{
public:
	typedef T1 l_type;
	typedef T2 r_type;
	typedef typename divides_operator_result<T1, T2>::type result_type;

private:
	typedef division_ex_round this_type;

public:
	inline result_type operator()(const l_type& l, const r_type& r) const
	{
		typedef typename upper_float<r_type>::type rf_type;
		return result_type(std::floor((l + r / rf_type(2)) / r));
	}
};

template<typename T1, typename T2>
struct division_ex_round<T1, T2, false>
{
public:
	typedef T1 l_type;
	typedef T2 r_type;
	typedef typename divides_operator_result<T1, T2>::type result_type;

private:
	typedef division_ex_round this_type;

public:
	inline result_type operator()(const l_type& l, const r_type& r) const
	{
		return result_type((l + r / 2) / r);
	}
};

// ceil

template<typename T1, typename T2,
			bool is_floating_point = is_using_float_div_ver<T1, T2>::value >
struct division_ex_ceil;

template<typename T1, typename T2>
struct division_ex_ceil<T1, T2, true>
{
public:
	typedef T1 l_type;
	typedef T2 r_type;
	typedef typename divides_operator_result<T1, T2>::type result_type;

private:
	typedef division_ex_ceil this_type;

public:
	inline result_type operator()(const l_type& l, const r_type& r) const
	{
		return result_type(std::ceil(l / r));
	}
};

template<typename T1, typename T2>
struct division_ex_ceil<T1, T2, false>
{
public:
	typedef T1 l_type;
	typedef T2 r_type;
	typedef typename divides_operator_result<T1, T2>::type result_type;

private:
	typedef division_ex_ceil this_type;

public:
	inline result_type operator()(const l_type& l, const r_type& r) const
	{
		return result_type((l + r - 1) / r);
	}
};

} // namespace detail


template<typename T1, typename T2> inline
typename detail::division_ex_floor<T1, T2>::result_type
	division_floor(const T1& l, const T2& r)
{
	typedef detail::division_ex_floor<T1, T2> cal_type;

	cal_type cal;
	return cal(l, r);
}

template<typename T1, typename T2> inline
typename detail::division_ex_round<T1, T2>::result_type
	division_round(const T1& l, const T2& r)
{
	typedef detail::division_ex_round<T1, T2> cal_type;

	cal_type cal;
	return cal(l, r);
}

template<typename T1, typename T2> inline
typename detail::division_ex_ceil<T1, T2>::result_type
	division_ceil(const T1& l, const T2& r)
{
	typedef detail::division_ex_ceil<T1, T2> cal_type;

	cal_type cal;
	return cal(l, r);
}

} // namespace math
} // namespace yggr

#endif //__YGGR_MATH_DIVISION_EX_HPP__
