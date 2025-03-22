//multi_index_sequenced_index.hpp

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

#ifndef __YGGR_CONTAINER_MULTI_INDEX_SEQUENCED_INDEX_HPP__
#define __YGGR_CONTAINER_MULTI_INDEX_SEQUENCED_INDEX_HPP__

#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/typename_expand.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/multi_index_config.hpp>
#include <yggr/container/node_handle_traits.hpp>

#include <yggr/container/detail/range_size_def.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_midx_sequenced_index.hpp>

#include <boost/multi_index/sequenced_index.hpp>

namespace yggr
{
namespace container
{

using boost::multi_index::sequenced;

} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#	if defined(YGGR_BOOST_MULTI_INDEX_HAS_EMPLACE)

		template< typename S, typename T, typename ...Args> inline
		std::pair< typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool >
			emplace_front(boost::multi_index::detail::sequenced_index<S, T>& c, BOOST_FWD_REF(Args)... args)
		{
			return c.emplace_front(boost::forward<Args>(args)...);
		}

		template< typename S, typename T, typename ...Args> inline
		std::pair< typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool >
			emplace_back(boost::multi_index::detail::sequenced_index<S, T>& c, BOOST_FWD_REF(Args)... args)
		{
			return c.emplace_back(boost::forward<Args>(args)...);
		}
		
		template< typename S, typename T, typename ...Args> inline
		std::pair< typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool >
			emplace(boost::multi_index::detail::sequenced_index<S, T>& c, 
						typename boost::multi_index::detail::sequenced_index<S, T>::iterator pos,
						BOOST_FWD_REF(Args)... args)
		{
			return c.emplace(pos, boost::forward<Args>(args)...);
		}


#	else

		template< typename S, typename T, typename ...Args> inline
		std::pair< typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool >
			emplace_front(boost::multi_index::detail::sequenced_index<S, T>& c, BOOST_FWD_REF(Args)... args)
		{
			return c.push_front(value_type(boost::forward<Args>(args)...));
		}

		template< typename S, typename T, typename ...Args> inline
		std::pair< typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool >
			emplace_back(boost::multi_index::detail::sequenced_index<S, T>& c, BOOST_FWD_REF(Args)... args)
		{
			return c.push_back(value_type(boost::forward<Args>(args)...));
		}

		template< typename S, typename T, typename ...Args> inline
		std::pair< typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool >
			emplace(boost::multi_index::detail::sequenced_index<S, T>& c, 
						typename boost::multi_index::detail::sequenced_index<S, T>::iterator pos,
						BOOST_FWD_REF(Args)... args)
		{
			typedef boost::multi_index::detail::sequenced_index<S, T> now_index_type;
			typedef typename now_index_type::value_type value_type;
			return c.insert(pos, value_type(boost::forward<Args>(args)...));
		}

#	endif // YGGR_BOOST_MULTI_INDEX_HAS_EMPLACE

#else

#	if defined(YGGR_BOOST_MULTI_INDEX_HAS_EMPLACE)

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			template< typename S, typename T \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool> \
				emplace_front(boost::multi_index::detail::sequenced_index<S, T>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace_front(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template< typename S, typename T \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool> \
				emplace_back(boost::multi_index::detail::sequenced_index<S, T>& c \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template< typename S, typename T \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool> \
				emplace(boost::multi_index::detail::sequenced_index<S, T>& c, \
							typename boost::multi_index::detail::sequenced_index<S, T>::iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }
#	else

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			template< typename S, typename T \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool> \
				emplace_front(boost::multi_index::detail::sequenced_index<S, T>& c \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				typedef boost::multi_index::detail::sequenced_index<S, T> now_index_type; \
				typedef typename now_index_type::value_type value_type; \
				return c.push_front(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
			\
			template< typename S, typename T \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool> \
				emplace_back(boost::multi_index::detail::sequenced_index<S, T>& c \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				typedef boost::multi_index::detail::sequenced_index<S, T> now_index_type; \
				typedef typename now_index_type::value_type value_type; \
				return c.push_back(value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
			\
			template< typename S, typename T \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			std::pair<typename boost::multi_index::detail::sequenced_index<S, T>::iterator, bool> \
				emplace(boost::multi_index::detail::sequenced_index<S, T>& c, \
							typename boost::multi_index::detail::sequenced_index<S, T>::iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				typedef boost::multi_index::detail::sequenced_index<S, T> now_index_type; \
				typedef typename now_index_type::value_type value_type; \
				return c.insert(pos, value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

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

// extract
template<typename S, typename T, typename Iter> inline
typename 
	detail::node_handle_foo_hacking_midx_sequenced_index
	<
		typename detail::nth_layer_hacking_cast<S>::type,
		T
	>::node_type
	extract(boost::multi_index::detail::sequenced_index<S, T>& c, const Iter& pos)
{
	typedef boost::multi_index::detail::sequenced_index<S, T> native_cont_type;
	typedef typename detail::index_hacking_cast<native_cont_type>::type hacking_cont_type;

	return reinterpret_cast<hacking_cont_type&>(c).extract(pos);
}

// node insert
template<typename S, typename T> inline
typename 
	detail::node_handle_foo_hacking_midx_sequenced_index
	<
		typename detail::nth_layer_hacking_cast<S>::type,
		T
	>::insert_return_type
	insert(boost::multi_index::detail::sequenced_index<S, T>& c, 
			typename boost::multi_index::detail::sequenced_index<S, T>::const_iterator pos,
			BOOST_RV_REF_BEG
				typename 
					detail::node_handle_foo_hacking_midx_sequenced_index
					<
						typename detail::nth_layer_hacking_cast<S>::type,
						T
					>::node_type
			BOOST_RV_REF_END node)
{
	typedef boost::multi_index::detail::sequenced_index<S, T> native_cont_type;
	typedef typename detail::index_hacking_cast<native_cont_type>::type hacking_cont_type;

	return reinterpret_cast<hacking_cont_type&>(c).insert(pos, boost::move(node));
}

#else

// extract
template<typename S, typename T, typename Iter> inline
typename boost::multi_index::detail::sequenced_index<S, T>::node_type
	extract(boost::multi_index::detail::sequenced_index<S, T>& c, const Iter& pos)
{
	return c.extract(pos);
}

// node insert
template<typename S, typename T> inline
typename boost::multi_index::detail::sequenced_index<S, T>::insert_return_type
	insert(boost::multi_index::detail::sequenced_index<S, T>& c, 
			typename boost::multi_index::detail::sequenced_index<S, T>::const_iterator pos,
			BOOST_RV_REF_BEG
				typename boost::multi_index::detail::sequenced_index<S, T>::node_type
			BOOST_RV_REF_END node)
{
	return c.insert(pos, boost::move(node));
}

#endif // (BOOST_VERSION < 107400)

} // namespace container
} // namespace yggr

namespace boost
{
namespace multi_index
{
namespace detail
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, boost::multi_index::detail::sequenced_index)

} // namespace detail
} // namespace multi_index
} // namespace boost

#endif // __YGGR_CONTAINER_MULTI_INDEX_SEQUENCED_INDEX_HPP__
