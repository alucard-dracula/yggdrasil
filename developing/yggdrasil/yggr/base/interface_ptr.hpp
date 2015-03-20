//interface_ptr.hpp

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

#ifndef __YGGR_INTERFACE_PTR_HPP__
#define __YGGR_INTERFACE_PTR_HPP__

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp> 
#include <boost/shared_ptr.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/serialization/shared_ptr.hpp>
#include <yggr/serialization/nvp.hpp>

namespace yggr
{

template<typename T, 
			template<typename _T> 
				class SPtr = boost::shared_ptr >
class interface_ptr
	: public SPtr<T>
{
public:
	typedef T obj_type;
	typedef SPtr<obj_type> base_type;
	typedef obj_type* ptr_type;

public:
	interface_ptr(void)
	{
	}

	explicit interface_ptr(ptr_type ptr)
		: base_type(ptr)
	{
	}

	interface_ptr(const interface_ptr& right)
        :base_type(right)
	{
	}

	virtual ~interface_ptr(void)
	{
	}

	ptr_type get(void) const
	{
		return base_type::get();
	}

	// thank you for qq [C++/ObjC/Java]ZERO
	// call: child_type *p = interface_ptr return 0 is failed
	template<typename Child>
	operator Child*(void)
	{
		ptr_type tmp = base_type::get();
		return dynamic_cast<Child*>(tmp);
	}

	// thank you for qq [C++/ObjC/Java]ZERO
	template<typename Child>
	operator const Child*(void) const
	{
		ptr_type tmp = base_type::get();
		return dynamic_cast<Child*>(tmp);
	}

	template<typename Child>
	const Child* child_cast(void) const
	{
		ptr_type tmp = base_type::get();
		return dynamic_cast<Child*>(tmp);
	}

	template<typename Child>
	Child* child_cast(void)
	{
		ptr_type tmp = base_type::get();
		return dynamic_cast<Child*>(tmp);
	}

	ptr_type operator->(void)
	{
		return base_type::get();
	}

	ptr_type operator->(void) const
	{
		return base_type::get();
	}

	operator bool(void) const
	{
		return base_type::get();
	}

	operator base_type&(void)
	{
	    base_type& base = *this;
	    return base;
	}

	operator const base_type&(void) const
	{
	    const base_type& base = *this;
	    return base;
	}

	interface_ptr& operator=(const interface_ptr& right)
	{
        base_type& base = *this;
        base = right;
        return *this;
	}

	bool operator ==(ptr_type ptr) const
	{
		const base_type& base = *this;
		return base.get() == ptr;
	}

	bool operator== (const interface_ptr& i_ptr) const
	{
		const base_type& base = *this;
		return base == i_ptr;
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("shared_ptr",
                                        boost::serialization::base_object<base_type>(*this));
	}

//private:
	//ptr_type _ptr;
};

} //namespace yggr

#endif // __YGGR_INTERFACE_PTR_HPP__
