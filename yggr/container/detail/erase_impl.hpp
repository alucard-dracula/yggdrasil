//erase_impl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_ERASE_IMPL_HPP__
#define __YGGR_CONTAINER_DETAIL_ERASE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container/size.hpp>
#include <yggr/container_ex/container_category_check.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>
#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <boost/utility/enable_if.hpp>

#include <algorithm>


namespace yggr
{
namespace container
{
namespace detail
{

template<typename Container, typename Cmper>
typename
	boost::enable_if
	<
		container_ex::is_random_access_container<Container>,
		typename boost::range_size<Container>::type
	>::type
   erase_if_impl(Container& c, Cmper cmper)
{
	typedef Container cont_type;
	typedef typename boost::range_size<cont_type>::type size_type;
	typedef typename range_ex::range_iterator_ex<cont_type>::type iter_type;

	iter_type iter = std::remove_if(boost::begin(c), boost::end(c), cmper);
	size_type rm_size = iterator_ex::distance(iter, boost::end(c));
	c.erase(iter, boost::end(c));
	return rm_size;
}

#if !defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename Container, typename Cmper>
typename
	boost::disable_if
	<
		container_ex::is_random_access_container<Container>,
		typename boost::range_size<Container>::type
	>::type
   erase_if_impl(Container& c, Cmper cmper)
{
	typedef Container cont_type;
	typedef typename boost::range_size<cont_type>::type size_type;
	typedef typename range_ex::range_iterator_ex<cont_type>::type iter_type;

	size_type old_size = ::yggr::size(c);
	iter_type iter_next;
	for(iter_type i = boost::begin(c), isize = boost::end(c); i != isize; )
	{
		if(cmper(*i))
		{
		    iter_next = yggr::advance_copy(i, 1);
			c.erase(i);
			i = iter_next;
		}
		else
		{
			iterator_ex::advance(i, 1);
		}
	}

	assert(::yggr::size(c) <= old_size);
	return old_size - ::yggr::size(c);
}


#else

template<typename Container, typename Cmper>
typename
	boost::disable_if
	<
		container_ex::is_random_access_container<Container>,
		typename boost::range_size<Container>::type
	>::type
   erase_if_impl(Container& c, Cmper cmper)
{
	typedef Container cont_type;
	typedef typename boost::range_size<cont_type>::type size_type;
	typedef typename range_ex::range_iterator_ex<cont_type>::type iter_type;

	size_type old_size = ::yggr::size(c);
	for(iter_type i = boost::begin(c), isize = boost::end(c); i != isize; )
	{
		if(cmper(*i))
		{
			i = c.erase(i);
		}
		else
		{
			iterator_ex::advance(i, 1);
		}
	}

	assert(::yggr::size(c) <= old_size);
	return old_size - ::yggr::size(c);
}

#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename Container, typename Value>
typename
	boost::enable_if
	<
		container_ex::is_random_access_container<Container>,
		typename boost::range_size<Container>::type
	>::type
   erase_impl(Container& c, const Value& val)
{
	typedef Container cont_type;
	typedef typename boost::range_size<cont_type>::type size_type;
	typedef typename range_ex::range_iterator_ex<cont_type>::type iter_type;

	iter_type iter = std::remove(boost::begin(c), boost::end(c), val);
	size_type rm_size = iterator_ex::distance(iter, boost::end(c));
	c.erase(iter, boost::end(c));
	return rm_size;
}

template<typename Container, typename Value>
typename
	boost::disable_if
	<
		container_ex::is_random_access_container<Container>,
		typename boost::range_size<Container>::type
	>::type
   erase_impl(Container& c, const Value& val)
{
	typedef Container cont_type;
	typedef Value value_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_val_type;

	struct now_cmper
	{
	public:
		now_cmper(const value_type& cmp_val)
			: _cmp_val(cmp_val)
		{
		}

	public:
		inline bool operator()(const cont_val_type& val) const
		{
			return _cmp_val == val;
		}

	private:
		const value_type& _cmp_val;
	};

	return erase_if_impl(c, now_cmper(val));
}


} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_ERASE_IMPL_HPP__
