// call_erase_helper.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_CALL_ERASE_HELPER_HPP__
#define __YGGR_CONTAINER_DETAIL_CALL_ERASE_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/ppex/has_member_check.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/container/detail/container_function_version_check.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>


namespace yggr
{
namespace container
{
namespace detail
{

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define _FOO_PARAMS() YGGR_PP_CONCAT(typename T::iterator) // c99 version
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS) // not c99 version
#	define  _ARGS typename T::iterator 
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

YGGR_PP_HAS_MEMBER_FOO_DEF(erase_iterator, T, typename T::iterator, erase, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())

#undef _FOO_PARAMS
#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
#	undef _ARGS
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define _FOO_PARAMS() YGGR_PP_CONCAT(typename T::iterator, typename T::iterator) // c99 version
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS) // not c99 version
#	define  _ARGS typename T::iterator, typename T::iterator
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

YGGR_PP_HAS_MEMBER_FOO_DEF(erase_range, T, typename T::iterator, erase, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())

#undef _FOO_PARAMS
#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
#	undef _ARGS
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

// return iterator version
template<typename Container, typename T>
struct call_erase_helper
{

	typedef Container container_type;
	typedef typename container_type::iterator iterator;
	typedef typename container_type::const_iterator const_iterator;
	typedef T result_type;

	template<typename Iter> inline
	result_type operator()(container_type& cont, Iter i) const
	{
		typedef typename 
			boost::mpl::if_c
			<
				has_erase_iterator<container_type>::value,
				iterator,
				const_iterator
			>::type iter_type;
		return cont.erase(iter_type(i));
	}

	template<typename Iter1, typename Iter2> inline
	result_type operator()(container_type& cont, Iter1 first, Iter2 last) const
	{
		typedef typename 
			boost::mpl::if_c
			<
				has_erase_range<container_type>::value,
				iterator,
				const_iterator
			>::type iter_type;
		return cont.erase(iter_type(first), iter_type(last));
	}
};

// return void version // kv c98
template<typename Container>
struct call_erase_helper<Container, boost::mpl::void_>
{
	typedef Container container_type;
	typedef typename container_type::iterator iterator;
	typedef iterator result_type;

	inline result_type operator()(container_type& cont, iterator i) const
	{
		iterator iter_next = i;
		std::advance(iter_next, static_cast<std::size_t>(iter_next != cont.end()));

		// if i is end a error has occurred
		// but at this place don't fix it, 
		//  because STL is so designed
		return (cont.erase(i), iter_next); 
	}

	inline result_type operator()(container_type& cont, iterator first, iterator last) const
	{
		return (cont.erase(first, last), last);
	}
};

} // namespace detail

template<typename Container, typename Iter> inline
typename 
	boost::enable_if
	<
		iterator_ex::is_iterator<Iter>,
		typename Container::iterator
	>::type
	erase(Container& cont, Iter i)
{
	typedef Container container_type;
	typedef typename detail::erase_result<container_type>::type result_type;
	typedef detail::call_erase_helper<container_type, result_type> h_type;
	h_type h;
	return h(cont, i);
}

template<typename Container, typename Iter1, typename Iter2> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			iterator_ex::is_iterator<Iter1>,
			iterator_ex::is_iterator<Iter2>
		>,
		typename Container::iterator
	>::type
	erase(Container& cont, Iter1 first, Iter2 last)
{
	typedef Container container_type;
	typedef typename detail::erase_result<container_type>::type result_type;
	typedef detail::call_erase_helper<container_type, result_type> h_type;
	h_type h;
	return h(cont, first, last);
}

} // namespace container
} // namespace yggr

#endif //__YGGR_CONTAINER_DETAIL_CALL_ERASE_HELPER_HPP__
