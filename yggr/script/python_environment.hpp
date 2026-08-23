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

#if (defined(YGGR_MSVC_USING_MTD_FLAG) && YGGR_MSVC_USING_MTD_FLAG) \
	|| (defined(YGGR_MSVC_USING_MT_FLAG) && YGGR_MSVC_USING_MT_FLAG)

#	error "!!! script python not support /MTd or /MT !!!"

#else

#include <yggr/script/python_base_environment.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/charset/string.hpp>

#include <boost/python.hpp>
#include <boost/ref.hpp>

#include <cassert>

namespace yggr
{
namespace script
{
namespace python
{

namespace detail
{

struct failed_dict
{
public:
	inline operator boost::python::dict&(void) const
	{
		return *reinterpret_cast<boost::python::dict*>(boost::addressof(dict_buff[0]));
	}

public:
	static char dict_buff[sizeof(boost::python::dict)];
};

} // namespace detail

class python_environment
{
private:
	typedef yggr::shared_ptr<boost::python::dict> py_dict_ptr_type;

private:
	typedef python_environment this_type;

public:
	python_environment(void)
	{
	}

	~python_environment(void)
	{
	}

	inline static boost::python::dict& s_get_global_environment(void)
	{
		assert(_s_pglobal);
		return _s_pglobal? *_s_pglobal : detail::failed_dict();
	}

	inline boost::python::dict& get_global_environment(void)
	{
		return this_type::s_get_global_environment();
	}

	inline const boost::python::dict& get_global_environment(void) const
	{
		return this_type::s_get_global_environment();
	}

	inline static bool append_module(const char* mod_name)
	{
		return 
			!Py_IsInitialized()
			&& PyImport_ImportModule(mod_name);
	}

	template<typename Module_Init_Foo> inline
	static bool append_module(const char* mod_name, Module_Init_Foo foo)
	{
		return 
			!Py_IsInitialized()
			&& !(PyImport_AppendInittab(mod_name, foo) == -1);
	}

	inline static bool py_initialize(void)
	{
		if(Py_IsInitialized())
		{
			return true;
		}
		else
		{
			Py_Initialize();
			if(!_s_pglobal)
			{
				_s_pglobal = py_dict_ptr_type(yggr_nothrow_new boost::python::dict(boost::python::import("__main__").attr("__dict__")));
			}
			return Py_IsInitialized();
		}
	}

	inline static bool py_finalize(void)
	{
		if(!Py_IsInitialized())
		{
			return true;
		}
		else
		{
			if(_s_pglobal)
			{
				py_dict_ptr_type tmp;
				tmp.swap(_s_pglobal);
				tmp->clear();
			}

			Py_Finalize();
			return !Py_IsInitialized();
		}
	}

private:
	static py_dict_ptr_type _s_pglobal;
};

} // nemspace python
} // namespace script
} // namespace yggr

#endif // YGGR_MSVC_USING_MTD_FLAG

#endif //__YGGR_SCRIPT_PYTHON_ENVIRONMENT_HPP__
