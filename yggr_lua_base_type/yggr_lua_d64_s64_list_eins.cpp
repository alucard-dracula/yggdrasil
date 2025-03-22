// yggr_lua_d64_s64_list_eins.cpp

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

void export_d64_s64_list_wrap_s64_list_iterator(void* pcobj)
{
	//luabind::class_<wrap_s64_list_iterator_type> s64_list_iterator_cobj("s64_list_iterator_type");
	typedef luabind::class_<wrap_s64_list_iterator_type> cobj_type;
	
	assert(pcobj);
	cobj_type& cobj = *(reinterpret_cast<cobj_type*>(pcobj));

	lua_wrap::export_wrap_bidirectional_iterator(cobj);
	lua_wrap::export_wrap_bidirectional_iterator_r_const_iterator<wrap_s64_list_const_iterator_type>(cobj);
}

void export_d64_s64_list_wrap_s64_list_const_iterator(void* pcobj)
{
	//luabind::class_<wrap_s64_list_const_iterator_type> s64_list_const_iterator_cobj("s64_list_const_iterator_type");
	typedef luabind::class_<wrap_s64_list_const_iterator_type> cobj_type;
	
	assert(pcobj);
	cobj_type& cobj = *(reinterpret_cast<cobj_type*>(pcobj));

	lua_wrap::export_wrap_bidirectional_const_iterator(cobj);
	lua_wrap::export_wrap_bidirectional_const_iterator_r_iterator<wrap_s64_list_iterator_type>(cobj);
}

void export_d64_s64_list_wrap_s64_list_reverse_iterator(void* pcobj)
{
	// luabind::class_<wrap_s64_list_reverse_iterator_type> s64_list_reverse_iterator_cobj("s64_list_reverse_iterator_type");
	typedef luabind::class_<wrap_s64_list_reverse_iterator_type> cobj_type;

	assert(pcobj);
	cobj_type& cobj = *(reinterpret_cast<cobj_type*>(pcobj));

	lua_wrap::export_wrap_reverse_iterator(cobj);
	lua_wrap::export_wrap_reverse_iterator_construct_other_iterator<wrap_s64_list_iterator_type>(cobj);
	lua_wrap::export_wrap_reverse_iterator_r_other_reverse_iterator<wrap_s64_list_const_reverse_iterator_type>(cobj);
}

void export_d64_s64_list_wrap_s64_list_const_reverse_iterator(void* pcobj)
{
	//luabind::class_<wrap_s64_list_const_reverse_iterator_type> s64_list_const_reverse_iterator_cobj("s64_list_const_reverse_iterator_type");
	typedef luabind::class_<wrap_s64_list_const_reverse_iterator_type> cobj_type;

	assert(pcobj);
	cobj_type& cobj = *(reinterpret_cast<cobj_type*>(pcobj));

	lua_wrap::export_wrap_reverse_iterator(cobj);
	lua_wrap::export_wrap_reverse_iterator_construct_other_iterator<wrap_s64_list_iterator_type>(cobj);
	lua_wrap::export_wrap_reverse_iterator_construct_other_iterator<wrap_s64_list_const_iterator_type>(cobj);
	lua_wrap::export_wrap_reverse_iterator_construct_other_iterator<wrap_s64_list_reverse_iterator_type>(cobj);
	lua_wrap::export_wrap_reverse_iterator_r_other_reverse_iterator<wrap_s64_list_reverse_iterator_type>(cobj);
}

} // namespace detail

void export_d64_s64_list(lua_State* L)
{
	luabind::class_<wrap_s64_list_iterator_type> s64_list_iterator_cobj("s64_list_iterator_type");
	luabind::class_<wrap_s64_list_const_iterator_type> s64_list_const_iterator_cobj("s64_list_const_iterator_type");
	luabind::class_<wrap_s64_list_reverse_iterator_type> s64_list_reverse_iterator_cobj("s64_list_reverse_iterator_type");
	luabind::class_<wrap_s64_list_const_reverse_iterator_type> s64_list_const_reverse_iterator_cobj("s64_list_const_reverse_iterator_type");
	luabind::class_<s64_list_type> s64_list_cobj("s64_list_type");

	//// iterators
	//lua_wrap::export_wrap_bidirectional_iterator(s64_list_iterator_cobj);
	//lua_wrap::export_wrap_bidirectional_iterator_r_const_iterator<wrap_s64_list_const_iterator_type>(s64_list_iterator_cobj);

	//lua_wrap::export_wrap_bidirectional_const_iterator(s64_list_const_iterator_cobj);
	//lua_wrap::export_wrap_bidirectional_const_iterator_r_iterator<wrap_s64_list_iterator_type>(s64_list_const_iterator_cobj);

	detail::export_d64_s64_list_wrap_s64_list_iterator(static_cast<void*>(boost::addressof(s64_list_iterator_cobj)));
	detail::export_d64_s64_list_wrap_s64_list_const_iterator(static_cast<void*>(boost::addressof(s64_list_const_iterator_cobj)));

	//// reverse_iterators
	//lua_wrap::export_wrap_reverse_iterator(s64_list_reverse_iterator_cobj);
	//lua_wrap::export_wrap_reverse_iterator_construct_other_iterator<wrap_s64_list_iterator_type>(s64_list_reverse_iterator_cobj);
	//lua_wrap::export_wrap_reverse_iterator_r_other_reverse_iterator<wrap_s64_list_const_reverse_iterator_type>(s64_list_reverse_iterator_cobj);

	//lua_wrap::export_wrap_reverse_iterator(s64_list_const_reverse_iterator_cobj);
	//lua_wrap::export_wrap_reverse_iterator_construct_other_iterator<wrap_s64_list_iterator_type>(s64_list_const_reverse_iterator_cobj);
	//lua_wrap::export_wrap_reverse_iterator_construct_other_iterator<wrap_s64_list_const_iterator_type>(s64_list_const_reverse_iterator_cobj);
	//lua_wrap::export_wrap_reverse_iterator_construct_other_iterator<wrap_s64_list_reverse_iterator_type>(s64_list_const_reverse_iterator_cobj);
	//lua_wrap::export_wrap_reverse_iterator_r_other_reverse_iterator<wrap_s64_list_reverse_iterator_type>(s64_list_const_reverse_iterator_cobj);

	detail::export_d64_s64_list_wrap_s64_list_reverse_iterator(static_cast<void*>(boost::addressof(s64_list_reverse_iterator_cobj)));
	detail::export_d64_s64_list_wrap_s64_list_const_reverse_iterator(static_cast<void*>(boost::addressof(s64_list_const_reverse_iterator_cobj)));

	//// s64_list_type
	//lua_wrap::export_wrap_list(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_wrap_base_t<wrap_s64_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_wrap_base_t<wrap_u64_type>(s64_list_cobj);

	//// u64_list_iterator
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_list_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_list_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_list_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_list_const_reverse_iterator_type>(s64_list_cobj);

	//// d32_vector_iterator
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_vector_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_vector_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_vector_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_vector_const_reverse_iterator_type>(s64_list_cobj);

	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_vector_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_vector_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_vector_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_vector_const_reverse_iterator_type>(s64_list_cobj);

	//// d64_vector_iterator
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_vector_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_vector_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_vector_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_vector_const_reverse_iterator_type>(s64_list_cobj);

	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_vector_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_vector_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_vector_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_vector_const_reverse_iterator_type>(s64_list_cobj);

	//// d32_deque_iterator
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_deque_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_deque_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_deque_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_deque_const_reverse_iterator_type>(s64_list_cobj);

	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_deque_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_deque_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_deque_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_deque_const_reverse_iterator_type>(s64_list_cobj);

	//// d64_deque_iterator
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_deque_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_deque_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_deque_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s64_deque_const_reverse_iterator_type>(s64_list_cobj);

	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_deque_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_deque_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_deque_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u64_deque_const_reverse_iterator_type>(s64_list_cobj);

	//// d32_list_iterator
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_list_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_list_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_list_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_s32_list_const_reverse_iterator_type>(s64_list_cobj);

	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_list_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_list_const_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_list_reverse_iterator_type>(s64_list_cobj);
	//lua_wrap::export_wrap_list_r_other_iterator<wrap_u32_list_const_reverse_iterator_type>(s64_list_cobj);

	detail::export_d64_s64_list_s64_list(static_cast<void*>(boost::addressof(s64_list_cobj)));

	luabind::module(L)
	[
		luabind::namespace_(YGGR_PP_CAST_TO_STRING(yggr))
		[
			luabind::namespace_(YGGR_PP_CAST_TO_STRING(lua))
			[
				s64_list_iterator_cobj,
				s64_list_const_iterator_cobj,

				s64_list_reverse_iterator_cobj,
				s64_list_const_reverse_iterator_cobj,

				s64_list_cobj,

				// reverse
				lua_wrap::export_foo_reverse<wrap_s64_list_iterator_type>(),
				lua_wrap::export_foo_reverse<wrap_s64_list_reverse_iterator_type>(),

				// find
				lua_wrap::export_foo_find<wrap_s64_list_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_find<wrap_s64_list_const_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_find<wrap_s64_list_reverse_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_find<wrap_s64_list_const_reverse_iterator_type, yggr::s64>(),

				// remove
				lua_wrap::export_foo_remove<wrap_s64_list_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_remove<wrap_s64_list_reverse_iterator_type, yggr::s64>(),

				// container
				lua_wrap::export_foo_size<s64_list_type>(),
				lua_wrap::export_foo_wraped_size<s64_list_type>(),

				lua_wrap::export_foo_ssize<s64_list_type>(),
				lua_wrap::export_foo_wraped_ssize<s64_list_type>(),

				lua_wrap::export_foo_resize<s64_list_type, double>(),
				lua_wrap::export_foo_resize<s64_list_type, double, yggr::s64>(),
				lua_wrap::export_foo_resize<s64_list_type, wrap_s64_type>(),
				lua_wrap::export_foo_resize<s64_list_type, wrap_s64_type, yggr::s64>(),
				lua_wrap::export_foo_resize<s64_list_type, wrap_u64_type>(),
				lua_wrap::export_foo_resize<s64_list_type, wrap_u64_type, yggr::s64>(),

				lua_wrap::export_foo_empty<s64_list_type>(),

				lua_wrap::export_foo_clear<s64_list_type>(),

				lua_wrap::export_foo_ref_begin<s64_list_type>(),
				lua_wrap::export_foo_cref_begin<s64_list_type>(),
				lua_wrap::export_foo_cbegin<s64_list_type>(),

				lua_wrap::export_foo_ref_end<s64_list_type>(),
				lua_wrap::export_foo_cref_end<s64_list_type>(),
				lua_wrap::export_foo_cend<s64_list_type>(),

				lua_wrap::export_foo_ref_rbegin<s64_list_type>(),
				lua_wrap::export_foo_cref_rbegin<s64_list_type>(),
				lua_wrap::export_foo_crbegin<s64_list_type>(),

				lua_wrap::export_foo_ref_rend<s64_list_type>(),
				lua_wrap::export_foo_cref_rend<s64_list_type>(),
				lua_wrap::export_foo_crend<s64_list_type>(),

				// iterator
				// advance
				lua_wrap::export_foo_advance<wrap_s64_list_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_advance<wrap_s64_list_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_advance<wrap_s64_list_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_advance<wrap_s64_list_const_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_advance<wrap_s64_list_const_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_advance<wrap_s64_list_const_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_advance<wrap_s64_list_reverse_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_advance<wrap_s64_list_reverse_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_advance<wrap_s64_list_reverse_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_advance<wrap_s64_list_const_reverse_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_advance<wrap_s64_list_const_reverse_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_advance<wrap_s64_list_const_reverse_iterator_type, wrap_u64_type>(),

				// advance_copy
				lua_wrap::export_foo_advance_copy<wrap_s64_list_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_advance_copy<wrap_s64_list_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_advance_copy<wrap_s64_list_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_advance_copy<wrap_s64_list_const_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_advance_copy<wrap_s64_list_const_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_advance_copy<wrap_s64_list_const_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_advance_copy<wrap_s64_list_reverse_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_advance_copy<wrap_s64_list_reverse_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_advance_copy<wrap_s64_list_reverse_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_advance_copy<wrap_s64_list_const_reverse_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_advance_copy<wrap_s64_list_const_reverse_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_advance_copy<wrap_s64_list_const_reverse_iterator_type, wrap_u64_type>(),

				// distance
				lua_wrap::export_foo_distance<wrap_s64_list_iterator_type, wrap_s64_list_iterator_type>(),
				lua_wrap::export_foo_distance<wrap_s64_list_iterator_type, wrap_s64_list_const_iterator_type>(),
				lua_wrap::export_foo_distance<wrap_s64_list_const_iterator_type, wrap_s64_list_iterator_type>(),
				lua_wrap::export_foo_distance<wrap_s64_list_const_iterator_type, wrap_s64_list_const_iterator_type>(),

				lua_wrap::export_foo_distance<wrap_s64_list_reverse_iterator_type, wrap_s64_list_reverse_iterator_type>(),
				lua_wrap::export_foo_distance<wrap_s64_list_reverse_iterator_type, wrap_s64_list_const_reverse_iterator_type>(),
				lua_wrap::export_foo_distance<wrap_s64_list_const_reverse_iterator_type, wrap_s64_list_reverse_iterator_type>(),
				lua_wrap::export_foo_distance<wrap_s64_list_const_reverse_iterator_type, wrap_s64_list_const_reverse_iterator_type>(),

				lua_wrap::export_foo_wraped_distance<wrap_s64_list_iterator_type, wrap_s64_list_iterator_type>(),
				lua_wrap::export_foo_wraped_distance<wrap_s64_list_iterator_type, wrap_s64_list_const_iterator_type>(),
				lua_wrap::export_foo_wraped_distance<wrap_s64_list_const_iterator_type, wrap_s64_list_iterator_type>(),
				lua_wrap::export_foo_wraped_distance<wrap_s64_list_const_iterator_type, wrap_s64_list_const_iterator_type>(),

				lua_wrap::export_foo_wraped_distance<wrap_s64_list_reverse_iterator_type, wrap_s64_list_reverse_iterator_type>(),
				lua_wrap::export_foo_wraped_distance<wrap_s64_list_reverse_iterator_type, wrap_s64_list_const_reverse_iterator_type>(),
				lua_wrap::export_foo_wraped_distance<wrap_s64_list_const_reverse_iterator_type, wrap_s64_list_reverse_iterator_type>(),
				lua_wrap::export_foo_wraped_distance<wrap_s64_list_const_reverse_iterator_type, wrap_s64_list_const_reverse_iterator_type>(),

				// next
				lua_wrap::export_foo_next<wrap_s64_list_iterator_type>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_next<wrap_s64_list_const_iterator_type>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_const_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_const_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_const_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_next<wrap_s64_list_reverse_iterator_type>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_reverse_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_reverse_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_reverse_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_next<wrap_s64_list_const_reverse_iterator_type>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_const_reverse_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_const_reverse_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_next_n<wrap_s64_list_const_reverse_iterator_type, wrap_u64_type>(),

				// prev
				lua_wrap::export_foo_prev<wrap_s64_list_iterator_type>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_prev<wrap_s64_list_const_iterator_type>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_const_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_const_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_const_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_prev<wrap_s64_list_reverse_iterator_type>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_reverse_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_reverse_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_reverse_iterator_type, wrap_u64_type>(),

				lua_wrap::export_foo_prev<wrap_s64_list_const_reverse_iterator_type>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_const_reverse_iterator_type, yggr::s64>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_const_reverse_iterator_type, wrap_s64_type>(),
				lua_wrap::export_foo_prev_n<wrap_s64_list_const_reverse_iterator_type, wrap_u64_type>(),

				//typeid
				lua_wrap::export_typeid<wrap_s64_list_iterator_type>(),
				lua_wrap::export_typeid<wrap_s64_list_const_iterator_type>(),

				lua_wrap::export_typeid<wrap_s64_list_reverse_iterator_type>(),
				lua_wrap::export_typeid<wrap_s64_list_const_reverse_iterator_type>(),

				lua_wrap::export_typeid<s64_list_type>()
			]
		]
	];
}

} // namespace lua
} // namespace yggr
