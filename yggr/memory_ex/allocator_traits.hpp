// allocator_traits.hpp

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

#ifndef __YGGR_MEMORY_EX_ALLOCATOR_TRAITS_HPP__
#define __YGGR_MEMORY_EX_ALLOCATOR_TRAITS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>
#include <yggr/type_traits/native_t.hpp>

#include <boost/container/allocator_traits.hpp>
#include <boost/container/detail/version_type.hpp>

namespace yggr
{
namespace memory_ex
{
namespace detail
{

// segment_manager
template<typename Container>
struct _has_segment_manager_t 
{
private:
	typedef Container cont_type;

	typedef u8 true_type;
	typedef u32 false_type;
		
	template<typename T>
	static true_type chk_foo(typename ::yggr::native_t<typename T::segment_manager>::type*); // If T's parent defines the value_type then it is true
		
	template<typename T>
	static false_type chk_foo(...);
		
public:
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type;
};

template<typename Container>
struct has_segment_manager_t 
	: public _has_segment_manager_t<Container>::type
{
};

template<typename T, bool has_segment_manager>
struct allocator_segment_manager_t_impl
	: public mplex::null_type
{
};

template<typename T>
struct allocator_segment_manager_t_impl<T, true>
{
	typedef typename T::segment_manager type;
};

template<typename T>
struct allocator_segment_manager_t
	: public 
		allocator_segment_manager_t_impl
		<
			T,
			has_segment_manager_t<T>::value
		>
{
};

// version
template<typename Container>
struct _has_version_t 
{
private:
	typedef Container cont_type;

	typedef u8 true_type;
	typedef u32 false_type;
		
	template<typename T>
	static true_type chk_foo(typename ::yggr::native_t<typename T::version>::type*); // If T's parent defines the value_type then it is true
		
	template<typename T>
	static false_type chk_foo(...);
		
public:
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type;
};

template<typename Container>
struct has_version_t 
	: public _has_version_t<Container>::type
{
};

template<typename T, bool has_version>
struct allocator_version_t_impl
{
#if (BOOST_VERSION < 106700)
	typedef boost::container::container_detail::version_type<T, 0> type;
#else
	typedef boost::container::dtl::version_type<T, 0> type;
#endif // (BOOST_VERSION < 106700)

};

template<typename T>
struct allocator_version_t_impl<T, true>
{
	typedef typename T::version type;
};

template<typename T>
struct allocator_version_t
	: public 
		allocator_version_t_impl
		<
			T,
			has_version_t<T>::value
		>
{
};

} // namespace detail
} // namespace memory_ex
} // namespace yggr

namespace yggr
{
namespace memory_ex
{

template<typename Alloc>
struct allocator_traits
	: public boost::container::allocator_traits<Alloc>
{
public:
	typedef mplex::null_type segment_manager;

#if (BOOST_VERSION < 106700)
	typedef boost::container::container_detail::version_type<Alloc, 0> version;
#else
	typedef boost::container::dtl::version_type<Alloc, 0> version;
#endif // (BOOST_VERSION < 106700)
};

template<template<typename _T1, typename T2> class TPLAlloc, typename T1, typename T2>
struct allocator_traits< TPLAlloc<T1, T2> >
	: public boost::container::allocator_traits< TPLAlloc<T1, T2> >
{
private:
	typedef boost::container::allocator_traits< TPLAlloc<T1, T2> > base_type;
	typedef typename base_type::allocator_type base_allocator_type;

public:
	typedef typename detail::allocator_segment_manager_t<base_allocator_type>::type segment_manager;
	typedef typename detail::allocator_version_t<base_allocator_type>::type version;
};

} // namespace memory_ex

using memory_ex::allocator_traits;

} // namespace yggr

#endif // __YGGR_MEMORY_EX_ALLOCATOR_TRAITS_HPP__
