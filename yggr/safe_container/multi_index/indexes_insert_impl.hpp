// indexes_insert_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_INSERT_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_INSERT_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/compatibility/stl_exception.hpp>

#include <yggr/func/foo_t_info.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/detail/boost_fusion_pair_midx_node_handle_patch.hpp>

#include <utility>

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#   include <initializer_list>
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#if BOOST_VERSION < 107400
#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_INSERT_OF_NODE_HANDLER_RET_MAKE(__inner_ret__, __ins_ret__) \
		boost::fusion::pair<index_id_type, base_insert_return_type> __inner_ret__; \
		inner_ret.second = boost::move(__ins_ret__);
#else
#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_INSERT_OF_NODE_HANDLER_RET_MAKE(__inner_ret__, __ins_ret__) \
		boost::fusion::pair<index_id_type, base_insert_return_type> __inner_ret__(boost::move(__ins_ret__));
#endif // BOOST_VERSION < 107400


#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#	define YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_IL_IMPL() \
		inline bool insert(std::initializer_list<value_type> il) { \
			return this_type::insert(il.begin(), il.end()); }

#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_IL_IMPL() \
		template<typename IterHandler> inline \
		bool insert(const IterHandler& iter_handler, \
						std::initializer_list<value_type> il) { \
			return this_type::insert(iter_handler, il.begin(), il.end()); }

#else

#	define YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_IL_IMPL()

#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_IL_IMPL()

#endif //YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_OF_SIZE_IMPL() \
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, size_type size, const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { _base.insert(iter_handler(_base), size, val); return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } }

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_OF_ITER_FL_IMPL() \
	template<typename IterHandler, typename InputIter> \
	bool insert(const IterHandler& iter_handler, InputIter first, InputIter last) { \
		write_lock_type lk(_mutex); \
		try { _base.insert(iter_handler(_base), first, last); return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } }

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_OF_VALUE_IMPL() \
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try { return _base.insert(iter_handler(_base), boost::move(val)).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { return _base.insert(iter_handler(_base), val).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	template<typename IterHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val, const Handler& handler) { \
		std::pair<iterator, bool> rst(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { rst = _base.insert(iter_handler(_base), boost::move(val)); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			rst.first = _base.end(); \
			rst.second = false; } \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							std::pair<iterator, bool> >(rst)); } \
	\
	template<typename IterHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, const value_type& val, const Handler& handler) { \
		std::pair<iterator, bool> rst(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { rst = _base.insert(iter_handler(_base), val); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			rst.first = _base.end(); \
			rst.second = false; } \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							std::pair<iterator, bool> >(rst)); }

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_OF_NODE_IMPL() \
	template<typename IterHandler> \
	insert_return_type insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh) { \
		write_lock_type lk(_mutex); \
		try{ return insert_return_type(container::insert(_base, iter_handler(_base), boost::move(nh))); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return insert_return_type(false, boost::move(nh)); } } \
	\
	template<typename IterHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh, const Handler& handler) { \
		base_insert_return_type ins_ret(iterator(), false, node_type()); \
		write_lock_type lk(_mutex); \
		try { ins_ret = container::insert(_base, iter_handler(_base), boost::move(nh)); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			ins_ret = base_insert_return_type(_base.end(), false, boost::move(nh)); } \
		YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_INSERT_OF_NODE_HANDLER_RET_MAKE(inner_ret, ins_ret); \
		return handler(_final, inner_ret); }


#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_IMPL() \
	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_OF_VALUE_IMPL() \
	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_OF_SIZE_IMPL() \
	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_OF_ITER_FL_IMPL() \
	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_IL_IMPL() \
	YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_OF_NODE_IMPL()


// kv
#define YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_ITER_FL_IMPL() \
	template<typename InputIter> \
	bool insert(InputIter first, InputIter last) { \
		write_lock_type lk(_mutex); \
		try { _base.insert(first, last); return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } }


#define YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_ITER_IMPL() \
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try { return _base.insert(iter_handler(_base), boost::move(val)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { return _base.insert(iter_handler(_base), val) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
	\
	template<typename IterHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val, const Handler& handler) { \
		std::pair<iterator, bool> rst(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { rst.first = _base.insert(iter_handler(_base), boost::move(val)); \
				rst.second = true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			rst.first = _base.end(); \
			rst.second = false; } \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							std::pair<iterator, bool> >(rst)); } \
	\
	template<typename IterHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, const value_type& val, const Handler& handler) { \
		std::pair<iterator, bool> rst(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { rst.first = _base.insert(iter_handler(_base), val); \
				rst.second = true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			rst.first = _base.end(); \
			rst.second = false; } \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							std::pair<iterator, bool> >(rst)); }


#define YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_VALUE_IMPL() \
	bool insert(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try{ return _base.insert(boost::move(val)).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	bool insert(const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { return _base.insert(val).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(BOOST_RV_REF(value_type) val, const Handler& handler) { \
		std::pair<iterator, bool> rst(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { rst = _base.insert(boost::move(val)); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			rst.first = _base.end(); } \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							std::pair<iterator, bool> >(rst)); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const value_type& val, const Handler& handler) { \
		std::pair<iterator, bool> rst(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { rst = _base.insert(val); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			rst.first = _base.end(); } \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							std::pair<iterator, bool> >(rst)); }

#define YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_NODE_IMPL() \
	insert_return_type insert(BOOST_RV_REF(node_type) nh) { \
		write_lock_type lk(_mutex); \
		try { return insert_return_type(container::insert(_base, boost::move(nh))); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return insert_return_type(); } } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(BOOST_RV_REF(node_type) nh, const Handler& handler) { \
		base_insert_return_type ins_ret(iterator(), false, node_type()); \
		write_lock_type lk(_mutex); \
		try { ins_ret = container::insert(_base, boost::move(nh)); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			ins_ret = base_insert_return_type(_base.end(), false, boost::move(nh)); } \
		YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_INSERT_OF_NODE_HANDLER_RET_MAKE(inner_ret, ins_ret); \
		return handler(_final, inner_ret); } \
	\
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh) { \
		write_lock_type lk(_mutex); \
		try { return container::insert(_base, iter_handler(_base), boost::move(nh)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	template<typename IterHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh, const Handler& handler) { \
		iterator iter; \
		write_lock_type lk(_mutex); \
		try { iter = container::insert(_base, iter_handler(_base), boost::move(nh)); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			iter = _base.end(); } \
		return handler(_final, boost::fusion::pair<index_id_type, iterator>(iter)); }

#define YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_VALUE_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_ITER_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_ITER_FL_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_IL_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_NODE_IMPL()

#define YGGR_PP_SAFE_MIDX_UKV_INDEXES_INSERT_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_VALUE_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_ITER_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_ITER_FL_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_IL_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_OF_NODE_IMPL()


//----------------opt version-----------------

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

// liner_opt
#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_INSERT_OF_NODE_IMPL() \
		template<typename IterHandler> \
		insert_return_type insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params = final_opt_type::s_gen_opt_keyinfo_params(tmp, nh.value()); \
			insert_return_type ins_ret; \
			{ write_lock_type lk(_mutex); \
				try { ins_ret = opt_cast().insert(iter_handler(_base), keyinfo_params, boost::move(nh)); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } } \
			if(ins_ret.inserted) { container::detail::node_handle_midx_release_node(ins_ret.node); } \
			return YGGR_MOVE_RET(insert_return_type, ins_ret); } \
		\
		template<typename IterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh, const Handler& handler) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params = final_opt_type::s_gen_opt_keyinfo_params(tmp, nh.value()); \
			base_insert_return_type ins_ret(iterator(), false, node_type()); \
			write_lock_type lk(_mutex); \
			try { ins_ret = opt_cast().insert(iter_handler(_base), keyinfo_params, boost::move(nh)); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				ins_ret = base_insert_return_type(_base.end(), false, boost::move(nh)); } \
			if(ins_ret.inserted) { container::detail::node_handle_midx_release_node(ins_ret.node); } \
			YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_INSERT_OF_NODE_HANDLER_RET_MAKE(inner_ret, ins_ret); \
			return handler(_final, inner_ret); }
//liner_h_opt

#	define YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_INSERT_OF_NODE_IMPL() \
		insert_return_type insert(BOOST_RV_REF(node_type) nh) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params = final_opt_type::s_gen_opt_keyinfo_params(tmp, nh.value()); \
			insert_return_type ins_ret; \
			{ write_lock_type lk(_mutex); \
				try { ins_ret = opt_cast().insert(keyinfo_params, boost::move(nh)); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } } \
			if(ins_ret.inserted) { container::detail::node_handle_midx_release_node(ins_ret.node); } \
			return YGGR_MOVE_RET(insert_return_type, ins_ret); } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(BOOST_RV_REF(node_type) nh, const Handler& handler) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params = final_opt_type::s_gen_opt_keyinfo_params(tmp, nh.value()); \
			base_insert_return_type ins_ret(iterator(), false, node_type()); \
			write_lock_type lk(_mutex); \
			try { ins_ret = opt_cast().insert(keyinfo_params, boost::move(nh)); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				ins_ret = base_insert_return_type(_base.end(), false, boost::move(nh)); } \
			if(ins_ret.inserted) { container::detail::node_handle_midx_release_node(ins_ret.node); } \
			YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_INSERT_OF_NODE_HANDLER_RET_MAKE(inner_ret, ins_ret); \
			return handler(_final, inner_ret); } \
		\
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params = final_opt_type::s_gen_opt_keyinfo_params(tmp, nh.value()); \
			bool bins = false; \
			{ write_lock_type lk(_mutex); \
				try { bins = opt_cast().insert(iter_handler(_base), keyinfo_params, boost::move(nh)) != _base.end(); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } } \
			if(bins) { container::detail::node_handle_midx_release_node(nh); } \
			return bins; } \
		\
		template<typename IterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh, const Handler& handler) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params = final_opt_type::s_gen_opt_keyinfo_params(tmp, nh.value()); \
			iterator iter; \
			write_lock_type lk(_mutex); \
			try { iter = opt_cast().insert(iter_handler(_base), keyinfo_params, boost::move(nh)); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				iter = _base.end(); } \
			if(iter != _base.end()) { container::detail::node_handle_midx_release_node(nh); } \
			return handler(_final, boost::fusion::pair<index_id_type, iterator>(iter)); }
// kv_h_opt

#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_HA_OPT_INSERT_OF_ITER_FL_IMPL() \
		template<typename IterHandler, typename InputIter> \
		bool insert(const IterHandler& iter_handler, InputIter first, InputIter last) { \
			typedef container::vector<keyinfo_params_type> keyinfo_params_vt_type; \
			typedef container::vector<node_ptr_type> node_ptr_vt_type; \
			typedef \
				std::pair< \
					typename keyinfo_params_vt_type::iterator, \
					typename keyinfo_params_vt_type::iterator \
				> keyinfo_params_iter_range_type; \
			typedef \
				std::pair< \
					typename node_ptr_vt_type::iterator, \
					typename node_ptr_vt_type::iterator \
				> node_ptr_iter_range_type; \
			allocator_type alloc = this_type::get_allocator(); \
			std::size_t s = std::distance(first, last); \
			if(!s) { return true; } \
			keyinfo_params_vt_type keyinfo_params_vt; \
			node_ptr_vt_type node_ptr_vt; \
			final_type tmp(alloc); \
			try { \
				keyinfo_params_vt.reserve(s); \
				node_ptr_vt.reserve(s); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } \
			keyinfo_params_iter_range_type keyinfo_params_range \
				= final_opt_type::s_gen_opt_keyinfo_params_range(keyinfo_params_vt, tmp, first, last); \
			node_ptr_iter_range_type node_ptr_iter_range \
				= final_opt_type::create_node_range(node_ptr_vt, alloc, first, last); \
			if(keyinfo_params_vt.size() != node_ptr_vt.size()) { return false; } \
			multi_destroy_holder_type holder(node_ptr_vt.size(), alloc); \
			write_lock_type lk(_mutex); \
			try { \
				opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
									iter_handler(_base), \
									keyinfo_params_range, node_ptr_iter_range); \
				return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } }

#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_HA_OPT_INSERT_OF_VALUE_IMPL() \
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, boost::move(val)); \
			if(!pnode) { return false; } \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			try { return opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
											iter_handler(_base), keyinfo_params, pnode).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
		\
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, const value_type& val) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, val); \
			if(!pnode) { return false; } \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			try { return opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
										iter_handler(_base), keyinfo_params, pnode).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
		\
		template<typename IterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val, const Handler& handler) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, boost::move(val)); \
			destroy_holder_type holder(alloc); \
			std::pair<iterator, bool> rst(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { rst = pnode? \
					opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
										iter_handler(_base), keyinfo_params, pnode) \
					: std::make_pair(_base.end(), false); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				rst.first = _base.end(); \
				rst.second = false; } \
			return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								std::pair<iterator, bool> >(rst)); } \
		\
		template<typename IterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, const value_type& val, const Handler& handler) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, val); \
			destroy_holder_type holder(alloc); \
			std::pair<iterator, bool> rst(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { rst = pnode? \
					opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
										iter_handler(_base), keyinfo_params, pnode) \
					: std::make_pair(_base.end(), false); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				rst.first = _base.end(); \
				rst.second = false; } \
			return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								std::pair<iterator, bool> >(rst)); }

#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_HA_OPT_INSERT_OF_SIZE_IMPL() \
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, size_type size, const value_type& val) { \
			typedef container::vector<node_ptr_type> node_ptr_vt_type; \
			typedef \
				std::pair< \
					typename node_ptr_vt_type::iterator, \
					typename node_ptr_vt_type::iterator \
				> node_ptr_iter_range_type; \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_vt_type node_ptr_vt; \
			try { node_ptr_vt.reserve(size); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } \
			node_ptr_iter_range_type node_ptr_iter_range \
				= final_opt_type::create_node_n(node_ptr_vt, alloc, size, val); \
			if(node_ptr_iter_range.first == node_ptr_iter_range.second) { return false; } \
			multi_destroy_holder_type holder(size, alloc); \
			write_lock_type lk(_mutex); \
			try { opt_cast().insert( \
								detail::allocator_destroy_holder_ref(holder), \
								iter_handler(_base), \
								keyinfo_params, \
								node_ptr_iter_range); \
					return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } }

#	define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_HA_OPT_INSERT_IMPL() \
		YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_HA_OPT_INSERT_OF_VALUE_IMPL() \
		YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_HA_OPT_INSERT_OF_SIZE_IMPL() \
		YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_HA_OPT_INSERT_OF_ITER_FL_IMPL() \
		YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_INSERT_IL_IMPL() \
		YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_OPT_INSERT_OF_NODE_IMPL()

// kv_ha_opt
#	define YGGR_PP_SAFE_MIDX_KV_INDEXES_HA_OPT_INSERT_OF_ITER_FL_IMPL() \
		template<typename InputIter> \
		bool insert(InputIter first, InputIter last) { \
			typedef container::vector<keyinfo_params_type> keyinfo_params_vt_type; \
			typedef container::vector<node_ptr_type> node_ptr_vt_type; \
			typedef \
				std::pair< \
					typename keyinfo_params_vt_type::iterator, \
					typename keyinfo_params_vt_type::iterator \
				> keyinfo_params_iter_range_type; \
			typedef \
				std::pair< \
					typename node_ptr_vt_type::iterator, \
					typename node_ptr_vt_type::iterator \
				> node_ptr_iter_range_type; \
			allocator_type alloc = this_type::get_allocator(); \
			std::size_t s = std::distance(first, last); \
			if(!s) { return true; } \
			keyinfo_params_vt_type keyinfo_params_vt; \
			node_ptr_vt_type node_ptr_vt; \
			final_type tmp(alloc); \
			try { \
				keyinfo_params_vt.reserve(s); \
				node_ptr_vt.reserve(s); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } \
			keyinfo_params_iter_range_type keyinfo_params_range \
				= final_opt_type::s_gen_opt_keyinfo_params_range(keyinfo_params_vt, tmp, first, last); \
			node_ptr_iter_range_type node_ptr_iter_range \
				= final_opt_type::create_node_range(node_ptr_vt, alloc, first, last); \
			if(keyinfo_params_vt.size() != node_ptr_vt.size()) { return false; } \
			multi_destroy_holder_type holder(node_ptr_vt.size(), alloc); \
			write_lock_type lk(_mutex); \
			try { \
				opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
									keyinfo_params_range, node_ptr_iter_range); \
				return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } }


#	define YGGR_PP_SAFE_MIDX_KV_INDEXES_HA_OPT_INSERT_OF_ITER_IMPL() \
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, boost::move(val)); \
			if(!pnode) { return false; } \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			try { return \
					opt_cast().insert( \
						detail::allocator_destroy_holder_ref(holder), \
						iter_handler(_base), keyinfo_params, pnode) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
		\
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, const value_type& val) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, val); \
			if(!pnode) { return false; } \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			try { return \
					opt_cast().insert( \
						detail::allocator_destroy_holder_ref(holder), \
						iter_handler(_base), keyinfo_params, pnode) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
		\
		template<typename IterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val, const Handler& handler) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, boost::move(val)); \
			destroy_holder_type holder(alloc); \
			std::pair<iterator, bool> rst(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { \
				rst = pnode? \
						std::make_pair( \
							opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
												iter_handler(_base), keyinfo_params, pnode), true) \
						: std::make_pair(_base.end(), false); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				rst.first = _base.end(); \
				rst.second = false; } \
			\
			return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								std::pair<iterator, bool> >(rst)); } \
		\
		template<typename IterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, const value_type& val, const Handler& handler) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, val); \
			destroy_holder_type holder(alloc); \
			std::pair<iterator, bool> rst(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { \
				rst = pnode? \
						std::make_pair( \
							opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
												iter_handler(_base), keyinfo_params, pnode), true) \
						: std::make_pair(_base.end(), false); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				rst.first = _base.end(); \
				rst.second = false; } \
			return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								std::pair<iterator, bool> >(rst)); }

#	define YGGR_PP_SAFE_MIDX_KV_INDEXES_HA_OPT_INSERT_OF_VALUE_IMPL() \
		bool insert(BOOST_RV_REF(value_type) val) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, boost::move(val)); \
			if(!pnode) { return false; } \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			try { \
				return opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
											keyinfo_params, pnode).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
		\
		bool insert(const value_type& val) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, val); \
			if(!pnode) { return false; } \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			try { \
				return opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
											keyinfo_params, pnode).second; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(BOOST_RV_REF(value_type) val, const Handler& handler) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, boost::move(val)); \
			destroy_holder_type holder(alloc); \
			std::pair<iterator, bool> rst(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { \
				rst = pnode? \
						opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
											keyinfo_params, pnode) \
						: std::make_pair(_base.end(), false); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				rst.first = _base.end(); } \
			return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								std::pair<iterator, bool> >(rst)); } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const value_type& val, const Handler& handler) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			node_ptr_type pnode = final_opt_type::create_node(alloc, val); \
			destroy_holder_type holder(alloc); \
			std::pair<iterator, bool> rst(iterator(), false); \
			write_lock_type lk(_mutex); \
			try { \
				rst = pnode? \
						opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
											keyinfo_params, pnode) \
						: std::make_pair(_base.end(), false); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				rst.first = _base.end(); } \
			return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								std::pair<iterator, bool> >(rst)); }

#	define YGGR_PP_SAFE_MIDX_KV_HA_OPT_INDEXES_INSERT_IMPL() \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_HA_OPT_INSERT_OF_VALUE_IMPL() \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_HA_OPT_INSERT_OF_ITER_IMPL() \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_HA_OPT_INSERT_OF_ITER_FL_IMPL() \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_IL_IMPL() \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_INSERT_OF_NODE_IMPL()

#	define YGGR_PP_SAFE_MIDX_UKV_HA_OPT_INDEXES_INSERT_IMPL() \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_HA_OPT_INSERT_OF_VALUE_IMPL() \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_HA_OPT_INSERT_OF_ITER_IMPL() \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_HA_OPT_INSERT_OF_ITER_FL_IMPL() \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_IL_IMPL() \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_OPT_INSERT_OF_NODE_IMPL()


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

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_INSERT_IMPL_HPP__
