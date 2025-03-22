//safe_list_operations_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_SAFE_LIST_OPERATIONS_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_SAFE_LIST_OPERATIONS_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#define YGGR_PP_SAFE_LIST_SORT_IMPL() \
	void sort(void) { \
		write_lock_type lk(_mutex); \
		_base.sort(); } \
	\
	template<typename Cmp> \
	void sort(Cmp cmp) { \
		write_lock_type lk(_mutex); \
		_base.sort(cmp); }

#define YGGR_PP_SAFE_LIST_REVERSE_IMPL() \
	void reverse(void) { \
		write_lock_type lk(_mutex); \
		_base.reverse(); }

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_SAFE_LIST_OPERATIONS_IMPL_HPP__
