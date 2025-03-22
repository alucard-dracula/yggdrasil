// is_const_iterator.hpp

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

#ifndef __YGGR_ITERATOR_EX_INSERTER_ITERATOR_HPP__
#define __YGGR_ITERATOR_EX_INSERTER_ITERATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/range_ex/range_iterator_ex.hpp>
#include <yggr/range_ex/range_reference_ex.hpp>
#include <yggr/range_ex/range_value_ex.hpp>

#include <yggr/iterator_ex/detail/advance.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_class.hpp>

#include <iterator>
#include <cassert>


namespace yggr
{
namespace iterator_ex
{

//#if defined(_MSC_VER) && (__cplusplus < 202000)
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_14)

namespace detail
{
template<typename Container, typename Iterator, typename Value,
			bool b_is_class = boost::is_class<Container>::value>
struct container_insert_helper;

template<typename Container, typename Iterator, typename Value>
struct container_insert_helper<Container, Iterator, Value, true>
{
public:
	typedef Container container_type;
	typedef Iterator iterator;
	typedef Value value_type;

public:
	iterator operator()(container_type* pcont, iterator iter, BOOST_RV_REF(value_type) val) const
	{
		assert(pcont);
		return pcont->insert(iter, boost::move(val));
	}

	iterator operator()(container_type* pcont, iterator iter, const value_type& val) const
	{
		assert(pcont);
		return pcont->insert(iter, val);
	}

};

template<typename Container, typename Iterator, typename Value>
struct container_insert_helper<Container, Iterator, Value, false>
{
public:
	typedef Container container_type;
	typedef Iterator iterator;
	typedef Value value_type;

public:
	iterator operator()(container_type* pcont, iterator iter, BOOST_RV_REF(value_type) val) const
	{
		*iter = boost::move(val);
		return iter;
	}

	iterator operator()(container_type* pcont, iterator iter, const value_type& val) const
	{
		*iter = val;
		return iter;
	}
};

} // namespace detail

// insert_iterator
template<typename Container>
class insert_iterator
	: public
		std::iterator
		<
			std::output_iterator_tag,
			void,
			void,
			void,
			void
		>
{
public:
	typedef
		std::iterator
		<
			std::output_iterator_tag,
			void,
			void,
			void,
			void
		> base_type;

public:
	typedef Container container_type;

	typedef typename range_ex::range_iterator_ex<container_type>::type iterator;

	typedef typename
		range_ex::range_reference_ex
		<
			typename boost::add_const<container_type>::type
		>::type const_reference;

	typedef typename range_ex::range_value_ex<container_type>::type value_type;

protected:
	typedef
		detail::container_insert_helper
		<
			container_type,
			iterator,
			value_type
		> container_insert_helper_type;

private:
	typedef insert_iterator this_type;

public:
	insert_iterator(container_type& cont, iterator iter)
		: container(boost::addressof(cont)), iter(iter)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(value_type) val)
	{
		// push value into container
		container_insert_helper_type helper;
		iter = helper(container, iter, boost::move(val));
		++iter;
		return *this;
	}

	inline this_type& operator=(const value_type& val)
	{
		container_insert_helper_type helper;
		iter = helper(container, iter, val);
		++iter;
		return *this;
	}

	inline this_type& operator*()
	{
		// pretend to return designated value
		return *this;
	}

	inline this_type& operator++(void)
	{
		// pretend to preincrement
		return *this;
	}

	inline this_type& operator++(int)
	{
		// pretend to postincrement
		return *this;
	}

protected:
	container_type *container;	// pointer to container
	iterator iter;	// iterator into container
};

template<typename Container> inline
insert_iterator<Container>
	inserter(Container& c,
				typename range_ex::range_iterator_ex<Container>::type i)
{
	typedef Container cont_type;

    return insert_iterator<cont_type>(c, i);
}

#else
	using std::insert_iterator;

	using std::inserter;

#endif // __cplusplus < 202000

} // namespace iterator_ex

// don't support back_insert_iterator front_insert_iterator

using iterator_ex::insert_iterator;
using iterator_ex::inserter;

} // namespace yggr

#endif // __YGGR_ITERATOR_EX_INSERTER_ITERATOR_HPP__

