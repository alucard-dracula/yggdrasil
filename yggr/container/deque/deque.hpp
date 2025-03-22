//deque.hpp

/****************************************************************************
Copyright (c) 2010-2022 yggdrasil

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

#ifndef __YGGR_CONTAINER_DEQUE_HPP__
#define __YGGR_CONTAINER_DEQUE_HPP__

#include <yggr/container/container_selector.hpp>

#include <deque>

#if (BOOST_VERSION == 105500)

namespace boost
{
namespace container
{

using std::equal;
using std::lexicographical_compare;

} // namespace container
} // namespace boost

#endif // (BOOST_VERSION == 105500)

#include <boost/container/deque.hpp>

#include <boost/utility/enable_if.hpp>

#include <yggr/move/move.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/mplex/typename_expand.hpp>

#include <yggr/container/is_isomeric_same_deque.hpp>

#if !(BOOST_VERSION < 107100)
#	include <yggr/container/is_isomeric_same_options_deque.hpp>
#endif // !(BOOST_VERSION < 107100)

#include <yggr/container/basic_function_adapter.hpp>
#include <yggr/container/detail/is_xxx_container.hpp>

#include <yggr/container/detail/call_erase_helper.hpp>
#include <yggr/container/detail/call_insert_helper.hpp>
#include <yggr/container/detail/swap_def.hpp>
#include <yggr/container/detail/comparer_def.hpp>
#include <yggr/container/detail/range_size_def.hpp>

#include <yggr/container/detail/has_emplace.hpp>
#include <yggr/container/detail/has_shrink_to_fit.hpp>


YGGR_PP_IS_XXX_CONTAINER_DEF(2, std_deque, std::deque);

#if (BOOST_VERSION < 107100)
	YGGR_PP_IS_XXX_CONTAINER_DEF(2, boost_deque, boost::container::deque);
#else
	YGGR_PP_IS_XXX_CONTAINER_DEF(3, boost_deque, boost::container::deque);
#endif // (BOOST_VERSION < 107100)

// compatibility caller
namespace yggr
{
namespace container
{

// shrink_to_fit
template<typename T, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_shrink_to_fit< std::deque<T, A> >,
		void
	>::type
	shrink_to_fit(std::deque<T, A>& c)
{
	c.shrink_to_fit();
}

template<typename T, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_shrink_to_fit< std::deque<T, A> >,
		void
	>::type
	shrink_to_fit(std::deque<T, A>& c)
{
	std::deque<T, A> tmp(c.begin(), c.end(), c.get_allocator());
	tmp.swap(c);
}

#if BOOST_VERSON < 107100

template<typename T, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_shrink_to_fit< boost::container::deque<T, A> >,
		void
	>::type
	shrink_to_fit(boost::container::deque<T, A>& c)
{
	c.shrink_to_fit();
}

template<typename T, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_shrink_to_fit< boost::container::deque<T, A> >,
		void
	>::type
	shrink_to_fit(boost::container::deque<T, A>& c)
{
	boost::container::deque<T, A> tmp(c.begin(), c.end(), c.get_allocator());
	tmp.swap(c);
}

#else

template<typename T, typename A, typename O> inline
typename
	boost::enable_if
	<
		detail::has_any_shrink_to_fit< boost::container::deque<T, A, O> >,
		void
	>::type
	shrink_to_fit(boost::container::deque<T, A, O>& c)
{
	c.shrink_to_fit();
}

template<typename T, typename A, typename O> inline
typename
	boost::disable_if
	<
		detail::has_any_shrink_to_fit< boost::container::deque<T, A, O> >,
		void
	>::type
	shrink_to_fit(boost::container::deque<T, A, O>& c)
{
	boost::container::deque<T, A> tmp(c.begin(), c.end(), c.get_allocator());
	tmp.swap(c);
}

#endif // BOOST_VERSON < 107100

//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< std::deque<T, A> >,
		typename std::deque<T, A>::iterator
	>::type
	emplace(std::deque<T, A>& c, 
			typename std::deque<T, A>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< std::deque<T, A> >,
		typename std::deque<T, A>::iterator
	>::type
	emplace(std::deque<T, A>& c, 
			typename std::deque<T, A>::iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return insert(c, pos, T(boost::forward<Args>(args)...));
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_front< std::deque<T, A> >,
		typename std::deque<T, A>::reference
	>::type
	emplace_front(std::deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_front(boost::forward<Args>(args)...);
	return c.front();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_front< std::deque<T, A> >,
		typename std::deque<T, A>::reference
	>::type
	emplace_front(std::deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_front(T(boost::forward<Args>(args)...));
	return c.front();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_back< std::deque<T, A> >,
		typename std::deque<T, A>::reference
	>::type
	emplace_back(std::deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_back< std::deque<T, A> >,
		typename std::deque<T, A>::reference
	>::type
	emplace_back(std::deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_back(T(boost::forward<Args>(args)...));
	return c.back();
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< std::deque<T, A> >, \
									typename std::deque<T, A>::iterator>::type \
			emplace(std::deque<T, A>& c, \
						typename std::deque<T, A>::const_iterator pos \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< std::deque<T, A> >, \
									typename std::deque<T, A>::iterator>::type \
			emplace(std::deque<T, A>& c, \
						typename std::deque<T, A>::iterator pos \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::deque<T, A>::reference \
			emplace_front(std::deque<T, A>& c \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_front(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.front(); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::deque<T, A>::reference \
			emplace_back(std::deque<T, A>& c \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_back(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.back(); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES



#if BOOST_VERSION < 107100

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< boost::container::deque<T, A> >,
		typename boost::container::deque<T, A>::iterator
	>::type
	emplace(boost::container::deque<T, A>& c, 
			typename boost::container::deque<T, A>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< boost::container::deque<T, A> >,
		typename boost::container::deque<T, A>::iterator
	>::type
	emplace(boost::container::deque<T, A>& c, 
			typename boost::container::deque<T, A>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return insert(c, pos, T(boost::forward<Args>(args)...));
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_front< boost::container::deque<T, A> >,
		typename boost::container::deque<T, A>::reference
	>::type
	emplace_front(boost::container::deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_front(boost::forward<Args>(args)...);
	return c.front();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_front< boost::container::deque<T, A> >,
		typename boost::container::deque<T, A>::reference
	>::type
	emplace_front(boost::container::deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_front(T(boost::forward<Args>(args)...));
	return c.front();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_back< boost::container::deque<T, A> >,
		typename boost::container::deque<T, A>::reference
	>::type
	emplace_back(boost::container::deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_back< boost::container::deque<T, A> >,
		typename boost::container::deque<T, A>::reference
	>::type
	emplace_back(boost::container::deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_back(T(boost::forward<Args>(args)...));
	return c.back();
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< boost::container::deque<T, A> >, \
									typename boost::container::deque<T, A>::iterator>::type \
			emplace(boost::container::deque<T, A>& c, \
						typename boost::container::deque<T, A>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< boost::container::deque<T, A> >, \
									typename boost::container::deque<T, A>::iterator>::type \
			emplace(boost::container::deque<T, A>& c, \
						typename boost::container::deque<T, A>::iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_front< boost::container::deque<T, A> >, \
			typename boost::container::deque<T, A>::reference>::type \
			emplace_front(boost::container::deque<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_front(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.front(); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_front< boost::container::deque<T, A> >, \
			typename boost::container::deque<T, A>::reference>::type \
			emplace_front(boost::container::deque<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_front(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.front(); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_back< boost::container::deque<T, A> >, \
			typename boost::container::deque<T, A>::reference >::type \
			emplace_back(boost::container::deque<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.back(); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_back< boost::container::deque<T, A> >, \
			typename boost::container::deque<T, A>::reference >::type \
			emplace_back(boost::container::deque<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_back(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.back(); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#else

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< boost::container::deque<T, A, O> >,
		typename boost::container::deque<T, A, O>::iterator
	>::type
	emplace(boost::container::deque<T, A, O>& c, 
			typename boost::container::deque<T, A, O>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< boost::container::deque<T, A, O> >,
		typename boost::container::deque<T, A, O>::iterator
	>::type
	emplace(boost::container::deque<T, A, O>& c, 
			typename boost::container::deque<T, A, O>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return insert(c, pos, T(boost::forward<Args>(args)...));
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_front< boost::container::deque<T, A, O> >,
		typename boost::container::deque<T, A, O>::reference
	>::type
	emplace_front(boost::container::deque<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_front(boost::forward<Args>(args)...);
	return c.front();
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_front< boost::container::deque<T, A, O> >,
		typename boost::container::deque<T, A, O>::reference
	>::type
	emplace_front(boost::container::deque<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_front(T(boost::forward<Args>(args)...));
	return c.front();
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_back< boost::container::deque<T, A, O> >,
		typename boost::container::deque<T, A, O>::reference
	>::type
	emplace_back(boost::container::deque<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

template<typename T, typename A, typename O, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_back< boost::container::deque<T, A, O> >,
		typename boost::container::deque<T, A, O>::reference
	>::type
	emplace_back(boost::container::deque<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_back(T(boost::forward<Args>(args)...));
	return c.back();
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< boost::container::deque<T, A, O> >, \
									typename boost::container::deque<T, A, O>::iterator>::type \
			emplace(boost::container::deque<T, A, O>& c, \
						typename boost::container::deque<T, A, O>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< boost::container::deque<T, A, O> >, \
									typename boost::container::deque<T, A, O>::iterator>::type \
			emplace(boost::container::deque<T, A, O>& c, \
						typename boost::container::deque<T, A, O>::iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_front< boost::container::deque<T, A, O> >, \
			typename boost::container::deque<T, A, O>::reference>::type \
			emplace_front(boost::container::deque<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_front(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.front(); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_front< boost::container::deque<T, A, O> >, \
			typename boost::container::deque<T, A, O>::reference>::type \
			emplace_front(boost::container::deque<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_front(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.front(); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_back< boost::container::deque<T, A, O> >, \
			typename boost::container::deque<T, A, O>::reference >::type \
			emplace_back(boost::container::deque<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.back(); } \
		\
		template<typename T, typename A, typename O \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_back< boost::container::deque<T, A, O> >, \
			typename boost::container::deque<T, A, O>::reference >::type \
			emplace_back(boost::container::deque<T, A, O>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_back(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.back(); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#endif // BOOST_VERSION < 107100

// erase
template<typename T, typename A, typename Val> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename std::deque<T, A>::iterator>,
				boost::is_same<Val, typename std::deque<T, A>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename std::deque<T, A>::reverse_iterator>,
				boost::is_same<Val, typename std::deque<T, A>::const_reverse_iterator>
			>
		>,
		typename std::deque<T, A>::size_type
	>::type
	erase(std::deque<T, A>& c, const Val& value)
{
	typedef std::deque<T, A> deque_type;
	typedef typename deque_type::iterator deque_iter_type;
	typedef typename deque_type::size_type deque_size_type;

	deque_iter_type it = std::remove(c.begin(), c.end(), value);
	deque_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename Pred> inline
typename std::deque<T, A>::size_type 
	erase_if(std::deque<T, A>& c, Pred pred)
{
	typedef std::deque<T, A> deque_type;
	typedef typename deque_type::iterator deque_iter_type;
	typedef typename deque_type::size_type deque_size_type;

	deque_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	deque_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

#if BOOST_VERSION < 107100
// erase
template<typename T, typename A, typename Val> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename boost::container::deque<T, A>::iterator>,
				boost::is_same<Val, typename boost::container::deque<T, A>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename boost::container::deque<T, A>::reverse_iterator>,
				boost::is_same<Val, typename boost::container::deque<T, A>::const_reverse_iterator>
			>
		>,
		typename boost::container::deque<T, A>::size_type
	>::type
	erase(boost::container::deque<T, A>& c, const Val& value)
{
	typedef boost::container::deque<T, A> deque_type;
	typedef typename deque_type::iterator deque_iter_type;
	typedef typename deque_type::size_type deque_size_type;

	deque_iter_type it = std::remove(c.begin(), c.end(), value);
	deque_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename Pred> inline
typename boost::container::deque<T, A>::size_type 
	erase_if(boost::container::deque<T, A>& c, Pred pred)
{
	typedef boost::container::deque<T, A> deque_type;
	typedef typename deque_type::iterator deque_iter_type;
	typedef typename deque_type::size_type deque_size_type;

	deque_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	deque_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

#else

// erase
template<typename T, typename A, typename O, typename Val> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename boost::container::deque<T, A, O>::iterator>,
				boost::is_same<Val, typename boost::container::deque<T, A, O>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename boost::container::deque<T, A, O>::reverse_iterator>,
				boost::is_same<Val, typename boost::container::deque<T, A, O>::const_reverse_iterator>
			>
		>,
		typename boost::container::deque<T, A, O>::size_type
	>::type
	erase(boost::container::deque<T, A, O>& c, const Val& value)
{
	typedef boost::container::deque<T, A, O> deque_type;
	typedef typename deque_type::iterator deque_iter_type;
	typedef typename deque_type::size_type deque_size_type;

	deque_iter_type it = std::remove(c.begin(), c.end(), value);
	deque_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename O, typename Pred> inline
typename boost::container::deque<T, A, O>::size_type 
	erase_if(boost::container::deque<T, A, O>& c, Pred pred)
{
	typedef boost::container::deque<T, A, O> deque_type;
	typedef typename deque_type::iterator deque_iter_type;
	typedef typename deque_type::size_type deque_size_type;

	deque_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	deque_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}


#endif // BOOST_VERSION < 107100

} // namespace container
} // namespace yggr

//#if !(BOOST_VERSION < 107100)
//
//#include <yggr/container/is_isomeric_same_options_deque.hpp>
//
//namespace boost
//{
//namespace container
//{
//
//YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_COMPARER_FULL(
//	3, 
//	::boost::container::deque, 
//	::yggr::container::is_isomeric_same_options_deque,
//	inline)
//
//} // namespace container
//} // namespace boost
//
//namespace boost
//{
//namespace container
//{
//namespace detail
//{
//namespace swap_support
//{
//	YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_SWAP(
//		3, 
//		::boost::container::deque, 
//		::yggr::container::is_isomeric_same_options_deque,
//		inline )
//
//} // namespace swap_support
//} // namespace detail
//} // namespace container
//} // namespace boost
//
//namespace std
//{
//	using ::boost::container::detail::swap_support::swap;
//}// namespace std
//
//namespace boost
//{
//	using container::detail::swap_support::swap;
//} // namespace boost
//
//#endif // !(BOOST_VERSION < 107100)

#include <yggr/container/is_isomeric_same_options_deque.hpp>

#if (BOOST_VERSION < 107100)

#	if (BOOST_VERSION == 107000)
namespace boost
{
namespace container
{

YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_COMPARER_FULL(
	2, 
	::boost::container::deque, 
	::yggr::container::is_isomeric_same_options_deque,
	inline)

} // namespace container
} // namespace boost
#	endif // (BOOST_VERSION == 107000)

namespace boost
{
namespace container
{
namespace detail
{
namespace swap_support
{
	YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_SWAP(
		2, 
		::boost::container::deque, 
		::yggr::container::is_isomeric_same_deque,
		inline )

} // namespace swap_support
} // namespace detail
} // namespace container
} // namespace boost

//namespace std
//{
//	using ::boost::container::detail::swap_support::swap;
//}// namespace std
//
//namespace boost
//{
//	using container::detail::swap_support::swap;
//} // namespace boost

#else

namespace boost
{
namespace container
{

YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_COMPARER_FULL(
	3, 
	::boost::container::deque, 
	::yggr::container::is_isomeric_same_options_deque,
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
		3, 
		::boost::container::deque, 
		::yggr::container::is_isomeric_same_options_deque,
		inline )

} // namespace swap_support
} // namespace detail
} // namespace container
} // namespace boost

//namespace std
//{
//	using ::boost::container::detail::swap_support::swap;
//}// namespace std
//
//namespace boost
//{
//	using container::detail::swap_support::swap;
//} // namespace boost

#endif // !(BOOST_VERSION < 107100)

namespace std
{
	using ::boost::container::detail::swap_support::swap;
}// namespace std

namespace boost
{
	using container::detail::swap_support::swap;
} // namespace boost

// boost::size support
namespace std
{

	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, std::deque)

} // namespace std

namespace boost
{
namespace container
{

#if BOOST_VERSION < 107100

	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, boost::container::deque)

#else

	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, boost::container::deque)

#endif // BOOST_VERSION < 107100

} // namespace container
} // namespace boost


#if !(BOOST_VERSION < 107100)

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T, typename A>
class deque;

} // namespace detail
} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T, 
			typename A =
				typename mplex::typename_expand_get<boost::container::deque<T>, 1>::type >
class deque
	: public boost::container::deque<T, A>
{
public:
	typedef A tpl_arg_allocator_type;

public:
	typedef boost::container::deque<T, A> base_type;

protected:
	typedef typename boost::container::get_deque_opt<void>::type options_type;

public:
	typedef typename base_type::value_type				value_type;
	typedef typename base_type::allocator_type			allocator_type;
	typedef typename base_type::pointer					pointer;
	typedef typename base_type::const_pointer			const_pointer;
	typedef typename base_type::reference				reference;
	typedef	typename base_type::const_reference			const_reference;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::stored_allocator_type	stored_allocator_type;
	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;
	typedef 
		boost::container::allocator_traits
		<
			allocator_type
		>												allocator_traits_type;

private:
	typedef deque this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

//public:
//	using base_type::get_block_size;

public:
	deque(void)
		: base_type()
	{
	}

	explicit deque(const allocator_type& a)
		: base_type(a)
	{
	}

	explicit deque(size_type n)
		: base_type(n)
	{
	}

	explicit deque(size_type n, const allocator_type& a)
		: base_type(n, value_type(), a)
	{
	}

	deque(size_type n, const value_type& value)
		: base_type(n, value, allocator_type())
	{ 
	}

	deque(size_type n, const value_type& value, const allocator_type& a)
		: base_type(n, value, a)
	{
	}

	template <typename InIt>
	deque(InIt first, InIt last)
		: base_type(first, last)
	{
	}

	template <typename InIt>
	deque(InIt first, InIt last, const allocator_type& a)
		: base_type(first, last, a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	deque(std::initializer_list<value_type> il, const allocator_type& a = allocator_type())
		: base_type(il, a)
	{
	}

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	deque(BOOST_RV_REF(base_type) x)
		: base_type(boost::move(static_cast<base_type&>(x)))
	{ 
	}

	deque(const base_type& x)
		:  base_type(x)
	{
	}

	deque(BOOST_RV_REF(this_type) x)
		: base_type(boost::move(static_cast<base_type&>(x)))
	{ 
	}

	deque(const this_type& x)
		:  base_type(static_cast<const base_type&>(x))
	{
	}

	deque(BOOST_RV_REF(base_type) x, const allocator_type& a)
		: base_type(boost::move(x), a)
	{
	}

	deque(const base_type& x, const allocator_type& a)
		:  base_type(x, a)
	{
	}

	deque(BOOST_RV_REF(this_type) x, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(x)), a)
	{
	}

	deque(const this_type& x, const allocator_type& a)
		:  base_type(static_cast<const base_type&>(x), a)
	{
	}

	template<typename A2, typename O2>
	deque(BOOST_RV_REF_BEG 
				boost::container::deque<value_type, A2, O2>
			BOOST_RV_REF_END x,
			typename 
				boost::enable_if
				<
					is_isomeric_same_options_deque
					<
						base_type, 
						boost::container::deque<value_type, A2, O2>
					>,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::deque<value_type, A2, O2>&>(x))))
	{ 
	}

	template<typename A2, typename O2>
	deque(const boost::container::deque<value_type, A2, O2>& x,
			typename 
				boost::enable_if
				<
					is_isomeric_same_options_deque
					<
						base_type, 
						boost::container::deque<value_type, A2, O2>
					>,
					mplex::sfinae_type
				>::type sfinae = 0)
		:  base_type(reinterpret_cast<const base_type&>(x))
	{
	}

	template<typename A2>
	deque(BOOST_RV_REF_BEG
				deque<value_type, A2>
			BOOST_RV_REF_END x,
			typename 
				boost::enable_if
				<
					is_isomeric_same_options_deque
					<
						base_type, 
						typename deque<value_type, A2>::base_type
					>,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename deque<value_type, A2>::base_type&>(x))))
	{ 
	}

	template<typename A2>
	deque(const deque<value_type, A2>& x,
			typename 
				boost::enable_if
				<
					is_isomeric_same_options_deque
					<
						base_type, 
						typename deque<value_type, A2>::base_type
					>,
					mplex::sfinae_type
				>::type sfinae = 0)
		:  base_type(reinterpret_cast<const base_type&>(x))
	{
	}

	template<typename A2, typename O2>
	deque(BOOST_RV_REF_BEG 
				boost::container::deque<value_type, A2, O2>
			BOOST_RV_REF_END x,
			const allocator_type& alloc,
			typename 
				boost::enable_if
				<
					is_isomeric_same_options_deque
					<
						base_type, 
						boost::container::deque<value_type, A2, O2>
					>,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::deque<value_type, A2, O2>&>(x))),
			alloc)
	{ 
	}

	template<typename A2, typename O2>
	deque(const boost::container::deque<value_type, A2, O2>& x,
			const allocator_type& alloc,
			typename 
				boost::enable_if
				<
					is_isomeric_same_options_deque
					<
						base_type, 
						boost::container::deque<value_type, A2, O2>
					>,
					mplex::sfinae_type
				>::type sfinae = 0)
		:  base_type(
			reinterpret_cast<const base_type&>(x),
			alloc)
	{
	}

	template<typename A2>
	deque(BOOST_RV_REF_BEG
				deque<value_type, A2>
			BOOST_RV_REF_END x,
			const allocator_type& alloc,
			typename 
				boost::enable_if
				<
					is_isomeric_same_options_deque
					<
						base_type, 
						typename deque<value_type, A2>::base_type
					>,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename deque<value_type, A2>::base_type&>(x))),
			alloc)
	{ 
	}

	template<typename A2>
	deque(const deque<value_type, A2>& x,
			const allocator_type& alloc,
			typename 
				boost::enable_if
				<
					is_isomeric_same_options_deque
					<
						base_type, 
						typename deque<value_type, A2>::base_type
					>,
					mplex::sfinae_type
				>::type sfinae = 0)
		:  base_type(
			reinterpret_cast<const base_type&>(x),
			alloc)
	{
	}
	
	~deque(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) x)
	{
		base_type::operator=(boost::move(x));
		return *this;
	}

	inline this_type& operator=(BOOST_COPY_ASSIGN_REF(base_type) x)
	{
		base_type::operator=(x);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) x)
	{
		base_type::operator=(boost::move(static_cast<base_type&>(x)));
		return *this;
	}

	inline this_type& operator=(BOOST_COPY_ASSIGN_REF(this_type) x)
	{
		base_type::operator=(x);
		return *this;
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
	inline this_type& operator=(std::initializer_list<value_type> il)
	{
		base_type::assign(il.begin(), il.end());
		return *this;
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	template<typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_deque
			<
				base_type, 
				boost::container::deque<value_type, A2, O2>
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG 
						boost::container::deque<value_type, A2, O2>
					BOOST_RV_REF_END x)
	{
		typedef boost::container::deque<value_type, A2, O2> right_type;
		base_type::operator=(
			boost::move(reinterpret_cast<base_type&>(static_cast<right_type&>(x))));
		return *this;
	}

	template<typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_deque
			<
				base_type, 
				boost::container::deque<value_type, A2, O2>
			>,
			this_type&
		>::type
		operator=(const boost::container::deque<value_type, A2, O2>& x)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(x));
		return *this;
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_deque
			<
				base_type, 
				typename deque<value_type, A2>::base_type
			>,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG 
						deque<value_type, A2>
					BOOST_RV_REF_END x)
	{
		typedef deque<value_type, A2> right_type;
		base_type::operator=(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<right_type&>(x))));
		return *this;
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_deque
			<
				base_type, 
				typename deque<value_type, A2>::base_type
			>,
			this_type&
		>::type
		operator=(const deque<value_type, A2>& x)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(x));
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

	template<typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_deque
			<
				base_type, 
				boost::container::deque<value_type, A2, O2>
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG 
					boost::container::deque<value_type, A2, O2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::deque<value_type, A2, O2> right_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename A2, typename O2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_deque
			<
				base_type, 
				boost::container::deque<value_type, A2, O2>
			>,
			void
		>::type
		swap(boost::container::deque<value_type, A2, O2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_deque
			<
				base_type, 
				typename deque<value_type, A2>::type
			>,
			void
		>::type
		swap(BOOST_RV_REF_BEG 
					deque<value_type, A2>
				BOOST_RV_REF_END right)
	{
		typedef deque<value_type, A2> right_type;
		typedef typename right_type::base_type right_base_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_base_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_options_deque
			<
				base_type, 
				typename deque<value_type, A2>::type
			>,
			void
		>::type
		swap(deque<value_type, A2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

//public:
//	template<typename A2, typename O2> inline
//	operator boost::container::deque<value_type, A2, O2>& (void)
//	{
//		BOOST_MPL_ASSERT((is_same_or_isomeric_same_options_deque<base_type, boost::container::deque<value_type, A2, O2> >));
//		return reinterpret_cast<boost::container::deque<value_type, A2, O2>&>(*this);
//	}
//
//	template<typename A2, typename O2> inline
//	operator const boost::container::deque<value_type, A2, O2>& (void) const
//	{
//		BOOST_MPL_ASSERT((is_same_or_isomeric_same_options_deque<base_type, boost::container::deque<value_type, A2, O2> >));
//		return reinterpret_cast<const boost::container::deque<value_type, A2, O2>&>(*this);
//	}

public:
	using base_type::assign;
	using base_type::get_allocator;
	//using base_type::get_stored_allocator;

	using base_type::begin;
	using base_type::end;

	using base_type::rbegin;
	using base_type::rend;

	using base_type::cbegin;
	using base_type::cend;

	using base_type::crbegin;
	using base_type::crend;

	using base_type::empty;
	
	using base_type::size;
	using base_type::max_size;
	
	using base_type::resize;

	using base_type::shrink_to_fit;

	using base_type::front;
	using base_type::back;

	using base_type::operator[];

#if BOOST_VERSION < 105800

	inline iterator nth(size_type n)
	{
		BOOST_ASSERT(n <= base_type::size());
		return 
			n < base_type::size()?
				iterator_ex::advance_copy(base_type::begin(), n)
				: base_type::end();
	}

	inline const_iterator nth(size_type n) const
	{
		BOOST_ASSERT(n <= base_type::size());
		return 
			n < base_type::size()?
				iterator_ex::advance_copy(base_type::begin(), n)
				: base_type::end();
	}

	inline size_type index_of(iterator iter)
	{
		BOOST_ASSERT(iter <= base_type::end());
		return iterator_ex::distance(base_type::begin(), iter);
	}

	size_type index_of(const_iterator iter) const
	{
		BOOST_ASSERT(iter <= base_type::end());
		return iterator_ex::distance(base_type::begin(), iter);
	}

#else
	using base_type::nth;
	using base_type::index_of;

#endif // BOOST_VERSION < 105800

	using base_type::at;

	using base_type::emplace_front;
	using base_type::emplace_back;
	using base_type::emplace;

	using base_type::push_front;
	using base_type::push_back;

	using base_type::insert;

	using base_type::pop_front;
	using base_type::pop_back;

	using base_type::erase;

	using base_type::clear;

};

// don't need defined YGGR_PP_CONTAINER_COMPARER_FULL, using boost::container::deque
//#if (BOOST_VERSION < 107100)
//	YGGR_PP_CONTAINER_COMPARER_FULL(2, deque, inline)
//#endif // BOOST_VERSION < 106700

//YGGR_PP_CONTAINER_ISOMERIC_SAME_BASE_OPTIONS_COMPARER_FULL(
//	2, 
//	::boost::container::deque, 
//	deque, 
//	is_isomeric_same_deque,
//	inline)

YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL_MN(
	3, 2,
	::boost::container::deque, 
	::yggr::container::detail::deque, 
	::yggr::container::is_isomeric_same_options_deque,
	inline)
	
YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, deque)


// shrink_to_fit
template<typename T, typename A> inline
void shrink_to_fit(deque<T, A>& c)
{
	c.shrink_to_fit();
}

//emplace
#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< deque<T, A> >,
		typename deque<T, A>::iterator
	>::type
	emplace(deque<T, A>& c, 
			typename deque<T, A>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< deque<T, A> >,
		typename deque<T, A>::iterator
	>::type
	emplace(deque<T, A>& c, 
			typename deque<T, A>::const_iterator pos, 
			BOOST_FWD_REF(Args)... args)
{
	return insert(c, pos, T(boost::forward<Args>(args)...));
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_front< deque<T, A> >,
		typename deque<T, A>::reference
	>::type
	emplace_front(deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_front(boost::forward<Args>(args)...);
	return c.front();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_front< deque<T, A> >,
		typename deque<T, A>::reference
	>::type
	emplace_front(deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_front(T(boost::forward<Args>(args)...));
	return c.front();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_back< deque<T, A> >,
		typename deque<T, A>::reference
	>::type
	emplace_back(deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_back< deque<T, A> >,
		typename deque<T, A>::reference
	>::type
	emplace_back(deque<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_back(T(boost::forward<Args>(args)...));
	return c.back();
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< deque<T, A> >, \
									typename deque<T, A>::iterator>::type \
			emplace(deque<T, A>& c, \
						typename deque<T, A>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< deque<T, A> >, \
									typename deque<T, A>::iterator>::type \
			emplace(deque<T, A>& c, \
						typename deque<T, A>::iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_front< deque<T, A> >, \
			typename deque<T, A>::reference>::type \
			emplace_front(deque<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_front(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.front(); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_front< deque<T, A> >, \
			typename deque<T, A>::reference>::type \
			emplace_front(deque<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_front(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.front(); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_back< deque<T, A> >, \
			typename deque<T, A>::reference >::type \
			emplace_back(deque<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.back(); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_back< deque<T, A> >, \
			typename deque<T, A>::reference >::type \
			emplace_back(deque<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_back(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.back(); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

// erase
template<typename T, typename A, typename Val> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename deque<T, A>::iterator>,
				boost::is_same<Val, typename deque<T, A>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename deque<T, A>::reverse_iterator>,
				boost::is_same<Val, typename deque<T, A>::const_reverse_iterator>
			>
		>,
		typename deque<T, A>::size_type
	>::type
	erase(deque<T, A>& c, const Val& value)
{
	typedef deque<T, A> deque_type;
	typedef typename deque_type::iterator deque_iter_type;
	typedef typename deque_type::size_type deque_size_type;

	deque_iter_type it = std::remove(c.begin(), c.end(), value);
	deque_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename Pred> inline
typename deque<T, A>::size_type 
	erase_if(deque<T, A>& c, Pred pred)
{
	typedef deque<T, A> deque_type;
	typedef typename deque_type::iterator deque_iter_type;
	typedef typename deque_type::size_type deque_size_type;

	deque_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	deque_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // !(BOOST_VERSION < 107100)

#if !(BOOST_VERSION < 107100)
namespace yggr
{
namespace container
{
namespace detail
{
namespace swap_support
{

	YGGR_PP_CONTAINER_SWAP(2, ::yggr::container::detail::deque, inline)
	
	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		3, 2,
		::boost::container::deque, 
		::yggr::container::detail::deque, 
		::yggr::container::is_isomeric_same_deque,
		inline)



} // namespace swap_support
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

#endif // !(BOOST_VERSION < 107100)

namespace yggr
{
namespace container
{

#if defined YGGR_USE_BOOST_CONTAINER
#	if (BOOST_VERSION < 107100)
		using boost::container::deque;
#	else
		using detail::deque;
#	endif // (BOOST_VERSION < 107100)
#elif defined YGGR_USE_STL_CONTAINER
	using std::deque;
#else
#	error "use deque type unknown error. please check source"
#endif //YGGR_USE_BOOST_CONTAINER

} // namespace container

using container::deque;

} // namespace yggr

#endif // __YGGR_CONTAINER_DEQUE_HPP__
