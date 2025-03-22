//boost_unordered_create_node.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105300_BOOST_UNORDERED_CREATE_NODE_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105300_BOOST_UNORDERED_CREATE_NODE_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UNORDERED_CREATE_NODE_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UNORDERED_CREATE_NODE_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/unordered/detail/allocate.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename NodePtr, typename NodeAlloc, typename NodeConstructor, typename VAlloc,
			BOOST_UNORDERED_EMPLACE_TEMPLATE> inline
NodePtr __create_u_node_1(const VAlloc& valloc, BOOST_UNORDERED_EMPLACE_ARGS)
{
	typedef NodePtr node_ptr_type;
	typedef NodeAlloc node_alloc_type;
	typedef NodeConstructor node_constructor_type;

	node_alloc_type alloc(valloc);
	node_constructor_type a(alloc);
	try
	{
		a.construct_with_value(BOOST_UNORDERED_EMPLACE_FORWARD);
	}
	catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
	{
		__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
		return node_ptr_type();
	}

	return a.release();
}

template<typename NodePtr, typename NodeAlloc, typename NodeConstructor, typename VAlloc, typename A0> inline
NodePtr __create_u_node_2(const VAlloc& valloc, BOOST_FWD_REF(A0) a0)
{
	typedef NodePtr node_ptr_type;
	typedef NodeAlloc node_alloc_type;
	typedef NodeConstructor node_constructor_type;

	node_alloc_type alloc(valloc);
	node_constructor_type a(alloc);

	try
	{
		a.construct_with_value2(a0);
	}
	catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
	{
		__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
		return node_ptr_type();
	}

	return a.release();
}

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105300_BOOST_UNORDERED_CREATE_NODE_HPP__
