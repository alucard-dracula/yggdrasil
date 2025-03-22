//defmodules.h

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

#ifndef __YGGR_MODULES_DEFMODULES_H__
#define __YGGR_MODULES_DEFMODULES_H__

#include <yggr/base/yggrdef.h>

#if defined(YGGR_AT_WINDOWS)
#	include <yggr/network/socket_conflict_fixer.hpp>
#   include <windows.h>
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)
#   include <dlfcn.h>
#else // YGGR_AT_LINUX || YGGR_AT_DARWIN
#   error "append other os module support code!"
#endif // YGGR_AT_WINDOWS

namespace yggr
{
namespace modules
{

#if defined(YGGR_AT_WINDOWS)
typedef HINSTANCE module_handle_type;
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)
typedef void* module_handle_type;
#else // YGGR_AT_LINUX || YGGR_AT_DARWIN
#   error "append other os module support code!"
#endif // YGGR_AT_WINDOWS

} // namespace modules
} // namespace yggr

#endif //__YGGR_MODULES_DEFMODULES_H__
