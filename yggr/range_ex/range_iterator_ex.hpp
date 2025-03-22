// range_iterator_ex.hpp

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

#ifndef __YGGR_RANGE_EX_RANGE_ITERATOR_EX_HPP__
#define __YGGR_RANGE_EX_RANGE_ITERATOR_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>

#include <boost/range/iterator.hpp>
#include <boost/range/has_range_iterator.hpp>

namespace yggr
{
namespace range_ex
{

namespace detail
{

template<typename T, bool has_iter = boost::has_range_iterator<T>::value>
struct range_iterator_ex_impl;

template<typename T>
struct range_iterator_ex_impl<T, true>
	: public boost::range_iterator<T>
{
};

template<typename T>
struct range_iterator_ex_impl<T, false>
	: public mplex::null_type
{
};

template<typename T, bool has_iter = boost::has_range_const_iterator<T>::value>
struct range_const_iterator_ex_impl;

template<typename T>
struct range_const_iterator_ex_impl<T, true>
	: public boost::range_const_iterator<T>
{
};

template<typename T>
struct range_const_iterator_ex_impl<T, false>
	: public mplex::null_type
{
};

} // namespace detail

template<typename T>
struct range_iterator_ex 
	: public detail::range_iterator_ex_impl<T>
{
};	

template<typename T>
struct range_const_iterator_ex 
	: public detail::range_const_iterator_ex_impl<T>
{
};	

} // namespace range_ex
} // namespace yggr

#endif // __YGGR_RANGE_EX_RANGE_ITERATOR_EX_HPP__
