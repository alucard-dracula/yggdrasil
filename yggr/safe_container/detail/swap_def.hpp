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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_SWAP_DEF_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_SWAP_DEF_HPP__

#include <yggr/move/move.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>

#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

// base_type& this_type&
// this_type& base_type&
// this_type& this_type&

#define YGGR_PP_SAFE_CONTAINER_SWAP( __type_count__, __type__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	void swap( typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type& l, \
				YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& r ) { \
		r.swap( l ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	void swap( YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& l, \
				typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type& r ) { \
		l.swap( r ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	void swap( YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& l, \
				YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& r ) { \
		l.swap( r ); }

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_SWAP(__type_count__, __type__, __is_isomeric_same__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename LT ) \
				YGGR_PP_SYMBOL_IF( BOOST_PP_SUB( __type_count__, 1 ), YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename RT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				Base< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), LT ) >, \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, RT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type >, \
			void >::type \
		swap( Base< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), LT ) >& l, \
				__type__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, RT ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					Base >& r) { \
		typedef \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, RT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base > safe_cont_type; \
		typedef typename safe_cont_type::base_type base_cont_type; \
		r.swap(reinterpret_cast<base_cont_type&>(l)); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename LT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename RT ) \
				YGGR_PP_SYMBOL_IF( BOOST_PP_SUB( __type_count__, 1 ), YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, LT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type, \
				Base< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), RT ) > >, \
			void >::type \
		swap( \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, LT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base >& l, \
			Base< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), RT ) >& r ) { \
		typedef \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, LT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base > safe_cont_type; \
		typedef typename safe_cont_type::base_type base_cont_type; \
		l.swap(reinterpret_cast<base_cont_type&>(r)); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename LT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename RT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, LT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type, \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, RT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type >, \
			void >::type \
		swap( \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, LT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base >& l, \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, RT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base >& r) { \
			l.swap(r); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename LT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename RT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, LT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type, \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, RT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type >, \
			void >::type \
		swap( \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, LT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base >& l, \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, RT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base >& r) { \
			l.swap(r); }

#define YGGR_PP_SAFE_UKV_ISOMERIC_SAME_SWAP(__type_count__, __type__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename LT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename RT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, LT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type, \
				typename \
					__type__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, RT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						Base >::base_type >, \
			void >::type \
		swap( \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, LT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base >& l, \
			__type__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, RT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				Base >& r) { \
			l.swap(r); }


namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_SWAP_DEF_HPP__
