//seh_sys_codes_darwin.hpp

/****************************************************************************
Copyright (c) 2025-2025 yggdrasil

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

#ifndef __YGGR_SEH_SEH_SYS_CODES_DARWIN_HPP__
#define __YGGR_SEH_SEH_SYS_CODES_DARWIN_HPP__

#ifndef YGGR_SEH_INCLUDE
#	error "seh_sys_codes_darwin.hpp please include seh.hpp."
#endif //YGGR_SEH_INCLUDE

#include <yggr/base/yggrdef.h>
#include <csignal>

namespace yggr
{
namespace seh
{

class seh_sys_codes_darwin
{
public:
    enum
    {
        E_SIGILL = SIGILL,  //stack overflow
        E_SIGABRT = SIGABRT, // about error
        E_SIGBUS = SIGBUS, //aliment err
        E_SIGFPE = SIGFPE, // div 0 or div 0.0
        E_SIGSEGV = SIGSEGV, // windows 0xc0000005
        E_SIGSYS = SIGSYS, // ill sys call
        E_SIGIOT = SIGIOT,
        //E_SIGQUIT = SIGQUIT,
        //E_SIGTRAP = SIGTRAP, // trap debug use
        E_SIGXCPU = SIGXCPU, // cpu time limit
        E_SIGXFSZ = SIGXFSZ, // file size limit

        E_unknown = 0xffffffff,

        E_compile_u32 = 0xffffffff
    };
};

} // namespace seh
} // namespace yggr

#endif // __YGGR_SEH_SEH_SYS_CODES_DARWIN_HPP__
