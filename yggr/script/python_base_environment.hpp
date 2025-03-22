//python_base_environment.hpp

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

#ifndef __YGGR_SCRIPT_PYTHON_BASE_ENVIRONMENT_HPP__
#define __YGGR_SCRIPT_PYTHON_BASE_ENVIRONMENT_HPP__

#include <yggr/base/yggrdef.h>

#define PYTHON_MODULE_NAME(x) (#x)

#if PY_VERSION_HEX >= 0x03000000
#	define PYTHON_MODULE_INIT_FOO(__name__) (PyInit_##__name__)
#else
#	define PYTHON_MODULE_INIT_FOO(__name__) (init##__name__)
#endif // PY_VERSION_HEX

namespace yggr
{
namespace script
{
namespace python
{

class python_base_environment
{
public:
	python_base_environment(void)
	{
		if(!Py_IsInitialized())
		{
			Py_Initialize();
		}
	}

	~python_base_environment(void)
	{
	}
};

} // nemspace python
} // namespace script
} // namespace yggr

#endif //__YGGR_SCRIPT_PYTHON_BASE_ENVIRONMENT_HPP__
