// container_category_check.hpp

/****************************************************************************
Copyright (c) 2014-2022 yggdrasil

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

#ifndef __YGGR_CONTAINER_EX_CONTAINER_CATEGORY_CHECK_HPP__
#define __YGGR_CONTAINER_EX_CONTAINER_CATEGORY_CHECK_HPP__

#include <yggr/container_ex/is_container.hpp>
#include <yggr/iterator_ex/iterator_category_check.hpp>

namespace yggr
{
namespace container_ex
{
namespace detail
{

template<typename T, bool is_container = is_container<T>::value >
struct container_category_ex_detail;

template<typename T>
struct container_category_ex_detail<T, false>
	: public mplex::null_type
{
};

template<typename T>
struct container_category_ex_detail<T, true>
	: public 
		boost::iterator_category
		<
			typename boost::range_iterator<T>::type
		>
{
};

} // namespace detail

template<typename T>
struct container_category_ex
	: public detail::container_category_ex_detail<T>
{
};

struct containter_category_check_result
	: public iterator_category_check_result
{
};

template<typename T>
struct is_random_access_container
	: public 
		boost::mpl::bool_
		<
			boost::is_base_of
			<
				std::random_access_iterator_tag,
				typename container_category_ex<T>::type
			>::value
		>::type
{
};

template<typename T>
struct is_bidirectional_container
	: public 
		boost::mpl::bool_
		<
			boost::is_base_of
			<
				std::bidirectional_iterator_tag,
				typename container_category_ex<T>::type
			>::value
		>::type
{
};

template<typename T>
struct is_forward_container
	: public
		boost::mpl::bool_
		<
			boost::is_base_of
			<
				std::forward_iterator_tag,
				typename container_category_ex<T>::type
			>::value
		>::type
{
};

} // namespace container_ex

using container_ex::container_category_ex;
using container_ex::containter_category_check_result;

using container_ex::is_random_access_container;
using container_ex::is_bidirectional_container;
using container_ex::is_forward_container;


} // namespace yggr

#endif // __YGGR_CONTAINER_EX_CONTAINER_CATEGORY_CHECK_HPP__
