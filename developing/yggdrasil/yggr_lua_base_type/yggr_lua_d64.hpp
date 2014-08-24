//yggr_lua_base_type.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_LUA_LUA_D64_HPP__
#define __YGGR_LUA_LUA_D64_HPP__

#include <yggr/base/yggrdef.h>
#include <lua.hpp>
#include <yggr/script/base_lua_environment.hpp>

#include <yggr/lua_wrap/wrap_base_t.hpp>
#include <yggr/lua_wrap/wrap_vector.hpp>
#include <yggr/lua_wrap/wrap_list.hpp>

namespace yggr
{
namespace lua
{

typedef yggr::lua_wrap::wrap_base_t<yggr::u64> u64_type;
typedef yggr::lua_wrap::wrap_base_t<yggr::s64> s64_type;

typedef lua_wrap::wrap_vector<u64_type> u64_vector_type;
typedef lua_wrap::wrap_vector<s64_type> s64_vector_type;

typedef lua_wrap::wrap_list<u64_type> u64_list_type;
typedef lua_wrap::wrap_list<s64_type> s64_list_type;

void export_d64_s64(lua_State* L);
void export_d64_u64(lua_State* L);

void export_d64_s64_vector(lua_State* L);
void export_d64_u64_vector(lua_State* L);

void export_d64_s64_list(lua_State* L);
void export_d64_u64_list(lua_State* L);

} // namespace lua
} // namespace yggr


#endif //__YGGR_LUA_LUA_D64_HPP__