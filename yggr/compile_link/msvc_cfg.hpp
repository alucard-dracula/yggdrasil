//msvc_cfg.hpp

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

#ifndef __YGGR_COMPILE_LINK_MSVC_CFG_HPP__
#define __YGGR_COMPILE_LINK_MSVC_CFG_HPP__

#if defined(_MSC_VER)

#include <yggr/base/yggrdef.h>

#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>
#include <yggr/ppex/cast_to_identity.hpp>

#include <boost/preprocessor/cat.hpp>

#if !defined(YGGR_COMPILE_LINK_LIB_ONLY_USING_STATIC)
#	define YGGR_COMPILE_LINK_LIB_ONLY_USING_STATIC 0
#endif //YGGR_COMPILE_LINK_LIB_ONLY_USING_STATIC

#	define YGGR_COMPILE_TAG() -vc

#	define YGGR_COMPILER_VERSION_TAG() BOOST_PP_CAT(YGGR_COMPILE_TAG(), YGGR_MSVC_NUMBER())

#	ifdef _DEBUG
#		define YGGR_COMPILE_DEBUG_TAG() -d
#	else
#		define YGGR_COMPILE_DEBUG_TAG()
#	endif // _DEBUG

// windows dll lib name rule
// dll:				${dll_name}${compile_tag}${runtime_tag}${debug_tag}.dll
// dll-symbol-lib:	${dll_name}${compile_tag}${runtime_tag}${debug_tag}.lib
// static-lib:		lib${lib_name}${compile_tag}${runtime_tag}${debug_tag}.lib

// some static lib exception:
// lib${lib_name}_static${runtime_tag}${debug_tag}.lib
// lib${lib_name}static${runtime_tag}${debug_tag}.lib

// Other libs that do not have specific naming rules belong to raw naming

#	if defined(YGGR_MSVC_USING_MT_FLAG) && YGGR_MSVC_USING_MT_FLAG
#		define YGGR_COMPILE_RUNTIME_TAG() -s
#	else
#		define YGGR_COMPILE_RUNTIME_TAG()
#	endif // YGGR_COMPILE_USING_MT

#	if defined(_LIB)

#		define YGGR_COMPILE_LINK_IMPL_LIB( __libname__ ) 
#		define YGGR_COMPILE_LINK_IMPL_LIB_NO_DEBUG_TAG( __libname__ ) 

#		define YGGR_COMPILE_LINK_STATIC_LIB( __libname__ ) 
#		define YGGR_COMPILE_LINK_STATIC_LIB_NO_DEBUG_TAG( __libname__ ) 

#		define YGGR_COMPILE_LINK_LIB( __libname__ ) 
#		define YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG( __libname__ )

#		define YGGR_COMPILE_LINK_NRULE_LIB( __libname__ )
#		define YGGR_COMPILE_LINK_RAW_FILE( __file_name__ )

#	else

#		define YGGR_COMPILE_LINK_IMPL_LIB(__libname__) \
			(lib, YGGR_PP_CAST(YGGR_PP_IDENTITY(__libname__)YGGR_COMPILER_VERSION_TAG()YGGR_COMPILE_RUNTIME_TAG()YGGR_COMPILE_DEBUG_TAG().lib, \
								YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_IMPL_LIB_NO_DEBUG_TAG(__libname__) \
			(lib, YGGR_PP_CAST(YGGR_PP_IDENTITY(__libname__)YGGR_COMPILER_VERSION_TAG()YGGR_COMPILE_RUNTIME_TAG().lib, \
								YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_STATIC_LIB( __libname__ ) \
			(lib, YGGR_PP_CAST(BOOST_PP_CAT(lib, __libname__)YGGR_COMPILER_VERSION_TAG()YGGR_COMPILE_RUNTIME_TAG()YGGR_COMPILE_DEBUG_TAG().lib, \
								YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_STATIC_LIB_NO_DEBUG_TAG( __libname__ ) \
			(lib, YGGR_PP_CAST(BOOST_PP_CAT(lib, __libname__)YGGR_COMPILER_VERSION_TAG()YGGR_COMPILE_RUNTIME_TAG().lib, \
								YGGR_PP_CAST_TO_STRING))

#		if defined(YGGR_COMPILE_LINK_LIB_ONLY_USING_STATIC)
#			define YGGR_COMPILE_LINK_LIB YGGR_COMPILE_LINK_STATIC_LIB
#			define YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG YGGR_COMPILE_LINK_STATIC_LIB_NO_DEBUG_TAG

#			define YGGR_COMPILE_LINK_LIB_IF_ONLY_USING_STATIC( __impl_name__, __static_name__ ) YGGR_COMPILE_LINK_LIB( __static_name__ )
#			define YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG_IF_ONLY_USING_STATIC( __impl_name__, __static_name__ ) YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG( __static_name__ )

#		else
#			define YGGR_COMPILE_LINK_LIB YGGR_COMPILE_LINK_IMPL_LIB
#			define YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG YGGR_COMPILE_LINK_IMPL_LIB_NO_DEBUG_TAG

#			define YGGR_COMPILE_LINK_LIB_IF_ONLY_USING_STATIC( __impl_name__, __static_name__ ) YGGR_COMPILE_LINK_IMPL_LIB( __impl_name__ )
#			define YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG_IF_ONLY_USING_STATIC( __impl_name__, __static_name__ ) YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG_IF_ONLY_USING_STATIC( __impl_name__ )


#		endif // YGGR_COMPILE_LINK_LIB_ONLY_USING_STATIC

#		define YGGR_COMPILE_LINK_NRULE_LIB( __libname__ ) \
			(lib, YGGR_PP_CAST(__libname__.lib, YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_RAW_FILE( __file_name__ ) \
			(lib, YGGR_PP_CAST(__file_name__, YGGR_PP_CAST_TO_STRING))

#		endif // _LIB

#else

#	define YGGR_COMPILE_LINK_IMPL_LIB( __libname__ ) 
#	define YGGR_COMPILE_LINK_IMPL_LIB_NO_DEBUG_TAG( __libname__ ) 

#	define YGGR_COMPILE_LINK_STATIC_LIB( __libname__ ) 
#	define YGGR_COMPILE_LINK_STATIC_LIB_NO_DEBUG_TAG( __libname__ ) 

#	define YGGR_COMPILE_LINK_LIB( __libname__ ) 
#	define YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG( __libname__ )

#	define YGGR_COMPILE_LINK_NRULE_LIB( __libname__ )
#	define YGGR_COMPILE_LINK_RAW_FILE( __file_name__ )

#endif // _MSC_VER

#endif // __YGGR_COMPILE_LINK_MSVC_CFG_HPP__
