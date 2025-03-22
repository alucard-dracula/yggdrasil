//push_pop_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_PUSH_POP_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_PUSH_POP_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/func/foo_t_info.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/bool.hpp>

#include <exception>

// push_front
#define YGGR_PP_SAFE_SEQ_PUSH_FRONT_IMPL() \
	bool push_front(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try { _base.push_front(boost::move(val)); return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	bool push_front(const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { _base.push_front(val); return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename Handler> \
	typename func::foo_t_info<Handler>::result_type \
		push_front(BOOST_RV_REF(value_type) val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		try { _base.push_front(boost::move(val)); \
				return handler(_base, true); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return handler(_base, false); } } \
	\
	template<typename Handler> \
	typename func::foo_t_info<Handler>::result_type \
		push_front(const value_type& val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		try { _base.push_front(val); \
			  return handler(_base, true); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return handler(_base, false); } }

// push_back
#define YGGR_PP_SAFE_SEQ_PUSH_BACK_IMPL() \
	bool push_back(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try { _base.push_back(boost::move(val)); return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	bool push_back(const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { _base.push_back(val); return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename Handler> \
	typename func::foo_t_info<Handler>::result_type \
		push_back(BOOST_RV_REF(value_type) val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		try { _base.push_back(boost::move(val)); \
			  return handler(_base, true); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return handler(_base, false); } } \
	\
	template<typename Handler> \
	typename func::foo_t_info<Handler>::result_type \
		push_back(const value_type& val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		try { _base.push_back(val); \
			  return handler(_base, true); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return handler(_base, false); } }
	
// pop_front
#define YGGR_PP_SAFE_SEQ_POP_FRONT_IMPL() \
	bool pop_front(void) { \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) && (_base.pop_front(), true); } \
	\
	bool pop_front(value_type& val) { \
		write_lock_type lk(_mutex); \
		return \
			(!_base.empty()) \
			&& (copy_or_move_or_swap(val, boost::move(_base.front())), \
				_base.pop_front(), \
				true); } \
	\
	template<typename Handler> \
	typename \
		boost::enable_if < \
			boost::mpl::and_< \
				boost::mpl::not_< boost::is_same<Handler, value_type> >, \
				typename ::yggr::func::foo_t_info<Handler>::is_callable_type >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		pop_front(const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_base, (!_base.empty()) && (_base.pop_front(), true)); } \
	\
	template<typename Handler> \
	typename func::foo_t_info<Handler>::result_type \
		pop_front(value_type& val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return \
			handler( \
				_base, \
				(!_base.empty()) \
				&& (copy_or_move_or_swap(val, boost::move(_base.front())), \
					_base.pop_front(), \
					true)); }

// pop_back
#define YGGR_PP_SAFE_SEQ_POP_BACK_IMPL() \
	bool pop_back(void) { \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) && (_base.pop_back(), true); } \
	\
	bool pop_back(value_type& val) { \
		write_lock_type lk(_mutex); \
		return \
			(!_base.empty()) \
			&& (copy_or_move_or_swap(val, _base.back()), \
				_base.pop_back(), \
				true); } \
	\
	template<typename Handler> \
	typename \
		boost::enable_if < \
			boost::mpl::and_< \
				boost::mpl::not_< boost::is_same<Handler, value_type> >, \
				typename ::yggr::func::foo_t_info<Handler>::is_callable_type >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		pop_back(const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_base, (!_base.empty()) && (_base.pop_back(), true)); } \
	\
	template<typename Handler> \
	typename func::foo_t_info<Handler>::result_type \
		pop_back(value_type& val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return \
			handler( \
				_base, \
				(!_base.empty()) \
				&& (copy_or_move_or_swap(val, _base.back()), \
					_base.pop_back(), \
					true)); }

#define YGGR_PP_SAFE_SEQ_WRAP_PUSH_IMPL() \
	bool push(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try { _base.push(boost::move(val)); return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	bool push(const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { _base.push(val); return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename Handler> \
	typename func::foo_t_info<Handler>::result_type \
		push(BOOST_RV_REF(value_type) val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		try { _base.push(boost::move(val)); \
			  return handler(_base, true); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return handler(_base, false); } } \
	\
	template<typename Handler> \
	typename func::foo_t_info<Handler>::result_type \
		push(const value_type& val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		try { _base.push(val); \
			  return handler(_base, true); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			handler(_base, false); } }

#define YGGR_PP_SAFE_SEQ_WRAP_POP_IMPL( __top_foo__ ) \
	bool pop(void) { \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) && (_base.pop(), true); } \
	\
	bool pop(value_type& val) { \
		write_lock_type lk(_mutex); \
		return \
			(!_base.empty()) \
			&& (copy_or_move_or_swap(val, boost::move(_base.__top_foo__())), \
				_base.pop(), \
				true); } \
	\
	template<typename Handler> \
	typename \
		boost::enable_if < \
			boost::mpl::and_< \
				boost::mpl::not_< boost::is_same<Handler, value_type> >, \
				typename ::yggr::func::foo_t_info<Handler>::is_callable_type >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		pop(const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_base, (!_base.empty()) && (_base.pop(), true)); } \
	\
	template<typename Handler> \
	typename func::foo_t_info<Handler>::result_type \
		pop(value_type& val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return \
			handler( \
				_base, \
				(!_base.empty()) \
				&& (copy_or_move_or_swap(val, boost::move(_base.__top_foo__())), \
					_base.pop(), \
					true) ); }


namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_PUSH_POP_IMPL_HPP__
