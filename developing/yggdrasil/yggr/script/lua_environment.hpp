//lua_environment.hpp

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

#ifndef __YGGR_SCRIPT_LUA_LUA_ENVIRONMENT_HPP__
#define __YGGR_SCRIPT_LUA_LUA_ENVIRONMENT_HPP__


#include <lua.hpp>
#include <boost/shared_ptr.hpp>
#include <yggr/base/yggrdef.h>


namespace yggr
{
namespace script
{
namespace lua
{

class lua_environment
{
public:
	typedef lua_State* lua_env_type;
	
	enum
	{
		E_lua_libs = 0,
		E_lib_base = 0x00000001,
		E_lib_package = 0x00000002,
		E_lib_string = 0x00000004,
		E_lib_table = 0x00000008,
		E_lib_math = 0x000000010,
		E_lib_io = 0x00000020,
		E_lib_os = 0x00000040,
		E_lib_all = 0x0000007f,
			//E_lib_base | E_lib_package | E_lib_string 
			//	| E_lib_table | E_lib_math | E_lib_io 
			//	| E_lib_os 
		E_lib_debug = 0x00000080,
		E_compile_u32 = 0xffffffff
	};

public:
	lua_environment(void);
	~lua_environment(void);
	bool append_module(u32 mod_idx = E_lib_base);

	inline u32 get_grobal_environment(void) const
	{
		return _env_code;
	}
	

protected:
	u32 _env_code;
};

} // nemspace lua
} // namespace script
} // namespace yggr

#endif //__YGGR_SCRIPT_LUA_LUA_ENVIRONMENT_HPP__