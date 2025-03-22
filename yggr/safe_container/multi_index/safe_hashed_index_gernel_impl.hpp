//safe_hashed_index_opt_gernel_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_HASHED_INDEX_GERNEL_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_HASHED_INDEX_GERNEL_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#define YGGR_PP_SAFE_MIDX_HASHED_INDEX_OBSERVERS_IMPL() \
	key_from_value key_extractor(void) const { \
		read_lock_type lk(_mutex); \
		return _base.key_extractor(); } \
	\
	hasher hash_function(void) const { \
		read_lock_type lk(_mutex); \
		return _base.hash_function(); } \
	\
	key_equal key_eq(void) const { \
		read_lock_type lk(_mutex); \
		return _base.key_eq(); }

#define YGGR_PP_SAFE_MIDX_HASHED_INDEX_BUCKET_GERNEL_IMPL() \
	size_type bucket_count(void) const { \
		read_lock_type lk(_mutex); \
		return _base.bucket_count(); } \
	\
	size_type max_bucket_count(void) const { \
		read_lock_type lk(_mutex); \
		return _base.max_bucket_count(); } \
	\
	size_type bucket_size(size_type n) const { \
		read_lock_type lk(_mutex); \
		return _base.bucket_size(n); }

#define YGGR_PP_SAFE_MIDX_HASHED_INDEX_BUCKET_IMPL() \
	YGGR_PP_SAFE_MIDX_HASHED_INDEX_BUCKET_GERNEL_IMPL() \
	\
	size_type bucket(const key_type& key) const { \
		read_lock_type lk(_mutex); \
		return _base.bucket(key); }

#define YGGR_PP_SAFE_MIDX_HASHED_INDEX_HASH_POLICY_IMPL() \
	float load_factor(void) const { \
		read_lock_type lk(_mutex); \
		return _base.load_factor(); } \
	\
	void max_load_factor(float z) { \
		write_lock_type lk(_mutex); \
		_base.max_load_factor(z); } \
	\
	float max_load_factor(void) const { \
		read_lock_type lk(_mutex); \
		return _base.max_load_factor(); } \
	\
	void rehash(size_type n) { \
		write_lock_type lk(_mutex); \
		_base.rehash(n); }


#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	define YGGR_PP_SAFE_MIDX_HASHED_INDEX_OPT_BUCKET_IMPL() \
		YGGR_PP_SAFE_MIDX_HASHED_INDEX_BUCKET_GERNEL_IMPL() \
		\
		size_type bucket(const key_type& key) const { \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			read_lock_type lk(_mutex); \
			return opt_cast().bucket(key_hash); }

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

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_SAFE_HASHED_INDEX_GERNEL_IMPL_HPP__
