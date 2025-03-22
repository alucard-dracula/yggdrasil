//safe_unordered_gernel_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_SAFE_UNORDERED_GERNEL_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_SAFE_UNORDERED_GERNEL_IMPL_HPP__

#include <yggr/safe_container/detail/safe_gernel_impl.hpp>

#define YGGR_PP_SAFE_UKV_CAPACITY_IMPL() \
	YGGR_PP_SAFE_KV_CAPACITY_IMPL()

#define YGGR_PP_SAFE_UKV_COUNT_IMPL() \
	YGGR_PP_SAFE_KV_COUNT_IMPL()

#define YGGR_PP_SAFE_UKV_BUCKETS_IMPL() \
	size_type bucket_count(void) const { \
		read_lock_type lk(_mutex); \
		return _base.bucket_count(); } \
	\
	size_type max_bucket_count(void) const { \
		read_lock_type lk(_mutex); \
		return _base.max_bucket_count(); } \
	\
	size_type bucket_size(size_type idx) const { \
		read_lock_type lk(_mutex); \
		return _base.bucket_size(idx); }

#define YGGR_PP_SAFE_UKV_BUCKET_FUNCTION_IMPL() \
	size_type bucket(const key_type& key) const { \
		read_lock_type lk(_mutex); \
		return _base.bucket(key); }

#define YGGR_PP_SAFE_UKV_HASH_POLICY_IMPL() \
	f32 load_factor(void) const { \
		read_lock_type lk(_mutex); \
		return _base.load_factor(); } \
	\
	f32 max_load_factor(void) const { \
		read_lock_type lk(_mutex); \
		return _base.max_load_factor(); } \
	\
	void max_load_factor(f32 z) { \
		write_lock_type lk(_mutex); \
		_base.max_load_factor(z); } \
	\
	void rehash(size_type n) { \
		write_lock_type lk(_mutex); \
		_base.rehash(n); } \
	\
	void reserve(size_type n) { \
		write_lock_type lk(_mutex); \
		_base.reserve(n); }

#define YGGR_PP_SAFE_UKV_OBSERVERS_IMPL() \
	hasher hash_function(void) const { \
		read_lock_type lk(_mutex); \
		return _base.hash_function(); } \
	\
	key_equal key_eq(void) const { \
		read_lock_type lk(_mutex); \
		return _base.key_eq(); }


#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	define YGGR_PP_SAFE_UKV_OPT_COUNT_IMPL() \
		size_type count(const key_type& key) const { \
			std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
			read_lock_type lk(_mutex); \
			return opt_cast().count(key_hash, key); }

#	define YGGR_PP_SAFE_UKV_OPT_BUCKET_FUNCTION_IMPL() \
		size_type bucket(const key_type& key) const { \
			std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
			read_lock_type lk(_mutex); \
			return opt_cast().bucket(key_hash); }

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION


namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_SAFE_UNORDERED_GERNEL_IMPL_HPP__
