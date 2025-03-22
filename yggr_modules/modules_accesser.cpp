//modules_accesser.cpp

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

#include <yggr/modules/modules_accesser.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace modules
{

module_handle_type modules_accesser::load_module(const yggr::string& module_name, int mode)
{
    module_handle_type handle;
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
    handle = LoadLibrary(module_name.data());
#elif (defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN))
    handle = dlopen(module_name.data(), mode);
#else
#   error "append other os module support code !!!"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

    if(!handle)
    {
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		u32 code = GetLastError();
        char tmsg[1024] = {0};
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                        0, code, 0, tmsg, 1023, 0);

        std::stringstream ss;
        ss << "os_code = " << code << ", message = " << tmsg;
		throw(error_maker_type::make_error(error_maker_type::E_invalid_module, ss.str()));

#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)
        std::string msg(dlerror());
		throw(error_maker_type::make_error(error_maker_type::E_invalid_module, msg));
#else
#   error "append other os moudle support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
        return module_handle_type();
    }

    return handle;
}

module_handle_type modules_accesser::load_module(error_type& e, const yggr::string& module_name, int mode)
{
    module_handle_type handle;
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
    handle = LoadLibrary(module_name.data());
#elif (defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN))
    handle = dlopen(module_name.data(), mode);
#else
#   error "append other os module support code !!!"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

    if(!handle)
    {
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		u32 code = GetLastError();
        char tmsg[1024] = {0};
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                        0, code, 0, tmsg, 1023, 0);

        std::stringstream ss;
        ss << "os_code = " << code << ", message = " << tmsg;
		e = error_maker_type::make_error(error_maker_type::E_invalid_module, ss.str());

#elif (defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN))
        std::string msg(dlerror());
		e = error_maker_type::make_error(error_maker_type::E_invalid_module, msg);
#else
#   error "append other os moudle support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
        return module_handle_type();
    }

    return handle;
}

void modules_accesser::free_module(module_handle_type handle)
{
    if(!handle)
    {
        return;
    }
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
    FreeLibrary(handle);
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)
    dlclose(handle);
#else
#   error "append other os module support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
}

void* modules_accesser::get_entry(module_handle_type handle, const yggr::string& entry_name)
{
	typedef void* foo_ptr_type;
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	foo_ptr_type foo_ptr = reinterpret_cast<foo_ptr_type>(GetProcAddress(handle, entry_name.data()));
#elif (defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN))
	foo_ptr_type foo_ptr = reinterpret_cast<foo_ptr_type>(dlsym(handle, entry_name.data()));
#else
#   error "append other os module support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

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

#elif (defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN))
		std::string msg(dlerror());
		throw(error_maker_type::make_error(error_maker_type::E_invalid_module_entry, msg));
#else
#   error "append other os moudle support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		return static_cast<void*>(0);
	}

	return foo_ptr;
}

void* modules_accesser::get_entry(error_type& e, module_handle_type handle, const yggr::string& entry_name)
{
	typedef void* foo_ptr_type;
#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
	foo_ptr_type foo_ptr = reinterpret_cast<foo_ptr_type>(GetProcAddress(handle, entry_name.data()));
#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)
	foo_ptr_type foo_ptr = reinterpret_cast<foo_ptr_type>(dlsym(handle, entry_name.data()));
#else
#   error "append other os module support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

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

#elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)
		std::string msg(dlerror());
		e = error_maker_type::make_error(error_maker_type::E_invalid_module_entry, msg);
#else
#   error "append other os moudle support code !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		return static_cast<void*>(0);
	}

	return foo_ptr;
}


} // namspace modules
} // namespace yggr
