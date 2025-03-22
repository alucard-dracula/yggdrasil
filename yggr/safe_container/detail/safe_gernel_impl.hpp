//safe_gernel_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_SAFE_GERNEL_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_SAFE_GERNEL_IMPL_HPP__

#include <yggr/base/yggrdef.h>

// capacity:
#define YGGR_PP_SAFE_KV_CAPACITY_IMPL() \
	bool empty(void) const { \
		read_lock_type lk(_mutex); \
		return _base.empty(); } \
	\
	size_type size(void) const { \
		read_lock_type lk(_mutex); \
		return _base.size(); } \
	\
	size_type max_size(void) const { \
		read_lock_type lk(_mutex); \
		return _base.max_size(); }

// observers
#define YGGR_PP_SAFE_KV_OBSERVERS_IMPL() \
	key_compare key_comp(void) const { \
		read_lock_type lk(_mutex); \
		return _base.key_comp(); } \
	\
	value_compare value_comp(void) const { \
		read_lock_type lk(_mutex); \
		return _base.value_comp(); }

//operations
#define YGGR_PP_SAFE_KV_COUNT_IMPL() \
	size_type count(const key_type& key) const { \
		read_lock_type lk(_mutex); \
		return _base.count(key); }

#define YGGR_PP_SAFE_CONTAINER_GET_ALLOCATOR_IMPL() \
	allocator_type get_allocator(void) const { \
		read_lock_type lk(_mutex); \
		return _base.get_allocator(); }

#define YGGR_PP_SAFE_OPTIONS_KV_REBALANCE_IMPL() \
	void rebalance(void) { \
		write_lock_type lk(_mutex); \
		_base.rebalance(); }

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_SAFE_GERNEL_IMPL_HPP__
