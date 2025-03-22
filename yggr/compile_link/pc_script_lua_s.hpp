//pc_script_lua_s.hpp

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

#ifndef __YGGR_COMPILE_LINK_PC_SCRIPT_LUA_S_HPP__
#define __YGGR_COMPILE_LINK_PC_SCRIPT_LUA_S_HPP__

#include <yggr/compile_link/msvc_cfg.hpp>
#include <yggr/script/lua_config.hpp>

#ifdef _MSC_VER

#	pragma comment YGGR_COMPILE_LINK_LIB(yggr_script_lua)
#	pragma comment YGGR_COMPILE_LINK_STATIC_LIB(luabind09)

#	if !defined(LUAJIT_VERSION_NUM)
#		if (501 <= LUA_VERSION_NUM && LUA_VERSION_NUM < 502)
#			pragma comment YGGR_COMPILE_LINK_LIB(lua501)
#		elif (502 <= LUA_VERSION_NUM && LUA_VERSION_NUM < 503)
#			pragma comment YGGR_COMPILE_LINK_LIB(lua502)
#		elif (503 <= LUA_VERSION_NUM && LUA_VERSION_NUM <= 503)
#			pragma comment YGGR_COMPILE_LINK_LIB(lua503)
#		else
#			pragma comment YGGR_COMPILE_LINK_NRULE_LIB(lua)
#		endif //LUA_VERSION_NUM
#	else
#		if (501 <= LUA_VERSION_NUM && LUA_VERSION_NUM < 502)
#			pragma comment YGGR_COMPILE_LINK_LIB(luajit501)
#		elif (502 <= LUA_VERSION_NUM && LUA_VERSION_NUM < 503)
#			pragma comment YGGR_COMPILE_LINK_LIB(luajit502)
#		elif (503 <= LUA_VERSION_NUM && LUA_VERSION_NUM <= 503)
#			pragma comment YGGR_COMPILE_LINK_LIB(luajit503)
#		else
#			pragma comment YGGR_COMPILE_LINK_NRULE_LIB(luajit)
#		endif //LUA_VERSION_NUM
#	endif // LUAJIT_VERSION_NUM

#endif // _MSC_VER

#endif // __YGGR_COMPILE_LINK_PC_SCRIPT_LUA_S_HPP__
