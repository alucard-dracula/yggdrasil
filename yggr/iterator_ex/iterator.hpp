// iterator.hpp

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

#ifndef __YGGR_ITERATOR_EX_ITERATOR_HPP__
#define __YGGR_ITERATOR_EX_ITERATOR_HPP__

#include <yggr/iterator_ex/detail/advance.hpp>
#include <yggr/iterator_ex/detail/distance.hpp>

namespace yggr
{
namespace iterator_ex
{

using detail::advance;
using detail::advance_copy;

using detail::distance;


template <typename T> YGGR_CONSTEXPR_OR_INLINE
T next(T x) 
{
	return ++x;
}

template <typename T, typename Dis> YGGR_CONSTEXPR_OR_INLINE
T next(T x, const Dis& n)
{
	return advance_copy(x, n);
}

template <typename T> YGGR_CONSTEXPR_OR_INLINE
T prev(T x) 
{
	return --x;
}

template <typename T, typename Dis> YGGR_CONSTEXPR_OR_INLINE
T prev(T x, const Dis& n)
{
	return advance_copy(x, -n);
}

} // namespace iterator_ex

using iterator_ex::advance;
using iterator_ex::advance_copy;

using iterator_ex::distance;

using iterator_ex::next;
using iterator_ex::prev;

} // namespace yggr

#endif // __YGGR_ITERATOR_EX_ITERATOR_HPP__

