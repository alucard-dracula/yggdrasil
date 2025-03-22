// boost_container_create_node.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106500_BOOST_CONTAINER_CREATE_NODE_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106500_BOOST_CONTAINER_CREATE_NODE_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_CREATE_NODE_HPP__
#	error "this file is inner file, can't include it"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_CREATE_NODE_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/ref.hpp>
#include <boost/container/detail/workaround.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>

#include <boost/container/allocator_traits.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename T>
struct __get_allocator_t;

template<typename T, unsigned Ver, 
			template<typename _T, unsigned _Ver> class Alloc_Ver_Traits>
struct __get_allocator_t< Alloc_Ver_Traits<T, Ver> >
{
	typedef T type;
};

#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

template<typename NodePtr, typename AllocVersionTraits, typename DAlloc, typename VAlloc, typename ...Args> inline
NodePtr __create_node(const VAlloc& valloc, BOOST_FWD_REF(Args)... args)
{
	typedef NodePtr node_ptr_type;
	typedef AllocVersionTraits alloc_version_traits_type;
	typedef DAlloc deallocator_type;
	typedef typename __get_allocator_t<alloc_version_traits_type>::type node_allocator_type;
	typedef typename node_allocator_type::value_type node_type;

	node_allocator_type alloc(valloc);
	node_ptr_type p = alloc_version_traits_type::allocate_one(alloc);
	deallocator_type node_deallocator(p, alloc);
	boost::container::allocator_traits<node_allocator_type>::construct(
		alloc,
		boost::container::container_detail::addressof(p->m_data),
		boost::forward<Args>(args)...);
	node_deallocator.release();
	//This does not throw
	typedef typename node_type::hook_type hook_type;
	::new(static_cast<hook_type*>(boost::movelib::to_raw_pointer(p))) hook_type;
	return (p);

}

#else //#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename NodePtr, typename AllocVersionTraits, typename DAlloc, typename VAlloc \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		NodePtr __create_node(const VAlloc& valloc \
								YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ){ \
			typedef NodePtr node_ptr_type; \
			typedef AllocVersionTraits alloc_version_traits_type; \
			typedef DAlloc deallocator_type; \
			typedef typename __get_allocator_t<alloc_version_traits_type>::type node_allocator_type; \
			typedef typename node_allocator_type::value_type node_type; \
			node_allocator_type alloc(valloc); \
			node_ptr_type p = alloc_version_traits_type::allocate_one(alloc); \
			deallocator_type node_deallocator(p, alloc); \
			boost::container::allocator_traits<node_allocator_type>::construct( \
				alloc, boost::container::container_detail::addressof(p->m_data) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			node_deallocator.release(); \
			typedef typename node_type::hook_type hook_type; \
			::new(static_cast<hook_type*>(boost::movelib::to_raw_pointer(p))) hook_type; \
			return (p); }

#	define YGGR_PP_FOO_ARG_NAME( ) arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif   //#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_106500_BOOST_CONTAINER_CREATE_NODE_HPP__
