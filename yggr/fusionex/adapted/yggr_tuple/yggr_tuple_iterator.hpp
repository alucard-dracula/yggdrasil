//yggr_tuple_iterator.hpp

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

#ifndef __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_ITERATOR_HPP__
#define __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_ITERATOR_HPP__

#include <yggr/tuple/tuple.hpp>

#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/apply.hpp>

namespace boost 
{ 
namespace fusion
{

struct forward_traversal_tag;

namespace detail
{

template <typename T>
struct yggr_tuple_is_empty : mpl::false_ {};

template <>
struct yggr_tuple_is_empty<yggr::tuples::null_type> : mpl::true_ {};

template <>
struct yggr_tuple_is_empty<yggr::tuples::null_type const> : mpl::true_ {};

template <>
struct yggr_tuple_is_empty<yggr::tuples::tuple<> > : mpl::true_ {};

template <>
struct yggr_tuple_is_empty<yggr::tuples::tuple<> const> : mpl::true_ {};

} // namespace detail

template <typename Cons = yggr::tuples::null_type>
struct yggr_tuple_iterator
	: iterator_facade<yggr_tuple_iterator<Cons>, forward_traversal_tag>
{
	typedef Cons cons_type;

	explicit yggr_tuple_iterator(Cons& in_cons)
		: cons(in_cons) 
	{
	}

	Cons& cons;

	template <typename Iterator>
	struct value_of 
		: mpl::identity<typename Iterator::cons_type::head_type> 
	{
	};

	template <typename Iterator>
	struct deref
	{
		typedef typename value_of<Iterator>::type element;

		typedef typename 
			mpl::if_
			<
				is_const<typename Iterator::cons_type>
				, typename yggr::tuples::access_traits<element>::const_type
				, typename yggr::tuples::access_traits<element>::non_const_type
			>::type type;

		inline static type call(Iterator const& iter)
		{
			return iter.cons.get_head();
		}
	};

	template <typename Iterator>
	struct next
	{
		typedef typename Iterator::cons_type cons_type;
		typedef typename cons_type::tail_type tail_type;

		typedef 
			yggr_tuple_iterator
			<
				typename mpl::eval_if
						<
							is_const<cons_type>
							, add_const<tail_type>
							, mpl::identity<tail_type>
						>::type 
			> type;

		inline static type call(Iterator const& iter)
		{
			return type(iter.cons.get_tail());
		}
	};
        
	template <typename I1, typename I2>
	struct distance;

	// detail

	template <typename I1, typename I2>
	struct lazy_next_distance
		: public 
			mpl::plus
			<
				mpl::int_<1>,
				typename distance
				<
					typename next<I1>::type,
					I2
				>::type
			>
	{
	};
        
	template <typename I1, typename I2>
	struct distance
	{
		typedef typename 
			mpl::eval_if
			<
				boost::is_same<I1, I2>,
				mpl::int_<0>,
				lazy_next_distance<I1, I2>
			>::type type;
            
		inline static type call(I1 const&, I2 const&)
		{
			return type();
		}
	};

private:
	// silence MSVC warning C4512: assignment operator could not be generated
	yggr_tuple_iterator& operator= (yggr_tuple_iterator const&);
};

template<typename Null>
struct yggr_tuple_null_iterator
	: iterator_facade
		<
			yggr_tuple_iterator<Null>, 
			forward_traversal_tag
		>
{
	typedef Null cons_type;

	template <typename I1, typename I2>
	struct equal_to
		: mpl::or_
			<
				is_same<I1, I2>, 
				mpl::and_
				<
					detail::yggr_tuple_is_empty<typename I1::cons_type>,
					detail::yggr_tuple_is_empty<typename I2::cons_type>
				>
			>
	{
	};
};

template <>
struct yggr_tuple_iterator<yggr::tuples::null_type>
	: yggr_tuple_null_iterator<yggr::tuples::null_type>
{
	template <typename Cons>
	explicit yggr_tuple_iterator(Cons const&) 
	{
	}
};

template <>
struct yggr_tuple_iterator<yggr::tuples::null_type const>
	: yggr_tuple_null_iterator<yggr::tuples::null_type const>
{
	template <typename Cons>
	explicit yggr_tuple_iterator(Cons const&) 
	{
	}
};

template <>
struct yggr_tuple_iterator<yggr::tuples::tuple<> >
	: yggr_tuple_null_iterator<yggr::tuples::tuple<> >
{
	template <typename Cons>
	explicit yggr_tuple_iterator(Cons const&)
	{
	}
};

template <>
struct yggr_tuple_iterator<yggr::tuples::tuple<> const>
	: yggr_tuple_null_iterator<yggr::tuples::tuple<> const>
{
	template <typename Cons>
	explicit yggr_tuple_iterator(Cons const&) 
	{
	}
};

} // namespace fusion
} // namespace boost

#endif // __YGGR_FUSIONEX_ADAPTED_YGGR_TUPLE_ITERATOR_HPP__


