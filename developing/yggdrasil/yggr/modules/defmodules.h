//defmodules.h

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_MODULES_DEFMODULES_H__
#define __YGGR_MODULES_DEFMODULES_H__

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
#   include <windows.h>
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_MAC)
#   include <dlfcn.h>
#else // defined(YGGR_AT_LINUX)
#   error "append other os module support code!"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

namespace yggr
{
namespace modules
{

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
typedef HINSTANCE module_handle_type;
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_MAC)
typedef void* module_handle_type;
#else // defined(YGGR_AT_LINUX)
#   error "append other os module support code!"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

} // namespace modules
} // namespace yggr

#endif //__YGGR_MODULES_DEFMODULES_H__
