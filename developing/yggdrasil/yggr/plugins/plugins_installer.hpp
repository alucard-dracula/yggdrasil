// plugins_installer.hpp

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

#ifndef __YGGR_PLUGINS_PLUGINS_INSTALLER_HPP__
#define __YGGR_PLUGINS_PLUGINS_INSTALLER_HPP__

#include <boost/tuple/tuple.hpp>

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

	template<typename Ptr, typename Handler>
    this_type& install(const std::string& module_name, Handler handler)
    {
		return this_type::install<Ptr, Handler>(module_name, "module_enrty", handler);
    }

    template<typename Ptr, typename Handler>
    this_type& install(const std::string& module_name, const std::string& entry_name, Handler handler)
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
			handler(_modules_mgr.template get_entry<ptr_type>(boost::get<0>(*i), boost::get<1>(*i)));
		}

		return *this;
	}

	this_type& uninstall(const std::string& module_name)
	{
		_modules_mgr.erase_module(module_name);
		return *this;
	}

	template<typename Ptr, typename Handler>
    this_type& uninstall(const std::string& module_name, Handler handler)
    {
		return this_type::install<Ptr, Handler>(module_name, "module_enrty", handler);
    }

    template<typename Ptr, typename Handler>
    this_type& uninstall(const std::string& module_name, const std::string& entry_name, Handler handler)
    {
		typedef Ptr ptr_type;
		handler(_modules_mgr.template get_entry<ptr_type>(module_name, entry_name));
		_modules_mgr.erase_module(module_name);
        return *this;
    }

	template<typename Ptr, typename Plugin_names, typename Handler>
	this_type& uninstall(const Plugin_names names, const Handler& handler)
	{
		typedef Ptr ptr_type;
		typedef Plugin_names plugins_names_type;
		typedef typename plugins_names_type::const_iterator plugins_names_c_iter_type;

		for(plugins_names_c_iter_type i = names.begin(), isize = names.end(); i != isize; ++i)
		{
			handler(_modules_mgr.template get_entry<ptr_type>(boost::get<0>(*i), boost::get<1>(*i)));
			_modules_mgr.erase_module(boost::get<0>(*i));
		}

		return *this;
	}

	this_type& uninstall_all(void)
	{
		_modules_mgr.clear();
		return *this;
	}

	template<typename Handler>
	this_type& uninstall_all(const Handler& handler)
	{
		_modules_mgr.use_handler(handler);
		return *this;
	}


private:
    modules_mgr_type _modules_mgr;
};

} // namespace plugins
} // namespace yggr

#endif // __YGGR_PLUGINS_PLUGINS_INSTALLER_HPP__
