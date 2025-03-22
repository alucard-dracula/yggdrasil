// is_isomeric_same_options_vector.hpp

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

#ifndef __YGGR_CONTAINER_IS_ISOMERIC_SAME_OPTIONS_VECTOR_HPP__
#define __YGGR_CONTAINER_IS_ISOMERIC_SAME_OPTIONS_VECTOR_HPP__

#include <yggr/type_traits/is_same_tpl.hpp>
#include <yggr/mplex/typename_expand.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/range_ex/range_size_ex.hpp>
#include <yggr/container_ex/allocator_t.hpp>

#include <yggr/container/get_vector_options.hpp>
#include <yggr/container/is_isomeric_same_vector.hpp>

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

// is_isomeric_same_vector
template<typename VtL, typename VtR, 
			bool is_same_tpl_v = is_same_tpl<VtL, VtR>::value,
			std::size_t tpl_args_size = mplex::typename_expand<VtL>::size_type::value>
struct _is_isomeric_same_options_vector_detail;

template<typename VtL, typename VtR, std::size_t tpl_args_size>
struct _is_isomeric_same_options_vector_detail<VtL, VtR, false, tpl_args_size>
	: public boost::mpl::false_
{
};

template<typename VtL, typename VtR>
struct _is_isomeric_same_options_vector_detail<VtL, VtR, true, 0>
	: public boost::mpl::false_
{
};

template<typename VtL, typename VtR>
struct _is_isomeric_same_options_vector_detail<VtL, VtR, true, 1>
	: public boost::mpl::false_
{
};

template<typename VtL, typename VtR>
struct _is_isomeric_same_options_vector_detail<VtL, VtR, true, 2>
	: public _is_isomeric_same_vector_detail<VtL, VtR>::type
{
};

template<typename VtL, typename VtR>
struct _is_isomeric_same_options_vector_detail<VtL, VtR, true, 3>
	: public
		boost::mpl::and_
		<
			boost::mpl::or_
			<
				boost::mpl::not_
				<
					boost::is_same
					<
						typename mplex::typename_expand_get<VtL, 1>::type,
						typename mplex::typename_expand_get<VtR, 1>::type
					>
				>,
				boost::mpl::not_
				<
					boost::is_same
					<
						typename mplex::typename_expand_get<VtL, 2>::type,
						typename mplex::typename_expand_get<VtR, 2>::type
					>
				>
			>,
			boost::is_same
			<
				typename range_ex::range_value_ex<VtL>::type,
				typename range_ex::range_value_ex<VtR>::type
			>,
			boost::is_same
			<
				typename container_ex::allocator_t<VtL>::type,
				typename container_ex::allocator_t<VtR>::type
			>,
			boost::is_same
			<
				typename 
					get_vector_options
					<
						typename mplex::typename_expand_get<VtL, 2>::type,
						typename range_ex::range_size_ex<VtL>::type
					>::type,
				typename 
					get_vector_options
					<
						typename mplex::typename_expand_get<VtR, 2>::type,
						typename range_ex::range_size_ex<VtR>::type
					>::type
			>
		>::type
{
};

template<typename VtL, typename VtR, std::size_t tpl_args_size>
struct _is_isomeric_same_options_vector_detail<VtL, VtR, true, tpl_args_size>
	: public _is_isomeric_same_options_vector_detail<VtL, VtR, true, 3>
{
};

template<typename VtL, typename VtR>
struct _is_isomeric_same_options_vector
	: public _is_isomeric_same_options_vector_detail<VtL, VtR>::type
{
};

} // namespace detail

template<typename VtL, typename VtR>
struct is_isomeric_same_options_vector
	: public detail::_is_isomeric_same_options_vector<VtL, VtR>
{
};

template<typename VtL, typename VtR>
struct is_same_or_isomeric_same_options_vector
	: public 
		boost::mpl::or_
		<
			boost::is_same<VtL, VtR>,
			is_isomeric_same_options_vector<VtL, VtR>
		>
{
};

} // namespace container

using container::is_isomeric_same_options_vector;
using container::is_same_or_isomeric_same_options_vector;

} // namespace yggr

#endif // __YGGR_CONTAINER_IS_ISOMERIC_SAME_OPTIONS_VECTOR_HPP__
