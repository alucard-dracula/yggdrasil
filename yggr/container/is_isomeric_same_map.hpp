// is_isomeric_same_map.hpp

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

#ifndef __YGGR_CONTAINER_IS_ISOMERIC_SAME_MAP_HPP__
#define __YGGR_CONTAINER_IS_ISOMERIC_SAME_MAP_HPP__

/*
boost 1.70 and later boost::container::map's sec tpl arg's default value is void
and some function not support other boost::container::basic_string tpl,
so string = basic_string<char, traits, default_allocator> is compile failed.
*/

#include <yggr/type_traits/is_same_tpl.hpp>
#include <yggr/mplex/typename_expand.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/container_ex/allocator_t.hpp>
#include <yggr/container_ex/container_compare_t.hpp>

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

// is_isomeric_same_map
template<typename MapL, typename MapR, 
			bool is_same_tpl_v = is_same_tpl<MapL, MapR>::value,
			std::size_t tpl_args_size = mplex::typename_expand<MapL>::size_type::value>
struct _is_isomeric_same_map_detail;

template<typename MapL, typename MapR, std::size_t tpl_args_size>
struct _is_isomeric_same_map_detail<MapL, MapR, false, tpl_args_size>
	: public boost::mpl::false_
{
};

template<typename MapL, typename MapR>
struct _is_isomeric_same_map_detail<MapL, MapR, true, 0>
	: public boost::mpl::false_
{
};

template<typename MapL, typename MapR>
struct _is_isomeric_same_map_detail<MapL, MapR, true, 1>
	: public boost::mpl::false_
{
};

template<typename MapL, typename MapR>
struct _is_isomeric_same_map_detail<MapL, MapR, true, 2>
	: public boost::mpl::false_
{
};

template<typename MapL, typename MapR>
struct _is_isomeric_same_map_detail<MapL, MapR, true, 3>
	: public boost::mpl::false_
{
};

template<typename MapL, typename MapR>
struct _is_isomeric_same_map_detail<MapL, MapR, true, 4>
	: public
		boost::mpl::and_
		<
			boost::mpl::and_
			<
				boost::mpl::not_
				<
					boost::is_same
					<
						typename mplex::typename_expand_get<MapL, 3>::type,
						typename mplex::typename_expand_get<MapR, 3>::type
					>
				>,
				boost::is_same
				<
					typename range_ex::range_value_ex<MapL>::type,
					typename range_ex::range_value_ex<MapR>::type
				>
			>,
			boost::mpl::and_
			<
				boost::is_same
				<
					typename container_ex::container_key_compare_t<MapL>::type,
					typename container_ex::container_key_compare_t<MapR>::type
				>,
				boost::is_same
				<
					typename container_ex::allocator_t<MapL>::type,
					typename container_ex::allocator_t<MapR>::type
				>
			>
		>::type
{
};

template<typename MapL, typename MapR, std::size_t tpl_args_size>
struct _is_isomeric_same_map_detail<MapL, MapR, true, tpl_args_size>
	: public _is_isomeric_same_map_detail<MapL, MapR, true, 4>
{
};

template<typename MapL, typename MapR>
struct _is_isomeric_same_map
	: public _is_isomeric_same_map_detail<MapL, MapR>::type
{
};

} // namespace detail

template<typename MapL, typename MapR>
struct is_isomeric_same_map
	: public detail::_is_isomeric_same_map<MapL, MapR>
{
};

template<typename MapL, typename MapR>
struct is_same_or_isomeric_same_map
	: public
		boost::mpl::or_
		<
			boost::is_same<MapL, MapR>,
			is_isomeric_same_map<MapL, MapR>
		>
{
};

template<typename MapL, typename MapR>
struct is_isomeric_same_multimap
	: public detail::_is_isomeric_same_map<MapL, MapR>
{
};

template<typename MapL, typename MapR>
struct is_same_or_isomeric_same_multimap
	: public
		boost::mpl::or_
		<
			boost::is_same<MapL, MapR>,
			is_isomeric_same_multimap<MapL, MapR>
		>
{
};

} // namespace container

using container::is_isomeric_same_map;
using container::is_same_or_isomeric_same_map;

using container::is_isomeric_same_multimap;
using container::is_same_or_isomeric_same_multimap;

} // namespace yggr

#endif // __YGGR_CONTAINER_IS_ISOMERIC_SAME_MAP_HPP__
