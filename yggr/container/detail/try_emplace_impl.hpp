//try_emplace_impl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_TRY_EMPLACE_IMPL_HPP__
#define __YGGR_CONTAINER_DETAIL_TRY_EMPLACE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/container/detail/has_emplace.hpp>

#include <boost/utility/enable_if.hpp>

#include <algorithm>

namespace yggr
{
namespace container
{
namespace detail
{

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename Map, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_try_emplace< Map >,
		std::pair< typename Map::iterator, bool>
	>::type
	try_emplace_impl(Map& c, BOOST_RV_REF(typename Map::key_type) key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(boost::move(key), boost::forward<Args>(args)...);
}

template<typename Map, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_try_emplace< Map >,
		std::pair< typename Map::iterator, bool>
	>::type
	try_emplace_impl(Map& c, const typename Map::key_type& key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(key, boost::forward<Args>(args)...);
}

template<typename Map, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_try_emplace< Map >,
		std::pair< typename Map::iterator, bool>
	>::type
	try_emplace_impl(Map& c, BOOST_RV_REF(typename Map::key_type) key, BOOST_FWD_REF(Args)... args)
{
	typedef Map map_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::iterator iter_type;
	typedef std::pair<iter_type, bool> ret_type;

	ret_type ret;

	ret.first = c.find(static_cast<key_type&>(key));
	ret.second = ret.first == c.end();

	return
		ret.second?
			c.insert(value_type(boost::move(key), mapped_type(boost::forward<Args>(args)...)))
			: ret;
}

template<typename Map, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_try_emplace< Map >,
		std::pair< typename Map::iterator, bool>
	>::type
	try_emplace_impl(Map& c, const typename Map::key_type& key, BOOST_FWD_REF(Args)... args)
{
	typedef Map map_type;
	typedef typename map_type::value_type value_type;
	//typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::iterator iter_type;
	typedef std::pair<iter_type, bool> ret_type;

	ret_type ret;

	ret.first = c.find(key);
	ret.second = ret.first == c.end();

	return
		ret.second?
			c.insert(value_type(key, mapped_type(boost::forward<Args>(args)...)))
			: ret;
}

template<typename Map, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_try_emplace< Map >,
		typename Map::iterator
	>::type
	try_emplace_impl(Map& c, typename Map::const_iterator hint,
						BOOST_RV_REF(typename Map::key_type) key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(hint, boost::move(key), boost::forward<Args>(args)...);
}

template<typename Map, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_try_emplace< Map >,
		typename Map::iterator
	>::type
	try_emplace_impl(Map& c, typename Map::const_iterator hint,
						const typename Map::key_type& key, BOOST_FWD_REF(Args)... args)
{
	return c.try_emplace(hint, key, boost::forward<Args>(args)...);
}

template<typename Map, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_try_emplace< Map >,
		typename Map::iterator
	>::type
	try_emplace_impl(Map& c, typename Map::const_iterator hint,
					BOOST_RV_REF(typename Map::key_type) key, BOOST_FWD_REF(Args)... args)
{
	typedef Map map_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::iterator iter_type;

	iter_type iter = c.find(static_cast<key_type&>(key));

	return
		iter == c.end()?
			c.insert(value_type(boost::move(key), mapped_type(boost::forward<Args>(args)...))).first
			: iter;
}

template<typename Map, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_try_emplace< Map >,
		typename Map::iterator
	>::type
	try_emplace_impl(Map& c, typename Map::const_iterator hint,
						const typename Map::key_type& key, BOOST_FWD_REF(Args)... args)
{
	typedef Map map_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::iterator iter_type;

	iter_type iter = c.find(key);

	return
		iter == c.end()?
			c.insert(value_type(key, mapped_type(boost::forward<Args>(args)...))).first
			: iter;
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template< typename Map \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) >  inline \
		typename \
			boost::enable_if< \
				detail::has_any_try_emplace< Map >, \
				std::pair< typename Map::iterator, bool> >::type \
			try_emplace_impl(Map& c, \
								BOOST_RV_REF( typename Map::key_type ) key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); } \
		\
		template< typename Map \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) >  inline \
		typename \
			boost::enable_if< \
				detail::has_any_try_emplace< Map >, \
				std::pair< typename Map::iterator, bool> >::type \
			try_emplace_impl(Map& c, \
								const typename Map::key_type& key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); } \
		\
		template< typename Map \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) >  inline \
		typename \
			boost::disable_if< \
				detail::has_any_try_emplace< Map >, \
				std::pair< typename Map::iterator, bool> >::type \
			try_emplace_impl(Map& c, \
								BOOST_RV_REF(typename Map::key_type) key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef Map map_type; \
			typedef typename map_type::value_type value_type; \
			typedef typename map_type::key_type key_type; \
			typedef typename map_type::mapped_type mapped_type; \
			typedef typename map_type::iterator iter_type; \
			typedef std::pair<iter_type, bool> ret_type; \
			ret_type ret; \
			ret.first = c.find(static_cast<key_type&>(key)); \
			ret.second = ret.first == c.end(); \
			return \
				ret.second? \
					c.insert( \
						value_type( \
							boost::move(key), \
							mapped_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA) ))) \
					: ret; } \
		\
		template< typename Map \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) >  inline \
		typename \
			boost::disable_if< \
				detail::has_any_try_emplace< Map >, \
				std::pair< typename Map::iterator, bool> >::type \
			try_emplace_impl(Map& c, \
								const typename Map::key_type& key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef Map map_type; \
			typedef typename map_type::value_type value_type; \
			typedef typename map_type::key_type key_type; \
			typedef typename map_type::mapped_type mapped_type; \
			typedef typename map_type::iterator iter_type; \
			typedef std::pair<iter_type, bool> ret_type; \
			ret_type ret; \
			ret.first = c.find(key); \
			ret.second = ret.first == c.end(); \
			return \
				ret.second? \
					c.insert( \
						value_type( \
							key, \
							mapped_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA) ))) \
					: ret; } \
		\
		template< typename Map \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) >  inline \
		typename \
			boost::enable_if< \
				detail::has_any_try_emplace< Map >, \
				typename Map::iterator >::type \
			try_emplace_impl(Map& c, \
								typename Map::const_iterator hint, \
								BOOST_RV_REF( typename Map::key_type ) key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					hint, boost::move(key) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); } \
		\
		template< typename Map \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) >  inline \
		typename \
			boost::enable_if< \
				detail::has_any_try_emplace< Map >, \
				typename Map::iterator >::type \
			try_emplace_impl(Map& c, \
								typename Map::const_iterator hint, \
								const typename Map::key_type& key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return \
				c.try_emplace( \
					hint, key \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA)); } \
		\
		template< typename Map \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) >  inline \
		typename \
			boost::disable_if< \
				detail::has_any_try_emplace< Map >, \
				typename Map::iterator >::type \
			try_emplace_impl(Map& c, \
								typename Map::const_iterator hint, \
								BOOST_RV_REF(typename Map::key_type) key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef Map map_type; \
			typedef typename map_type::value_type value_type; \
			typedef typename map_type::key_type key_type; \
			typedef typename map_type::mapped_type mapped_type; \
			typedef typename map_type::iterator iter_type; \
			iter_type iter = c.find(static_cast<key_type&>(key)); \
			return \
				iter == c.end()? \
					c.insert( \
						value_type( \
							boost::move(key), \
							mapped_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA) ))).first \
					: iter; } \
		\
		template< typename Map \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) >  inline \
		typename \
			boost::disable_if< \
				detail::has_any_try_emplace< Map >, \
				typename Map::iterator >::type \
			try_emplace_impl(Map& c, \
								typename Map::const_iterator hint, \
								const typename Map::key_type& key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef Map map_type; \
			typedef typename map_type::value_type value_type; \
			typedef typename map_type::key_type key_type; \
			typedef typename map_type::mapped_type mapped_type; \
			typedef typename map_type::iterator iter_type; \
			iter_type iter = c.find(key); \
			return \
				iter == c.end()? \
					c.insert( \
						value_type( \
							key, \
							mapped_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA) ))).first \
					: iter; }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
			YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_TRY_EMPLACE_IMPL_HPP__
