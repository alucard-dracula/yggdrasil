// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef LUA_INCLUDE_HPP_INCLUDED
#define LUA_INCLUDE_HPP_INCLUDED

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) || defined(WINDOWS)
#	if !defined(_WIN32)
#		define _WIN32
#	endif // _WIN32
#elif (defined(__APPLE__) || defined(__APPLE_CC__))
#	if !defined(LUA_USE_MACOSX)
#		define LUA_USE_MACOSX
#	endif // LUA_USE_MACOSX
#elif defined(__linux__) || defined(__linux)
#	if !defined(LUA_USE_LINUX)
#		define LUA_USE_LINUX
#	endif // LUA_USE_LINUX
#else
#	if !defined(LUA_USE_LINUX)
#		define LUA_USE_LINUX
#	endif // LUA_USE_LINUX	
#endif // WIN32

// lua514 needed
#if !defined(LUA_USELONGLONG)
#	define LUA_USELONGLONG
#endif // LUA_USELONGLONG

// lua521 neeed
#if !defined(LUA_COMPAT_ALL)
#	define LUA_COMPAT_ALL
#endif // LUA_COMPAT_ALL

// lua534 neeed
#if !defined(LUA_COMPAT_5_2)
#	define LUA_COMPAT_5_2
#endif // LUA_COMPAT_5_2

#if !defined(LUA_COMPAT_5_1)
#	define LUA_COMPAT_5_1
#endif // LUA_COMPAT_5_1

#ifdef __cplusplus

#	include <lua.hpp>

#else

#	ifndef LUABIND_CPLUSPLUS_LUA
	extern "C"
	{
#	endif

#		include <lua.h>
#		include <lauxlib.h>

#	ifndef LUABIND_CPLUSPLUS_LUA
	}
#	endif
#endif // __cplusplus



#endif // LUA_INCLUDE_HPP_INCLUDED

