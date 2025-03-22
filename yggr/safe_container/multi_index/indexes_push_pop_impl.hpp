//indexes_push_pop_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_PUSH_POP_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_PUSH_POP_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/bool.hpp>

#define YGGR_PP_SAFE_MIDX_INDEXES_PUSH_FRONT_IMPL() \
	bool push_front(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try { return _base.push_front(boost::move(val)).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	bool push_front(const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { return _base.push_front(val).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		push_front(BOOST_RV_REF(value_type) val, const Handler& handler) { \
		std::pair<iterator, bool> rst(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { rst = _base.push_front(boost::move(val)); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) rst.first = _base.end(); } \
		return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								std::pair<iterator, bool> >(rst)); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		push_front(const value_type& val, const Handler& handler) { \
		std::pair<iterator, bool> rst(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { rst = _base.push_front(val); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) rst.first = _base.end(); } \
		return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								std::pair<iterator, bool> >(rst)); }

#define YGGR_PP_SAFE_MIDX_INDEXES_POP_FRONT_IMPL() \
	bool pop_front(void) { \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) && (_base.pop_front(), true); } \
	\
	bool pop_front(value_type& val) { \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) && (val = _base.front(), _base.pop_front(), true); } \
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
		return handler( \
				_final, \
				boost::fusion::pair<index_id_type, bool >( \
					(!_base.empty()) && (_base.pop_front(), true) ) ); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		pop_front(value_type& val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler( \
				_final, \
				boost::fusion::pair<index_id_type, bool >( \
					(!_base.empty()) && (val = _base.front(), _base.pop_front(), true) ) ); }


#define YGGR_PP_SAFE_MIDX_INDEXES_PUSH_BACK_IMPL() \
	bool push_back(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try { return _base.push_back(boost::move(val)).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	bool push_back(const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { return _base.push_back(val).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		push_back(BOOST_RV_REF(value_type) val, const Handler& handler) { \
		std::pair<iterator, bool> rst(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { rst = _base.push_back(boost::move(val)); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) rst.first = _base.end(); } \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							std::pair<iterator, bool> >(rst)); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		push_back(const value_type& val, const Handler& handler) { \
		std::pair<iterator, bool> rst(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { rst = _base.push_back(val); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) rst.first = _base.end(); } \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							std::pair<iterator, bool> >(rst)); }


#define YGGR_PP_SAFE_MIDX_INDEXES_POP_BACK_IMPL() \
	bool pop_back(void) { \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) && (_base.pop_back(), true); } \
	\
	bool pop_back(value_type& val) { \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) && (val = _base.back(), _base.pop_back(), true); } \
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
		return handler( \
				_final, \
				boost::fusion::pair<index_id_type, bool >( \
					(!_base.empty()) && (_base.pop_back(), true) ) ); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		pop_back(value_type& val, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler( \
				_final, \
				boost::fusion::pair<index_id_type, bool >( \
					(!_base.empty()) && (val = _base.back(), _base.pop_back(), true) ) ); }

//---------------opt version-------------

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

// ha_opt
// push_front
#	define YGGR_PP_SAFE_MIDX_INDEXES_HA_OPT_PUSH_FRONT_IMPL() \
		bool push_front(BOOST_RV_REF(value_type) val) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, boost::move(val)); \
			if(!pnode) { return false; } \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			try { return opt_cast().push_front(detail::allocator_destroy_holder_ref(holder), \
													keyinfo_params, pnode).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
		\
		bool push_front(const value_type& val) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, val); \
			if(!pnode) { return false; } \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			try { return opt_cast().push_front(holder, keyinfo_params, pnode).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			push_front(BOOST_RV_REF(value_type) val, const Handler& handler) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, boost::move(val)); \
			destroy_holder_type holder(alloc); \
			std::pair<iterator, bool> rst(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { rst = pnode? \
					opt_cast().push_front(holder, keyinfo_params, pnode) \
					: std::make_pair(_base.end(), false); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) rst.first = _base.end(); } \
			return handler(_final, \
								boost::fusion::pair< \
									index_id_type, \
									std::pair<iterator, bool> >(rst)); } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			push_front(const value_type& val, const Handler& handler) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, val); \
			destroy_holder_type holder(alloc); \
			std::pair<iterator, bool> rst(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { rst = pnode? \
					opt_cast().push_front(holder, keyinfo_params, pnode) \
					: std::make_pair(_base.end(), false); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) rst.first = _base.end(); } \
			return handler(_final, \
								boost::fusion::pair< \
									index_id_type, \
									std::pair<iterator, bool> >(rst)); }

#	define YGGR_PP_SAFE_MIDX_INDEXES_HA_OPT_POP_FRONT_IMPL() \
		bool pop_front(void) { \
			destroy_holder_type holder(this_type::get_allocator()); { \
				write_lock_type lk(_mutex); \
				if(_base.empty()) { return false; } \
				opt_cast().pop_front(holder); } \
			assert(holder.size() == 1); \
			return true; } \
		\
		bool pop_front(value_type& val) { \
			destroy_holder_type holder(this_type::get_allocator()); { \
				write_lock_type lk(_mutex); \
				if(_base.empty()) { return false; } \
				opt_cast().pop_front(holder); } \
			assert(holder.size() == 1); \
			return holder.get_value(0, val); } \
		\
		template<typename Handler> \
		typename \
			boost::enable_if < \
				boost::mpl::and_< \
					boost::mpl::not_< boost::is_same<Handler, value_type> >, \
					typename ::yggr::func::foo_t_info<Handler>::is_callable_type >, \
				typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			pop_front(const Handler& handler) { \
			destroy_holder_type holder(this_type::get_allocator()); { \
				write_lock_type lk(_mutex); \
				if(_base.empty()) { \
					return \
						handler( \
							_final, \
							boost::fusion::pair<index_id_type, bool >( false ) ); } \
				opt_cast().pop_front(holder); } \
			assert(holder.size() == 1); \
			return \
				handler( \
					_final, \
					boost::fusion::pair<index_id_type, bool >( true ) ); } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			pop_front(value_type& val, const Handler& handler) { \
			destroy_holder_type holder(this_type::get_allocator()); { \
				write_lock_type lk(_mutex); \
				if(_base.empty()) { \
					return \
						handler( \
							_final, \
							boost::fusion::pair<index_id_type, bool >( false ) ); } \
				opt_cast().pop_front(holder); } \
			assert(holder.size() == 1); \
			return \
				handler( \
					_final, \
					boost::fusion::pair<index_id_type, bool >( holder.get_value(0, val) ) ); }

// push_back
#	define YGGR_PP_SAFE_MIDX_INDEXES_HA_OPT_PUSH_BACK_IMPL() \
		bool push_back(BOOST_RV_REF(value_type) val) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, boost::move(val)); \
			if(!pnode) { return false; } \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			try { return opt_cast().push_back(detail::allocator_destroy_holder_ref(holder), \
													keyinfo_params, pnode).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
		\
		bool push_back(const value_type& val) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, val); \
			if(!pnode) { return false; } \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			try { return opt_cast().push_back(holder, keyinfo_params, pnode).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			push_back(BOOST_RV_REF(value_type) val, const Handler& handler) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, boost::move(val)); \
			destroy_holder_type holder(alloc); \
			std::pair<iterator, bool> rst(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { rst = pnode? \
					opt_cast().push_back(holder, keyinfo_params, pnode) \
					: std::make_pair(_base.end(), false); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) rst.first = _base.end(); } \
			return handler(_final, \
								boost::fusion::pair< \
									index_id_type, \
									std::pair<iterator, bool> >(rst)); } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			push_back(const value_type& val, const Handler& handler) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, val); \
			destroy_holder_type holder(alloc); \
			std::pair<iterator, bool> rst(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { rst = pnode? \
					opt_cast().push_back(holder, keyinfo_params, pnode) \
					: std::make_pair(_base.end(), false); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) rst.first = _base.end(); } \
			return handler(_final, \
								boost::fusion::pair< \
									index_id_type, \
									std::pair<iterator, bool> >(rst)); }

#	define YGGR_PP_SAFE_MIDX_INDEXES_HA_OPT_POP_BACK_IMPL() \
		bool pop_back(void) { \
			destroy_holder_type holder(this_type::get_allocator()); { \
				write_lock_type lk(_mutex); \
				if(_base.empty()) { return false; } \
				opt_cast().pop_back(holder); } \
			assert(holder.size() == 1); \
			return true; } \
		\
		bool pop_back(value_type& val) { \
			destroy_holder_type holder(this_type::get_allocator()); { \
				write_lock_type lk(_mutex); \
				if(_base.empty()) { return false; } \
				opt_cast().pop_back(holder); } \
			assert(holder.size() == 1); \
			return holder.get_value(0, val); } \
		\
		template<typename Handler> \
		typename \
			boost::enable_if < \
				boost::mpl::and_< \
					boost::mpl::not_< boost::is_same<Handler, value_type> >, \
					typename ::yggr::func::foo_t_info<Handler>::is_callable_type >, \
				typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			pop_back(const Handler& handler) { \
			destroy_holder_type holder(this_type::get_allocator()); { \
				write_lock_type lk(_mutex); \
				if(_base.empty()) { \
					return \
						handler( \
							_final, \
							boost::fusion::pair<index_id_type, bool >( false ) ); } \
				opt_cast().pop_back(holder); } \
			assert(holder.size() == 1); \
			return \
				handler( \
					_final, \
					boost::fusion::pair<index_id_type, bool >( true ) ); } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			pop_back(value_type& val, const Handler& handler) { \
			destroy_holder_type holder(this_type::get_allocator()); { \
				write_lock_type lk(_mutex); \
				if(_base.empty()) { \
					return \
						handler( \
							_final, \
							boost::fusion::pair<index_id_type, bool >( false ) ); } \
				opt_cast().pop_back(holder); } \
			assert(holder.size() == 1); \
			return \
				handler( \
					_final, \
					boost::fusion::pair<index_id_type, bool >( holder.get_value(0, val) ) ); }



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

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_PUSH_POP_IMPL_HPP__

