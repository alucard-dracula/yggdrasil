//python_script.cpp

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

#include <yggr/script/python_script.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>
#include <yggr/exception/exception.hpp>

#include <boost/python/exec.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace script
{
namespace python
{

python_script::python_script(const enter_foo_name_type& enter_foo,
								const string_code_type& code,
								py_dict_type& global)
	: _enter_foo(enter_foo), _bsuccess(false)
{
	if(enter_foo.empty())
	{
		return;
	}
	_local = global.copy();
	compile_from_string(code);
}

python_script::python_script(const enter_foo_name_type& enter_foo,
								const file_code_type& code,
								py_dict_type& global)
	: _enter_foo(enter_foo), _bsuccess(false)
{
	if(enter_foo.empty())
	{
		return;
	}
	_local = global.copy();
	compile_from_file(code);
}

python_script::~python_script(void)
{
}

std::string python_script::get_exception_msg(void) const
{
	PyObject *exc = 0, *val = 0, *tb = 0;
	PyErr_Fetch(&exc, &val, &tb);
	PyErr_NormalizeException(&exc, &val, &tb);
	boost::python::handle<> hexc(exc), hval(boost::python::allow_null(val)), htb(boost::python::allow_null(tb));
	if(!hval)
	{
		return boost::python::extract<std::string>(boost::python::str(hexc));
	}
	else
	{
		py_object_type traceback(boost::python::import("traceback"));
		py_object_type format_exception(traceback.attr("format_exception"));
		py_object_type formatted_list(format_exception(hexc, hval, htb));
		py_object_type formatted(boost::python::str("").join(formatted_list));
		return boost::python::extract<std::string>(formatted);
	}
}

void python_script::compile_from_string(const string_type& code)
{
	try
	{
		// boost.python has some memory leaks. but it is harmless
		_object = boost::python::exec(code.data(), _local, _local);
		_bsuccess = true;
	}
	catch(const boost::python::error_already_set&)
	{
		exception::exception::throw_error(
			error_maker_type::make_error(
				error_maker_type::E_python_code_compile_error,
				code.str<std::string>()));
	}
}

void python_script::compile_from_file(const string_type& code)
{
	typedef file_system::local_fsys_operators local_fsys_operators_type;

	::yggr::string code_buf;

	try
	{
		local_fsys_operators_type::read_file_of_binary(
			local_fsys_operators_type::make_path_object(code), code_buf);
	}
	catch(const ::yggr::stl_exception& e)
	{
		exception::exception::throw_error(e);
		return;
	}

	try
	{
		// boost.python has some memory leaks. but it is harmless
		_object = boost::python::exec(code_buf.data(), _local, _local);
		_bsuccess = true;
	}
	catch(const boost::python::error_already_set&)
	{
		exception::exception::throw_error(
			error_maker_type::make_error(
				error_maker_type::E_python_code_compile_error,
				code.str<std::string>()));
	}
}

} // namespace python
} // namespace script
} // namespace yggr

