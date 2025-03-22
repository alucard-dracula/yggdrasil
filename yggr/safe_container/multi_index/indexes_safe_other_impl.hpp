//indexes_safe_other_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_SAFE_OTHER_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_SAFE_OTHER_IMPL_HPP__

#include <yggr/safe_container/detail/safe_other_impl.hpp>

#define YGGR_PP_SAFE_MIDX_LINER_INDEXES_IS_EXISTS_IMPL() \
	bool is_exists(const value_type& val) const { \
		read_lock_type lk(_mutex); \
		return std::find(_base.begin(), _base.end(), val) != _base.end(); } \
	\
	template<typename Pred> \
	bool is_exists(const Pred& pred) const { \
		read_lock_type lk(_mutex); \
		return std::find_if(_base.begin(), _base.end(), pred) != _base.end(); }

#define YGGR_PP_SAFE_MIDX_KV_INDEXES_IS_EXISTS_GENEL_IMPL() \
	template<typename CKey> inline \
	bool is_exists(const CKey& key) const { \
		return this_type::find(key); }

#define YGGR_PP_SAFE_MIDX_KV_INDEXES_IS_EXISTS_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_IS_EXISTS_GENEL_IMPL() \
	\
	template<typename CKey, typename Pred> inline \
	bool is_exists(const CKey& key, const Pred& pred) const { \
		return this_type::find(key, pred); }

#define YGGR_PP_SAFE_MIDX_UKV_INDEXES_IS_EXISTS_IMPL() \
	YGGR_PP_SAFE_MIDX_KV_INDEXES_IS_EXISTS_GENEL_IMPL() \
	\
	template<typename CKey, typename CHash, typename CEq> inline \
	bool is_exists(const CKey& key, const CHash& hash, const CEq& eq) const { \
		return this_type::find(key, hash, eq); }

#define YGGR_PP_SAFE_MIDX_INDEXES_COPY_TO_BASE_IMPL() \
	YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL() \
	\
	final_type& copy_to_base(final_type& out) const { \
		base_type& right_ref = out.template get<N>(); \
		if(&_base == &right_ref) { return out; } \
		{read_lock_type lk(_mutex); \
		right_ref = _base; } \
		return out; }

#define YGGR_PP_SAFE_MIDX_INDEXES_LOAD_IMPL() \
	final_type load(void) const { \
		read_lock_type lk(_mutex); \
		return _final; }

#define YGGR_PP_SAFE_MIDX_INDEXES_STONE_IMPL() \
	YGGR_PP_SAFE_CONTAINER_STONE_IMPL() \
	\
	inline void store(BOOST_RV_REF(final_type) final) { \
		this_type::operator=(boost::move(final.template get<N>())); } \
	\
	inline void store(const final_type& final) { \
		this_type::operator=(final.template get<N>()); }

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_SAFE_OTHER_IMPL_HPP__
