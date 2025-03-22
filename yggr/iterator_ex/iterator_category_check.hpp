//iterator_category_check.hpp

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

#ifndef __YGGR_ITERATOR_EX_ITERATOR_CATEGORY_CHECK_HPP__
#define __YGGR_ITERATOR_EX_ITERATOR_CATEGORY_CHECK_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/mplex/null_t.hpp>

#include <yggr/iterator_ex/iterator_traits.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

#include <iterator>

namespace yggr
{
namespace iterator_ex
{
// iterator_category inherit figure
// input  <---- forward <---- bidirectional <----random_access
// output <----

struct iterator_category_check_result
{
	//unified text, because boost has multi true_type and false_type
	typedef boost::mpl::true_ true_type;
	typedef boost::mpl::false_ false_type;

	YGGR_STATIC_CONSTANT(bool, value_false = false_type::value);
	YGGR_STATIC_CONSTANT(bool, value_true = true_type::value);
};

// is_random_access_iterator
template<typename T>
struct is_random_access_iterator
	: public 
		boost::mpl::bool_
		<
			boost::is_base_of
			<
				std::random_access_iterator_tag,
				typename iterator_category_ex<T>::type
			>::value
		>::type
						
{
};

// is_bidirectional_iterator
template<typename T>
struct is_bidirectional_iterator
	: public 
		boost::mpl::bool_
		<
			boost::is_base_of
			<
				std::bidirectional_iterator_tag,
				typename iterator_category_ex<T>::type
			>::value
		>::type
						
{
};

// is_forward_iterator
template<typename T>
struct is_forward_iterator
	: public
		boost::mpl::bool_
		<
			boost::is_base_of
			<
				std::forward_iterator_tag,
				typename iterator_category_ex<T>::type
			>::value
		>::type
						
{
};

// is_output_iterator
template<typename T>
struct is_output_iterator
	: public 
		boost::mpl::bool_
		<
			boost::is_base_of
			<
				std::output_iterator_tag,
				typename iterator_category_ex<T>::type
			>::value
		>::type
						
{
};

// is_input_iterator
template<typename T>
struct is_input_iterator
	: public
		boost::mpl::bool_
		<
			boost::is_base_of
			<
				std::input_iterator_tag,
				typename iterator_category_ex<T>::type
			>::value
		>::type
						
{
};

} // namespace iterator_ex

using iterator_ex::iterator_category_check_result;

using iterator_ex::is_random_access_iterator;
using iterator_ex::is_bidirectional_iterator;
using iterator_ex::is_forward_iterator;
using iterator_ex::is_output_iterator;
using iterator_ex::is_input_iterator;

} // namespace yggr

#endif // __YGGR_ITERATOR_EX_ITERATOR_CATEGORY_CHECK_HPP__

