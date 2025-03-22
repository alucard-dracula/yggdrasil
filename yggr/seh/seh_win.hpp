//seh_win.hpp

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

#ifndef __YGGR_SEH_SEH_WIN_HPP__
#define __YGGR_SEH_SEH_WIN_HPP__

#include <yggr/base/yggrdef.h>

#if defined(_MSC_VER)

#ifndef YGGR_SEH_INCLUDE
#	error "seh_win.hpp include error please include seh.hpp."
#endif // YGGR_SEH_ENV_INCLUDE


#include <yggr/base/static_constant.hpp>

#include <yggr/bind/bind.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/thread/boost_thread.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/safe_container/safe_unordered_set.hpp>

#include <yggr/seh/seh_env_win.hpp>
#include <yggr/seh/seh_helper_win.hpp>

#include <boost/thread/mutex.hpp>

#include <sstream>

namespace yggr
{
namespace seh
{

class seh_win;

} // namespace seh
} // namespace yggr

namespace yggr
{
namespace seh
{
namespace detail
{

struct stack_over_flow_params
{
	const seh_win* pthis;
	u32 code;
	LPEXCEPTION_POINTERS pep;
	bool bfixed;
	HANDLE hthread;
	HANDLE hevent;
};

} // nmaespace detail
} // namespace seh
} // namespace yggr

namespace yggr
{
namespace seh
{

class seh_win
{
public:
	typedef u32 code_type;

	YGGR_STATIC_CONSTANT(u32, E_limit_stack_overflow = 20);
	YGGR_STATIC_CONSTANT(u32, E_limit_normal = 0x7fffffff);
	
private:
    typedef seh_env_win seh_env_type;
    typedef seh_helper_win seh_helper_type;
	typedef safe_container::safe_unordered_set<u32> code_set_type;
	typedef boost::mutex mutex_type;
	typedef mutex_type::scoped_lock lock_type;

public:
    typedef seh_env_type::seh_sys_codes_type seh_sys_codes_type;

private:
	typedef detail::stack_over_flow_params stack_over_flow_params_type;

private:
	typedef seh_win this_type;

public:

	seh_win(void)
	{
	}

	~seh_win(void)
	{
	}

public:
	template<typename Ret, typename Handler> inline
	bool safe_invoke(Ret& ret, const Handler& handler) const
	{
		typedef Ret ret_type;
		return 
			seh_env_type::safe_invoke(
				ret, handler,
				boost::bind(&this_type::filter, this, _1, _2),
				boost::bind(&this_type::ret_def_cleaner<ret_type>, this),
				boost::bind(&this_type::def_fixer, this));
	}

	template<typename Handler> inline
	bool safe_invoke(const Handler& handler) const
	{
		return 
			seh_env_type::safe_invoke(
				handler,
				boost::bind(&this_type::filter, this, _1, _2),
				boost::bind(&this_type::nonret_def_cleaner, this),
				boost::bind(&this_type::def_fixer, this));
	}

	inline bool register_code(code_type code)
	{
		return _set.insert(code);
	}

	inline bool unregister_code(code_type code)
	{
		return !!(_set.erase(code));
	}

private:

	template<typename Ret> inline
	Ret ret_def_cleaner(void) const
	{
		typedef Ret ret_type;
		return ret_type();
	}

	inline void nonret_def_cleaner(void) const
	{
		return;
	}

	inline bool def_fixer(void) const
	{
		//return true;
		return false;
	}

	inline static DWORD WINAPI filter_handler(LPVOID pparams)
	{
		stack_over_flow_params_type* preal_param 
			= reinterpret_cast<stack_over_flow_params_type*>(pparams);
		if(!(preal_param 
				&& preal_param->pthis 
				&& preal_param->code
				&& preal_param->pep
				&& !preal_param->bfixed
				&& preal_param->hthread
				&& preal_param->hevent))
		{
			return 0;
		}

		preal_param->bfixed = 
			preal_param->pthis->throw_error(preal_param->code, preal_param->pep, preal_param->hthread);

		::SetEvent(preal_param->hevent);

		return 0;
	}

	inline bool throw_error(code_type code, LPEXCEPTION_POINTERS pep, HANDLE hthread = 0) const
	{
		seh_helper_type::dump_call_stack_type call_stack;
		{
			lock_type lk(_mutex);
			seh_helper_type::dump_call_stack(pep, call_stack, 0, hthread, 
												code == seh_sys_codes_win::E_thread_stack_overflow?
													E_limit_stack_overflow : E_limit_normal);
		}

		char tmsg[1024] = {0};
		std::stringstream ss;

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							0, code, 0, tmsg, 1023, 0);
		ss << "os_code = " << code << ", message = " << tmsg
            << seh_helper_type::format_dump_call_stack_msg(call_stack);

		exception::exception::throw_error(code, ss.str());

		return _set.is_exists(code);
	}

	inline u32 filter(code_type code, LPEXCEPTION_POINTERS pep) const
	{
		// warning the stackoverflow only write this place, 
		// because now thread's stack is only about ~ 7080 bytes
		bool bfixed = false;
		DWORD tid = 0;
		HANDLE h = 0;
		stack_over_flow_params_type* pparam 
			= yggr_nothrow_new stack_over_flow_params_type;
		if(!pparam)
		{
			goto stackoverflow_end;
		}
		pparam->pthis = this;
		pparam->code = code;
		pparam->pep = pep;
		pparam->bfixed = false;
		pparam->hthread = GetCurrentThread();
		pparam->hevent = ::CreateEvent(0, false, false, "seh_fix_stackoverflow");
		if(!pparam->hevent)
		{
			goto stackoverflow_end;
		}

		h = ::CreateThread(0, 0, &this_type::filter_handler, reinterpret_cast<LPVOID>(pparam), 0, &tid);
		::CloseHandle(h);
		::WaitForSingleObject(pparam->hevent, INFINITE);
		::CloseHandle(pparam->hevent);

		bfixed = pparam->bfixed;

stackoverflow_end:
		if(pparam)
		{
			delete pparam;
		}

		if(bfixed)
		{
#if defined(_DEBUG)
#	if defined(YGGR_INCLUDE_SEH_DEBUG)
			return EXCEPTION_EXECUTE_HANDLER; // continue run
#	else
			return EXCEPTION_CONTINUE_SEARCH; // dump
#	endif // defined(YGGR_INCLUDE_SEH_DEBUG)
#endif // defined(_DEBUG)
		}

#ifdef _DEBUG
		return EXCEPTION_CONTINUE_SEARCH;
#else
		return EXCEPTION_EXECUTE_HANDLER;
#endif // _DEBUG
	}

private:
	code_set_type _set;
	mutable mutex_type _mutex;

};

} // namespace seh
} // namespace yggr

#endif // _MSC_VER

#endif // __YGGR_SEH_SEH_WIN_HPP__
