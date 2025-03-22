//indexes_assign_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_ASSIGN_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_ASSIGN_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#   include <initializer_list>
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#	define YGGR_PP_SAFE_MIDX_INDEXES_ASSIGN_IL_IMPL() \
		inline void assign(std::initializer_list<value_type> il) { \
			final_type tmp(this_type::get_allocator()); \
			base_type& tmp_base = tmp.template get<N>(); \
			tmp_base.assign(boost::begin(il), boost::end(il)); \
			this_type::swap(tmp); }

#else

#	define YGGR_PP_SAFE_MIDX_INDEXES_ASSIGN_IL_IMPL()

#endif //YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#define YGGR_PP_SAFE_MIDX_INDEXES_ASSIGN_IMPL() \
	inline void assign(size_type n, const value_type& val) { \
		final_type tmp(this_type::get_allocator()); \
		base_type& tmp_base = tmp.template get<N>(); \
		tmp_base.assign(n, val); \
		this_type::swap(tmp_base); } \
	\
	template<typename InputIter> inline \
	void assign(InputIter s, InputIter e) { \
		final_type tmp(this_type::get_allocator()); \
		base_type& tmp_base = tmp.template get<N>(); \
		tmp_base.assign(s, e); \
		this_type::swap(tmp_base); } \
	\
	YGGR_PP_SAFE_MIDX_INDEXES_ASSIGN_IL_IMPL()


namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_ASSIGN_IMPL_HPP__

