// call_insert_helper.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_CALL_INSERT_HELPER_HPP__
#define __YGGR_CONTAINER_DETAIL_CALL_INSERT_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/container/detail/container_function_version_check.hpp>

#include <yggr/container_ex/is_container.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>

#include <iterator>

namespace yggr
{
namespace container
{
namespace detail
{

// return iterator version
template<typename Container, typename T>
struct call_insert_helper
{

	typedef Container container_type;
	typedef typename container_type::size_type size_type;
	typedef typename container_type::value_type value_type;
	typedef typename container_type::iterator iterator;
	typedef T result_type;


	template<typename Iter> inline
	result_type operator()(container_type& cont, Iter pos, size_type n, const value_type& val) const
	{
		return cont.insert(pos, n, val);
	}

	template<typename Iter, typename InputIter> inline
	result_type operator()(container_type& cont, Iter pos, InputIter first, InputIter last) const
	{
		return cont.insert(pos, first, last);
	}
};

// return void version // kv c98
template<typename Container>
struct call_insert_helper<Container, boost::mpl::void_>
{
	typedef Container container_type;
	typedef typename container_type::size_type size_type;
	typedef typename container_type::value_type value_type;
	typedef typename container_type::iterator iterator;
	typedef iterator result_type;

	template<typename Iter> inline
	result_type operator()(container_type& cont, Iter pos, size_type n, const value_type& val) const
	{
		size_type size = ::yggr::distance<Iter>(cont.begin(), pos);
		cont.insert(pos, n, val);
		return ::yggr::advance_copy(cont.begin(), size);
	}

	template<typename Iter, typename InputIter> inline
	result_type operator()(container_type& cont, Iter pos, InputIter first, InputIter last) const
	{
		size_type size = ::yggr::distance<Iter>(cont.begin(), pos);
		cont.insert(pos, first, last);

		return ::yggr::advance_copy(cont.begin(), size);
	}
};

} // namespace detail

template<typename Container, typename Iter> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			::yggr::container_ex::is_container<Container>,
			::yggr::iterator_ex::is_iterator<Iter>
		>, 
		typename Container::iterator 
	>::type
	insert(Container& cont, Iter pos, 
			const typename Container::value_type& val)
{
	typedef Container container_type;
	typedef typename detail::insert_result<container_type>::type result_type;
	typedef detail::call_insert_helper<container_type, result_type> h_type;
	h_type h;
	return h(cont, pos, 1, val);
}

template<typename Container, typename Iter> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			::yggr::container_ex::is_container<Container>,
			::yggr::iterator_ex::is_iterator<Iter>
		>, 
		typename Container::iterator 
	>::type
	insert(Container& cont, Iter pos,
			typename Container::size_type n, 
			const typename Container::value_type& val)
{
	typedef Container container_type;
	typedef typename detail::insert_result<container_type>::type result_type;
	typedef detail::call_insert_helper<container_type, result_type> h_type;
	h_type h;
	return h(cont, pos, n, val);
}

template<typename Container, typename Iter, typename Iter1, typename Iter2> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			::yggr::container_ex::is_container<Container>,
			::yggr::iterator_ex::is_iterator<Iter>,
			::yggr::iterator_ex::is_iterator<Iter1>,
			::yggr::iterator_ex::is_iterator<Iter2>
		>, 
		typename Container::iterator 
	>::type
	insert(Container& cont, Iter pos, Iter1 first, Iter2 last)
{
	typedef Container container_type;
	typedef typename detail::insert_result<container_type>::type result_type;
	typedef detail::call_insert_helper<container_type, result_type> h_type;
	h_type h;
	return h(cont, pos, first, last);
}

} // namespace container
} // namespace yggr

#endif //__YGGR_CONTAINER_DETAIL_CALL_INSERT_HELPER_HPP__
