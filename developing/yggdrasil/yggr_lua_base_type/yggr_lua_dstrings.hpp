//yggr_lua_strings.hpp

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

#ifndef __YGGR_LUA_LUA_DSTRINGS_HPP__
#define __YGGR_LUA_LUA_DSTRINGS_HPP__

#include <yggr/base/yggrdef.h>
#include <lua.hpp>
#include <yggr/script/base_lua_environment.hpp>

#include <yggr/lua_wrap/wrap_vector.hpp>
#include <yggr/lua_wrap/wrap_list.hpp>
#include <yggr/charset/utf8_string.hpp>


namespace yggr
{
namespace lua
{

typedef yggr::charset::utf8_string utf8_string_type;
typedef lua_wrap::wrap_vector<utf8_string_type> utf8_string_vector_type;
typedef lua_wrap::wrap_vector<std::string> string_vector_type;
typedef lua_wrap::wrap_list<utf8_string_type> utf8_string_list_type;
typedef lua_wrap::wrap_list<std::string> string_list_type;

void export_dstrings(lua_State* L);

} // namespace lua
} // namespace yggr


#endif //__YGGR_LUA_LUA_STRINGS_HPP__