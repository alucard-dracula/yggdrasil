// node_tmp.hpp 

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_TMP_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_TMP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/support/operator_t.hpp>

#if (BOOST_VERSION < 106400)
#	include <boost/unordered/detail/allocate.hpp>
#else
#	include <boost/unordered/detail/implementation.hpp>
#endif // (BOOST_VERSION < 106400)

namespace yggr
{
namespace container
{
namespace detail
{

template <typename NodeAlloc>
struct node_tmp
{
	typedef 
		boost::unordered::detail::allocator_traits
		<
			NodeAlloc
		> node_allocator_traits;

	typedef typename node_allocator_traits::pointer node_pointer;

	NodeAlloc& alloc_;
	node_pointer node_;

	explicit node_tmp(node_pointer n, NodeAlloc& a)
		: alloc_(a),  node_(n)
	{
	}

	~node_tmp(void)
	{
		if(node_) 
		{
			node_allocator_traits::destroy(alloc_, node_->value_ptr());
#if (BOOST_VERSION < 105500)
			boost::unordered::detail::destroy(boost::addressof(*node_));
#else
			boost::unordered::detail::func::destroy(boost::addressof(*node_));
#endif // BOOST_VERSION < 105500
			
			node_allocator_traits::deallocate(alloc_, node_, 1);
		}
	}

public:
	// no throw
	inline node_pointer release(void) YGGR_NOEXCEPT
	{
		node_pointer p = node_;
		node_ = node_pointer();
		return p;
	}
};

} // namespace detail
} // namespace container
} // namespace boost

#endif // __YGGR_CONTAINER_DETAIL_NODE_TMP_HPP__
