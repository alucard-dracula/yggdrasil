//get_hint_insert_return.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_GET_HINT_INSERT_RETURN_HPP__
#define __YGGR_CONTAINER_DETAIL_GET_HINT_INSERT_RETURN_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/typeof/typeof.hpp>
#include <yggr/mplex/null_t.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/container/detail/insert_return_type_base.hpp>

#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T> YGGR_CONSTEXPR_OR_INLINE
typename boost::enable_if< iterator_ex::is_iterator<T>, T>::type
	get_hint_insert_return(T iter) YGGR_NOEXCEPT
{
	return iter;
}

template<typename T, bool is_iter>
struct hint_insert_return_t_getter_impl;

template<typename T>
struct hint_insert_return_t_getter_impl<T, false>
{
	typedef YGGR_TYPEOF_TPL(declval<T>().position) type;
};

template<typename T>
struct hint_insert_return_t_getter_impl<T, true>
	: public mplex::null_type
{
};

template<typename T>
struct hint_insert_return_t_getter
	: public hint_insert_return_t_getter_impl<T, iterator_ex::is_iterator<T>::value>
{
};


template<typename T> YGGR_CONSTEXPR_OR_INLINE
typename 
	boost::disable_if
	<
		iterator_ex::is_iterator<T>, 
		typename hint_insert_return_t_getter<T>::type
	>::type
	get_hint_insert_return(const T& irt) YGGR_NOEXCEPT
{
	return irt.position;
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_GET_HINT_INSERT_RETURN_HPP__
