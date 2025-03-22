//equal_range_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_EQUAL_RANGE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_EQUAL_RANGE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>

#define YGGR_PP_SAVE_KV_EQUAL_RANGE_IMPL() \
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		equal_range(const key_type& key, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_base, _base.equal_range(key)); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		equal_range(const key_type& key, const Handler& handler) const { \
		typedef std::pair<const_iterator, const_iterator> param_type; \
		read_lock_type lk(_mutex); \
		return handler(_base, param_type(_base.equal_range(key))); }

// ukv

#define YGGR_PP_SAVE_UKV_EQUAL_RANGE_IMPL() \
	YGGR_PP_SAVE_KV_EQUAL_RANGE_IMPL()

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	define YGGR_PP_SAVE_UKV_OPT_EQUAL_RANGE_IMPL() \
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			equal_range(const key_type& key, const Handler& handler) { \
			std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
			write_lock_type lk(_mutex); \
			return handler(_base, opt_cast().equal_range(key_hash, key)); } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			equal_range(const key_type& key, const Handler& handler) const { \
			std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
			read_lock_type lk(_mutex); \
			return handler(_base, opt_cast().equal_range(key_hash, key)); }

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

#endif // __YGGR_SAFE_CONTAINER_DETAIL_EQUAL_RANGE_IMPL_HPP__
