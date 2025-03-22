//emplace_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_EMPLACE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_EMPLACE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/bind/bind.hpp>
#include <yggr/move/move.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tags.hpp>

#include <yggr/func/foo_t_info.hpp>
#include <yggr/func/is_callable.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/utility/enable_if.hpp>

#include <iterator>

// emplace
// seq
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#	define YGGR_PP_SAFE_SEQ_EMPLACE_FUNCTION_IMPL( _ ) \
		template <typename IterHandler, typename ...Args> \
		bool emplace(const IterHandler& iter_handler, tag_non_handler, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { container::emplace(_base, iter_handler(_base), boost::forward<Args>(args)...); \
					return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template <typename IterHandler, typename Handler, typename ...Args> \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			emplace(const IterHandler& iter_handler, const Handler& handler, \
					BOOST_FWD_REF(Args)... args) { \
			iterator iter; \
			bool bright = false; \
			write_lock_type lk(_mutex); \
			try { iter = container::emplace(_base, iter_handler(_base), boost::forward<Args>(args)...); \
					bright = true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
			return handler(_base, std::make_pair(iter, bright)); }

#	define YGGR_PP_SAFE_SEQ_EMPLACE_FRONT_FUNCTION_IMPL( _ ) \
		template<typename ...Args> \
		bool emplace_front(BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { container::emplace_front(_base, boost::forward<Args>(args)...); \
				return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false;  } }

#	define YGGR_PP_SAFE_SEQ_EMPLACE_BACK_FUNCTION_IMPL( _ ) \
		template<typename ...Args> \
		bool emplace_back(BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { container::emplace_back(_base, boost::forward<Args>(args)...); \
					return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } }

// stack queue emplace
#	define YGGR_PP_SAFE_SEQ_EMPLACE_VALUE_IMPL( _ ) \
		template<typename ...Args> \
		bool emplace(BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { container::emplace(_base, boost::forward<Args>(args)...); \
					return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } }

// kv
// emplace
#	define YGGR_PP_SAFE_KV_UNIQUE_EMPLACE_FUNCTION_IMPL( _ ) \
		template<typename ...Args> \
		bool emplace(tag_non_handler, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return container::emplace(_base, boost::forward<Args>(args)...).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename Handler, typename ...Args> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			emplace(const Handler& handler, BOOST_FWD_REF(Args)... args) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret = container::emplace(_base, boost::forward<Args>(args)...); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
			return handler(_base, ret); }

#	define YGGR_PP_SAFE_KV_UNIQUE_TRY_EMPLACE_FUNCTION_IMPL( _ ) \
		template<typename ...Args> \
		bool try_emplace(tag_non_handler, BOOST_RV_REF(key_type) key, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return container::try_emplace(_base, boost::move(key), boost::forward<Args>(args)...).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename ...Args> \
		bool try_emplace(tag_non_handler, const key_type& key, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return container::try_emplace(_base, key, boost::forward<Args>(args)...).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename Handler, typename ...Args> \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const Handler& handler, BOOST_RV_REF(key_type) key, BOOST_FWD_REF(Args)... args) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret = container::try_emplace(_base, boost::move(key), boost::forward<Args>(args)...); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
			return handler(_base, ret); } \
		\
		template<typename Handler, typename ...Args> \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, const key_type& key, BOOST_FWD_REF(Args)... args) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret = container::try_emplace(_base, key, boost::forward<Args>(args)...); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
			return handler(_base, ret); } \
		\
		template<typename IterHandler, typename ...Args> \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, BOOST_RV_REF(key_type) key, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return \
					container::try_emplace( \
						_base, iter_handler(_base), boost::move(key), boost::forward<Args>(args)...) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename IterHandler, typename ...Args> \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, const key_type& key, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return \
					container::try_emplace( \
						_base, iter_handler(_base), key, boost::forward<Args>(args)...) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename IterHandler, typename Handler, typename ...Args> \
		typename boost::enable_if< \
			boost::mpl::and_<func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						BOOST_RV_REF(key_type) key, BOOST_FWD_REF(Args)... args) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret.first = container::try_emplace(_base, iter_handler(_base), boost::move(key), boost::forward<Args>(args)...); \
					ret.second = ret.first != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
			return handler(_base, ret); } \
		\
		template<typename IterHandler, typename Handler, typename ...Args> \
		typename boost::enable_if< \
			boost::mpl::and_<func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						const key_type& key, BOOST_FWD_REF(Args)... args) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret.first = container::try_emplace(_base, iter_handler(_base), key, boost::forward<Args>(args)...); \
					ret.second = ret.first != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
			return handler(_base, ret); }

#	define YGGR_PP_SAFE_KV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( _ ) \
		template<typename ...Args> \
		bool emplace(tag_non_handler, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return container::emplace(_base, boost::forward<Args>(args)...) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename Handler, typename ...Args> \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			emplace(const Handler& handler, BOOST_FWD_REF(Args)... args) { \
			iterator iter; \
			bool bright = false; \
			write_lock_type lk(_mutex); \
			try { iter = container::emplace(_base, boost::forward<Args>(args)...); bright = true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
			return handler(_base, std::make_pair(iter, bright) ); }

// emplace hit
#	define YGGR_PP_SAFE_KV_EMPLACE_HINT_FUNCTION_IMPL( _ ) \
		template< typename IterHandler, typename ...Args> \
		bool emplace_hint(const IterHandler& iter_handler, tag_non_handler, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return container::emplace_hint(_base, iter_handler(_base), boost::forward<Args>(args)...) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template< typename IterHandler, typename Handler, typename ...Args> \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			emplace_hint(const IterHandler& iter_handler, const Handler& handler, \
							BOOST_FWD_REF(Args)... args) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret.first = container::emplace_hint(_base, iter_handler(_base), boost::forward<Args>(args)...); \
					ret.second = ret.first != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
			return handler(_base, ret); }

// ukv
#define YGGR_PP_SAFE_UKV_UNIQUE_EMPLACE_FUNCTION_IMPL( _ ) \
	YGGR_PP_SAFE_KV_UNIQUE_EMPLACE_FUNCTION_IMPL( _ )

#define YGGR_PP_SAFE_UKV_EMPLACE_HINT_FUNCTION_IMPL( _ ) \
	YGGR_PP_SAFE_KV_EMPLACE_HINT_FUNCTION_IMPL( _ )

#define YGGR_PP_SAFE_UKV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( _ ) \
	YGGR_PP_SAFE_KV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( _ )

#define YGGR_PP_SAFE_UKV_UNIQUE_TRY_EMPLACE_FUNCTION_IMPL( _ ) \
	YGGR_PP_SAFE_KV_UNIQUE_TRY_EMPLACE_FUNCTION_IMPL( _ )

#else

// seq
// emplace

// I don't know how to do "is_constructiable" in previous versions of C++11
// so I use tag_non_handler to distinguish the parameter boundary

#	define YGGR_PP_SAFE_SEQ_EMPLACE_FUNCTION_IMPL( __n__ ) \
	public: \
		template< typename IterHandler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		bool emplace(const IterHandler& iter_handler, tag_non_handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
			write_lock_type lk(_mutex); \
			try { container::emplace( _base, iter_handler(_base) \
										YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
					return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template< typename IterHandler, typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			emplace( const IterHandler& iter_handler, const Handler& handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
			iterator iter; \
			bool bright = false; \
			write_lock_type lk(_mutex); \
			try { iter = container::emplace( \
							_base, iter_handler(_base) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
					bright = true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
			return handler( _base, std::make_pair(iter, bright)); } \

// emplace front
#	define YGGR_PP_SAFE_SEQ_EMPLACE_FRONT_FUNCTION_IMPL( __n__ ) \
	public: \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) \
		bool emplace_front( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { container::emplace_front(_base \
											YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
					return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \

// emplace back
#	define YGGR_PP_SAFE_SEQ_EMPLACE_BACK_FUNCTION_IMPL( __n__ ) \
	public: \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) \
		bool emplace_back( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { container::emplace_back(_base \
											YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
					return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } }

// stack queue emplace
#	define YGGR_PP_SAFE_SEQ_EMPLACE_VALUE_IMPL( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) \
		bool emplace( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { container::emplace(_base \
										YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
					return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } }

//kv

#	define YGGR_PP_SAFE_KV_UNIQUE_EMPLACE_FUNCTION_IMPL( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) \
		bool emplace(tag_non_handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { return container::emplace(_base YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
												YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false;} } \
		\
		template<typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			emplace(const Handler& handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				std::pair<iterator, bool> ret(iterator(), false); \
				write_lock_type lk(_mutex); \
				try { ret = container::emplace(_base YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
												YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
				return handler(_base, ret); }

#	define YGGR_PP_SAFE_KV_UNIQUE_TRY_EMPLACE_FUNCTION_IMPL( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) \
		bool try_emplace(tag_non_handler, BOOST_RV_REF(key_type) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { return \
					container::try_emplace( \
						_base, boost::move(key) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false;} } \
		\
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) \
		bool try_emplace(tag_non_handler, const key_type& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { return \
					container::try_emplace( \
						_base, key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false;} } \
		\
		template<typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, BOOST_RV_REF(key_type) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				std::pair<iterator, bool> ret(iterator(), false); \
				write_lock_type lk(_mutex); \
				try { ret = container::try_emplace( \
								_base, boost::move(key) \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
				return handler(_base, ret); } \
		\
		template<typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, const key_type& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				std::pair<iterator, bool> ret(iterator(), false); \
				write_lock_type lk(_mutex); \
				try { ret = container::try_emplace( \
								_base, key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
				return handler(_base, ret); } \
		\
		template<typename IterHandler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, \
							BOOST_RV_REF(key_type) key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { return \
					container::try_emplace( \
						_base, iter_handler(_base), boost::move(key) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false;} } \
		\
		template<typename IterHandler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, \
							const key_type& key \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { return \
					container::try_emplace( \
						_base, iter_handler(_base), key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false;} } \
		\
		template<typename IterHandler, typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						BOOST_RV_REF(key_type) key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				std::pair<iterator, bool> ret(iterator(), false); \
				write_lock_type lk(_mutex); \
				try { ret.first = container::try_emplace( \
								_base, iter_handler(_base), boost::move(key) \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
						ret.second = ret.first != _base.end(); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
				return handler(_base, ret); } \
		\
		template<typename IterHandler, typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						const key_type& key \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				std::pair<iterator, bool> ret(iterator(), false); \
				write_lock_type lk(_mutex); \
				try { ret.first = container::try_emplace( \
								_base, iter_handler(_base), key \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
						ret.second = ret.first != _base.end(); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
				return handler(_base, ret); }

#	define YGGR_PP_SAFE_KV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) \
		bool emplace(tag_non_handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { return \
					container::emplace(_base YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			emplace(const Handler& handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				iterator iter; \
				bool bright = false; \
				write_lock_type lk(_mutex); \
				try { iter = container::emplace(_base YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
												YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
						bright = true; } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
				return handler(_base, std::make_pair(iter, bright)); }


#	define YGGR_PP_SAFE_KV_EMPLACE_HINT_FUNCTION_IMPL( __n__ ) \
		template< typename IterHandler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		bool emplace_hint(const IterHandler& iter_handler, tag_non_handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try {  return \
					container::emplace_hint( \
						_base, iter_handler(_base) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template< typename IterHandler, typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			emplace_hint( const IterHandler& iter_handler, const Handler& handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret.first = container::emplace_hint( \
					_base, iter_handler(_base) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				  ret.second = ret.first != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
			return handler(_base, ret); }

// ukv
#define YGGR_PP_SAFE_UKV_UNIQUE_EMPLACE_FUNCTION_IMPL( __n__ ) \
	YGGR_PP_SAFE_KV_UNIQUE_EMPLACE_FUNCTION_IMPL( __n__ )

#define YGGR_PP_SAFE_UKV_EMPLACE_HINT_FUNCTION_IMPL( __n__ ) \
	YGGR_PP_SAFE_KV_EMPLACE_HINT_FUNCTION_IMPL( __n__ )

#define YGGR_PP_SAFE_UKV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( __n__ ) \
	YGGR_PP_SAFE_KV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( __n__ )

#define YGGR_PP_SAFE_UKV_UNIQUE_TRY_EMPLACE_FUNCTION_IMPL( __n__ ) \
	YGGR_PP_SAFE_KV_UNIQUE_TRY_EMPLACE_FUNCTION_IMPL( __n__ )

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


// ------------------------------opt_version------------------------
#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	define YGGR_PP_SAFE_KV_UNIQUE_OPT_TRY_EMPLACE_IMPL_0( ) \
		inline bool try_emplace(tag_non_handler, BOOST_RV_REF(key_type) key) { \
			return this_type::insert_detail(this_type::create_node(boost::move(key), mapped_type())); } \
		\
		inline bool try_emplace(tag_non_handler, const key_type& key) { \
			return this_type::insert_detail(this_type::create_node(key, mapped_type())); } \
		\
		template<typename Handler> inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, BOOST_RV_REF(key_type) key) { \
			return this_type::insert_detail(this_type::create_node(boost::move(key), mapped_type()), handler); } \
		\
		template<typename Handler> inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, const key_type& key) { \
			return this_type::insert_detail(this_type::create_node(key, mapped_type()), handler); } \
		\
		template<typename IterHandler> inline \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, BOOST_RV_REF(key_type) key) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(boost::move(key), mapped_type())); } \
		\
		template<typename IterHandler> inline \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, const key_type& key) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(key, mapped_type())); } \
		\
		template<typename IterHandler, typename Handler> inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, BOOST_RV_REF(key_type) key) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(boost::move(key), mapped_type()), handler); } \
		\
		template<typename IterHandler, typename Handler> inline \
		typename boost::enable_if< \
			boost::mpl::and_<func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, const key_type& key) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(key, mapped_type()), handler); }

#if defined(BOOST_CONTAINER_PERFECT_FORWARDING)

#	define YGGR_PP_SAFE_KV_OPT_EMPLACE_IMPL( _ ) \
		template<typename ...Args> inline \
		bool emplace(tag_non_handler, BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail(this_type::create_node(boost::forward<Args>(args)...)); } \
		\
		template<typename Handler, typename ...Args> inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			emplace(const Handler& handler, BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail(this_type::create_node(boost::forward<Args>(args)...), handler); } \
		\
		template< typename IterHandler, typename ...Args> inline \
		bool emplace_hint(const IterHandler& iter_handler, tag_non_handler, \
							BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(boost::forward<Args>(args)...)); } \
		\
		template< typename IterHandler, typename Handler, typename ...Args> inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			emplace_hint(const IterHandler& iter_handler, const Handler& handler, \
							BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(boost::forward<Args>(args)...), handler); }

#	define YGGR_PP_SAFE_KV_UNIQUE_OPT_TRY_EMPLACE_IMPL_N( _ ) \
		template<typename ArgZero, typename ...Args> inline \
		bool try_emplace(tag_non_handler, BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail( \
					this_type::create_node(boost::move(key), boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...)); } \
		\
		template<typename ArgZero, typename ...Args> inline \
		bool try_emplace(tag_non_handler, const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail( \
					this_type::create_node(key, boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...)); } \
		\
		template<typename Handler, typename ArgZero, typename ...Args> inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail( \
					this_type::create_node(boost::move(key), boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...), \
					handler); } \
		\
		template<typename Handler, typename ArgZero, typename ...Args> inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail( \
					this_type::create_node(key, boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...), \
					handler); } \
		\
		template<typename IterHandler, typename ArgZero, typename ...Args> inline \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, \
							BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail( \
					iter_handler, \
					this_type::create_node(boost::move(key), boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...)); } \
		\
		template<typename IterHandler, typename ArgZero, typename ...Args> inline \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, \
							const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail( \
					iter_handler, \
					this_type::create_node(key, boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...)); } \
		\
		template<typename IterHandler, typename Handler, typename ArgZero, typename ...Args> inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
							BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail( \
					iter_handler, \
					this_type::create_node(boost::move(key), boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...), \
					handler); } \
		\
		template<typename IterHandler, typename Handler, typename ArgZero, typename ...Args> inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			return this_type::insert_detail( \
						iter_handler, \
						this_type::create_node(key, boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...), \
						handler); }

#else

#	define YGGR_PP_SAFE_KV_OPT_EMPLACE_IMPL( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) inline \
		bool emplace(tag_non_handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( \
						this_type::create_node(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			emplace(const Handler& handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( \
						this_type::create_node(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )), \
						handler ); } \
		\
		template< typename IterHandler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool emplace_hint(const IterHandler& iter_handler, tag_non_handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( iter_handler, \
						this_type::create_node(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )));} \
		\
		template< typename IterHandler, typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			emplace_hint(const IterHandler& iter_handler, const Handler& handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( iter_handler, \
						this_type::create_node(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )), \
						handler ); }

#	define YGGR_PP_SAFE_KV_UNIQUE_OPT_TRY_EMPLACE_IMPL_N( __n__ ) \
		template<typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool try_emplace(tag_non_handler, BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( \
						this_type::create_node( \
							boost::move(key), boost::forward<ArgZero>(arg_zero) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool try_emplace(tag_non_handler, const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( \
						this_type::create_node( \
							key, boost::forward<ArgZero>(arg_zero) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename Handler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( \
						this_type::create_node( \
							boost::move(key), boost::forward<ArgZero>(arg_zero) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )), \
						handler ); } \
		\
		template<typename Handler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( \
						this_type::create_node( \
							key, boost::forward<ArgZero>(arg_zero) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )), \
						handler ); } \
		\
		template< typename IterHandler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, \
							BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( \
						iter_handler, \
						this_type::create_node( \
							boost::move(key), boost::forward<ArgZero>(arg_zero) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template< typename IterHandler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, \
							const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( \
						iter_handler, \
						this_type::create_node( \
							key, boost::forward<ArgZero>(arg_zero) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename IterHandler, typename Handler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( \
						iter_handler, \
						this_type::create_node( \
							boost::move(key), boost::forward<ArgZero>(arg_zero) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )), \
						handler ); } \
		\
		template<typename IterHandler, typename Handler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::insert_detail( \
						iter_handler, \
						this_type::create_node( \
							key, boost::forward<ArgZero>(arg_zero) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )), \
						handler ); }

#endif // BOOST_CONTAINER_PERFECT_FORWARDING


#define YGGR_PP_SAFE_UKV_HA_OPT_TRY_EMPLACE_FUNCTION_MAP_SP_IMPL_0() \
	inline bool try_emplace(tag_non_handler, BOOST_RV_REF(key_type) key) { \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node(key_hash, boost::move(key), mapped_type()); \
		return this_type::insert_detail(key_hash, pnode); } \
	\
	inline bool try_emplace(tag_non_handler, const key_type& key) { \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node(key_hash, key, mapped_type()); \
		return this_type::insert_detail(key_hash, pnode); } \
	\
	template<typename Handler> inline \
	typename boost::enable_if< \
		func::is_callable<Handler>, \
		typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		try_emplace(const Handler& handler, BOOST_RV_REF(key_type) key) { \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node(key_hash, boost::move(key), mapped_type()); \
		return this_type::insert_detail(key_hash, pnode, handler); } \
	\
	template<typename Handler> inline \
	typename boost::enable_if< \
		func::is_callable<Handler>, \
		typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		try_emplace(const Handler& handler, const key_type& key) { \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node(key_hash, key, mapped_type()); \
		return this_type::insert_detail(key_hash, pnode, handler); } \
	\
	template<typename IterHandler> inline \
	bool try_emplace(const IterHandler& iter_handler, tag_non_handler, BOOST_RV_REF(key_type) key) { \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node(key_hash, boost::move(key), mapped_type()); \
		return this_type::insert_detail(iter_handler, key_hash, pnode); } \
	\
	template<typename IterHandler> inline \
	bool try_emplace(const IterHandler& iter_handler, tag_non_handler, const key_type& key) { \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node(key_hash, key, mapped_type()); \
		return this_type::insert_detail(iter_handler, key_hash, pnode); } \
	\
	template<typename IterHandler, typename Handler> inline \
	typename boost::enable_if< \
		boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
		typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
		try_emplace(const IterHandler& iter_handler, const Handler& handler, BOOST_RV_REF(key_type) key) { \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node(key_hash, boost::move(key), mapped_type()); \
		return this_type::insert_detail(iter_handler, key_hash, pnode, handler); } \
	\
	template<typename IterHandler, typename Handler> inline \
	typename boost::enable_if< \
		boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
		typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
		try_emplace(const IterHandler& iter_handler, const Handler& handler, const key_type& key) { \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node(key_hash, key, mapped_type()); \
		return this_type::insert_detail(iter_handler, key_hash, pnode, handler); }


#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

// ukv_h

#	define YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_FUNCTION_IMPL( _ ) \
		template<typename ...Args> inline \
		bool emplace(tag_non_handler, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::forward<Args>(args)...); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		template<typename Handler, typename ...Args> inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			emplace(const Handler& handler, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::forward<Args>(args)...); \
			return this_type::insert_detail(key_hash, pnode, handler); }

#	define YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_HINT_FUNCTION_IMPL( _ ) \
		template< typename IterHandler, typename ...Args> inline \
		bool emplace_hint(const IterHandler& iter_handler, tag_non_handler, \
							BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::forward<Args>(args)...); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template< typename IterHandler, typename Handler, typename ...Args> inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			emplace_hint(const IterHandler& iter_handler, \
							const Handler& handler, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::forward<Args>(args)...); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); }

#	define YGGR_PP_SAFE_UKV_HA_OPT_TRY_EMPLACE_FUNCTION_IMPL_N( _ ) \
		template<typename ArgZero, typename ...Args> inline \
		bool try_emplace(tag_non_handler, BOOST_RV_REF(key_type) key, \
							BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					boost::move(key), boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		template<typename ArgZero, typename ...Args> inline \
		bool try_emplace(tag_non_handler, const key_type& key, \
							BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					key, boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		template<typename Handler, typename ArgZero, typename ...Args> inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, BOOST_RV_REF(key_type) key, \
						BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					boost::move(key), boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...); \
			return this_type::insert_detail(key_hash, pnode, handler); } \
		\
		template<typename Handler, typename ArgZero, typename ...Args> inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, \
						const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					key, boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...); \
			return this_type::insert_detail(key_hash, pnode, handler); } \
		\
		template<typename IterHandler, typename ArgZero, typename ...Args> inline \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, \
							BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					boost::move(key), boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template<typename IterHandler, typename ArgZero, typename ...Args> inline \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, \
							const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					key, boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template<typename IterHandler, typename Handler, typename ArgZero, typename ...Args> inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					boost::move(key), boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); } \
		\
		template<typename IterHandler, typename Handler, typename ArgZero, typename ...Args> inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero, BOOST_FWD_REF(Args)... args) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					key, boost::forward<ArgZero>(arg_zero), boost::forward<Args>(args)...); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); }

#else

// ukv_h

#	define YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_FUNCTION_IMPL( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) inline \
		bool emplace(tag_non_handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		template<typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			emplace(const Handler& handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return this_type::insert_detail(key_hash, pnode, handler); }

#	define YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_HINT_FUNCTION_IMPL( __n__ ) \
		template< typename IterHandler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool emplace_hint(const IterHandler& iter_handler, \
							tag_non_handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template< typename IterHandler, typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			emplace_hint( const IterHandler& iter_handler, \
							const Handler& handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); }

#	define YGGR_PP_SAFE_UKV_HA_OPT_TRY_EMPLACE_FUNCTION_IMPL_N( __n__ ) \
		template<typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool try_emplace(tag_non_handler, \
							BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					boost::move(key), boost::forward<ArgZero>(arg_zero) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		template<typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool try_emplace(tag_non_handler, \
							const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					key, boost::forward<ArgZero>(arg_zero) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		template<typename Handler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, \
						BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					boost::move(key), boost::forward<ArgZero>(arg_zero) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return this_type::insert_detail(key_hash, pnode, handler); } \
		\
		template<typename Handler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			try_emplace(const Handler& handler, \
						const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					key, boost::forward<ArgZero>(arg_zero) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return this_type::insert_detail(key_hash, pnode, handler); } \
		\
		template<typename IterHandler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, \
							BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					boost::move(key), boost::forward<ArgZero>(arg_zero) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template<typename IterHandler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool try_emplace(const IterHandler& iter_handler, tag_non_handler, \
							const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					key, boost::forward<ArgZero>(arg_zero) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template<typename IterHandler, typename Handler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						BOOST_RV_REF(key_type) key, BOOST_FWD_REF(ArgZero) arg_zero \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					boost::move(key), boost::forward<ArgZero>(arg_zero) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); } \
		\
		template<typename IterHandler, typename Handler, typename ArgZero \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			try_emplace(const IterHandler& iter_handler, const Handler& handler, \
						const key_type& key, BOOST_FWD_REF(ArgZero) arg_zero \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = \
				this_type::create_node( \
					key_hash, \
					key, boost::forward<ArgZero>(arg_zero) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); }


#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_EMPLACE_IMPL_HPP__
