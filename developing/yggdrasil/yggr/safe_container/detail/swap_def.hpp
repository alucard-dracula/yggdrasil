//swap_def.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

// base_type&& this_type&
// base_type& this_type&
// this_type& base_type&&
// this_type& base_type&
// this_type& this_type&

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES

#define YGGR_PP_SAFE_CONTAINER_SWAP( __type_count__, __base_type_count__, __type__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename _T ) > class Base > inline \
	void swap( BOOST_RV_REF( typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type ) l, \
					YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& r ) { \
		r.swap( l ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename _T ) > class Base > inline \
	void swap( typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type& l, \
				YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& r ) { \
		r.swap( l ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename _T ) > class Base > inline  \
	void swap( YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& l, \
				BOOST_RV_REF( typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type ) r ) { \
		l.swap( r ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename _T ) > class Base > inline \
	void swap( YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& l, \
				typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type& r ) { \
		l.swap( r ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename _T ) > class Base > inline \
	void swap( YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& l, \
				YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& r ) { \
		l.swap( r ); }

#else

#define YGGR_PP_SAFE_CONTAINER_SWAP( __type_count__, __base_type_count__, __type__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename _T ) > class Base > inline \
	void swap( BOOST_RV_REF( typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type ) l, \
					YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& r ) { \
		typedef typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type now_base_type; \
		now_base_type& tmp = l; \
		r.swap( tmp ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename _T ) > class Base > inline \
	void swap( typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type& l, \
				YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& r ) { \
		r.swap( l ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename _T ) > class Base > inline  \
	void swap( YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& l, \
				BOOST_RV_REF( typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type ) r ) { \
		typedef typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type now_base_type; \
		now_base_type& tmp = r; \
		l.swap( tmp ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename _T ) > class Base > inline \
	void swap( YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& l, \
				typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::base_type& r ) { \
		l.swap( r ); } \
		\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( __base_type_count__, typename _T ) > class Base > inline \
	void swap( YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& l, \
				YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& r ) { \
		l.swap( r ); }

#endif // YGGR_NO_CXX11_RVALUE_REFERENCES


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
