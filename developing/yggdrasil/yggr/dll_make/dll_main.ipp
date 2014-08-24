//dll_main.ipp

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

#ifndef __YGGR_DLL_MAKE_DLL_MAIN_IPP__
#define __YGGR_DLL_MAKE_DLL_MAIN_IPP__

#ifdef _DEBUG
#	include <iostream>
#endif // _DEBUG

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)

#include <windows.h>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
#ifdef _DEBUG
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			std::cout << "process attach" << std::endl;
			break;
		case DLL_THREAD_ATTACH:
			std::cout << "thread attach" << std::endl;
			break;
		case DLL_THREAD_DETACH:
			std::cout << "thread detach" << std::endl;
			break;
		case DLL_PROCESS_DETACH:
			std::cout << "process detach" << std::endl;
			break;
	}
#else
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
#endif // _DEBUG
	return TRUE;

}

#elif defined(YGGR_AT_LINUX)
//	don't need def
#else // defined(YGGR_AT_LINUX)
#	error "dll_main.ipp only support windows dll and linux so, plase add your dll main code!!"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

#endif //__YGGR_DLL_MAKE_DLL_MAIN_IPP__
