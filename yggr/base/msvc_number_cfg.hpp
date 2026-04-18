// msvc_number_cfg.hpp

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

#ifndef __YGGR_BASE_MSVC_NUMBER_CFG_HPP__
#define __YGGR_BASE_MSVC_NUMBER_CFG_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__


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

#if defined(_MSC_VER)
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
#endif // _MSC_VER

#endif // __YGGR_BASE_MSVC_NUMBER_CFG_HPP__
