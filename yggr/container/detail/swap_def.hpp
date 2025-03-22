//swap_def.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_SWAP_DEF_HPP__
#define __YGGR_CONTAINER_DETAIL_SWAP_DEF_HPP__

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>

#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/comparison/less.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/type_traits/is_same.hpp>

#define YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_SWAP( __type_count__, __type__, __is_isomeric_same__, __inline__) \
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TL ) \
		YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TR ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) __inline__ \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) >, \
				__type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) > >, \
			void >::type \
		swap( __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) >& l, \
				__type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) >& r) { \
		typedef \
			__type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) > cont_type; \
		l.swap(reinterpret_cast<cont_type&>(r)); }

//base_type& this_type&
//this_type& base_type&
//this_type& this_type&

#define YGGR_PP_CONTAINER_SWAP( __type_count__, __type__, __inline__ ) \
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) __inline__ \
	void swap( typename YGGR_PP_TEMPLATE_TYPE( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ) )::base_type& l, \
				YGGR_PP_TEMPLATE_TYPE( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ) )& r ) { \
		r.swap( l ); } \
		\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) __inline__ \
	void swap( YGGR_PP_TEMPLATE_TYPE( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ) )& l, \
				typename YGGR_PP_TEMPLATE_TYPE( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ) )::base_type& r ) { \
		l.swap( r ); } \
		\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) __inline__ \
	void swap( YGGR_PP_TEMPLATE_TYPE( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ) )& l, \
				YGGR_PP_TEMPLATE_TYPE( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ) )& r ) { \
		l.swap( r ); }

// support options_contianer
#define YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_M_LESS_N( __base_type_count__, __type_count__, __base_type__, __type__, __is_isomeric_same__, __inline__) \
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TL ) \
		YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TR ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) __inline__ \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) >::base_type, \
				typename __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) >::base_type >, \
			void >::type \
		swap(  __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) >& l, \
				__type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) >& r) { \
		l.swap(r); } \

#define YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_M_NLESS_N( __base_type_count__, __type_count__, __base_type__, __type__, __is_isomeric_same__, __inline__)

#define YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN( __base_type_count__, __type_count__, __base_type__, __type__, __is_isomeric_same__, __inline__) \
	BOOST_PP_EXPR_IF( BOOST_PP_ADD( __base_type_count__, __type_count__ ), template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename TL ) \
		YGGR_PP_SYMBOL_IF( __base_type_count__, YGGR_PP_SYMBOL_COMMA) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TR ) \
	BOOST_PP_EXPR_IF( BOOST_PP_ADD( __base_type_count__, __type_count__ ), > ) __inline__ \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__base_type__< YGGR_PP_TEMPLATE_PARAMS_TYPES(__base_type_count__, TL ) >, \
				typename __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES(__type_count__, TR ) >::base_type >, \
			void >::type \
		swap( __base_type__< YGGR_PP_TEMPLATE_PARAMS_TYPES(__base_type_count__, TL ) >& l, \
				__type__< YGGR_PP_TEMPLATE_PARAMS_TYPES(__type_count__, TR ) >& r) { \
		typedef __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES(__type_count__, TR ) > cont_type; \
		typedef typename cont_type::base_type base_cont_type; \
		r.swap(reinterpret_cast<base_cont_type&>(l)); } \
	\
	BOOST_PP_EXPR_IF( BOOST_PP_ADD( __base_type_count__, __type_count__ ), template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TL ) \
		YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename TR ) \
	BOOST_PP_EXPR_IF( BOOST_PP_ADD( __base_type_count__, __type_count__ ), > ) __inline__ \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				typename __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) >::base_type, \
				__base_type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, TR ) > >, \
			void >::type \
		swap( __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) >& l, \
				__base_type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, TR ) >& r ) { \
		typedef __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) > cont_type; \
		typedef typename cont_type::base_type base_cont_type; \
		l.swap(reinterpret_cast<base_cont_type&>(r)); } \
	\
	BOOST_PP_IF( \
		BOOST_PP_LESS(__base_type_count__, __type_count__), \
		YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_M_LESS_N, \
		YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_M_NLESS_N)( \
			__base_type_count__, __type_count__, __base_type__, __type__, __is_isomeric_same__, __inline__ ) \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TL ) \
		YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename TR ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) __inline__ \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				typename __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) >::base_type, \
				typename __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) >::base_type >, \
			void >::type \
		swap(  __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) >& l, \
				__type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TR ) >& r) { \
		typedef __type__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, TL ) > cont_type; \
		l.swap(reinterpret_cast<cont_type&>(r)); }


#define YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP( __type_count__, __base_type__, __type__, __is_isomeric_same__, __inline__) \
	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(__type_count__, __type_count__, __base_type__, __type__, __is_isomeric_same__, __inline__)

namespace yggr
{
namespace container
{
namespace detail
{
} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_SWAP_DEF_HPP__
