//for_each_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_FOR_EACH_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_FOR_EACH_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/safe_container/detail/for_each_adapter.hpp>

#define YGGR_PP_SAFE_CONTAINER_FOR_EACH_ITERATOR_IMPL() \
	template<typename Handler> \
	void for_each_iterator(const Handler& handler) { \
		write_lock_type lk(_mutex); \
		for(iterator i = _base.begin(), isize = _base.end(); \
				i != isize; ++i) { handler(i); } } \
	\
	template<typename Handler> \
	void for_each_iterator(const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		for(const_iterator i = _base.begin(), isize = _base.end(); \
				i != isize; ++i) { handler(i); } } \
	\
	template<typename Handler, typename Filter_Handler> \
	void for_each_iterator(const Handler& handler, \
							const Filter_Handler& filter_handler) { \
		write_lock_type lk(_mutex); \
		for(iterator i = _base.begin(), isize = _base.end(); \
				i != isize; ++i) { if(filter_handler(i)) { handler(i); } } } \
	\
	template<typename Handler, typename Filter_Handler> \
	void for_each_iterator(const Handler& handler, \
							const Filter_Handler& filter_handler) const { \
		read_lock_type lk(_mutex); \
		for(const_iterator i = _base.begin(), isize = _base.end(); \
				i != isize; ++i) { if(filter_handler(i)) { handler(i); } } }

#define YGGR_PP_SAFE_CONTAINER_FOR_EACH_ITERATOR_REVERSE_IMPL() \
	template<typename Handler> \
	void for_each_iterator_reverse(const Handler& handler) { \
		write_lock_type lk(_mutex); \
		for(reverse_iterator i = _base.rbegin(), isize = _base.rend(); \
				i != isize; ++i) { handler(i); } } \
	\
	template<typename Handler> \
	void for_each_iterator_reverse(const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		for(const_reverse_iterator i = _base.rbegin(), isize = _base.rend(); \
				i != isize; ++i) { handler(i); } } \
	\
	template<typename Handler, typename Filter_Handler> \
	void for_each_iterator_reverse(const Handler& handler, \
									const Filter_Handler& filter_handler) { \
		write_lock_type lk(_mutex); \
		for(reverse_iterator i = _base.rbegin(), isize = _base.rend(); \
				i != isize; ++i) { if(filter_handler(i)) { handler(i); } } } \
	\
	template<typename Handler, typename Filter_Handler> \
	void for_each_iterator_reverse(const Handler& handler, \
									const Filter_Handler& filter_handler) const { \
		read_lock_type lk(_mutex); \
		for(const_reverse_iterator i = _base.rbegin(), isize = _base.rend(); \
				i != isize; ++i) { if(filter_handler(i)) { handler(i); } } }

#define YGGR_PP_SAFE_UKV_FOR_EACH_LOCAL_ITERATOR_IMPL() \
	template<typename Handler> \
	void for_each_local_iterator(size_type n, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		for(local_iterator i = _base.begin(n), isize = _base.end(n); \
				i != isize; ++i) { handler(i); } } \
	\
	template<typename Handler> \
	void for_each_local_iterator(size_type n, const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		for(const_local_iterator i = _base.begin(n), isize = _base.end(n); \
				i != isize; ++i) { handler(i); } } \
	\
	template<typename Handler, typename Filter_Handler> \
	void for_each_local_iterator(size_type n, \
									const Handler& handler, \
									const Filter_Handler& filter_handler) { \
		write_lock_type lk(_mutex); \
		for(local_iterator i = _base.begin(n), isize = _base.end(n); \
				i != isize; ++i) { if(filter_handler(i)) { handler(i); } } } \
	\
	template<typename Handler, typename Filter_Handler> \
	void for_each_local_iterator(size_type n, \
									const Handler& handler, \
									const Filter_Handler& filter_handler) const { \
		read_lock_type lk(_mutex); \
		for(const_local_iterator i = _base.begin(n), isize = _base.end(n); \
				i != isize; ++i) { if(filter_handler(i)) { handler(i); } } }


#define YGGR_PP_SAFE_CONTAINER_FOR_EACH_IMPL() \
	template<typename Handler> inline \
	void for_each(const Handler& handler) { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		adapter_type adp(handler); \
		this_type::for_each_iterator(adp); } \
	\
	template<typename Handler> inline \
	void for_each(const Handler& handler) const { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		adapter_type adp(handler); \
		this_type::for_each_iterator(adp); } \
	\
	template<typename Handler, typename Filter_Handler> inline \
	void for_each(const Handler& handler, const Filter_Handler& filter_handler) { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		typedef detail::for_each_adapter<Filter_Handler> fadapter_type; \
		adapter_type adp(handler); \
		fadapter_type fadp(filter_handler); \
		this_type::for_each_iterator(adp, fadp); } \
	\
	template<typename Handler, typename Filter_Handler> inline \
	void for_each(const Handler& handler, \
					const Filter_Handler& filter_handler) const { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		typedef detail::for_each_adapter<Filter_Handler> fadapter_type; \
		adapter_type adp(handler); \
		fadapter_type fadp(filter_handler); \
		this_type::for_each_iterator(adp, fadp); }


#define YGGR_PP_SAFE_CONTAINER_FOR_EACH_REVERSE_IMPL() \
	template<typename Handler> inline \
	void for_each_reverse(const Handler& handler) { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		adapter_type adp(handler); \
		this_type::for_each_iterator_reverse(adp); } \
	\
	template<typename Handler> inline \
	void for_each_reverse(const Handler& handler) const { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		adapter_type adp(handler); \
		this_type::for_each_iterator_reverse(adp); } \
	\
	template<typename Handler, typename Filter_Handler> inline \
	void for_each_reverse(const Handler& handler, \
							const Filter_Handler& filter_handler) { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		typedef detail::for_each_adapter<Filter_Handler> fadapter_type; \
		adapter_type adp(handler); \
		fadapter_type fadp(filter_handler); \
		this_type::for_each_iterator_reverse(adp, fadp); } \
	\
	template<typename Handler, typename Filter_Handler> inline \
	void for_each_reverse(const Handler& handler, \
							const Filter_Handler& filter_handler) const { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		typedef detail::for_each_adapter<Filter_Handler> fadapter_type; \
		adapter_type adp(handler); \
		fadapter_type fadp(filter_handler); \
		this_type::for_each_iterator_reverse(adp, fadp); }

#define YGGR_PP_SAFE_UKV_FOR_EACH_LOCAL_IMPL() \
	template<typename Handler> inline \
	void for_each_local(size_type n, const Handler& handler) { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		adapter_type adp(handler); \
		this_type::for_each_local_iterator(n, adp); } \
	\
	template<typename Handler> inline \
	void for_each_local(size_type n, const Handler& handler) const { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		adapter_type adp(handler); \
		this_type::for_each_local_iterator(n, adp); } \
	\
	template<typename Handler, typename Filter_Handler> inline \
	void for_each_local(size_type n, const Handler& handler, const Filter_Handler& filter_handler) { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		typedef detail::for_each_adapter<Filter_Handler> fadapter_type; \
		adapter_type adp(handler); \
		fadapter_type fadp(filter_handler); \
		this_type::for_each_local_iterator(n, adp, fadp); } \
	\
	template<typename Handler, typename Filter_Handler> inline \
	void for_each_local(size_type n, const Handler& handler, \
							const Filter_Handler& filter_handler) const { \
		typedef detail::for_each_adapter<Handler> adapter_type; \
		typedef detail::for_each_adapter<Filter_Handler> fadapter_type; \
		adapter_type adp(handler); \
		fadapter_type fadp(filter_handler); \
		this_type::for_each_local_iterator(n, adp, fadp); }

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_FOR_EACH_IMPL_HPP__
