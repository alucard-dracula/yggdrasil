//indexes_linear_gernel_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_LINEAR_GERNEL_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_LINEAR_GERNEL_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/func/foo_t_info.hpp>
#include <yggr/safe_container/detail/splice_impl.hpp>
#include <yggr/safe_container/detail/merge_impl.hpp>
#include <yggr/safe_container/detail/safe_list_operations_impl.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_FRONT_IMPL() \
	value_type front(void) const { \
		read_lock_type lk(_mutex); \
		if(_base.empty()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return _base.front(); } \
	\
	bool get_front(value_type& val) const { \
		read_lock_type lk(_mutex); \
		return (!_base.empty()) && (val = _base.front(), true); }

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_BACK_IMPL() \
	value_type back(void) const { \
		read_lock_type lk(_mutex); \
		if(_base.empty()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return _base.back(); } \
	\
	bool get_back(value_type& val) const { \
		read_lock_type lk(_mutex); \
		return (!_base.empty()) && (val = _base.back(), true); }

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_SPLICE_IMPL() \
	YGGR_PP_SAFE_CONTAINER_SPLICE_IMPL()

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_REMOVE_IMPL() \
	void remove(const value_type& val) { \
		write_lock_type lk(_mutex); \
		_base.remove(val); } \
	\
	template<typename Predicate> \
	void remove_if(Predicate pred) { \
		write_lock_type lk(_mutex); \
		_base.remove_if(pred); }

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_UNIQUE_IMPL() \
	void unique(void) { \
		write_lock_type lk(_mutex); \
		_base.unique(); } \
	\
	template<typename Cmp> \
	void unique(Cmp cmp) { \
		write_lock_type lk(_mutex); \
		_base.unique(cmp); }


#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_MERGE_IMPL() \
	YGGR_PP_SAFE_CONTAINER_SEQ_MERGE_IMPL()

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_SORT_IMPL() \
	YGGR_PP_SAFE_LIST_SORT_IMPL()
	
#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_REVERSE_IMPL() \
	YGGR_PP_SAFE_LIST_REVERSE_IMPL()

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_RELOCATE_IMPL() \
	template<typename PosIterHandler, typename EIterHandler> \
	typename \
		boost::enable_if< \
			boost::is_same < \
				typename boost::remove_reference< \
					typename boost::remove_cv< \
						typename func::foo_t_info < \
								EIterHandler \
							>::result_type >::type >::type, iterator >, \
			void >::type \
		relocate(const PosIterHandler& pos_iter_handler, \
					const EIterHandler& eins_iter_handler) { \
		write_lock_type lk(_mutex); \
		iterator iter = eins_iter_handler(_base); \
		if(iter == _base.end()) { return; } \
		_base.relocate(pos_iter_handler(_base), iter); } \
	\
	template<typename PosIterHandler, typename EIterHandler> \
	typename \
		boost::disable_if< \
			boost::is_same < \
				typename boost::remove_reference< \
					typename boost::remove_cv< \
						typename func::foo_t_info < \
								EIterHandler \
							>::result_type >::type >::type, iterator >, \
			void >::type \
		relocate(const PosIterHandler& pos_iter_handler, \
					const EIterHandler& eins_iter_handler) { \
		write_lock_type lk(_mutex); \
		std::pair<iterator, iterator> iter_pair =  eins_iter_handler(_base); \
		_base.relocate(pos_iter_handler(_base), iter_pair.first, iter_pair.second); } \
	\
	template<typename PosIterHandler, typename EIterHandler, typename ZIterHandler> \
	void relocate(const PosIterHandler& pos_iter_handler, \
					const EIterHandler& eins_iter_handler, \
					const ZIterHandler& zwei_iter_handler) { \
		write_lock_type lk(_mutex); \
		_base.relocate(pos_iter_handler(_base), \
						eins_iter_handler(_base), \
						zwei_iter_handler(_base)); }

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_REARRANGE_IMPL() \
	template<typename ViewIterHandler> \
	bool rearrange(const ViewIterHandler& view_iter_handler) { \
		write_lock_type lk(_mutex); \
		try { _base.rearrange(view_iter_handler(_final)); return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } }

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_HA_OPT_REMOVE_IMPL() \
	void remove(const value_type& val) { \
		multi_destroy_holder_type holder(this_type::get_allocator()); \
		write_lock_type lk(_mutex); \
		opt_cast().remove(detail::allocator_destroy_holder_ref(holder), val); } \
	\
	template<typename Predicate> \
	void remove_if(Predicate pred) { \
		multi_destroy_holder_type holder(this_type::get_allocator()); \
		write_lock_type lk(_mutex); \
		opt_cast().remove_if(detail::allocator_destroy_holder_ref(holder), pred); }

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

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_LINEAR_GERNEL_IMPL_HPP__

