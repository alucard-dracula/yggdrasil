//lua_script.cpp

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

#include <yggr/script/lua_script.hpp>

namespace yggr
{
namespace script
{
namespace lua
{

//lua_script::lua_script(const std::string& enter_foo, const std::string& code, u32 env)
//	: _lua_env(luaL_newstate()), _enter_foo(enter_foo)
//{
//	open_libs(env, code);
//	compile_from_string(code);
//}

lua_script::lua_script(const std::string& enter_foo, const string_code& code, u32 env)
	: _lua_env(luaL_newstate()), _enter_foo(enter_foo)
{
	open_libs(env, code);
	compile_from_string(code);
}

lua_script::lua_script(const std::string& enter_foo, const file_code& code, u32 env)
	: _lua_env(luaL_newstate()), _enter_foo(enter_foo)
{
	open_libs(env, code);
	compile_from_file(code);
}

lua_script::~lua_script(void)
{
	if(_lua_env) lua_close(_lua_env);
}

void lua_script::open_libs(u32 env)
{
	if((!_lua_env) || (!env))
	{
		return;
	}

	if(env == lua_env_info_type::E_lib_all)
	{
		luaL_openlibs(_lua_env);
		return;
	}

	if(env & lua_env_info_type::E_lib_base) luaopen_base(_lua_env);
	if(env & lua_env_info_type::E_lib_package) luaopen_package(_lua_env);
	if(env & lua_env_info_type::E_lib_string) luaopen_string(_lua_env);
	if(env & lua_env_info_type::E_lib_table) luaopen_table(_lua_env);
	if(env & lua_env_info_type::E_lib_math) luaopen_math(_lua_env);
	if(env & lua_env_info_type::E_lib_io) luaopen_io(_lua_env);
	if(env & lua_env_info_type::E_lib_os) luaopen_os(_lua_env);
	if(env & lua_env_info_type::E_lib_debug) luaopen_debug(_lua_env);
}

void lua_script::open_libs(u32 g_env_code, u32 l_env_code)
{
	u32 tenv = g_env_code | l_env_code;
	open_libs(tenv);
}

void lua_script::compile_from_string(const std::string& code)
{
	if(!_lua_env)
	{
		return;
	}

	if(luaL_dostring(_lua_env, code.c_str()))
	{
		const char* str_err = lua_tostring(_lua_env, -1);
		lua_pop(_lua_env, 1 );
		exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_compile_error,
													code + std::string("\r\n") + std::string(get_exception_msg())));
		lua_close(_lua_env);
		_lua_env = 0;
	}
}

void lua_script::compile_from_file(const std::string& fname)
{
	if(!_lua_env)
	{
		return;
	}

	if(luaL_dofile(_lua_env, fname.c_str()))
	{
		exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_compile_error,
													fname + std::string("\r\n") + std::string(get_exception_msg())));
		lua_close(_lua_env);
		_lua_env = 0;
	}
}

const char* lua_script::get_exception_msg(void) const
{
	if(!_lua_env)
	{
		return "empty lua environment";
	}

	const char* str_err = lua_tostring(_lua_env, -1);

	if(!str_err)
	{
		return "lua environment error";
	}

	lua_pop(_lua_env, 1 );
	return str_err;
}

} // namespace lua
} // namespace script
} // namespace yggr
