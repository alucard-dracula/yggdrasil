//seh_env_win.hpp

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

#ifndef __YGGR_SEH_SEH_ENV_WIN_HPP__
#define __YGGR_SEH_SEH_ENV_WIN_HPP__

#include <yggr/base/yggrdef.h>

#if defined(_MSC_VER)

#ifndef YGGR_SEH_INCLUDE
#	error "seh_env_win.hpp include error please include seh.hpp."
#endif // YGGR_SEH_ENV_INCLUDE


#include <yggr/network/socket_conflict_fixer.hpp>
#include <windows.h>

#include <yggr/seh/seh_sys_codes_win.hpp>

#include <cassert>

#include <excpt.h>

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(error:4535)
#endif // _MSC_VER

namespace yggr
{
namespace seh
{

class seh_env_win
{

public:
    typedef seh_sys_codes_win seh_sys_codes_type;

public:
	template<typename Ret, typename Handler, typename Filter_Handler, typename Clean_Handler, typename FixHandler>
	static bool safe_invoke(	Ret& ret,
								const Handler& handler,
								const Filter_Handler& filter,
								const Clean_Handler& cleaner,
								const FixHandler& fixer)
	{
		bool bright = false;
		__try
		{
			__try
			{
				ret = handler();
				bright = true;
				__leave;
			}
			__finally
			{
				ret = cleaner();
			}
		}
		__except(filter(GetExceptionCode(), GetExceptionInformation()))
		{
			return fixer();
		}

		return bright;
	}

	template<typename Handler, typename Filter_Handler, typename Clean_Handler, typename FixHandler>
	static bool safe_invoke(	const Handler& handler,
								const Filter_Handler& filter,
								const Clean_Handler& cleaner,
								const FixHandler& fixer)
	{
		bool bright = false;
		__try
		{
			__try
			{
				handler();
				bright = true;
				__leave;
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

		return bright;
	}
};

} // namesapce seh
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning(pop)
#endif // _MSC_VER

#endif // _MSC_VER

#endif // __YGGR_SEH_SEH_ENV_WIN_HPP__
