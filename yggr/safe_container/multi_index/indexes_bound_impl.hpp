//indexes_bound_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_BOUND_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_BOUND_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>

// lower bound
#define YGGR_PP_SAFE_MIDX_KV_INDEXES_LOWER_BOUND_IMPL() \
	template<typename CKey, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		lower_bound(const CKey& key, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							iterator >(_base.lower_bound(key))); } \
	\
	template<typename CKey, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		lower_bound(const CKey& key, const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							const_iterator >(_base.lower_bound(key))); } \
	\
	template<typename CKey, typename CPred, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		lower_bound(const CKey& key, const CPred& pred, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							iterator >(_base.lower_bound(key, pred))); } \
	\
	template<typename CKey, typename CPred, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		lower_bound(const CKey& key, const CPred& pred, const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							const_iterator >(_base.lower_bound(key, pred))); }

// upper_bound
#define YGGR_PP_SAFE_MIDX_KV_INDEXES_UPPER_BOUND_IMPL() \
	template<typename CKey, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		upper_bound(const CKey& key, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							iterator >(_base.upper_bound(key))); } \
	\
	template<typename CKey, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		upper_bound(const CKey& key, const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							const_iterator >(_base.upper_bound(key))); } \
	\
	template<typename CKey, typename CPred, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		upper_bound(const CKey& key, const CPred& pred, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							iterator >(_base.upper_bound(key, pred))); } \
	\
	template<typename CKey, typename CPred, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		upper_bound(const CKey& key, const CPred& pred, const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							const_iterator >(_base.upper_bound(key, pred))); }


#define YGGR_PP_SAFE_MIDX_KV_INDEXES_RANGE_IMPL() \
	template<typename LowerBounder, typename UpperBounder, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		range(const LowerBounder& lower, const UpperBounder& upper, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							std::pair<iterator, iterator> >(_base.range(lower, upper))); } \
	\
	template<typename LowerBounder, typename UpperBounder, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		range(const LowerBounder& lower, const UpperBounder& upper, const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							std::pair<const_iterator, const_iterator> >(_base.range(lower, upper))); }


namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_BOUND_IMPL_HPP__

