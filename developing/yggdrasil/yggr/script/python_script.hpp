//python_script.hpp

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

#ifndef __YGGR_SCRIPT_PYTHON_PYTHON_SCRIPT_HPP__
#define __YGGR_SCRIPT_PYTHON_PYTHON_SCRIPT_HPP__

#include <boost/python.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/base/exception_sort.hpp>
#include <yggr/base/error_make.hpp>

namespace yggr
{
namespace script
{
namespace python
{

class python_script : private nonable::noncopyable
{
private:
	typedef boost::python::dict py_dict_type;
	typedef boost::python::object py_object_type;

private:
	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("python_script")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_19)
			ERROR_CODE_DEF(E_python_code_compile_error)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_19)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_python_code_compile_error, "python code compile error")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------

public:
	class string_code
	{
	public:
		string_code(const std::string& code)
			: str_code(code)
		{
		}

		string_code(const string_code& right)
			: str_code(right.str_code)
		{
		}

		~string_code(void)
		{
		}

		operator const std::string&(void) const
		{
			return str_code;
		}

		const std::string& str_code;
	};

	class file_code
	{
	public:
		file_code(const std::string& fname)
			: file_name(fname)
		{
		}

		file_code(const file_code& right)
			: file_name(right.file_name)
		{
		}

		~file_code(void)
		{
		}

		operator const std::string&(void) const
		{
			return file_name;
		}

		const std::string& file_name;
	};

public:

	python_script(const std::string& enter_foo, const string_code& code, py_dict_type& grobal);
	python_script(const std::string& enter_foo, const file_code& code, py_dict_type& grobal);
	~python_script(void);
	
	inline bool success(void) const
	{
		return _bsuccess;
	}

	template<typename Output_Real_Data, typename Input_Real_Data> 
	Output_Real_Data execute_rule(const Input_Real_Data& input)
	{
		typedef Output_Real_Data o_real_data_type;

		return boost::python::call<o_real_data_type>(boost::python::object(_local[_enter_foo.c_str()]).ptr(), &input);
	}

	template<typename Output_Real_Data> 
	Output_Real_Data execute_rule(void)
	{
		typedef Output_Real_Data o_real_data_type;

		return boost::python::call<o_real_data_type>(boost::python::object(_local[_enter_foo.c_str()]).ptr());
	}

	template<typename Output_Real_Data, typename Input_Real_Data>
	void execute_rule_no_ret(const Input_Real_Data& input)
	{
		typedef Output_Real_Data o_real_data_type;

		boost::python::call<o_real_data_type>(boost::python::object(_local[_enter_foo.c_str()]).ptr(), &input);
		return;
	}

	void execute_rule_no_ret(void)
	{
		boost::python::call<void>(boost::python::object(_local[_enter_foo.c_str()]).ptr());
		return;
	}

private:
	void compile_from_string(const std::string& code);

	void compile_from_file(const std::string& code);


private:
	std::string _enter_foo;
	py_dict_type _local;
	py_object_type _object;
	bool _bsuccess;
};

} // namespace python
} // namespace script
} // namespace yggr

#endif // __YGGR_SCRIPT_PYTHON_PYTHON_SCRIPT_HPP__
