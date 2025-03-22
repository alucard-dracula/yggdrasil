// node_handle_foo_hacking_node_alloc_holder.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_NODE_ALLOC_HOLDER_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_NODE_ALLOC_HOLDER_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_NODE_ALLOC_HOLDER_HPP__
#	error "this file is inner file, can't include it!"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_NODE_ALLOC_HOLDER_HPP__

#include <yggr/mplex/static_assert.hpp>

#include <boost/container/detail/node_alloc_holder.hpp>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename NodeAllocHolder>
struct node_handle_foo_hacking_node_alloc_holder;

template<template<typename _A, typename _I, typename _P> class NodeAllocHolder,
			typename A, typename I, typename P>
struct node_handle_foo_hacking_node_alloc_holder< NodeAllocHolder<A, I, P> >
{
private:
	typedef node_handle_foo_hacking_node_alloc_holder this_type;

public:
	typedef NodeAllocHolder<A, I, P> node_alloc_holder_type;
	typedef node_handle_foo_hacking_node_alloc_holder hacking_node_alloc_holder_type;

	typedef I ICont;
	typedef typename node_alloc_holder_type::NodeAlloc NodeAlloc;
	typedef typename node_alloc_holder_type::NodePtr NodePtr;

	struct members_holder
		: public NodeAlloc
	{
	public:
		ICont m_icont;
	};

public:
	inline ICont& icont(void)
	{  
		return members_.m_icont;
	}

	inline const ICont& icont(void) const
	{  
		return members_.m_icont;   
	}

	inline NodeAlloc& node_alloc(void)
	{
		return static_cast<NodeAlloc&>(members_);
	}

	inline const NodeAlloc &node_alloc(void) const
	{  
		return static_cast<const NodeAlloc&>(members_);  
	}

public:
	members_holder members_;
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_NODE_ALLOC_HOLDER_HPP__
