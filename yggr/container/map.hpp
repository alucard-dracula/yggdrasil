//map.hpp

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

#ifndef __YGGR_CONTAINER_MAP_HPP__
#define __YGGR_CONTAINER_MAP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container/container_selector.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
//#include <yggr/utility/pair_compare.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/typename_expand.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/get_map_options.hpp>
#include <yggr/container/basic_function_adapter.hpp>
#include <yggr/container/node_handle_traits.hpp>

#include <yggr/container/is_isomeric_same_map.hpp>
#if !(BOOST_VERSION < 105600)
#	include <yggr/container/is_isomeric_same_options_map.hpp>
#endif // !(BOOST_VERSION < 105600)

#include <yggr/container/detail/is_xxx_container.hpp>
#include <yggr/container/detail/is_s_or_m_container.hpp>
#include <yggr/container/detail/call_erase_helper.hpp>
#include <yggr/container/detail/swap_def.hpp>
#include <yggr/container/detail/comparer_def.hpp>
#include <yggr/container/detail/range_size_def.hpp>

#include <yggr/container/detail/has_at.hpp>
#include <yggr/container/detail/has_emplace.hpp>
#include <yggr/container/detail/has_merge.hpp>
#include <yggr/container/detail/has_contains.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_map.hpp>
#include <yggr/container/detail/try_emplace_impl.hpp>
#include <yggr/container/detail/insert_or_assign_impl.hpp>

#include <yggr/container/detail/erase_impl.hpp>
#include <yggr/container/detail/map_merge_impl.hpp>
#include <yggr/container/detail/map_merger_sel.hpp>

#include <boost/container/map.hpp>
#include <boost/utility/enable_if.hpp>

#include <map>
#include <stdexcept>


YGGR_PP_IS_XXX_CONTAINER_DEF(4, std_map, std::map);
YGGR_PP_IS_XXX_CONTAINER_DEF(4, std_multimap, std::multimap);

#if BOOST_VERSION < 105600

	YGGR_PP_IS_XXX_CONTAINER_DEF(4, boost_map, boost::container::map);
	YGGR_PP_IS_XXX_CONTAINER_DEF(4, boost_multimap, boost::container::multimap);

#else

	YGGR_PP_IS_XXX_CONTAINER_DEF(5, boost_map, boost::container::map);
	YGGR_PP_IS_XXX_CONTAINER_DEF(5, boost_multimap, boost::container::multimap);

#endif // BOOST_VERSION < 105600

// compatibility caller
// -----------------map---------------
namespace yggr
{
namespace container
{

template<typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_at< std::map<K, T, C, A> >,
		T&
	>::type
	at(std::map<K, T, C, A>& c, const K& k)
{
	return c.at(k);
}

template<typename K, typename T, typename C, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_at< std::map<K, T, C, A> >,
		T&
	>::type
	at(std::map<K, T, C, A>& c, const K& k)
{
	typedef typename std::map<K, T, C, A>::iterator iter_type;
	iter_type i = c.find(k);
	if(i == c.end())
	{
		throw std::out_of_range("key not found");
	}
	return i->second;
}

template<typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_at< std::map<K, T, C, A> >,
		const T&
	>::type
	at(const std::map<K, T, C, A>& c, const K& k)
{
	return c.at(k);
}

template<typename K, typename T, typename C, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_at< std::map<K, T, C, A> >,
		const T&
	>::type
	at(const std::map<K, T, C, A>& c, const K& k)
{
	typedef typename std::map<K, T, C, A>::const_iterator iter_type;
	iter_type i = c.find(k);
	if(i == c.end())
	{
		throw std::out_of_range("key not found");
	}
	return i->second;
}

template<typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_at< boost::container::map<K, T, C, A> >,
		T&
	>::type
	at(boost::container::map<K, T, C, A>& c, const K& k)
{
	return c.at(k);
}

template<typename K, typename T, typename C, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_at< boost::container::map<K, T, C, A> >,
		T&
	>::type
	at(boost::container::map<K, T, C, A>& c, const K& k)
{
	typedef typename boost::container::map<K, T, C, A>::iterator iter_type;
	iter_type i = c.find(k);
	if(i == c.end())
	{
		throw std::out_of_range("key not found");
	}
	return i->second;
}

template<typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_at< boost::container::map<K, T, C, A> >,
		const T&
	>::type
	at(const boost::container::map<K, T, C, A>& c, const K& k)
{
	return c.at(k);
}

template<typename K, typename T, typename C, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_at< boost::container::map<K, T, C, A> >,
		const T&
	>::type
	at(const boost::container::map<K, T, C, A>& c, const K& k)
{
	typedef typename boost::container::map<K, T, C, A>::const_iterator iter_type;
	iter_type i = c.find(k);
	if(i == c.end())
	{
		throw std::out_of_range("key not found");
	}
	return i->second;
}

//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< std::map<K, T, C, A> >,
		std::pair< typename std::map<K, T, C, A>::iterator, bool>
	>::type
	emplace(std::map<K, T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< std::map<K, T, C, A> >,
		std::pair< typename std::map<K, T, C, A>::iterator, bool>
	>::type
	emplace(std::map<K, T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	typedef typename std::map<K, T, C, A>::value_type value_type;
	return c.insert(value_type(boost::forward<Args>(args)...));
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_hint< std::map<K, T, C, A> >,
		typename std::map<K, T, C, A>::iterator
	>::type
	emplace_hint(std::map<K, T, C, A>& c,
					typename std::map<K, T, C, A>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_hint< std::map<K, T, C, A> >,
		typename std::map<K, T, C, A>::iterator
	>::type
	emplace_hint(std::map<K, T, C, A>& c,
					typename std::map<K, T, C, A>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	typedef typename std::map<K, T, C, A>::value_type value_type;
	typedef typename std::map<K, T, C, A>::iterator iterator_type;
	value_type val = value_type(boost::forward<Args>(args)...);
	iterator_type iter = c.find(val.first);
	return iter == c.end()? c.insert(val).first : iter;
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair< typename std::map<K, T, C, A>::iterator, bool> \
			emplace(std::map<K, T, C, A>& c \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename std::map<K, T, C, A>::value_type value_type; \
			return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::map<K, T, C, A>::iterator \
			emplace_hint(std::map<K, T, C, A>& c, \
							typename std::map<K, T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename std::map<K, T, C, A>::value_type value_type; \
			typedef typename std::map<K, T, C, A>::iterator iterator_type; \
			value_type val = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			iterator_type iter = c.find(val.first); \
			return iter == c.end()? c.insert(val).first : iter; }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF
#   undef YGGR_TMP_PP_EMPLACE_DEF_NO_HINT
#   undef YGGR_TMP_PP_EMPLACE_DEF_HAS_HINT

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// extract
template<typename K, typename T, typename C, typename A, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			detail::native_extractable< std::map<K, T, C, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename std::map<K, T, C, A>::node_type
	>::type
	extract(std::map<K, T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename K, typename T, typename C, typename A, typename KeyOrIter> inline
typename
	boost::enable_if
	<
		detail::native_extractable< std::map<K, T, C, A> >,
		typename std::map<K, T, C, A>::node_type
	>::type
	extract(std::map<K, T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// insert_node_type
template<typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable< std::map<K, T, C, A> >,
		typename std::map<K, T, C, A>::insert_return_type
	>::type
	insert(std::map<K, T, C, A>& c,
			BOOST_RV_REF_BEG
				typename std::map<K, T, C, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable< std::map<K, T, C, A> >,
		typename std::map<K, T, C, A>::iterator
	>::type
	insert(std::map<K, T, C, A>& c,
			typename std::map<K, T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename std::map<K, T, C, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// merge

namespace detail
{

template<typename K, typename T, typename C, typename A, typename K2, typename T2, typename C2, typename A2>
struct map_merger_sel_rule_is_same_namespace<std::map<K, T, C, A>, std::multimap<K2, T2, C2, A2> >
	: public
		map_merger_sel_rule_is_different_comp_only
		<
			std::map<K, T, C, A>,
			std::multimap<K2, T2, C2, A2>
		>
{
};

} // namespace detail

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(std::map<K, T, C, A>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2> BOOST_RV_REF_END right)
{
	typedef std::map<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(std::map<K, T, C, A>& c, MapOrMMap<K, T, C2, A2>& right)
{
	typedef std::map<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(std::map<K, T, C, A>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef std::map<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(std::map<K, T, C, A>& c, MapOrMMap<K, T, C2, A2, O2>& right)
{
	typedef std::map<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}


// contains
template<typename K, typename T, typename C, typename A, typename Key> inline
typename boost::enable_if<detail::has_any_contains< std::map<K, T, C, A> >, bool>::type
	contains(const std::map<K, T, C, A>& c, const Key& key)
{
	return c.contains(key);
}

template<typename K, typename T, typename C, typename A, typename Key> inline
typename boost::disable_if<detail::has_any_contains< std::map<K, T, C, A> >, bool>::type
	contains(const std::map<K, T, C, A>& c, const Key& key)
{
	return c.find(key) != c.end();
}

// try_emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename K, typename T, typename C, typename A, typename ...Args> inline
std::pair<typename std::map<K, T, C, A>::iterator, bool>
	try_emplace(std::map<K, T, C, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
{
	return detail::try_emplace_impl(c, boost::move(key), boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
std::pair<typename std::map<K, T, C, A>::iterator, bool>
	try_emplace(std::map<K, T, C, A>& c, const K& key, BOOST_FWD_REF(Args)... args)
{
	return detail::try_emplace_impl(c, key, boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename std::map<K, T, C, A>::iterator
	try_emplace(std::map<K, T, C, A>& c,
				typename std::map<K, T, C, A>::const_iterator hint,
				BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
{
	return detail::try_emplace_impl(c, hint, boost::move(key), boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename std::map<K, T, C, A>::iterator
	try_emplace(std::map<K, T, C, A>& c,
				typename std::map<K, T, C, A>::const_iterator hint,
				const K& key, BOOST_FWD_REF(Args)... args)
{
	return detail::try_emplace_impl(c, hint, key, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename std::map<K, T, C, A>::iterator, bool> \
			try_emplace(std::map<K, T, C, A>& c, BOOST_RV_REF(K) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				detail::try_emplace_impl( \
					c, boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename std::map<K, T, C, A>::iterator, bool> \
			try_emplace(std::map<K, T, C, A>& c, const K& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				detail::try_emplace_impl( \
					c, key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::map<K, T, C, A>::iterator \
			try_emplace(std::map<K, T, C, A>& c, \
						typename std::map<K, T, C, A>::const_iterator hint, \
						BOOST_RV_REF(K) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				detail::try_emplace_impl( \
					c, hint, boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::map<K, T, C, A>::iterator \
			try_emplace(std::map<K, T, C, A>& c, \
						typename std::map<K, T, C, A>::const_iterator hint, \
						const K& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				detail::try_emplace_impl( \
					c, hint, key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// insert_or_assign
template<typename K, typename T, typename C, typename A, typename M> inline
std::pair<typename std::map<K, T, C, A>::iterator, bool>
	insert_or_assign(std::map<K, T, C, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename M> inline
std::pair<typename std::map<K, T, C, A>::iterator, bool>
	insert_or_assign(std::map<K, T, C, A>& c, const K& key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, key, boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename M> inline
typename std::map<K, T, C, A>::iterator
	insert_or_assign(std::map<K, T, C, A>& c,
				typename std::map<K, T, C, A>::const_iterator hint,
				BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, hint, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename M> inline
typename std::map<K, T, C, A>::iterator
	insert_or_assign(std::map<K, T, C, A>& c,
				typename std::map<K, T, C, A>::const_iterator hint,
				const K& key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, hint, key, boost::forward<M>(m));
}

// erase_if
template<typename K, typename T, typename C, typename A, typename Cmper> inline
typename std::map<K, T, C, A>::size_type
    erase_if(std::map<K, T, C, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#	if (BOOST_VERSION < 105600)

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::enable_if
		<
			detail::has_any_emplace< boost::container::map<K, T, C, A> >,
			std::pair< typename boost::container::map<K, T, C, A>::iterator, bool>
		>::type
		emplace(boost::container::map<K, T, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::disable_if
		<
			detail::has_any_emplace< boost::container::map<K, T, C, A> >,
			std::pair< typename boost::container::map<K, T, C, A>::iterator, bool>
		>::type
		emplace(boost::container::map<K, T, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		typedef typename boost::container::map<K, T, C, A>::value_type value_type;
		return c.insert(value_type(boost::forward<Args>(args)...));
	}

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::enable_if
		<
			detail::has_any_emplace_hint< boost::container::map<K, T, C, A> >,
			typename boost::container::map<K, T, C, A>::iterator
		>::type
		emplace_hint(boost::container::map<K, T, C, A>& c,
						typename boost::container::map<K, T, C, A>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::disable_if
		<
			detail::has_any_emplace_hint< boost::container::map<K, T, C, A> >,
			typename boost::container::map<K, T, C, A>::iterator
		>::type
		emplace_hint(boost::container::map<K, T, C, A>& c,
						typename boost::container::map<K, T, C, A>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		typedef typename boost::container::map<K, T, C, A>::value_type value_type;
		return c.insert(pos, value_type(boost::forward<Args>(args)...));
	}

#	else

	template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
	std::pair<typename boost::container::map<K, T, C, A, O>::iterator, bool>
		emplace(boost::container::map<K, T, C, A, O>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
	typename boost::container::map<K, T, C, A, O>::iterator
		emplace_hint(boost::container::map<K, T, C, A, O>& c,
						typename boost::container::map<K, T, C, A, O>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#	endif // BOOST_VERSION < 105600

#else

#	if BOOST_VERSION < 105600

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair< typename boost::container::map<K, T, C, A>::iterator, bool> \
			emplace(boost::container::map<K, T, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename boost::container::map<K, T, C, A>::value_type value_type; \
			return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::container::map<K, T, C, A>::iterator \
			emplace_hint(boost::container::map<K, T, C, A>& c, \
							typename boost::container::map<K, T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename boost::container::map<K, T, C, A>::value_type value_type; \
			return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#	else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename boost::container::map<K, T, C, A, O>::iterator, bool> \
			emplace(boost::container::map<K, T, C, A, O>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::container::map<K, T, C, A, O>::iterator \
			emplace_hint(boost::container::map<K, T, C, A, O>& c, \
							typename boost::container::map<K, T, C, A, O>::const_iterator pos \
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
template<typename K, typename T, typename C, typename A, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::node_handle_traits<boost::container::map<K, T, C, A> >::node_type
	>::type
	extract(boost::container::map<K, T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return detail::map_node_extract(c, boost::move(key));
}

template<typename K, typename T, typename C, typename A, typename KeyOrIter> inline
typename detail::node_handle_traits<boost::container::map<K, T, C, A> >::node_type
	extract(boost::container::map<K, T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return detail::map_node_extract(c, key_or_iter);
}

// insert node
template<typename K, typename T, typename C, typename A> inline
typename detail::node_handle_traits<boost::container::map<K, T, C, A> >::insert_return_type
	insert(boost::container::map<K, T, C, A>& c,
			BOOST_RV_REF_BEG
				typename detail::node_handle_traits<boost::container::map<K, T, C, A> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::map_node_insert(c, boost::move(node));
}

template<typename K, typename T, typename C, typename A> inline
typename detail::node_handle_traits< boost::container::map<K, T, C, A> >::hint_insert_return_type
	insert(boost::container::map<K, T, C, A>& c,
			typename boost::container::map<K, T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename detail::node_handle_traits<boost::container::map<K, T, C, A> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::map_node_insert(c, hint, boost::move(node));
}

//merge

namespace detail
{

/*
why not use has_extract to judge whether to use chopsticks or not,
because global extract function is hacing into boost::container,
so has_extract not sure if you have extract.
*/

template<typename K, typename T, typename C, typename A, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::map<K, T, C, A>,
													boost::container::map<K, T, C2, A> >
	: public map_merger_eti
{
};

template<typename K, typename T, typename C, typename A, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::map<K, T, C, A>,
													boost::container::multimap<K, T, C2, A> >
	: public map_merger_eti
{
};

template<typename K, typename T, typename C, typename A, typename K2, typename T2, typename C2, typename A2>
struct map_merger_sel_rule_is_same_namespace<boost::container::map<K, T, C, A>,
												boost::container::multimap<K2, T2, C2, A2> >
	: public
		map_merger_sel_rule_is_different_comp_only
		<
			boost::container::map<K, T, C, A>,
			boost::container::multimap<K2, T2, C2, A2>
		>
{
};

} // namespace detail

template< template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(boost::container::map<K, T, C, A>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2> BOOST_RV_REF_END right)
{
	typedef boost::container::map<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template< template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(boost::container::map<K, T, C, A>& c, MapOrMMap<K, T, C2, A2>& right)
{
	typedef boost::container::map<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template< template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(boost::container::map<K, T, C, A>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef boost::container::map<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template< template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(boost::container::map<K, T, C, A>& c, MapOrMMap<K, T, C2, A2, O2>& right)
{
	typedef boost::container::map<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename K, typename T, typename C, typename A, typename Key> inline
typename boost::enable_if<detail::has_any_contains< boost::container::map<K, T, C, A> >, bool>::type
	contains(const boost::container::map<K, T, C, A>& c, const Key& key)
{
	return c.contains(key);
}

template<typename K, typename T, typename C, typename A, typename Key> inline
typename boost::disable_if<detail::has_any_contains< boost::container::map<K, T, C, A> >, bool>::type
	contains(const boost::container::map<K, T, C, A>& c, const Key& key)
{
	return c.find(key) != c.end();
}

// try_emplace
#	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	std::pair<typename boost::container::map<K, T, C, A>::iterator, bool>
		try_emplace(boost::container::map<K, T, C, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
	{
		return detail::try_emplace_impl(c, boost::move(key), boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	std::pair<typename boost::container::map<K, T, C, A>::iterator, bool>
		try_emplace(boost::container::map<K, T, C, A>& c, const K& key, BOOST_FWD_REF(Args)... args)
	{
		return detail::try_emplace_impl(c, key, boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename boost::container::map<K, T, C, A>::iterator
		try_emplace(boost::container::map<K, T, C, A>& c,
					typename boost::container::map<K, T, C, A>::const_iterator hint,
					BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
	{
		return detail::try_emplace_impl(c, hint, boost::move(key), boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename boost::container::map<K, T, C, A>::iterator
		try_emplace(boost::container::map<K, T, C, A>& c,
					typename boost::container::map<K, T, C, A>::const_iterator hint,
					const K& key, BOOST_FWD_REF(Args)... args)
	{
		return detail::try_emplace_impl(c, hint, key, boost::forward<Args>(args)...);
	}

#	else

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			template<typename K, typename T, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::container::map<K, T, C, A>::iterator, bool> \
				try_emplace(boost::container::map<K, T, C, A>& c, BOOST_RV_REF(K) key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						c, boost::move(key) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename K, typename T, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::container::map<K, T, C, A>::iterator, bool> \
				try_emplace(boost::container::map<K, T, C, A>& c, const K& key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						c, key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename K, typename T, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::container::map<K, T, C, A>::iterator \
				try_emplace(boost::container::map<K, T, C, A>& c, \
							typename boost::container::map<K, T, C, A>::const_iterator hint, \
							BOOST_RV_REF(K) key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						c, hint, boost::move(key) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename K, typename T, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::container::map<K, T, C, A>::iterator \
				try_emplace(boost::container::map<K, T, C, A>& c, \
							typename boost::container::map<K, T, C, A>::const_iterator hint, \
							const K& key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						c, hint, key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }


#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#		define YGGR_PP_FOO_ARG_NAME() init_arg

#		define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#		include BOOST_PP_LOCAL_ITERATE()

#		undef YGGR_PP_FOO_ARG_NAME
#		undef YGGR_TMP_PP_EMPLACE_DEF

#	endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// insert_or_assign
template<typename K, typename T, typename C, typename A, typename M> inline
std::pair<typename boost::container::map<K, T, C, A>::iterator, bool>
	insert_or_assign(boost::container::map<K, T, C, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename M> inline
std::pair<typename boost::container::map<K, T, C, A>::iterator, bool>
	insert_or_assign(boost::container::map<K, T, C, A>& c, const K& key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, key, boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename M> inline
typename boost::container::map<K, T, C, A>::iterator
	insert_or_assign(boost::container::map<K, T, C, A>& c,
					typename boost::container::map<K, T, C, A>::const_iterator hint,
					BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, hint, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename M> inline
typename boost::container::map<K, T, C, A>::iterator
	insert_or_assign(boost::container::map<K, T, C, A>& c,
						typename boost::container::map<K, T, C, A>::const_iterator hint,
						const K& key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, hint, key, boost::forward<M>(m));
}

// erase_if
template<typename K, typename T, typename C, typename A, typename Cmper> inline
typename boost::container::map<K, T, C, A>::size_type
    erase_if(boost::container::map<K, T, C, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#else

// node_handle_traits using default

// extract
template<typename K, typename T, typename C, typename A, typename MO, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::node_handle_traits< boost::container::map<K, T, C, A, MO> >::node_type
	>::type
	extract(boost::container::map<K, T, C, A, MO>& c, BOOST_RV_REF(Key) key)
{
	return detail::map_node_extract(c, boost::move(key));
}

template<typename K, typename T, typename C, typename A, typename MO, typename KeyOrIter> inline
typename detail::node_handle_traits< boost::container::map<K, T, C, A, MO> >::node_type
	extract(boost::container::map<K, T, C, A, MO>& c, const KeyOrIter& key_or_iter)
{
	return detail::map_node_extract(c, key_or_iter);
}

// insert
template<typename K, typename T, typename C, typename A, typename MO> inline
typename detail::node_handle_traits< boost::container::map<K, T, C, A, MO> >::insert_return_type
	insert(boost::container::map<K, T, C, A, MO>& c,
			BOOST_RV_REF_BEG
				typename detail::node_handle_traits< boost::container::map<K, T, C, A, MO> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::map_node_insert(c, boost::move(node));
}


template<typename K, typename T, typename C, typename A, typename MO> inline
typename detail::node_handle_traits< boost::container::map<K, T, C, A, MO> >::hint_insert_return_type
	insert(boost::container::map<K, T, C, A, MO>& c,
			typename boost::container::map<K, T, C, A, MO>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename detail::node_handle_traits< boost::container::map<K, T, C, A, MO> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::map_node_insert(c, hint, boost::move(node));
}

//merge

namespace detail
{

#if (BOOST_VERSION < 106200)

template<typename K, typename T, typename C, typename A, typename O, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::map<K, T, C, A, O>,
													boost::container::map<K, T, C2, A, O> >
	: public map_merger_eti
{
};

template<typename K, typename T, typename C, typename A, typename O, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::map<K, T, C, A, O>,
													boost::container::multimap<K, T, C2, A, O> >
	: public map_merger_eti
{
};

#else

template<typename K, typename T, typename C, typename A, typename O, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::map<K, T, C, A, O>,
													boost::container::map<K, T, C2, A, O> >
	: public map_merger_inner
{
};

template<typename K, typename T, typename C, typename A, typename O, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::map<K, T, C, A, O>,
													boost::container::multimap<K, T, C2, A, O> >
	: public map_merger_inner
{
};


#endif // (BOOST_VERSION < 106200)

template<typename K, typename T, typename C, typename A, typename O,
			typename K2, typename T2, typename C2, typename A2, typename O2>
struct map_merger_sel_rule_is_same_namespace<boost::container::map<K, T, C, A, O>,
												boost::container::multimap<K2, T2, C2, A2, O2> >
	: public
		map_merger_sel_rule_is_different_comp_only
		<
			boost::container::map<K, T, C, A, O>,
			boost::container::multimap<K2, T2, C2, A2, O2>
		>
{
};

} // namespace detail

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O, typename C2, typename A2> inline
void merge(boost::container::map<K, T, C, A, O>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2> BOOST_RV_REF_END right)
{
	typedef boost::container::map<K, T, C, A, O> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O, typename C2, typename A2> inline
void merge(boost::container::map<K, T, C, A, O>& c, MapOrMMap<K, T, C2, A2>& right)
{
	typedef boost::container::map<K, T, C, A, O> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O, typename C2, typename A2, typename O2> inline
void merge(boost::container::map<K, T, C, A, O>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef boost::container::map<K, T, C, A, O> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O, typename C2, typename A2, typename O2> inline
void merge(boost::container::map<K, T, C, A, O>& c, MapOrMMap<K, T, C2, A2, O2>& right)
{
	typedef boost::container::map<K, T, C, A, O> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename K, typename T, typename C, typename A, typename MO, typename Key> inline
typename boost::enable_if<detail::has_any_contains< boost::container::map<K, T, C, A, MO> >, bool>::type
	contains(const boost::container::map<K, T, C, A, MO>& c, const Key& key)
{
	return c.contains(key);
}

template<typename K, typename T, typename C, typename A, typename MO, typename Key> inline
typename boost::disable_if<detail::has_any_contains< boost::container::map<K, T, C, A, MO> >, bool>::type
	contains(const boost::container::map<K, T, C, A, MO>& c, const Key& key)
{
	return c.find(key) != c.end();
}

// try_emplace
#	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename K, typename T, typename C, typename A, typename MO, typename ...Args> inline
	std::pair<typename boost::container::map<K, T, C, A, MO>::iterator, bool>
		try_emplace(boost::container::map<K, T, C, A, MO>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
	{
		return detail::try_emplace_impl(c, boost::move(key), boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename MO, typename ...Args> inline
	std::pair<typename boost::container::map<K, T, C, A, MO>::iterator, bool>
		try_emplace(boost::container::map<K, T, C, A, MO>& c, const K& key, BOOST_FWD_REF(Args)... args)
	{
		return detail::try_emplace_impl(c, key, boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename MO, typename ...Args> inline
	typename boost::container::map<K, T, C, A, MO>::iterator
		try_emplace(boost::container::map<K, T, C, A, MO>& c,
					typename boost::container::map<K, T, C, A, MO>::const_iterator hint,
					BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
	{
		return detail::try_emplace_impl(c, hint, boost::move(key), boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename MO, typename ...Args> inline
	typename boost::container::map<K, T, C, A, MO>::iterator
		try_emplace(boost::container::map<K, T, C, A, MO>& c,
					typename boost::container::map<K, T, C, A, MO>::const_iterator hint,
					const K& key, BOOST_FWD_REF(Args)... args)
	{
		return detail::try_emplace_impl(c, hint, key, boost::forward<Args>(args)...);
	}

#	else

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			template<typename K, typename T, typename C, typename A, typename MO \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::container::map<K, T, C, A, MO>::iterator, bool> \
				try_emplace(boost::container::map<K, T, C, A, MO>& c, BOOST_RV_REF(K) key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						c, boost::move(key) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename K, typename T, typename C, typename A, typename MO \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::container::map<K, T, C, A, MO>::iterator, bool> \
				try_emplace(boost::container::map<K, T, C, A, MO>& c, const K& key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						c, key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename K, typename T, typename C, typename A, typename MO \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::container::map<K, T, C, A, MO>::iterator \
				try_emplace(boost::container::map<K, T, C, A, MO>& c, \
							typename boost::container::map<K, T, C, A, MO>::const_iterator hint, \
							BOOST_RV_REF(K) key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						c, hint, boost::move(key) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename K, typename T, typename C, typename A, typename MO \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::container::map<K, T, C, A, MO>::iterator \
				try_emplace(boost::container::map<K, T, C, A, MO>& c, \
							typename boost::container::map<K, T, C, A, MO>::const_iterator hint, \
							const K& key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						c, hint, key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#		define YGGR_PP_FOO_ARG_NAME() init_arg

#		define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#		include BOOST_PP_LOCAL_ITERATE()

#		undef YGGR_PP_FOO_ARG_NAME
#		undef YGGR_TMP_PP_EMPLACE_DEF

#	endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// insert_or_assign

template<typename K, typename T, typename C, typename A, typename MO, typename M> inline
std::pair<typename boost::container::map<K, T, C, A, MO>::iterator, bool>
	insert_or_assign(boost::container::map<K, T, C, A, MO>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename MO, typename M> inline
std::pair<typename boost::container::map<K, T, C, A, MO>::iterator, bool>
	insert_or_assign(boost::container::map<K, T, C, A, MO>& c, const K& key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, key, boost::forward<M>(m));
}

#	if ((106100 < BOOST_VERSION) && (BOOST_VERSION < 107100) && !defined(YGGR_FIXBUG_BOOST_162_TO_170_MAP_INSERT_OR_ASSIGN_HINT))

YGGR_PRAGMA_MESSAGE("!!!please using boost code patch fix this boost::map insert_or_assign bug!!!")

template<typename K, typename T, typename C, typename A, typename MO, typename M> inline
typename boost::container::map<K, T, C, A, MO>::iterator
	insert_or_assign(boost::container::map<K, T, C, A, MO>& c,
						typename boost::container::map<K, T, C, A, MO>::const_iterator hint,
						BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::non_has_insert_or_assign_impl(c, hint, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename MO, typename M> inline
typename boost::container::map<K, T, C, A, MO>::iterator
	insert_or_assign(boost::container::map<K, T, C, A, MO>& c,
						typename boost::container::map<K, T, C, A, MO>::const_iterator hint,
						const K& key, BOOST_FWD_REF(M) m)
{
	return detail::non_has_insert_or_assign_impl(c, hint, key, boost::forward<M>(m));
}

#	else

template<typename K, typename T, typename C, typename A, typename MO, typename M> inline
typename boost::container::map<K, T, C, A, MO>::iterator
	insert_or_assign(boost::container::map<K, T, C, A, MO>& c,
						typename boost::container::map<K, T, C, A, MO>::const_iterator hint,
						BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, hint, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename MO, typename M> inline
typename boost::container::map<K, T, C, A, MO>::iterator
	insert_or_assign(boost::container::map<K, T, C, A, MO>& c,
						typename boost::container::map<K, T, C, A, MO>::const_iterator hint,
						const K& key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, hint, key, boost::forward<M>(m));
}
#endif // if((106100 < BOOST_VERSION) && (BOOST_VERSION < 107100) && !defined(YGGR_FIXBUG_BOOST_162_TO_170_MAP_INSERT_OR_ASSIGN_HINT))

// erase_if
template<typename K, typename T, typename C, typename A, typename MO, typename Cmper> inline
typename boost::container::map<K, T, C, A, MO>::size_type
    erase_if(boost::container::map<K, T, C, A, MO>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#endif // (BOOST_VERSION < 105600)

} // namespace container
} // namespace yggr

// -----------------multimap---------------
namespace yggr
{
namespace container
{

//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< std::multimap<K, T, C, A> >,
		typename std::multimap<K, T, C, A>::iterator
	>::type
	emplace(std::multimap<K, T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< std::multimap<K, T, C, A> >,
		typename std::multimap<K, T, C, A>::iterator
	>::type
	emplace(std::multimap<K, T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	typedef typename std::multimap<K, T, C, A>::value_type value_type;
	return c.insert(value_type(boost::forward<Args>(args)...));
}


template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_hint< std::multimap<K, T, C, A> >,
		typename std::multimap<K, T, C, A>::iterator
	>::type
	emplace_hint(std::multimap<K, T, C, A>& c,
					typename std::multimap<K, T, C, A>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_hint< std::multimap<K, T, C, A> >,
		typename std::multimap<K, T, C, A>::iterator
	>::type
	emplace_hint(std::multimap<K, T, C, A>& c,
					typename std::multimap<K, T, C, A>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	typedef typename std::multimap<K, T, C, A>::value_type value_type;
	return c.insert(pos, value_type(boost::forward<Args>(args)...));
}

#else

//#   define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
//        template<typename K, typename T, typename C, typename A \
//                    YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
//                    YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
//        typename std::multimap<K, T, C, A>::iterator \
//            emplace(std::multimap<K, T, C, A>& c \
//                        YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
//                        YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
//            typedef typename std::multimap<K, T, C, A>::value_type value_type; \
//            return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
//        \
//        template<typename K, typename T, typename C, typename A \
//                    YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
//                    YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
//        typename std::multimap<K, T, C, A>::iterator \
//            emplace_hint(std::multimap<K, T, C, A>& c, \
//                            typename std::multimap<K, T, C, A>::const_iterator pos \
//                            YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
//                            YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
//            typedef typename std::multimap<K, T, C, A>::value_type value_type; \
//            return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

//  !!!emplace_hint using typename HitIter, can Compatible cpp03 older and cpp11 later !!!
#   define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
        template<typename K, typename T, typename C, typename A \
                    YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
                    YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
        typename std::multimap<K, T, C, A>::iterator \
            emplace(std::multimap<K, T, C, A>& c \
                        YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
                        YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
            typedef typename std::multimap<K, T, C, A>::value_type value_type; \
            return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
        \
        template<typename K, typename T, typename C, typename A, typename HitIter \
                    YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
                    YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
        typename std::multimap<K, T, C, A>::iterator \
            emplace_hint(std::multimap<K, T, C, A>& c, \
                            HitIter pos \
                            YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
                            YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
            typedef typename std::multimap<K, T, C, A>::value_type value_type; \
            return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// extract

template<typename K, typename T, typename C, typename A, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			detail::native_extractable< std::multimap<K, T, C, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename std::multimap<K, T, C, A>::node_type
	>::type
	extract(std::multimap<K, T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename K, typename T, typename C, typename A, typename KeyOrIter> inline
typename
	boost::enable_if
	<
		detail::native_extractable< std::multimap<K, T, C, A> >,
		typename std::multimap<K, T, C, A>::node_type
	>::type
	extract(std::multimap<K, T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// insert node
template<typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable< std::multimap<K, T, C, A> >,
		typename std::multimap<K, T, C, A>::iterator
	>::type
	insert(std::multimap<K, T, C, A>& c,
			BOOST_RV_REF_BEG
				typename std::multimap<K, T, C, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable< std::multimap<K, T, C, A> >,
		typename std::multimap<K, T, C, A>::iterator
	>::type
	insert(std::multimap<K, T, C, A>& c,
			typename std::multimap<K, T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename std::multimap<K, T, C, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// merge

namespace detail
{

template<typename K, typename T, typename C, typename A,
			typename K2, typename T2, typename C2, typename A2>
struct map_merger_sel_rule_is_same_namespace<std::multimap<K, T, C, A>, std::map<K2, T2, C2, A2> >
	: public
		map_merger_sel_rule_is_different_comp_only
		<
			std::multimap<K, T, C, A>,
			std::map<K2, T2, C2, A2> >
{
};

} // namespace detail

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(std::multimap<K, T, C, A>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2> BOOST_RV_REF_END right)
{
	typedef std::multimap<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(std::multimap<K, T, C, A>& c, MapOrMMap<K, T, C2, A2>& right)
{
	typedef std::multimap<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(std::multimap<K, T, C, A>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef std::multimap<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(std::multimap<K, T, C, A>& c, MapOrMMap<K, T, C2, A2, O2>& right)
{
	typedef std::multimap<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}


// contains
template<typename K, typename T, typename C, typename A, typename Key> inline
typename boost::enable_if<detail::has_any_contains< std::multimap<K, T, C, A> >, bool>::type
	contains(const std::multimap<K, T, C, A>& c, const Key& key)
{
	return c.contains(key);
}

template<typename K, typename T, typename C, typename A, typename Key> inline
typename boost::disable_if<detail::has_any_contains< std::multimap<K, T, C, A> >, bool>::type
	contains(const std::multimap<K, T, C, A>& c, const Key& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename K, typename T, typename C, typename A, typename Cmper> inline
typename std::multimap<K, T, C, A>::size_type
    erase_if(std::multimap<K, T, C, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#	if BOOST_VERSION < 105600

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::enable_if
		<
			detail::has_any_emplace< boost::container::multimap<K, T, C, A> >,
			typename boost::container::multimap<K, T, C, A>::iterator
		>::type
		emplace(boost::container::multimap<K, T, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::disable_if
		<
			detail::has_any_emplace< boost::container::multimap<K, T, C, A> >,
			typename boost::container::multimap<K, T, C, A>::iterator
		>::type
		emplace(boost::container::multimap<K, T, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		typedef typename boost::container::multimap<K, T, C, A>::value_type value_type;
		return c.insert(value_type(boost::forward<Args>(args)...));
	}


	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::enable_if
		<
			detail::has_any_emplace_hint< boost::container::multimap<K, T, C, A> >,
			typename boost::container::multimap<K, T, C, A>::iterator
		>::type
		emplace_hint(boost::container::multimap<K, T, C, A>& c,
						typename boost::container::multimap<K, T, C, A>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename
		boost::disable_if
		<
			detail::has_any_emplace_hint< boost::container::multimap<K, T, C, A> >,
			typename boost::container::multimap<K, T, C, A>::iterator
		>::type
		emplace_hint(boost::container::multimap<K, T, C, A>& c,
						typename boost::container::multimap<K, T, C, A>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		typedef typename boost::container::multimap<K, T, C, A>::value_type value_type;
		return c.insert(pos, value_type(boost::forward<Args>(args)...));
	}

#	else

	template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
	typename boost::container::multimap<K, T, C, A, O>::iterator
		emplace(boost::container::multimap<K, T, C, A, O>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename O, typename ...Args> inline
	typename boost::container::multimap<K, T, C, A, O>::iterator
		emplace_hint(boost::container::multimap<K, T, C, A, O>& c,
						typename boost::container::multimap<K, T, C, A, O>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#	endif // BOOST_VERSION < 105600

#else

#	if BOOST_VERSION < 105600

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::container::multimap<K, T, C, A>::iterator \
			emplace(boost::container::multimap<K, T, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename boost::container::multimap<K, T, C, A>::value_type value_type; \
			return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::container::multimap<K, T, C, A>::iterator \
			emplace_hint(boost::container::multimap<K, T, C, A>& c, \
							typename boost::container::multimap<K, T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef typename boost::container::multimap<K, T, C, A>::value_type value_type; \
			return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::container::multimap<K, T, C, A, O>::iterator \
			emplace(boost::container::multimap<K, T, C, A, O>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::container::multimap<K, T, C, A, O>::iterator \
			emplace_hint(boost::container::multimap<K, T, C, A, O>& c, \
							typename boost::container::multimap<K, T, C, A, O>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#endif // BOOST_VERSION < 105600

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#if (BOOST_VERSION < 105600)


// extract
template<typename K, typename T, typename C, typename A, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::node_handle_traits<boost::container::multimap<K, T, C, A> >::node_type
	>::type
	extract(boost::container::multimap<K, T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return detail::multimap_node_extract(c, boost::move(key));
}

template<typename K, typename T, typename C, typename A, typename KeyOrIter> inline
typename detail::node_handle_traits<boost::container::multimap<K, T, C, A> >::node_type
	extract(boost::container::multimap<K, T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return detail::multimap_node_extract(c, key_or_iter);
}

// insert node
template<typename K, typename T, typename C, typename A> inline
typename boost::container::multimap<K, T, C, A>::iterator
	insert(boost::container::multimap<K, T, C, A>& c,
			BOOST_RV_REF_BEG
				typename detail::node_handle_traits<boost::container::multimap<K, T, C, A> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::multimap_node_insert(c, boost::move(node));
}

template<typename K, typename T, typename C, typename A> inline
typename boost::container::multimap<K, T, C, A>::iterator
	insert(boost::container::multimap<K, T, C, A>& c,
			typename boost::container::multimap<K, T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename detail::node_handle_traits<boost::container::multimap<K, T, C, A> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::multimap_node_insert(c, hint, boost::move(node));
}

//merge

namespace detail
{

/*
why not use has_extract to judge whether to use chopsticks or not,
because global extract function is hacing into boost::container,
so has_extract not sure if you have extract.
*/

template<typename K, typename T, typename C, typename A, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::multimap<K, T, C, A>,
													boost::container::multimap<K, T, C2, A> >
	: public map_merger_eti
{
};

template<typename K, typename T, typename C, typename A, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::multimap<K, T, C, A>,
													boost::container::map<K, T, C2, A> >
	: public map_merger_eti
{
};

template<typename K, typename T, typename C, typename A, typename K2, typename T2, typename C2, typename A2>
struct map_merger_sel_rule_is_same_namespace<boost::container::multimap<K, T, C, A>,
												boost::container::map<K2, T2, C2, A2> >
	: public
		map_merger_sel_rule_is_different_comp_only
		<
			boost::container::multimap<K, T, C, A>,
			boost::container::map<K2, T2, C2, A2>
		>
{
};

} // namespace detail

template< template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(boost::container::multimap<K, T, C, A>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2> BOOST_RV_REF_END right)
{
	typedef boost::container::multimap<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template< template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(boost::container::multimap<K, T, C, A>& c, MapOrMMap<K, T, C2, A2>& right)
{
	typedef boost::container::multimap<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template< template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(boost::container::multimap<K, T, C, A>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef boost::container::multimap<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template< template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(boost::container::multimap<K, T, C, A>& c, MapOrMMap<K, T, C2, A2, O2>& right)
{
	typedef boost::container::multimap<K, T, C, A> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename K, typename T, typename C, typename A, typename Key> inline
typename boost::enable_if<detail::has_any_contains< boost::container::multimap<K, T, C, A> >, bool>::type
	contains(const boost::container::multimap<K, T, C, A>& c, const Key& key)
{
	return c.contains(key);
}

template<typename K, typename T, typename C, typename A, typename Key> inline
typename boost::disable_if<detail::has_any_contains< boost::container::multimap<K, T, C, A> >, bool>::type
	contains(const boost::container::multimap<K, T, C, A>& c, const Key& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename K, typename T, typename C, typename A, typename Cmper> inline
typename boost::container::multimap<K, T, C, A>::size_type
    erase_if(boost::container::multimap<K, T, C, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#else

// node_handle_traits using default

// extract
template<typename K, typename T, typename C, typename A, typename MO, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::node_handle_traits< boost::container::multimap<K, T, C, A, MO> >::node_type
	>::type
	extract(boost::container::multimap<K, T, C, A, MO>& c, BOOST_RV_REF(Key) key)
{
	return detail::multimap_node_extract(c, boost::move(key));
}

template<typename K, typename T, typename C, typename A, typename MO, typename KeyOrIter> inline
typename detail::node_handle_traits< boost::container::multimap<K, T, C, A, MO> >::node_type
	extract(boost::container::multimap<K, T, C, A, MO>& c, const KeyOrIter& key_or_iter)
{
	return detail::multimap_node_extract(c, key_or_iter);
}

// insert node
template<typename K, typename T, typename C, typename A, typename MO> inline
typename boost::container::multimap<K, T, C, A, MO>::iterator
	insert(boost::container::multimap<K, T, C, A, MO>& c,
			BOOST_RV_REF_BEG
				typename detail::node_handle_traits< boost::container::multimap<K, T, C, A, MO> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::multimap_node_insert(c, boost::move(node));
}

template<typename K, typename T, typename C, typename A, typename MO> inline
typename boost::container::multimap<K, T, C, A, MO>::iterator
	insert(boost::container::multimap<K, T, C, A, MO>& c,
			typename boost::container::multimap<K, T, C, A, MO>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename detail::node_handle_traits< boost::container::multimap<K, T, C, A, MO> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::multimap_node_insert(c, hint, boost::move(node));
}


//merge

namespace detail
{


#if (BOOST_VERSION < 106200)


template<typename K, typename T, typename C, typename A, typename O, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::multimap<K, T, C, A, O>,
													boost::container::multimap<K, T, C2, A, O> >
	: public map_merger_eti
{
};

template<typename K, typename T, typename C, typename A, typename O, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::multimap<K, T, C, A, O>,
													boost::container::map<K, T, C2, A, O> >
	: public map_merger_eti
{
};

#else

template<typename K, typename T, typename C, typename A, typename O, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::multimap<K, T, C, A, O>,
													boost::container::multimap<K, T, C2, A, O> >
	: public map_merger_inner
{
};

template<typename K, typename T, typename C, typename A, typename O, typename C2>
struct map_merger_sel_rule_is_different_comp_only<boost::container::multimap<K, T, C, A, O>,
													boost::container::map<K, T, C2, A, O> >
	: public map_merger_inner
{
};


#endif // (BOOST_VERSION < 106200)

template<typename K, typename T, typename C, typename A, typename O,
			typename K2, typename T2, typename C2, typename A2, typename O2>
struct map_merger_sel_rule_is_same_namespace<boost::container::multimap<K, T, C, A, O>,
												boost::container::map<K2, T2, C2, A2, O2> >
	: public
		map_merger_sel_rule_is_different_comp_only
		<
			boost::container::multimap<K, T, C, A, O>,
			boost::container::map<K2, T2, C2, A2, O2>
		>
{
};

} // namespace detail

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O, typename C2, typename A2> inline
void merge(boost::container::multimap<K, T, C, A, O>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2> BOOST_RV_REF_END right)
{
	typedef boost::container::multimap<K, T, C, A, O> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O, typename C2, typename A2> inline
void merge(boost::container::multimap<K, T, C, A, O>& c, MapOrMMap<K, T, C2, A2>& right)
{
	typedef boost::container::multimap<K, T, C, A, O> left_map_type;
	typedef MapOrMMap<K, T, C2, A2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O, typename C2, typename A2, typename O2> inline
void merge(boost::container::multimap<K, T, C, A, O>& c,
			BOOST_RV_REF_BEG MapOrMMap<K, T, C2, A2, O2> BOOST_RV_REF_END right)
{
	typedef boost::container::multimap<K, T, C, A, O> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	right_map_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O, typename C2, typename A2, typename O2> inline
void merge(boost::container::multimap<K, T, C, A, O>& c, MapOrMMap<K, T, C2, A2, O2>& right)
{
	typedef boost::container::multimap<K, T, C, A, O> left_map_type;
	typedef MapOrMMap<K, T, C2, A2, O2> right_map_type;
	typedef detail::map_merger_sel<left_map_type, right_map_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename K, typename T, typename C, typename A, typename MO, typename Key> inline
typename boost::enable_if<detail::has_any_contains< boost::container::multimap<K, T, C, A, MO> >, bool>::type
	contains(const boost::container::multimap<K, T, C, A, MO>& c, const Key& key)
{
	return c.contains(key);
}

template<typename K, typename T, typename C, typename A, typename MO, typename Key> inline
typename boost::disable_if<detail::has_any_contains< boost::container::multimap<K, T, C, A, MO> >, bool>::type
	contains(const boost::container::multimap<K, T, C, A, MO>& c, const Key& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename K, typename T, typename C, typename A, typename MO, typename Cmper> inline
typename boost::container::multimap<K, T, C, A, MO>::size_type
    erase_if(boost::container::multimap<K, T, C, A, MO>& c, Cmper cmper)
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
	5,
	::boost::container::map,
	::yggr::container::is_isomeric_same_options_map,
	inline)

YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_COMPARER_FULL(
	5,
	::boost::container::multimap,
	::yggr::container::is_isomeric_same_options_multimap,
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
		5,
		::boost::container::map,
		::yggr::container::is_isomeric_same_options_map,
		inline )

	YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_SWAP(
		5,
		::boost::container::multimap,
		::yggr::container::is_isomeric_same_options_multimap,
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

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, std::map)
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, std::multimap)

} // namespace std

namespace boost
{

#if BOOST_VERSION < 105600

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, boost::container::map)
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, boost::container::multimap)

#else

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(5, boost::container::map)
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(5, boost::container::multimap)

#endif // BOOST_VERSION < 105600

} // namespace boost

//---------------map----------------
#if !(BOOST_VERSION < 105600)

namespace yggr
{
namespace container
{
namespace detail
{

template<typename K, typename T, typename C, typename A>
class map;

template<typename K, typename T, typename C, typename A>
class multimap;

} // namespace detail
} // namespace container
} // namespace yggr


namespace yggr
{
namespace container
{
namespace detail
{

template<typename K, typename T,
			typename C = typename mplex::typename_expand_get<boost::container::map<K, T>, 2>::type,
			typename A = typename mplex::typename_expand_get<boost::container::map<K, T>, 3>::type >
class map
	: public boost::container::map<K, T, C, A>
{
public:
	typedef A tpl_arg_allocator_type;

public:
	typedef boost::container::map<K, T, C, A> base_type;

protected:
	typedef typename
		get_map_options
		<
			typename mplex::typename_expand_get<base_type, 4>::type
		>::type options_type;

public:
	typedef typename base_type::key_type				key_type;
	typedef typename base_type::allocator_traits_type	allocator_traits_type;
	typedef typename base_type::mapped_type				mapped_type;
	typedef typename base_type::value_type				value_type;
	typedef typename base_type::pointer					pointer;
	typedef typename base_type::const_pointer			const_pointer;
	typedef typename base_type::reference				reference;
	typedef typename base_type::const_reference			const_reference;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::allocator_type			allocator_type;
	typedef typename base_type::stored_allocator_type	stored_allocator_type;
	typedef typename base_type::value_compare			value_compare;
	typedef typename base_type::key_compare				key_compare;
	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;
	//typedef typename base_type::nonconst_value_type		nonconst_value_type; // boost 176 discarded
	typedef typename base_type::movable_value_type		movable_value_type;

#	if (BOOST_VERSION < 106200)
	typedef typename node_handle_foo_hacking_map<base_type>::node_type node_type;
	typedef typename node_handle_foo_hacking_map<base_type>::insert_return_type insert_return_type;
#	else
	typedef typename base_type::node_type node_type;
	typedef typename base_type::insert_return_type insert_return_type;
#	endif // (BOOST_VERSION < 106200)

private:
	typedef map this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(void)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	map(const key_compare& comp,
					const allocator_type& a = allocator_type())
		: base_type(comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	map(const allocator_type& a)
		: base_type(a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(InputIterator first, InputIterator last,
			const key_compare& comp = key_compare(),
			const allocator_type& a = allocator_type())
		: base_type(first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(boost::container::ordered_unique_range_t,
			InputIterator first, InputIterator last,
			const key_compare& comp = key_compare(),
			const allocator_type& a = allocator_type())
		: base_type(boost::container::ordered_range, first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(InputIterator first, InputIterator last, const allocator_type& a)
		: base_type(first, last, key_compare(), a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(std::initializer_list<value_type> il,
		const key_compare& comp = key_compare(),
		const allocator_type& a = allocator_type())
		: base_type(il.begin(), il.end(), comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(std::initializer_list<value_type> il, const allocator_type& a)
		: base_type(il.begin(), il.end(), key_compare(), a)
	{
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(const base_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(const this_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(BOOST_RV_REF(base_type) right, const allocator_type& a)
		: base_type(boost::move(right), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(const base_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(const this_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	// isomeric_same
	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(BOOST_RV_REF_BEG
			boost::container::map<key_type, mapped_type, C2, A2>
		BOOST_RV_REF_END right,
		typename
			boost::enable_if
			<
				is_isomeric_same_map
				<
					boost::container::map<key_type, mapped_type, C2, A2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::map<key_type, mapped_type, C2, A2>&>(right))))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(const boost::container::map<key_type, mapped_type, C2, A2>& right,
		typename
			boost::enable_if
			<
				is_isomeric_same_map
				<
					boost::container::map<key_type, mapped_type, C2, A2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(BOOST_RV_REF_BEG
			boost::container::map<key_type, mapped_type, C2, A2>
		BOOST_RV_REF_END right,
		const allocator_type& alloc,
		typename
			boost::enable_if
			<
				is_isomeric_same_map
				<
					boost::container::map<key_type, mapped_type, C2, A2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::map<key_type, mapped_type, C2, A2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(const boost::container::map<key_type, mapped_type, C2, A2>& right,
		const allocator_type& alloc,
		typename
			boost::enable_if
			<
				is_isomeric_same_map
				<
					boost::container::map<key_type, mapped_type, C2, A2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(BOOST_RV_REF_BEG
			map<key_type, mapped_type, C2, A2>
		BOOST_RV_REF_END right,
		typename
			boost::enable_if
			<
				is_isomeric_same_map
				<
					typename map<key_type, mapped_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename map<key_type, mapped_type, C2, A2>::base_type&>(right))))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(const map<key_type, mapped_type, C2, A2>& right,
		typename
			boost::enable_if
			<
				is_isomeric_same_map
				<
					typename map<key_type, mapped_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(BOOST_RV_REF_BEG
			map<key_type, mapped_type, C2, A2>
		BOOST_RV_REF_END right,
		const allocator_type& a,
		typename
			boost::enable_if
			<
				is_isomeric_same_map
				<
					typename map<key_type, mapped_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename map<key_type, mapped_type, C2, A2>::base_type&>(right))),
			a)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	map(const map<key_type, mapped_type, C2, A2>& right,
		const allocator_type& a,
		typename
			boost::enable_if
			<
				is_isomeric_same_map
				<
					typename map<key_type, mapped_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

	~map(void)
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

		base_type::operator=(boost::move(static_cast<base_type&>(right)));
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
			is_isomeric_same_options_map
			<
				boost::container::map<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::map<key_type, mapped_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::map<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::operator=(boost::move(reinterpret_cast<base_type&>(right_ref)));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				boost::container::map<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::map<key_type, mapped_type, C2, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				typename map<key_type, mapped_type, C2, A2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						map<key_type, mapped_type, C2, A2>
					BOOST_RV_REF_END right)
	{
		typedef map<key_type, mapped_type, C2, A2> right_type;
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
			is_isomeric_same_options_map
			<
				typename map<key_type, mapped_type, C2, A2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(const map<key_type, mapped_type, C2, A2>& right)
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
			is_isomeric_same_options_map
			<
				boost::container::map<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::map<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::map<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				boost::container::map<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::map<key_type, mapped_type, C2, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				typename map<key_type, mapped_type, C2, A2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					map<key_type, mapped_type, C2, A2>
				BOOST_RV_REF_END right)
	{
		typedef map<key_type, mapped_type, C2, A2> right_type;
		typedef typename right_type::base_type right_base_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_base_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_map
			<
				typename map<key_type, mapped_type, C2, A2>::base_type,
				base_type
			>,
			void
		>::type
		swap(map<key_type, mapped_type, C2, A2>& right)
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

	using base_type::operator[];
	using base_type::at;

	// insert
	//using base_type::insert;
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

	template<typename P> inline
	std::pair<iterator, bool>
		insert(BOOST_RV_REF(P) p)
	{
		return base_type::insert(boost::forward<P>(p));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(hint, boost::forward<value_type>(val));
	}

	inline iterator insert(const_iterator hint, const value_type& val)
	{
		return base_type::insert(hint, val);
	}

	template<typename P> inline
	iterator insert(const_iterator hint, BOOST_RV_REF(P) p)
	{
		return base_type::insert(hint, boost::forward<P>(p));
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
			detail::map_node_insert(
				static_cast<base_type&>(*this), boost::move(nh));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{
		return
			detail::map_node_insert(
				static_cast<base_type&>(*this), hint, boost::move(nh));
	}

	using base_type::emplace;
	using base_type::emplace_hint;

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
			detail::map_node_extract(
				static_cast<base_type&>(*this), boost::move(key));
	}

	inline node_type extract(const key_type& key)
	{
		return
			detail::map_node_extract(
				static_cast<base_type&>(*this), key);
	}

	inline node_type extract(const_iterator iter)
	{
		return
			detail::map_node_extract(
				static_cast<base_type&>(*this), iter);
	}
#	else

	using base_type::extract;

#	endif // (BOOST_VERSION < 106200)

	// merge

	template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
				typename C2, typename A2> inline
	void merge(BOOST_RV_REF_BEG MapOrMMap<key_type, mapped_type, C2, A2> BOOST_RV_REF_END right)
	{
		typedef MapOrMMap<key_type, mapped_type, C2, A2> right_type;

		right_type& right_ref = right;
		container::merge(static_cast<base_type&>(*this), right_ref);
	}

	template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
				typename C2, typename A2> inline
	void merge(MapOrMMap<key_type, mapped_type, C2, A2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
				typename C2, typename A2, typename O2> inline
	void merge(BOOST_RV_REF_BEG MapOrMMap<key_type, mapped_type, C2, A2, O2> BOOST_RV_REF_END right)
	{
		typedef MapOrMMap<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		container::merge(static_cast<base_type&>(*this), right_ref);
	}

	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
				typename C2, typename A2, typename O2> inline
	void merge(MapOrMMap<key_type, mapped_type, C2, A2, O2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	// contains
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

	// try_emplace
#if (BOOST_VERSION < 106200)
#	if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

	template<typename ...Args> inline
	std::pair<iterator, bool>
		try_emplace(BOOST_RV_REF(key_type) key, BOOST_FWD_REF(Args)... args)
	{
		return
			detail::try_emplace_impl(
				static_cast<base_type&>(*this), boost::move(key), boost::forward<Args>(args)...);
	}

	template<typename ...Args> inline
	std::pair<iterator, bool>
		try_emplace(const key_type& key, BOOST_FWD_REF(Args)... args)
	{
		return
			detail::try_emplace_impl(
				static_cast<base_type&>(*this), key, boost::forward<Args>(args)...);
	}

	template<typename ...Args> inline
	iterator try_emplace(const_iterator hint,
							BOOST_RV_REF(key_type) key,
							BOOST_FWD_REF(Args)... args)
	{
		return
			detail::try_emplace_impl(
				static_cast<base_type&>(*this), hint, boost::move(key), boost::forward<Args>(args)...);
	}

	template<typename ...Args> inline
	iterator try_emplace(const_iterator hint,
							const key_type& key,
							BOOST_FWD_REF(Args)... args)
	{
		return
			detail::try_emplace_impl(
				static_cast<base_type&>(*this), hint, key, boost::forward<Args>(args)...);
	}

#	else

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			BOOST_PP_EXPR_IF( __n__, template< ) \
				YGGR_PP_FOO_TYPES_DEF(__n__) \
			BOOST_PP_EXPR_IF( __n__, > ) inline \
			std::pair<iterator, bool> \
				try_emplace(BOOST_RV_REF(key_type) key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						static_cast<base_type&>(*this), boost::move(key) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); } \
			\
			BOOST_PP_EXPR_IF( __n__, template< ) \
				YGGR_PP_FOO_TYPES_DEF(__n__) \
			BOOST_PP_EXPR_IF( __n__, > ) inline \
			std::pair<iterator, bool> \
				try_emplace(const key_type& key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						static_cast<base_type&>(*this), key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); } \
			\
			BOOST_PP_EXPR_IF( __n__, template< ) \
				YGGR_PP_FOO_TYPES_DEF(__n__) \
			BOOST_PP_EXPR_IF( __n__, > ) inline \
			iterator try_emplace(const_iterator hint, \
									BOOST_RV_REF(key_type) key \
									YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						static_cast<base_type&>(*this), hint, boost::move(key) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); } \
			\
			BOOST_PP_EXPR_IF( __n__, template< ) \
				YGGR_PP_FOO_TYPES_DEF(__n__) \
			BOOST_PP_EXPR_IF( __n__, > ) inline \
			iterator try_emplace(const_iterator hint, \
									const key_type& key \
									YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						static_cast<base_type&>(*this), hint, key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); }

#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#		define YGGR_PP_FOO_ARG_NAME() init_arg

#		define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#		include BOOST_PP_LOCAL_ITERATE()

#		undef YGGR_PP_FOO_ARG_NAME
#		undef YGGR_TMP_PP_EMPLACE_DEF

#	endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#else

	using base_type::try_emplace;

#endif // (BOOST_VERSION < 106200)

	// insert_or_assign
#if (BOOST_VERSION < 106200)
	template<typename M> inline
	std::pair<iterator, bool>
		insert_or_assign(BOOST_RV_REF(key_type) key, BOOST_FWD_REF(M) m)
	{
		return
			detail::insert_or_assign_impl(
				static_cast<base_type&>(*this), boost::move(key), boost::forward<M>(m));
	}

	template<typename M> inline
	std::pair<iterator, bool>
		insert_or_assign(const key_type& key, BOOST_FWD_REF(M) m)
	{
		return
			detail::insert_or_assign_impl(
				static_cast<base_type&>(*this), key, boost::forward<M>(m));
	}

	template<typename M> inline
	iterator insert_or_assign(const_iterator hint,
								BOOST_RV_REF(key_type) key,
								BOOST_FWD_REF(M) m)
	{
		return
			detail::insert_or_assign_impl(
				static_cast<base_type&>(*this), hint, boost::move(key), boost::forward<M>(m));
	}

	template<typename M> inline
	iterator insert_or_assign(const_iterator hint,
								const key_type& key,
								BOOST_FWD_REF(M) m)
	{
		return
			detail::insert_or_assign_impl(
				static_cast<base_type&>(*this), hint, key, boost::forward<M>(m));
	}

#else

#	if ((106100 < BOOST_VERSION) && (BOOST_VERSION < 107100) && !defined(YGGR_FIXBUG_BOOST_162_TO_170_MAP_INSERT_OR_ASSIGN_HINT))

YGGR_PRAGMA_MESSAGE("!!!please using boost code patch fix this boost::map insert_or_assign bug!!!")

	template<typename M> inline
	std::pair<iterator, bool>
		insert_or_assign(BOOST_RV_REF(key_type) key, BOOST_FWD_REF(M) m)
	{
		return base_type::insert_or_assign(boost::move(key), boost::forward<M>(m));
	}

	template<typename M> inline
	std::pair<iterator, bool>
		insert_or_assign(const key_type& key, BOOST_FWD_REF(M) m)
	{
		return base_type::insert_or_assign(key, boost::forward<M>(m));
	}

	template<typename M> inline
	iterator insert_or_assign(const_iterator hint,
								BOOST_RV_REF(key_type) key,
								BOOST_FWD_REF(M) m)
	{
		return
			detail::non_has_insert_or_assign_impl(
				static_cast<base_type&>(*this), hint, boost::move(key), boost::forward<M>(m));
	}

	template<typename M> inline
	iterator insert_or_assign(const_iterator hint,
								const key_type& key,
								BOOST_FWD_REF(M) m)
	{
		return
			detail::non_has_insert_or_assign_impl(
				static_cast<base_type&>(*this), hint, key, boost::forward<M>(m));
	}
#	else
	using base_type::insert_or_assign;
#endif // if(BOOST_VERSION < 107100) && !defined(YGGR_FIXBUG_BOOST_162_TO_170_MAP_INSERT_OR_ASSIGN_HINT)

#endif // (BOOST_VERSION < 106200)

};

YGGR_PP_CONTAINER_COMPARER_FULL(4, map, inline)

YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL_MN(
	5, 4,
	::boost::container::map,
	map,
	is_isomeric_same_options_map,
	inline)


YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, map)

} // namespace detail

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	std::pair<typename detail::map<K, T, C, A>::iterator, bool>
		emplace(detail::map<K, T, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename detail::map<K, T, C, A>::iterator
		emplace_hint(detail::map<K, T, C, A>& c,
						typename detail::map<K, T, C, A>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename detail::map<K, T, C, A>::iterator, bool> \
			emplace(detail::map<K, T, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::map<K, T, C, A>::iterator \
			emplace_hint(detail::map<K, T, C, A>& c, \
							typename detail::map<K, T, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint( \
					pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
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
template<typename K, typename T, typename C, typename A, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::map<K, T, C, A>::node_type
	>::type
	extract(detail::map<K, T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename K, typename T, typename C, typename A, typename KeyOrIter> inline
typename detail::map<K, T, C, A>::node_type
	extract(detail::map<K, T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// insert node
template<typename K, typename T, typename C, typename A> inline
typename detail::map<K, T, C, A>::insert_return_type
	insert(detail::map<K, T, C, A>& c,
			BOOST_RV_REF_BEG
				typename detail::map<K, T, C, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename T, typename C, typename A> inline
typename detail::map<K, T, C, A>::itrator
	insert(detail::map<K, T, C, A>& c,
			typename detail::map<K, T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename detail::map<K, T, C, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// merge

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(detail::map<K, T, C, A>& c,
			BOOST_RV_REF_BEG
				MapOrMMap<K, T, C2, A2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(detail::map<K, T, C, A>& c, MapOrMMap<K, T, C2, A2>& right)
{
	c.merge(right);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(detail::map<K, T, C, A>& c,
			BOOST_RV_REF_BEG
				MapOrMMap<K, T, C2, A2, O2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(detail::map<K, T, C, A>& c, MapOrMMap<K, T, C2, A2, O2>& right)
{
	c.merge(right);
}

// contains
template<typename K, typename T, typename C, typename A, typename Key> inline
bool contains(const detail::map<K, T, C, A>& c, const Key& key)
{
	return c.contains(key);
}

// try_emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename K, typename T, typename C, typename A, typename ...Args> inline
std::pair<typename detail::map<K, T, C, A>::iterator, bool>
	try_emplace(detail::map<K, T, C, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(boost::move(key), boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
std::pair<typename detail::map<K, T, C, A>::iterator, bool>
	try_emplace(detail::map<K, T, C, A>& c, const K& key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(key, boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename detail::map<K, T, C, A>::iterator
	try_emplace(detail::map<K, T, C, A>& c,
				typename detail::map<K, T, C, A>::const_iterator hint,
				BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(hint, boost::move(key), boost::forward<Args>(args)...);
}

template<typename K, typename T, typename C, typename A, typename ...Args> inline
typename detail::map<K, T, C, A>::iterator
	try_emplace(detail::map<K, T, C, A>& c,
				typename detail::map<K, T, C, A>::const_iterator hint,
				const K& key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(hint, key, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename detail::map<K, T, C, A>::iterator, bool> \
			try_emplace(detail::map<K, T, C, A>& c, BOOST_RV_REF(K) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename detail::map<K, T, C, A>::iterator, bool> \
			try_emplace(detail::map<K, T, C, A>& c, const K& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::map<K, T, C, A>::iterator \
			try_emplace(detail::map<K, T, C, A>& c, \
						typename detail::map<K, T, C, A>::const_iterator hint, \
						BOOST_RV_REF(K) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					hint, boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::map<K, T, C, A>::iterator \
			try_emplace(detail::map<K, T, C, A>& c, \
						typename detail::map<K, T, C, A>::const_iterator hint, \
						const K& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					hint, key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// insert_or_assign
template<typename K, typename T, typename C, typename A, typename M> inline
std::pair<typename detail::map<K, T, C, A>::iterator, bool>
	insert_or_assign(detail::map<K, T, C, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename M> inline
std::pair<typename detail::map<K, T, C, A>::iterator, bool>
	insert_or_assign(detail::map<K, T, C, A>& c, const K& key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(key, boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename M> inline
typename detail::map<K, T, C, A>::iterator
	insert_or_assign(detail::map<K, T, C, A>& c,
						typename detail::map<K, T, C, A>::const_iterator hint,
						BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(hint, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename C, typename A, typename M> inline
typename detail::map<K, T, C, A>::iterator
	insert_or_assign(detail::map<K, T, C, A>& c,
						typename detail::map<K, T, C, A>::const_iterator hint,
						const K& key, BOOST_FWD_REF(M) m)
{
	return c.insert_or_assign(hint, key, boost::forward<M>(m));
}

// erase_if
template<typename K, typename T, typename C, typename A, typename Cmper> inline
typename detail::map<K, T, C, A>::size_type
    erase_if(detail::map<K, T, C, A>& c, Cmper cmper)
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
	YGGR_PP_CONTAINER_SWAP(4, ::yggr::container::detail::map, inline)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		5, 4,
		::boost::container::map,
		::yggr::container::detail::map,
		::yggr::container::is_isomeric_same_options_map,
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

//---------------multi_map--------------

namespace yggr
{
namespace container
{
namespace detail
{

template<typename K, typename T,
			typename C = typename mplex::typename_expand_get<boost::container::multimap<K, T>, 2>::type,
			typename A = typename mplex::typename_expand_get<boost::container::multimap<K, T>, 3>::type >
class multimap
	: public boost::container::multimap<K, T, C, A>
{

public:
	typedef A tpl_arg_allocator_type;

public:
	typedef boost::container::multimap<K, T, C, A> base_type;

protected:
	typedef typename
		get_multimap_options
		<
			typename mplex::typename_expand_get<base_type, 4>::type
		>::type options_type;

public:
	typedef typename base_type::key_type				key_type;
	typedef typename base_type::mapped_type				mapped_type;
	typedef typename base_type::value_type				value_type;
	typedef typename base_type::pointer					pointer;
	typedef typename base_type::const_pointer			const_pointer;
	typedef typename base_type::reference				reference;
	typedef typename base_type::const_reference			const_reference;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::allocator_type			allocator_type;
	typedef typename base_type::stored_allocator_type	stored_allocator_type;
	typedef typename base_type::value_compare			value_compare;
	typedef typename base_type::key_compare				key_compare;
	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;
	//typedef typename base_type::nonconst_value_type		nonconst_value_type; // boost 176 discarded
	typedef typename base_type::movable_value_type		movable_value_type;

#	if (BOOST_VERSION < 106200)
	typedef typename node_handle_foo_hacking_map<base_type>::node_type node_type;
#	else
	typedef typename base_type::node_type node_type;
#	endif // (BOOST_VERSION < 106200)

private:
	typedef multimap this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(void)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	multimap(const key_compare& comp,
				const allocator_type& a = allocator_type())
		: base_type(comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	multimap(const allocator_type& a)
		: base_type(a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(InputIterator first, InputIterator last,
				const key_compare& comp = key_compare(),
				const allocator_type& a = allocator_type())
		: base_type(first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(boost::container::ordered_range_t,
				InputIterator first, InputIterator last,
				const key_compare& comp = key_compare(),
				const allocator_type& a = allocator_type())
		: base_type(boost::container::ordered_range, first, last, comp, a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(InputIterator first, InputIterator last,
				const allocator_type& a)
		: base_type(first, last, key_compare(), a)
	{
	}

	template<typename InputIterator> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(boost::container::ordered_range_t,
				InputIterator first, InputIterator last,
				const allocator_type& a)
		: base_type(boost::container::ordered_range, first, last, key_compare(), a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(std::initializer_list<value_type> il,
				const key_compare& comp = key_compare(),
				const allocator_type& a = allocator_type())
		: base_type(il.begin(), il.end(), comp, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(std::initializer_list<value_type> il, const allocator_type& a)
		: base_type(il.begin(), il.end(), key_compare(), a)
	{
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(const base_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(const this_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(BOOST_RV_REF(base_type) right, const allocator_type& a)
		: base_type(boost::move(right), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(const base_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(const this_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	// isomeric_same
	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(BOOST_RV_REF_BEG
			boost::container::multimap<key_type, mapped_type, C2, A2>
		BOOST_RV_REF_END right,
		typename
			boost::enable_if
			<
				is_isomeric_same_multimap
				<
					boost::container::multimap<key_type, mapped_type, C2, A2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multimap<key_type, mapped_type, C2, A2>&>(right))))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(const boost::container::multimap<key_type, mapped_type, C2, A2>& right,
		typename
			boost::enable_if
			<
				is_isomeric_same_multimap
				<
					boost::container::multimap<key_type, mapped_type, C2, A2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(BOOST_RV_REF_BEG
			boost::container::multimap<key_type, mapped_type, C2, A2>
		BOOST_RV_REF_END right,
		const allocator_type& alloc,
		typename
			boost::enable_if
			<
				is_isomeric_same_multimap
				<
					boost::container::multimap<key_type, mapped_type, C2, A2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::multimap<key_type, mapped_type, C2, A2>&>(right))),
			alloc)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(const boost::container::multimap<key_type, mapped_type, C2, A2>& right,
		const allocator_type& alloc,
		typename
			boost::enable_if
			<
				is_isomeric_same_multimap
				<
					boost::container::multimap<key_type, mapped_type, C2, A2>,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), alloc)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(BOOST_RV_REF_BEG
			multimap<key_type, mapped_type, C2, A2>
		BOOST_RV_REF_END right,
		typename
			boost::enable_if
			<
				is_isomeric_same_multimap
				<
					typename multimap<key_type, mapped_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename multimap<key_type, mapped_type, C2, A2>::base_type&>(right))))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(const multimap<key_type, mapped_type, C2, A2>& right,
		typename
			boost::enable_if
			<
				is_isomeric_same_multimap
				<
					typename multimap<key_type, mapped_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(BOOST_RV_REF_BEG
			multimap<key_type, mapped_type, C2, A2>
		BOOST_RV_REF_END right,
		const allocator_type& a,
		typename
			boost::enable_if
			<
				is_isomeric_same_multimap
				<
					typename multimap<key_type, mapped_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename multimap<key_type, mapped_type, C2, A2>::base_type&>(right))),
			a)
	{
	}

	template<typename C2, typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	multimap(const multimap<key_type, mapped_type, C2, A2>& right,
		const allocator_type& a,
		typename
			boost::enable_if
			<
				is_isomeric_same_multimap
				<
					typename multimap<key_type, mapped_type, C2, A2>::base_type,
					base_type
				>,
				mplex::sfinae_type
			>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

	~multimap(void)
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

		base_type::operator=(boost::move(static_cast<base_type&>(right)));
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
			is_isomeric_same_options_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::multimap<key_type, mapped_type, C2, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::multimap<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::operator=(boost::move(reinterpret_cast<base_type&>(right_ref)));
		return *this;
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			this_type&
		>::type
		operator=(const boost::container::multimap<key_type, mapped_type, C2, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				typename multimap<key_type, mapped_type, C2, A2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						multimap<key_type, mapped_type, C2, A2>
					BOOST_RV_REF_END right)
	{
		typedef multimap<key_type, mapped_type, C2, A2> right_type;
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
			is_isomeric_same_options_multimap
			<
				typename multimap<key_type, mapped_type, C2, A2>::base_type,
				base_type
			>,
			this_type&
		>::type
		operator=(const multimap<key_type, mapped_type, C2, A2>& right)
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
			is_isomeric_same_options_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::multimap<key_type, mapped_type, C2, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::multimap<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		base_type::swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename C2, typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				boost::container::multimap<key_type, mapped_type, C2, A2, O2>,
				base_type
			>,
			void
		>::type
		swap(boost::container::multimap<key_type, mapped_type, C2, A2, O2>& right)
	{
		base_type::swap(reinterpret_cast<base_type&>(right));
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				typename multimap<key_type, mapped_type, C2, A2>::base_type,
				base_type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					multimap<key_type, mapped_type, C2, A2>
				BOOST_RV_REF_END right)
	{
		typedef multimap<key_type, mapped_type, C2, A2> right_type;
		typedef typename right_type::base_type right_base_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_base_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename C2, typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_multimap
			<
				typename multimap<key_type, mapped_type, C2, A2>::base_type,
				base_type
			>,
			void
		>::type
		swap(multimap<key_type, mapped_type, C2, A2>& right)
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

	// insert
	//	using base_type::insert;
	inline iterator insert(BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(boost::forward<value_type>(val));
	}

	inline iterator insert(const value_type& val)
	{
		return base_type::insert(val);
	}

	template<typename P> inline
	iterator insert(BOOST_RV_REF(P) p)
	{
		return base_type::insert(boost::forward<P>(p));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(value_type) val)
	{
		return base_type::insert(hint, boost::forward<value_type>(val));
	}

	inline iterator insert(const_iterator hint, const value_type& val)
	{
		return base_type::insert(hint, val);
	}

	template<typename P> inline
	iterator insert(const_iterator hint, BOOST_RV_REF(P) p)
	{
		return base_type::insert(hint, boost::forward<P>(p));
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

	inline iterator insert(BOOST_RV_REF(node_type) nh)
	{
		return
			detail::multimap_node_insert(
				static_cast<base_type&>(*this), boost::move(nh));
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{
		return
			detail::multimap_node_insert(
				static_cast<base_type&>(*this), hint, boost::move(nh));
	}

	using base_type::emplace;
	using base_type::emplace_hint;

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
			detail::multimap_node_extract(
				static_cast<base_type&>(*this), boost::move(key));
	}

	inline node_type extract(const key_type& key)
	{
		return
			detail::multimap_node_extract(
				static_cast<base_type&>(*this), key);
	}

	inline node_type extract(const_iterator iter)
	{
		return
			detail::multimap_node_extract(
				static_cast<base_type&>(*this), iter);
	}
#	else
	using base_type::extract;
#	endif // (BOOST_VERSION < 106200)

	// merge

	template< template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
				typename C2, typename A2> inline
	void merge(BOOST_RV_REF_BEG MapOrMMap<key_type, mapped_type, C2, A2> BOOST_RV_REF_END right)
	{
		typedef MapOrMMap<key_type, mapped_type, C2, A2> right_type;

		right_type& right_ref = right;
		container::merge(static_cast<base_type&>(*this), right_ref);
	}

	template< template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
				typename C2, typename A2> inline
	void merge(MapOrMMap<key_type, mapped_type, C2, A2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	template< template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
				typename C2, typename A2, typename O2> inline
	void merge(BOOST_RV_REF_BEG MapOrMMap<key_type, mapped_type, C2, A2, O2> BOOST_RV_REF_END right)
	{
		typedef MapOrMMap<key_type, mapped_type, C2, A2, O2> right_type;

		right_type& right_ref = right;
		container::merge(static_cast<base_type&>(*this), right_ref);
	}

	template< template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
				typename C2, typename A2, typename O2> inline
	void merge(MapOrMMap<key_type, mapped_type, C2, A2, O2>& right)
	{
		container::merge(static_cast<base_type&>(*this), right);
	}

	// contains
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

YGGR_PP_CONTAINER_COMPARER_FULL(4, multimap, inline)

YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL_MN(
	5, 4,
	::boost::container::multimap,
	multimap,
	is_isomeric_same_options_multimap,
	inline)

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, multimap)

} // namespace detail

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename detail::multimap<K, T, C, A>::iterator
		emplace(detail::multimap<K, T, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename C, typename A, typename ...Args> inline
	typename detail::multimap<K, T, C, A>::iterator
		emplace_hint(detail::multimap<K, T, C, A>& c,
						typename detail::multimap<K, T, C, A>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::multimap<K, T, C, A>::iterator \
			emplace(detail::multimap<K, T, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::multimap<K, T, C, A>::iterator \
			emplace_hint(detail::multimap<K, T, C, A>& c, \
							typename detail::multimap<K, T, C, A>::const_iterator pos \
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
template<typename K, typename T, typename C, typename A, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename detail::multimap<K, T, C, A>::node_type
	>::type
	extract(detail::multimap<K, T, C, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename K, typename T, typename C, typename A, typename KeyOrIter> inline
typename detail::multimap<K, T, C, A>::node_type
	extract(detail::multimap<K, T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// insert node
template<typename K, typename T, typename C, typename A> inline
typename detail::multimap<K, T, C, A>::iterator
	insert(detail::multimap<K, T, C, A>& c,
			BOOST_RV_REF_BEG
				typename detail::multimap<K, T, C, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename T, typename C, typename A> inline
typename detail::multimap<K, T, C, A>::iterator
	insert(detail::multimap<K, T, C, A>& c,
			typename detail::multimap<K, T, C, A>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename detail::multimap<K, T, C, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// merge

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(detail::multimap<K, T, C, A>& c,
			BOOST_RV_REF_BEG
				MapOrMMap<K, T, C2, A2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2> inline
void merge(detail::multimap<K, T, C, A>& c, MapOrMMap<K, T, C2, A2>& right)
{
	c.merge(right);
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(detail::multimap<K, T, C, A>& c,
			BOOST_RV_REF_BEG
				MapOrMMap<K, T, C2, A2, O2>
			BOOST_RV_REF_END right)
{
	c.merge(boost::move(right));
}

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename C2, typename A2, typename O2> inline
void merge(detail::multimap<K, T, C, A>& c, MapOrMMap<K, T, C2, A2, O2>& right)
{
	c.merge(right);
}

// contains
template<typename K, typename T, typename C, typename A, typename Key> inline
bool contains(const detail::multimap<K, T, C, A>& c, const Key& key)
{
	return c.contains(key);
}

// erase_if
template<typename K, typename T, typename C, typename A, typename Cmper> inline
typename detail::multimap<K, T, C, A>::size_type
    erase_if(detail::multimap<K, T, C, A>& c, Cmper cmper)
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
	YGGR_PP_CONTAINER_SWAP(4, ::yggr::container::detail::multimap, inline)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		5, 4,
		::boost::container::multimap,
		::yggr::container::detail::multimap,
		::yggr::container::is_isomeric_same_options_multimap,
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
		using boost::container::map;
		using boost::container::multimap;
#	else
		using detail::map;
		using detail::multimap;
#	endif // BOOST_VERSION < 105600
#elif defined YGGR_USE_STL_CONTAINER
	using std::map;
	using std::multimap;
#else
#	error "use map type unknown error. please check source"
#endif //YGGR_USE_BOOST_CONTAINER

} // namespace container

using container::map;
using container::multimap;

} // namespace yggr

#endif // __YGGR_CONTAINER_MAP_HPP__
