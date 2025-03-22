//safe_ranked_index_gernel_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_RANKED_INDEX_GERNEL_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_RANKED_INDEX_GERNEL_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>
#include <yggr/safe_container/multi_index/safe_ordered_index_gernel_impl.hpp>

#define YGGR_PP_SAFE_MIDX_RANKED_INDEX_OBSERVERS_IMPL() \
	YGGR_PP_SAFE_MIDX_ORDERED_INDEX_OBSERVERS_IMPL()

#define YGGR_PP_SAFE_MIDX_RANKED_INDEX_RANK_OPERATIONS_IMPL() \
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		nth(size_type n, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							iterator \
						>(_base.nth(n))); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		nth(size_type n, const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							const_iterator \
						>(_base.nth(n))); } \
	\
	template<typename IterHandler> \
	size_type rank(const IterHandler& iter_handler) const { \
		read_lock_type lk(_mutex); \
		return _base.rank(iter_handler(_base)); } \
	\
	template<typename CKey> \
	size_type find_rank(const CKey& key) const { \
		read_lock_type lk(_mutex); \
		return _base.find_rank(key); } \
	\
	template<typename CKey, typename CPred> \
	size_type find_rank(const CKey& key, const CPred& pred) const { \
		read_lock_type lk(_mutex); \
		return _base.find_rank(key, pred); } \
	\
	template<typename CKey> \
	size_type lower_bound_rank(const CKey& key) const { \
		read_lock_type lk(_mutex); \
		return _base.lower_bound_rank(key); } \
	\
	template<typename CKey, typename CPred> \
	size_type lower_bound_rank(const CKey& key, const CPred& pred) const { \
		read_lock_type lk(_mutex); \
		return _base.lower_bound_rank(key, pred); } \
	\
	template<typename CKey> \
	size_type upper_bound_rank(const CKey& key) const { \
		read_lock_type lk(_mutex); \
		return _base.upper_bound_rank(key); } \
	\
	template<typename CKey, typename CPred> \
	size_type upper_bound_rank(const CKey& key, const CPred& pred) const { \
		read_lock_type lk(_mutex); \
		return _base.upper_bound_rank(key, pred); } \
	\
	template<typename CKey> \
	std::pair<size_type, size_type> \
		equal_range_rank(const CKey key) const { \
		read_lock_type lk(_mutex); \
		return _base.equal_range_rank(key); } \
	\
	template<typename CKey, typename CPred> \
	std::pair<size_type, size_type> \
		equal_range_rank(const CKey& key, const CPred& pred) const { \
		read_lock_type lk(_mutex); \
		return _base.equal_range_rank(key, pred); } \
	\
	template<typename LowerBounder, typename UpperBounder> \
	std::pair<size_type, size_type> \
		range_rank(const LowerBounder& lower, const UpperBounder& upper) const { \
		read_lock_type lk(_mutex); \
		return _base.range_rank(lower, upper); }

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_RANKED_INDEX_GERNEL_IMPL_HPP__
