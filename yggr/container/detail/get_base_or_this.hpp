//get_base_or_this.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_GET_BASE_OR_THIS_HPP__
#define __YGGR_CONTAINER_DETAIL_GET_BASE_OR_THIS_HPP__

#include <yggr/type_traits/has_base_t.hpp>
#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T, bool b_has_base = has_base_t<T>::value>
struct get_base_or_this_t_detail;

template<typename T>
struct get_base_or_this_t_detail<T, true>
{
	typedef typename T::base_type type;
};

template<typename T>
struct get_base_or_this_t_detail<T, false>
{
	typedef T type;
};

template<typename T>
struct get_base_or_this_t
	: public get_base_or_this_t_detail<T>
{
};

template<typename T> inline
typename
	boost::enable_if
	<
		has_base_t<T>,
		typename T::base_type&
	>::type
	get_base_or_this(T& t)
{
	typedef typename T::base_type base_type;
	return static_cast<base_type&>(t);
}

template<typename T> inline
typename
	boost::enable_if
	<
		has_base_t<T>,
		const typename T::base_type&
	>::type
	get_base_or_this(const T& t)
{
	typedef typename T::base_type base_type;
	return static_cast<const base_type&>(t);
}

template<typename T> inline
typename
	boost::disable_if
	<
		has_base_t<T>,
		T&
	>::type
	get_base_or_this(T& t)
{
	return t;
}

template<typename T> inline
typename
	boost::disable_if
	<
		has_base_t<T>,
		const T&
	>::type
	get_base_or_this(const T& t)
{
	return t;
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_GET_BASE_OR_THIS_HPP__
