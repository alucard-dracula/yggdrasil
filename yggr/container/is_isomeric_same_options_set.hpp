// is_isomeric_same_options_set.hpp

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

#ifndef __YGGR_CONTAINER_IS_ISOMERIC_SAME_OPTIONS_SET_HPP__
#define __YGGR_CONTAINER_IS_ISOMERIC_SAME_OPTIONS_SET_HPP__

/*
boost 1.70 and later boost::container::options_set's sec tpl arg's default value is void
and some function not support other boost::container::basic_string tpl,
so string = basic_string<char, traits, default_allocator> is compile failed.
*/

#include <yggr/type_traits/is_same_tpl.hpp>
#include <yggr/mplex/typename_expand.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/container_ex/allocator_t.hpp>
#include <yggr/container_ex/container_compare_t.hpp>

#include <yggr/container/get_set_options.hpp>
#include <yggr/container/is_isomeric_same_set.hpp>

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

// is_isomeric_same_options_set
template<typename SetL, typename SetR, 
			bool is_same_tpl_v = is_same_tpl<SetL, SetR>::value,
			std::size_t tpl_args_size = mplex::typename_expand<SetL>::size_type::value>
struct _is_isomeric_same_options_set_detail;

template<typename SetL, typename SetR, std::size_t tpl_args_size>
struct _is_isomeric_same_options_set_detail<SetL, SetR, false, tpl_args_size>
	: public boost::mpl::false_
{
};

template<typename SetL, typename SetR>
struct _is_isomeric_same_options_set_detail<SetL, SetR, true, 0>
	: public boost::mpl::false_
{
};

template<typename SetL, typename SetR>
struct _is_isomeric_same_options_set_detail<SetL, SetR, true, 1>
	: public boost::mpl::false_
{
};

template<typename SetL, typename SetR>
struct _is_isomeric_same_options_set_detail<SetL, SetR, true, 2>
	: public boost::mpl::false_
{
};

template<typename SetL, typename SetR>
struct _is_isomeric_same_options_set_detail<SetL, SetR, true, 3>
	: public _is_isomeric_same_set_detail<SetL, SetR>::type
{
};


template<typename SetL, typename SetR>
struct _is_isomeric_same_options_set_detail<SetL, SetR, true, 4>
	: public
		boost::mpl::and_
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					boost::mpl::not_
					<
						boost::is_same
						<
							typename mplex::typename_expand_get<SetL, 2>::type,
							typename mplex::typename_expand_get<SetR, 2>::type
						>
					>,
					boost::mpl::not_
					<
						boost::is_same
						<
							typename mplex::typename_expand_get<SetL, 3>::type,
							typename mplex::typename_expand_get<SetR, 3>::type
						>
					>
				>,
				boost::is_same
				<
					typename range_ex::range_value_ex<SetL>::type,
					typename range_ex::range_value_ex<SetR>::type
				>
			>,
			boost::mpl::and_
			<
				boost::is_same
				<
					typename container_ex::container_key_compare_t<SetL>::type,
					typename container_ex::container_key_compare_t<SetR>::type
				>,
				boost::is_same
				<
					typename container_ex::allocator_t<SetL>::type,
					typename container_ex::allocator_t<SetR>::type
				>,
				boost::is_same
				<
					typename 
						detail::get_tree_options
						<
							typename mplex::typename_expand_get<SetL, 3>::type
						>::type,
					typename 
						detail::get_tree_options
						<
							typename mplex::typename_expand_get<SetR, 3>::type
						>::type
				>
			>
		>::type
{
};

template<typename SetL, typename SetR, std::size_t tpl_args_size>
struct _is_isomeric_same_options_set_detail<SetL, SetR, true, tpl_args_size>
	: public _is_isomeric_same_options_set_detail<SetL, SetR, true, 4>
{
};

template<typename SetL, typename SetR>
struct _is_isomeric_same_options_set
	: public _is_isomeric_same_options_set_detail<SetL, SetR>::type
{
};

} // namespace detail

template<typename SetL, typename SetR>
struct is_isomeric_same_options_set
	: public detail::_is_isomeric_same_options_set<SetL, SetR>
{
};

template<typename SetL, typename SetR>
struct is_same_or_isomeric_same_options_set
	: public
		boost::mpl::or_
		<
			boost::is_same<SetL, SetR>,
			is_isomeric_same_options_set<SetL, SetR>
		>
{
};

template<typename SetL, typename SetR>
struct is_isomeric_same_options_multiset
	: public detail::_is_isomeric_same_options_set<SetL, SetR>
{
};

template<typename SetL, typename SetR>
struct is_same_or_isomeric_same_options_multiset
	: public
		boost::mpl::or_
		<
			boost::is_same<SetL, SetR>,
			is_isomeric_same_options_multiset<SetL, SetR>
		>
{
};

} // namespace container

using container::is_isomeric_same_options_set;
using container::is_same_or_isomeric_same_options_set;

using container::is_isomeric_same_options_multiset;
using container::is_same_or_isomeric_same_options_multiset;

} // namespace yggr

#endif // __YGGR_CONTAINER_IS_ISOMERIC_SAME_OPTIONS_SET_HPP__
