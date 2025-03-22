//indexes_extract_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_EXTRACT_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_EXTRACT_IMPL_HPP__

#include <yggr/func/foo_t_info.hpp>
#include <boost/utility/enable_if.hpp>


//#define YGGR_PP_SAFE_MIDX_KV_INDEXES_EXTRACT_FUNCTION_SP_IMPL()

#define YGGR_PP_SAFE_MIDX_INDEXES_EXTRACT_FUNCTION_OF_KEY_IMPL() \
	node_type extract(const key_type& key) { \
		write_lock_type lk(_mutex); \
		return container::extract(_base, key); }

#define YGGR_PP_SAFE_MIDX_INDEXES_EXTRACT_FUNCTION_OF_ITER_IMPL() \
	template<typename IterHandler> \
	node_type extract(const IterHandler& iter_handler) { \
		write_lock_type lk(_mutex); \
		return container::extract(_base, iter_handler(_base)); }

#define YGGR_PP_SAFE_MIDX_INDEXES_LINEAR_EXTRACT_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_EXTRACT_FUNCTION_OF_ITER_IMPL()

#define YGGR_PP_SAFE_MIDX_INDEXES_KV_EXTRACT_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_EXTRACT_FUNCTION_OF_KEY_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_EXTRACT_FUNCTION_OF_ITER_IMPL()


#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#define YGGR_PP_SAFE_MIDX_INDEXES_OPT_EXTRACT_FUNCTION_OF_KEY_IMPL() \
	node_type extract(const key_type& key) { \
		write_lock_type lk(_mutex); \
		return opt_cast().extract(key); }

#define YGGR_PP_SAFE_MIDX_INDEXES_HA_OPT_EXTRACT_FUNCTION_OF_KEY_IMPL() \
	node_type extract(const key_type& key) { \
		std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
		write_lock_type lk(_mutex); \
		return opt_cast().extract(key_hash, key); }

#define YGGR_PP_SAFE_MIDX_INDEXES_OPT_EXTRACT_FUNCTION_OF_ITER_IMPL() \
	template<typename IterHandler> \
	node_type extract(const IterHandler& iter_handler) { \
		write_lock_type lk(_mutex); \
		return opt_cast().extract(iter_handler(_base)); }


#define YGGR_PP_SAFE_MIDX_INDEXES_OPT_LINEAR_EXTRACT_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_OPT_EXTRACT_FUNCTION_OF_ITER_IMPL()

#define YGGR_PP_SAFE_MIDX_INDEXES_KV_OPT_EXTRACT_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_EXTRACT_FUNCTION_OF_KEY_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_EXTRACT_FUNCTION_OF_ITER_IMPL()

#define YGGR_PP_SAFE_MIDX_INDEXES_UKV_OPT_EXTRACT_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_HA_OPT_EXTRACT_FUNCTION_OF_KEY_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_EXTRACT_FUNCTION_OF_ITER_IMPL()

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_EXTRACT_IMPL_HPP__
