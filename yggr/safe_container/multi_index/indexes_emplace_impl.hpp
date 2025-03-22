// indexes_emplace_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_EMPLACE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_EMPLACE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>

#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/bind/bind.hpp>
#include <yggr/move/move.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tags.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/func/foo_t_info.hpp>
#include <yggr/func/is_callable.hpp>
#include <yggr/container/multi_index_config.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>

#include <boost/preprocessor/logical/not.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/utility/enable_if.hpp>

#include <iterator>


#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

// linear
#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_IMPL( _ ) \
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
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			emplace(const IterHandler& iter_handler, \
					const Handler& handler, BOOST_FWD_REF(Args)... args) { \
			typedef boost::fusion::pair<index_id_type, std::pair<iterator, bool> > ret_type; \
			std::pair<iterator, bool> ret_val(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret_val = container::emplace(_base, iter_handler(_base), boost::forward<Args>(args)...); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret_val.first = _base.end(); } \
			return handler(_final, ret_type(ret_val)); }

#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_FRONT_IMPL( _ ) \
		template<typename ...Args> \
		bool emplace_front(tag_non_handler, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return container::emplace_front(_base, boost::forward<Args>(args)...).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false;  } } \
		\
		template<typename Handler, typename ...Args> \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			emplace_front(const Handler& handler, BOOST_FWD_REF(Args)... args) { \
			typedef boost::fusion::pair<index_id_type, std::pair<iterator, bool> > ret_type; \
			std::pair<iterator, bool> ret_val(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret_val = container::emplace_front(_base, boost::forward<Args>(args)...); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret_val.first = _base.end(); } \
			return handler(_final, ret_type(ret_val)); }

#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_BACK_IMPL( _ ) \
		template<typename ...Args> \
		bool emplace_back(tag_non_handler, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return container::emplace_back(_base, boost::forward<Args>(args)...).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename Handler, typename ...Args> \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			emplace_back(const Handler& handler, BOOST_FWD_REF(Args)... args) { \
			typedef boost::fusion::pair<index_id_type, std::pair<iterator, bool> > ret_type; \
			std::pair<iterator, bool> ret_val(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret_val = container::emplace_back(_base, boost::forward<Args>(args)...); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret_val.first = _base.end(); } \
			return handler(_final, ret_type(ret_val)); }

// kv

#	define YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_IMPL( _ ) \
		template<typename ...Args> \
		bool emplace(tag_non_handler, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return container::emplace(_base, boost::forward<Args>(args)...).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename Handler, typename ...Args> \
		typename boost::enable_if< \
			func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			emplace(const Handler& handler, BOOST_FWD_REF(Args)... args) { \
			typedef boost::fusion::pair<index_id_type, std::pair<iterator, bool> > ret_type; \
			std::pair<iterator, bool> ret_val(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret_val = container::emplace(_base, boost::forward<Args>(args)...); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret_val.first = _base.end(); } \
			return handler(_final, ret_type(ret_val)); }


#	define YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_HINT_IMPL( _ ) \
		template< typename IterHandler, typename ...Args> \
		bool emplace_hint(const IterHandler& iter_handler, tag_non_handler, BOOST_FWD_REF(Args)... args) { \
			write_lock_type lk(_mutex); \
			try { return \
					container::emplace_hint( \
						_base, iter_handler(_base), boost::forward<Args>(args)...) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template< typename IterHandler, typename Handler, typename ...Args> \
		typename boost::enable_if< \
			boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
			emplace_hint(const IterHandler& iter_handler, const Handler& handler, \
							BOOST_FWD_REF(Args)... args) { \
			typedef boost::fusion::pair<index_id_type, std::pair<iterator, bool> > ret_type; \
			iterator iter; \
			bool bright = false; \
			write_lock_type lk(_mutex); \
			try { iter = container::emplace_hint(_base, iter_handler(_base), boost::forward<Args>(args)...); \
					bright = (iter != _base.end()); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
			return handler(_final, ret_type(std::make_pair(iter, bright))); }
		
#	define YGGR_PP_SAFE_MIDX_UKV_INDEXES_EMPLACE_IMPL( _ ) \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_IMPL( _ )

#	define YGGR_PP_SAFE_MIDX_UKV_INDEXES_EMPLACE_HINT_IMPL( _ ) \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_HINT_IMPL( _ )

#else
// linear

#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_IMPL( __n__ ) \
		template< typename IterHandler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		bool emplace(const IterHandler& iter_handler, \
						tag_non_handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
			write_lock_type lk(_mutex); \
			try { return container::emplace( _base, iter_handler(_base) \
										YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template< typename IterHandler, typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			emplace( const IterHandler& iter_handler, \
						const Handler& handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
			typedef boost::fusion::pair<index_id_type, std::pair<iterator, bool> > ret_type; \
			std::pair<iterator, bool> ret_val(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret_val = container::emplace( \
								_base, iter_handler(_base) YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret_val.first = _base.end(); } \
			return handler(_final, ret_type(ret_val)); } \

// emplace front
#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_FRONT_IMPL( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) \
		bool emplace_front( tag_non_handler YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { return container::emplace_front( \
							_base YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template< typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			emplace_front( const Handler& handler YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef boost::fusion::pair<index_id_type, std::pair<iterator, bool> > ret_type; \
			std::pair<iterator, bool> ret_val(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret_val = container::emplace_front( \
								_base YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret_val.first = _base.end(); } \
			return handler(_final, ret_type(ret_val)); }

// emplace back
#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_EMPLACE_BACK_IMPL( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, template<) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF(__n__, >) \
		bool emplace_back( tag_non_handler YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try { return container::emplace_back( \
							_base YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template< typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			emplace_back( const Handler& handler YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef boost::fusion::pair<index_id_type, std::pair<iterator, bool> > ret_type; \
			std::pair<iterator, bool> ret_val(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { ret_val = container::emplace_back( \
								_base YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret_val.first = _base.end(); } \
			return handler(_final, ret_type(ret_val)); }

// kv
#	define YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_IMPL( __n__ ) \
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
				typedef boost::fusion::pair<index_id_type, std::pair<iterator, bool> > ret_type; \
				std::pair<iterator, bool> ret_val(iterator(), false); \
				write_lock_type lk(_mutex); \
				try { ret_val = container::emplace(_base YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
												YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret_val.first = _base.end(); } \
				return handler(_final, ret_type(ret_val)); }


#	define YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_HINT_IMPL( __n__ ) \
		template< typename IterHandler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		bool emplace_hint(const IterHandler& iter_handler, \
							tag_non_handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			write_lock_type lk(_mutex); \
			try {  return container::emplace_hint( \
							_base, iter_handler(_base) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ) \
							!= _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template< typename IterHandler, typename Handler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			emplace_hint( const IterHandler& iter_handler, \
							const Handler& handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			typedef boost::fusion::pair<index_id_type, std::pair<iterator, bool> > ret_type; \
			iterator iter; \
			bool bright = false; \
			write_lock_type lk(_mutex); \
			try { iter = container::emplace_hint( \
					_base, iter_handler(_base) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				  bright = true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
			return handler(_final, ret_type(std::make_pair(iter, bright))); }


#	define YGGR_PP_SAFE_MIDX_UKV_INDEXES_EMPLACE_IMPL( __n__ ) \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_IMPL( __n__ )

#	define YGGR_PP_SAFE_MIDX_UKV_INDEXES_EMPLACE_HINT_IMPL( __n__ ) \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_HINT_IMPL( __n__ )

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

//----------------opt version------------------

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

// linear_h_opt
#		define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_EMPLACE_IMPL( _ ) \
			template <typename IterHandler, typename ...Args> inline \
			bool emplace(const IterHandler& iter_handler, tag_non_handler, BOOST_FWD_REF(Args)... args) { \
				value_type tmp = value_type(boost::forward<Args>(args)...); \
				return this_type::insert(iter_handler, boost::move(tmp)); } \
			\
			template <typename IterHandler, typename Handler, typename ...Args> inline \
			typename boost::enable_if< \
				boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
				typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
				emplace(const IterHandler& iter_handler, \
						const Handler& handler, BOOST_FWD_REF(Args)... args) { \
				value_type tmp = value_type(boost::forward<Args>(args)...); \
				return this_type::insert(iter_handler, boost::move(tmp), handler); }

#		define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_EMPLACE_FRONT_IMPL( _ ) \
			template<typename ...Args> inline \
			bool emplace_front(tag_non_handler, BOOST_FWD_REF(Args)... args) { \
				value_type tmp = value_type(boost::forward<Args>(args)...); \
				return this_type::push_front(boost::move(tmp)); } \
			\
			template<typename Handler, typename ...Args> inline \
			typename boost::enable_if< \
				func::is_callable<Handler>, \
				typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
				emplace_front(const Handler& handler, BOOST_FWD_REF(Args)... args) { \
				value_type tmp = value_type(boost::forward<Args>(args)...); \
				return this_type::push_front(boost::move(tmp), handler); }

#		define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_EMPLACE_BACK_IMPL( _ ) \
			template<typename ...Args> inline \
			bool emplace_back(tag_non_handler, BOOST_FWD_REF(Args)... args) { \
				value_type tmp = value_type(boost::forward<Args>(args)...); \
				return this_type::push_back(boost::move(tmp)); } \
			\
			template<typename Handler, typename ...Args> inline \
			typename boost::enable_if< \
				func::is_callable<Handler>, \
				typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
				emplace_back(const Handler& handler, BOOST_FWD_REF(Args)... args) { \
				value_type tmp = value_type(boost::forward<Args>(args)...); \
				return this_type::push_back(boost::move(tmp), handler); }

// kv_h_opt
#		define YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_EMPLACE_IMPL( _ ) \
			template<typename ...Args> inline \
			bool emplace(tag_non_handler, BOOST_FWD_REF(Args)... args) { \
				value_type tmp = value_type(boost::forward<Args>(args)...); \
				return this_type::insert(boost::move(tmp)); } \
			\
			template<typename Handler, typename ...Args> inline \
			typename boost::enable_if< \
				func::is_callable<Handler>, \
				typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
				emplace(const Handler& handler, BOOST_FWD_REF(Args)... args) { \
				value_type tmp = value_type(boost::forward<Args>(args)...); \
				return this_type::insert(boost::move(tmp), handler); }

#		define YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_EMPLACE_HINT_IMPL( _ ) \
			template< typename IterHandler, typename ...Args> inline \
			bool emplace_hint(const IterHandler& iter_handler, tag_non_handler, BOOST_FWD_REF(Args)... args) { \
				value_type tmp = value_type(boost::forward<Args>(args)...); \
				return this_type::insert(iter_handler, boost::move(tmp)); } \
			\
			template< typename IterHandler, typename Handler, typename ...Args> inline \
			typename boost::enable_if< \
				boost::mpl::and_< func::is_callable<IterHandler>, func::is_callable<Handler> >, \
				typename ::yggr::func::foo_t_info<Handler>::result_type>::type \
				emplace_hint(const IterHandler& iter_handler, const Handler& handler, \
								BOOST_FWD_REF(Args)... args) { \
				value_type tmp(boost::forward<Args>(args)...); \
				return this_type::insert(iter_handler, boost::move(tmp), handler); }

#		define YGGR_PP_SAFE_MIDX_UKV_INDEXES_OPT_EMPLACE_IMPL( _ ) \
			YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_EMPLACE_IMPL( _ )

#		define YGGR_PP_SAFE_MIDX_UKV_INDEXES_OPT_EMPLACE_HINT_IMPL( _ ) \
			YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_EMPLACE_HINT_IMPL( _ )			

#	else

// linear

#		define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_EMPLACE_IMPL( __n__ ) \
			template< typename IterHandler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
			bool emplace(const IterHandler& iter_handler, \
							tag_non_handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
				value_type tmp = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return this_type::insert(iter_handler, boost::move(tmp)); } \
			\
			template< typename IterHandler, typename Handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
			typename ::yggr::func::foo_t_info<Handler>::result_type \
				emplace( const IterHandler& iter_handler, \
							const Handler& handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS )) { \
				value_type tmp = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return this_type::insert(iter_handler, boost::move(tmp), handler); }

// emplace front
#		define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_EMPLACE_FRONT_IMPL( __n__ ) \
			BOOST_PP_EXPR_IF(__n__, template<) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) \
			BOOST_PP_EXPR_IF(__n__, >) inline \
			bool emplace_front( tag_non_handler YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				value_type tmp = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return this_type::push_front(boost::move(tmp)); } \
			\
			template< typename Handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
			typename ::yggr::func::foo_t_info<Handler>::result_type \
				emplace_front( const Handler& handler YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				value_type tmp = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return this_type::push_front(boost::move(tmp), handler); }

// emplace back
#		define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_EMPLACE_BACK_IMPL( __n__ ) \
			BOOST_PP_EXPR_IF(__n__, template<) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) \
			BOOST_PP_EXPR_IF(__n__, >) inline \
			bool emplace_back( tag_non_handler YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				value_type tmp = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return this_type::push_back(boost::move(tmp)); } \
			\
			template< typename Handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
			typename ::yggr::func::foo_t_info<Handler>::result_type \
				emplace_back( const Handler& handler YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				value_type tmp = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return this_type::push_back(boost::move(tmp), handler); }

// kv_h_opt
#		define YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_EMPLACE_IMPL( __n__ ) \
			BOOST_PP_EXPR_IF(__n__, template<) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) \
			BOOST_PP_EXPR_IF(__n__, >) inline \
			bool emplace(tag_non_handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				value_type tmp = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
				return this_type::insert(boost::move(tmp)); } \
			\
			template<typename Handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
			typename ::yggr::func::foo_t_info<Handler>::result_type \
				emplace(const Handler& handler \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
					value_type tmp = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
					return this_type::insert(boost::move(tmp), handler); }


#		define YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_EMPLACE_HINT_IMPL( __n__ ) \
			template< typename IterHandler \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
			bool emplace_hint(const IterHandler& iter_handler, \
								tag_non_handler \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				value_type tmp = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
				return this_type::insert(iter_handler, boost::move(tmp)); } \
			\
			template< typename IterHandler, typename Handler \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
			typename ::yggr::func::foo_t_info<Handler>::result_type \
				emplace_hint( const IterHandler& iter_handler, \
								const Handler& handler \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
				value_type tmp = value_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
				return this_type::insert(iter_handler, boost::move(tmp), handler); }


// kv_uh_opt
#		define YGGR_PP_SAFE_MIDX_UKV_INDEXES_OPT_EMPLACE_IMPL( __n__ ) \
			YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_EMPLACE_IMPL( __n__ )

#		define YGGR_PP_SAFE_MIDX_UKV_INDEXES_OPT_EMPLACE_HINT_IMPL( __n__ ) \
			YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_EMPLACE_HINT_IMPL( __n__ )

#	endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION


namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_EMPLACE_IMPL_HPP__
