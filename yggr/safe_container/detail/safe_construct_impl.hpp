//safe_construct_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_CONSTRUCT_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_CONSTRUCT_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>

#include <yggr/mplex/tag_sfinae.hpp>

#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#   include <initializer_list>
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST


#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

// kv
#	define YGGR_PP_SAFE_KV_CONSTRUCTOR_IL_IMPL( __c_name__ ) \
		__c_name__(std::initializer_list<value_type> l) \
			: _base(l.begin(), l.end()) {} \
		\
		__c_name__(std::initializer_list<value_type> l, const allocator_type& alloc) \
			: _base(l.begin(), l.end(), key_compare(), alloc) {} \
		\
		__c_name__(std::initializer_list<value_type> l, \
					const key_compare& key_comp, \
					const allocator_type& alloc) \
			: _base(l.begin(), l.end(), key_comp, alloc) {}

// ukv
#	define YGGR_PP_SAFE_UKV_CONSTRUCTOR_IL_IMPL( __c_name__ ) \
		__c_name__(std::initializer_list<value_type> l) \
			: _base(l.begin(), l.end()) {} \
		\
		__c_name__(std::initializer_list<value_type> l, \
							const allocator_type& alloc) \
			: _base(l.begin(), l.end(), \
						boost::unordered::detail::default_bucket_count, \
						hasher(), key_equal(), alloc) {} \
		\
		__c_name__(std::initializer_list<value_type> l, \
							const key_equal& cmp, \
							const allocator_type& alloc) \
			: _base(l.begin(), l.end(), \
						boost::unordered::detail::default_bucket_count, \
						hasher(), cmp, alloc) {} \
		\
		__c_name__(std::initializer_list<value_type> l, \
							const hasher& hash_foo, \
							const key_equal& cmp, \
							const allocator_type& alloc) \
			: _base(l.begin(), l.end(), \
						boost::unordered::detail::default_bucket_count, \
							hash_foo, cmp, alloc) {} \
		\
		__c_name__(std::initializer_list<value_type> l, \
							size_type n, \
							const hasher& hash_foo, \
							const key_equal& cmp, \
							const allocator_type& alloc) \
			: _base(l.begin(), l.end(), n, hash_foo, cmp, alloc) {}

#else

#	define YGGR_PP_SAFE_KV_CONSTRUCTOR_IL_IMPL( __c_name__ )

#	define YGGR_PP_SAFE_UKV_CONSTRUCTOR_IL_IMPL( __c_name__ )

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST


#	define YGGR_PP_SAFE_KV_CONSTRUCTOR_RVREF_IMPL( __c_name__ ) \
		explicit __c_name__(BOOST_RV_REF(base_type) right) \
			: _base(right.key_comp(), right.get_allocator()) { \
			base_type& right_ref = right; \
			_base.swap(right_ref); } \
		\
		__c_name__(BOOST_RV_REF(base_type) right, const allocator_type& alloc) \
			: _base(right.key_comp(), alloc) { \
			base_type& right_ref = right; \
			_base.swap(right_ref); }

// ukv
#	define YGGR_PP_SAFE_UKV_CONSTRUCTOR_RVREF_IMPL( __c_name__ ) \
		explicit __c_name__(BOOST_RV_REF(base_type) right) \
			: _base(right.get_allocator()) { \
			base_type& right_ref = right; \
			_base.swap(right_ref); } \
		\
		__c_name__(BOOST_RV_REF(base_type) right, const allocator_type& alloc) \
			: _base(alloc) { \
			base_type& right_ref = right; \
			_base.swap(right_ref); }

// constructor
// kv

/*
boost::containr changes the writing of the allocator from inclusion to inheritance,
so using sfinae boost::is_same check it
container(other_no_empty_tpl_container) -> empty_container // called container(allocator)
*/

#define YGGR_PP_SAFE_KV_CONSTRUCTOR_IMPL( __c_name__ ) \
	__c_name__(void) {} \
	\
	explicit __c_name__(const key_compare& cmp) \
		: _base(cmp) {} \
	\
	template<typename Ax> \
	explicit __c_name__(const Ax& alloc, \
							typename \
								boost::enable_if< \
									boost::is_same<Ax, allocator_type>, \
									mplex::sfinae_type \
								>::type sfinae = 0) \
		: _base(key_compare(), alloc) {} \
	\
	template<typename Ax> \
	__c_name__(const key_compare& cmp, const Ax& alloc, \
				typename \
					boost::enable_if< \
						boost::is_same<Ax, allocator_type>, \
						mplex::sfinae_type \
					>::type sfinae = 0) \
		: _base(cmp, alloc) {} \
	\
	template<typename InputIterator> \
	__c_name__(InputIterator first, InputIterator last) \
		: _base(first, last) {} \
	\
	template<typename InputIterator> \
	__c_name__(InputIterator first, InputIterator last, const key_compare& cmp) \
		: _base(first, last, cmp) {} \
	\
	template<typename InputIterator> \
	__c_name__(InputIterator first, InputIterator last, const allocator_type& alloc) \
		: _base(first, last, key_compare(), alloc) {} \
	\
	template<typename InputIterator> \
	__c_name__(InputIterator first, InputIterator last, \
					const key_compare& cmp, const allocator_type& alloc) \
		: _base(first, last, cmp, alloc) {} \
	\
	__c_name__(const base_type& right) \
		: _base(right) {} \
	\
	__c_name__(const base_type& right, const allocator_type& alloc) \
		: _base(right.key_comp(), alloc) { \
	    base_type tmp(right); \
	    _base.swap(tmp); } \
	\
	__c_name__(BOOST_RV_REF(this_type) right) \
		: _base(right.key_comp(), right.get_allocator()) { \
		this_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	__c_name__(const this_type& right) \
		: _base(right.key_comp(), right.get_allocator()) { \
		right.copy_to_base(_base); } \
	\
	__c_name__(BOOST_RV_REF(this_type) right, const allocator_type& alloc) \
		: _base(right.key_comp(), alloc) { \
		this_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	__c_name__(const this_type& right, const allocator_type& alloc) \
		: _base(right.key_comp(), alloc) { \
		right.copy_to_base(_base); } \
	\
	YGGR_PP_SAFE_KV_CONSTRUCTOR_RVREF_IMPL( __c_name__ ) \
	\
	YGGR_PP_SAFE_KV_CONSTRUCTOR_IL_IMPL( __c_name__ )


#define YGGR_PP_SAFE_KV_ISOMERIC_SAME_CONSTRUCTOR_IMPL( \
		__c_name__, __type_count__, __base_tpl__, __is_isomeric_same__ ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename IMCT ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), > ) \
	__c_name__(BOOST_RV_REF_BEG \
					__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) > \
				BOOST_RV_REF_END right, \
				typename \
					boost::enable_if< \
						__is_isomeric_same__< \
							__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.key_comp(), right.get_allocator()) { \
		typedef \
			__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) > right_type; \
		right_type& right_ref = right; \
		_base.swap(reinterpret_cast<base_type&>(right_ref)); } \
	\
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename IMCT ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), > ) \
	__c_name__(const \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >& right, \
				typename \
					boost::enable_if< \
						__is_isomeric_same__< \
							__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(reinterpret_cast<const base_type&>(right)){} \
	\
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename IMCT ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), > ) \
	__c_name__(BOOST_RV_REF_BEG \
					__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) > \
				BOOST_RV_REF_END right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						__is_isomeric_same__< \
							__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >, \
							base_type>, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.key_comp(), alloc) { \
		typedef __base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) > right_type; \
		right_type& right_ref = right; \
		_base.swap(reinterpret_cast<base_type&>(right_ref)); } \
	\
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename IMCT ) \
	BOOST_PP_EXPR_IF( BOOST_PP_SUB( __type_count__, 1 ), > ) \
	__c_name__(const \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >& right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						__is_isomeric_same__< \
							__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), IMCT ) >, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(reinterpret_cast<const base_type&>(right), alloc) {} \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(BOOST_RV_REF_BEG \
					__c_name__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ > \
				BOOST_RV_REF_END right, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.key_comp(), right.get_allocator()) { \
		typedef \
			__c_name__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		right_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(const \
				__c_name__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					__base_tpl__ >& right, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.key_comp(), right.get_allocator()) { \
		right.copy_to_base(_base); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(BOOST_RV_REF_BEG \
					__c_name__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ > \
				BOOST_RV_REF_END right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.key_comp(), alloc) { \
		typedef \
			__c_name__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		right_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(const \
				__c_name__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					__base_tpl__ >& right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.key_comp(), alloc) { \
		right.copy_to_base(_base); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(BOOST_RV_REF_BEG \
					__c_name__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ > \
				BOOST_RV_REF_END right, \
				typename \
					boost::enable_if< \
						__is_isomeric_same__< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.key_comp(), right.get_allocator()) { \
		typedef \
			__c_name__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_type& right_ref = right; \
		right_ref.swap(reinterpret_cast<right_base_type&>(_base)); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(const \
				__c_name__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					__base_tpl__ >& right, \
				typename \
					boost::enable_if< \
						__is_isomeric_same__< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.key_comp(), right.get_allocator()) { \
		typedef \
			__c_name__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		typedef typename right_type::base_type right_base_type; \
		right.copy_to_base(reinterpret_cast<right_base_type&>(_base)); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(BOOST_RV_REF_BEG \
					__c_name__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ > \
				BOOST_RV_REF_END right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						__is_isomeric_same__< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.key_comp(), alloc) { \
		typedef \
			__c_name__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_type& right_ref = right; \
		right_ref.swap(reinterpret_cast<right_base_type&>(_base)); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(const \
				__c_name__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					__base_tpl__ >& right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						__is_isomeric_same__< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.key_comp(), alloc) { \
		typedef \
			__c_name__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		typedef typename right_type::base_type right_base_type; \
		right.copy_to_base(reinterpret_cast<right_base_type&>(_base)); }

// ukv
#define YGGR_PP_SAFE_UKV_CONSTRUCTOR_IMPL( __c_name__ ) \
	__c_name__(void) {} \
	\
	explicit __c_name__(const allocator_type& alloc) \
		: _base(alloc) {} \
	\
	explicit __c_name__(const key_equal& cmp, const allocator_type& alloc) \
		: _base(boost::unordered::detail::default_bucket_count, \
						hasher(), cmp, alloc) {} \
	\
	explicit __c_name__(const hasher& hash_foo, const key_equal& cmp, const allocator_type& alloc) \
		: _base(boost::unordered::detail::default_bucket_count, hash_foo, cmp, alloc) {} \
	\
	__c_name__(size_type n, const hasher& hash_foo, const key_equal& cmp, const allocator_type& alloc) \
		: _base(n, hash_foo, cmp, alloc) {} \
	\
	template<typename InputIt> \
	__c_name__(InputIt start, InputIt last) \
		: _base(start, last) {} \
	\
	template<typename InputIt> \
	__c_name__(InputIt start, InputIt last, \
				const allocator_type& alloc) \
		: _base(start, last, \
				boost::unordered::detail::default_bucket_count, \
				hasher(), key_equal(), alloc) {} \
	\
	template<typename InputIt> \
	__c_name__(InputIt start, InputIt last, \
				const key_equal& cmp, \
				const allocator_type& alloc) \
		: _base(start, last, \
				boost::unordered::detail::default_bucket_count, \
				hasher(), cmp, alloc) {} \
	\
	template<typename InputIt> \
	__c_name__(InputIt start, InputIt last, \
					const hasher& hash_foo, \
					const key_equal& cmp, \
					const allocator_type& alloc) \
		: _base(start, last, \
				boost::unordered::detail::default_bucket_count, \
				hash_foo, cmp, alloc) {} \
	\
	template <class InputIt> \
	__c_name__(InputIt start, InputIt last, \
						size_type n, \
						const hasher& hash_foo, \
						const key_equal& cmp, \
						const allocator_type& alloc) \
		: _base(start, last, n, hash_foo, cmp, alloc) {} \
	\
	__c_name__(const base_type& right) \
		: _base(right) {} \
	\
	__c_name__(const base_type& right, const allocator_type& alloc) \
		: _base(right, alloc) {} \
	\
	__c_name__(BOOST_RV_REF(this_type) right) \
		: _base(right.get_allocator()) { \
		this_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	__c_name__(const this_type& right) \
		: _base(right.get_allocator()) { \
		right.copy_to_base(_base); } \
	\
	__c_name__(BOOST_RV_REF(this_type) right, const allocator_type& alloc) \
		: _base(alloc) { \
		this_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	__c_name__(const this_type& right, const allocator_type& alloc) \
		: _base(alloc) { \
		right.copy_to_base(_base); } \
	\
	YGGR_PP_SAFE_UKV_CONSTRUCTOR_RVREF_IMPL( __c_name__ ) \
	\
	YGGR_PP_SAFE_UKV_CONSTRUCTOR_IL_IMPL( __c_name__ )

#define YGGR_PP_SAFE_UKV_ISOMERIC_SAME_CONSTRUCTOR_IMPL( __c_name__, __type_count__, __base_tpl__ ) \
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(BOOST_RV_REF_BEG \
					__c_name__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ > \
				BOOST_RV_REF_END right, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.get_allocator()) { \
		typedef \
			__c_name__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		right_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(const \
				__c_name__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					__base_tpl__ >& right, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.get_allocator()) { \
		right.copy_to_base(_base); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(BOOST_RV_REF_BEG \
					__c_name__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
						YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
						__base_tpl__ > \
				BOOST_RV_REF_END right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(alloc) { \
		typedef \
			__c_name__< \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
				YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
				__base_tpl__ > right_type; \
		right_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) \
	__c_name__(const \
				__c_name__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
					YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
					__base_tpl__ >& right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename \
								__c_name__< \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) \
									YGGR_PP_SYMBOL_IF( __type_count__, YGGR_PP_SYMBOL_COMMA) \
									__base_tpl__ >::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(alloc) { \
		right.copy_to_base(_base); }

// destructor
#define YGGR_PP_SAFE_KV_DESTRUCTOR_IMPL( __c_name__ ) \
	~__c_name__(void) {}

#define YGGR_PP_SAFE_UKV_DESTRUCTOR_IMPL( __c_name__ ) \
	YGGR_PP_SAFE_KV_DESTRUCTOR_IMPL( __c_name__ )

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_CONSTRUCT_IMPL_HPP__
