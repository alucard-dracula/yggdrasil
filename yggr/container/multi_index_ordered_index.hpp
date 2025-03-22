//multi_index_ordered_index.hpp

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

#ifndef __YGGR_CONTAINER_MULTI_INDEX_ORDERED_INDEX_HPP__
#define __YGGR_CONTAINER_MULTI_INDEX_ORDERED_INDEX_HPP__

#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/typename_expand.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/multi_index_config.hpp>
#include <yggr/container/node_handle_traits.hpp>

#include <yggr/container/detail/range_size_def.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_midx_ordered_index.hpp>

#include <boost/multi_index/ordered_index.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>


namespace yggr
{
namespace container
{

using boost::multi_index::ordered_unique;
using boost::multi_index::ordered_non_unique;

} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#	if defined(YGGR_BOOST_MULTI_INDEX_HAS_EMPLACE)

#		if defined YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

template<typename K, typename P, 
			typename S, typename T, typename C, typename A,
			typename ...Args> inline
std::pair< typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator, bool >
	emplace(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename K, typename P, 
			typename S, typename T, typename C, typename A,
			typename ...Args> inline
typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator
	emplace_hint(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, 
					typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#		else

template<typename K, typename P, 
			typename S, typename T, typename C,
			typename ...Args> inline
std::pair< typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator, bool >
	emplace(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c, BOOST_FWD_REF(Args)... args)
{
	return c.emplace(boost::forward<Args>(args)...);
}

template<typename K, typename P, 
			typename S, typename T, typename C,
			typename ...Args> inline
typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator
	emplace_hint(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c, 
					typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_hint(pos, boost::forward<Args>(args)...);
}

#		endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

#	else

#		if defined YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX
			
template<typename K, typename P, 
			typename S, typename T, typename C, typename A,
			typename ...Args> inline
std::pair< typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator, bool >
	emplace(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, BOOST_FWD_REF(Args)... args)
{
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C, A> now_index_type;
	typedef typename now_index_type::value_type value_type;
	return c.insert(value_type(boost::forward<Args>(args)...));
}

template<typename K, typename P, 
			typename S, typename T, typename C, typename A, 
			typename ...Args> inline
typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator
	emplace_hint(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, 
					typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C, A> now_index_type;
	typedef typename now_index_type::value_type value_type;
	return c.insert(pos, value_type(boost::forward<Args>(args)...));
}

#		else

template<typename K, typename P, 
			typename S, typename T, typename C,
			typename ...Args> inline
std::pair< typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator, bool >
	emplace(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c, BOOST_FWD_REF(Args)... args)
{
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C> now_index_type;
	typedef typename now_index_type::value_type value_type;
	return c.insert(value_type(boost::forward<Args>(args)...));
}

template<typename K, typename P, 
			typename S, typename T, typename C,
			typename ...Args> inline
typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator
	emplace_hint(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c, 
					typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator pos,
					BOOST_FWD_REF(Args)... args)
{
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C> now_index_type;
	typedef typename now_index_type::value_type value_type;
	return c.insert(pos, value_type(boost::forward<Args>(args)...));
}

#		endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

#	endif // YGGR_BOOST_MULTI_INDEX_HAS_EMPLACE

#else

#	if defined(YGGR_BOOST_MULTI_INDEX_HAS_EMPLACE)
#		if defined YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

#			define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
				template<typename K, typename P, \
							typename S, typename T, typename C, typename A \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				std::pair<typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator, bool> \
					emplace(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
				\
				template<typename K, typename P, \
							typename S, typename T, typename C, typename A \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator \
					emplace_hint(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, \
									typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator pos \
									YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }
#		else

#			define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
				template<typename K, typename P, \
							typename S, typename T, typename C \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				std::pair<typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator, bool> \
					emplace(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					return c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
				\
				template<typename K, typename P, \
							typename S, typename T, typename C \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator \
					emplace_hint(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c, \
									typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator pos \
									YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					return c.emplace_hint(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#		endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

#	else

#		if defined YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

#			define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
				template<typename K, typename P, \
							typename S, typename T, typename C, typename A \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				std::pair<typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator, bool> \
					emplace(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					typedef boost::multi_index::detail::ordered_index<K, P, S, T, C, A> now_index_type; \
					typedef typename now_index_type::value_type value_type; \
					return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
				\
				template<typename K, typename P, \
							typename S, typename T, typename C, typename A \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator \
					emplace_hint(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, \
									typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator pos \
									YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					typedef boost::multi_index::detail::ordered_index<K, P, S, T, C, A> now_index_type; \
					typedef typename now_index_type::value_type value_type; \
					return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#		else

#			define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
				template<typename K, typename P, \
							typename S, typename T, typename C \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				std::pair<typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator, bool> \
					emplace(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					typedef boost::multi_index::detail::ordered_index<K, P, S, T, C> now_index_type; \
					typedef typename now_index_type::value_type value_type; \
					return c.insert(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
				\
				template<typename K, typename P, \
							typename S, typename T, typename C \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
				typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator \
					emplace_hint(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c, \
									typename boost::multi_index::detail::ordered_index<K, P, S, T, C>::iterator pos \
									YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
					typedef boost::multi_index::detail::ordered_index<K, P, S, T, C> now_index_type; \
					typedef typename now_index_type::value_type value_type; \
					return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#		endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

#	endif // YGGR_BOOST_MULTI_INDEX_HAS_EMPLACE

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF


#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#if (BOOST_VERSION < 107400)
#	if defined YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

// extract
template<typename K, typename P, 
			typename S, typename T, typename C, typename A, 
			typename KeyOrIter> inline
typename 
	detail::node_handle_foo_hacking_midx_ordered_index
	<
		K, P,
		typename detail::nth_layer_hacking_cast<S>::type,
		T, C, A
	>::node_type
	extract(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, const KeyOrIter& key_or_iter)
{
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C, A> native_cont_type;
	typedef typename detail::index_hacking_cast<native_cont_type>::type hacking_cont_type;

	return reinterpret_cast<hacking_cont_type&>(c).extract(key_or_iter);
}

// node insert
template<typename K, typename P, typename S, typename T, typename C, typename A> inline
typename 
	detail::node_handle_foo_hacking_midx_ordered_index
	<
		K, P,
		typename detail::nth_layer_hacking_cast<S>::type,
		T, C, A
	>::insert_return_type
	insert(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, 
			BOOST_RV_REF_BEG
				typename 
					detail::node_handle_foo_hacking_midx_ordered_index
					<
						K, P,
						typename detail::nth_layer_hacking_cast<S>::type,
						T, C, A
					>::node_type
			BOOST_RV_REF_END node)
{
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C, A> native_cont_type;
	typedef typename detail::index_hacking_cast<native_cont_type>::type hacking_cont_type;

	return reinterpret_cast<hacking_cont_type&>(c).insert(boost::move(node));
}

template<typename K, typename P, typename S, typename T, typename C, typename A> inline
typename 
	detail::node_handle_foo_hacking_midx_ordered_index
	<
		K, P,
		typename detail::nth_layer_hacking_cast<S>::type,
		T, C, A
	>::iterator
	insert(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, 
			typename 
				boost::multi_index::detail::ordered_index
				<
					K, P, S, 
					T, C, A
				>::const_iterator pos,
			BOOST_RV_REF_BEG
				typename 
					detail::node_handle_foo_hacking_midx_ordered_index
					<
						K, P,
						typename detail::nth_layer_hacking_cast<S>::type,
						T, C, A
					>::node_type
			BOOST_RV_REF_END node)
{
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C, A> native_cont_type;
	typedef typename detail::index_hacking_cast<native_cont_type>::type hacking_cont_type;

	return reinterpret_cast<hacking_cont_type&>(c).insert(pos, boost::move(node));
}

#	else

// extract
template<typename K, typename P, 
			typename S, typename T, typename C, 
			typename KeyOrIter> inline
typename 
	detail::node_handle_foo_hacking_midx_ordered_index
	<
		K, P,
		typename detail::nth_layer_hacking_cast<S>::type,
		T, C
	>::node_type
	extract(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c, const KeyOrIter& key_or_iter)
{
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C> native_cont_type;
	typedef typename detail::index_hacking_cast<native_cont_type>::type hacking_cont_type;

	return reinterpret_cast<hacking_cont_type&>(c).extract(key_or_iter);
}

// node insert
template<typename K, typename P, typename S, typename T, typename C> inline
typename 
	detail::node_handle_foo_hacking_midx_ordered_index
	<
		K, P,
		typename detail::nth_layer_hacking_cast<S>::type,
		T, C
	>::insert_return_type
	insert(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c, 
			BOOST_RV_REF_BEG
				typename 
					detail::node_handle_foo_hacking_midx_ordered_index
					<
						K, P,
						typename detail::nth_layer_hacking_cast<S>::type,
						T, C
					>::node_type
			BOOST_RV_REF_END node)
{
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C> native_cont_type;
	typedef typename detail::index_hacking_cast<native_cont_type>::type hacking_cont_type;

	return reinterpret_cast<hacking_cont_type&>(c).insert(boost::move(node));
}

template<typename K, typename P, typename S, typename T, typename C> inline
typename 
	detail::node_handle_foo_hacking_midx_ordered_index
	<
		K, P,
		typename detail::nth_layer_hacking_cast<S>::type,
		T, C
	>::iterator
	insert(boost::multi_index::detail::ordered_index<K, P, S, T, C>& c, 
			typename 
				boost::multi_index::detail::ordered_index
				<
					K, P, S, 
					T, C
				>::const_iterator pos,
			BOOST_RV_REF_BEG
				typename 
					detail::node_handle_foo_hacking_midx_ordered_index
					<
						K, P,
						typename detail::nth_layer_hacking_cast<S>::type,
						T, C
					>::node_type
			BOOST_RV_REF_END node)
{
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C> native_cont_type;
	typedef typename detail::index_hacking_cast<native_cont_type>::type hacking_cont_type;

	return reinterpret_cast<hacking_cont_type&>(c).insert(pos, boost::move(node));
}

#	endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

#else

#	if !defined(YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX)
#		error "boost multi_index version panic"
#	endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

// extract
template<typename K, typename P, 
			typename S, typename T, typename C, typename A, 
			typename KeyOrIter> inline
typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::node_type
	extract(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// node insert
template<typename K, typename P, typename S, typename T, typename C, typename A> inline
typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::insert_return_type
	insert(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, 
			BOOST_RV_REF_BEG
				typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<typename K, typename P, typename S, typename T, typename C, typename A> inline
typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::iterator
	insert(boost::multi_index::detail::ordered_index<K, P, S, T, C, A>& c, 
			typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::const_iterator pos,
			BOOST_RV_REF_BEG
				typename boost::multi_index::detail::ordered_index<K, P, S, T, C, A>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(pos, boost::move(node));
}

#endif // BOOST_VERSION < 107400

} // namespace container
} // namespace yggr

namespace boost
{
namespace multi_index
{
namespace detail
{

#ifdef YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(6, boost::multi_index::detail::ordered_index)

#else

	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(5, boost::multi_index::detail::ordered_index)

#endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

} // namespace detail
} // namespace multi_index
} // namespace boost

#endif // __YGGR_CONTAINER_MULTI_INDEX_ORDERED_INDEX_HPP__
