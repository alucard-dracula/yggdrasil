//indexes_comparer_def.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_COMPARER_DEF_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_COMPARER_DEF_HPP__

#include <yggr/bind/bind.hpp>

#include <yggr/safe_container/detail/compare_self_conflict_fix_helper.hpp>
#include <yggr/any_val/operator_wrap.hpp>

#include <boost/ref.hpp>

// const base_type& const this_type&
// const this_type& const base_type&
// const this_type& const this_type&

#define YGGR_PP_SAFE_MIDX_INDEXES_COMPARER_FUNCTION( __op__, __foo_name__, __type__ ) \
	template< int N, typename Multi_Index_Container, typename Mutex > inline \
	bool __op__( const typename __type__< N, Multi_Index_Container, Mutex >::base_type& l, \
					const __type__< N, Multi_Index_Container, Mutex >& r ) { \
		typedef __type__< N, Multi_Index_Container, Mutex > safe_type; \
		typedef typename safe_type::base_type base_type; \
		return r.using_handler( boost::bind(&__foo_name__<base_type, base_type>, boost::cref(l), _1) ); } \
		\
	template< int N, typename Multi_Index_Container, typename Mutex > inline \
	bool __op__( const __type__< N, Multi_Index_Container, Mutex >& l, \
					const typename __type__< N, Multi_Index_Container, Mutex >::base_type& r ) { \
		typedef __type__< N, Multi_Index_Container, Mutex > safe_type; \
		typedef typename safe_type::base_type base_type; \
		return l.using_handler( boost::bind(&__foo_name__<base_type, base_type>, _1, boost::cref(r) ) ); } \
		\
	template< int N, typename Multi_Index_Container, typename Mutex > inline \
	bool __op__( const __type__< N, Multi_Index_Container, Mutex >& l, \
					const __type__< N, Multi_Index_Container, Mutex >& r ) { \
		typedef __type__< N, Multi_Index_Container, Mutex > safe_type; \
		typedef typename safe_type::base_type base_type; \
		typedef bool (*cmp_type)(const base_type&, const base_type&); \
		return (boost::addressof(l) == boost::addressof(r)) ? \
				l.using_handler( boost::bind(&detail::comparer_self_handler<const base_type, cmp_type>, \
									_1, &__foo_name__<base_type, base_type>) ) \
				: l.using_handler( boost::bind(&detail::comparer_handler<const base_type, const safe_type, cmp_type>, \
									_1, boost::cref(r), &__foo_name__<base_type, base_type>) ); }

#define YGGR_PP_SAFE_MIDX_INDEXES_EQUALTO_AND_NOTEQUALTO( __type__ ) \
	YGGR_PP_SAFE_MIDX_INDEXES_COMPARER_FUNCTION(operator==, yggr::any_val::operator_wrap::equal_to, __type__) \
	YGGR_PP_SAFE_MIDX_INDEXES_COMPARER_FUNCTION(operator!=, yggr::any_val::operator_wrap::not_equal_to, __type__)

#define YGGR_PP_SAFE_MIDX_INDEXES_COMPARER_FULL( __type__ ) \
	YGGR_PP_SAFE_MIDX_INDEXES_EQUALTO_AND_NOTEQUALTO( __type__ ) \
	YGGR_PP_SAFE_MIDX_INDEXES_COMPARER_FUNCTION(operator<, yggr::any_val::operator_wrap::less, __type__) \
	YGGR_PP_SAFE_MIDX_INDEXES_COMPARER_FUNCTION(operator<=, yggr::any_val::operator_wrap::less_equal, __type__) \
	YGGR_PP_SAFE_MIDX_INDEXES_COMPARER_FUNCTION(operator>, yggr::any_val::operator_wrap::greater, __type__) \
	YGGR_PP_SAFE_MIDX_INDEXES_COMPARER_FUNCTION(operator>=, yggr::any_val::operator_wrap::greater_equal, __type__)


#define YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_COMPARER_FUNCTION( __op__, __foo_name__, __type__ ) \
	template< int N, typename Multi_Index_Container, typename Mutex, typename Mutex2 > inline \
	bool __op__( const __type__< N, Multi_Index_Container, Mutex >& l, \
					const __type__< N, Multi_Index_Container, Mutex2 >& r ) { \
		typedef __type__< N, Multi_Index_Container, Mutex > safe_left_type; \
		typedef __type__< N, Multi_Index_Container, Mutex2 > safe_right_type; \
		typedef typename safe_left_type::base_type base_type; \
		typedef bool (*cmp_type)(const base_type&, const base_type&); \
		return \
			l.using_handler( \
				boost::bind(&detail::comparer_handler<const base_type, const safe_right_type, cmp_type>, \
				_1, boost::cref(r), &__foo_name__<base_type, base_type>) ); }

#define YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_EQUALTO_AND_NOTEQUALTO( __type__ ) \
	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_COMPARER_FUNCTION(operator==, yggr::any_val::operator_wrap::equal_to, __type__) \
	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_COMPARER_FUNCTION(operator!=, yggr::any_val::operator_wrap::not_equal_to, __type__)

#define YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_COMPARER_FULL( __type__ ) \
	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_EQUALTO_AND_NOTEQUALTO( __type__ ) \
	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_COMPARER_FUNCTION(operator<, yggr::any_val::operator_wrap::less, __type__) \
	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_COMPARER_FUNCTION(operator<=, yggr::any_val::operator_wrap::less_equal, __type__) \
	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_COMPARER_FUNCTION(operator>, yggr::any_val::operator_wrap::greater, __type__) \
	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_COMPARER_FUNCTION(operator>=, yggr::any_val::operator_wrap::greater_equal, __type__)

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_COMPARER_DEF_HPP__
