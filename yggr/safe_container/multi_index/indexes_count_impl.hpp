// indexes_count_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_COUNT_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_COUNT_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#define YGGR_PP_SAFE_MIDX_INDEXES_COUNT_IMPL() \
	template<typename CompatibleKey> \
	size_type count(const CompatibleKey& key) const { \
		read_lock_type lk(_mutex); \
		return _base.count(key); }

#define YGGR_PP_SAFE_MIDX_KV_INDEXES_COUNT_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_COUNT_IMPL() \
	\
	template<typename CompatibleKey, \
				typename CompatiblePred > \
	size_type count(const CompatibleKey& key, \
						const CompatiblePred& pred) const { \
		read_lock_type lk(_mutex); \
		return _base.count(key, pred); }

#define YGGR_PP_SAFE_MIDX_UKV_INDEXES_COUNT_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_COUNT_IMPL() \
	\
	template<typename CompatibleKey, \
				typename CompatibleHash, \
				typename CompatiblePred > \
	size_type count(const CompatibleKey& key, \
						const CompatibleHash& hash, \
						const CompatiblePred& eq) const { \
		read_lock_type lk(_mutex); \
		return _base.count(key, hash, eq); }


#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	define YGGR_PP_SAFE_MIDX_UKV_OPT_INDEXES_COUNT_IMPL() \
		template<typename CompatibleKey> \
		size_type count(const CompatibleKey& key) const { \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			read_lock_type lk(_mutex); \
			return opt_cast().count(key_hash, key); } \
		\
		template<typename CompatibleKey, \
					typename CompatibleHash, \
					typename CompatiblePred > \
		size_type count(const CompatibleKey& key, \
							const CompatibleHash& hash, \
							const CompatiblePred& eq) const { \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			read_lock_type lk(_mutex); \
			return opt_cast().count(key_hash, key, eq); }


#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_COUNT_IMPL_HPP__
