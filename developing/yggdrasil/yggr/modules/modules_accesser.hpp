//modules_accesser.hpp

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

#ifndef __YGGR_MODULESS_MODULES_ACCESSER_HPP__
#define __YGGR_MODULESS_MODULES_ACCESSER_HPP__

#include <sstream>

#include <yggr/base/yggrdef.h>
#include <yggr/modules/defmodules.h>
#include <yggr/charset/string.hpp>

//#include <yggr/base/exception.hpp>
#include <yggr/base/exception_sort.hpp>
#include <boost/shared_ptr.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/base/error_make.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace modules
{

class modules_accesser
	: private nonable::noncreateable
{

public:
	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("modules_accesser")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_12)
			ERROR_CODE_DEF(E_invalid_module)
			ERROR_CODE_DEF(E_invalid_module_entry)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_12)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_module, "invalid module")
			ERROR_CODE_MSG(E_invalid_module_entry, "invalid module entry")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------

public:
    static module_handle_type load_module(const std::string& module_name, int mode = 1);
	static module_handle_type load_module(error_type& e, const std::string& module_name, int mode = 1);
    static void free_module(module_handle_type handle);

	static void* get_entry(module_handle_type handle, const std::string& entry_name);
	static void* get_entry(error_type& e, module_handle_type handle, const std::string& entry_name);

    template<typename FOO_PTR>
    static FOO_PTR get_entry(module_handle_type handle, const std::string& entry_name)
    {
        typedef FOO_PTR foo_ptr_type;
#if defined(WIN32) || defined(WIN64) || defined(WINDOS)
        foo_ptr_type foo_ptr = (foo_ptr_type)GetProcAddress(handle, entry_name.c_str());
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_MAC)
        foo_ptr_type foo_ptr = (foo_ptr_type)dlsym(handle, entry_name.c_str());
#else // defined(YGGR_AT_LINUX)
#   error "append other os module support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOS)

        if(!foo_ptr)
        {
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
			u32 code = ::GetLastError();
            char tmsg[1024] = {0};
			::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                            0, code, 0, tmsg, 1023, 0);

            std::stringstream ss;
            ss << "os_code = " << code << ", message = " << tmsg;
			throw(error_maker_type::make_error(error_maker_type::E_invalid_module_entry, ss.str()));

#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_MAC)
            std::string msg(dlerror());
			throw(error_maker_type::make_error(error_maker_type::E_invalid_module_entry, msg));
#else // defined(YGGR_AT_LINUX)
#   error "append other os moudle support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
            return (foo_ptr_type)0;
        }

        return foo_ptr;
    }

	template<typename FOO_PTR>
    static FOO_PTR get_entry(error_type& e, module_handle_type handle, const std::string& entry_name)
    {
        typedef FOO_PTR foo_ptr_type;
#if defined(WIN32) || defined(WIN64) || defined(WINDOS)
        foo_ptr_type foo_ptr = (foo_ptr_type)GetProcAddress(handle, entry_name.c_str());
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_MAC)
        foo_ptr_type foo_ptr = (foo_ptr_type)dlsym(handle, entry_name.c_str());
#else // defined(YGGR_AT_LINUX)
#   error "append other os module support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOS)

        if(!foo_ptr)
        {
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
			u32 code = ::GetLastError();
            char tmsg[1024] = {0};
			::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                            0, code, 0, tmsg, 1023, 0);

            std::stringstream ss;
            ss << "os_code = " << code << ", message = " << tmsg;
			e = error_maker_type::make_error(error_maker_type::E_invalid_module_entry, ss.str());

#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_MAC)
            std::string msg(dlerror());
			e = error_maker_type::make_error(error_maker_type::E_invalid_module_entry, msg);
#else // defined(YGGR_AT_LINUX)
#   error "append other os moudle support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
            return (foo_ptr_type)0;
        }

        return foo_ptr;
    }
};

} // namespace modules
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif //__YGGR_MODULESS_MODULES_ACCESSER_HPP__
