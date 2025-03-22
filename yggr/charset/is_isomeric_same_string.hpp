// is_isomeric_same_string.hpp

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


#ifndef __YGGR_CHARSET_IS_ISOMERIC_SAME_STRING_HPP__
#define __YGGR_CHARSET_IS_ISOMERIC_SAME_STRING_HPP__

/*
boost 1.70 and later boost::container::basic_string's trd tpl arg's default value is void
and some function not support other boost::container::basic_string tpl,
so string = basic_string<char, traits, default_allocator> is compile failed.

*/

#include <yggr/base/yggrdef.h>

#include <yggr/type_traits/is_same_tpl.hpp>
#include <yggr/mplex/typename_expand.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/container_ex/allocator_t.hpp>

#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>

namespace yggr
{
namespace charset
{
namespace detail
{

// is_isomeric_same_string

template<typename StringL, typename StringR, 
			bool is_same_tpl_v = is_same_tpl<StringL, StringR>::value,
			std::size_t tpl_args_size = mplex::typename_expand<StringL>::size_type::value>
struct _is_isomeric_same_string_detail;

template<typename StringL, typename StringR, std::size_t tpl_args_size>
struct _is_isomeric_same_string_detail<StringL, StringR, false, tpl_args_size>
	: public boost::mpl::false_
{
};

template<typename StringL, typename StringR>
struct _is_isomeric_same_string_detail<StringL, StringR, true, 0>
	: public boost::mpl::false_
{
};

template<typename StringL, typename StringR>
struct _is_isomeric_same_string_detail<StringL, StringR, true, 1>
	: public boost::mpl::false_
{
};

template<typename StringL, typename StringR>
struct _is_isomeric_same_string_detail<StringL, StringR, true, 2>
	: public boost::mpl::false_
{
};

template<typename StringL, typename StringR>
struct _is_isomeric_same_string_detail<StringL, StringR, true, 3>
	: public
		boost::mpl::and_
		<
			boost::mpl::and_
			<
				boost::mpl::not_
				<
					boost::is_same
					<
						typename mplex::typename_expand_get<StringL, 2>::type,
						typename mplex::typename_expand_get<StringR, 2>::type
					>
				>,
				boost::is_same
				<
					typename range_ex::range_value_ex<StringL>::type,
					typename range_ex::range_value_ex<StringR>::type
				>
			>,
			boost::mpl::and_
			<
				boost::is_same
				<
					typename StringL::traits_type,
					typename StringR::traits_type
				>,
				boost::is_same
				<
					typename container_ex::allocator_t<StringL>::type,
					typename container_ex::allocator_t<StringR>::type
				>
			>
		>::type
{
};

template<typename StringL, typename StringR, std::size_t tpl_args_size>
struct _is_isomeric_same_string_detail<StringL, StringR, true, tpl_args_size>
	: public _is_isomeric_same_string_detail<StringL, StringR, true, 3>
{
};

template<typename StringL, typename StringR>
struct _is_isomeric_same_string
	: public _is_isomeric_same_string_detail<StringL, StringR>::type
{
};

} // namespace detail


template<typename StringL, typename StringR>
struct is_isomeric_same_string
	: public detail::_is_isomeric_same_string<StringL, StringR>
{
};

template<typename StringL, typename StringR>
struct is_same_or_isomeric_same_string
	: public
		boost::mpl::or_
		<
			boost::is_same<StringL, StringR>,
			is_isomeric_same_string<StringL, StringR>
		>
{
};

} // namespace charset

using charset::is_isomeric_same_string;
using charset::is_same_or_isomeric_same_string;

} // namespace yggr

#endif // __YGGR_CHARSET_IS_ISOMERIC_SAME_STRING_HPP__
