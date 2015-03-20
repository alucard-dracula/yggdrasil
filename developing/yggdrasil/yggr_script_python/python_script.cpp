//python_script.cpp

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

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER

#include <boost/python/exec.hpp>

#include <yggr/script/python_script.hpp>
#include <yggr/file_system/local_file_operator.hpp>
#include <yggr/base/exception.hpp>

namespace yggr
{
namespace script
{
namespace python
{

//python_script::python_script(const std::string& enter_foo, const std::string& code, py_dict_type& grobal)
//	: _enter_foo(enter_foo), _bsuccess(false)
//{
//	if(enter_foo.empty())
//	{
//		return;
//	}
//	_local = grobal.copy();
//	compile_from_string(code);
//}

python_script::python_script(const std::string& enter_foo, const string_code& code, py_dict_type& grobal)
	: _enter_foo(enter_foo), _bsuccess(false)
{
	if(enter_foo.empty())
	{
		return;
	}
	_local = grobal.copy();
	compile_from_string(code);
}

python_script::python_script(const std::string& enter_foo, const file_code& code, py_dict_type& grobal)
	: _enter_foo(enter_foo), _bsuccess(false)
{
	if(enter_foo.empty())
	{
		return;
	}
	_local = grobal.copy();
	compile_from_file(code);
}

python_script::~python_script(void)
{
}

void python_script::compile_from_string(const std::string& code)
{
	try
	{
		_object = boost::python::exec(code.c_str(), _local, _local);
		_bsuccess = true;
	}
	catch(const boost::python::error_already_set&)
	{
		exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_python_code_compile_error, code));
		//_bsuccess = false;
	}
}

void python_script::compile_from_file(const std::string& code)
{
	std::string code_buf;
	try
	{
		file_system::local_file_operator_type::read_file_of_binary(code, code_buf);
	}
	catch(const compatibility::stl_exception& e)
	{
		exception::exception::throw_error(e);
		return;
	}

	try
	{
		_object = boost::python::exec(code_buf.c_str(), _local, _local);
		_bsuccess = true;
	}
	catch(const boost::python::error_already_set&)
	{
		exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_python_code_compile_error, code));
		//_bsuccess = false;
	}
}

} // namespace python
} // namespace script
} // namespace yggr

