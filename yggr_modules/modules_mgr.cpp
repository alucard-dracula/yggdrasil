//modules_mgr.cpp

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

#include <yggr/modules/modules_mgr.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace modules
{

modules_mgr::modules_mgr(void)
{
}

modules_mgr::~modules_mgr(void)
{
	_module_ptr_map.clear();
}

/*static*/ 
modules_mgr::module_ptr_type 
	modules_mgr::prv_s_handler_get_module(module_ptr_map_type::base_type& base, 
											module_ptr_map_type::iterator rst,
											const yggr::string& name, int mode)
{

	if(rst == base.end())
	{
		module_ptr_type ptr = construct_shared<module_type>(yggr_nothrow_new module_type(name, mode));
		if(!ptr || ptr->empty())
		{
			return module_ptr_type();
		}

		try
		{
			base[name] = ptr;
		}
		catch(::yggr::stl_exception&)
		{
			return module_ptr_type();
		}
		return ptr;
	}
	else
	{
		if(rst->second->empty())
		{
			module_ptr_type ptr = construct_shared<module_type>(yggr_nothrow_new module_type(name, mode));
			if(!ptr || ptr->empty())
			{
				base.erase(rst);
				return module_ptr_type();
			}
			rst->second.swap(ptr);
		}
		
		return rst->second;
	}
}


} // namespace yggr
} // namespace modules
