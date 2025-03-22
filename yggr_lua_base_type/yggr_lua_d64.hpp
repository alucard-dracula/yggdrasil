//yggr_lua_d64.hpp

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

#ifndef __YGGR_LUA_YGGR_LUA_D64_HPP__
#define __YGGR_LUA_YGGR_LUA_D64_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/script/lua_base_environment.hpp>
#include <yggr/script/lua_config.hpp>

#include <yggr/lua_wrap/wrap_vector.hpp>
#include <yggr/lua_wrap/wrap_deque.hpp>
#include <yggr/lua_wrap/wrap_list.hpp>

#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace lua
{

typedef lua_wrap::wrap_base_t_maker<yggr::s64>::type wrap_s64_type;
typedef lua_wrap::wrap_base_t_maker<yggr::u64>::type wrap_u64_type;

// vector
typedef yggr::lua_wrap::wrap_vector<yggr::s64> s64_vector_type;
typedef s64_vector_type::wrap_allocator_type wrap_s64_vector_allocator_type;
typedef s64_vector_type::wrap_iterator_type wrap_s64_vector_iterator_type;
typedef s64_vector_type::wrap_const_iterator_type wrap_s64_vector_const_iterator_type;
typedef s64_vector_type::wrap_reverse_iterator_type wrap_s64_vector_reverse_iterator_type;
typedef s64_vector_type::wrap_const_reverse_iterator_type wrap_s64_vector_const_reverse_iterator_type;

typedef yggr::lua_wrap::wrap_vector<yggr::u64> u64_vector_type;
typedef u64_vector_type::wrap_allocator_type wrap_u64_vector_allocator_type;
typedef u64_vector_type::wrap_iterator_type wrap_u64_vector_iterator_type;
typedef u64_vector_type::wrap_const_iterator_type wrap_u64_vector_const_iterator_type;
typedef u64_vector_type::wrap_reverse_iterator_type wrap_u64_vector_reverse_iterator_type;
typedef u64_vector_type::wrap_const_reverse_iterator_type wrap_u64_vector_const_reverse_iterator_type;

// deque
typedef yggr::lua_wrap::wrap_deque<yggr::s64> s64_deque_type;
typedef s64_deque_type::wrap_allocator_type wrap_s64_deque_allocator_type;
typedef s64_deque_type::wrap_iterator_type wrap_s64_deque_iterator_type;
typedef s64_deque_type::wrap_const_iterator_type wrap_s64_deque_const_iterator_type;
typedef s64_deque_type::wrap_reverse_iterator_type wrap_s64_deque_reverse_iterator_type;
typedef s64_deque_type::wrap_const_reverse_iterator_type wrap_s64_deque_const_reverse_iterator_type;

typedef yggr::lua_wrap::wrap_deque<yggr::u64> u64_deque_type;
typedef u64_deque_type::wrap_allocator_type wrap_u64_deque_allocator_type;
typedef u64_deque_type::wrap_iterator_type wrap_u64_deque_iterator_type;
typedef u64_deque_type::wrap_const_iterator_type wrap_u64_deque_const_iterator_type;
typedef u64_deque_type::wrap_reverse_iterator_type wrap_u64_deque_reverse_iterator_type;
typedef u64_deque_type::wrap_const_reverse_iterator_type wrap_u64_deque_const_reverse_iterator_type;

// list
typedef yggr::lua_wrap::wrap_list<yggr::s64> s64_list_type;
typedef s64_list_type::wrap_allocator_type wrap_s64_list_allocator_type;
typedef s64_list_type::wrap_iterator_type wrap_s64_list_iterator_type;
typedef s64_list_type::wrap_const_iterator_type wrap_s64_list_const_iterator_type;
typedef s64_list_type::wrap_reverse_iterator_type wrap_s64_list_reverse_iterator_type;
typedef s64_list_type::wrap_const_reverse_iterator_type wrap_s64_list_const_reverse_iterator_type;

typedef yggr::lua_wrap::wrap_list<yggr::u64> u64_list_type;
typedef u64_list_type::wrap_allocator_type wrap_u64_list_allocator_type;
typedef u64_list_type::wrap_iterator_type wrap_u64_list_iterator_type;
typedef u64_list_type::wrap_const_iterator_type wrap_u64_list_const_iterator_type;
typedef u64_list_type::wrap_reverse_iterator_type wrap_u64_list_reverse_iterator_type;
typedef u64_list_type::wrap_const_reverse_iterator_type wrap_u64_list_const_reverse_iterator_type;

/*
!!! GCC must use 2 different namespaces to separate these BOOST_MPL_ASSERT to avoid !!!
!!! error: 'mpl_assertion_in_line_96' conflicts with a previous declaration !!!
*/
namespace detail_d64
{

BOOST_MPL_ASSERT((boost::is_same<wrap_s64_vector_allocator_type, wrap_s64_deque_allocator_type>));
BOOST_MPL_ASSERT((boost::is_same<wrap_u64_vector_allocator_type, wrap_u64_deque_allocator_type>));

BOOST_MPL_ASSERT((boost::is_same<wrap_s64_vector_allocator_type, wrap_s64_list_allocator_type>));
BOOST_MPL_ASSERT((boost::is_same<wrap_u64_vector_allocator_type, wrap_u64_list_allocator_type>));

} // namespace detail_d64

typedef wrap_s64_vector_allocator_type wrap_s64_allocator_type;
typedef wrap_u64_vector_allocator_type wrap_u64_allocator_type;

void export_d64_s64(lua_State* L);
void export_d64_u64(lua_State* L);

void export_d64_s64_allocator(lua_State* L);
void export_d64_u64_allocator(lua_State* L);

namespace detail
{

void export_d64_s64_vector_wrap_s64_vector_iterator(void* pcobj);
void export_d64_s64_vector_wrap_s64_vector_const_iterator(void* pcobj);
void export_d64_s64_vector_wrap_s64_vector_reverse_iterator(void* pcobj);
void export_d64_s64_vector_wrap_s64_vector_const_reverse_iterator(void* pcobj);
void export_d64_s64_vector_s64_vector(void* pcobj);
void export_d64_s64_vector_s64_vector_part_eins(void* pcobj);
void export_d64_s64_vector_s64_vector_part_zwei(void* pcobj);

void export_d64_u64_vector_wrap_u64_vector_iterator(void* pcobj);
void export_d64_u64_vector_wrap_u64_vector_const_iterator(void* pcobj);
void export_d64_u64_vector_wrap_u64_vector_reverse_iterator(void* pcobj);
void export_d64_u64_vector_wrap_u64_vector_const_reverse_iterator(void* pcobj);
void export_d64_u64_vector_u64_vector(void* pcobj);
void export_d64_u64_vector_u64_vector_part_eins(void* pcobj);
void export_d64_u64_vector_u64_vector_part_zwei(void* pcobj);

} // namespace detail

void export_d64_s64_vector(lua_State* L);
void export_d64_u64_vector(lua_State* L);

namespace detail
{

void export_d64_s64_queue_wrap_s64_queue_iterator(void* pcobj);
void export_d64_s64_queue_wrap_s64_queue_const_iterator(void* pcobj);
void export_d64_s64_queue_wrap_s64_queue_reverse_iterator(void* pcobj);
void export_d64_s64_queue_wrap_s64_queue_const_reverse_iterator(void* pcobj);
void export_d64_s64_queue_s64_queue(void* pcobj);
void export_d64_s64_queue_s64_queue_part_eins(void* pcobj);
void export_d64_s64_queue_s64_queue_part_zwei(void* pcobj);

void export_d64_u64_queue_wrap_u64_queue_iterator(void* pcobj);
void export_d64_u64_queue_wrap_u64_queue_const_iterator(void* pcobj);
void export_d64_u64_queue_wrap_u64_queue_reverse_iterator(void* pcobj);
void export_d64_u64_queue_wrap_u64_queue_const_reverse_iterator(void* pcobj);
void export_d64_u64_queue_u64_queue(void* pcobj);
void export_d64_u64_queue_u64_queue_part_eins(void* pcobj);
void export_d64_u64_queue_u64_queue_part_zwei(void* pcobj);

} // namespace detail

void export_d64_s64_deque(lua_State* L);
void export_d64_u64_deque(lua_State* L);

namespace detail
{

void export_d64_s64_list_wrap_s64_list_iterator(void* pcobj);
void export_d64_s64_list_wrap_s64_list_const_iterator(void* pcobj);
void export_d64_s64_list_wrap_s64_list_reverse_iterator(void* pcobj);
void export_d64_s64_list_wrap_s64_list_const_reverse_iterator(void* pcobj);
void export_d64_s64_list_s64_list(void* pcobj);
void export_d64_s64_list_s64_list_part_eins(void* pcobj);
void export_d64_s64_list_s64_list_part_zwei(void* pcobj);

void export_d64_u64_list_wrap_u64_list_iterator(void* pcobj);
void export_d64_u64_list_wrap_u64_list_const_iterator(void* pcobj);
void export_d64_u64_list_wrap_u64_list_reverse_iterator(void* pcobj);
void export_d64_u64_list_wrap_u64_list_const_reverse_iterator(void* pcobj);
void export_d64_u64_list_u64_list(void* pcobj);
void export_d64_u64_list_u64_list_part_eins(void* pcobj);
void export_d64_u64_list_u64_list_part_zwei(void* pcobj);

} // namespace detail

void export_d64_s64_list(lua_State* L);
void export_d64_u64_list(lua_State* L);

} // namespace lua
} // namespace yggr

#endif //__YGGR_LUA_YGGR_LUA_D64_HPP__
