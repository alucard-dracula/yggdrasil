//seh_helper_android.hpp

/****************************************************************************
Copyright (c) 2010-2022 yggdrasil

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

#ifndef __YGGR_SEH_SEH_HELPER_ANDROID_HPP__
#define __YGGR_SEH_SEH_HELPER_ANDROID_HPP__

#ifndef YGGR_SEH_INCLUDE
#	error "seh_helper_android.hpp include error please include seh.hpp."
#endif // YGGR_SEH_ENV_INCLUDE

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/container/list.hpp>

#include <sstream>

#include <execinfo.h>

namespace yggr
{
namespace seh
{

class seh_helper_android
{
public:
	typedef ::yggr::list<std::string> dump_call_stack_type;

public:
    static std::string format_dump_call_stack_msg(const dump_call_stack_type& call_stack);
	static bool dump_call_stack(dump_call_stack_type& call_stack);
};

} // namespace seh
} // namespace yggr

#endif // __YGGR_SEH_SEH_HELPER_ANDROID_HPP__
