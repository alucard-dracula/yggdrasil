//lua_script.hpp

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

#ifndef __YGGR_SCRIPT_LUA_LUA_SCRIPT_HPP__
#define __YGGR_SCRIPT_LUA_LUA_SCRIPT_HPP__

#include <luabind/luabind.hpp>

#include <compatibility/stl_exception.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/script/lua_environment.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/base/exception.hpp>

#include <yggr/base/error_make.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4800)
#endif //_MSC_VER

namespace yggr
{
namespace script
{
namespace lua
{

class lua_script : private nonable::noncopyable
{
private:
	typedef lua_environment lua_env_info_type;
	typedef	lua_env_info_type::lua_env_type lua_env_type;

private:
	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("lua_script")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_19)
			ERROR_CODE_DEF(E_lua_code_compile_error)
			ERROR_CODE_DEF(E_lua_code_runtime_error)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_19)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_lua_code_compile_error, "lua code compile error")
			ERROR_CODE_MSG(E_lua_code_runtime_error, "lua code runtime error")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------
public:
	class string_code
	{
	public:
		string_code(const std::string& code)
			: str_code(code), env(0)
		{
		}


		string_code(const std::string& code, u32 nenv)
			: str_code(code), env(nenv)
		{
		}

		string_code(const string_code& right)
			: str_code(right.str_code), env(right.env)
		{
		}

		~string_code(void)
		{
		}

		operator const std::string&(void) const
		{
			return str_code;
		}

		operator const u32&(void) const
		{
			return env;
		}

		const std::string& str_code;
		u32 env;
	};

	class file_code
	{
	public:
		file_code(const std::string& fname)
			: file_name(fname), env(0)
		{
		}

		file_code(const std::string& fname, u32 nenv)
			: file_name(fname), env(nenv)
		{
		}

		file_code(const file_code& right)
			: file_name(right.file_name), env(right.env)
		{
		}

		~file_code(void)
		{
		}

		operator const std::string&(void) const
		{
			return file_name;
		}

		operator const u32&(void) const
		{
			return env;
		}

		const std::string& file_name;
		u32 env;
	};

public:
	//lua_script(const std::string& enter_foo, const std::string& code, u32 env);
	lua_script(const std::string& enter_foo, const string_code& code, u32 env);
	lua_script(const std::string& enter_foo, const file_code& code, u32 env);
	~lua_script(void);

	inline bool success(void) const
	{
		return (_lua_env) && (!_enter_foo.empty());
	}

	template<typename Output_Real_Data, typename Input_Real_Data>
	Output_Real_Data execute_rule(const Input_Real_Data& input)
	{
		typedef Output_Real_Data o_real_data_type;

		if(!_lua_env)
		{
			return o_real_data_type();
		}
		try
		{
			//return luabind::call_function<o_real_data_type>(_lua_env, _enter_foo.c_str(), boost::cref(input));
			return luabind::call_function<o_real_data_type>(_lua_env, _enter_foo.c_str(), input);
		}
		catch(const luabind::error& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}
		catch(const luabind::cast_failed& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}

		return o_real_data_type();
	}

	template<typename Output_Real_Data>
	Output_Real_Data execute_rule(void)
	{
		typedef Output_Real_Data o_real_data_type;

		if(!_lua_env)
		{
			return o_real_data_type();
		}
		try
		{
			return luabind::call_function<o_real_data_type>(_lua_env, _enter_foo.c_str());
		}
		catch(const luabind::error& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}
		catch(const luabind::cast_failed& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}

		return o_real_data_type();
	}

	template<typename Input_Real_Data>
	void execute_rule_no_ret(const Input_Real_Data& input)
	{
		if(!_lua_env)
		{
			return;
		}

		try
		{
			luabind::call_function<void>(_lua_env, _enter_foo.c_str(), input);
			//luabind::call_function<void>(_lua_env, _enter_foo.c_str(), boost::cref(input));
		}
		catch(luabind::error& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}
		catch(luabind::cast_failed& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}
		catch(compatibility::stl_exception& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}
		return;
	}

	void execute_rule_no_ret(void)
	{
		if(!_lua_env)
		{
			return;
		}

		try
		{
			luabind::call_function<void>(_lua_env, _enter_foo.c_str());
		}
		catch(luabind::error& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}
		catch(luabind::cast_failed& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}
		catch(compatibility::stl_exception& e)
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_lua_code_runtime_error, 
														e.what() + std::string("\r\n") + std::string(get_exception_msg())));
		}
		return;
	}

private:

	const char* get_exception_msg(void) const;
	void open_libs(u32 env);
	void open_libs(u32 g_env_code, u32 l_env_code);

	void compile_from_string(const std::string& code);
	void compile_from_file(const std::string& fname);

private:
	std::string _enter_foo;
	lua_env_type _lua_env;
};

} // namespace lua
} // namespace script
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif // __YGGR_SCRIPT_LUA_LUA_SCRIPT_HPP__
