//safe_random_access_index_gernel_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_RANDOM_ACCESS_INDEX_GERNEL_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_RANDOM_ACCESS_INDEX_GERNEL_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/compatibility/stl_exception.hpp>

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_SHRINK_TO_FIT

#	define YGGR_PP_SAFE_MIDX_RND_INDEXES_SHRINK_TO_FIT_IMPL() \
		void shrink_to_fit(void) { \
			write_lock_type lk(_mutex); \
			_base.shrink_to_fit(); }

#else

#	define YGGR_PP_SAFE_MIDX_RND_INDEXES_SHRINK_TO_FIT_IMPL() \
		void shrink_to_fit(void) { \
			final_type tmp(this_type::get_allocator()); \
			base_type& tmp_index = tmp.get<N>(); \
			write_lock_type lk(_mutex); \
			tmp_index.splice(tmp_index.end(), _base, _base.begin(), _base.end()); \
			_base.swap(tmp_index); }

#endif // YGGR_BOOST_MULTI_INDEX_HAS_SHRINK_TO_FIT

#define YGGR_PP_SAFE_MIDX_RND_INDEXES_CAPACITY_IMPL() \
	size_type capacity(void) const { \
		read_lock_type lk(_mutex); \
		return _base.capacity(); } \
	\
	bool reserve(size_type size) { \
		write_lock_type lk(_mutex); \
		try { \
			_base.reserve(size); \
			return true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
			return false; } } \
	\
	YGGR_PP_SAFE_MIDX_RND_INDEXES_SHRINK_TO_FIT_IMPL()
	

#define YGGR_PP_SAFE_MIDX_RND_INDEXES_AT_IMPL() \
	value_type operator[](size_type idx) const { \
		read_lock_type lk(_mutex); \
		if(!(idx < _base.size())) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return _base[idx]; } \
	\
	value_type at(size_type idx) const { \
		read_lock_type lk(_mutex); \
		if(!(idx < _base.size())) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return _base.at(idx); } \
	\
	bool get_at(size_type idx, value_type& val) const { \
		read_lock_type lk(_mutex); \
		if(!(idx < _base.size())) { \
			return false; } \
		val = _base.at(idx); \
		return true; } \
	\
	inline bool get_value(size_type idx, value_type& val) const { \
		return this_type::get_at(idx, val); }
	


namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_RANDOM_ACCESS_INDEX_GERNEL_IMPL_HPP__
