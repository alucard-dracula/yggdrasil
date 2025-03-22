//operator_set_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_OPERATOR_SET_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_OPERATOR_SET_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>

#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#   include <initializer_list>
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#	define YGGR_PP_SAFE_SEQ_OPERATOR_SET_IL_IMPL() \
		this_type& operator=(std::initializer_list<value_type> il) { \
			base_type tmp(il.begin(), il.end(), this_type::get_allocator()); \
			this_type::swap(tmp); \
			return *this; }

#	define YGGR_PP_SAFE_KV_OPERATOR_SET_IL_IMPL() \
		this_type& operator=(std::initializer_list<value_type> il) { \
			base_type tmp(il.begin(), il.end(), \
							this_type::key_compare(), this_type::get_allocator()); \
			this_type::swap(tmp); \
			return *this; }

#	define YGGR_PP_SAFE_UKV_OPERATOR_SET_IL_IMPL() \
		this_type& operator=(std::initializer_list<value_type> il) { \
			base_type tmp(il.begin(), il.end(), \
							0, \
							this_type::hash_function(), \
							this_type::key_eq(), \
							this_type::get_allocator()); \
			this_type::swap(tmp); \
			return *this; }

#else

#	define YGGR_PP_SAFE_SEQ_OPERATOR_SET_IL_IMPL()

#	define YGGR_PP_SAFE_KV_OPERATOR_SET_IL_IMPL()

#	define YGGR_PP_SAFE_UKV_OPERATOR_SET_IL_IMPL()

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#define YGGR_PP_SAFE_CONTAINER_OPERATOR_SET_IMPL() \
	inline this_type& operator=(BOOST_RV_REF(base_type) right) { \
		this_type::swap(boost::move(right)); \
		return *this; } \
	\
	this_type& operator=(const base_type& right) { \
		if(&_base == &right) { return *this; } \
		base_type tmp(right); \
		this_type::swap(tmp); \
		return *this; } \
	\
	inline this_type& operator=(BOOST_RV_REF(this_type) right) { \
		if(this == rvref_to_pointer(boost::move(right))) { \
			return *this; } \
		this_type& right_ref = right; \
		{ write_lock_type lk(_mutex); \
			right_ref.swap(_base); } \
		return *this; } \
	\
	this_type& operator=(const this_type& right) { \
		if(this == &right) { return *this; } \
		{ write_lock_type lk(_mutex); \
			right.copy_to_base(_base); } \
		return *this; }

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_OPERATOR_SET_IMPL( \
			__type_count__, __base_tpl__, __safe_tpl__, __is_isomeric_same__ ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename IMCT ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), > ) inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >, \
				base_type >, \
			this_type& >::type \
		operator=(BOOST_RV_REF_BEG \
						__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) > \
					BOOST_RV_REF_END right) { \
		typedef __base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) > right_type; \
		right_type& tmp = right; \
		this_type::swap(reinterpret_cast<base_type&>(tmp)); \
		return *this; } \
	\
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename IMCT ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), > ) inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >, \
				base_type >, \
			this_type& >::type \
		operator=(const __base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >& right) { \
		base_type tmp(reinterpret_cast<const base_type&>(right)); \
		this_type::swap(tmp); \
		return *this; } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >::base_type, \
				base_type >, \
			this_type& >::type \
		operator=(BOOST_RV_REF_BEG \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ > \
					BOOST_RV_REF_END right) { \
		typedef \
			__safe_tpl__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		right_type& right_ref = right; \
		{ write_lock_type lk(_mutex); \
			right_ref.swap(_base); } \
		return *this; } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >::base_type, \
				base_type >, \
			this_type& >::type \
		operator=(const \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >& right) { \
		{ write_lock_type lk(_mutex); \
			right.copy_to_base(_base); } \
		return *this; } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				typename \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >::base_type, \
				base_type >, \
			this_type& >::type \
		operator=(BOOST_RV_REF_BEG \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ > \
					BOOST_RV_REF_END right) { \
		typedef \
			__safe_tpl__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_type& right_ref = right; \
		{ write_lock_type lk(_mutex); \
			right_ref.swap(reinterpret_cast<right_base_type&>(_base)); } \
		return *this; } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				typename \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >::base_type, \
				base_type >, \
			this_type& >::type \
		operator=(const \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >& right) { \
		typedef \
			__safe_tpl__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		typedef typename right_type::base_type right_base_type; \
		{ write_lock_type lk(_mutex); \
			right.copy_to_base(reinterpret_cast<right_base_type&>(_base)); } \
		return *this; }


#define YGGR_PP_SAFE_SEQ_OPERATOR_SET_IMPL() \
	YGGR_PP_SAFE_CONTAINER_OPERATOR_SET_IMPL() \
	\
	YGGR_PP_SAFE_SEQ_OPERATOR_SET_IL_IMPL()

#define YGGR_PP_SAFE_KV_OPERATOR_SET_IMPL() \
	YGGR_PP_SAFE_CONTAINER_OPERATOR_SET_IMPL() \
	\
	YGGR_PP_SAFE_KV_OPERATOR_SET_IL_IMPL()

#define YGGR_PP_SAFE_UKV_OPERATOR_SET_IMPL() \
	YGGR_PP_SAFE_CONTAINER_OPERATOR_SET_IMPL() \
	\
	YGGR_PP_SAFE_UKV_OPERATOR_SET_IL_IMPL()

#define YGGR_PP_SAFE_UKV_ISOMERIC_SAME_OPERATOR_SET_IMPL(__type_count__, __base_tpl__, __safe_tpl__ ) \
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >::base_type, \
				base_type >, \
			this_type& >::type \
		operator=(BOOST_RV_REF_BEG \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ > \
					BOOST_RV_REF_END right) { \
		typedef \
			__safe_tpl__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		right_type& right_ref = right; \
		{ write_lock_type lk(_mutex); \
			right_ref.swap(_base); } \
		return *this; } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >::base_type, \
				base_type >, \
			this_type& >::type \
		operator=(const \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >& right) { \
		{ write_lock_type lk(_mutex); \
			right.copy_to_base(_base); } \
		return *this; }

#define YGGR_PP_SAFE_MIDX_ISOMERIC_SAME_OPERATOR_SET_IMPL YGGR_PP_SAFE_UKV_ISOMERIC_SAME_OPERATOR_SET_IMPL

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_OPERATOR_SET_IMPL_HPP__
