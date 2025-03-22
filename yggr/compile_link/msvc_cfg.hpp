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

#ifdef _MSC_VER

//MSVC++ 14.0 _MSC_VER == 1924 (Visual Studio 2019)
//MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
//MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
//MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
//MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
//MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
//MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
//MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio .NET 2003)
//MSVC++ 7.0  _MSC_VER == 1300
//MSVC++ 6.0  _MSC_VER == 1200
//MSVC++ 5.0  _MSC_VER == 1100
//MSVC++ 4.0  _MSC_VER == 1000
//MSVC++ 2.0  _MSC_VER ==  900
//MSVC++ 1.0  _MSC_VER ==  800
//C/C++  7.0  _MSC_VER ==  700
//C      6.0  _MSC_VER ==  600

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>
#include <yggr/ppex/cast_to_identity.hpp>

#include <boost/preprocessor/cat.hpp>

#	if _MSC_VER > 1200
#		if _MSC_VER == 1300
#			define YGGR_MSVC_NUMBER() 70
#		elif _MSC_VER == 1310
#			define YGGR_MSVC_NUMBER() 71
#		elif _MSC_VER == 1400
#			define YGGR_MSVC_NUMBER() 80
#		elif _MSC_VER == 1500
#			define YGGR_MSVC_NUMBER() 90
#		elif _MSC_VER == 1600
#			define YGGR_MSVC_NUMBER() 100
#		elif _MSC_VER == 1700
#			define YGGR_MSVC_NUMBER() 110
#		elif _MSC_VER == 1800
#			define YGGR_MSVC_NUMBER() 120
#		elif _MSC_VER == 1900
#			define YGGR_MSVC_NUMBER() 140
#		elif _MSC_VER == 1910
#			define YGGR_MSVC_NUMBER() 141
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			define YGGR_MSVC_NUMBER() 142
#		elif 1930 <= _MSC_VER && _MSC_VER < 2000
#			define YGGR_MSVC_NUMBER() 143
#		else
#			error "!!!!!unknown _MSC_VER!!!!!"
#		endif // _MSC_VER
#	else
#		error "!!!!!yggdrasil not support msvc6 and before!!!!!"
#	endif //_MSC_VER

#	define YGGR_COMPILE_TAG() -vc

#	define YGGR_COMPILER_VERSION_TAG() BOOST_PP_CAT(YGGR_COMPILE_TAG(), YGGR_MSVC_NUMBER())

#	ifdef _DEBUG
#		define YGGR_COMPILE_DEBUG_TAG() -d
#	else
#		define YGGR_COMPILE_DEBUG_TAG()
#	endif // _DEBUG

#	define YGGR_COMPILE_STATIC_TAG() -s

#	ifdef _LIB

#		define YGGR_COMPILE_LINK_LIB( __libname__ )
#		define YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG( __libname__ ) 
#		define YGGR_COMPILE_LINK_LIB_NO_FIRST_LIB_TAG( __libname__ )
#		define YGGR_COMPILE_LINK_LIB_NO_FIRST_LIB_TAG_AND_DEBUG_TAG( __libname__ )

#		define YGGR_COMPILE_LINK_STATIC_LIB( __libname__ ) 
#		define YGGR_COMPILE_LINK_STATIC_LIB_NO_DEBUG_TAG( __libname__ ) 

#		define YGGR_COMPILE_LINK_TP_LIB( __libname__ )
#		define YGGR_COMPILE_LINK_NRULE_LIB( __libname__ )

#	else

#		define YGGR_COMPILE_LINK_LIB( __libname__ ) \
			(lib, YGGR_PP_CAST(BOOST_PP_CAT(lib, __libname__)YGGR_COMPILER_VERSION_TAG()YGGR_COMPILE_DEBUG_TAG().lib, \
								YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_LIB_NO_FIRST_LIB_TAG( __libname__ ) \
			(lib, YGGR_PP_CAST(YGGR_PP_IDENTITY(__libname__)YGGR_COMPILER_VERSION_TAG()YGGR_COMPILE_DEBUG_TAG().lib, \
								YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_LIB_NO_FIRST_LIB_TAG_AND_DEBUG_TAG( __libname__ ) \
			(lib, YGGR_PP_CAST(YGGR_PP_IDENTITY(__libname__)YGGR_COMPILER_VERSION_TAG().lib, \
								YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG( __libname__ ) \
			(lib, YGGR_PP_CAST(BOOST_PP_CAT(lib, __libname__)YGGR_COMPILER_VERSION_TAG().lib, \
								YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_STATIC_LIB( __libname__ ) \
			(lib, YGGR_PP_CAST(BOOST_PP_CAT(lib, __libname__)YGGR_COMPILER_VERSION_TAG()YGGR_COMPILE_STATIC_TAG()YGGR_COMPILE_DEBUG_TAG().lib, \
								YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_STATIC_LIB_NO_DEBUG_TAG( __libname__ ) \
			(lib, YGGR_PP_CAST(BOOST_PP_CAT(lib, __libname__)YGGR_COMPILER_VERSION_TAG()YGGR_COMPILE_STATIC_TAG().lib, \
								YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_TP_LIB( __libname__ ) \
			(lib, YGGR_PP_CAST(BOOST_PP_CAT(lib, __libname__).lib, YGGR_PP_CAST_TO_STRING))

#		define YGGR_COMPILE_LINK_NRULE_LIB( __libname__ ) \
			(lib, YGGR_PP_CAST(__libname__.lib, YGGR_PP_CAST_TO_STRING))

#		endif // _LIB

#else

#	define YGGR_COMPILE_LINK_LIB( __libname__ ) 
#	define YGGR_COMPILE_LINK_STATIC_LIB( __libname__ ) 
#	define YGGR_COMPILE_LINK_LIB_NO_DEBUG_TAG( __libname__ ) 
#	define YGGR_COMPILE_LINK_STATIC_LIB_NO_DEBUG_TAG( __libname__ ) 
#	define YGGR_COMPILE_LINK_TP_LIB( __libname__ )
#	define YGGR_COMPILE_LINK_NRULE_LIB( __libname__ )

#endif // _MSC_VER

#endif // __YGGR_COMPILE_LINK_MSVC_CFG_HPP__
