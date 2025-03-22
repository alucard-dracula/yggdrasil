//dll_mark_define.hpp

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

#ifndef __YGGR_DLL_MARK_DEFINE_HPP__
#define __YGGR_DLL_MARK_DEFINE_HPP__

#include <yggr/base/yggrdef.h>

#if	defined(WIN32) || defined(WIN64) || defined(WINDOWS)
#	ifdef YGGR_DLL_DEF
#		define YGGR_DLL_API __declspec(dllexport)
#	else
#		define YGGR_DLL_API __declspec(dllimport)
#	endif //YGGR_DLL_DEF
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)
#	if defined(__GNUC__) && __GNUC__ >= 4
#		define YGGR_DLL_API __attribute__ ((visibility("default")))
#	else
#		define YGGR_DLL_API
#	endif //__GNUC__
# else // defined(YGGR_AT_LINUX)
#	error "dll_mark_define.hpp now only support dll and so, please append you export code!!"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

#ifndef YGGR_MODULE_CALL_T
#	define YGGR_MODULE_CALL_T()
#endif  //YGGR_MODULE_CALL_T

namespace yggr
{
namespace dll_make
{
} // namespace dll_make
} // namespace yggr

#endif //__YGGR_DLL_MARK_DEFINE_HPP__
