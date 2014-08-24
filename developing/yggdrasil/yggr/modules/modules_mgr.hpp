//modules_mgr.hpp

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

#ifndef __YGGR_MODULES_MODULES_MANAGR_HPP__
#define __YGGR_MODULES_MODULES_MANAGR_HPP__

#include <boost/shared_ptr.hpp>

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
    typedef boost::shared_ptr<module> module_ptr_type;

private:
	typedef yggr::safe_container::safe_unordered_map<const std::string, module_ptr_type> module_ptr_map_type;
    typedef module_ptr_map_type::value_type ins_val_type;
    typedef module_ptr_map_type::iterator module_ptr_map_iter_type;
    typedef module_ptr_map_type::const_iterator module_ptr_mpa_c_iter_type;

private:
    typedef modules_mgr this_type;

public:
    modules_mgr(void);
    ~modules_mgr(void);
    module_ptr_type get_module(const std::string& name, int mode = 1); // call use try catch
	module_ptr_type get_exists_module(const std::string& name); // call use try catch
    this_type& erase_module(const std::string& name);
    this_type& clear(void);

	void* get_entry(const std::string& module_name, const std::string& entry_name); // call use try catch

    template<typename FOO_PTR>
    FOO_PTR get_entry(const std::string& module_name, const std::string& entry_name) // call use try catch
    {
        typedef FOO_PTR foo_ptr_type;

		module_ptr_type pmodule(this_type::get_module(module_name));
		if(!pmodule)
		{
			return foo_ptr_type();
		}

        return pmodule->get_entry<foo_ptr_type>(entry_name);
    }

	void* get_exists_entry(const std::string& module_name, const std::string& entry_name); // call use try catch

    template<typename FOO_PTR>
    FOO_PTR get_exists_entry(const std::string& module_name, const std::string& entry_name) // call use try catch
    {
        typedef FOO_PTR foo_ptr_type;

		module_ptr_type pmodule(this_type::get_exists_module(module_name));
		if(!pmodule)
		{
			return foo_ptr_type();
		}

        return pmodule->get_entry<foo_ptr_type>(entry_name);
    }

private:
	module_ptr_type handler_get_module(module_ptr_map_type::base_type& base, const std::string& name, int mode);
	module_ptr_type handler_get_exists_module(module_ptr_map_type::base_type& base, const std::string& name);

private:
    module_ptr_map_type _module_ptr_map;
};

} // namespace modules
} // namespace yggr

#endif //__YGGR_MODULES_MODULES_MANAGR_HPP__
