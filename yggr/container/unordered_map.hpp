//unordered_map.hpp

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

#ifndef __YGGR_CONTAINER_UNORDERED_MAP_HPP__
#define __YGGR_CONTAINER_UNORDERED_MAP_HPP__

#include <yggr/container/container_selector.hpp>

//#include <yggr/utility/pair_compare.hpp>
#include <yggr/move/move.hpp>

#include <yggr/mplex/typename_expand.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/container/basic_function_adapter.hpp>
#include <yggr/container/node_handle_traits.hpp>

#include <yggr/container/detail/is_xxx_container.hpp>
#include <yggr/container/detail/is_s_or_m_container.hpp>
#include <yggr/container/detail/range_size_def.hpp>

#include <yggr/container/detail/has_merge.hpp>
#include <yggr/container/detail/has_contains.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_umap.hpp>
#include <yggr/container/detail/try_emplace_impl.hpp>
#include <yggr/container/detail/insert_or_assign_impl.hpp>

#include <yggr/container/detail/umap_merge_impl.hpp>
#include <yggr/container/detail/erase_impl.hpp>
#include <yggr/container/detail/umap_merger_sel.hpp>

#include <boost/unordered_map.hpp>

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
#	include <unordered_map>
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

namespace yggr
{
namespace container
{

#if defined YGGR_USE_BOOST_CONTAINER
	using boost::unordered_map;
	using boost::unordered_multimap;
#elif defined YGGR_USE_STL_CONTAINER
#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
		using std::unordered_map;
		using std::unordered_multimap;
#	else
		using boost::unordered_map;
		using boost::unordered_multimap;
#	endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP
#else
#	error "use unordered_map type unknown error. please check source"
#endif //YGGR_USE_BOOST_CONTAINER

} // namespace container

using container::unordered_map;
using container::unordered_multimap;

} // namespace yggr


#ifndef YGGR_NO_CXX11_HDR_UNORDERED_MAP

YGGR_PP_IS_XXX_CONTAINER_DEF(5, std_unordered_map, std::unordered_map);
YGGR_PP_IS_XXX_CONTAINER_DEF(5, std_unordered_multimap, std::unordered_multimap);

#else

namespace yggr
{
namespace container
{

template<typename Container>
struct is_std_unordered_map
	: public boost::mpl::false_
{
};

template<typename Container>
struct is_std_unordered_multimap
	: public boost::mpl::false_
{
};

} // namespace container
} // namespace yggr

#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

YGGR_PP_IS_XXX_CONTAINER_DEF(5, boost_unordered_map, boost::unordered_map);
YGGR_PP_IS_XXX_CONTAINER_DEF(5, boost_unordered_multimap, boost::unordered_multimap);

// compatibility caller
// unordered_container not need compatibility caller,
// because boost is same stl

// -----------------map---------------
namespace yggr
{
namespace container
{

//std

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)

	template<typename K, typename T, typename H, typename C, typename A, typename ...Args> inline
	std::pair< typename std::unordered_map<K, T, H, C, A>::iterator, bool>
		emplace(std::unordered_map<K, T, H, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}


	template<typename K, typename T, typename H, typename C, typename A, typename ...Args> inline
	typename std::unordered_map<K, T, H, C, A>::iterator
		emplace_hint(std::unordered_map<K, T, H, C, A>& c,
						typename std::unordered_map<K, T, H, C, A>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#	endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

#else

#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			template<typename K, typename T, typename H, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename std::unordered_map<K, T, H, C, A>::iterator, bool> \
				emplace(std::unordered_map<K, T, H, C, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename K, typename T, typename H, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename std::unordered_map<K, T, H, C, A>::iterator \
				emplace_hint(std::unordered_map<K, T, H, C, A>& c, \
								typename std::unordered_map<K, T, H, C, A>::const_iterator pos \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#		define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#		define YGGR_PP_FOO_ARG_NAME() init_arg

#		define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#		include BOOST_PP_LOCAL_ITERATE()

#		undef YGGR_PP_FOO_ARG_NAME
#		undef YGGR_TMP_PP_EMPLACE_DEF

#	endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)

// unordered_map
// extract
template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			detail::native_extractable<std::unordered_map<K, T, H, P, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename std::unordered_map<K, T, H, P, A>::node_type
	>::type
	extract(std::unordered_map<K, T, H, P, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename K, typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename
	boost::enable_if
	<
		detail::native_extractable<std::unordered_map<K, T, H, P, A> >,
		typename std::unordered_map<K, T, H, P, A>::node_type
	>::type
	extract(std::unordered_map<K, T, H, P, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// node_insert
template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable<std::unordered_map<K, T, H, P, A> >,
		typename std::unordered_map<K, T, H, P, A>::insert_return_type
	>::type
	insert(std::unordered_map<K, T, H, P, A>& c,
			BOOST_RV_REF_BEG
				typename std::unordered_map<K, T, H, P, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable<std::unordered_map<K, T, H, P, A> >,
		typename std::unordered_map<K, T, H, P, A>::iterator
	>::type
	insert(std::unordered_map<K, T, H, P, A>& c,
			typename std::unordered_map<K, T, H, P, A>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename std::unordered_map<K, T, H, P, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// merge

namespace detail
{

template<typename K, typename T, typename H, typename P, typename A,
			typename K2, typename T2, typename H2, typename P2, typename A2>
struct umap_merger_sel_rule_is_same_namespace<std::unordered_map<K, T, H, P, A>,
												std::unordered_multimap<K2, T2, H2, P2, A2> >
	: public
		umap_merger_sel_rule_is_different_hash_and_equal_only
		<
			std::unordered_map<K, T, H, P, A>,
			std::unordered_multimap<K2, T2, H2, P2, A2>
		>
{
};

} // namespace detail

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMMap,
			typename K, typename T, typename H, typename P, typename A,
			typename H2, typename P2, typename A2> inline
void merge(std::unordered_map<K, T, H, P, A>& c,
			BOOST_RV_REF_BEG
				UMapOrUMMap<K, T, H2, P2, A2>
			BOOST_RV_REF_END right)
{
	typedef std::unordered_map<K, T, H, P, A> left_umap_type;
	typedef UMapOrUMMap<K, T, H2, P2, A2> right_umap_type;
	typedef detail::umap_merger_sel<left_umap_type, right_umap_type> merger_type;

	right_umap_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMMap,
			typename K, typename T, typename H, typename P, typename A,
			typename H2, typename P2, typename A2> inline
void merge(std::unordered_map<K, T, H, P, A>& c, UMapOrUMMap<K, T, H2, P2, A2>& right)
{
	typedef std::unordered_map<K, T, H, P, A> left_umap_type;
	typedef UMapOrUMMap<K, T, H2, P2, A2> right_umap_type;
	typedef detail::umap_merger_sel<left_umap_type, right_umap_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename boost::enable_if<detail::has_any_contains< std::unordered_map<K, T, H, P, A> >, bool>::type
	contains(const std::unordered_map<K, T, H, P, A>& c, const Key& key)
{
	return c.contains(key);
}

template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename boost::disable_if<detail::has_any_contains< std::unordered_map<K, T, H, P, A> >, bool>::type
	contains(const std::unordered_map<K, T, H, P, A>& c, const Key& key)
{
	return c.find(key) != c.end();
}

// try_emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename K, typename T, typename H, typename P, typename A, typename ...Args> inline
std::pair<typename std::unordered_map<K, T, H, P, A>::iterator, bool>
	try_emplace(std::unordered_map<K, T, H, P, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
{
	return detail::try_emplace_impl(c, boost::move(key), boost::forward<Args>(args)...);
}

template<typename K, typename T, typename H, typename P, typename A, typename ...Args> inline
std::pair<typename std::unordered_map<K, T, H, P, A>::iterator, bool>
	try_emplace(std::unordered_map<K, T, H, P, A>& c, const K& key, BOOST_FWD_REF(Args)... args)
{
	return detail::try_emplace_impl(c, key, boost::forward<Args>(args)...);
}

template<typename K, typename T, typename H, typename P, typename A, typename ...Args> inline
typename std::unordered_map<K, T, H, P, A>::iterator
	try_emplace(std::unordered_map<K, T, H, P, A>& c,
				typename std::unordered_map<K, T, H, P, A>::const_iterator hint,
				BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
{
	return detail::try_emplace_impl(c, hint, boost::move(key), boost::forward<Args>(args)...);
}

template<typename K, typename T, typename H, typename P, typename A, typename ...Args> inline
typename std::unordered_map<K, T, H, P, A>::iterator
	try_emplace(std::unordered_map<K, T, H, P, A>& c,
				typename std::unordered_map<K, T, H, P, A>::const_iterator hint,
				const K& key, BOOST_FWD_REF(Args)... args)
{
	return detail::try_emplace_impl(c, hint, key, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename H, typename P, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename std::unordered_map<K, T, H, P, A>::iterator, bool> \
			try_emplace(std::unordered_map<K, T, H, P, A>& c, BOOST_RV_REF(K) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				detail::try_emplace_impl( \
					c, boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename H, typename P, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename std::unordered_map<K, T, H, P, A>::iterator, bool> \
			try_emplace(std::unordered_map<K, T, H, P, A>& c, const K& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				detail::try_emplace_impl( \
					c, key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename H, typename P, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::unordered_map<K, T, H, P, A>::iterator \
			try_emplace(std::unordered_map<K, T, H, P, A>& c, \
						typename std::unordered_map<K, T, H, P, A>::const_iterator hint, \
						BOOST_RV_REF(K) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				detail::try_emplace_impl( \
					c, hint, boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename H, typename P, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::unordered_map<K, T, H, P, A>::iterator \
			try_emplace(std::unordered_map<K, T, H, P, A>& c, \
						typename std::unordered_map<K, T, H, P, A>::const_iterator hint, \
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
template<typename K, typename T, typename H, typename P, typename A, typename M> inline
std::pair<typename std::unordered_map<K, T, H, P, A>::iterator, bool>
	insert_or_assign(std::unordered_map<K, T, H, P, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename H, typename P, typename A, typename M> inline
std::pair<typename std::unordered_map<K, T, H, P, A>::iterator, bool>
	insert_or_assign(std::unordered_map<K, T, H, P, A>& c, const K& key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, key, boost::forward<M>(m));
}

template<typename K, typename T, typename H, typename P, typename A, typename M> inline
typename std::unordered_map<K, T, H, P, A>::iterator
	insert_or_assign(std::unordered_map<K, T, H, P, A>& c,
				typename std::unordered_map<K, T, H, P, A>::const_iterator hint,
				BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, hint, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename H, typename P, typename A, typename M> inline
typename std::unordered_map<K, T, H, P, A>::iterator
	insert_or_assign(std::unordered_map<K, T, H, P, A>& c,
				typename std::unordered_map<K, T, H, P, A>::const_iterator hint,
				const K& key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, hint, key, boost::forward<M>(m));
}

// erase_if
template<typename K, typename T, typename H, typename P, typename A, typename Cmper> inline
typename std::unordered_map<K, T, H, P, A>::size_type
    erase_if(std::unordered_map<K, T, H, P, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#endif // (YGGR_NO_CXX11_HDR_UNORDERED_MAP)

// boost
//emplace

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename K, typename T, typename H, typename C, typename A, typename ...Args> inline
std::pair<typename boost::unordered_map<K, T, H, C, A>::iterator, bool>
	emplace(boost::unordered_map<K, T, H, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename K, typename T, typename H, typename C, typename A, typename ...Args> inline
typename boost::unordered_map<K, T, H, C, A>::iterator
	emplace_hint(boost::unordered_map<K, T, H, C, A>& c,
					typename boost::unordered_map<K, T, H, C, A>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename H, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename boost::unordered_map<K, T, H, C, A>::iterator, bool> \
			emplace(boost::unordered_map<K, T, H, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename H, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::unordered_map<K, T, H, C, A>::iterator \
			emplace_hint(boost::unordered_map<K, T, H, C, A>& c, \
							typename boost::unordered_map<K, T, H, C, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// extract
template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename node_handle_traits<boost::unordered_map<K, T, H, P, A> >::node_type
	>::type
	extract(boost::unordered_map<K, T, H, P, A>& c, BOOST_RV_REF(Key) key)
{
	return detail::umap_node_extract(c, boost::move(key));
}

template<typename K, typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename node_handle_traits<boost::unordered_map<K, T, H, P, A> >::node_type
	extract(boost::unordered_map<K, T, H, P, A>& c, const KeyOrIter& key_or_iter)
{
	return detail::umap_node_extract(c, key_or_iter);
}

// node_insert
template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable<boost::unordered_map<K, T, H, P, A> >,
		typename node_handle_traits<boost::unordered_map<K, T, H, P, A> >::insert_return_type
	>::type
	insert(boost::unordered_map<K, T, H, P, A>& c,
			BOOST_RV_REF_BEG
				typename node_handle_traits<boost::unordered_map<K, T, H, P, A> >::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::disable_if
	<
		detail::native_node_insertable<boost::unordered_map<K, T, H, P, A> >,
		typename node_handle_traits<boost::unordered_map<K, T, H, P, A> >::insert_return_type
	>::type
	insert(boost::unordered_map<K, T, H, P, A>& c,
			BOOST_RV_REF_BEG
				typename node_handle_traits<boost::unordered_map<K, T, H, P, A> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::umap_node_insert(c, boost::move(node));
}

template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable<boost::unordered_map<K, T, H, P, A> >,
		typename boost::unordered_map<K, T, H, P, A>::iterator
	>::type
	insert(boost::unordered_map<K, T, H, P, A>& c,
				typename boost::unordered_map<K, T, H, P, A>::const_iterator hint,
				BOOST_RV_REF_BEG
					typename node_handle_traits<boost::unordered_map<K, T, H, P, A> >::node_type
				BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::disable_if
	<
		detail::native_node_insertable<boost::unordered_map<K, T, H, P, A> >,
		typename boost::unordered_map<K, T, H, P, A>::iterator
	>::type
	insert(boost::unordered_map<K, T, H, P, A>& c,
			typename boost::unordered_map<K, T, H, P, A>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename node_handle_traits<boost::unordered_map<K, T, H, P, A> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::umap_node_insert(c, hint, boost::move(node));
}

//merge

namespace detail
{

/*
why not use has_extract to judge whether to use chopsticks or not,
because global extract function is hacing into boost::container,
so has_extract not sure if you have extract.
*/

#if (BOOST_VERSION < 106400)


template<typename K, typename T, typename H, typename P, typename A, typename H2, typename P2>
struct umap_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_map<K, T, H, P, A>,
																boost::unordered_map<K, T, H2, P2, A> >
	: public umap_merger_eti
{
};

/*
boost::unordered_map, boost::unordered_multimap use umap_merger_eei,
because bidirectional conversion ptr_node and grouped_ptr_node need realloc memory
*/

#elif (BOOST_VERSION == 106400)

template<typename K, typename T, typename H, typename P, typename A, typename H2, typename P2>
struct umap_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_map<K, T, H, P, A>,
																boost::unordered_map<K, T, H2, P2, A> >
	: public umap_merger_inner
{
};

template<typename K, typename T, typename H, typename P, typename A, typename H2, typename P2>
struct umap_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_map<K, T, H, P, A>,
																boost::unordered_multimap<K, T, H2, P2, A> >
	: public umap_merger_eei
{
};

#else

template<typename K, typename T, typename H, typename P, typename A, typename H2, typename P2>
struct umap_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_map<K, T, H, P, A>,
																boost::unordered_map<K, T, H2, P2, A> >
	: public umap_merger_inner
{
};

template<typename K, typename T, typename H, typename P, typename A, typename H2, typename P2>
struct umap_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_map<K, T, H, P, A>,
																boost::unordered_multimap<K, T, H2, P2, A> >
	: public umap_merger_inner
{
};


#endif // (BOOST_VERSION < 106400)

template<typename K, typename T, typename H, typename P, typename A,
			typename K2, typename T2, typename H2, typename P2, typename A2>
struct umap_merger_sel_rule_is_same_namespace<boost::unordered_map<K, T, H, P, A>,
												boost::unordered_multimap<K2, T2, H2, P2, A2> >
	: public
		umap_merger_sel_rule_is_different_hash_and_equal_only
		<
			boost::unordered_map<K, T, H, P, A>,
			boost::unordered_multimap<K2, T2, H2, P2, A2>
		>
{
};

} // namespace detail


template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMMap,
			typename K, typename T, typename H, typename P, typename A,
			typename H2, typename P2, typename A2> inline
void merge(boost::unordered_map<K, T, H, P, A>& c,
			BOOST_RV_REF_BEG
				UMapOrUMMap<K, T, H2, P2, A2>
			BOOST_RV_REF_END right)
{
	typedef boost::unordered_map<K, T, H, P, A> left_umap_type;
	typedef UMapOrUMMap<K, T, H2, P2, A2> right_umap_type;
	typedef detail::umap_merger_sel<left_umap_type, right_umap_type> merger_type;

	right_umap_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMMap,
			typename K, typename T, typename H, typename P, typename A,
			typename H2, typename P2, typename A2> inline
void merge(boost::unordered_map<K, T, H, P, A>& c, UMapOrUMMap<K, T, H2, P2, A2>& right)
{
	typedef boost::unordered_map<K, T, H, P, A> left_umap_type;
	typedef UMapOrUMMap<K, T, H2, P2, A2> right_umap_type;
	typedef detail::umap_merger_sel<left_umap_type, right_umap_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename boost::enable_if<detail::has_any_contains< boost::unordered_map<K, T, H, P, A> >, bool>::type
	contains(const boost::unordered_map<K, T, H, P, A>& c, const Key& key)
{
	return c.contains(key);
}

template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename boost::disable_if<detail::has_any_contains< boost::unordered_map<K, T, H, P, A> >, bool>::type
	contains(const boost::unordered_map<K, T, H, P, A>& c, const Key& key)
{
	return c.find(key) != c.end();
}

// try_emplace
#	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#		if ((!(BOOST_VERSION < 106400)) && defined(YGGR_FIX_BOOST_UNORDERED_TRY_EMPLACE_HINT_NOT_MOVE_KEY))
			template<typename K, typename T, typename H, typename P, typename A, typename ...Args> inline
			std::pair<typename boost::unordered_map<K, T, H, P, A>::iterator, bool>
				try_emplace(boost::unordered_map<K, T, H, P, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
			{
				std::pair<typename boost::unordered_map<K, T, H, P, A>::iterator, bool> ret =
					detail::try_emplace_impl(c, boost::move(key), boost::forward<Args>(args)...);

				if(ret.second)
				{
					K tmp = ::boost::move(key);
					tmp = tmp; // disable gcc unused-variable
				}
				return ret;
			}

			template<typename K, typename T, typename H, typename P, typename A, typename ...Args> inline
			typename boost::unordered_map<K, T, H, P, A>::iterator
				try_emplace(boost::unordered_map<K, T, H, P, A>& c,
							typename boost::unordered_map<K, T, H, P, A>::const_iterator hint,
							BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
			{
				typename boost::unordered_map<K, T, H, P, A>::iterator ret = c.find(key);
				if(ret != c.end())
				{
					return ret;
				}
				else
				{
					K tmp_key = ::boost::move(key);
					return detail::try_emplace_impl(c, hint, boost::move(tmp_key), boost::forward<Args>(args)...);
				}
			}
#		else
			template<typename K, typename T, typename H, typename P, typename A, typename ...Args> inline
			std::pair<typename boost::unordered_map<K, T, H, P, A>::iterator, bool>
				try_emplace(boost::unordered_map<K, T, H, P, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
			{
				return detail::try_emplace_impl(c, boost::move(key), boost::forward<Args>(args)...);
			}

			template<typename K, typename T, typename H, typename P, typename A, typename ...Args> inline
			typename boost::unordered_map<K, T, H, P, A>::iterator
				try_emplace(boost::unordered_map<K, T, H, P, A>& c,
							typename boost::unordered_map<K, T, H, P, A>::const_iterator hint,
							BOOST_RV_REF(K) key, BOOST_FWD_REF(Args)... args)
			{
				return detail::try_emplace_impl(c, hint, boost::move(key), boost::forward<Args>(args)...);
			}
#		endif // ((!(BOOST_VERSION < 106400)) && defined(YGGR_FIX_BOOST_UNORDERED_TRY_EMPLACE_HINT_NOT_MOVE_KEY))

	template<typename K, typename T, typename H, typename P, typename A, typename ...Args> inline
	std::pair<typename boost::unordered_map<K, T, H, P, A>::iterator, bool>
		try_emplace(boost::unordered_map<K, T, H, P, A>& c, const K& key, BOOST_FWD_REF(Args)... args)
	{
		return detail::try_emplace_impl(c, key, boost::forward<Args>(args)...);
	}

	template<typename K, typename T, typename H, typename P, typename A, typename ...Args> inline
	typename boost::unordered_map<K, T, H, P, A>::iterator
		try_emplace(boost::unordered_map<K, T, H, P, A>& c,
					typename boost::unordered_map<K, T, H, P, A>::const_iterator hint,
					const K& key, BOOST_FWD_REF(Args)... args)
	{
		return detail::try_emplace_impl(c, hint, key, boost::forward<Args>(args)...);
	}

#	else

#		if ((!(BOOST_VERSION < 106400)) && defined(YGGR_FIX_BOOST_UNORDERED_TRY_EMPLACE_HINT_NOT_MOVE_KEY))
#			define YGGR_TMP_PP_EMPLACE_DEF_EINS(__n__) \
				template<typename K, typename T, typename H, typename P, typename A \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				std::pair<typename boost::unordered_map<K, T, H, P, A>::iterator, bool> \
					try_emplace(boost::unordered_map<K, T, H, P, A>& c, BOOST_RV_REF(K) key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					std::pair<typename boost::unordered_map<K, T, H, P, A>::iterator, bool> ret = \
						detail::try_emplace_impl( \
							c, boost::move(key) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
					if(ret.second) { K tmp = ::boost::move(key); } \
					return ret; } \
				\
				template<typename K, typename T, typename H, typename P, typename A \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				typename boost::unordered_map<K, T, H, P, A>::iterator \
					try_emplace(boost::unordered_map<K, T, H, P, A>& c, \
								typename boost::unordered_map<K, T, H, P, A>::const_iterator hint, \
								BOOST_RV_REF(K) key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				typename boost::unordered_map<K, T, H, P, A>::iterator ret = c.find(key); \
				if(ret != c.end()) { return ret; } \
				else { \
					K tmp_key = ::boost::move(key); \
					return \
						detail::try_emplace_impl( \
							c, hint, boost::move(tmp_key) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } }
#		else

#			define YGGR_TMP_PP_EMPLACE_DEF_EINS(__n__) \
				template<typename K, typename T, typename H, typename P, typename A \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				std::pair<typename boost::unordered_map<K, T, H, P, A>::iterator, bool> \
					try_emplace(boost::unordered_map<K, T, H, P, A>& c, BOOST_RV_REF(K) key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					return \
						detail::try_emplace_impl( \
							c, boost::move(key) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
				\
				template<typename K, typename T, typename H, typename P, typename A \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				typename boost::unordered_map<K, T, H, P, A>::iterator \
					try_emplace(boost::unordered_map<K, T, H, P, A>& c, \
								typename boost::unordered_map<K, T, H, P, A>::const_iterator hint, \
								BOOST_RV_REF(K) key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					return \
						detail::try_emplace_impl( \
							c, hint, boost::move(key) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }
#		endif // ((!(BOOST_VERSION < 106400)) && defined(YGGR_FIX_BOOST_UNORDERED_TRY_EMPLACE_HINT_NOT_MOVE_KEY))

#		define YGGR_TMP_PP_EMPLACE_DEF_ZWEI(__n__) \
			template<typename K, typename T, typename H, typename P, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::unordered_map<K, T, H, P, A>::iterator, bool> \
				try_emplace(boost::unordered_map<K, T, H, P, A>& c, const K& key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						c, key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename K, typename T, typename H, typename P, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::unordered_map<K, T, H, P, A>::iterator \
				try_emplace(boost::unordered_map<K, T, H, P, A>& c, \
							typename boost::unordered_map<K, T, H, P, A>::const_iterator hint, \
							const K& key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return \
					detail::try_emplace_impl( \
						c, hint, key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }


#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF_EINS( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF_ZWEI( __n__ )

#		define YGGR_PP_FOO_ARG_NAME() init_arg

#		define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#		include BOOST_PP_LOCAL_ITERATE()

#		undef YGGR_PP_FOO_ARG_NAME
#		undef YGGR_TMP_PP_EMPLACE_DEF

#	endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// insert_or_assign
template<typename K, typename T, typename H, typename P, typename A, typename M> inline
std::pair<typename boost::unordered_map<K, T, H, P, A>::iterator, bool>
	insert_or_assign(boost::unordered_map<K, T, H, P, A>& c, BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename H, typename P, typename A, typename M> inline
std::pair<typename boost::unordered_map<K, T, H, P, A>::iterator, bool>
	insert_or_assign(boost::unordered_map<K, T, H, P, A>& c, const K& key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, key, boost::forward<M>(m));
}

template<typename K, typename T, typename H, typename P, typename A, typename M> inline
typename boost::unordered_map<K, T, H, P, A>::iterator
	insert_or_assign(boost::unordered_map<K, T, H, P, A>& c,
					typename boost::unordered_map<K, T, H, P, A>::const_iterator hint,
					BOOST_RV_REF(K) key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, hint, boost::move(key), boost::forward<M>(m));
}

template<typename K, typename T, typename H, typename P, typename A, typename M> inline
typename boost::unordered_map<K, T, H, P, A>::iterator
	insert_or_assign(boost::unordered_map<K, T, H, P, A>& c,
						typename boost::unordered_map<K, T, H, P, A>::const_iterator hint,
						const K& key, BOOST_FWD_REF(M) m)
{
	return detail::insert_or_assign_impl(c, hint, key, boost::forward<M>(m));
}

// erase_if
template<typename K, typename T, typename H, typename P, typename A, typename Cmper> inline
typename boost::unordered_map<K, T, H, P, A>::size_type
    erase_if(boost::unordered_map<K, T, H, P, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

} // namespace container
} // namespace yggr

// -----------------multimap---------------
namespace yggr
{
namespace container
{

// std
//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)

	template<typename K, typename T, typename H, typename C, typename A, typename ...Args> inline
	typename std::unordered_multimap<K, T, H, C, A>::iterator
		emplace(std::unordered_multimap<K, T, H, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}


	template<typename K, typename T, typename H, typename C, typename A, typename ...Args> inline
	typename std::unordered_multimap<K, T, H, C, A>::iterator
		emplace_hint(std::unordered_multimap<K, T, H, C, A>& c,
						typename std::unordered_multimap<K, T, H, C, A>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#	endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

#else

#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			template<typename K, typename T, typename H, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename std::unordered_multimap<K, T, H, C, A>::iterator \
				emplace(std::unordered_multimap<K, T, H, C, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename K, typename T, typename H, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename std::unordered_multimap<K, T, H, C, A>::iterator \
				emplace_hint(std::unordered_multimap<K, T, H, C, A>& c, \
								typename std::unordered_multimap<K, T, H, C, A>::const_iterator pos \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#		define YGGR_PP_FOO_ARG_NAME() init_arg

#		define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#		include BOOST_PP_LOCAL_ITERATE()

#		undef YGGR_PP_FOO_ARG_NAME
#		undef YGGR_TMP_PP_EMPLACE_DEF

#	endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)

// extract
template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			detail::native_extractable<std::unordered_multimap<K, T, H, P, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename std::unordered_multimap<K, T, H, P, A>::node_type
	>::type
	extract(std::unordered_multimap<K, T, H, P, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<typename K, typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename
	boost::enable_if
	<
		detail::native_extractable<std::unordered_multimap<K, T, H, P, A> >,
		typename std::unordered_multimap<K, T, H, P, A>::node_type
	>::type
	extract(std::unordered_multimap<K, T, H, P, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// node_insert
template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable<std::unordered_multimap<K, T, H, P, A> >,
		typename std::unordered_multimap<K, T, H, P, A>::iterator
	>::type
	insert(std::unordered_multimap<K, T, H, P, A>& c,
				BOOST_RV_REF_BEG
					typename std::unordered_multimap<K, T, H, P, A>::node_type
				BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable<std::unordered_multimap<K, T, H, P, A> >,
		typename std::unordered_multimap<K, T, H, P, A>::iterator
	>::type
	insert(std::unordered_multimap<K, T, H, P, A>& c,
				typename std::unordered_multimap<K, T, H, P, A>::const_iterator hint,
				BOOST_RV_REF_BEG
					typename std::unordered_multimap<K, T, H, P, A>::node_type
				BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// merge

namespace detail
{

template<typename K, typename T, typename H, typename P, typename A,
			typename K2, typename T2, typename H2, typename P2, typename A2>
struct umap_merger_sel_rule_is_same_namespace<std::unordered_multimap<K, T, H, P, A>,
												std::unordered_map<K2, T2, H2, P2, A2> >
	: public
		umap_merger_sel_rule_is_different_hash_and_equal_only
		<
			std::unordered_multimap<K, T, H, P, A>,
			std::unordered_map<K2, T2, H2, P2, A2>
		>
{
};

} // namespace detail

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMMap,
			typename K, typename T, typename H, typename P, typename A,
			typename H2, typename P2, typename A2> inline
void merge(std::unordered_multimap<K, T, H, P, A>& c,
			BOOST_RV_REF_BEG
				UMapOrUMMap<K, T, H2, P2, A2>
			BOOST_RV_REF_END right)
{
	typedef std::unordered_multimap<K, T, H, P, A> left_umap_type;
	typedef UMapOrUMMap<K, T, H2, P2, A2> right_umap_type;
	typedef detail::umap_merger_sel<left_umap_type, right_umap_type> merger_type;

	right_umap_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMMap,
			typename K, typename T, typename H, typename P, typename A,
			typename H2, typename P2, typename A2> inline
void merge(std::unordered_multimap<K, T, H, P, A>& c, UMapOrUMMap<K, T, H2, P2, A2>& right)
{
	typedef std::unordered_multimap<K, T, H, P, A> left_umap_type;
	typedef UMapOrUMMap<K, T, H2, P2, A2> right_umap_type;
	typedef detail::umap_merger_sel<left_umap_type, right_umap_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename boost::enable_if<detail::has_any_contains< std::unordered_multimap<K, T, H, P, A> >, bool>::type
	contains(const std::unordered_multimap<K, T, H, P, A>& c, const Key& key)
{
	return c.contains(key);
}

template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename boost::disable_if<detail::has_any_contains< std::unordered_multimap<K, T, H, P, A> >, bool>::type
	contains(const std::unordered_multimap<K, T, H, P, A>& c, const Key& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename K, typename T, typename H, typename P, typename A, typename Cmper> inline
typename std::unordered_multimap<K, T, H, P, A>::size_type
    erase_if(std::unordered_multimap<K, T, H, P, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

// boost
//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename K, typename T, typename H, typename C, typename A, typename ...Args> inline
typename boost::unordered_multimap<K, T, H, C, A>::iterator
	emplace(boost::unordered_multimap<K, T, H, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename K, typename T, typename H, typename C, typename A, typename ...Args> inline
typename boost::unordered_multimap<K, T, H, C, A>::iterator
	emplace_hint(boost::unordered_multimap<K, T, H, C, A>& c,
					typename boost::unordered_multimap<K, T, H, C, A>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename K, typename T, typename H, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::unordered_multimap<K, T, H, C, A>::iterator \
			emplace(boost::unordered_multimap<K, T, H, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename K, typename T, typename H, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::unordered_multimap<K, T, H, C, A>::iterator \
			emplace_hint(boost::unordered_multimap<K, T, H, C, A>& c, \
							typename boost::unordered_multimap<K, T, H, C, A>::const_iterator pos \
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

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// extract
template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename node_handle_traits<boost::unordered_multimap<K, T, H, P, A> >::node_type
	>::type
	extract(boost::unordered_multimap<K, T, H, P, A>& c, BOOST_RV_REF(Key) key)
{
	return detail::umultimap_node_extract(c, boost::move(key));
}

template<typename K, typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename node_handle_traits<boost::unordered_multimap<K, T, H, P, A> >::node_type
	extract(boost::unordered_multimap<K, T, H, P, A>& c, const KeyOrIter& key_or_iter)
{
	return detail::umultimap_node_extract(c, key_or_iter);
}

// node_insert
template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable<boost::unordered_multimap<K, T, H, P, A> >,
		typename node_handle_traits<boost::unordered_multimap<K, T, H, P, A> >::insert_return_type
	>::type
	insert(boost::unordered_multimap<K, T, H, P, A>& c,
			BOOST_RV_REF_BEG
				typename node_handle_traits<boost::unordered_multimap<K, T, H, P, A> >::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::disable_if
	<
		detail::native_node_insertable<boost::unordered_multimap<K, T, H, P, A> >,
		typename node_handle_traits<boost::unordered_multimap<K, T, H, P, A> >::insert_return_type
	>::type
	insert(boost::unordered_multimap<K, T, H, P, A>& c,
			BOOST_RV_REF_BEG
				typename node_handle_traits<boost::unordered_multimap<K, T, H, P, A> >::node_type
			BOOST_RV_REF_END node)
{
	return detail::umultimap_node_insert(c, boost::move(node));
}

template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::enable_if
	<
		detail::native_node_insertable<boost::unordered_multimap<K, T, H, P, A> >,
		typename boost::unordered_multimap<K, T, H, P, A>::iterator
	>::type
	insert(boost::unordered_multimap<K, T, H, P, A>& c,
			typename boost::unordered_multimap<K, T, H, P, A>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename node_handle_traits<boost::unordered_multimap<K, T, H, P, A> >::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

template<typename K, typename T, typename H, typename P, typename A> inline
typename
	boost::disable_if
	<
		detail::native_node_insertable<boost::unordered_multimap<K, T, H, P, A> >,
		typename boost::unordered_multimap<K, T, H, P, A>::iterator
	>::type
	insert(boost::unordered_multimap<K, T, H, P, A>& c,
				typename boost::unordered_multimap<K, T, H, P, A>::const_iterator hint,
				BOOST_RV_REF_BEG
					typename node_handle_traits<boost::unordered_multimap<K, T, H, P, A> >::node_type
				BOOST_RV_REF_END node)
{
	return detail::umultimap_node_insert(c, hint, boost::move(node));
}

//merge

namespace detail
{

/*
why not use has_extract to judge whether to use chopsticks or not,
because global extract function is hacing into boost::container,
so has_extract not sure if you have extract.
*/

#if (BOOST_VERSION < 106400)


template<typename K, typename T, typename H, typename P, typename A, typename H2, typename P2>
struct umap_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_multimap<K, T, H, P, A>,
																boost::unordered_multimap<K, T, H2, P2, A> >
	: public umap_merger_eti
{
};

/*
boost::unordered_map, boost::unordered_multimap use umap_merger_eei,
because bidirectional conversion ptr_node and grouped_ptr_node need realloc memory
*/

#elif (BOOST_VERSION == 106400)

template<typename K, typename T, typename H, typename P, typename A, typename H2, typename P2>
struct umap_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_multimap<K, T, H, P, A>,
																boost::unordered_multimap<K, T, H2, P2, A> >
	: public umap_merger_inner
{
};

template<typename K, typename T, typename H, typename P, typename A, typename H2, typename P2>
struct umap_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_multimap<K, T, H, P, A>,
																boost::unordered_map<K, T, H2, P2, A> >
	: public umap_merger_eei
{
};

#else

template<typename K, typename T, typename H, typename P, typename A, typename H2, typename P2>
struct umap_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_multimap<K, T, H, P, A>,
																boost::unordered_multimap<K, T, H2, P2, A> >
	: public umap_merger_inner
{
};

template<typename K, typename T, typename H, typename P, typename A, typename H2, typename P2>
struct umap_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_multimap<K, T, H, P, A>,
																boost::unordered_map<K, T, H2, P2, A> >
	: public umap_merger_inner
{
};


#endif // (BOOST_VERSION < 106400)

template<typename K, typename T, typename H, typename P, typename A,
			typename K2, typename T2, typename H2, typename P2, typename A2>
struct umap_merger_sel_rule_is_same_namespace<boost::unordered_multimap<K, T, H, P, A>,
												boost::unordered_map<K2, T2, H2, P2, A2> >
	: public
		umap_merger_sel_rule_is_different_hash_and_equal_only
		<
			boost::unordered_multimap<K, T, H, P, A>,
			boost::unordered_map<K2, T2, H2, P2, A2>
		>
{
};

} // namespace detail


template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMMap,
			typename K, typename T, typename H, typename P, typename A,
			typename H2, typename P2, typename A2> inline
void merge(boost::unordered_multimap<K, T, H, P, A>& c,
			BOOST_RV_REF_BEG
				UMapOrUMMap<K, T, H2, P2, A2>
			BOOST_RV_REF_END right)
{
	typedef boost::unordered_multimap<K, T, H, P, A> left_umap_type;
	typedef UMapOrUMMap<K, T, H2, P2, A2> right_umap_type;
	typedef detail::umap_merger_sel<left_umap_type, right_umap_type> merger_type;

	right_umap_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMMap,
			typename K, typename T, typename H, typename P, typename A,
			typename H2, typename P2, typename A2> inline
void merge(boost::unordered_multimap<K, T, H, P, A>& c, UMapOrUMMap<K, T, H2, P2, A2>& right)
{
	typedef boost::unordered_multimap<K, T, H, P, A> left_umap_type;
	typedef UMapOrUMMap<K, T, H2, P2, A2> right_umap_type;
	typedef detail::umap_merger_sel<left_umap_type, right_umap_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename boost::enable_if<detail::has_any_contains< boost::unordered_multimap<K, T, H, P, A> >, bool>::type
	contains(const boost::unordered_multimap<K, T, H, P, A>& c, const Key& key)
{
	return c.contains(key);
}

template<typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename boost::disable_if<detail::has_any_contains< boost::unordered_multimap<K, T, H, P, A> >, bool>::type
	contains(const boost::unordered_multimap<K, T, H, P, A>& c, const Key& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename K, typename T, typename H, typename P, typename A, typename Cmper> inline
typename boost::unordered_multimap<K, T, H, P, A>::size_type
    erase_if(boost::unordered_multimap<K, T, H, P, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}


} // namespace container
} // namespace yggr

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)

namespace std
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(5, std::unordered_map)
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(5, std::unordered_multimap)

} // namespace std

#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

namespace boost
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(5, boost::unordered_map)
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(5, boost::unordered_multimap)

} // namespace boost

#endif // __YGGR_CONTAINER_UNORDERED_MAP_HPP__
