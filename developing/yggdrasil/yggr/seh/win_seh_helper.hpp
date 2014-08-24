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

#include <yggr/base/yggrdef.h>
#include <list>
#include <yggr/charset/string.hpp>
#include <sstream>


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
	static WORD get_this_machine_type(void)
	{
		HMODULE hmod = GetModuleHandle(0);

		IMAGE_DOS_HEADER* pdos_header = (IMAGE_DOS_HEADER*)hmod;
		IMAGE_FILE_HEADER* p = (IMAGE_FILE_HEADER*)(((u8*)pdos_header) + pdos_header->e_lfanew + sizeof(DWORD));

		return p->Machine;
	}

	static const std::string format_dump_call_stack_msg(const dump_call_stack_type& call_stack)
	{
		typedef dump_call_stack_type::const_iterator citer_type;

		std::stringstream ss;
		size_type line = 0;
		for(citer_type i = call_stack.begin(), isize = call_stack.end(); i != isize; ++i, ++line)
		{
			for(size_type j = 0, jsize = line; j != jsize; ++j)
			{
				ss << "\t";
			}

			ss << *i << "\n";
		}

		return ss.str();
	}

	static bool dump_call_stack(LPEXCEPTION_POINTERS ex_ptr, dump_call_stack_type& call_stack)
	{
		HANDLE h = GetCurrentProcess();
		if(!SymInitialize(h, 0, true))
		{
			return false;
		}

		handler_dump_call_stack(ex_ptr->ContextRecord, call_stack);

		if(!SymCleanup(h))
		{
			return false;
		}

		return true;
	}

private:
	static void handler_dump_call_stack(PCONTEXT context_ptr, dump_call_stack_type& call_stack)
	{
		call_stack.clear();
		STACKFRAME sf = {0};

#if defined(CONTEXT_i386) || defined(CONTEXT_i486)
		sf.AddrPC.Offset = context_ptr->Eip;
		sf.AddrPC.Mode = AddrModeFlat;
		sf.AddrStack.Offset = context_ptr->Esp;
		sf.AddrStack.Mode = AddrModeFlat;
		sf.AddrFrame.Offset = context_ptr->Ebp;
		sf.AddrFrame.Mode = AddrModeFlat;
#elif defined(CONTEXT_AMD64)
		// don't need operator
#else
#	error "please test it and mend this place"
#endif //defined(CONTEXT_i386) || defined(CONTEXT_i486)
		WORD machine_code = get_this_machine_type();

		HANDLE hprocess = GetCurrentProcess();
		HANDLE hthread = GetCurrentThread();

		SYMBOL_INFO_PACKAGE symbol_info = {0};
		PSYMBOL_INFO symbol_ptr = &symbol_info.si;
		symbol_ptr->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol_ptr->MaxNameLen = 1024;
		DWORD64 sym_disp = 0;

		IMAGEHLP_LINE line_info = {sizeof(IMAGEHLP_LINE)};
		DWORD line_disp = 0;

		char tmp_msg[1024] = {0};
		DWORD os_code = 0;
		for(;StackWalk(machine_code, hprocess, hthread, &sf, context_ptr, 0, SymFunctionTableAccess, SymGetModuleBase, 0)
				&& sf.AddrFrame.Offset;)
		{
			std::stringstream ss;

			if(SymFromAddr(hprocess, sf.AddrPC.Offset, 0, symbol_ptr))
			{
				ss << "[ foo_name = " << symbol_ptr->Name << " ] ";
			}
			else
			{
				os_code = GetLastError();
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
									0, os_code, 0, tmp_msg, 1023, 0);
				ss << "[ get foo name err addr = " << sf.AddrPC.Offset
					<< ", os_code = " << os_code
					<< ", message = " << tmp_msg
					<< " ] ";
			}

			if(SymGetLineFromAddr(hprocess, sf.AddrPC.Offset, &line_disp, &line_info ))
			{
				ss << "[ source file : " << line_info.FileName
					<<", sourec line : " << line_info.LineNumber << " ] ";
			}
			else
			{
				os_code = GetLastError();
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
									0, os_code, 0, tmp_msg, 1023, 0);
				ss << "[ get foo file info err addr = " << sf.AddrPC.Offset
					<< ", os_code = " << os_code
					<< ", message = " << tmp_msg
					<< " ] ";
			}

			call_stack.push_back(ss.str());
		}

		call_stack.reverse();
	}

private:
	/*
	*	this foo must at .hpp file only check seh environment
	*/
#ifdef YGGR_USE_SEH
	static void check_seh_env_eha_set(void)
	{
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

