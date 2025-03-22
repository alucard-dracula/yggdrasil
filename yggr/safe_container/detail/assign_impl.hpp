//assert_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_ASSIGN_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_ASSIGN_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#   include <initializer_list>
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#	define YGGR_PP_SAFE_SEQ_ASSIGN_INITIALIZER_LIST_IMPL() \
		inline void assign(std::initializer_list<value_type> l) { \
			base_type tmp(this_type::get_allocator()); \
			tmp.assign(l.begin(), l.end()); \
			this_type::swap(tmp); }

#else

#	define YGGR_PP_SAFE_SEQ_ASSIGN_INITIALIZER_LIST_IMPL()

#endif //YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#define YGGR_PP_SAFE_SEQ_ASSIGN_IMPL() \
	template<typename InputIterator> inline \
	void assign(InputIterator first, InputIterator last) { \
		base_type tmp(this_type::get_allocator()); \
		tmp.assign(first, last); \
		this_type::swap(tmp); } \
	\
	inline void assign(size_type size, const value_type& val) { \
		base_type tmp(this_type::get_allocator()); \
		tmp.assign(size, val); \
		this_type::swap(tmp); } \
	\
	YGGR_PP_SAFE_SEQ_ASSIGN_INITIALIZER_LIST_IMPL()


namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_ASSIGN_IMPL_HPP__
