//win_seh.hpp

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

#ifndef __YGGR_SEH_WIN_SEH_HPP__
#define __YGGR_SEH_WIN_SEH_HPP__

#ifndef YGGR_SEH_INCLUDE
#	error "win_seh.hpp include error please include seh.hpp."
#endif // YGGR_SEH_ENV_INCLUDE

#include <yggr/base/yggrdef.h>
#include <yggr/safe_container/safe_unordered_set.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <yggr/base/exception.hpp>
#include <sstream>

#include <yggr/seh/win_seh_env.hpp>
#include <yggr/seh/win_seh_helper.hpp>

namespace yggr
{
namespace seh
{

class win_seh
{
public:
	typedef u32 code_type;


private:
    typedef win_seh_env seh_env_type;
    typedef win_seh_helper seh_helper_type;
	typedef safe_container::safe_unordered_set<u32> code_set_type;
	typedef boost::mutex mutex_type;
	typedef mutex_type::scoped_lock lock_type;

public:
    typedef seh_env_type::seh_sys_codes_type seh_sys_codes_type;

private:
	typedef win_seh this_type;

public:

	win_seh(void);
	~win_seh(void);

	template<typename Ret, typename Handler>
	bool safe_invoke(Ret& ret, const Handler& handler) const
	{
		typedef Ret ret_type;
		return seh_env_type::safe_invoke(ret, handler,
											boost::bind(&this_type::filter, this, _1, _2),
											boost::bind(&this_type::ret_def_cleaner<ret_type>, this),
											boost::bind(&this_type::def_fixer, this));
	}

	template<typename Handler>
	bool safe_invoke(const Handler& handler) const
	{
		return seh_env_type::safe_invoke(handler,
											boost::bind(&this_type::filter, this, _1, _2),
											boost::bind(&this_type::nonret_def_cleaner, this),
											boost::bind(&this_type::def_fixer, this));
	}

	bool register_code(code_type code);
	bool unregister_code(code_type code);

private:

	template<typename Ret>
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

	inline u32 filter(code_type code, LPEXCEPTION_POINTERS pep) const
	{
		seh_helper_type::dump_call_stack_type call_stack;
		{
			lock_type lk(_mutex);
			seh_helper_type::dump_call_stack(pep, call_stack);
		}

		char tmsg[1024] = {0};
		std::stringstream ss;

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							0, code, 0, tmsg, 1023, 0);
		ss << "os_code = " << code << ", message = " << tmsg
            << seh_helper_type::format_dump_call_stack_msg(call_stack);

		exception::exception::throw_error(code, ss.str());


		if(_set.is_exists(code))
		{
#if defined(_DEBUG)
#	if defined(YGGR_INCLUDE_SEH_DEBUG)
			return EXCEPTION_EXECUTE_HANDLER;
#	else
			return EXCEPTION_CONTINUE_SEARCH;
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

#endif // __YGGR_SEH_WIN_SEH_HPP__
