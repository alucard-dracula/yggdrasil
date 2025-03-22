//create_node_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_CREATE_NODE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_CREATE_NODE_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggr_msg_macro.h>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>

#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

#	define YGGR_PP_SAFE_CONTAINER_OPT_CREATE_NODE( _ ) \
		template<typename ...Args> inline \
		node_ptr_type create_node(BOOST_FWD_REF(Args)... args) const { \
			try { return opt_type::create_node(this_type::get_allocator(), boost::forward<Args>(args)...); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } \
			return node_ptr_type(); } \
		\
		template<typename ...Args> inline \
		node_ptr_type create_node_alloc(const allocator_type& alloc, BOOST_FWD_REF(Args)... args) const { \
			try { return opt_type::create_node(alloc, boost::forward<Args>(args)...); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } \
			return node_ptr_type(); }

#else

#	define YGGR_PP_SAFE_CONTAINER_OPT_CREATE_NODE( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		node_ptr_type create_node( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const{ \
			try{ \
				return \
					opt_type::create_node( \
						this_type::get_allocator() \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } \
			return node_ptr_type(); } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		node_ptr_type create_node_alloc(const allocator_type& alloc \
										YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const{ \
			try{ \
				return \
					opt_type::create_node( \
						alloc \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } \
			return node_ptr_type(); }

#endif // BOOST_CONTAINER_PERFECT_FORWARDING

#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

#	define YGGR_PP_SAFE_CONTAINER_HA_OPT_CREATE_NODE( _ ) \
		template<typename ...Args> inline \
		node_ptr_type create_node(std::size_t& key_hash, BOOST_FWD_REF(Args)... args) const { \
			return \
				opt_type::create_node_1( \
					key_hash, this_type::get_allocator(), \
					this_type::hash_function(), boost::forward<Args>(args)...); } \
		\
		template<typename ...Args> inline \
		node_ptr_type create_node_alloc(std::size_t& key_hash, const allocator_type& alloc, \
										BOOST_FWD_REF(Args)... args) const { \
			return \
				opt_type::create_node_1( \
					key_hash, alloc, \
					this_type::hash_function(), boost::forward<Args>(args)...); }

#else

#	define YGGR_PP_SAFE_CONTAINER_HA_OPT_CREATE_NODE( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		node_ptr_type create_node( std::size_t& key_hash \
									YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const{ \
			return \
				opt_type::create_node_1( \
					key_hash, \
					this_type::get_allocator(), \
					this_type::hash_function() \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		node_ptr_type create_node_alloc(std::size_t& key_hash, const allocator_type& alloc \
										YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const{ \
			return \
				opt_type::create_node_1( \
					key_hash, \
					alloc, \
					this_type::hash_function() \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#endif // BOOST_CONTAINER_PERFECT_FORWARDING

namespace yggr
{
namespace safe_container
{
namespace detail
{

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETAIL_CREATE_NODE_IMPL_HPP__
