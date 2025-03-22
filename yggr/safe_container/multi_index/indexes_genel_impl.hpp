//indexes_genel_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_GENEL_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_GENEL_IMPL_HPP__

#include <yggr/safe_container/detail/safe_gernel_impl.hpp>
#include <yggr/func/foo_t_info.hpp>

//operator final_type
#define YGGR_PP_SAFE_MIDX_INDEXES_OPERATOR_FINAL_IMPL() \
	operator final_type(void) const { \
		read_lock_type lk(_mutex); \
		return _final; }

// iterator_to
#define YGGR_PP_SAFE_MIDX_INDEXES_ITERATOR_TO_IMPL() \
	template<typename ValueHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		iterator_to(const ValueHandler& val_handler, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								iterator >(_base.iterator_to(val_handler(_final, index_id_type())))); } \
	\
	template<typename ValueHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		iterator_to(const ValueHandler& val_handler, const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								iterator >(_base.iterator_to(val_handler(_final, index_id_type())))); }

// gernel
#define YGGR_PP_SAFE_MIDX_INDEXES_CAPACITY_IMPL() \
	YGGR_PP_SAFE_KV_CAPACITY_IMPL()

#define YGGR_PP_SAFE_MIDX_LINEAR_INDEXES_CAPACITY_IMPL() \
	YGGR_PP_SAFE_KV_CAPACITY_IMPL() \
	\
	void resize(size_type n) { \
		write_lock_type lk(_mutex); \
		_base.resize(n); } \
	\
	void resize(size_type n, const value_type& x) { \
		write_lock_type lk(_mutex); \
		_base.resize(n, x); }

#define YGGR_PP_SAFE_MIDX_INDEXES_CLEAR_IMPL() \
	void clear(void) { \
		final_type tmp(this_type::get_allocator()); { \
			write_lock_type lk(_mutex); \
			_base.swap(tmp.template get<N>()); } }

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_GENEL_IMPL_HPP__
