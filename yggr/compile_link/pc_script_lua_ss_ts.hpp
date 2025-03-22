//pc_script_lua_ss_ts.hpp

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

#ifndef __YGGR_COMPILE_LINK_PC_SCRIPT_LUA_SS_TS_HPP__
#define __YGGR_COMPILE_LINK_PC_SCRIPT_LUA_SS_TS_HPP__

#include <yggr/compile_link/msvc_cfg.hpp>

#if !defined(YGGR_LUA_THREAD_SAFE)
#	error "using lua thread safe mode need defined YGGR_LUA_THREAD_SAFE (e.g in user_config.hpp)"
#endif // YGGR_LUA_THREAD_SAFE

#include <yggr/script/lua_config.hpp>
#include <pthread.h>

#ifdef _MSC_VER

#	pragma comment YGGR_COMPILE_LINK_LIB(yggr_script_lua)
#	pragma comment YGGR_COMPILE_LINK_STATIC_LIB(luabind09_ts)

#	if !defined(LUAJIT_VERSION_NUM)
#		if ((501 <= LUA_VERSION_NUM) && (LUA_VERSION_NUM < 502))
#			pragma comment YGGR_COMPILE_LINK_STATIC_LIB(lua501_ts)
#		elif ((502 <= LUA_VERSION_NUM) && (LUA_VERSION_NUM < 503))
#			pragma comment YGGR_COMPILE_LINK_STATIC_LIB(lua502_ts)
#		elif (503 <= LUA_VERSION_NUM && LUA_VERSION_NUM <= 503)
#			pragma comment YGGR_COMPILE_LINK_STATIC_LIB(lua503_ts)
#		else
#			pragma comment YGGR_COMPILE_LINK_NRULE_LIB(lua_ts)
#		endif //LUA_VERSION_NUM
#	else
#		error "!!! luajit is not support thread safe version !!!!"
#	endif //LUAJIT_VERSION_NUM

#	if defined(__PTW32_VERSION_MAJOR)
#		if (__PTW32_VERSION_MAJOR < 3)
#			pragma comment YGGR_COMPILE_LINK_NRULE_LIB(pthreadVC2)
#		else
#			pragma comment YGGR_COMPILE_LINK_NRULE_LIB(pthreadVC3)
#		endif // (__PTW32_VERSION_MAJOR < 3)
#	else
#		pragma comment YGGR_COMPILE_LINK_NRULE_LIB(pthreadVC2)
#	endif // defined(__PTW32_VERSION_MAJOR)

#endif // _MSC_VER

#endif // __YGGR_COMPILE_LINK_PC_SCRIPT_LUA_SS_TS_HPP__
