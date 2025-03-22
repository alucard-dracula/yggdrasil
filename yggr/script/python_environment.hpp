//python_environment.hpp

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

#ifndef __YGGR_SCRIPT_PYTHON_ENVIRONMENT_HPP__
#define __YGGR_SCRIPT_PYTHON_ENVIRONMENT_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/script/python_base_environment.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/base/ptr_single.hpp>

#include <boost/python.hpp>

namespace yggr
{
namespace script
{
namespace python
{

class python_environment
	: public python_base_environment
{
public:
	python_environment(void)
		: _global(boost::python::import("__main__").attr("__dict__"))
	{
	}

	~python_environment(void)
	{
	}

	inline boost::python::dict& get_global_environment(void)
	{
		return _global;
	}

	inline const boost::python::dict& get_global_environment(void) const
	{
		return _global;
	}

	inline bool append_module(const char* mod_name)
	{
		//if(!PyImport_ImportModule(mod_name))
		//{
		//	exception::exception::throw_error(
		//		std::runtime_error(std::string("init python module \"")
		//							+ std::string(mod_name)
		//							+ std::string("\" faild")));
		//	return false;
		//}
		//else
		//{
		//	return true;
		//}

		return PyImport_ImportModule(mod_name);
	}

	template<typename Module_Init_Foo>
	bool append_module(char* mod_name, Module_Init_Foo foo)
	{
		//if(PyImport_AppendInittab(mod_name, foo) == -1)
		//{
		//	exception::exception::throw_error(
		//		std::runtime_error(std::string("init python module \"")
		//							+ std::string(mod_name)
		//							+ std::string("\" faild")));
		//	return false;
		//}
		//else
		//{
		//	return true;
		//}

		return !(PyImport_AppendInittab(mod_name, foo) == -1);
	}
protected:
	boost::python::dict _global;
};

} // nemspace python
} // namespace script
} // namespace yggr

#endif //__YGGR_SCRIPT_PYTHON_ENVIRONMENT_HPP__
