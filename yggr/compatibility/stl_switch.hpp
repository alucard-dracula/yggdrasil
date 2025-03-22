//stl_switch.hpp

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

#ifndef __YGGR_COMPATIBILITY_STL_SWITCH_HPP__
#define __YGGR_COMPATIBILITY_STL_SWITCH_HPP__

#define YGGR_USE_MICROSOFT_STL 1
#define YGGR_USE_GNUC_STL 2
#define YGGR_USE_CLANG_STL 3

#define YGGR_USE_SGI_STL 100 //it not tested

#if !defined(YGGR_USE_STL_VER) || (YGGR_USE_STL_VER < 0) || (YGGR_USE_STL_VER > 3)
#	if defined(_MSC_VER)
#		define YGGR_USE_STL_VER YGGR_USE_MICROSOFT_STL
#	elif defined (__GNUC__)
#		define YGGR_USE_STL_VER YGGR_USE_GNUC_STL
#	elif defined(__APPLE__) || defined(__APPLE_CC__)
#		define YGGR_USE_STL_VER YGGR_USE_CLANG_STL
#	else
#		define YGGR_USE_STL_VER 0
#		error "add now use stl version"
#	endif // _MSC_VER
#endif // YGGR_USE_STL_VER

#endif //__YGGR_COMPATIBILITY_STL_SWITCH_HPP__