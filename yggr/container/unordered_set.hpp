//unordered_set.hpp

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

#ifndef __YGGR_CONTAINER_UNORDERED_SET_HPP__
#define __YGGR_CONTAINER_UNORDERED_SET_HPP__

#include <yggr/container/container_selector.hpp>
#include <yggr/move/move.hpp>

#include <yggr/mplex/typename_expand.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>

#include <yggr/container/basic_function_adapter.hpp>
#include <yggr/container/node_handle_traits.hpp>

#include <yggr/container/detail/is_xxx_container.hpp>
#include <yggr/container/detail/is_s_or_m_container.hpp>
#include <yggr/container/detail/range_size_def.hpp>

#include <yggr/container/detail/has_merge.hpp>
#include <yggr/container/detail/has_contains.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_uset.hpp>

#include <yggr/container/detail/uset_merge_impl.hpp>
#include <yggr/container/detail/erase_impl.hpp>
#include <yggr/container/detail/uset_merger_sel.hpp>

#include <boost/unordered_set.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
#	include <unordered_set>
#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

namespace yggr
{
namespace container
{

#if defined YGGR_USE_BOOST_CONTAINER
	using boost::unordered_set;
	using boost::unordered_multiset;
#elif defined YGGR_USE_STL_CONTAINER
#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
		using std::unordered_set;
		using std::unordered_multiset;
#	else
		using boost::unordered_set;
		using boost::unordered_multiset;
#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
#else
#	error "use unordered_set type unknown error. please check source"
#endif //YGGR_USE_BOOST_CONTAINER

} // namespace container

using container::unordered_set;
using container::unordered_multiset;

} // namespace yggr

#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET

YGGR_PP_IS_XXX_CONTAINER_DEF(4, std_unordered_set, std::unordered_set);
YGGR_PP_IS_XXX_CONTAINER_DEF(4, std_unordered_multiset, std::unordered_multiset);

#else

namespace yggr
{
namespace container
{

template<typename Container>
struct is_std_unordered_set
	: public boost::mpl::false_
{
};

template<typename Container>
struct is_std_unordered_multiset
	: public boost::mpl::false_
{
};

} // namespace container
} // namespace yggr

#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

YGGR_PP_IS_XXX_CONTAINER_DEF(4, boost_unordered_set, boost::unordered_set);
YGGR_PP_IS_XXX_CONTAINER_DEF(4, boost_unordered_multiset, boost::unordered_multiset);

// compatibility caller
// unordered_container not need compatibility caller,
// because boost is same stl

namespace yggr
{
namespace container
{

//std
// emplace 

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

	template<typename T, typename H, typename C, typename A, typename ...Args> inline
	std::pair< typename std::unordered_set<T, H, C, A>::iterator, bool>
		emplace(std::unordered_set<T, H, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}

	template<typename T, typename H, typename C, typename A, typename ...Args> inline
	typename std::unordered_set<T, H, C, A>::iterator
		emplace_hint(std::unordered_set<T, H, C, A>& c,
						typename std::unordered_set<T, H, C, A>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

#else

#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			template<typename T, typename H, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename std::unordered_set<T, H, C, A>::iterator, bool> \
				emplace(std::unordered_set<T, H, C, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename T, typename H, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename std::unordered_set<T, H, C, A>::iterator \
				emplace_hint(std::unordered_set<T, H, C, A>& c, \
								typename std::unordered_set<T, H, C, A>::const_iterator pos \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); }

#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#		define YGGR_PP_FOO_ARG_NAME() init_arg

#		define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#		include BOOST_PP_LOCAL_ITERATE()

#		undef YGGR_PP_FOO_ARG_NAME
#		undef YGGR_TMP_PP_EMPLACE_DEF

#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

// unordered_set
// extract
template<typename T, typename H, typename P, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			detail::native_extractable<std::unordered_set<T, H, P, A> >,
			boost::mpl::not_<iterator_ex::is_iterator<Key> >
		>,
		typename std::unordered_set<T, H, P, A>::node_type
	>::type
	extract(std::unordered_set<T, H, P, A>& s, BOOST_RV_REF(Key) key)
{
	return s.extract(boost::move(key));
}

template<typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename 
	boost::enable_if
	<
		detail::native_extractable<std::unordered_set<T, H, P, A> >,
		typename std::unordered_set<T, H, P, A>::node_type
	>::type
	extract(std::unordered_set<T, H, P, A>& s, const KeyOrIter& key_or_iter)
{
	return s.extract(key_or_iter);
}

// node_insert
template<typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable<std::unordered_set<T, H, P, A> >,
		typename std::unordered_set<T, H, P, A>::insert_return_type
	>::type
	insert(std::unordered_set<T, H, P, A>& s,
				BOOST_RV_REF_BEG
					typename std::unordered_set<T, H, P, A>::node_type
				BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable<std::unordered_set<T, H, P, A> >,
		typename std::unordered_set<T, H, P, A>::iterator
	>::type
	insert(std::unordered_set<T, H, P, A>& s,
				typename std::unordered_set<T, H, P, A>::const_iterator hint,
				BOOST_RV_REF_BEG
					typename std::unordered_set<T, H, P, A>::node_type
				BOOST_RV_REF_END node)
{
	return s.insert(hint, boost::move(node));
}

// merge

namespace detail
{

template<typename T, typename H, typename P, typename A, 
			typename T2, typename H2, typename P2, typename A2>	
struct uset_merger_sel_rule_is_same_namespace<std::unordered_set<T, H, P, A>, 
												std::unordered_multiset<T2, H2, P2, A2> >
	: public 
		uset_merger_sel_rule_is_different_hash_and_equal_only
		<
			std::unordered_set<T, H, P, A>, 
			std::unordered_multiset<T2, H2, P2, A2>
		>
{
};

} // namespace detail

template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMSet,
			typename T, typename H, typename P, typename A, 
			typename H2, typename P2, typename A2> inline
void merge(std::unordered_set<T, H, P, A>& c, 
			BOOST_RV_REF_BEG
				USetOrUMSet<T, H2, P2, A2> 
			BOOST_RV_REF_END right)
{
	typedef std::unordered_set<T, H, P, A> left_uset_type;
	typedef USetOrUMSet<T, H2, P2, A2> right_uset_type;
	typedef detail::uset_merger_sel<left_uset_type, right_uset_type> merger_type;

	right_uset_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMSet,
			typename T, typename H, typename P, typename A, 
			typename H2, typename P2, typename A2> inline
void merge(std::unordered_set<T, H, P, A>& c, USetOrUMSet<T, H2, P2, A2>& right)
{
	typedef std::unordered_set<T, H, P, A> left_uset_type;
	typedef USetOrUMSet<T, H2, P2, A2> right_uset_type;
	typedef detail::uset_merger_sel<left_uset_type, right_uset_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename T, typename H, typename P, typename A, typename K> inline
typename boost::enable_if<detail::has_any_contains< std::unordered_set<T, H, P, A> >, bool>::type
	contains(const std::unordered_set<T, H, P, A>& c, const K& key)
{
	return c.contains(key);
}

template<typename T, typename H, typename P, typename A, typename K> inline
typename boost::disable_if<detail::has_any_contains< std::unordered_set<T, H, P, A> >, bool>::type
	contains(const std::unordered_set<T, H, P, A>& c, const K& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename T, typename H, typename P, typename A, typename Cmper> inline
typename std::unordered_set<T, H, P, A>::size_type
    erase_if(std::unordered_set<T, H, P, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#endif // (YGGR_NO_CXX11_HDR_UNORDERED_SET)

// boost
//emplace 

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename H, typename C, typename A, typename ...Args> inline
std::pair<typename boost::unordered_set<T, H, C, A>::iterator, bool>
	emplace(boost::unordered_set<T, H, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename H, typename C, typename A, typename ...Args> inline
typename boost::unordered_set<T, H, C, A>::iterator
	emplace_hint(boost::unordered_set<T, H, C, A>& c,
					typename boost::unordered_set<T, H, C, A>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename H, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		std::pair<typename boost::unordered_set<T, H, C, A>::iterator, bool> \
			emplace(boost::unordered_set<T, H, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename H, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::unordered_set<T, H, C, A>::iterator \
			emplace_hint(boost::unordered_set<T, H, C, A>& c, \
							typename boost::unordered_set<T, H, C, A>::const_iterator pos \
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

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// extract
template<typename T, typename H, typename P, typename A, typename Key> inline
typename 
	boost::disable_if
	<
		iterator_ex::is_iterator<Key>,
		typename node_handle_traits<boost::unordered_set<T, H, P, A> >::node_type
	>::type
	extract(boost::unordered_set<T, H, P, A>& s, BOOST_RV_REF(Key) key)
{
	return detail::uset_node_extract(s, boost::move(key));
}

template<typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename node_handle_traits<boost::unordered_set<T, H, P, A> >::node_type
	extract(boost::unordered_set<T, H, P, A>& s, const KeyOrIter& key_or_iter)
{
	return detail::uset_node_extract(s, key_or_iter);
}

// node_insert
template<typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable<boost::unordered_set<T, H, P, A> >,
		typename node_handle_traits<boost::unordered_set<T, H, P, A> >::insert_return_type
	>::type
	insert(boost::unordered_set<T, H, P, A>& s,
				BOOST_RV_REF_BEG
					typename node_handle_traits<boost::unordered_set<T, H, P, A> >::node_type
				BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<typename T, typename H, typename P, typename A> inline
typename 
	boost::disable_if
	<
		detail::native_node_insertable<boost::unordered_set<T, H, P, A> >,
		typename node_handle_traits<boost::unordered_set<T, H, P, A> >::insert_return_type
	>::type
	insert(boost::unordered_set<T, H, P, A>& s,
				BOOST_RV_REF_BEG
					typename node_handle_traits<boost::unordered_set<T, H, P, A> >::node_type
				BOOST_RV_REF_END node)
{
	return detail::uset_node_insert(s, boost::move(node));
}

template<typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable<boost::unordered_set<T, H, P, A> >,
		typename boost::unordered_set<T, H, P, A>::iterator
	>::type
	insert(boost::unordered_set<T, H, P, A>& s,
				typename boost::unordered_set<T, H, P, A>::const_iterator hint,
				BOOST_RV_REF_BEG
					typename node_handle_traits<boost::unordered_set<T, H, P, A> >::node_type
				BOOST_RV_REF_END node)
{
	return s.insert(hint, boost::move(node));
}

template<typename T, typename H, typename P, typename A> inline
typename 
	boost::disable_if
	<
		detail::native_node_insertable<boost::unordered_set<T, H, P, A> >,
		typename boost::unordered_set<T, H, P, A>::iterator
	>::type
	insert(boost::unordered_set<T, H, P, A>& s,
				typename boost::unordered_set<T, H, P, A>::const_iterator hint,
				BOOST_RV_REF_BEG
					typename node_handle_traits<boost::unordered_set<T, H, P, A> >::node_type
				BOOST_RV_REF_END node)
{
	return detail::uset_node_insert(s, hint, boost::move(node));
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


template<typename T, typename H, typename P, typename A, typename H2, typename P2>	
struct uset_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_set<T, H, P, A>, 
																boost::unordered_set<T, H2, P2, A> >
	: public uset_merger_eti
{
};

/*
boost::unordered_set, boost::unordered_multiset use uset_merger_eei,
because bidirectional conversion ptr_node and grouped_ptr_node need realloc memory
*/

#elif (BOOST_VERSION == 106400)

template<typename T, typename H, typename P, typename A, typename H2, typename P2>	
struct uset_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_set<T, H, P, A>, 
																boost::unordered_set<T, H2, P2, A> >
	: public uset_merger_inner
{
};

template<typename T, typename H, typename P, typename A, typename H2, typename P2>	
struct uset_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_set<T, H, P, A>, 
																boost::unordered_multiset<T, H2, P2, A> >
	: public uset_merger_eei
{
};

#else

template<typename T, typename H, typename P, typename A, typename H2, typename P2>	
struct uset_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_set<T, H, P, A>, 
																boost::unordered_set<T, H2, P2, A> >
	: public uset_merger_inner
{
};

template<typename T, typename H, typename P, typename A, typename H2, typename P2>	
struct uset_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_set<T, H, P, A>, 
																boost::unordered_multiset<T, H2, P2, A> >
	: public uset_merger_inner
{
};


#endif // (BOOST_VERSION < 106400)

template<typename T, typename H, typename P, typename A, 
			typename T2, typename H2, typename P2, typename A2>	
struct uset_merger_sel_rule_is_same_namespace<boost::unordered_set<T, H, P, A>, 
												boost::unordered_multiset<T2, H2, P2, A2> >
	: public 
		uset_merger_sel_rule_is_different_hash_and_equal_only
		<
			boost::unordered_set<T, H, P, A>, 
			boost::unordered_multiset<T2, H2, P2, A2>
		>
{
};

} // namespace detail


template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMSet,
			typename T, typename H, typename P, typename A, 
			typename H2, typename P2, typename A2> inline
void merge(boost::unordered_set<T, H, P, A>& c, 
			BOOST_RV_REF_BEG
				USetOrUMSet<T, H2, P2, A2> 
			BOOST_RV_REF_END right)
{
	typedef boost::unordered_set<T, H, P, A> left_uset_type;
	typedef USetOrUMSet<T, H2, P2, A2> right_uset_type;
	typedef detail::uset_merger_sel<left_uset_type, right_uset_type> merger_type;

	right_uset_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMSet,
			typename T, typename H, typename P, typename A, 
			typename H2, typename P2, typename A2> inline
void merge(boost::unordered_set<T, H, P, A>& c, USetOrUMSet<T, H2, P2, A2>& right)
{
	typedef boost::unordered_set<T, H, P, A> left_uset_type;
	typedef USetOrUMSet<T, H2, P2, A2> right_uset_type;
	typedef detail::uset_merger_sel<left_uset_type, right_uset_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename T, typename H, typename P, typename A, typename K> inline
typename boost::enable_if<detail::has_any_contains< boost::unordered_set<T, H, P, A> >, bool>::type
	contains(const boost::unordered_set<T, H, P, A>& c, const K& key)
{
	return c.contains(key);
}

template<typename T, typename H, typename P, typename A, typename K> inline
typename boost::disable_if<detail::has_any_contains< boost::unordered_set<T, H, P, A> >, bool>::type
	contains(const boost::unordered_set<T, H, P, A>& c, const K& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename T, typename H, typename P, typename A, typename Cmper> inline
typename boost::unordered_set<T, H, P, A>::size_type
    erase_if(boost::unordered_set<T, H, P, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

} // namespace container
} // namespace yggr

// -----------------multiset---------------
namespace yggr
{
namespace container
{
// std
//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

	template<typename T, typename H, typename C, typename A, typename ...Args> inline
	typename std::unordered_multiset<T, H, C, A>::iterator
		emplace(std::unordered_multiset<T, H, C, A>& c, BOOST_FWD_REF(Args)... args)
	{
		return c.emplace(boost::forward<Args>(args)...);
	}


	template<typename T, typename H, typename C, typename A, typename ...Args> inline
	typename std::unordered_multiset<T, H, C, A>::iterator
		emplace_hint(std::unordered_multiset<T, H, C, A>& c,
						typename std::unordered_multiset<T, H, C, A>::const_iterator pos,
						BOOST_FWD_REF(Args)... args)
	{
		return c.emplace_hint(pos, boost::forward<Args>(args)...);
	}

#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

#else

#	if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			template<typename T, typename H, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename std::unordered_multiset<T, H, C, A>::iterator \
				emplace(std::unordered_multiset<T, H, C, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename T, typename H, typename C, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename std::unordered_multiset<T, H, C, A>::iterator \
				emplace_hint(std::unordered_multiset<T, H, C, A>& c, \
								typename std::unordered_multiset<T, H, C, A>::const_iterator pos \
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

#	endif // YGGR_NO_CXX11_HDR_UNORDERED_SET
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

// extract
template<typename T, typename H, typename P, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			detail::native_extractable<std::unordered_multiset<T, H, P, A> >,
			boost::mpl::not_<iterator_ex::is_iterator<Key> >
		>,
		typename std::unordered_multiset<T, H, P, A>::node_type
	>::type
	extract(std::unordered_multiset<T, H, P, A>& s, BOOST_RV_REF(Key) key)
{
	return s.extract(boost::move(key));
}

template<typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename 
	boost::enable_if
	<
		detail::native_extractable<std::unordered_multiset<T, H, P, A> >,
		typename std::unordered_multiset<T, H, P, A>::node_type
	>::type
	extract(std::unordered_multiset<T, H, P, A>& s, const KeyOrIter& key_or_iter)
{
	return s.extract(key_or_iter);
}

// node_insert
template<typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable<std::unordered_multiset<T, H, P, A> >,
		typename std::unordered_multiset<T, H, P, A>::iterator
	>::type
	insert(std::unordered_multiset<T, H, P, A>& s,
			BOOST_RV_REF_BEG
				typename std::unordered_multiset<T, H, P, A>::node_type
			BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable<std::unordered_multiset<T, H, P, A> >,
		typename std::unordered_multiset<T, H, P, A>::iterator
	>::type
	insert(std::unordered_multiset<T, H, P, A>& s,
			typename std::unordered_multiset<T, H, P, A>::const_iterator hint,
			BOOST_RV_REF_BEG
				typename std::unordered_multiset<T, H, P, A>::node_type
			BOOST_RV_REF_END node)
{
	return s.insert(hint, boost::move(node));
}

// merge

namespace detail
{

template<typename T, typename H, typename P, typename A, 
			typename T2, typename H2, typename P2, typename A2>	
struct uset_merger_sel_rule_is_same_namespace<std::unordered_multiset<T, H, P, A>, 
												std::unordered_set<T2, H2, P2, A2> >
	: public 
		uset_merger_sel_rule_is_different_hash_and_equal_only
		<
			std::unordered_multiset<T, H, P, A>, 
			std::unordered_set<T2, H2, P2, A2>
		>
{
};

} // namespace detail

template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMSet,
			typename T, typename H, typename P, typename A, 
			typename H2, typename P2, typename A2> inline
void merge(std::unordered_multiset<T, H, P, A>& c, 
			BOOST_RV_REF_BEG
				USetOrUMSet<T, H2, P2, A2> 
			BOOST_RV_REF_END right)
{
	typedef std::unordered_multiset<T, H, P, A> left_uset_type;
	typedef USetOrUMSet<T, H2, P2, A2> right_uset_type;
	typedef detail::uset_merger_sel<left_uset_type, right_uset_type> merger_type;

	right_uset_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMSet,
			typename T, typename H, typename P, typename A, 
			typename H2, typename P2, typename A2> inline
void merge(std::unordered_multiset<T, H, P, A>& c, USetOrUMSet<T, H2, P2, A2>& right)
{
	typedef std::unordered_multiset<T, H, P, A> left_uset_type;
	typedef USetOrUMSet<T, H2, P2, A2> right_uset_type;
	typedef detail::uset_merger_sel<left_uset_type, right_uset_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename T, typename H, typename P, typename A, typename K> inline
typename boost::enable_if<detail::has_any_contains< std::unordered_multiset<T, H, P, A> >, bool>::type
	contains(const std::unordered_multiset<T, H, P, A>& c, const K& key)
{
	return c.contains(key);
}

template<typename T, typename H, typename P, typename A, typename K> inline
typename boost::disable_if<detail::has_any_contains< std::unordered_multiset<T, H, P, A> >, bool>::type
	contains(const std::unordered_multiset<T, H, P, A>& c, const K& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename T, typename H, typename P, typename A, typename Cmper> inline
typename std::unordered_multiset<T, H, P, A>::size_type
    erase_if(std::unordered_multiset<T, H, P, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

// boost
//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename H, typename C, typename A, typename ...Args> inline
typename boost::unordered_multiset<T, H, C, A>::iterator
	emplace(boost::unordered_multiset<T, H, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename H, typename C, typename A, typename ...Args> inline
typename boost::unordered_multiset<T, H, C, A>::iterator
	emplace_hint(boost::unordered_multiset<T, H, C, A>& c,
					typename boost::unordered_multiset<T, H, C, A>::const_iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename H, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::unordered_multiset<T, H, C, A>::iterator \
			emplace(boost::unordered_multiset<T, H, C, A>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename H, typename C, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::unordered_multiset<T, H, C, A>::iterator \
			emplace_hint(boost::unordered_multiset<T, H, C, A>& c, \
							typename boost::unordered_multiset<T, H, C, A>::const_iterator pos \
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
template<typename T, typename H, typename P, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::not_< iterator_ex::is_iterator<Key> >,
		typename node_handle_traits<boost::unordered_multiset<T, H, P, A> >::node_type
	>::type
	extract(boost::unordered_multiset<T, H, P, A>& s, BOOST_RV_REF(Key) key)
{
	return detail::umultiset_node_extract(s, boost::move(key));
}

template<typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename node_handle_traits<boost::unordered_multiset<T, H, P, A> >::node_type
	extract(boost::unordered_multiset<T, H, P, A>& s, const KeyOrIter& key_or_iter)
{
	return detail::umultiset_node_extract(s, key_or_iter);
}

// node_insert
template<typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable<boost::unordered_multiset<T, H, P, A> >,
		typename node_handle_traits<boost::unordered_multiset<T, H, P, A> >::insert_return_type
	>::type
	insert(boost::unordered_multiset<T, H, P, A>& s,
				BOOST_RV_REF_BEG
					typename node_handle_traits<boost::unordered_multiset<T, H, P, A> >::node_type
				BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<typename T, typename H, typename P, typename A> inline
typename 
	boost::disable_if
	<
		detail::native_node_insertable<boost::unordered_multiset<T, H, P, A> >,
		typename node_handle_traits<boost::unordered_multiset<T, H, P, A> >::insert_return_type
	>::type
	insert(boost::unordered_multiset<T, H, P, A>& s,
				BOOST_RV_REF_BEG
					typename node_handle_traits<boost::unordered_multiset<T, H, P, A> >::node_type
				BOOST_RV_REF_END node)
{
	return detail::umultiset_node_insert(s, boost::move(node));
}

template<typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		detail::native_node_insertable<boost::unordered_multiset<T, H, P, A> >,
		typename boost::unordered_multiset<T, H, P, A>::iterator
	>::type
	insert(boost::unordered_multiset<T, H, P, A>& s,
				typename boost::unordered_multiset<T, H, P, A>::const_iterator hint,
				BOOST_RV_REF_BEG
					typename node_handle_traits<boost::unordered_multiset<T, H, P, A> >::node_type
				BOOST_RV_REF_END node)
{
	return s.insert(hint, boost::move(node));
}

template<typename T, typename H, typename P, typename A> inline
typename 
	boost::disable_if
	<
		detail::native_node_insertable<boost::unordered_multiset<T, H, P, A> >,
		typename boost::unordered_multiset<T, H, P, A>::iterator
	>::type
	insert(boost::unordered_multiset<T, H, P, A>& s,
				typename boost::unordered_multiset<T, H, P, A>::const_iterator hint,
				BOOST_RV_REF_BEG
					typename node_handle_traits<boost::unordered_multiset<T, H, P, A> >::node_type
				BOOST_RV_REF_END node)
{
	return detail::umultiset_node_insert(s, hint, boost::move(node));
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


template<typename T, typename H, typename P, typename A, typename H2, typename P2>	
struct uset_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_multiset<T, H, P, A>, 
																boost::unordered_multiset<T, H2, P2, A> >
	: public uset_merger_eti
{
};

/*
boost::unordered_set, boost::unordered_multiset use uset_merger_eei,
because bidirectional conversion ptr_node and grouped_ptr_node need realloc memory
*/


#elif (BOOST_VERSION == 106400)

template<typename T, typename H, typename P, typename A, typename H2, typename P2>	
struct uset_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_multiset<T, H, P, A>, 
																boost::unordered_multiset<T, H2, P2, A> >
	: public uset_merger_inner
{
};

template<typename T, typename H, typename P, typename A, typename H2, typename P2>	
struct uset_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_multiset<T, H, P, A>, 
																boost::unordered_set<T, H2, P2, A> >
	: public uset_merger_eei
{
};

#else

template<typename T, typename H, typename P, typename A, typename H2, typename P2>	
struct uset_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_multiset<T, H, P, A>, 
																boost::unordered_multiset<T, H2, P2, A> >
	: public uset_merger_inner
{
};

template<typename T, typename H, typename P, typename A, typename H2, typename P2>	
struct uset_merger_sel_rule_is_different_hash_and_equal_only<boost::unordered_multiset<T, H, P, A>, 
																boost::unordered_set<T, H2, P2, A> >
	: public uset_merger_inner
{
};


#endif // (BOOST_VERSION < 106400)

template<typename T, typename H, typename P, typename A, 
			typename T2, typename H2, typename P2, typename A2>	
struct uset_merger_sel_rule_is_same_namespace<boost::unordered_multiset<T, H, P, A>, 
												boost::unordered_set<T2, H2, P2, A2> >
	: public 
		uset_merger_sel_rule_is_different_hash_and_equal_only
		<
			boost::unordered_multiset<T, H, P, A>, 
			boost::unordered_set<T2, H2, P2, A2>
		>
{
};

} // namespace detail


template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMSet,
			typename T, typename H, typename P, typename A, 
			typename H2, typename P2, typename A2> inline
void merge(boost::unordered_multiset<T, H, P, A>& c, 
			BOOST_RV_REF_BEG
				USetOrUMSet<T, H2, P2, A2> 
			BOOST_RV_REF_END right)
{
	typedef boost::unordered_multiset<T, H, P, A> left_uset_type;
	typedef USetOrUMSet<T, H2, P2, A2> right_uset_type;
	typedef detail::uset_merger_sel<left_uset_type, right_uset_type> merger_type;

	right_uset_type& right_ref = right;
	merger_type merger;
	merger(c, right_ref);
}

template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMSet,
			typename T, typename H, typename P, typename A, 
			typename H2, typename P2, typename A2> inline
void merge(boost::unordered_multiset<T, H, P, A>& c, USetOrUMSet<T, H2, P2, A2>& right)
{
	typedef boost::unordered_multiset<T, H, P, A> left_uset_type;
	typedef USetOrUMSet<T, H2, P2, A2> right_uset_type;
	typedef detail::uset_merger_sel<left_uset_type, right_uset_type> merger_type;

	merger_type merger;
	merger(c, right);
}

// contains
template<typename T, typename H, typename P, typename A, typename K> inline
typename boost::enable_if<detail::has_any_contains< boost::unordered_multiset<T, H, P, A> >, bool>::type
	contains(const boost::unordered_multiset<T, H, P, A>& c, const K& key)
{
	return c.contains(key);
}

template<typename T, typename H, typename P, typename A, typename K> inline
typename boost::disable_if<detail::has_any_contains< boost::unordered_multiset<T, H, P, A> >, bool>::type
	contains(const boost::unordered_multiset<T, H, P, A>& c, const K& key)
{
	return c.find(key) != c.end();
}

// erase_if
template<typename T, typename H, typename P, typename A, typename Cmper> inline
typename boost::unordered_multiset<T, H, P, A>::size_type
    erase_if(boost::unordered_multiset<T, H, P, A>& c, Cmper cmper)
{
	return detail::erase_if_impl(c, cmper);
}

} // namespace container
} // namespace yggr

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

namespace std
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, std::unordered_set)
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, std::unordered_multiset)

} // namespace std

#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

namespace boost
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, boost::unordered_set)
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(4, boost::unordered_multiset)

} // namespace boost

#endif // __YGGR_CONTAINER_UNORDERED_SET_HPP__
