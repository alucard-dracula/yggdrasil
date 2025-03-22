// msvc_link.hpp
// Copyright (c) 2005 xuyang

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

#ifndef MSVC_LINK_HPP_INCLUDED
#define MSVC_LINK_HPP_INCLUDED

#include <luabind/lua_include.hpp>
#include <luabind/config.hpp>

#if defined(_MSC_VER)

#include <boost/preprocessor/cat.hpp>

#define LUABIND_CAST( __x__, __to__ ) __to__( __x__ )
#define LUABIND_CAST_TO_STRING( __x__ ) #__x__
#define LUABIND_CAST_TO_CODE( __x__ ) __x__

#if _MSC_VER > 1200
#	if _MSC_VER == 1300
#		define LUABIND_MSVC_NUMBER() 70
#	elif _MSC_VER == 1310
#		define LUABIND_MSVC_NUMBER() 71
#	elif _MSC_VER == 1400
#		define LUABIND_MSVC_NUMBER() 80
#	elif _MSC_VER == 1500
#		define LUABIND_MSVC_NUMBER() 90
#	elif _MSC_VER == 1600
#		define LUABIND_MSVC_NUMBER() 100
#	elif _MSC_VER == 1700
#		define LUABIND_MSVC_NUMBER() 110
#	elif _MSC_VER == 1800
#		define LUABIND_MSVC_NUMBER() 120
#	elif _MSC_VER == 1900
#		define LUABIND_MSVC_NUMBER() 140
#	elif _MSC_VER == 1924
#		define LUABIND_MSVC_NUMBER() 142
#	elif _MSC_VER > 1924 && _MSC_VER < 2000
#		define LUABIND_MSVC_NUMBER() 143
#	else
#		error "!!!!unknown _MSC_VER!!!!!"
#	endif // _MSC_VER
#else
#	error "!!!!yggdrasil not support msvc6 and before!!!!!"
#endif //_MSV_VER

#define LUABIND_COMPILE_TAG() -vc

#define LUABIND_COMPILE_VERSION_TAG() BOOST_PP_CAT(LUABIND_COMPILE_TAG(), LUABIND_MSVC_NUMBER())

#ifdef _DEBUG
#	define LUABIND_COMPILE_DEBUG_TAG() -d
#else
#	define LUABIND_COMPILE_DEBUG_TAG()
#endif // _DEBUG

#if defined(LUAJIT_VERSION_NUM)
#	define LUABIND_LUA_LINK_VER() BOOST_PP_CAT(jit, LUA_VERSION_NUM)
#else
#	define LUABIND_LUA_LINK_VER() LUA_VERSION_NUM
#endif // LUAJIT_VERSION_NUM

#define LUABIND_LIB_LUA_LINK_TITLE() BOOST_PP_CAT(LUABIND_LIB_LUA_TITLE(), LUABIND_LUA_LINK_VER())

#define LUABIND_COMPILE_LINK_LIB( ) \
			(lib, LUABIND_CAST(BOOST_PP_CAT(liblua, LUABIND_LUA_LINK_VER())LUABIND_COMPILE_VERSION_TAG()LUABIND_COMPILE_DEBUG_TAG().lib, \
									LUABIND_CAST_TO_STRING))

#define LINK_LUABIND_IMPL_LIB( ) \
			(lib, LUABIND_CAST(LUABIND_CAST(libluabind09, LUABIND_CAST_TO_CODE)LUABIND_COMPILE_VERSION_TAG()LUABIND_COMPILE_DEBUG_TAG().lib, \
									LUABIND_CAST_TO_STRING))

#define LINK_LUABIND_STATIC_LIB( ) \
			(lib, LUABIND_CAST(LUABIND_CAST(libluabind09, LUABIND_CAST_TO_CODE)LUABIND_COMPILE_VERSION_TAG()-sLUABIND_COMPILE_DEBUG_TAG().lib, \
									LUABIND_CAST_TO_STRING))

#define LINK_LUABIND_TS_STATIC_LIB( ) \
			(lib, LUABIND_CAST(LUABIND_CAST(libluabind09_ts, LUABIND_CAST_TO_CODE)LUABIND_COMPILE_VERSION_TAG()-sLUABIND_COMPILE_DEBUG_TAG().lib, \
									LUABIND_CAST_TO_STRING))

#endif // (_MSC_VER)

#endif // MSVC_LINK_HPP_INCLUDED
