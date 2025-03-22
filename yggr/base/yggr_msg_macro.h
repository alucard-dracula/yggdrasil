// yggr_msg_macro.h

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

#ifndef __YGGR_MSG_MACRO_H__
#define __YGGR_MSG_MACRO_H__

#include <yggr/base/yggrdef.h>

#ifdef _DEBUG
#	include <iostream>
#	include <string>
#endif // _DEBUG

#ifdef _DEBUG

#	define __YGGR_MSG_PCRE_WARNING( __msg__ ) \
	std::cerr << "!!!!!PCRE WARNING:" \
				<< ( __msg__ ) << "!!!!!!" << std::endl;

#	define __YGGR_MSG_EXCEPTION( __msg__ ) \
	std::cerr << "!!!!!EXCEPTION MSG:" \
				<< ( __msg__ ) << "!!!!!!" << std::endl;

#else

#	define __YGGR_MSG_PCRE_WARNING( __msg__ )

#	define __YGGR_MSG_EXCEPTION( __msg__ )

#endif // _DEBUG

#define YGGR_MSG_PCRE_WARNING __YGGR_MSG_PCRE_WARNING

namespace yggr
{
} // namespace yggr

#endif // __YGGR_MSG_MACRO_H__
