// yggr_lua_d64_s64_list_drei.cpp

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

#include "yggr_lua_d32.hpp"
#include "yggr_lua_d64.hpp"

#include <yggr/lua_wrap/lua_export_wrap_allocator.hpp>
#include <yggr/lua_wrap/lua_export_wrap_bidirectional_iterator.hpp>
#include <yggr/lua_wrap/lua_export_wrap_reverse_iterator.hpp>
#include <yggr/lua_wrap/lua_export_wrap_foo_reverse.hpp>
#include <yggr/lua_wrap/lua_export_wrap_foo_shuffle.hpp>
#include <yggr/lua_wrap/lua_export_wrap_foo_find.hpp>
#include <yggr/lua_wrap/lua_export_wrap_foo_remove.hpp>
#include <yggr/lua_wrap/lua_export_wrap_foo_of_iterator.hpp>
#include <yggr/lua_wrap/lua_export_wrap_foo_of_container.hpp>
#include <yggr/lua_wrap/lua_export_wrap_list.hpp>

#include <boost/ref.hpp>

#include <cassert>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace lua
{
namespace detail
{

void export_d64_s64_list_s64_list_part_zwei(void* pcobj)
{
	//luabind::class_<s64_list_type> s64_list_cobj("s64_list_type");
	typedef luabind::class_<s64_list_type> cobj_type;

	assert(pcobj);
	cobj_type& cobj = *(reinterpret_cast<cobj_type*>(pcobj));

	//// s64_list_type
	//lua_wrap::export_wrap_list(cobj);
	//lua_wrap::export_wrap_list_r_wrap_base_t<wrap_s64_type>(cobj);
	//lua_wrap::export_wrap_list_r_wrap_base_t<wrap_u64_type>(cobj);

	//// u64_list_iterator
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_list_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_list_const_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_list_reverse_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_list_const_reverse_iterator_type>(cobj);

	//// d32_vector_iterator
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_vector_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_vector_const_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_vector_reverse_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_vector_const_reverse_iterator_type>(cobj);

	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_vector_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_vector_const_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_vector_reverse_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_vector_const_reverse_iterator_type>(cobj);

	//// d64_vector_iterator
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_vector_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_vector_const_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_vector_reverse_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_vector_const_reverse_iterator_type>(cobj);

	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_vector_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_vector_const_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_vector_reverse_iterator_type>(cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_vector_const_reverse_iterator_type>(cobj);

	// d32_deque_iterator
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_deque_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_deque_const_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_deque_reverse_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_deque_const_reverse_iterator_type>(cobj);

	lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_deque_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_deque_const_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_deque_reverse_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_deque_const_reverse_iterator_type>(cobj);

	// d64_deque_iterator
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_deque_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_deque_const_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_deque_reverse_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_deque_const_reverse_iterator_type>(cobj);

	lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_deque_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_deque_const_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_deque_reverse_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_deque_const_reverse_iterator_type>(cobj);

	// d32_list_iterator
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_list_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_list_const_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_list_reverse_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_list_const_reverse_iterator_type>(cobj);

	lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_list_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_list_const_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_list_reverse_iterator_type>(cobj);
	lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_list_const_reverse_iterator_type>(cobj);
}

} // namespace detail
} // namespace lua
} // namespace yggr
