// bezier_curve.hpp

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

#ifndef __YGGR_GEOMETRY_DETAIL_BEZIER_CURVE_HPP__
#define __YGGR_GEOMETRY_DETAIL_BEZIER_CURVE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/container/resize.hpp>
#include <yggr/container/size.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/geometry/detail/decasteljau.hpp>

namespace yggr
{
namespace geometry
{
namespace detail
{

// -----------------------------value--------------
template<typename Iter, typename T> inline
typename 
	boost::enable_if
	<
		is_iterator<Iter>,
		typename 
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	bezier_curve_value(Iter s, Iter e, T t)
{
	assert(std::distance(s, e));
	return decasteljau(s, e, static_cast<u32>(std::distance(s, e) - 1), t);
}

template<typename Container, typename T> inline
typename 
	boost::enable_if
	<
		container_ex::is_container<Container>,
		typename 
			upper_float
			<
				typename range_ex::range_value_ex<Container>::type
			>::type
	>::type
	bezier_curve_value(const Container& cont, T t)
{
	assert(container::size(cont));
	return decasteljau(cont, static_cast<u32>(container::size(cont) - 1), t);
}

template<typename Iter, typename T1, typename T2> inline
typename 
	boost::enable_if
	<
		is_iterator<Iter>,
		typename 
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	bezier_curve_value(Iter s, Iter e, T1 t1, T2 t2)
{
	assert(std::distance(s, e));
	return decasteljau(s, e, static_cast<u32>(std::distance(s, e) - 1), t1, t2);
}

template<typename Container, typename T1, typename T2> inline
typename 
	boost::enable_if
	<
		container_ex::is_container<Container>,
		typename 
			upper_float
			<
				typename range_ex::range_value_ex<Container>::type
			>::type
	>::type
	bezier_curve_value(const Container& cont, T1 t1, T2 t2)
{
	assert(container::size(cont));
	return decasteljau(cont, static_cast<u32>(container::size(cont) - 1), t1, t2);
}

// --------------------derivation---------------------------
template<typename Container, typename Iter>
Container& bezier_curve_derivation_init(Container& cont, Iter s, Iter e, u32 n)
{
	typedef Iter iter_type;
	typedef Container cont_type;
	typedef typename range_ex::range_iterator_ex<cont_type>::type cont_iter_type;
		
	if(container::size(cont) < n)
	{
		container::resize(cont, n);
	}
	std::advance(e, -1);
	iter_type s_next = s;
	std::advance(s_next, 1);
	for(cont_iter_type i = boost::begin(cont); s != e; 
			std::advance(s, 1), std::advance(s_next, 1), std::advance(i, 1))
	{
		*i = *s_next - *s;
	}

	return cont;
}

template<typename Iter, typename T> inline
typename 
	boost::enable_if
	<
		is_iterator<Iter>,
		typename 
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	bezier_curve_derivation_cal_of_stack(Iter s, Iter e, u32 n, T t)
{
	typedef Iter iter_type;
	typedef typename boost::iterator_value<iter_type>::type value_type;
	typedef typename upper_float<value_type>::type result_type;
	typedef container::array<value_type, decasteljau_stack_limit::value> stack_array_type;
	typedef typename range_ex::range_iterator_ex<stack_array_type>::type arr_iter_type;

	assert(T(0) < t && t < T(1));
	assert((n < decasteljau_stack_limit::value));
	assert(std::distance(s, e) - 1 == n);

	stack_array_type arr;
	bezier_curve_derivation_init(arr, s, e, n);
	arr_iter_type as = boost::begin(arr);
	arr_iter_type ae = as;
	std::advance(ae, n);
	return n * decasteljau(as, ae, n - 1, t);
}

template<typename Iter, typename T1, typename T2> inline
typename 
	boost::enable_if
	<
		is_iterator<Iter>,
		typename 
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	bezier_curve_derivation_cal_of_stack(Iter s, Iter e, u32 n, T1 t1, T2 t2)
{
	typedef Iter iter_type;
	typedef typename boost::iterator_value<iter_type>::type value_type;
	typedef typename upper_float<value_type>::type result_type;
	typedef container::array<value_type, decasteljau_stack_limit::value> stack_array_type;
	typedef typename range_ex::range_iterator_ex<stack_array_type>::type arr_iter_type;
	typedef math::flaw_comparer<T1> t1_cmp_type;

	assert(T1(0) < t1 && t1_cmp_type(t1) < t2);
	assert((n < decasteljau_stack_limit::value));
	assert(std::distance(s, e) - 1 == n);

	stack_array_type arr;
	bezier_curve_derivation_init(arr, s, e, n);
	arr_iter_type as = boost::begin(arr);
	arr_iter_type ae = as;
	std::advance(ae, n);

	return n * decasteljau(as, ae, n - 1, t1, t2);
}

template<typename Iter, typename T> inline
typename 
	boost::enable_if
	<
		is_iterator<Iter>,
		typename 
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	bezier_curve_derivation_cal_of_heap(Iter s, Iter e, u32 n, T t)
{
	typedef Iter iter_type;
	typedef typename boost::iterator_value<iter_type>::type value_type;
	typedef typename upper_float<value_type>::type result_type;
	typedef container::vector<value_type> heap_array_type;

	assert(T(0) < t && t < T(1));
	assert(!(n < decasteljau_stack_limit::value));
	assert(std::distance(s, e) - 1 == n);

	heap_array_type arr;
	return n * decasteljau(bezier_curve_derivation_init(arr, s, e, n), n - 1, t);
}

template<typename Iter, typename T1, typename T2> inline
typename 
	boost::enable_if
	<
		is_iterator<Iter>,
		typename 
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	bezier_curve_derivation_cal_of_heap(Iter s, Iter e, u32 n, T1 t1, T2 t2)
{
	typedef Iter iter_type;
	typedef typename boost::iterator_value<iter_type>::type value_type;
	typedef typename upper_float<value_type>::type result_type;
	typedef container::vector<value_type> heap_array_type;
	typedef math::flaw_comparer<T1> t1_cmp_type;


	assert(T1(0) < t1 && t1_cmp_type(t1) < t2);
	assert(!(n < decasteljau_stack_limit::value));
	assert(std::distance(s, e) - 1 == n);

	heap_array_type arr;
	return n * decasteljau(bezier_curve_derivation_init(arr, s, e, n), n - 1, t1, t2);
}

template<typename Iter, typename T> inline
typename 
	boost::enable_if
	<
		is_iterator<Iter>,
		typename 
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	bezier_curve_derivation(Iter s, Iter e, u32 n, T t)
{
	typedef Iter iter_type;
	typedef typename boost::iterator_value<iter_type>::type value_type;
	typedef typename upper_float<value_type>::type result_type;

	assert(T(0) <= t && t <= T(1));
	assert(std::distance(s, e) - 1 == n);

	if(n)
	{
		if(t == T(0))
		{
			iter_type s_next = s;
			std::advance(s_next, 1);
			return n * (*s_next - *s); 
		}
		else if(t == T(1))
		{
			iter_type e_before = e;
			std::advance(e_before, -1);
			iter_type e_bbefore = e_before;
			std::advance(e_bbefore, -1);

			return n * (*e_before - *e_bbefore);
		}
		else
		{
			return 
				n < decasteljau_stack_limit::value?
					bezier_curve_derivation_cal_of_stack(s, e, n, t)
					: bezier_curve_derivation_cal_of_heap(s, e, n, t);
		}
	}
	else
	{
		return result_type() - result_type();
	}
}

template<typename Container, typename T> inline
typename 
	boost::enable_if
	<
		container_ex::is_container<Container>,
		typename 
			upper_float
			<
				typename range_ex::range_value_ex<Container>::type
			>::type
	>::type
	bezier_curve_derivation(const Container& cont, T t)
{
	return bezier_curve_derivation(boost::begin(cont), boost::end(cont), container::size(cont) - 1, t);
}

template<typename Iter, typename T1, typename T2> inline
typename 
	boost::enable_if
	<
		is_iterator<Iter>,
		typename 
			upper_float
			<
				typename boost::iterator_value<Iter>::type
			>::type
	>::type
	bezier_curve_derivation(Iter s, Iter e, u32 n, T1 t1, T2 t2)
{
	typedef Iter iter_type;
	typedef typename boost::iterator_value<iter_type>::type value_type;
	typedef typename upper_float<value_type>::type result_type;
	typedef container::array<value_type, decasteljau_stack_limit::value> stack_array_type;
	typedef container::vector<value_type> heap_array_type;
	typedef math::flaw_comparer<T1> t1_cmp_type;

	assert(T1(0) <= t1 && t1_cmp_type(t1) <= t2);
	assert(std::distance(s, e) - 1 == n);

	if(n)
	{
		if(T1(0) == t1)
		{
			iter_type s_next = s;
			std::advance(s_next, 1);
			return n * (*s_next - *s); 
		}
		else if(t1_cmp_type(t1) == t2)
		{
			iter_type e_before = e;
			std::advance(e_before, -1);
			iter_type e_bbefore = e_before;
			std::advance(e_bbefore, -1);

			return n * (*e_before - *e_bbefore);
		}
		else
		{
			return 
				n < decasteljau_stack_limit::value?
					bezier_curve_derivation_cal_of_stack(s, e, n, t1, t2)
					: bezier_curve_derivation_cal_of_heap(s, e, n, t1, t2);
		}
	}
	else
	{
		return result_type() - result_type();
	}
}

template<typename Container, typename T1, typename T2> inline
typename 
	boost::enable_if
	<
		container_ex::is_container<Container>,
		typename 
			upper_float
			<
				typename range_ex::range_value_ex<Container>::type
			>::type
	>::type
	bezier_curve_derivation(const Container& cont, T1 t1, T2 t2)
{
	return bezier_curve_derivation(boost::begin(cont), boost::end(cont), container::size(cont) - 1, t1, t2);
}

} // namespace detail
} // namespace geometry
} // namespace yggr

#endif // __YGGR_GEOMETRY_DETAIL_BEZIER_CURVE_HPP__
