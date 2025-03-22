//comparer_def.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_COMPARER_DEF_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_COMPARER_DEF_HPP__

#include <yggr/bind/bind.hpp>

#include <yggr/safe_container/detail/compare_self_conflict_fix_helper.hpp>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/any_val/operator_wrap.hpp>

#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

// const base_type& const this_type&
// const this_type& const base_type&
// const this_type& const this_type&

#define YGGR_PP_SAFE_CONTAINER_COMPARER_FUNCTION( __op__, __foo_name__, __type_count__, __type__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	bool __op__( const typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type& l, \
					const YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& r ) { \
		typedef YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base ) safe_type; \
		typedef typename safe_type::base_type base_type; \
		return r.using_handler( boost::bind(&__foo_name__<base_type, base_type>, boost::cref(l), _1) ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	bool __op__( const YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& l, \
					const typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type& r ) { \
		typedef YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base ) safe_type; \
		typedef typename safe_type::base_type base_type; \
		return l.using_handler( boost::bind(&__foo_name__<base_type, base_type>, _1, boost::cref(r) ) ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	bool __op__( const YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& l, \
					const YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& r ) { \
		typedef YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base ) safe_type; \
		typedef typename safe_type::base_type base_type; \
		typedef bool (*cmp_type)(const base_type&, const base_type&); \
		return (boost::addressof(l) == boost::addressof(r)) ? \
				l.using_handler( boost::bind(&detail::comparer_self_handler<const base_type, cmp_type>, \
									_1, &__foo_name__<base_type, base_type>) ) \
				: l.using_handler( boost::bind(&detail::comparer_handler<const base_type, const safe_type, cmp_type>, \
									_1, boost::cref(r), &__foo_name__<base_type, base_type>) ); }

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FUNCTION( \
			__op__, __op_symbol__, __foo_name__, __type_count__, __type__, __is_isomeric_same__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename TL ) \
				YGGR_PP_SYMBOL_IF( BOOST_PP_SUB( __type_count__, 1 ), YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TR ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				Base< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), TL ) >, \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type >, \
			bool >::type \
		__op__( \
			const Base< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), TL ) >& l, \
			const \
				__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base >& r) { \
		typedef \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base > safe_cont_type; \
		typedef typename safe_cont_type::base_type base_cont_type; \
		return reinterpret_cast<const base_cont_type&>(l) __op_symbol__ r; } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TL ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename TR ) \
				YGGR_PP_SYMBOL_IF( BOOST_PP_SUB( __type_count__, 1 ), YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type, \
				Base< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), TR ) > >, \
			bool >::type \
		__op__( \
			const \
				__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base >& l, \
			const \
				Base< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), TR ) >& r ) { \
		typedef \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base > safe_cont_type; \
		typedef typename safe_cont_type::base_type base_cont_type; \
		return l __op_symbol__ reinterpret_cast<const base_cont_type&>(r); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TL ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TR ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > \
		class Base> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type, \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type >, \
			bool >::type \
		__op__( \
			const \
				__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base >& l, \
			const \
				__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base >& r) { \
		typedef \
			__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base > r_safe_type; \
		typedef typename r_safe_type::base_type base_type; \
		typedef bool (*cmp_type)(const base_type&, const base_type&); \
			return \
				l.using_handler( \
					boost::bind( \
						&detail::comparer_handler<const base_type, const r_safe_type, cmp_type>, \
						_1, boost::cref(r), &__foo_name__<base_type, base_type>) ); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TL ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TR ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > \
		class Base> inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type, \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type >, \
			bool >::type \
		__op__( \
			const \
				__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base >& l, \
			const \
				__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base >& r) { \
		typedef \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base > l_safe_type; \
		typedef typename l_safe_type::base_type l_base_type; \
		typedef \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base > r_safe_type; \
		typedef typename r_safe_type::base_type r_base_type; \
		typedef bool (*cmp_type)(const l_base_type&, const r_base_type&); \
			return \
				l.using_handler( \
					boost::bind( \
						&detail::comparer_handler<const l_base_type, const r_safe_type, cmp_type>, \
						_1, boost::cref(r), &__foo_name__<l_base_type, r_base_type>) ); }


#define YGGR_PP_SAFE_CONTAINER_EQUALTO_AND_NOTEQUALTO( __type_count__, __type__ ) \
	YGGR_PP_SAFE_CONTAINER_COMPARER_FUNCTION(operator==, yggr::any_val::operator_wrap::equal_to, __type_count__, __type__) \
	YGGR_PP_SAFE_CONTAINER_COMPARER_FUNCTION(operator!=, yggr::any_val::operator_wrap::not_equal_to, __type_count__, __type__)

#define YGGR_PP_SAFE_CONTAINER_COMPARER_FULL( __type_count__, __type__ ) \
	YGGR_PP_SAFE_CONTAINER_EQUALTO_AND_NOTEQUALTO(__type_count__, __type__) \
	YGGR_PP_SAFE_CONTAINER_COMPARER_FUNCTION(operator<, yggr::any_val::operator_wrap::less, __type_count__, __type__) \
	YGGR_PP_SAFE_CONTAINER_COMPARER_FUNCTION(operator<=, yggr::any_val::operator_wrap::less_equal, __type_count__, __type__) \
	YGGR_PP_SAFE_CONTAINER_COMPARER_FUNCTION(operator>, yggr::any_val::operator_wrap::greater, __type_count__, __type__) \
	YGGR_PP_SAFE_CONTAINER_COMPARER_FUNCTION(operator>=, yggr::any_val::operator_wrap::greater_equal, __type_count__, __type__)

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_EQUALTO_AND_NOTEQUALTO( __type_count__, __type__, __is_isomeric_same__ ) \
	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator==, ==, yggr::any_val::operator_wrap::equal_to, __type_count__, __type__, __is_isomeric_same__ ) \
	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator!=, !=, yggr::any_val::operator_wrap::not_equal_to, __type_count__, __type__, __is_isomeric_same__ )

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FULL( __type_count__, __type__, __is_isomeric_same__ ) \
	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_EQUALTO_AND_NOTEQUALTO( __type_count__, __type__, __is_isomeric_same__ ) \
	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator<, <, yggr::any_val::operator_wrap::less, __type_count__, __type__, __is_isomeric_same__ ) \
	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator<=, <=, yggr::any_val::operator_wrap::less_equal, __type_count__, __type__, __is_isomeric_same__ ) \
	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator>, >, yggr::any_val::operator_wrap::greater, __type_count__, __type__, __is_isomeric_same__ ) \
	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator>=, >=, yggr::any_val::operator_wrap::greater_equal, __type_count__, __type__, __is_isomeric_same__ )

#define YGGR_PP_SAFE_UKV_ISOMERIC_SAME_COMPARER_FUNCTION( \
			__op__, __op_symbol__, __foo_name__, __type_count__, __type__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TL ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TR ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > \
		class Base> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type, \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type >, \
			bool >::type \
		__op__( \
			const \
				__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base >& l, \
			const \
				__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base >& r) { \
		typedef \
			__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base > r_safe_type; \
		typedef typename r_safe_type::base_type base_type; \
		typedef bool (*cmp_type)(const base_type&, const base_type&); \
			return \
				l.using_handler( \
					boost::bind( \
						&detail::comparer_handler<const base_type, const r_safe_type, cmp_type>, \
						_1, boost::cref(r), &__foo_name__<base_type, base_type>) ); }

#define YGGR_PP_SAFE_UKV_ISOMERIC_SAME_EQUALTO_AND_NOTEQUALTO( __type_count__, __type__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator==, ==, yggr::any_val::operator_wrap::equal_to, __type_count__, __type__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator!=, !=, yggr::any_val::operator_wrap::not_equal_to, __type_count__, __type__ )

#define YGGR_PP_SAFE_UKV_ISOMERIC_SAME_COMPARER_FULL( __type_count__, __type__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_EQUALTO_AND_NOTEQUALTO( __type_count__, __type__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator<, <, yggr::any_val::operator_wrap::less, __type_count__, __type__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator<=, <=, yggr::any_val::operator_wrap::less_equal, __type_count__, __type__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator>, >, yggr::any_val::operator_wrap::greater, __type_count__, __type__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_COMPARER_FUNCTION( \
		operator>=, >=, yggr::any_val::operator_wrap::greater_equal, __type_count__, __type__ )

#define YGGR_PP_SAFE_MIDX_ISOMERIC_SAME_EQUALTO_AND_NOTEQUALTO YGGR_PP_SAFE_UKV_ISOMERIC_SAME_EQUALTO_AND_NOTEQUALTO

#define YGGR_PP_SAFE_MIDX_ISOMERIC_SAME_COMPARER_FULL YGGR_PP_SAFE_UKV_ISOMERIC_SAME_COMPARER_FULL


namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_COMPARER_DEF_HPP__
