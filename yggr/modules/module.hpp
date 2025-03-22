//module.h

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

#ifndef __YGGR_MODULES_MODULE_H__
#define __YGGR_MODULES_MODULE_H__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/move/move.hpp>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/modules/defmodules.h>
#include <yggr/modules/modules_accesser.hpp>

#include <boost/ref.hpp>

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
    module(const yggr::string& module_name, int mode = 1);

	module(BOOST_RV_REF(this_type) right)
		: _name(boost::move(right._name)),
			_handle(boost::move(right._handle))
	{
	}

    module(const this_type& right);
    ~module(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_name, boost::move(right._name));
		copy_or_move_or_swap(_handle, boost::move(right._handle));
		return *this;
	}
    
	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_name.swap(right._name);
		yggr::swap(_handle, right._handle);
	}

public:
	inline bool load(const yggr::string& module_name, int mode = 1)
	{
		_name = module_name;
		_handle = modules_accesser::load_module(module_name, mode);

		return _handle != module_handle_type(0);
	}

	inline void close(void)
	{
		modules_accesser::free_module(_handle);
	}

public:
	inline bool compare_eq(const yggr::string& name) const
	{
		 return name == _name;
	}

    inline bool compare_eq(const this_type& right) const
	{
		 return 
			 (this == boost::addressof(right))
			 || _name == right._name;
	}

public:
    inline bool empty(void) const
	{
		return !_handle;
	}

	inline void* get_entry(const yggr::string& name) const
	{
		return modules_accesser::get_entry(_handle, name);
	}

    template<typename FOO_PTR> inline
    FOO_PTR get_entry(const yggr::string& name) const
    {
        typedef FOO_PTR foo_ptr_type;
        return modules_accesser::get_entry<foo_ptr_type>(_handle, name);
    }
private:
    yggr::string _name;
    module_handle_type _handle;
};

// non member foo

//==
inline bool operator==(const yggr::string& l, const module& r)
{
	return r.compare_eq(l);
}

inline bool operator==(const module& l, const yggr::string& r)
{
	return l.compare_eq(r);
}

inline bool operator==(const module& l, const module& r)
{
	return l.compare_eq(r);
}

// !=
inline bool operator!=(const yggr::string& l, const module& r)
{
	return !r.compare_eq(l);
}

inline bool operator!=(const module& l, const yggr::string& r)
{
	return !l.compare_eq(r);
}

inline bool operator!=(const module& l, const module& r)
{
	return !l.compare_eq(r);
}

} // namespace modules
} // namespace yggr

// swap
namespace yggr
{
namespace modules
{
namespace swap_support
{

	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(module)

} // namespace swap_support

using swap_support::swap;

} // namespace modules
} // namespace yggr

namespace std
{
	using ::yggr::modules::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::modules::swap_support::swap;
} // namespace boost

#endif //__YGGR_MODULES_MODULE_H__

