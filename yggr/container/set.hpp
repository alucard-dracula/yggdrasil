//set.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, andz/or sell
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

#ifndef __YGGR_CONTAINER_SET_HPP__
#define __YGGR_CONTAINER_SET_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container/container_selector.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/typename_expand.hpp>
#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/typeof/default_value.hpp>

#include <yggr/container/get_set_options.hpp>
#include <yggr/container/basic_function_adapter.hpp>
#include <yggr/container/node_handle_traits.hpp>

#include <yggr/container/is_isomeric_same_set.hpp>
#if !(BOOST_VERSION < 105600)
#	include <yggr/container/is_isomeric_same_options_set.hpp>
#endif // !(BOOST_VERSION < 105600)

#include <yggr/container/detail/is_xxx_container.hpp>
#include <yggr/container/detail/is_s_or_m_container.hpp>
#include <yggr/container/detail/call_erase_helper.hpp>
#include <yggr/container/detail/swap_def.hpp>
#include <yggr/container/detail/comparer_def.hpp>
#include <yggr/container/detail/range_size_def.hpp>

#include <yggr/container/detail/has_emplace.hpp>
#include <yggr/container/detail/has_merge.hpp>
#include <yggr/container/detail/has_contains.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_set.hpp>

#include <yggr/container/detail/erase_impl.hpp>
#include <yggr/container/detail/set_merge_impl.hpp>
#include <yggr/container/detail/set_merger_sel.hpp>

#include <boost/container/set.hpp>
#include <boost/utility/enable_if.hpp>

#include <set>

YGGR_PP_IS_XXX_CONTAINER_DEF(3, std_set, std::set);
YGGR_PP_IS_XXX_CONTAINER_DEF(3, std_multiset, std::multiset);

#if BOOST_VERSION < 105600

	YGGR_PP_IS_XXX_CONTAINER_DEF(3, boost_set, boost::container::set);
	YGGR_PP_IS_XXX_CONTAINER_DEF(3, boost_multiset, boost::container::multiset);

#else

	YGGR_PP_IS_XXX_CONTAINER_DEF(4, boost_set, boost::container::set);
	YGGR_PP_IS_XXX_CONTAINER_DEF(4, boost_multiset, boost::container::multiset);

#endif // BOOST_VERSION < 105600


// compatibility caller
// -----------------set---------------
namespace yggr
{
namespace container
{

//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename C, typename A, typename ...Args> inline
typename 
	boost::enable_if
	<
		detail::has_any_emplace< std::set<T, C, A> >,
		std::pair< typename std::set<T, C, A>::iterator, bool> 
	>::type
	emplace(std::set<T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename C, typename A, typename ...Args> inline
typename 
	boost::disable_if
	<
		detail::has_any_emplace< std::set<T, C, A> >,
		std::pair< typename std::set<T, C, A>::iterator, bool> 
	>::type
	emplace(std::set<T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.insert(T(boost::forward<Args>(args)...));
}

template<typename T, typename C, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_hint< std::set<T, C, A> >,
		typename std::set<T, C, A>::iterator 
	>::type
	emplace_hint(std::set<T, C, A>& c, 
					typename std::set<T, C, A>::const_iterator pos, 
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

template<typename T, typename C, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_hint< std::set<T, C, A> >,
		typename std::set<T, C, A>::iterator 
	>::type
	emplace_hint(std::set<T, C, A>& c, 
					typename std::set<T, C, A>::const_iterator pos, 
					BOOST_FWD_REF(Args)... args)
{
	typedef typename std::set<T, C, A>::value_type value_type;
	return c.insert(pos, value_type(boost::forward<Args>(args)...));
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace< std::set<T, C, A> >, \
			std::pair< typename std::set<T, C, A>::iterator, bool> >::type \
			emplace(std::set<T, C, A>& c \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename std::set<T, C, A>::value_type value_type; \
			return \
				c.emplace(BOOST_PP_EXPR_IF(BOOST_PP_NOT(__n__), value_type) \
							YGGR_PP_SYMBOL_IF(BOOST_PP_NOT(__n__), YGGR_PP_SYMBOL_PARENTHESES_L) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) \
							YGGR_PP_SYMBOL_IF(BOOST_PP_NOT(__n__), YGGR_PP_SYMBOL_PARENTHESES_R) ); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace< std::set<T, C, A> >, \
			std::pair< typename std::set<T, C, A>::iterator, bool> >::type \
			emplace(std::set<T, C, A>& c \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename std::set<T, C, A>::value_type value_type; \
			return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace_hint< std::set<T, C, A> >, \
									typename std::set<T, C, A>::iterator>::type \
			emplace_hint(std::set<T, C, A>& c, \
							typename std::set<T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename std::set<T, C, A>::value_type value_type; \
			return c.emplace_hint(pos, \
									BOOST_PP_EXPR_IF(BOOST_PP_NOT(__n__), value_type) \
									YGGR_PP_SYMBOL_IF(BOOST_PP_NOT(__n__), YGGR_PP_SYMBOL_PARENTHESES_L) \
										YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) \
									YGGR_PP_SYMBOL_IF(BOOST_PP_NOT(__n__), YGGR_PP_SYMBOL_PARENTHESES_R) ); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace_hint< std::set<T, C, A> >, \
									typename std::set<T, C, A>::iterator>::type \
			emplace_hint(std::set<T, C, A>& c, \
							typename std::set<T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename std::set<T, C, A>::value_type value_type; \
			return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }


#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// extract
template<typename T, typename C, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			detail::native_extractable< std::set<T, C, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename std::set<T, C, A>::node_type
	>::type
	extract(std::set<T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename T, typename C, typename A, typename KeyOrIter> inline
typename 
	boost::enable_if
	<
		detail::native_extractable< std::set<T, C, A> >,
		typename std::set<T, C, A>::node_type
	>::type
	extract(std::set<T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

//template<typename T, typename C, typename A> inline
//typename 
//	boost::enable_if
//	<
//		detail::native_extractable< std::set<T, C, A> >,
//		typename std::set<T, C, A>::node_type
//	>::type
//	extract(std::set<T, C, A>& c, typename std::set<T, C, A>::const_iterator iter)
//{
//	return c.extract(iter);
//}

// insert_node_type
template<typename T, typename C, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable< std::set<T, C, A> >,
		typename std::set<T, C, A>::insert_return_type
	>::type
	insert(std::set<T, C, A>& c, 
			BOOST_RV_REF_BEG typename std::set<T, C, A>::node_type BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename T, typename C, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable< std::set<T, C, A> >,
		typename std::set<T, C, A>::iterator
	>::type
	insert(std::set<T, C, A>& c, 
			typename std::set<T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG typename std::set<T, C, A>::node_type BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// merge

namespace detail
{

template<typename T, typename C, typename A, typename T2, typename C2, typename A2>	
struct set_merger_sel_rule_is_same_namespace<std::set<T, C, A>, std::multiset<T2, C2, A2> >
	: public 
		set_merger_sel_rule_is_different_comp_only
		<
			std::set<T, C, A>, 
			std::multiset<T2, C2, A2> 
		>
{
};

} // namespace detail

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(std::set<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2> BOOST_RV_REF_END right)
{
	typedef std::set<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(std::set<T, C, A>& c, SetOrMSet<T, C2, A2>& right)
{
	typedef std::set<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(std::set<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef std::set<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(std::set<T, C, A>& c, SetOrMSet<T, C2, A2, O2>& right)
{
	typedef std::set<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename T, typename C, typename A, typename K> inline
typename boost::enable_if<detail::has_any_contains< std::set<T, C, A> >, bool>::type
	contains(const std::set<T, C, A>& c, const K& key)
{
	return c.contains(key);
}

template<typename T, typename C, typename A, typename K> inline
typename boost::disable_if<detail::has_any_contains< std::set<T, C, A> >, bool>::type
	contains(const std::set<T, C, A>& c, const K& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename T, typename C, typename A, typename Cmper> inline
typename std::set<T, C, A>::size_type
    erase_if(std::set<T, C, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#	if BOOST_VERSION < 105600

	template<typename T, typename C, typename A, typename ...Args> inline
	typename 
		boost::enable_if
		<
			detail::has_any_emplace< boost::container::set<T, C, A> >,
			std::pair< typename boost::container::set<T, C, A>::iterator, bool> 
		>::type
		emplace(boost::container::set<T, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename T, typename C, typename A, typename ...Args> inline
	typename 
		boost::disable_if
		<
			detail::has_any_emplace< boost::container::set<T, C, A> >,
			std::pair< typename boost::container::set<T, C, A>::iterator, bool> 
		>::type
		emplace(boost::container::set<T, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.insert(T(boost::forward<Args>(args)...));
	}

	template<typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::enable_if
		<
			detail::has_any_emplace_hint< boost::container::set<T, C, A> >,
			typename boost::container::set<T, C, A>::iterator 
		>::type
		emplace_hint(boost::container::set<T, C, A>& c, 
						typename boost::container::set<T, C, A>::const_iterator pos, 
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

	template<typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::disable_if
		<
			detail::has_any_emplace_hint< boost::container::set<T, C, A> >,
			typename boost::container::set<T, C, A>::iterator 
		>::type
		emplace_hint(boost::container::set<T, C, A>& c, 
						typename boost::container::set<T, C, A>::const_iterator pos, 
						BOOST_FWD_REF(Args)... args)
	{
		return c.insert(pos, T(boost::forward<Args>(args)...));
	}

#	else

	template<typename T, typename C, typename A, typename O, typename ...Args> inline
	std::pair<typename boost::container::set<T, C, A, O>::iterator, bool>
		emplace(boost::container::set<T, C, A, O>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename T, typename C, typename A, typename O, typename ...Args> inline
	typename boost::container::set<T, C, A, O>::iterator 
		emplace_hint(boost::container::set<T, C, A, O>& c, 
						typename boost::container::set<T, C, A, O>::const_iterator pos, 
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#	endif // BOOST_VERSION < 105600

#else

#	if BOOST_VERSION < 105600

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename boost::container::set<T, C, A>::iterator, bool> \
			emplace(boost::container::set<T, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename boost::container::set<T, C, A>::value_type value_type; \
			return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::container::set<T, C, A>::iterator \
			emplace_hint(boost::container::set<T, C, A>& c, \
							typename boost::container::set<T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename boost::container::set<T, C, A>::value_type value_type; \
			return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#	else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename boost::container::set<T, C, A, O>::iterator, bool> \
			emplace(boost::container::set<T, C, A, O>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C, typename A , typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::container::set<T, C, A, O>::iterator \
			emplace_hint(boost::container::set<T, C, A, O>& c, \
							typename boost::container::set<T, C, A, O>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	endif // BOOST_VERSION < 105600

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#if (BOOST_VERSION < 105600)

// extract
template<typename T, typename C, typename A, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::node_handle_traits<boost::container::set<T, C, A> >::node_type
	>::type
	extract(boost::container::set<T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return detail::set_node_extract(c, boost::move(key));
}

template<typename T, typename C, typename A, typename KeyOrIter> inline
typename detail::node_handle_traits<boost::container::set<T, C, A> >::node_type
	extract(boost::container::set<T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return detail::set_node_extract(c, key_or_iter);
}

// insert
template<typename T, typename C, typename A> inline
typename detail::node_handle_traits<boost::container::set<T, C, A> >::insert_return_type
	insert(boost::container::set<T, C, A>& c, 
			BOOST_RV_REF_BEG 
				typename detail::node_handle_traits<boost::container::set<T, C, A> >::node_type 
			BOOST_RV_REF_END node)
{
	return detail::set_node_insert(c, boost::move(node));
}

template<typename T, typename C, typename A> inline
typename detail::node_handle_traits<boost::container::set<T, C, A> >::hint_insert_return_type
	insert(boost::container::set<T, C, A>& c, 
			typename boost::container::set<T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG 
				typename detail::node_handle_traits<boost::container::set<T, C, A> >::node_type 
			BOOST_RV_REF_END node)
{
	return detail::set_node_insert(c, hint, boost::move(node));
}

//merge

namespace detail
{

/*
why not use has_extract to judge whether to use chopsticks or not,
because global extract function is hacing into boost::container,
so has_extract not sure if you have extract.
*/

template<typename T, typename C, typename A, typename C2>	
struct set_merger_sel_rule_is_different_comp_only<boost::container::set<T, C, A>, 
													boost::container::set<T, C2, A> >
	: public set_merger_eti
{
};

template<typename T, typename C, typename A, typename C2>	
struct set_merger_sel_rule_is_different_comp_only<boost::container::set<T, C, A>, 
													boost::container::multiset<T, C2, A> >
	: public set_merger_eti
{
};

template<typename T, typename C, typename A, typename T2, typename C2, typename A2>	
struct set_merger_sel_rule_is_same_namespace<boost::container::set<T, C, A>, 
												boost::container::multiset<T2, C2, A2> >
	: public 
		set_merger_sel_rule_is_different_comp_only
		<
			boost::container::set<T, C, A>, 
			boost::container::multiset<T2, C2, A2> 
		>
{
};

} // namespace detail

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(boost::container::set<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2> BOOST_RV_REF_END right)
{
	typedef boost::container::set<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(boost::container::set<T, C, A>& c, SetOrMSet<T, C2, A2>& right)
{
	typedef boost::container::set<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(boost::container::set<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef boost::container::set<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(boost::container::set<T, C, A>& c, SetOrMSet<T, C2, A2, O2>& right)
{
	typedef boost::container::set<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename T, typename C, typename A, typename K> inline
typename boost::enable_if<detail::has_any_contains< boost::container::set<T, C, A> >, bool>::type
	contains(const boost::container::set<T, C, A>& c, const K& key)
{
	return c.contains(key);
}

template<typename T, typename C, typename A, typename K> inline
typename boost::disable_if<detail::has_any_contains< boost::container::set<T, C, A> >, bool>::type
	contains(const boost::container::set<T, C, A>& c, const K& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename T, typename C, typename A, typename Cmper> inline
typename boost::container::set<T, C, A>::size_type
    erase_if(boost::container::set<T, C, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#else

// node_handle_traits using default

// extract
template<typename T, typename C, typename A, typename SO, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::node_handle_traits< boost::container::set<T, C, A, SO> >::node_type
	>::type
	extract(boost::container::set<T, C, A, SO>& c, BOOST_RV_REF(Key) key)
{
	return detail::set_node_extract(c, boost::move(key));
}

template<typename T, typename C, typename A, typename SO, typename KeyOrIter> inline
typename detail::node_handle_traits< boost::container::set<T, C, A, SO> >::node_type
	extract(boost::container::set<T, C, A, SO>& c, const KeyOrIter& key_or_iter)
{
	return detail::set_node_extract(c, key_or_iter);
}

// insert
template<typename T, typename C, typename A, typename SO> inline
typename detail::node_handle_traits< boost::container::set<T, C, A, SO> >::insert_return_type
	insert(boost::container::set<T, C, A, SO>& c, 
			BOOST_RV_REF_BEG 
				typename detail::node_handle_traits< boost::container::set<T, C, A, SO> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::set_node_insert(c, boost::move(node));
}


template<typename T, typename C, typename A, typename SO> inline
typename detail::node_handle_traits< boost::container::set<T, C, A, SO> >::hint_insert_return_type
	insert(boost::container::set<T, C, A, SO>& c, 
			typename boost::container::set<T, C, A, SO>::const_iterator hint,
			BOOST_RV_REF_BEG 
				typename detail::node_handle_traits< boost::container::set<T, C, A, SO> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::set_node_insert(c, hint, boost::move(node));
}

//merge

namespace detail
{


#if (BOOST_VERSION < 106200)

template<typename T, typename C, typename A, typename O, typename C2>	
struct set_merger_sel_rule_is_different_comp_only<boost::container::set<T, C, A, O>,
													boost::container::set<T, C2, A, O> >
	: public set_merger_eti
{
};

template<typename T, typename C, typename A, typename O, typename C2>
struct set_merger_sel_rule_is_different_comp_only<boost::container::set<T, C, A, O>,
													boost::container::multiset<T, C2, A, O> >
	: public set_merger_eti
{
};

#else

template<typename T, typename C, typename A, typename O, typename C2>	
struct set_merger_sel_rule_is_different_comp_only<boost::container::set<T, C, A, O>,
													boost::container::set<T, C2, A, O> >
	: public set_merger_inner
{
};

template<typename T, typename C, typename A, typename O, typename C2>
struct set_merger_sel_rule_is_different_comp_only<boost::container::set<T, C, A, O>,
													boost::container::multiset<T, C2, A, O> >
	: public set_merger_inner
{
};


#endif // (BOOST_VERSION < 106200)

template<typename T, typename C, typename A, typename O, typename T2, typename C2, typename A2, typename O2>	
struct set_merger_sel_rule_is_same_namespace<boost::container::set<T, C, A, O>, 
												boost::container::multiset<T2, C2, A2, O2> >
	: public 
		set_merger_sel_rule_is_different_comp_only
		<
			boost::container::set<T, C, A, O>, 
			boost::container::multiset<T2, C2, A2, O2> 
		>
{
};

} // namespace detail

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename O, typename C2, typename A2> inline
void merge(boost::container::set<T, C, A, O>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2> BOOST_RV_REF_END right)
{
	typedef boost::container::set<T, C, A, O> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename O, typename C2, typename A2> inline
void merge(boost::container::set<T, C, A, O>& c, SetOrMSet<T, C2, A2>& right)
{
	typedef boost::container::set<T, C, A, O> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename O, typename C2, typename A2, typename O2> inline
void merge(boost::container::set<T, C, A, O>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef boost::container::set<T, C, A, O> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename O, typename C2, typename A2, typename O2> inline
void merge(boost::container::set<T, C, A, O>& c, SetOrMSet<T, C2, A2, O2>& right)
{
	typedef boost::container::set<T, C, A, O> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename T, typename C, typename A, typename SO, typename K> inline
typename boost::enable_if<detail::has_any_contains< boost::container::set<T, C, A, SO> >, bool>::type
	contains(const boost::container::set<T, C, A, SO>& c, const K& key)
{
	return c.contains(key);
}

template<typename T, typename C, typename A, typename SO, typename K> inline
typename boost::disable_if<detail::has_any_contains< boost::container::set<T, C, A, SO> >, bool>::type
	contains(const boost::container::set<T, C, A, SO>& c, const K& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename T, typename C, typename A, typename SO, typename Cmper> inline
typename boost::container::set<T, C, A, SO>::size_type
    erase_if(boost::container::set<T, C, A, SO>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#endif // (BOOST_VERSION < 105600)


} // namespace container
} // namespace yggr

// -----------------multiset---------------
namespace yggr
{
namespace container
{

//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename C, typename A, typename ...Args> inline
typename 
	boost::enable_if
	<
		detail::has_any_emplace< std::multiset<T, C, A> >,
		typename std::multiset<T, C, A>::iterator
	>::type
	emplace(std::multiset<T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename C, typename A, typename ...Args> inline
typename 
	boost::disable_if
	<
		detail::has_any_emplace< std::multiset<T, C, A> >,
		typename std::multiset<T, C, A>::iterator
	>::type
	emplace(std::multiset<T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.insert(T(boost::forward<Args>(args)...));
}


template<typename T, typename C, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_hint< std::multiset<T, C, A> >,
		typename std::multiset<T, C, A>::iterator 
	>::type
	emplace_hint(std::multiset<T, C, A>& c, 
					typename std::multiset<T, C, A>::const_iterator pos, 
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

template<typename T, typename C, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_hint< std::multiset<T, C, A> >,
		typename std::multiset<T, C, A>::iterator 
	>::type
	emplace_hint(std::multiset<T, C, A>& c, 
					typename std::multiset<T, C, A>::const_iterator pos, 
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::multiset<T, C, A>::iterator \
			emplace(std::multiset<T, C, A>& c \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename std::multiset<T, C, A>::value_type value_type; \
			return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_hint< std::multiset<T, C, A> >, \
			typename std::multiset<T, C, A>::iterator>::type \
			emplace_hint(std::multiset<T, C, A>& c, \
							typename std::multiset<T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename std::multiset<T, C, A>::value_type value_type; \
			return c.emplace_hint(pos, \
									BOOST_PP_EXPR_IF(BOOST_PP_NOT(__n__), value_type) \
									YGGR_PP_SYMBOL_IF(BOOST_PP_NOT(__n__), YGGR_PP_SYMBOL_PARENTHESES_L) \
										YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) \
									YGGR_PP_SYMBOL_IF(BOOST_PP_NOT(__n__), YGGR_PP_SYMBOL_PARENTHESES_R) ); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_hint< std::multiset<T, C, A> >, \
					typename std::multiset<T, C, A>::iterator>::type \
			emplace_hint(std::multiset<T, C, A>& c, \
							typename std::multiset<T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename std::multiset<T, C, A>::value_type value_type; \
			return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// extract

template<typename T, typename C, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			detail::native_extractable< std::multiset<T, C, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename std::multiset<T, C, A>::node_type
	>::type
	extract(std::multiset<T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename T, typename C, typename A, typename KeyOrIter> inline
typename 
	boost::enable_if
	<
		detail::native_extractable< std::multiset<T, C, A> >,
		typename std::multiset<T, C, A>::node_type
	>::type
	extract(std::multiset<T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

//template<typename T, typename C, typename A> inline
//typename 
//	boost::enable_if
//	<
//		detail::native_extractable< std::multiset<T, C, A> >,
//		typename std::multiset<T, C, A>::node_type
//	>::type
//	extract(std::multiset<T, C, A>& c, typename std::multiset<T, C, A>::const_iterator iter)
//{
//	return c.extract(iter);
//}

// insert
template<typename T, typename C, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable< std::multiset<T, C, A> >,
		typename std::multiset<T, C, A>::iterator
	>::type
	insert(std::multiset<T, C, A>& c, 
			BOOST_RV_REF_BEG typename std::multiset<T, C, A>::node_type BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename T, typename C, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable< std::multiset<T, C, A> >,
		typename std::multiset<T, C, A>::iterator
	>::type
	insert(std::multiset<T, C, A>& c, 
			typename std::multiset<T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG typename std::multiset<T, C, A>::node_type BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// merge

namespace detail
{

template<typename T, typename C, typename A, typename T2, typename C2, typename A2>	
struct set_merger_sel_rule_is_same_namespace<std::multiset<T, C, A>, std::set<T2, C2, A2> >
	: public 
		set_merger_sel_rule_is_different_comp_only
		<
			std::multiset<T, C, A>, 
			std::set<T2, C2, A2> >
{
};

} // namespace detail

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(std::multiset<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2> BOOST_RV_REF_END right)
{
	typedef std::multiset<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(std::multiset<T, C, A>& c, SetOrMSet<T, C2, A2>& right)
{
	typedef std::multiset<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(std::multiset<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef std::multiset<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(std::multiset<T, C, A>& c, SetOrMSet<T, C2, A2, O2>& right)
{
	typedef std::multiset<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename T, typename C, typename A, typename K> inline
typename boost::enable_if<detail::has_any_contains< std::multiset<T, C, A> >, bool>::type
	contains(const std::multiset<T, C, A>& c, const K& key)
{
	return c.contains(key);
}

template<typename T, typename C, typename A, typename K> inline
typename boost::disable_if<detail::has_any_contains< std::multiset<T, C, A> >, bool>::type
	contains(const std::multiset<T, C, A>& c, const K& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename T, typename C, typename A, typename Cmper> inline
typename std::multiset<T, C, A>::size_type
    erase_if(std::multiset<T, C, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#	if BOOST_VERSION < 105600

	template<typename T, typename C, typename A, typename ...Args> inline
	typename 
		boost::enable_if
		<
			detail::has_any_emplace< boost::container::multiset<T, C, A> >,
			typename boost::container::multiset<T, C, A>::iterator
		>::type
		emplace(boost::container::multiset<T, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename T, typename C, typename A, typename ...Args> inline
	typename 
		boost::disable_if
		<
			detail::has_any_emplace< boost::container::multiset<T, C, A> >,
			typename boost::container::multiset<T, C, A>::iterator
		>::type
		emplace(boost::container::multiset<T, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.insert(T(boost::forward<Args>(args)...));
	}


	template<typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::enable_if
		<
			detail::has_any_emplace_hint< boost::container::multiset<T, C, A> >,
			typename boost::container::multiset<T, C, A>::iterator 
		>::type
		emplace_hint(boost::container::multiset<T, C, A>& c, 
						typename boost::container::multiset<T, C, A>::const_iterator pos, 
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

	template<typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::disable_if
		<
			detail::has_any_emplace_hint< boost::container::multiset<T, C, A> >,
			typename boost::container::multiset<T, C, A>::iterator 
		>::type
		emplace_hint(boost::container::multiset<T, C, A>& c, 
						typename boost::container::multiset<T, C, A>::const_iterator pos, 
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#	else

	template<typename T, typename C, typename A, typename O, typename ...Args> inline
	typename boost::container::multiset<T, C, A, O>::iterator 
		emplace(boost::container::multiset<T, C, A, O>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename T, typename C, typename A, typename O, typename ...Args>
	typename boost::container::multiset<T, C, A, O>::iterator 
		emplace_hint(boost::container::multiset<T, C, A, O>& c, 
						typename boost::container::multiset<T, C, A, O>::const_iterator pos, 
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#	endif // BOOST_VERSION < 105600

#else

#	if BOOST_VERSION < 105600

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< boost::container::multiset<T, C, A> >, \
									typename boost::container::multiset<T, C, A>::iterator>::type \
			emplace(boost::container::multiset<T, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< boost::container::multiset<T, C, A> >, \
									typename boost::container::multiset<T, C, A>::iterator>::type \
			emplace(boost::container::multiset<T, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename boost::container::multiset<T, C, A>::value_type value_type; \
			return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace_hint< boost::container::multiset<T, C, A> >, \
									typename boost::container::multiset<T, C, A>::iterator>::type \
			emplace_hint(boost::container::multiset<T, C, A>& c, \
							typename boost::container::multiset<T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace_hint< boost::container::multiset<T, C, A> >, \
									typename boost::container::multiset<T, C, A>::iterator>::type \
			emplace_hint(boost::container::multiset<T, C, A>& c, \
							typename boost::container::multiset<T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename boost::container::multiset<T, C, A>::value_type value_type; \
			return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#	else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::container::multiset<T, C, A, O>::iterator \
			emplace(boost::container::multiset<T, C, A, O>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::container::multiset<T, C, A, O>::iterator \
			emplace_hint(boost::container::multiset<T, C, A, O>& c, \
							typename boost::container::multiset<T, C, A, O>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	endif // BOOST_VERSION < 105600

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#if (BOOST_VERSION < 105600)

// extract
template<typename T, typename C, typename A, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::node_handle_traits<boost::container::multiset<T, C, A> >::node_type
	>::type
	extract(boost::container::multiset<T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return detail::multiset_node_extract(c, boost::move(key));
}

template<typename T, typename C, typename A, typename KeyOrIter> inline
typename detail::node_handle_traits<boost::container::multiset<T, C, A> >::node_type
	extract(boost::container::multiset<T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return detail::multiset_node_extract(c, key_or_iter);
}

// insert
template<typename T, typename C, typename A> inline
typename boost::container::multiset<T, C, A>::iterator
	insert(boost::container::multiset<T, C, A>& c, 
			BOOST_RV_REF_BEG 
				typename detail::node_handle_traits<boost::container::multiset<T, C, A> >::node_type 
			BOOST_RV_REF_END node)
{
	return detail::multiset_node_insert(c, boost::move(node));
}

template<typename T, typename C, typename A> inline
typename boost::container::multiset<T, C, A>::iterator
	insert(boost::container::multiset<T, C, A>& c, 
			typename boost::container::multiset<T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG 
				typename detail::node_handle_traits<boost::container::multiset<T, C, A> >::node_type 
			BOOST_RV_REF_END node)
{
	return detail::multiset_node_insert(c, hint, boost::move(node));
}

//merge

namespace detail
{

template<typename T, typename C, typename A, typename C2>	
struct set_merger_sel_rule_is_different_comp_only<boost::container::multiset<T, C, A>, 
													boost::container::multiset<T, C2, A> >
	: public set_merger_eti
{
};

template<typename T, typename C, typename A, typename C2>	
struct set_merger_sel_rule_is_different_comp_only<boost::container::multiset<T, C, A>, 
													boost::container::set<T, C2, A> >
	: public set_merger_eti
{
};

template<typename T, typename C, typename A, typename T2, typename C2, typename A2>	
struct set_merger_sel_rule_is_same_namespace<boost::container::multiset<T, C, A>, 
												boost::container::set<T2, C2, A2> >
	: public 
		set_merger_sel_rule_is_different_comp_only
		<
			boost::container::multiset<T, C, A>, 
			boost::container::set<T2, C2, A2> 
		>
{
};

} // namespace detail

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(boost::container::multiset<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2> BOOST_RV_REF_END right)
{
	typedef boost::container::multiset<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(boost::container::multiset<T, C, A>& c, SetOrMSet<T, C2, A2>& right)
{
	typedef boost::container::multiset<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(boost::container::multiset<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef boost::container::multiset<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(boost::container::multiset<T, C, A>& c, SetOrMSet<T, C2, A2, O2>& right)
{
	typedef boost::container::multiset<T, C, A> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename T, typename C, typename A, typename K> inline
typename boost::enable_if<detail::has_any_contains< boost::container::multiset<T, C, A> >, bool>::type
	contains(const boost::container::multiset<T, C, A>& c, const K& key)
{
	return c.contains(key);
}

template<typename T, typename C, typename A, typename K> inline
typename boost::disable_if<detail::has_any_contains< boost::container::multiset<T, C, A> >, bool>::type
	contains(const boost::container::multiset<T, C, A>& c, const K& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename T, typename C, typename A, typename Cmper> inline
typename boost::container::multiset<T, C, A>::size_type
    erase_if(boost::container::multiset<T, C, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#else

// node_handle_traits using default

// extract
template<typename T, typename C, typename A, typename SO, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::node_handle_traits< boost::container::multiset<T, C, A, SO> >::node_type
	>::type
	extract(boost::container::multiset<T, C, A, SO>& c, BOOST_RV_REF(Key) key)
{
	return detail::multiset_node_extract(c, boost::move(key));
}

template<typename T, typename C, typename A, typename SO, typename KeyOrIter> inline
typename detail::node_handle_traits< boost::container::multiset<T, C, A, SO> >::node_type
	extract(boost::container::multiset<T, C, A, SO>& c, const KeyOrIter& key_or_iter)
{
	return detail::multiset_node_extract(c, key_or_iter);
}
// insert
template<typename T, typename C, typename A, typename SO> inline
typename boost::container::multiset<T, C, A, SO>::iterator
	insert(boost::container::multiset<T, C, A, SO>& c, 
			BOOST_RV_REF_BEG 
				typename detail::node_handle_traits< boost::container::multiset<T, C, A, SO> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::multiset_node_insert(c, boost::move(node));
}

template<typename T, typename C, typename A, typename SO> inline
typename boost::container::multiset<T, C, A, SO>::iterator
	insert(boost::container::multiset<T, C, A, SO>& c, 
			typename boost::container::multiset<T, C, A, SO>::const_iterator hint,
			BOOST_RV_REF_BEG 
				typename detail::node_handle_traits< boost::container::multiset<T, C, A, SO> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::multiset_node_insert(c, hint, boost::move(node));
}

//merge

namespace detail
{

#if (BOOST_VERSION < 106200)

template<typename T, typename C, typename A, typename O, typename C2>	
struct set_merger_sel_rule_is_different_comp_only<boost::container::multiset<T, C, A, O>,
													boost::container::multiset<T, C2, A, O> >
	: public set_merger_eti
{
};

template<typename T, typename C, typename A, typename O, typename C2>
struct set_merger_sel_rule_is_different_comp_only<boost::container::multiset<T, C, A, O>,
													boost::container::set<T, C2, A, O> >
	: public set_merger_eti
{
};

#else

template<typename T, typename C, typename A, typename O, typename C2>	
struct set_merger_sel_rule_is_different_comp_only<boost::container::multiset<T, C, A, O>,
													boost::container::multiset<T, C2, A, O> >
	: public set_merger_inner
{
};

template<typename T, typename C, typename A, typename O, typename C2>
struct set_merger_sel_rule_is_different_comp_only<boost::container::multiset<T, C, A, O>,
													boost::container::set<T, C2, A, O> >
	: public set_merger_inner
{
};

#endif // (BOOST_VERSION < 106200)

template<typename T, typename C, typename A, typename O, typename T2, typename C2, typename A2, typename O2>	
struct set_merger_sel_rule_is_same_namespace<boost::container::multiset<T, C, A, O>, 
												boost::container::set<T2, C2, A2, O2> >
	: public 
		set_merger_sel_rule_is_different_comp_only
		<
			boost::container::multiset<T, C, A, O>, 
			boost::container::set<T2, C2, A2, O2> 
		>
{
};

} // namespace detail

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename O, typename C2, typename A2> inline
void merge(boost::container::multiset<T, C, A, O>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2> BOOST_RV_REF_END right)
{
	typedef boost::container::multiset<T, C, A, O> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename O, typename C2, typename A2> inline
void merge(boost::container::multiset<T, C, A, O>& c, SetOrMSet<T, C2, A2>& right)
{
	typedef boost::container::multiset<T, C, A, O> left_set_type;
	typedef SetOrMSet<T, C2, A2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename O, typename C2, typename A2, typename O2> inline
void merge(boost::container::multiset<T, C, A, O>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef boost::container::multiset<T, C, A, O> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	right_set_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename O, typename C2, typename A2, typename O2> inline
void merge(boost::container::multiset<T, C, A, O>& c, SetOrMSet<T, C2, A2, O2>& right)
{
	typedef boost::container::multiset<T, C, A, O> left_set_type;
	typedef SetOrMSet<T, C2, A2, O2> right_set_type;
	typedef detail::set_merger_sel<left_set_type, right_set_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename T, typename C, typename A, typename SO, typename K> inline
typename boost::enable_if<detail::has_any_contains< boost::container::multiset<T, C, A, SO> >, bool>::type
	contains(const boost::container::multiset<T, C, A, SO>& c, const K& key)
{
	return c.contains(key);
}

template<typename T, typename C, typename A, typename SO, typename K> inline
typename boost::disable_if<detail::has_any_contains< boost::container::multiset<T, C, A, SO> >, bool>::type
	contains(const boost::container::multiset<T, C, A, SO>& c, const K& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename T, typename C, typename A, typename SO, typename Cmper> inline
typename boost::container::multiset<T, C, A, SO>::size_type
    erase_if(boost::container::multiset<T, C, A, SO>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#endif // (BOOST_VERSION < 105600)


} // namespace container
} // namespace yggr

#if !(BOOST_VERSION < 105600)

namespace boost
{
namespace container
{

YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_COMPARER_FULL(
	4, 
	::boost::container::set, 
	::yggr::container::is_isomeric_same_options_set,
	inline)

YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_COMPARER_FULL(
	4, 
	::boost::container::multiset, 
	::yggr::container::is_isomeric_same_options_multiset,
	inline)

} // namespace container
} // namespace boost

namespace boost
{
namespace container
{
namespace detail
{
namespace swap_support
{
	YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_SWAP(
		4, 
		::boost::container::set, 
		::yggr::container::is_isomeric_same_options_set,
		inline )

	YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_SWAP(
		4, 
		::boost::container::multiset, 
		::yggr::container::is_isomeric_same_options_multiset,
		inline )

} // namespace swap_support
} // namespace detail

using detail::swap_support::swap;

} // namespace container
} // namespace boost

namespace std
{
	using ::boost::container::detail::swap_support::swap;
}// namespace std

namespace boost
{
	using container::detail::swap_support::swap;
} // namespace boost

#endif // !(BOOST_VERSION < 105600)

// boost::size support
namespace std
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, std::set)
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, std::multiset)

} // namespace std

namespace boost
{

#if BOOST_VERSION < 105600

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, boost::container::set)
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, boost::container::multiset)

#else

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, boost::container::set)
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, boost::container::multiset)

#endif // BOOST_VERSION < 105600

} // namespace boost

#if !(BOOST_VERSION < 105600)

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T, typename C, typename A>
class set;

template<typename T, typename C, typename A>
class multiset;

} // namespace detail
} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{

//-----------------set-----------------

template<typename T, 
			typename C = typename mplex::typename_expand_get<boost::container::set<T>, 1>::type, 
			typename A = typename mplex::typename_expand_get<boost::container::set<T>, 2>::type >
class set
	: public boost::container::set<T, C, A>
{
public:
	typedef A tpl_arg_allocator_type;

public:
	typedef boost::container::set<T, C, A> base_type;

protected:
	typedef typename 
		get_set_options
		<
			typename mplex::typename_expand_get<base_type, 3>::type
		>::type options_type;

public:
	typedef typename base_type::key_type				key_type;
	typedef typename base_type::value_type				value_type;
	typedef typename base_type::key_compare				key_compare;
	typedef typename base_type::value_compare			value_compare;
	typedef typename base_type::allocator_traits_type	allocator_traits_type;
	typedef typename base_type::pointer					pointer;
	typedef typename base_type::const_pointer			const_pointer;
	typedef typename base_type::reference				reference;
	typedef typename base_type::const_reference			const_reference;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::allocator_type			allocator_type;
	typedef typename base_type::stored_allocator_type	stored_allocator_type;
	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;

#	if (BOOST_VERSION < 106200)
	typedef typename node_handle_foo_hacking_set<base_type>::node_type node_type;
	typedef typename node_handle_foo_hacking_set<base_type>::insert_return_type insert_return_type;
#	else
	typedef typename base_type::node_type node_type;
	typedef typename base_type::insert_return_type insert_return_type;
#	endif // (BOOST_VERSION < 106200)

private:
	typedef set this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(void)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	set(const key_compare& comp,
		const allocator_type& a = allocator_type())
		: base_type(comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit 
	set(const allocator_type& a)
		: base_type(key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(InputIterator first, InputIterator last, 
			const key_compare& comp = key_compare(),
			const allocator_type& a = allocator_type())
		: base_type(first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(boost::container::ordered_unique_range_t, 
		InputIterator first, InputIterator last, 
		const key_compare& comp = key_compare(), 
		const allocator_type& a = allocator_type())
		: base_type(boost::container::ordered_range, first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(InputIterator first, InputIterator last, 
		const allocator_type& a)
		: base_type(first, last, key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(boost::container::ordered_range_t, 
		InputIterator first, InputIterator last, 
		const allocator_type& a)
		: base_type(boost::container::ordered_range, first, last, key_compare(), a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(std::initializer_list<value_type> il,
			const key_compare& comp = key_compare(), 
			const allocator_type& a = allocator_type())
		: base_type(il.begin(), il.end(), comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(std::initializer_list<value_type> il, const allocator_type& a)
		: base_type(il.begin(), il.end(), key_compare(), a)
	{
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(const base_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(BOOST_RV_REF(base_type) right, const allocator_type& a)
		: base_type(boost::move(right), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(const base_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(const this_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(const this_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	// isomeric_same
	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(BOOST_RV_REF_BEG
			boost::container::set<value_type, C2, A2, O2>
		BOOST_RV_REF_END right,
		typename
			boost::enable_if
			<
				is_isomeric_same_options_set
				<
					boost::container::set<value_type, C2, A2, O2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::set<value_type, C2, A2, O2>&>(right))))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(const boost::container::set<value_type, C2, A2, O2>& right,
		typename
			boost::enable_if
			<
				is_isomeric_same_options_set
				<
					boost::container::set<value_type, C2, A2, O2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(BOOST_RV_REF_BEG
			boost::container::set<value_type, C2, A2, O2>
		BOOST_RV_REF_END right,
		const allocator_type& alloc,
		typename
			boost::enable_if
			<
				is_isomeric_same_options_set
				<
					boost::container::set<value_type, C2, A2, O2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::set<value_type, C2, A2, O2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(const boost::container::set<value_type, C2, A2, O2>& right,
		const allocator_type& alloc,
		typename
			boost::enable_if
			<
				is_isomeric_same_options_set
				<
					boost::container::set<value_type, C2, A2, O2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(BOOST_RV_REF_BEG
			set<value_type, C2, A2>
		BOOST_RV_REF_END right,
		typename
			boost::enable_if
			<
				is_isomeric_same_options_set
				<
					typename set<value_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename set<value_type, C2, A2>::base_type&>(right))))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(const set<value_type, C2, A2>& right,
		typename
			boost::enable_if
			<
				is_isomeric_same_options_set
				<
					typename set<value_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(BOOST_RV_REF_BEG
			set<value_type, C2, A2>
		BOOST_RV_REF_END right, 
		const allocator_type& a,
		typename
			boost::enable_if
			<
				is_isomeric_same_options_set
				<
					typename set<value_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename set<value_type, C2, A2>::base_type&>(right))),
			a)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	set(const set<value_type, C2, A2>& right,
		const allocator_type& a,
		typename
			boost::enable_if
			<
				is_isomeric_same_options_set
				<
					typename set<value_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

	~set(void)
	{
	}
	
public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{  
		base_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::operator=(boost::move(right));
		return *this;
	}

	this_type& operator=(const base_type& right)
	{ 
		if(this == &right)
		{
			return *this;
		}
		
		base_type::operator=(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{  
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::operator=(boost::move(static_cast<base_type&>(right_ref)));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{ 
		if(this == &right)
		{
			return *this;
		}
		
		base_type::operator=(right);
		return *this;
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	inline this_type& operator=(std::initializer_list<value_type> il)
	{
		base_type::assign(il.begin(), il.end());
		return *this;
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				boost::container::set<value_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::set<value_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{  
		typedef boost::container::set<value_type, C2, A2, O2> right_type;
		
		right_type& right_ref = right;
		base_type::operator=(boost::move(reinterpret_cast<base_type&>(right_ref)));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				boost::container::set<value_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::set<value_type, C2, A2, O2>& right)
	{ 
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				typename set<value_type, C2, A2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						set<value_type, C2, A2>
					BOOST_RV_REF_END right)
	{
		typedef set<value_type, C2, A2> right_type;
		typedef typename right_type::base_type right_base_type;

		right_type& right_ref = right;
		base_type::operator=(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<right_base_type&>(right_ref))));
		return *this;
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				typename set<value_type, C2, A2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(const set<value_type, C2, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

public:
	using base_type::swap;

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		base_type::swap(right);
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				boost::container::set<value_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::set<value_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::set<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				boost::container::set<value_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::set<value_type, C2, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				typename set<value_type, C2, A2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					set<value_type, C2, A2>
				BOOST_RV_REF_END right)
	{
		typedef set<value_type, C2, A2> right_type;
		typedef typename right_type::base_type right_base_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_base_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_set
			<
				typename set<value_type, C2, A2>::base_type,
				base_type
			>,
			void
		>::type
		swap(set<value_type, C2, A2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

public:
	using base_type::get_allocator;
	using base_type::get_stored_allocator;

	using base_type::begin;
	using base_type::cbegin;

	using base_type::end;
	using base_type::cend;

	using base_type::rbegin; 
	using base_type::crbegin;

	using base_type::rend;
	using base_type::crend;

	using base_type::empty;
	
	using base_type::size;
	using base_type::max_size;
	
	using base_type::emplace;
	using base_type::emplace_hint;

	
	//insert
	// need compatibility node_insert, so using insert function wrapper
	inline std::pair<iterator, bool> 
		insert(BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(boost::forward<value_type>(val));
	}

	inline std::pair<iterator, bool> 
		insert(const value_type& val)
	{
		return base_type::insert(val);
	}
	
	inline iterator insert(const_iterator hint, BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(hint, boost::forward<value_type>(val));
	}

	inline iterator insert(const_iterator hint, const value_type& val)
	{
		return base_type::insert(hint, val);
	}

	template<typename Iter> inline
	void insert(Iter s, Iter e)
	{
		return base_type::insert(s, e);
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline void insert(std::initializer_list<value_type> il)
	{
		return base_type::insert(il);
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline insert_return_type insert(BOOST_RV_REF(node_type) nh)
	{
		return 
			detail::set_node_insert(
				static_cast<base_type&>(*this), boost::move(nh));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{
		return 
			detail::set_node_insert(
				static_cast<base_type&>(*this), hint, boost::move(nh));
	}

	using base_type::erase;
	
	using base_type::clear;

	using base_type::key_comp;
	using base_type::value_comp;

	using base_type::find;

	using base_type::count;

	using base_type::lower_bound;
	using base_type::upper_bound;
	
	using base_type::equal_range;

	using base_type::rebalance;

	// extract
#	if (BOOST_VERSION < 106200)
	template<typename Key> inline
	node_type extract(BOOST_RV_REF(Key) key)
	{
		return 
			detail::set_node_extract(
				static_cast<base_type&>(*this), boost::move(key));
	}

	inline node_type extract(const key_type& key)
	{
		return 
			detail::set_node_extract(
				static_cast<base_type&>(*this), key);
	}

	inline node_type extract(const_iterator iter)
	{
		return 
			detail::set_node_extract(
				static_cast<base_type&>(*this), iter);
	}
#	else

	using base_type::extract;

#	endif // (BOOST_VERSION < 106200)

	// merge-member

#if !(BOOST_VERSION < 106200)
	using base_type::merge;
#endif // !(BOOST_VERSION < 106200)

	template<template<typename _T, typename _C, typename _A> class SetOrMSet,
				typename C2, typename A2> inline
	void merge(BOOST_RV_REF_BEG SetOrMSet<value_type, C2, A2> BOOST_RV_REF_END right)
	{
		typedef SetOrMSet<value_type, C2, A2> right_type;

		right_type& right_ref = right;
		container::merge(static_cast<base_type&>(*this), right_ref);
	}

	template<template<typename _T, typename _C, typename _A> class SetOrMSet,
				typename C2, typename A2> inline
	void merge(SetOrMSet<value_type, C2, A2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
				typename C2, typename A2, typename O2> inline
	void merge(BOOST_RV_REF_BEG SetOrMSet<value_type, C2, A2, O2> BOOST_RV_REF_END right)
	{
		typedef SetOrMSet<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		container::merge(static_cast<base_type&>(*this), right_ref);
	}

	template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
				typename C2, typename A2, typename O2> inline
	void merge(SetOrMSet<value_type, C2, A2, O2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

#if (BOOST_VERSION < 106900)
	inline bool contains(const key_type& key) const
	{
		return base_type::find(key) != base_type::end();
	}

	template<typename K> inline
	bool contains(const K& key) const
	{
		return base_type::find(key) != base_type::end();
	}

#else
	using base_type::contains;
#endif // (BOOST_VERSION < 106900)
};

YGGR_PP_CONTAINER_COMPARER_FULL(3, set, inline)

YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL_MN(
	4, 3,
	::boost::container::set, 
	set, 
	is_isomeric_same_options_set,
	inline)


YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, set)

} // namespace detail


#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename T, typename C, typename A, typename ...Args> inline
std::pair<typename detail::set<T, C, A>::iterator, bool> 
	emplace(detail::set<T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename C, typename A, typename ...Args> inline
typename detail::set<T, C, A>::iterator 
	emplace_hint(detail::set<T, C, A>& c, 
					typename detail::set<T, C, A>::const_iterator pos, 
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename detail::set<T, C, A>::iterator, bool> \
			emplace(detail::set<T, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::set<T, C, A>::iterator \
			emplace_hint(detail::set<T, C, A>& c, \
							typename detail::set<T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

// extract
template<typename T, typename C, typename A, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::set<T, C, A>::node_type
	>::type
	extract(detail::set<T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename T, typename C, typename A, typename KeyOrIter> inline
typename detail::set<T, C, A>::node_type
	extract(detail::set<T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// insert
template<typename T, typename C, typename A> inline
typename detail::set<T, C, A>::insert_return_type
	insert(detail::set<T, C, A>& c, 
			BOOST_RV_REF_BEG 
				typename detail::set<T, C, A>::node_type 
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename T, typename C, typename A> inline
typename detail::set<T, C, A>::iterator
	insert(detail::set<T, C, A>& c, 
			typename detail::set<T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG 
				typename detail::set<T, C, A>::node_type 
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// merge
template<template<typename _T, typename _C, typename _A> class SetOrMset,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(detail::set<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMset<T, C2, A2> BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _T, typename _C, typename _A> class SetOrMset,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(detail::set<T, C, A>& c, SetOrMset<T, C2, A2>& right)
{
	c.merge(right);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMset,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(detail::set<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMset<T, C2, A2, O2> BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMset,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(detail::set<T, C, A>& c, SetOrMset<T, C2, A2, O2>& right)
{
	c.merge(right);
}

// contains
template<typename T, typename C, typename A, typename K> inline
bool contains(const detail::set<T, C, A>& c, const K& key)
{
	return c.contains(key);
}

// erase_if
template<typename T, typename C, typename A, typename Cmper> inline
typename detail::set<T, C, A>::size_type
    erase_if(detail::set<T, C, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{
namespace swap_support
{
	YGGR_PP_CONTAINER_SWAP(3, ::yggr::container::detail::set, inline)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		4, 3, 
		::boost::container::set, 
		::yggr::container::detail::set, 
		::yggr::container::is_isomeric_same_options_set,
		inline)

} // namespace swap_support

using swap_support::swap;

} // namespace detail
} // namespace container
} // namespace yggr

namespace std
{
	using ::yggr::container::detail::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::container::detail::swap_support::swap;
} // namespace boost

//---------------multiset--------------

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T, 
			typename C = typename mplex::typename_expand_get<boost::container::multiset<T>, 1>::type, 
			typename A = typename mplex::typename_expand_get<boost::container::multiset<T>, 2>::type >
class multiset
	: public boost::container::multiset<T, C, A>
{
public:
	typedef A tpl_arg_allocator_type;

public:
	typedef boost::container::multiset<T, C, A> base_type;

protected:
	typedef typename 
		get_multiset_options
		<
			typename mplex::typename_expand_get<base_type, 3>::type
		>::type options_type;

public:
	typedef typename base_type::key_type				key_type;
	typedef typename base_type::value_type				value_type;
	typedef typename base_type::key_compare				key_compare;
	typedef typename base_type::value_compare			value_compare;
	typedef typename base_type::allocator_traits_type	allocator_traits_type;
	typedef typename base_type::pointer					pointer;
	typedef typename base_type::const_pointer			const_pointer;
	typedef typename base_type::reference				reference;
	typedef typename base_type::const_reference			const_reference;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::allocator_type			allocator_type;
	typedef typename base_type::stored_allocator_type	stored_allocator_type;
	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;

#	if (BOOST_VERSION < 106200)
	typedef typename node_handle_foo_hacking_set<base_type>::node_type node_type;
#	else
	typedef typename base_type::node_type node_type;
#	endif // (BOOST_VERSION < 106200)

private:
	typedef multiset this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(void)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit 
	multiset(const key_compare& comp,
				const allocator_type& a = allocator_type())
		: base_type(comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit 
	multiset(const allocator_type& a)
		: base_type(a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(InputIterator first, InputIterator last, 
				const key_compare& comp = key_compare(),
				const allocator_type& a = allocator_type())
		: base_type(first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(boost::container::ordered_range_t, 
				InputIterator first, InputIterator last, 
				const key_compare& comp = key_compare(), 
				const allocator_type& a = allocator_type())
		: base_type(boost::container::ordered_range, first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(InputIterator first, InputIterator last, 
				const allocator_type& a)
		: base_type(first, last, key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(boost::container::ordered_range_t, 
				InputIterator first, InputIterator last, 
				const allocator_type& a)
		: base_type(boost::container::ordered_range, first, last, key_compare(), a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(std::initializer_list<value_type> il,
				const key_compare& comp = key_compare(), 
				const allocator_type& a = allocator_type())
		: base_type(il.begin(), il.end(), comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(std::initializer_list<value_type> il, const allocator_type& a)
		: base_type(il.begin(), il.end(), key_compare(), a)
	{
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(const base_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(const this_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(const this_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	// isomeric_same
	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(BOOST_RV_REF_BEG
					boost::container::multiset<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							boost::container::multiset<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multiset<value_type, C2, A2, O2>&>(right))))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(const boost::container::multiset<value_type, C2, A2, O2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							boost::container::multiset<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(BOOST_RV_REF_BEG
					boost::container::multiset<value_type, C2, A2, O2>
				BOOST_RV_REF_END right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							boost::container::multiset<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multiset<value_type, C2, A2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(const boost::container::multiset<value_type, C2, A2, O2>& right,
				const allocator_type& alloc,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							boost::container::multiset<value_type, C2, A2, O2>,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(BOOST_RV_REF_BEG
					multiset<value_type, C2, A2>
				BOOST_RV_REF_END right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							typename multiset<value_type, C2, A2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename multiset<value_type, C2, A2>::base_type&>(right))))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(const multiset<value_type, C2, A2>& right,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							typename multiset<value_type, C2, A2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(BOOST_RV_REF_BEG
					multiset<value_type, C2, A2>
				BOOST_RV_REF_END right, 
				const allocator_type& a,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							typename multiset<value_type, C2, A2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename multiset<value_type, C2, A2>::base_type&>(right))),
			a)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multiset(const multiset<value_type, C2, A2>& right,
				const allocator_type& a,
				typename
					boost::enable_if
					<
						is_isomeric_same_options_multiset
						<
							typename multiset<value_type, C2, A2>::base_type,
							base_type
						>,
						mplex::sfinae_type
					>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

	~multiset(void)
	{
	}
	
public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{  
		base_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::operator=(boost::move(right));
		return *this;
	}

	this_type& operator=(const base_type& right)
	{ 
		if(this == &right)
		{
			return *this;
		}
		
		base_type::operator=(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{  
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::operator=(boost::move(static_cast<base_type&>(right_ref)));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{ 
		if(this == &right)
		{
			return *this;
		}
		
		base_type::operator=(right);
		return *this;
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	inline this_type& operator=(std::initializer_list<value_type> il)
	{
		base_type::assign(il.begin(), il.end());
		return *this;
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				boost::container::multiset<value_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::multiset<value_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{  
		typedef boost::container::multiset<value_type, C2, A2, O2> right_type;
		
		right_type& right_ref = right;
		base_type::operator=(boost::move(reinterpret_cast<base_type&>(right_ref)));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				boost::container::multiset<value_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::multiset<value_type, C2, A2, O2>& right)
	{ 
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				typename multiset<value_type, C2, A2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						multiset<value_type, C2, A2>
					BOOST_RV_REF_END right)
	{
		typedef multiset<value_type, C2, A2> right_type;
		typedef typename right_type::base_type right_base_type;

		right_type& right_ref = right;
		base_type::operator=(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<right_base_type&>(right_ref))));
		return *this;
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				typename multiset<value_type, C2, A2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(const multiset<value_type, C2, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

public:
	using base_type::swap;

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		base_type::swap(right);
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				boost::container::multiset<value_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::multiset<value_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::multiset<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				boost::container::multiset<value_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::multiset<value_type, C2, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				typename multiset<value_type, C2, A2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					multiset<value_type, C2, A2>
				BOOST_RV_REF_END right)
	{
		typedef multiset<value_type, C2, A2> right_type;
		typedef typename right_type::base_type right_base_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_base_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multiset
			<
				typename multiset<value_type, C2, A2>::base_type,
				base_type
			>,
			void
		>::type
		swap(multiset<value_type, C2, A2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

public:
	using base_type::get_allocator;
	using base_type::get_stored_allocator;

	using base_type::begin;
	using base_type::cbegin;

	using base_type::end;
	using base_type::cend;

	using base_type::rbegin; 
	using base_type::crbegin;

	using base_type::rend;
	using base_type::crend;

	using base_type::empty;
	
	using base_type::size;
	using base_type::max_size;
	
	using base_type::emplace;
	using base_type::emplace_hint;

	// insert
	//using base_type::insert;
	inline iterator insert(BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(boost::move(val));
	}

	inline iterator insert(const value_type& val)
	{
		return base_type::insert(val);
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(hint, boost::move(val));
	}

	inline iterator insert(const_iterator hint, const value_type& val)
	{
		return base_type::insert(hint, val);
	}
	
	template<typename Iter> inline 
	void insert(Iter s, Iter e)
	{
		return base_type::insert(s, e);
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline void basic_string_assign(std::initializer_list<value_type> il)
	{
		return base_type::insert(il);
	}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	inline iterator insert(BOOST_RV_REF(node_type) nh)
	{
		return 
			detail::multiset_node_insert(
				static_cast<base_type&>(*this), boost::move(nh));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{
		return 
			detail::multiset_node_insert(
				static_cast<base_type&>(*this), hint, boost::move(nh));
	}

	using base_type::erase;
	
	using base_type::clear;

	using base_type::key_comp;
	using base_type::value_comp;

	using base_type::find;

	using base_type::count;

	using base_type::lower_bound;
	using base_type::upper_bound;
	
	using base_type::equal_range;

	using base_type::rebalance;

#	if (BOOST_VERSION < 106200)
	template<typename Key> inline
	node_type extract(BOOST_RV_REF(Key) key)
	{
		return 
			detail::multiset_node_extract(
				static_cast<base_type&>(*this), boost::move(key));
	}

	inline node_type extract(const key_type& key)
	{
		return 
			detail::multiset_node_extract(
				static_cast<base_type&>(*this), key);
	}

	inline node_type extract(const_iterator iter)
	{
		return 
			detail::multiset_node_extract(
				static_cast<base_type&>(*this), iter);
	}
#	else
	using base_type::extract;
#	endif // (BOOST_VERSION < 106200)

	// merge-member
#if !(BOOST_VERSION < 106200)
	using base_type::merge;
#endif // !(BOOST_VERSION < 106200)

	template<template<typename _T, typename _C, typename _A> class SetOrMSet,
				typename C2, typename A2> inline
	void merge(BOOST_RV_REF_BEG SetOrMSet<value_type, C2, A2> BOOST_RV_REF_END right)
	{
		typedef SetOrMSet<value_type, C2, A2> right_type;

		right_type& right_ref = right;
		container::merge(static_cast<base_type&>(*this), right_ref);
	}

	template<template<typename _T, typename _C, typename _A> class SetOrMSet,
				typename C2, typename A2> inline
	void merge(SetOrMSet<value_type, C2, A2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
				typename C2, typename A2, typename O2> inline
	void merge(BOOST_RV_REF_BEG SetOrMSet<value_type, C2, A2, O2> BOOST_RV_REF_END right)
	{
		typedef SetOrMSet<value_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		container::merge(static_cast<base_type&>(*this), right_ref);
	}

	template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
				typename C2, typename A2, typename O2> inline
	void merge(SetOrMSet<value_type, C2, A2, O2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

#if (BOOST_VERSION < 106900)
	inline bool contains(const key_type& key) const
	{
		return base_type::find(key) != base_type::end();
	}

	template<typename K> inline
	bool contains(const K& key) const
	{
		return base_type::find(key) != base_type::end();
	}

#else
	using base_type::contains;
#endif // (BOOST_VERSION < 106900)

};

YGGR_PP_CONTAINER_COMPARER_FULL(3, multiset, inline)

YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL_MN(
	4, 3,
	::boost::container::multiset, 
	multiset, 
	is_isomeric_same_options_set,
	inline)

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, multiset)

} // namespace detail

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename T, typename C, typename A, typename ...Args> inline
typename detail::multiset<T, C, A>::iterator 
	emplace(detail::multiset<T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename C, typename A, typename ...Args> inline
typename detail::multiset<T, C, A>::iterator 
	emplace_hint(detail::multiset<T, C, A>& c, 
					typename detail::multiset<T, C, A>::const_iterator pos, 
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}


#else


#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::multiset<T, C, A>::iterator \
			emplace(detail::multiset<T, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::multiset<T, C, A>::iterator \
			emplace_hint(detail::multiset<T, C, A>& c, \
							typename detail::multiset<T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

// extract
template<typename T, typename C, typename A, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::multiset<T, C, A>::node_type
	>::type
	extract(detail::multiset<T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename T, typename C, typename A, typename KeyOrIter> inline
typename detail::multiset<T, C, A>::node_type
	extract(detail::multiset<T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// insert
template<typename T, typename C, typename A> inline
typename detail::multiset<T, C, A>::iterator
	insert(detail::multiset<T, C, A>& c, 
			BOOST_RV_REF_BEG 
				typename detail::multiset<T, C, A>::node_type 
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename T, typename C, typename A> inline
typename detail::multiset<T, C, A>::iterator
	insert(detail::multiset<T, C, A>& c, 
			typename detail::multiset<T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG 
				typename detail::multiset<T, C, A>::node_type 
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// merge
template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(detail::multiset<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2> BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2> inline
void merge(detail::multiset<T, C, A>& c, SetOrMSet<T, C2, A2>& right)
{
	c.merge(right);
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(detail::multiset<T, C, A>& c, 
			BOOST_RV_REF_BEG SetOrMSet<T, C2, A2, O2> BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(detail::multiset<T, C, A>& c, SetOrMSet<T, C2, A2, O2>& right)
{
	c.merge(right);
}

// contains
template<typename T, typename C, typename A, typename K> inline
bool contains(const detail::multiset<T, C, A>& c, const K& key)
{
	return c.contains(key);
}

// erase_if
template<typename T, typename C, typename A, typename Cmper> inline
typename detail::multiset<T, C, A>::size_type
    erase_if(detail::multiset<T, C, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{
namespace swap_support
{
	YGGR_PP_CONTAINER_SWAP(3, ::yggr::container::detail::multiset, inline)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		4, 3, 
		::boost::container::multiset, 
		::yggr::container::detail::multiset, 
		::yggr::container::is_isomeric_same_options_multiset,
		inline)

} // namespace swap_support

using swap_support::swap;

} // namespace detail
} // namespace container
} // namespace yggr

namespace std
{
	using ::yggr::container::detail::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::container::detail::swap_support::swap;
} // namespace boost

#endif  // !(BOOST_VERSION < 105600)

namespace yggr
{
namespace container
{

#if defined YGGR_USE_BOOST_CONTAINER
#	if BOOST_VERSION < 105600
		using boost::container::set;
		using boost::container::multiset;
#	else
		using detail::set;
		using detail::multiset;
#	endif // BOOST_VERSION < 105600
#elif defined YGGR_USE_STL_CONTAINER
	using std::set;
	using std::multiset;
#else
#	error "use set type unknown error. please check source"
#endif //YGGR_USE_BOOST_CONTAINER

} // namespace container

using container::set;
using container::multiset;

} // namespace yggr

#endif // __YGGR_CONTAINER_SET_HPP__
