//byte_size_detail_array.hpp

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

#ifndef __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_ARRAY_HPP__
#define __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_ARRAY_HPP__

#include <yggr/bytes/detail/byte_size_detail_basic.hpp>

namespace yggr
{
namespace bytes
{
namespace detail
{

template<typename T, std::size_t N, bool is_class>
struct native_array_inner_helper;

template<typename T, std::size_t N>
struct native_array_inner_helper<T, N, true>
{
	typedef T value_type[N];
	typedef T cv_sub_val_type;
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type;
	typedef byte_size_helper<sub_val_type> h_type;

	inline u64 operator()(const value_type& val) const
	{
		h_type h;
		return 
			std::accumulate(
				boost::begin(val), boost::end(val), 0,
				boost::bind(
					&acc_op::foo<sub_val_type, h_type>, _1, _2, boost::cref(h)));
	}
};

template<typename T, std::size_t N>
struct native_array_inner_helper<T, N, false>
{
	typedef T value_type[N];
	typedef T cv_sub_val_type;
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type;
	typedef byte_size_helper<sub_val_type> h_type;

	inline u64 operator()(const value_type& val) const
	{
		h_type h;
		return N * h();
	}
};

// type val[m]
template<typename T, std::size_t N >
struct byte_size_helper< T[N] >
	: public boost::mpl::true_
{
public:
	typedef T value_type[N];

private:
	typedef T cv_sub_val_type;
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type;
	typedef boost::is_class<sub_val_type> is_class_type;
	typedef byte_size_helper<sub_val_type> value_helper_type;
	typedef 
		native_array_inner_helper
		<
			T, N,
			(is_class_type::value && value_helper_type::value)
		> ih_type;

public:
	inline u64 operator()(const value_type& val) const
	{
		ih_type ih;
		return ih(val);
	}
};

// like boost::array

template<typename T, bool is_class>
struct tpl_array_inner_helper;

template<template<typename _T, std::size_t _N> class C,
			typename T, std::size_t N>
struct tpl_array_inner_helper<C<T, N>, true>
{
	typedef C<T, N> value_type;
	typedef T cv_sub_val_type;
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type;
	typedef byte_size_helper<sub_val_type> h_type;

	inline u64 operator()(const value_type& val) const
	{
		h_type h;
		return
			std::accumulate(
				boost::begin(val), boost::end(val), 0,
				boost::bind(&acc_op::foo<sub_val_type, h_type>, _1, _2, boost::cref(h)));
	}
};

template<template<typename _T, std::size_t _N> class C,
			typename T, std::size_t N>
struct tpl_array_inner_helper<C<T, N>, false>
{
	typedef C<T, N> value_type;
	typedef T cv_sub_val_type;
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type;
	typedef byte_size_helper<sub_val_type> h_type;

	inline u64 operator()(const value_type& val) const
	{
		h_type h;
		return boost::size(val) * h();
	}
};

template<typename T, std::size_t N,
			template<typename _T, std::size_t _N> class C>
struct byte_size_helper< C<T, N> >
	: public boost::mpl::true_
{
public:
	typedef C<T, N> value_type;

private:
	typedef T cv_sub_val_type;
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type;
	typedef boost::is_class<sub_val_type> is_class_type;
	typedef byte_size_helper<sub_val_type> value_helper_type;
	typedef 
		tpl_array_inner_helper
		<
			value_type,
			(is_class_type::value && value_helper_type::value)
		> ih_type;

public:
	inline u64 operator()(const value_type& val) const
	{
		ih_type ih;
		return ih(val);
	}
};

} // namespace detail
} // namespace bytes
} // namespace yggr

#endif // __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_ARRAY_HPP__
