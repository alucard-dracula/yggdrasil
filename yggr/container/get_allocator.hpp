//get_allocator.hpp

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

#ifndef __YGGR_CONTAINER_GET_ALLOCATOR_HPP__
#define __YGGR_CONTAINER_GET_ALLOCATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container_ex/allocator_t.hpp>
#include <yggr/container_ex/has_get_allocator_function.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace yggr
{
namespace container
{

template<typename T> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			has_allocator_t<T>,
			has_get_allocator_function<T>
		>,
		typename allocator_t<T>::type
	>::type
	get_allocator(const T& t)
{
	return t.get_allocator();
}

template<typename T> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			has_allocator_t<T>,
			boost::mpl::not_< has_get_allocator_function<T> >
		>,
		typename allocator_t<T>::type
	>::type
	get_allocator(const T& t)
{
	typedef typename allocator_t<T>::type ret_type;
	return ret_type();
}

template<typename T> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_< has_allocator_t<T> >,
			boost::mpl::not_< has_get_allocator_function<T> >
		>,
		typename allocator_t<T>::type
	>::type
	get_allocator(const T& t)
{
	typedef typename allocator_t<T>::type ret_type;
	return ret_type();
}

// create_empty_object_of_allocator
template<typename T> inline 
typename
	boost::enable_if
	<
		boost::is_base_of<mplex::null_type, typename allocator_t<T>::type>,
		T
	>::type
	create_empty_object_of_allocator(const T& t)
{
	return T();
}

template<typename T> inline 
typename
	boost::disable_if
	<
		boost::is_base_of<mplex::null_type, typename allocator_t<T>::type>,
		T
	>::type
	create_empty_object_of_allocator(const T& t)
{
	return T(get_allocator(t));
}

} // namespace container

using container::get_allocator;
using container::create_empty_object_of_allocator;

} // namespace yggr

#endif //__YGGR_CONTAINER_GET_ALLOCATOR_HPP__