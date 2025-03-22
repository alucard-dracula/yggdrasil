//seh.hpp

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

#ifndef __YGGR_SEH_SEH_HPP__
#define __YGGR_SEH_SEH_HPP__

#include <yggr/base/yggrdef.h>

#if defined(_MSC_VER) || defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)

#if defined(YGGR_USE_SEH)

#if defined(__MINGW32__) || defined(__CYGWIN__)
#   error "seh must use to system compiler"
#endif // defined(__MINGW32__) || defined(__CYGWIN__)

#include <yggr/base/ptr_single.hpp>

#if !defined(YGGR_SEH_INCLUDE)
#	define YGGR_SEH_INCLUDE
#endif // YGGR_SEH_INCLUDE

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
#   include <yggr/seh/seh_win.hpp>
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_ANDROID)
#	if  defined(YGGR_AT_ANDROID)
#		include <yggr/seh/seh_android.hpp>
#	else
#		include <yggr/seh/seh_linux.hpp>
#	endif // defined(YGGR_AT_ANDROID)
#elif defined(YGGR_AT_DARWIN)
#	include <yggr/seh/seh_darwin.hpp>
#else
#   error "please add now platform seh code !!!"
#endif //defined(WIN32) || defined(WIN64) || defined(WINDOWS)

namespace yggr
{
namespace seh
{

template<typename Base>
class seh : public Base
{
private:
	typedef Base base_type;

public:
    typedef typename base_type::seh_sys_codes_type seh_sys_codes_type;
	typedef typename base_type::code_type code_type;

private:
	typedef seh this_type;
	typedef ptr_single<this_type> this_single_type;

public:
	typedef typename this_single_type::obj_ptr_type obj_ptr_type;
	typedef const obj_ptr_type obj_cptr_type;

public:
	seh(void)
	{
	}

	~seh(void)
	{
	}

public:
	inline static void init_ins(void)
	{
		this_single_type::init_ins();
	}

	inline static void init_ins(const obj_ptr_type& pobj)
	{
		this_single_type::init_ins(pobj);
	}

	inline static obj_ptr_type get_ins(void)
	{
		return this_single_type::get_ins();
	}

	inline static void uninstall(void)
	{
		this_single_type::uninstall();
	}

	static bool s_register_code(code_type code)
	{
		obj_ptr_type ptr = this_single_type::get_ins();
		if(!ptr)
		{
			return false;
		}

		return ptr->register_code(code);
	}

	inline static bool s_unregister_code(code_type code)
	{
		obj_ptr_type ptr = this_single_type::get_ins();
		if(!ptr)
		{
			return false;
		}

		return ptr->unregister_code(code);
	}

	template<typename Ret, typename Handler> inline
	static bool s_safe_invoke(Ret& ret, const Handler& handler)
	{
		obj_cptr_type ptr = this_single_type::get_ins();
		if(!ptr)
		{
			ret = handler();
			return true;
		}

		return ptr->safe_invoke(ret, handler);
	}

	template<typename Handler> inline
	static bool s_safe_invoke(const Handler& handler)
	{
		obj_cptr_type ptr = this_single_type::get_ins();
		if(!ptr)
		{
			handler();
			return true;
		}

		return ptr->safe_invoke(handler);
	}
};

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
typedef seh<seh_win> seh_type;
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_ANDROID)
#	if  defined(YGGR_AT_ANDROID)
typedef seh<seh_android> seh_type;
#	else
typedef seh<seh_linux> seh_type;
#	endif // defined(YGGR_AT_ANDROID)
#elif defined(YGGR_AT_DARWIN)
typedef seh<seh_darwin> seh_type;
#else
#   error "please add now platform seh type !!!"
#endif //defined(WIN32) || defined(WIN64) || defined(WINDOWS)

} // namespace seh
} // namesapce yggr

#endif // YGGR_USE_SEH

#endif // #if defined(_MSC_VER) || defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)

#endif // __YGGR_SEH_SEH_HPP__
