// is_iterator.hpp

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

#ifndef __YGGR_ITERATOR_EX_IS_ITERATOR_HPP__
#define __YGGR_ITERATOR_EX_IS_ITERATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/compatibility/boost_iterator.hpp>
#include <yggr/iterator_ex/has_iterator_category.hpp>
#include <yggr/iterator_ex/insert_iterator.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <iterator>

namespace yggr
{
namespace iterator_ex
{

template<typename T>
struct is_iterator
	: public 
		boost::mpl::bool_
		<
			boost::mpl::or_
			<
				boost::is_pointer<T>,
				has_iterator_category<T>
			>::type::value
		>::type
{
};

} // namespace iterator_ex

using iterator_ex::is_iterator;

} // namespace yggr

#endif // __YGGR_ITERATOR_EX_IS_ITERATOR_HPP__
