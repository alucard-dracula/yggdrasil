// is_isomeric_same_list.hpp

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

#ifndef __YGGR_CONTAINER_IS_ISOMERIC_SAME_LIST_HPP__
#define __YGGR_CONTAINER_IS_ISOMERIC_SAME_LIST_HPP__

/*
boost 1.70 and later boost::container::vector's sec tpl arg's default value is void
and some function not support other boost::container::basic_string tpl,
so string = basic_string<char, traits, default_allocator> is compile failed.

*/

#include <yggr/type_traits/is_same_tpl.hpp>
#include <yggr/mplex/typename_expand.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/container_ex/allocator_t.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

// is_isomeric_same_list
template<typename LstL, typename LstR, 
			bool is_same_tpl_v = is_same_tpl<LstL, LstR>::value,
			std::size_t tpl_args_size = mplex::typename_expand<LstL>::size_type::value>
struct _is_isomeric_same_list_detail;

template<typename LstL, typename LstR, std::size_t tpl_args_size>
struct _is_isomeric_same_list_detail<LstL, LstR, false, tpl_args_size>
	: public boost::mpl::false_
{
};

template<typename LstL, typename LstR>
struct _is_isomeric_same_list_detail<LstL, LstR, true, 0>
	: public boost::mpl::false_
{
};

template<typename LstL, typename LstR>
struct _is_isomeric_same_list_detail<LstL, LstR, true, 1>
	: public boost::mpl::false_
{
};

template<typename LstL, typename LstR>
struct _is_isomeric_same_list_detail<LstL, LstR, true, 2>
	: public
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::is_same
				<
					typename mplex::typename_expand_get<LstL, 1>::type,
					typename mplex::typename_expand_get<LstR, 1>::type
				>
			>,
			boost::is_same
			<
				typename range_ex::range_value_ex<LstL>::type,
				typename range_ex::range_value_ex<LstR>::type
			>,
			boost::is_same
			<
				typename container_ex::allocator_t<LstL>::type,
				typename container_ex::allocator_t<LstR>::type
			>
		>::type
{
};

template<typename LstL, typename LstR, std::size_t tpl_args_size>
struct _is_isomeric_same_list_detail<LstL, LstR, true, tpl_args_size>
	: public _is_isomeric_same_list_detail<LstL, LstR, true, 2>
{
};

template<typename LstL, typename LstR>
struct _is_isomeric_same_list
	: public _is_isomeric_same_list_detail<LstL, LstR>::type
{
};

} // namespace detail

template<typename LstL, typename LstR>
struct is_isomeric_same_list
	: public detail::_is_isomeric_same_list<LstL, LstR>
{
};

template<typename LstL, typename LstR>
struct is_same_or_isomeric_same_list
	: public
		boost::mpl::or_
		<
			boost::is_same<LstL, LstR>,
			is_isomeric_same_list<LstL, LstR>
		>
{
};

} // namespace container

using container::is_isomeric_same_list;
using container::is_same_or_isomeric_same_list;

} // namespace yggr

#endif // __YGGR_CONTAINER_IS_ISOMERIC_SAME_LIST_HPP__
