// yggr_cpp_ver_config.hpp

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

#ifndef __YGGR_BASE_YGGR_CPP_VER_CONFIG_HPP__
#define __YGGR_BASE_YGGR_CPP_VER_CONFIG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__


#define YGGR_CPP_VER_98 98
#define YGGR_CPP_VER_03 300
#define YGGR_CPP_VER_11 1100
#define YGGR_CPP_VER_14 1400
#define YGGR_CPP_VER_17 1700
#define YGGR_CPP_VER_20 2000
#define YGGR_CPP_VER_23 2300

#define YGGR__CPLUSPLUS_ISO_98 199711
#define YGGR__CPLUSPLUS_ISO_11 201103
#define YGGR__CPLUSPLUS_ISO_14 201402
#define YGGR__CPLUSPLUS_ISO_17 201703
#define YGGR__CPLUSPLUS_ISO_20 202002

#if defined(_MSC_VER)
#	if defined(_MSVC_LANG)
#		define YGGR__CPLUSPLUS _MSVC_LANG
#	else
#		define YGGR__CPLUSPLUS 200300 // vs2010 support 03 
#	endif // YGGR__CPLUSPLUS
#else
#	define YGGR__CPLUSPLUS __cplusplus
#endif // 

#if !defined(YGGR_CPP_VER)
#	if (YGGR__CPLUSPLUS < 200300)
#		define YGGR_CPP_VER YGGR_CPP_VER_98
#	elif (YGGR__CPLUSPLUS < 201100)
#		define YGGR_CPP_VER YGGR_CPP_VER_03
#	elif (YGGR__CPLUSPLUS < 201400)
#		define YGGR_CPP_VER YGGR_CPP_VER_11
#	elif (YGGR__CPLUSPLUS < 201700)
#		define YGGR_CPP_VER YGGR_CPP_VER_14
#	elif (YGGR__CPLUSPLUS < 202000)
#		define YGGR_CPP_VER YGGR_CPP_VER_17
#	elif (YGGR__CPLUSPLUS < 202300)
#		define YGGR_CPP_VER YGGR_CPP_VER_20
#	elif (YGGR__CPLUSPLUS < 202600)
#		define YGGR_CPP_VER YGGR_CPP_VER_23
#	else
#		error "please define YGGR_CPP_VERSION or expand this macro"
#	endif // __cplusplus < 201402
#endif // YGGR_CPP_VERSION

#define YGGR_CPP_VERSION YGGR_CPP_VER

#if !(defined(YGGR_USING_CXX98) \
		|| defined(YGGR_USING_CXX03) \
		|| defined(YGGR_USING_CXX11) \
		|| defined(YGGR_USING_CXX14) \
		|| defined(YGGR_USING_CXX17) \
		|| defined(YGGR_USING_CXX20) \
		|| defined(YGGR_USING_CXX23) )
#	if (YGGR_CPP_VERSION == YGGR_CPP_VER_98)
#		define YGGR_USING_CXX98
#	elif(YGGR_CPP_VERSION == YGGR_CPP_VER_03)
#		define YGGR_USING_CXX03
#	elif(YGGR_CPP_VERSION == YGGR_CPP_VER_11)
#		define YGGR_USING_CXX11
#	elif(YGGR_CPP_VERSION == YGGR_CPP_VER_14)
#		define YGGR_USING_CXX14
#	elif(YGGR_CPP_VERSION == YGGR_CPP_VER_17)
#		define YGGR_USING_CXX17
#	elif(YGGR_CPP_VERSION == YGGR_CPP_VER_20)
#		define YGGR_USING_CXX20
#	elif(YGGR_CPP_VERSION == YGGR_CPP_VER_23)
#		define YGGR_USING_CXX23
#	else
		// ......
#	endif // YGGR_CPP_VERSION == YGGR_CPP_VER_11
#else
#	if defined(YGGR_USING_CXX98)
#		undef YGGR_CPP_VERSION
#		define YGGR_CPP_VERSION YGGR_CPP_VER_98
#	elif defined(YGGR_USING_CXX03)
#		undef YGGR_CPP_VERSION
#		define YGGR_CPP_VERSION YGGR_CPP_VER_03
#	elif defined(YGGR_USING_CXX11)
#		undef YGGR_CPP_VERSION
#		define YGGR_CPP_VERSION YGGR_CPP_VER_11
#	elif defined(YGGR_USING_CXX14)
#		undef YGGR_CPP_VERSION
#		define YGGR_CPP_VERSION YGGR_CPP_VER_14
#	elif defined(YGGR_USING_CXX17)
#		undef YGGR_CPP_VERSION
#		define YGGR_CPP_VERSION YGGR_CPP_VER_17
#	elif defined(YGGR_USING_CXX20)
#		undef YGGR_CPP_VERSION
#		define YGGR_CPP_VERSION YGGR_CPP_VER_20
#	elif defined(YGGR_USING_CXX23)
#		undef YGGR_CPP_VERSION
#		define YGGR_CPP_VERSION YGGR_CPP_VER_23
#	else
	// .....
#	endif // YGGR_CPP_VER_98
#endif // YGGR_USING_CXX11

#endif // __YGGR_BASE_YGGR_CPP_VER_CONFIG_HPP__
