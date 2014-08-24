//modules_mgr.cpp

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

#include <yggr/modules/modules_mgr.hpp>

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

modules_mgr::module_ptr_type modules_mgr::get_module(const std::string& name, int mode)
{
	return _module_ptr_map.use_handler(
							boost::bind(
											&this_type::handler_get_module,
											this, _1, boost::cref(name), mode
										)
									);
}

modules_mgr::module_ptr_type modules_mgr::get_exists_module(const std::string& name)
{
	return _module_ptr_map.use_handler(
							boost::bind(
											&this_type::handler_get_exists_module,
											this, _1, boost::cref(name)
										)
									);
}

void* modules_mgr::get_entry(const std::string& module_name, const std::string& entry_name)
{
	module_ptr_type ptr(this_type::get_module(module_name));
	if(!ptr)
	{
		return 0;
	}

	return ptr->get_entry(entry_name);
}

void* modules_mgr::get_exists_entry(const std::string& module_name, const std::string& entry_name)
{
	module_ptr_type ptr(this_type::get_exists_module(module_name));
	if(!ptr)
	{
		return 0;
	}

	return ptr->get_entry(entry_name);
}

modules_mgr::this_type& modules_mgr::erase_module(const std::string& name)
{
	_module_ptr_map.erase(name);
	return *this;
}

modules_mgr::this_type& modules_mgr::clear(void)
{
	_module_ptr_map.clear();
	return *this;
}

modules_mgr::module_ptr_type 
	modules_mgr::handler_get_module(module_ptr_map_type::base_type& base, 
										const std::string& name, int mode)
{
	module_ptr_map_iter_type iter = base.find(name);

	if(iter == base.end())
	{
		module_ptr_type ptr(new module(name, mode));
		if(!ptr || ptr->empty())
		{
			return module_ptr_type();
		}

		base[name] = ptr;
		return ptr;
	}

	if(iter->second->empty())
	{
		module_ptr_type ptr(new module(name, mode));
		if(!ptr || ptr->empty())
		{
			base.erase(iter);
			return module_ptr_type();
		}
		iter->second.swap(ptr);
		return iter->second;
	}

	return iter->second;
}

modules_mgr::module_ptr_type 
	modules_mgr::handler_get_exists_module(module_ptr_map_type::base_type& base, 
								const std::string& name)
{
	module_ptr_map_iter_type iter = base.find(name);

	if(iter == base.end())
	{
		return module_ptr_type();
	}

	return iter->second;
}

} // namespace yggr
} // namespace modules
