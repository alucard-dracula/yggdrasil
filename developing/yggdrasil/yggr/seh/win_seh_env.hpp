//win_seh_env.hpp

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

#ifndef __YGGR_SEH_WINDOWS_SEH_ENV_HPP__
#define __YGGR_SEH_WINDOWS_SEH_ENV_HPP__

#ifndef YGGR_SEH_INCLUDE
#	error "win_seh_env.hpp include error please include seh.hpp."
#endif // YGGR_SEH_ENV_INCLUDE

#include <yggr/base/yggrdef.h>

#include <windows.h>
#include <excpt.h>
#include <yggr/seh/win_seh_sys_codes.hpp>

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(error:4535)
#endif // _MSC_VER

namespace yggr
{
namespace seh
{

class win_seh_env
{

public:
    typedef win_seh_sys_codes seh_sys_codes_type;

public:
	template<typename Ret, typename Handler, typename Filter_Handler, typename Clean_Handler, typename FixHandler>
	static bool safe_invoke(	Ret& ret,
								const Handler& handler,
								const Filter_Handler& filter,
								const Clean_Handler& cleaner,
								const FixHandler& fixer)
	{
		__try
		{
			__try
			{
				ret = handler();
				return true;
			}
			__finally
			{
				ret = cleaner();
			}
		}
		__except(filter(::GetExceptionCode(), ::GetExceptionInformation()))
		{
			return fixer();
		}
	}

	template<typename Handler, typename Filter_Handler, typename Clean_Handler, typename FixHandler>
	static bool safe_invoke(	const Handler& handler,
								const Filter_Handler& filter,
								const Clean_Handler& cleaner,
								const FixHandler& fixer)
	{
		__try
		{
			__try
			{
				handler();
				return true;
			}
			__finally
			{
				cleaner();
			}
		}
		__except(filter(GetExceptionCode(), GetExceptionInformation()))
		{
			return fixer();
		}
	}
};

} // namesapce seh
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning(pop)
#endif // _MSC_VER

#endif // __YGGR_SEH_WINDOWS_SEH_ENV_HPP__
