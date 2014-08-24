//module.h

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

#ifndef __YGGR_MODULES_MODULE_H__
#define __YGGR_MODULES_MODULE_H__

#include <yggr/move/move.hpp>
#include <yggr/modules/defmodules.h>
#include <yggr/modules/modules_accesser.hpp>

namespace yggr
{
namespace modules
{

class module
{
private:
	typedef module this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
    module(void);
    module(const std::string& module_name, int mode = 1);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	module(BOOST_RV_REF(this_type) right)
		: _name(boost::forward<std::string>(right._name)),
			_handle(boost::forward<module_handle_type>(right._handle))
	{
	}
#else
	module(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		_name.swap(right_ref._name);
		std::swap(_handle, right_ref._handle);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

    module(const this_type& right);
    ~module(void);

	bool load(const std::string& module_name, int mode = 1);
	void close(void);

	//this_type& operator=(BOOST_RV_REF(this_type) right)

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_name = boost::forward<std::string>(right._name);
		_handle = boost::forward<module_handle_type>(right._handle);
#else
		this_type& right_ref = right;
		_name.swap(right_ref._name);
		std::swap(_handle, right_ref._handle);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}
    this_type& operator=(const this_type& right);

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//_name.swap(boost::forward<std::string>(right._name));
		_name.swap(right._name);
		std::swap(_handle, right._handle);
#else
		this_type& right_ref = right;
		_name.swap(right_ref._name);
		std::swap(_handle, right_ref._handle);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_name.swap(right._name);
		std::swap(_handle, right._handle);
	}

    bool operator==(const std::string& name) const;
    bool operator==(const this_type& right) const;
    bool empty(void) const;

	void* get_entry(const std::string& name) const;

    template<typename FOO_PTR>
    FOO_PTR get_entry(const std::string& name) const
    {
        typedef FOO_PTR foo_ptr_type;
        return modules_accesser::get_entry<foo_ptr_type>(_handle, name);
    }
private:
    std::string _name;
    module_handle_type _handle;
};

} // namespace modules
} // namespace yggr


#define _YGGR_MODULES_MODULE_SWAP_DEF() \
	void swap(yggr::modules::module& l, yggr::modules::module& r); \
	\
	inline void swap(BOOST_RV_REF(yggr::modules::module) l, yggr::modules::module& r) { \
		r.swap(l); } \
	\
	inline void swap(yggr::modules::module& l, BOOST_RV_REF(yggr::modules::module) r) { \
		l.swap(r); }


namespace std
{
	_YGGR_MODULES_MODULE_SWAP_DEF()
} // namespace std

namespace boost
{
	_YGGR_MODULES_MODULE_SWAP_DEF()
} // namespace boost

#undef _YGGR_MODULES_MODULE_SWAP_DEF

#endif //__YGGR_MODULES_MODULE_H__

