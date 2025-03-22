// plugins_installer.hpp

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

#ifndef __YGGR_PLUGINS_PLUGINS_INSTALLER_HPP__
#define __YGGR_PLUGINS_PLUGINS_INSTALLER_HPP__

#include <yggr/charset/string.hpp>
#include <yggr/tuple/tuple.hpp>

namespace yggr
{
namespace plugins
{

template<typename Modules_Manager>
class plugins_installer
{
public:
    typedef Modules_Manager modules_mgr_type;
	typedef plugins_installer this_type;
public:
    plugins_installer(void)
    {
    }

    ~plugins_installer(void)
    {
		uninstall_all();
    }

public:
	template<typename Ptr, typename Handler> inline
    this_type& install(const yggr::string& module_name, Handler handler)
    {
		return this_type::install<Ptr, Handler>(module_name, "module_enrty", handler);
    }

    template<typename Ptr, typename Handler> inline
    this_type& install(const yggr::string& module_name, const yggr::string& entry_name, Handler handler)
    {
		typedef Ptr ptr_type;
		handler(_modules_mgr.template get_entry<ptr_type>(module_name, entry_name));
        return *this;
    }

	template<typename Ptr, typename Plugin_names, typename Handler>
	this_type& install(const Plugin_names names, const Handler& handler)
	{
		typedef Ptr ptr_type;
		typedef Plugin_names plugins_names_type;
		typedef typename plugins_names_type::const_iterator plugins_names_c_iter_type;

		for(plugins_names_c_iter_type i = names.begin(), isize = names.end(); i != isize; ++i)
		{
			handler(_modules_mgr.template get_entry<ptr_type>(yggr::get<0>(*i), yggr::get<1>(*i)));
		}

		return *this;
	}

	inline this_type& uninstall(const yggr::string& module_name)
	{
		_modules_mgr.erase_module(module_name);
		return *this;
	}

	template<typename Ptr, typename Handler> inline
    this_type& uninstall(const yggr::string& module_name, Handler handler)
    {
		return this_type::install<Ptr, Handler>(module_name, "module_enrty", handler);
    }

    template<typename Ptr, typename Handler> inline
    this_type& uninstall(const yggr::string& module_name, const yggr::string& entry_name, Handler handler)
    {
		typedef Ptr ptr_type;
		handler(_modules_mgr.template get_entry<ptr_type>(module_name, entry_name));
		_modules_mgr.erase_module(module_name);
        return *this;
    }

	template<typename Ptr, typename Plugin_names, typename Handler>
	this_type& uninstall(const Plugin_names& names, const Handler& handler)
	{
		typedef Ptr ptr_type;
		typedef Plugin_names plugins_names_type;
		typedef typename plugins_names_type::const_iterator plugins_names_c_iter_type;

		for(plugins_names_c_iter_type i = names.begin(), isize = names.end(); i != isize; ++i)
		{
			handler(_modules_mgr.template get_entry<ptr_type>(yggr::get<0>(*i), yggr::get<1>(*i)));
			_modules_mgr.erase_module(yggr::get<0>(*i));
		}

		return *this;
	}

	inline this_type& uninstall_all(void)
	{
		_modules_mgr.clear();
		return *this;
	}

	template<typename Handler> inline
	this_type& uninstall_all(const Handler& handler)
	{
		_modules_mgr.using_handler(handler);
		return *this;
	}


private:
    modules_mgr_type _modules_mgr;
};

} // namespace plugins
} // namespace yggr

#endif // __YGGR_PLUGINS_PLUGINS_INSTALLER_HPP__
