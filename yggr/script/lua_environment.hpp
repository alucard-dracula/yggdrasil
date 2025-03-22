//lua_environment.hpp

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

#ifndef __YGGR_SCRIPT_LUA_ENVIRONMENT_HPP__
#define __YGGR_SCRIPT_LUA_ENVIRONMENT_HPP__

#include <yggr/script/lua_config.hpp>
#include <yggr/base/static_constant.hpp>

namespace yggr
{
namespace script
{
namespace lua
{

class lua_environment
{
private:
	typedef lua_environment this_type;

public:
	typedef lua_State* lua_env_type;
	
	YGGR_STATIC_CONSTANT(yggr::u32, E_lua_lib = 0x00000000);
	YGGR_STATIC_CONSTANT(yggr::u32, E_lib_base = 0x00000001);
	YGGR_STATIC_CONSTANT(yggr::u32, E_lib_package = 0x00000002);
	YGGR_STATIC_CONSTANT(yggr::u32, E_lib_string = 0x00000004);
	YGGR_STATIC_CONSTANT(yggr::u32, E_lib_table = 0x00000008);
	YGGR_STATIC_CONSTANT(yggr::u32, E_lib_math = 0x000000010);
	YGGR_STATIC_CONSTANT(yggr::u32, E_lib_io = 0x00000020);
	YGGR_STATIC_CONSTANT(yggr::u32, E_lib_os = 0x00000040);

	YGGR_STATIC_CONSTANT(
		yggr::u32, 
		E_lib_all = 
			this_type::E_lib_base
			| this_type::E_lib_package
			| this_type::E_lib_string
			| this_type::E_lib_table
			| this_type::E_lib_math
			| this_type::E_lib_io
			| this_type::E_lib_os); // 0x0000007f

	YGGR_STATIC_CONSTANT(yggr::u32, E_lib_debug = 0x00000080);

public:
	lua_environment(void)
		: _env_code(0)
	{
	}

	~lua_environment(void)
	{
	}
	
public:
	inline bool append_module(u32 mod_idx = E_lib_base)
	{
		_env_code |= mod_idx;
		return true;
	}

	inline u32 get_global_environment(void) const
	{
		return _env_code;
	}
	

protected:
	u32 _env_code;
};

} // nemspace lua
} // namespace script
} // namespace yggr

#endif //__YGGR_SCRIPT_LUA_ENVIRONMENT_HPP__