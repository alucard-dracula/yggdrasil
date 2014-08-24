//python_environment.hpp

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

#ifndef __YGGR_SCRIPT_PYTHON_PYTHON_ENVIRONMENT_HPP__
#define __YGGR_SCRIPT_PYTHON_PYTHON_ENVIRONMENT_HPP__

#include <boost/python.hpp>
#include <yggr/script/base_python_environment.hpp>
#include <yggr/base/exception.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/base/ptr_single.hpp>

namespace yggr
{
namespace script
{
namespace python
{

class python_environment : public base_python_environment
{
public:
	python_environment(void);
	~python_environment(void);

	bool append_module(const char* mod_name);

	inline boost::python::dict& get_grobal_environment(void)
	{
		return _grobal;
	}

	template<typename Module_Init_Foo>
	bool append_module(char* mod_name, Module_Init_Foo foo)
	{
		try
		{
			if(PyImport_AppendInittab(mod_name, foo) == -1)
			{
				throw std::runtime_error(std::string("init python module \"") + std::string(mod_name) + std::string("\" faild"));
			}
		}
		catch(const std::runtime_error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}
protected:
	boost::python::dict _grobal;
};

} // nemspace python
} // namespace script
} // namespace yggr

#endif //__YGGR_SCRIPT_PYTHON_PYTHON_ENVIRONMENT_HPP__