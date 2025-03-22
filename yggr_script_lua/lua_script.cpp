//lua_script.cpp

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

#include <yggr/script/lua_script.hpp>
#include <sstream>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace script
{
namespace lua
{

lua_script::lua_script(u32 global_env, u32 local_env)
	: _lua_env(luaL_newstate())
{
	open_libs(global_env, local_env);
}

lua_script::lua_script(const enter_foo_name_type& enter_foo,
						u32 global_env, u32 local_env)
	: _lua_env(luaL_newstate()), _enter_foo(enter_foo)
{
	open_libs(global_env, local_env);
}

lua_script::lua_script(const enter_foo_name_type& enter_foo,
						const string_code_type& code,
						u32 global_env, u32 local_env)
	: _lua_env(luaL_newstate()), _enter_foo(enter_foo)
{
	open_libs(global_env, local_env);

	try
	{
		compile_from_string(code);
	}
	catch(const error_type& e)
	{
		exception::exception::throw_error(e);
		lua_close(_lua_env);
		_lua_env = 0;
	}
}

lua_script::lua_script(const enter_foo_name_type& enter_foo,
						const file_code_type& code,
						u32 global_env, u32 local_env)
	: _lua_env(luaL_newstate()), _enter_foo(enter_foo)
{
	open_libs(global_env, local_env);

	try
	{
		compile_from_file(code);
	}
	catch(const error_type& e)
	{
		exception::exception::throw_error(e);
		lua_close(_lua_env);
		_lua_env = 0;
	}
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

#if LUA_VERSION_NUM <= 501

#define __YGGR_TMP_PP_LUAOPEN(__mark__, __env_sat__, __foo__, __name__) \
			if(env & __mark__) { \
				lua_pushcfunction(__env_sat__, __foo__); \
				lua_pushstring(__env_sat__, __name__); \
				lua_call(__env_sat__, 1, 0); }

	__YGGR_TMP_PP_LUAOPEN(lua_env_info_type::E_lib_base, _lua_env, luaopen_base, "");
	__YGGR_TMP_PP_LUAOPEN(lua_env_info_type::E_lib_package, _lua_env, luaopen_package, LUA_LOADLIBNAME);
	__YGGR_TMP_PP_LUAOPEN(lua_env_info_type::E_lib_table, _lua_env, luaopen_table, LUA_TABLIBNAME);
	__YGGR_TMP_PP_LUAOPEN(lua_env_info_type::E_lib_io, _lua_env, luaopen_io, LUA_IOLIBNAME);
	__YGGR_TMP_PP_LUAOPEN(lua_env_info_type::E_lib_os, _lua_env, luaopen_os, LUA_OSLIBNAME);
	__YGGR_TMP_PP_LUAOPEN(lua_env_info_type::E_lib_string, _lua_env, luaopen_string, LUA_STRLIBNAME);
	__YGGR_TMP_PP_LUAOPEN(lua_env_info_type::E_lib_math, _lua_env, luaopen_math, LUA_MATHLIBNAME);
	__YGGR_TMP_PP_LUAOPEN(lua_env_info_type::E_lib_debug, _lua_env, luaopen_debug, LUA_DBLIBNAME);

#undef __YGGR_TMP_PP_LUAOPEN

#else

	if(env & lua_env_info_type::E_lib_base) luaopen_base(_lua_env);
	if(env & lua_env_info_type::E_lib_package) luaopen_package(_lua_env);
	if(env & lua_env_info_type::E_lib_table) luaopen_table(_lua_env);
	if(env & lua_env_info_type::E_lib_io) luaopen_io(_lua_env);
	if(env & lua_env_info_type::E_lib_os) luaopen_os(_lua_env);
	if(env & lua_env_info_type::E_lib_string) luaopen_string(_lua_env);
	if(env & lua_env_info_type::E_lib_math) luaopen_math(_lua_env);
	if(env & lua_env_info_type::E_lib_debug) luaopen_debug(_lua_env);

#endif // LUA_VERSION_NUM
}

luabind::object lua_script::prv_get_function(const enter_foo_name_type& foo_name) const
{
	if(foo_name.empty())
	{
		return luabind::globals(_lua_env);
	}
	else
	{
		luabind::object obj = luabind::globals(_lua_env);
		std::size_t start = 0, end = 0;

		for(;enter_foo_name_type::npos != (end = foo_name.find('.', start)); start = end + 1)
		{
			obj = obj[foo_name.substr(start, end)];
			if(luabind::type(obj) != LUA_TTABLE)
			{
				return luabind::globals(_lua_env);
			}
		}

		return luabind::type(obj) == LUA_TTABLE?
				obj[foo_name.substr(start)] : obj;
	}
}

void lua_script::compile_from_string(const string_type& code)
{
	if(!(_lua_env
		&& 0 == luaL_dostring(_lua_env, code.data())))
	{
		error_type err(error_maker_type::make_error(
						error_maker_type::E_lua_code_compile_error,
						code.str<std::string>()
							+ std::string("\r\n")
							+ std::string(this_type::get_exception_msg())));

		throw err;

		// don't append this, because need continue append
		//lua_close(_lua_env);
		// _lua_env = 0; // need test
	}
}

void lua_script::compile_from_file(const string_type& fname)
{
	if(!(_lua_env && 0 == luaL_dofile(_lua_env, fname.data())))
	{
		error_type err(error_maker_type::make_error(
							error_maker_type::E_lua_code_compile_error,
							fname.str<std::string>()
								+ std::string("\r\n")
								+ std::string(this_type::get_exception_msg())));

		throw err;
		// don't append this, because need continue append
		// lua_close(_lua_env);
		// _lua_env = 0;
	}
}

::std::string lua_script::format_error_msg(const enter_foo_name_type& foo_name, const luabind::error& e) const
{
	std::stringstream ss;
	ss << "call foo: " << foo_name << ", "
		<< "message: " << e.what() << ", "
		<< "detail mesage: " << this_type::get_exception_msg();

	return ss.str();
}

::std::string lua_script::format_error_msg(const enter_foo_name_type& foo_name, const luabind::cast_failed& e) const
{
	std::stringstream ss;
	ss << "call foo: " << foo_name << ", "
		<< "message: " << e.what() << ", "
		<< "type id: " << e.info().name() << ", "
		<< "detail mesage: " << this_type::get_exception_msg();

	return ss.str();
}

::std::string lua_script::format_error_msg(const enter_foo_name_type& foo_name, const compatibility::stl_exception& e) const
{
	std::stringstream ss;
	ss << "call foo: " << foo_name << ", "
		<< "message: " << e.what() << ", "
		<< "detail mesage: " << this_type::get_exception_msg();

	return ss.str();
}

::std::string lua_script::get_exception_msg(void) const
{
	static const ::std::string str_ele = "empty lua environment";
	static const ::std::string str_lee = "lua environment error";

	if(!_lua_env)
	{
		return str_ele;
	}

	const char* c_str_err = lua_tostring(_lua_env, -1);

	if(!c_str_err)
	{
		return str_lee;
	}

	::std::string str_err(c_str_err);

	lua_pop(_lua_env, 1 );
	return str_err;
}

} // namespace lua
} // namespace script
} // namespace yggr
