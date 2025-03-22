//boost_unordered_node_handle_uset_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106400_BOOST_UNORDERED_HANDLE_USET_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106400_BOOST_UNORDERED_HANDLE_USET_OPT_HPP__

#include <yggr/base/yggrdef.h>

#if !(BOOST_VERSION < 106400)

#include <boost/unordered/detail/implementation.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename N, typename T, typename A> 
class boost_node_handle_uset_opt
{
public:
	typedef typename boost::unordered::detail::rebind_wrap<A, T>::type value_allocator;
	typedef boost::unordered::detail::allocator_traits<value_allocator> value_allocator_traits;
	typedef N node;
	typedef typename boost::unordered::detail::rebind_wrap<A, node>::type node_allocator;
	typedef boost::unordered::detail::allocator_traits<node_allocator> node_allocator_traits;
	typedef typename node_allocator_traits::pointer node_pointer;

public:
	typedef T value_type;
	typedef A allocator_type;

public:
	node_pointer ptr_;
	bool has_alloc_;
	boost::unordered::detail::value_base<value_allocator> alloc_;
};

template<template<typename _N, typename _T, typename _A> class NodeHandleUSet,
			typename N, typename T, typename A> YGGR_CONSTEXPR_OR_INLINE
boost_node_handle_uset_opt<N, T, A>&
	boost_unordered_node_handler_opt_cast(NodeHandleUSet<N, T, A>& node_handle) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef boost_node_handle_uset_opt<N, T, A> dst_type;
	return reinterpret_cast<dst_type&>(node_handle);
}

template<template<typename _N, typename _T, typename _A> class NodeHandleUSet,
			typename N, typename T, typename A> YGGR_CONSTEXPR_OR_INLINE
const boost_node_handle_uset_opt<N, T, A>&
	boost_unordered_node_handler_opt_cast(const NodeHandleUSet<N, T, A>& node_handle) YGGR_NOEXCEPT_OR_NOTHROW
{
	typedef boost_node_handle_uset_opt<N, T, A> dst_type;
	return reinterpret_cast<dst_type&>(node_handle);
}

#endif // !(BOOST_VERSION < 106400)

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106400_BOOST_UNORDERED_HANDLE_USET_OPT_HPP__
