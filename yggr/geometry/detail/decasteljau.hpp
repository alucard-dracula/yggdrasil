// decasteljau.hpp

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

#ifndef __YGGR_GEOMETRY_DETAIL_DECASTELJAU_HPP__
#define __YGGR_GEOMETRY_DETAIL_DECASTELJAU_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/container/array.hpp>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/container_ex/is_container.hpp>
#include <yggr/math/flaw_comparer.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>
#include <yggr/range_ex/range_value_ex.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/integral_c.hpp>

#include <cassert>
#include <iterator>

namespace yggr
{
namespace geometry
{
namespace detail
{

// ----------------------details--------------------

struct decasteljau_stack_limit
	: public boost::mpl::integral_c<u32, 5>
{
};

template<typename Iter, typename T>
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
	decasteljau_of_stack(Iter s, Iter e, u32 n, T t)
{
	typedef typename upper_float<T>::type cal_t_type;
	typedef typename boost::iterator_value<Iter>::type value_type;
	typedef typename upper_float<value_type>::type result_type;
	typedef yggr::array<result_type, decasteljau_stack_limit::value> cal_array_type;
	typedef typename range_ex::range_iterator_ex<cal_array_type>::type array_iter_type;

	assert(T(0) <= t && t <= T(1));
	assert(n < decasteljau_stack_limit::value);
	assert(std::distance(s, e) - 1 == n);

	if(n)
	{
		if(t == T(0))
		{
			return *s;
		}
		else if(t == T(1))
		{
			std::advance(e, -1);
			return *e;
		}
		else
		{
			cal_array_type cal_arr;
			std::copy(s, e, boost::begin(cal_arr));
			array_iter_type i = boost::begin(cal_arr);
			array_iter_type isize = i;
			std::advance(isize, n + 1);

			array_iter_type j;
			array_iter_type j_next;
			array_iter_type jsize = isize;

			cal_t_type cal_t = static_cast<cal_t_type>(t);
			cal_t_type cal_cmplt = cal_t_type(1) - cal_t;

			for(; i != isize; std::advance(i, 1))
			{
				j = boost::begin(cal_arr);
				j_next = j;
				std::advance(j_next, 1);
				std::advance(jsize, -1);
				for(; j != jsize; std::advance(j, 1), std::advance(j_next, 1))
				{
					*j = cal_cmplt * (*j) + cal_t * (*j_next);
				}
			}
		
			return *(boost::begin(cal_arr));
		}
	}
	else
	{
		return *s;
	}
}

template<typename Iter, typename T1, typename T2>
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
	decasteljau_of_stack(Iter s, Iter e, u32 n, T1 t1, T2 t2)
{
	typedef typename upper_float<T1>::type cal_t_type;
	typedef typename boost::iterator_value<Iter>::type value_type;
	typedef typename upper_float<value_type>::type result_type;
	typedef yggr::array<result_type, decasteljau_stack_limit::value> cal_array_type;
	typedef typename range_ex::range_iterator_ex<cal_array_type>::type array_iter_type;
	typedef math::flaw_comparer<T1> t1_cmp_type;

	assert(T1(0) <= t1 && t1_cmp_type(t1) <= t2);
	assert(n < decasteljau_stack_limit::value);
	assert(std::distance(s, e) - 1 == n);

	if(n)
	{
		if(T1(0) == t1)
		{
			return *s;
		}
		else if(t1_cmp_type(t1) == t2)
		{
			std::advance(e, -1);
			return *e;
		}
		else
		{
			assert(T1(0) < t1 && t1_cmp_type(t1) < t2);
			cal_array_type cal_arr;
			std::copy(s, e, boost::begin(cal_arr));
			array_iter_type i = boost::begin(cal_arr);
			array_iter_type isize = i;
			std::advance(isize, n + 1);

			array_iter_type j;
			array_iter_type j_next;
			array_iter_type jsize = isize;

			cal_t_type cal_t = static_cast<cal_t_type>(t1);
			cal_t_type cal_cmplt = static_cast<cal_t_type>(t2 - cal_t);

			for(; i != isize; std::advance(i, 1))
			{
				j = boost::begin(cal_arr);
				j_next = j;
				std::advance(j_next, 1);
				std::advance(jsize, -1);
				for(; j != jsize; std::advance(j, 1), std::advance(j_next, 1))
				{
					*j = (cal_cmplt * (*j) + cal_t * (*j_next)) / t2;
				}
			}
		
			return *(boost::begin(cal_arr));
		}
	}
	else
	{
		return *s;
	}
}

template<typename Iter, typename T>
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
	decasteljau_of_heap(Iter s, Iter e, u32 n, T t)
{
	typedef typename upper_float<T>::type cal_t_type;
	typedef typename boost::iterator_value<Iter>::type value_type;
	typedef typename upper_float<value_type>::type result_type;
	typedef yggr::vector<result_type> cal_array_type;
	typedef typename range_ex::range_iterator_ex<cal_array_type>::type array_iter_type;

	assert(T(0) <= t && t <= T(1));
	assert(!(n < decasteljau_stack_limit::value));
	assert(std::distance(s, e) - 1 == n);

	if(n)
	{
		if(t == T(0))
		{
			return *s;
		}
		else if(t == T(1))
		{
			std::advance(e, -1);
			return *e;
		}
		else
		{
			cal_array_type cal_arr(s, e);
			array_iter_type i = boost::begin(cal_arr);
			array_iter_type isize = boost::end(cal_arr);

			array_iter_type j;
			array_iter_type j_next;
			array_iter_type jsize = boost::end(cal_arr);

			cal_t_type cal_t = static_cast<cal_t_type>(t);
			cal_t_type cal_cmplt = cal_t_type(1) - cal_t;

			for(; i != isize; std::advance(i, 1))
			{
				j = boost::begin(cal_arr);
				j_next = j;
				std::advance(j_next, 1);
				std::advance(jsize, -1);
				for(; j != jsize; std::advance(j, 1), std::advance(j_next, 1))
				{
					*j = cal_cmplt * (*j) + cal_t * (*j_next);
				}
			}
		
			return *(boost::begin(cal_arr));
		}
	}
	else
	{
		return *s;
	}
}

template<typename Iter, typename T1, typename T2>
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
	decasteljau_of_heap(Iter s, Iter e, u32 n, T1 t1, T2 t2)
{
	typedef typename upper_float<T1>::type cal_t_type;
	typedef typename boost::iterator_value<Iter>::type value_type;
	typedef typename upper_float<value_type>::type result_type;
	typedef yggr::vector<result_type> cal_array_type;
	typedef typename range_ex::range_iterator_ex<cal_array_type>::type array_iter_type;
	typedef math::flaw_comparer<T1> t1_cmp_type;

	assert(T1(0) <= t1 && t1_cmp_type(t1) <= t2);
	assert(!(n < decasteljau_stack_limit::value));
	assert(std::distance(s, e) - 1 == n);

	if(n)
	{
		if(T1(0) == t1)
		{
			return *s;
		}
		else if(t1_cmp_type(t1) == t2)
		{
			std::advance(e, -1);
			return *e;
		}
		else
		{
			cal_array_type cal_arr(s, e);
			array_iter_type i = boost::begin(cal_arr);
			array_iter_type isize = boost::end(cal_arr);

			array_iter_type j;
			array_iter_type j_next;
			array_iter_type jsize = boost::end(cal_arr);

			cal_t_type cal_t = static_cast<cal_t_type>(t1);
			cal_t_type cal_cmplt = static_cast<cal_t_type>(t2 - cal_t);

			for(; i != isize; std::advance(i, 1))
			{
				j = boost::begin(cal_arr);
				j_next = j;
				std::advance(j_next, 1);
				std::advance(jsize, -1);
				for(; j != jsize; std::advance(j, 1), std::advance(j_next, 1))
				{
					*j = (cal_cmplt * (*j) + cal_t * (*j_next)) / t2;
				}
			}
		
			return *(boost::begin(cal_arr));
		}
	}
	else
	{
		return *s;
	}
}

// ----------------------interface----------------------

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
	decasteljau(Iter s, Iter e, u32 n, T t)
{
	return 
		n < decasteljau_stack_limit::value? 
			decasteljau_of_stack(s, e, n, t)
			: decasteljau_of_heap(s, e, n, t);
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
	decasteljau(Iter s, Iter e, u32 n, T1 t1, T2 t2)
{
	return 
		n < decasteljau_stack_limit::value? 
			decasteljau_of_stack(s, e, n, t1, t2)
			: decasteljau_of_heap(s, e, n, t1, t2);
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
	decasteljau(const Container& cont, u32 n, T t)
{
	return 
		n < decasteljau_stack_limit::value? 
			decasteljau_of_stack(boost::begin(cont), boost::end(cont), n, t)
			: decasteljau_of_heap(boost::begin(cont), boost::end(cont), n, t);
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
	decasteljau(const Container& cont, u32 n, T1 t1, T2 t2)
{
	return 
		n < decasteljau_stack_limit::value? 
			decasteljau_of_stack(boost::begin(cont), boost::end(cont), n, t1, t2)
			: decasteljau_of_heap(boost::begin(cont), boost::end(cont), n, t1, t2);
}


} // namespace detail
} // namespace geometry
} // namespace yggr

#endif // __YGGR_GEOMETRY_DETAIL_DECASTELJAU_HPP__
