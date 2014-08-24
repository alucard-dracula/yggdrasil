//yggr_lua_base_type.cpp

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

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER

#include <lua.hpp>
#include <iostream>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/lua_include.hpp>
#include <yggr/script/base_lua_environment.hpp>

#include "yggr_lua_base_type.hpp"

#include <yggr/dll_make/dll_mark_define.hpp>

#include <yggr/dll_make/dll_make_begin.ipp>

YGGR_DLL_API int LUA_MODULE_INIT_FOO(yggr_lua_base_type)(lua_State* L);

#include <yggr/dll_make/dll_make_end.ipp>


int LUA_MODULE_INIT_FOO(yggr_lua_base_type)(lua_State* L)
{
	
#ifdef _DEBUG
	std::cout << "luaopen_yggr_lua_base_type" << std::endl;
#endif //_DEBUG

	luabind::open(L);

	yggr::lua::export_d32_s32_vector(L);
	yggr::lua::export_d32_u32_vector(L);
	yggr::lua::export_d32_s32_list(L);
	yggr::lua::export_d32_u32_list(L);

	yggr::lua::export_d64_s64(L);
	yggr::lua::export_d64_u64(L);
	yggr::lua::export_d64_s64_vector(L);
	yggr::lua::export_d64_u64_vector(L);
	yggr::lua::export_d64_s64_list(L);
	yggr::lua::export_d64_u64_list(L);

	//yggr::lua::export_dstrings(L);

	return 0;
}