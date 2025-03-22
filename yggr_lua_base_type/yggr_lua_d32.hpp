//yggr_lua_d32.hpp

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

#ifndef __YGGR_LUA_YGGR_LUA_D32_HPP__
#define __YGGR_LUA_YGGR_LUA_D32_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/script/lua_base_environment.hpp>
#include <yggr/script/lua_config.hpp>

#include <yggr/lua_wrap/wrap_vector.hpp>
#include <yggr/lua_wrap/wrap_deque.hpp>
#include <yggr/lua_wrap/wrap_list.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace lua
{

// vector
typedef yggr::lua_wrap::wrap_vector<yggr::s32> s32_vector_type;
typedef s32_vector_type::wrap_allocator_type wrap_s32_vector_allocator_type;
typedef s32_vector_type::wrap_iterator_type wrap_s32_vector_iterator_type;
typedef s32_vector_type::wrap_const_iterator_type wrap_s32_vector_const_iterator_type;
typedef s32_vector_type::wrap_reverse_iterator_type wrap_s32_vector_reverse_iterator_type;
typedef s32_vector_type::wrap_const_reverse_iterator_type wrap_s32_vector_const_reverse_iterator_type;

typedef yggr::lua_wrap::wrap_vector<yggr::u32> u32_vector_type;
typedef u32_vector_type::wrap_allocator_type wrap_u32_vector_allocator_type;
typedef u32_vector_type::wrap_iterator_type wrap_u32_vector_iterator_type;
typedef u32_vector_type::wrap_const_iterator_type wrap_u32_vector_const_iterator_type;
typedef u32_vector_type::wrap_reverse_iterator_type wrap_u32_vector_reverse_iterator_type;
typedef u32_vector_type::wrap_const_reverse_iterator_type wrap_u32_vector_const_reverse_iterator_type;

// deque
typedef yggr::lua_wrap::wrap_deque<yggr::s32> s32_deque_type;
typedef s32_deque_type::wrap_allocator_type wrap_s32_deque_allocator_type;
typedef s32_deque_type::wrap_iterator_type wrap_s32_deque_iterator_type;
typedef s32_deque_type::wrap_const_iterator_type wrap_s32_deque_const_iterator_type;
typedef s32_deque_type::wrap_reverse_iterator_type wrap_s32_deque_reverse_iterator_type;
typedef s32_deque_type::wrap_const_reverse_iterator_type wrap_s32_deque_const_reverse_iterator_type;

typedef yggr::lua_wrap::wrap_deque<yggr::u32> u32_deque_type;
typedef u32_deque_type::wrap_allocator_type wrap_u32_deque_allocator_type;
typedef u32_deque_type::wrap_iterator_type wrap_u32_deque_iterator_type;
typedef u32_deque_type::wrap_const_iterator_type wrap_u32_deque_const_iterator_type;
typedef u32_deque_type::wrap_reverse_iterator_type wrap_u32_deque_reverse_iterator_type;
typedef u32_deque_type::wrap_const_reverse_iterator_type wrap_u32_deque_const_reverse_iterator_type;

// list
typedef yggr::lua_wrap::wrap_list<yggr::s32> s32_list_type;
typedef s32_list_type::wrap_allocator_type wrap_s32_list_allocator_type;
typedef s32_list_type::wrap_iterator_type wrap_s32_list_iterator_type;
typedef s32_list_type::wrap_const_iterator_type wrap_s32_list_const_iterator_type;
typedef s32_list_type::wrap_reverse_iterator_type wrap_s32_list_reverse_iterator_type;
typedef s32_list_type::wrap_const_reverse_iterator_type wrap_s32_list_const_reverse_iterator_type;

typedef yggr::lua_wrap::wrap_list<yggr::u32> u32_list_type;
typedef u32_list_type::wrap_allocator_type wrap_u32_list_allocator_type;
typedef u32_list_type::wrap_iterator_type wrap_u32_list_iterator_type;
typedef u32_list_type::wrap_const_iterator_type wrap_u32_list_const_iterator_type;
typedef u32_list_type::wrap_reverse_iterator_type wrap_u32_list_reverse_iterator_type;
typedef u32_list_type::wrap_const_reverse_iterator_type wrap_u32_list_const_reverse_iterator_type;

/*
!!! GCC must use 2 different namespaces to separate these BOOST_MPL_ASSERT to avoid !!!
!!! error: 'mpl_assertion_in_line_96' conflicts with a previous declaration !!!
*/

namespace detail_d32
{
BOOST_MPL_ASSERT((boost::is_same<wrap_s32_vector_allocator_type, wrap_s32_deque_allocator_type>));
BOOST_MPL_ASSERT((boost::is_same<wrap_u32_vector_allocator_type, wrap_u32_deque_allocator_type>));

BOOST_MPL_ASSERT((boost::is_same<wrap_s32_vector_allocator_type, wrap_s32_list_allocator_type>));
BOOST_MPL_ASSERT((boost::is_same<wrap_u32_vector_allocator_type, wrap_u32_list_allocator_type>));
} // namespace detail_d32

typedef wrap_s32_vector_allocator_type wrap_s32_allocator_type;
typedef wrap_u32_vector_allocator_type wrap_u32_allocator_type;

void export_d32_s32_allocator(lua_State* L);
void export_d32_u32_allocator(lua_State* L);

namespace detail
{

void export_d32_s32_vector_wrap_s32_vector_iterator(void* pcobj);
void export_d32_s32_vector_wrap_s32_vector_const_iterator(void* pcobj);
void export_d32_s32_vector_wrap_s32_vector_reverse_iterator(void* pcobj);
void export_d32_s32_vector_wrap_s32_vector_const_reverse_iterator(void* pcobj);
void export_d32_s32_vector_s32_vector(void* pcobj);
void export_d32_s32_vector_s32_vector_part_eins(void* pcobj);
void export_d32_s32_vector_s32_vector_part_zwei(void* pcobj);

void export_d32_u32_vector_wrap_u32_vector_iterator(void* pcobj);
void export_d32_u32_vector_wrap_u32_vector_const_iterator(void* pcobj);
void export_d32_u32_vector_wrap_u32_vector_reverse_iterator(void* pcobj);
void export_d32_u32_vector_wrap_u32_vector_const_reverse_iterator(void* pcobj);
void export_d32_u32_vector_u32_vector(void* pcobj);
void export_d32_u32_vector_u32_vector_part_eins(void* pcobj);
void export_d32_u32_vector_u32_vector_part_zwei(void* pcobj);

} // namespace detail

void export_d32_s32_vector(lua_State* L);
void export_d32_u32_vector(lua_State* L);

namespace detail
{

void export_d32_s32_list_wrap_s32_list_iterator(void* pcobj);
void export_d32_s32_list_wrap_s32_list_const_iterator(void* pcobj);
void export_d32_s32_list_wrap_s32_list_reverse_iterator(void* pcobj);
void export_d32_s32_list_wrap_s32_list_const_reverse_iterator(void* pcobj);
void export_d32_s32_list_s32_list(void* pcobj);
void export_d32_s32_list_s32_list_part_eins(void* pcobj);
void export_d32_s32_list_s32_list_part_zwei(void* pcobj);


void export_d32_u32_list_wrap_u32_list_iterator(void* pcobj);
void export_d32_u32_list_wrap_u32_list_const_iterator(void* pcobj);
void export_d32_u32_list_wrap_u32_list_reverse_iterator(void* pcobj);
void export_d32_u32_list_wrap_u32_list_const_reverse_iterator(void* pcobj);
void export_d32_u32_list_u32_list(void* pcobj);
void export_d32_u32_list_u32_list_part_eins(void* pcobj);
void export_d32_u32_list_u32_list_part_zwei(void* pcobj);

} // namespace detail

void export_d32_s32_list(lua_State* L);
void export_d32_u32_list(lua_State* L);


namespace detail
{

void export_d32_s32_queue_wrap_s32_queue_iterator(void* pcobj);
void export_d32_s32_queue_wrap_s32_queue_const_iterator(void* pcobj);
void export_d32_s32_queue_wrap_s32_queue_reverse_iterator(void* pcobj);
void export_d32_s32_queue_wrap_s32_queue_const_reverse_iterator(void* pcobj);
void export_d32_s32_queue_s32_queue(void* pcobj);
void export_d32_s32_queue_s32_queue_part_eins(void* pcobj);
void export_d32_s32_queue_s32_queue_part_zwei(void* pcobj);

void export_d32_u32_queue_wrap_u32_queue_iterator(void* pcobj);
void export_d32_u32_queue_wrap_u32_queue_const_iterator(void* pcobj);
void export_d32_u32_queue_wrap_u32_queue_reverse_iterator(void* pcobj);
void export_d32_u32_queue_wrap_u32_queue_const_reverse_iterator(void* pcobj);
void export_d32_u32_queue_u32_queue(void* pcobj);
void export_d32_u32_queue_u32_queue_part_eins(void* pcobj);
void export_d32_u32_queue_u32_queue_part_zwei(void* pcobj);

} // namespace detail

void export_d32_s32_deque(lua_State* L);
void export_d32_u32_deque(lua_State* L);


} // namespace lua
} // namespace yggr


#endif //__YGGR_LUA_YGGR_LUA_D32_HPP__
