//modules_mgr.hpp

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

#ifndef __YGGR_MODULES_MODULES_MANAGR_HPP__
#define __YGGR_MODULES_MODULES_MANAGR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/modules/module.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

namespace yggr
{
namespace modules
{

class modules_mgr
{
public:
	typedef module module_type;
    typedef ::yggr::shared_ptr<module_type> module_ptr_type;

private:
	typedef yggr::safe_container::safe_unordered_map<yggr::string, module_ptr_type> module_ptr_map_type;
    typedef module_ptr_map_type::value_type ins_val_type;
    typedef module_ptr_map_type::iterator module_ptr_map_iter_type;
    typedef module_ptr_map_type::const_iterator module_ptr_mpa_c_iter_type;

private:
    typedef modules_mgr this_type;

public:
    modules_mgr(void);
    ~modules_mgr(void);

public:
	// call use try catch modules_accesser::error_type
    inline module_ptr_type get_module(const yggr::string& name, int mode = 1) 
	{
		return 
			_module_ptr_map.find(
				name,
				boost::bind(
					&this_type::prv_s_handler_get_module,
					_1, _2, boost::cref(name), mode) );
	}

	// call use try catch modules_accesser::error_type
	inline module_ptr_type get_exists_module(const yggr::string& name)
	{
		module_ptr_type ptr;
		_module_ptr_map.get_value(name, ptr);
		return ptr;
	}

    inline this_type& erase_module(const yggr::string& name)
	{
		_module_ptr_map.erase(name);
		return *this;
	}

    inline this_type& clear(void)
	{
		_module_ptr_map.clear();
		return *this;
	}

public:
	// call use try catch modules_accesser::error_type
	inline void* get_entry(const yggr::string& module_name, const yggr::string& entry_name)
	{
		module_ptr_type ptr(this_type::get_module(module_name));
		return ptr? ptr->get_entry(entry_name) : static_cast<void*>(0);
	}

	// call use try catch modules_accesser::error_type
    template<typename FOO_PTR> inline
    FOO_PTR get_entry(const yggr::string& module_name, const yggr::string& entry_name)
    {
        typedef FOO_PTR foo_ptr_type;

		module_ptr_type pmodule(this_type::get_module(module_name));
		return pmodule? pmodule->get_entry<foo_ptr_type>(entry_name) : foo_ptr_type();
    }

	// call use try catch modules_accesser::error_type
	inline void* get_exists_entry(const yggr::string& module_name, const yggr::string& entry_name)
	{
		module_ptr_type ptr(this_type::get_exists_module(module_name));
		return ptr? ptr->get_entry(entry_name) : static_cast<void*>(0);
	}

	// call use try catch modules_accesser::error_type
    template<typename FOO_PTR> inline
    FOO_PTR get_exists_entry(const yggr::string& module_name, const yggr::string& entry_name)
    {
        typedef FOO_PTR foo_ptr_type;

		module_ptr_type pmodule(this_type::get_exists_module(module_name));
		return pmodule? pmodule->get_entry<foo_ptr_type>(entry_name) : foo_ptr_type();
    }

private:
	static module_ptr_type prv_s_handler_get_module(module_ptr_map_type::base_type& base, 
													module_ptr_map_type::iterator rst,
													const yggr::string& name, int mode);

private:
    module_ptr_map_type _module_ptr_map;
};

} // namespace modules
} // namespace yggr

#endif //__YGGR_MODULES_MODULES_MANAGR_HPP__
