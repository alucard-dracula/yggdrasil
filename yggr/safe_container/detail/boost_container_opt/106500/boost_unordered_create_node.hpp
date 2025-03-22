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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106500_BOOST_UNORDERED_CREATE_NODE_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106500_BOOST_UNORDERED_CREATE_NODE_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UNORDERED_CREATE_NODE_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_UNORDERED_CREATE_NODE_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/unordered/detail/implementation.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define YGGR_BOOST_UNORDERED_EMPLACE_TEMPLATE typename... Args
#define YGGR_BOOST_UNORDERED_EMPLACE_ARGS BOOST_FWD_REF(Args)... args
#define YGGR_BOOST_UNORDERED_EMPLACE_FORWARD boost::forward<Args>(args)...

#else

#define YGGR_BOOST_UNORDERED_EMPLACE_TEMPLATE typename Args
#define YGGR_BOOST_UNORDERED_EMPLACE_ARGS Args const& args
#define YGGR_BOOST_UNORDERED_EMPLACE_FORWARD args

#endif // BOOST_NO_CXX11_VARIADIC_TEMPLATES

#if BOOST_UNORDERED_CXX11_CONSTRUCTION

#	define YGGR_BOOST_UNORDERED_CALL_CONSTRUCT1(Traits, alloc, address, a0) Traits::construct(alloc, address, a0)

#elif !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename... Args> inline
void construct_value(T* address, BOOST_FWD_REF(Args)... args)
{
    new (static_cast<void*>(address)) T(boost::forward<Args>(args)...);
}

#	define YGGR_BOOST_UNORDERED_CALL_CONSTRUCT1(Traits, alloc, address, a0) \
		::yggr::safe_container::detail::construct_value(address, a0)

#else

template<typename T> inline 
void construct_value(T* address)
{
    new (static_cast<void*>(address)) T();
}

template <typename T, typename A0> inline
void construct_value(T* address, BOOST_FWD_REF(A0) a0)
{
    new (static_cast<void*>(address)) T(boost::forward<A0>(a0));
}

#	define YGGR_BOOST_UNORDERED_CALL_CONSTRUCT1(Traits, alloc, address, a0) \
		::yggr::safe_container::detail::construct_value(address, a0)

#endif // BOOST_UNORDERED_CXX11_CONSTRUCTION

template<typename NodePtr, typename NodeAlloc, typename NodeConstructor, typename VAlloc,
			YGGR_BOOST_UNORDERED_EMPLACE_TEMPLATE> inline
NodePtr __create_u_node_1(const VAlloc& valloc, YGGR_BOOST_UNORDERED_EMPLACE_ARGS)
{
	typedef NodePtr node_ptr_type;
	typedef NodeAlloc node_alloc_type;
	typedef NodeConstructor node_constructor_type;

	node_alloc_type alloc(valloc);
	node_constructor_type a(alloc);
	try
	{
		a.create_node();
		boost::unordered::detail::func::construct_from_args(
			alloc, a.node_->value_ptr(), YGGR_BOOST_UNORDERED_EMPLACE_FORWARD);
	}
	catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
	{
		__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
		return node_ptr_type();
	}

	return a.release();
}

template<typename NodePtr, typename NodeAlloc, typename NodeConstructor, typename VAlloc,
			typename A0> inline
NodePtr __create_u_node_2(const VAlloc& valloc, BOOST_FWD_REF(A0) a0)
{
	typedef NodePtr node_ptr_type;
	typedef NodeAlloc node_alloc_type;
	typedef NodeConstructor node_constructor_type;
	typedef boost::unordered::detail::allocator_traits<node_alloc_type> allocator_traits_type;

	node_alloc_type alloc(valloc);
	node_constructor_type a(alloc);

	try
	{
		a.create_node();
		
		YGGR_BOOST_UNORDERED_CALL_CONSTRUCT1(
				allocator_traits_type, 
				alloc, 
				a.node_->value_ptr(), 
				boost::forward<A0>(a0));
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

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106500_BOOST_UNORDERED_CREATE_NODE_HPP__
