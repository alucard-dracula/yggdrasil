// yggr_c_ver_config.hpp

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

#ifndef __YGGR_BASE_YGGR_C_VER_CONFIG_HPP__
#define __YGGR_BASE_YGGR_C_VER_CONFIG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__


#define YGGR_C_VER_89 89
#define YGGR_C_VER_95 95
#define YGGR_C_VER_99 99
#define YGGR_C_VER_11 1100
#define YGGR_C_VER_17 1700
#define YGGR_C_VER_18 1800
#define YGGR_C_VER_23 2300

#define YGGR__C_ISO_89 199409
#define YGGR__C_ISO_95 199409
#define YGGR__C_ISO_99 199901
#define YGGR__C_ISO_11 201112
#define YGGR__C_ISO_17 201710
#define YGGR__C_ISO_18 201805
#define YGGR__C_ISO_23 202002

#if defined(__STDC_VERSION__)

#	define YGGR___STDC_VERSION__ __STDC_VERSION__

#if !defined(YGGR_C_VER)
#	if (YGGR___STDC_VERSION__ < YGGR__C_ISO_99)
#		define YGGR_C_VER YGGR_C_VER_95
#	elif (YGGR___STDC_VERSION__ < YGGR__C_ISO_11)
#		define YGGR_C_VER YGGR_C_VER_99
#	elif (YGGR___STDC_VERSION__ < YGGR__C_ISO_17)
#		define YGGR_C_VER YGGR_C_VER_11
#	elif (YGGR___STDC_VERSION__ < YGGR__C_ISO_18)
#		define YGGR_C_VER YGGR_C_VER_17
#	elif (YGGR___STDC_VERSION__ < YGGR__C_ISO_23)
#		define YGGR_C_VER YGGR_C_VER_18
#	else
#		define YGGR_C_VER YGGR__C_ISO_23
#	endif // __cplusplus < 201402
#else
#	define YGGR_C_VER YGGR_C_VER_95
#endif // YGGR_CPP_VERSION

#endif // __STDC_VERSION__

#define YGGR_C_VERSION YGGR_C_VER

#if !(defined(YGGR_USING_C95) \
		|| defined(YGGR_USING_C99) \
		|| defined(YGGR_USING_C11) \
		|| defined(YGGR_USING_C17) \
		|| defined(YGGR_USING_C18) \
		|| defined(YGGR_USING_C23) )
#	if (YGGR_C_VERSION == YGGR_C_VER_95)
#		define YGGR_USING_C95
#	elif(YGGR_C_VERSION == YGGR_C_VER_99)
#		define YGGR_USING_C99
#	elif(YGGR_C_VERSION == YGGR_C_VER_11)
#		define YGGR_USING_C11
#	elif(YGGR_C_VERSION == YGGR_C_VER_17)
#		define YGGR_USING_C17
#	elif(YGGR_C_VERSION == YGGR_C_VER_18)
#		define YGGR_USING_C18
#	elif(YGGR_C_VERSION == YGGR_C_VER_23)
#		define YGGR_USING_C23
#	else
		// ......
#	endif // YGGR_USING_C95 ||
#else
#	if defined(YGGR_USING_C95)
#		undef YGGR_C_VERSION
#		define YGGR_C_VERSION YGGR_C_VER_95
#	elif defined(YGGR_USING_C99)
#		undef YGGR_C_VERSION
#		define YGGR_C_VERSION YGGR_C_VER_99
#	elif defined(YGGR_USING_C11)
#		undef YGGR_C_VERSION
#		define YGGR_C_VERSION YGGR_C_VER_11
#	elif defined(YGGR_USING_C17)
#		undef YGGR_C_VERSION
#		define YGGR_C_VERSION YGGR_C_VER_17
#	elif defined(YGGR_USING_C18)
#		undef YGGR_C_VERSION
#		define YGGR_C_VERSION YGGR_C_VER_18
#	elif defined(YGGR_USING_C23)
#		undef YGGR_C_VERSION
#		define YGGR_C_VERSION YGGR_C_VER_23
#	else
	// .....
#	endif // YGGR_USING_C95
#endif // YGGR_USING_C95 ||

#endif // __YGGR_BASE_YGGR_C_VER_CONFIG_HPP__
