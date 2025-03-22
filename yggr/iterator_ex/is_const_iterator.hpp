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

#ifndef __YGGR_ITERATOR_EX_IS_CONST_ITERATOR_HPP__
#define __YGGR_ITERATOR_EX_IS_CONST_ITERATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/compatibility/boost_iterator.hpp>
#include <yggr/iterator_ex/has_iterator_category.hpp>

#include <boost/mpl/bool.hpp>

#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <iterator>

namespace yggr
{
namespace iterator_ex
{
namespace detail
{

template<typename T, bool is_pointer, bool is_iter>
struct _is_const_iterator_t
	: public boost::mpl::false_
{
};

template<typename T>
struct _is_const_iterator_t<T, true, false>
	: public
		boost::mpl::bool_
		<
			boost::is_const
			<
				typename boost::remove_pointer<T>::type
			>::value
		>
{
};

template<typename T>
struct _is_const_iterator_t<T, false, true>
	: public
		boost::mpl::bool_
		<
			boost::is_const
			<
				typename boost::remove_pointer<typename T::pointer>::type
			>::value
		>
{
};

} // namespace detail
} // namespace iterator_ex
} // namespace yggr

namespace yggr
{
namespace iterator_ex
{

template<typename T>
struct is_const_iterator
	: public 
		detail::_is_const_iterator_t
		<
			T,
			!!(boost::is_pointer<T>::value),
			!!(has_iterator_category<T>::value)
		>::type
{
};

} // namespace iterator_ex

using iterator_ex::is_const_iterator;

} // namespace yggr

#endif // __YGGR_ITERATOR_EX_IS_CONST_ITERATOR_HPP__
