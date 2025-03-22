//pair.hpp

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

#ifndef __YGGR_UTILITY_PAIR_HPP__
#define __YGGR_UTILITY_PAIR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/index_t_value.hpp>
#include <yggr/tuple/tuple_utility_decl.hpp>

#include <boost/mpl/size_t.hpp>

#include <utility>

namespace yggr
{
namespace utility
{
namespace detail
{

template<typename T1, typename T2, std::size_t idx>
struct pair_getter;

template<typename T1, typename T2>
struct pair_getter<T1, T2, 0>
{
	typedef std::pair<T1, T2> pair_type;
	typedef typename pair_type::first_type result_type;

	YGGR_CONSTEXPR_OR_INLINE 
	result_type& operator()(pair_type& val) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return val.first;
	}

	YGGR_CONSTEXPR_OR_INLINE 
	const result_type& operator()(const pair_type& val) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return val.first;
	}
};

template<typename T1, typename T2>
struct pair_getter<T1, T2, 1>
{
	typedef std::pair<T1, T2> pair_type;
	typedef typename pair_type::second_type result_type;

	YGGR_CONSTEXPR_OR_INLINE 
	result_type& operator()(pair_type& val) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return val.second;
	}

	YGGR_CONSTEXPR_OR_INLINE 
	const result_type& operator()(const pair_type& val) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return val.second;
	}
};

} // namespace detail
} // namespace utility

//#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
//
//using std::get;
//
//#else

//using std::get; // don't using std::array, because in stl get<typename Index> version is occupied

template<std::size_t idx, typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE 
typename utility::detail::pair_getter<T1, T2, idx>::result_type& 
	get(std::pair<T1, T2>& val) YGGR_NOEXCEPT_OR_NOTHROW
{ 
	typedef utility::detail::pair_getter<T1, T2, idx> getter_type;
	getter_type getter;
	return getter(val);
}

template<std::size_t idx, typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE 
const typename utility::detail::pair_getter<T1, T2, idx>::result_type&
	get(const std::pair<T1, T2>& val) YGGR_NOEXCEPT_OR_NOTHROW
{ 
	typedef utility::detail::pair_getter<T1, T2, idx> getter_type;
	getter_type getter;
	return getter(val);
}

//#endif // YGGR_NO_CXX11_HDR_TUPLE

template<typename Index, typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE
typename 
	utility::detail::pair_getter
	<
		T1, 
		T2, 
		YGGR_INDEX_T_VALUE_GET_CAST(std::size_t, Index)
	>::result_type& 
	get(std::pair<T1, T2>& val) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef 
		utility::detail::pair_getter
		<
			T1, 
			T2, 
			YGGR_INDEX_T_VALUE_GET_CAST(std::size_t, Index)
		> getter_type;

	getter_type getter;
	return getter(val);
}

template<typename Index, typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE
const 
	typename
		utility::detail::pair_getter
		<
			T1, 
			T2, 
			YGGR_INDEX_T_VALUE_GET_CAST(std::size_t, Index)
		>::result_type& 
	get(const std::pair<T1, T2>& val) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef 
		utility::detail::pair_getter
		<
			T1, 
			T2, 
			YGGR_INDEX_T_VALUE_GET_CAST(std::size_t, Index)
		> getter_type;

	getter_type getter;
	return getter(val);
}

} // namespace yggr

// tuple_utility
namespace yggr
{
namespace utility
{
namespace detail
{

template<std::size_t I, typename Pair>
struct pair_element_imp;

template<std::size_t I,
			template<typename _T1, typename _T2> class Pair,
			typename T1, typename T2>
struct pair_element_imp<I, Pair<T1, T2> >
{
	typedef typename pair_getter<T1, T2, I>::result_type type;
};

} // namespace detail
} // namespace utility

// tuple_size
template<template<typename _T1, typename _T2> class Pair,
			typename T1, typename T2>
struct tuple_size<Pair<T1, T2> >
	: public boost::mpl::size_t<2>
{
};

// tuple_element
template<std::size_t I,
			template<typename _T1, typename _T2> class Pair,
			typename T1, typename T2>
struct tuple_element<I, Pair<T1, T2> >
	: public utility::detail::pair_element_imp<I, Pair<T1, T2> >
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(I < 2)>));
};

} // namespace yggr

#endif // __YGGR_UTILITY_PAIR_HPP__
