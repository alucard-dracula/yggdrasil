//output_iterator_value.hpp

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

#ifndef __YGGR_ITERATOR_EX_OUTPUT_ITERATOR_VALUE_HPP__
#define __YGGR_ITERATOR_EX_OUTPUT_ITERATOR_VALUE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>

#include <yggr/type_traits/has_value_t.hpp>
#include <yggr/type_traits/has_char_t.hpp>
#include <yggr/iterator_ex/iterator_category_check.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace iterator_ex
{
namespace detail
{

template<typename T, 
			bool is_output_iter = is_output_iterator<T>::value, 
			bool has_value_t = has_value_t<T>::value, 
			bool has_char_t = has_char_t<T>::value>
struct _output_iterator_value_t
	: public mplex::null_type
{
};

template<typename T>
struct _output_iterator_value_t<T, true, false, false>
	: public mplex::null_type
{
};

template<typename T>
struct _output_iterator_value_t<T, true, true, false>
	: public
		boost::mpl::if_
		<
			boost::is_same<typename T::value_type, void>,
			mplex::null_type,
			typename T::value_type
		>
{
};

template<typename T>
struct _output_iterator_value_t<T, true, false, true>
	: public
		boost::mpl::if_
		<
			boost::is_same<typename T::char_type, void>,
			mplex::null_type,
			typename T::char_type
		>
{
};

template<typename T>
struct _output_iterator_value_t<T, true, true, true>
	: public 
		_output_iterator_value_t
		<
			T, true,
			!boost::is_same<typename T::value_type, void>::value,
			!boost::is_same<typename T::char_type, void>::value
		>
{
};

} // namesapce detail

template<typename T>
struct output_iterator_value
	: public detail::_output_iterator_value_t<T>
{
};


} // namespace iterator_ex

using iterator_ex::output_iterator_value;

} // namespace yggr

#endif // __YGGR_ITERATOR_EX_OUTPUT_ITERATOR_VALUE_HPP__
