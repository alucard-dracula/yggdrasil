//win_seh_helper.hpp

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

#ifndef __YGGR_SEH_WIN_SEH_HELPER_HPP__
#define __YGGR_SEH_WIN_SEH_HELPER_HPP__

#ifndef _MSC_VER
#	error "win_seh_helper.hpp must use on cl.exe"
#endif // _MSC_VER

#ifndef YGGR_SEH_INCLUDE
#	error "win_seh.hpp include error please include seh.hpp."
#endif // YGGR_SEH_ENV_INCLUDE

#include <windows.h>
#include <dbghelp.h>
#include <Tlhelp32.h>

#include <cassert>
#include <list>
#include <sstream>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>

#pragma comment(lib, "dbghelp.lib")

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(error:4535)
#endif // _MSC_VER

namespace yggr
{
namespace seh
{

class win_seh_helper
{

public:
	typedef std::list<std::string> dump_call_stack_type;

public:
	static WORD get_this_machine_type(void);

	static const std::string format_dump_call_stack_msg(const dump_call_stack_type& call_stack);

	static bool dump_call_stack(LPEXCEPTION_POINTERS ex_ptr, dump_call_stack_type& call_stack);

private:
	static void handler_dump_call_stack(PCONTEXT context_ptr, dump_call_stack_type& call_stack);

private:
	/*
	*	this foo must at .hpp file only check seh environment
	*/
#ifdef YGGR_USE_SEH
	inline static void check_seh_env_eha_set(void)
	{
		assert(false);
		_set_se_translator(0);
	}
#endif // YGGR_USE_SEH
};

} // namespace seh
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning(pop)
#endif // _MSC_VER

#endif // __YGGR_SEH_WIN_SEH_HELPER_HPP__

