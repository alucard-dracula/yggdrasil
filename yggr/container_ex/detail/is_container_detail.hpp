//is_container_detail.hpp

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

#ifndef __YGGR_CONTAINER_EX_DETAIL_IS_CONTAINER_DETAIL_HPP__
#define __YGGR_CONTAINER_EX_DETAIL_IS_CONTAINER_DETAIL_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <yggr/container_ex/has_value_t.hpp>
#include <yggr/container_ex/has_iterator_t.hpp>
#include <yggr/container_ex/has_allocator_t.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>

#include <utility>

namespace yggr
{
namespace container_ex
{
namespace detail
{

template<typename T>
struct is_container_helper
	: public 
		boost::mpl::and_
		<
			has_value_t<T>,
			has_iterator_t<T>,
			has_allocator_t<T>
		>::type
{
};

// type val[m]
template<typename T, std::size_t N >
struct is_container_helper< T[N] >
	: public boost::mpl::true_
{
};

// like boost::array
template<typename T, std::size_t N,
			template<typename _T, std::size_t _N> class C>
struct is_container_helper< C<T, N> >
	: public 
		boost::mpl::and_
		<
			typename has_value_t< C<T, N> >::type,
			typename has_iterator_t< C<T, N> >::type 
		>::type
{
};

} // namespace detail
} // namespace container_ex
} // namespace yggr

#endif // __YGGR_TYPE_TRAITS_DETAIL_IS_CONTAINER_DETAIL_HPP__