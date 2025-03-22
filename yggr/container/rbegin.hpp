// rbegin.hpp

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

#ifndef __YGGR_CONTAINER_RBEGIN_HPP__
#define __YGGR_CONTAINER_RBEGIN_HPP__

#include <yggr/range_ex/range_reverse_iterator_ex.hpp>
#include <yggr/container/detail/has_rbegin.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/range/rbegin.hpp>

namespace yggr
{
namespace container
{

template<typename Cont> YGGR_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		detail::has_any_rbegin<Cont>,
		typename Cont::reverse_iterator
	>::type
	rbegin(Cont& cont)
{
	return cont.rbegin();
}

template<typename Cont> YGGR_CONSTEXPR_OR_INLINE
typename 
	boost::disable_if
	<
		detail::has_any_rbegin<Cont>,
		typename range_ex::range_reverse_iterator_ex<Cont>
	>::type
	rbegin(Cont& cont)
{
	return ::boost::rbegin(cont);
}

template<typename Cont> YGGR_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		detail::has_any_rbegin<Cont>,
		typename Cont::const_reverse_iterator
	>::type
	rbegin(const Cont& cont)
{
	return cont.rbegin();
}

template<typename Cont> YGGR_CONSTEXPR_OR_INLINE
typename 
	boost::disable_if
	<
		detail::has_any_rbegin<Cont>,
		typename range_ex::range_reverse_iterator_ex<const Cont>
	>::type
	rbegin(const Cont& cont)
{
	return ::boost::rbegin(cont);
}

template<typename Cont> YGGR_CONSTEXPR_OR_INLINE
typename 
	boost::enable_if
	<
		detail::has_any_rbegin<Cont>,
		typename Cont::const_reverse_iterator
	>::type
	crbegin(const Cont& cont)
{
	return ::yggr::container::rbegin(cont);
}

template<typename Cont> YGGR_CONSTEXPR_OR_INLINE
typename 
	boost::disable_if
	<
		detail::has_any_rbegin<Cont>,
		typename range_ex::range_reverse_iterator_ex<const Cont>
	>::type
	crbegin(const Cont& cont)
{
	return ::yggr::container::rbegin(cont);
}

} // namespace container

using container::rbegin;
using container::crbegin;

} // namespace yggr

#endif // __YGGR_CONTAINER_RBEGIN_HPP__

