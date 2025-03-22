//swap_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_SWAP_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_SWAP_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/swap.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>

#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#define YGGR_PP_SAFE_CONTAINER_SWAP_IMPL() \
	inline void swap(BOOST_RV_REF(base_type) right) { \
		base_type& right_ref = right; \
		this_type::swap(right_ref); } \
	\
	void swap(base_type& right) { \
		if(boost::addressof(_base) == boost::addressof(right)) { return; } \
		write_lock_type lk(_mutex); \
		_base.swap(right); } \
	\
	inline void swap(BOOST_RV_REF(this_type) right) { \
		this_type& right_ref = right; \
		this_type::swap(right_ref); } \
	\
	void swap(this_type& right) { \
		if(this == boost::addressof(right)) { return; } \
		write_lock_type lk(_mutex); \
		right.swap(_base); }

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_SWAP_IMPL( \
		__type_count__, __base_tpl__, __safe_tpl__, __is_isomeric_same__ ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename IMCT ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), > ) inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >, \
				base_type >, \
			void >::type \
		swap(BOOST_RV_REF_BEG \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) > \
				BOOST_RV_REF_END right) { \
		typedef \
			__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) > right_type; \
		right_type& right_ref = right; \
		this_type::swap(reinterpret_cast<base_type&>(right_ref)); } \
	\
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename IMCT ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), > ) inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >, \
				base_type >, \
			void >::type \
		swap( __base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >& right) { \
		this_type::swap(reinterpret_cast<base_type&>(right));} \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >::base_type, \
				base_type >, \
			void >::type \
		swap(BOOST_RV_REF_BEG \
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
		this_type::swap(right_ref); } \
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
			void >::type \
		swap( __safe_tpl__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ >& right) { \
		write_lock_type lk(_mutex); \
		right.swap(_base);  } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				typename \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >::base_type, \
				base_type >, \
			void >::type \
		swap(BOOST_RV_REF_BEG \
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
		this_type::swap(right_ref); } \
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
			void >::type \
		swap( __safe_tpl__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ >& right) { \
		typedef \
			__safe_tpl__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		typedef typename right_type::base_type right_base_type; \
		write_lock_type lk(_mutex); \
		right.swap(reinterpret_cast<right_base_type&>(_base)); }

#define YGGR_PP_SAFE_UKV_ISOMERIC_SAME_SWAP_IMPL( __type_count__, __base_tpl__, __safe_tpl__) \
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename \
					__safe_tpl__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ >::base_type, \
				base_type >, \
			void >::type \
		swap(BOOST_RV_REF_BEG \
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
		this_type::swap(right_ref); } \
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
			void >::type \
		swap( __safe_tpl__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ >& right) { \
		write_lock_type lk(_mutex); \
		right.swap(_base);  }

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_SWAP_IMPL_HPP__
