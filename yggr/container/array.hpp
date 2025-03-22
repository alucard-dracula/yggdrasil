//array.hpp

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

#ifndef __YGGR_CONTAINER_ARRAY_HPP__
#define __YGGR_CONTAINER_ARRAY_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container/container_selector.hpp>
#include <yggr/move/move.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/container/basic_function_adapter.hpp>
#include <yggr/container/detail/is_xxx_container.hpp>

#include <yggr/container_ex/array_t_maker.hpp>
#include <yggr/container_ex/allocator_t.hpp>

#include <boost/ref.hpp>
#include <boost/array.hpp>
#include <boost/mpl/bool.hpp>

#if !defined(YGGR_NO_CXX11_HDR_ARRAY)
#	include <array>
#endif // YGGR_NO_CXX11_HDR_ARRAY

namespace yggr
{
namespace container
{

#if defined(YGGR_USE_BOOST_CONTAINER)
	using boost::array;
#elif defined YGGR_USE_STL_CONTAINER
#	if !defined(YGGR_NO_CXX11_HDR_ARRAY)
		using std::array;
#	else
		using boost::array;
#	endif // YGGR_NO_CXX11_HDR_ARRAY
#else
#	error "use array type unknown error. please check source"
#endif //YGGR_USE_BOOST_CONTAINER

} // namespace container

using container::array;

} // namespace yggr

// allocator_t
namespace yggr
{
namespace container_ex
{
#if !defined YGGR_NO_CXX11_HDR_ARRAY

template<typename T, std::size_t N>
struct allocator_t< std::array<T, N> >
{
	typedef std::allocator<T> type;
};

#endif // YGGR_NO_CXX11_HDR_ARRAY

template<typename T, std::size_t N>
struct allocator_t<boost::array<T, N> >
{
	typedef std::allocator<T> type;
};

} // namespace container_ex
} // namespace yggr

namespace yggr
{
namespace container
{

template<typename Container>
struct is_std_array
	: public boost::mpl::false_
{
};

#ifndef YGGR_NO_CXX11_HDR_ARRAY

template<typename T, std::size_t N>
struct is_std_array< std::array<T, N> >
	: public boost::mpl::true_
{
};

#endif // YGGR_NO_CXX11_HDR_ARRAY

template<typename Container>
struct is_boost_array
	: public boost::mpl::false_
{
};

template<typename T, std::size_t N>
struct is_boost_array< boost::array<T, N> >
	: public boost::mpl::true_
{
};

// std::array
#ifndef YGGR_NO_CXX11_HDR_ARRAY

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
typename array_reference_t_maker<T, N>::type
	cast_to_native_array_reference(std::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef std::array<T, N> now_array_type;
	typedef typename array_reference_t_maker<T, N>::type ref_type;
	typedef typename array_pointer_t_maker<T, N>::type pointer_type;
	typedef typename boost::remove_reference<ref_type>::type now_native_array_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(now_native_array_type) == sizeof(now_array_type)>));

	return *static_cast<pointer_type>(static_cast<void*>(boost::addressof(arr[0])));
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
typename array_const_reference_t_maker<T, N>::type
	cast_to_native_array_reference(const std::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef std::array<T, N> now_array_type;
	typedef typename array_const_reference_t_maker<T, N>::type cref_type;
	typedef typename array_const_pointer_t_maker<T, N>::type cpointer_type;
	typedef typename boost::remove_reference<cref_type>::type now_native_array_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(now_native_array_type) == sizeof(now_array_type)>));

	return *static_cast<cpointer_type>(static_cast<const void*>(boost::addressof(arr[0])));
}

#endif // YGGR_NO_CXX11_HDR_ARRAY

// boost::array
template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
typename array_reference_t_maker<T, N>::type
	cast_to_native_array_reference(boost::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef boost::array<T, N> now_array_type;
	typedef typename array_reference_t_maker<T, N>::type ref_type;
	typedef typename array_pointer_t_maker<T, N>::type pointer_type;
	typedef typename boost::remove_reference<ref_type>::type now_native_array_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(now_native_array_type) == sizeof(now_array_type)>));

	return *static_cast<pointer_type>(static_cast<void*>(boost::addressof(arr[0])));
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
typename array_const_reference_t_maker<T, N>::type
	cast_to_native_array_reference(const boost::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef boost::array<T, N> now_array_type;
	typedef typename array_const_reference_t_maker<T, N>::type cref_type;
	typedef typename array_const_pointer_t_maker<T, N>::type cpointer_type;
	typedef typename boost::remove_reference<cref_type>::type now_native_array_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(now_native_array_type) == sizeof(now_array_type)>));

	return *static_cast<cpointer_type>(static_cast<const void*>(boost::addressof(arr[0])));
}

// other_array
template<template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
typename array_reference_t_maker<T, N>::type
	cast_to_native_array_reference(Array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef Array<T, N> now_array_type;
	typedef typename array_reference_t_maker<T, N>::type ref_type;
	typedef typename array_pointer_t_maker<T, N>::type pointer_type;
	typedef typename boost::remove_reference<ref_type>::type now_native_array_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(now_native_array_type) == sizeof(now_array_type)>));

	return *static_cast<pointer_type>(static_cast<void*>(boost::addressof(arr[0])));
}

template<template<typename _T, std::size_t _N> class Array,
			typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
typename array_const_reference_t_maker<T, N>::type
	cast_to_native_array_reference(const Array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef Array<T, N> now_array_type;
	typedef typename array_const_reference_t_maker<T, N>::type cref_type;
	typedef typename array_const_pointer_t_maker<T, N>::type cpointer_type;
	typedef typename boost::remove_reference<cref_type>::type now_native_array_type;
	BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(now_native_array_type) == sizeof(now_array_type)>));

	return *static_cast<cpointer_type>(static_cast<const void*>(boost::addressof(arr[0])));
}


} // namespace container
} // namespace yggr

// compatibility caller
// array not need compatibility caller,
// because boost is same stl

#endif // __YGGR_CONTAINER_UNORDERED_SET_HPP__
