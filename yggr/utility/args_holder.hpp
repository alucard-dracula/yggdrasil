//args_holder.hpp

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

#ifndef __YGGR_UTILITY_ARGS_HOLDER_HPP__
#define __YGGR_UTILITY_ARGS_HOLDER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <boost/ref.hpp>

#include <typeinfo> // std::bad_cast
#include <cassert>

namespace yggr
{
namespace utility
{

// bad_args_holder_cast

class bad_args_holder_cast
	: public std::bad_cast
{
public:
	virtual const char* what(void) const BOOST_NOEXCEPT_OR_NOTHROW //throw()
	{
		return "yggr::bad_args_holder_cast: "
				"failed conversion using yggr::utility::args_holder_ref_wrap_dynamic_cast_throw_if_nil";
	}
};


// basic_args_holder
// storealbe, transable
class basic_args_holder
{
private:
	typedef basic_args_holder this_type;

public:
	basic_args_holder(void)
	{
	}

	virtual ~basic_args_holder(void)
	{
	}
};

// args_holder_ref_wrap

template<typename T>
class args_holder_ref_wrap
	: public basic_args_holder,
		public boost::reference_wrapper<T>
{
public:
	typedef T value_type;
	typedef basic_args_holder base_holder_type;
	typedef boost::reference_wrapper<value_type> base_wrapper_type;

private:
	typedef args_holder_ref_wrap this_type;

public:
	explicit args_holder_ref_wrap(value_type& val)
		: base_wrapper_type(val)
	{
	}

	virtual ~args_holder_ref_wrap(void)
	{
	}

public:
	using base_wrapper_type::get_pointer;
};

template<>
class args_holder_ref_wrap<void>
	: public basic_args_holder
{
public:
	typedef void value_type;
	typedef basic_args_holder base_holder_type;

private:
	typedef args_holder_ref_wrap this_type;

public:
	args_holder_ref_wrap(void)
	{
	}

	template<typename T>
	explicit args_holder_ref_wrap(const T&)
	{
	}

	virtual ~args_holder_ref_wrap(void)
	{
	}

public:
	inline void* get_pointer(void) const
	{
		return 0;
	}
};

template<typename T> inline
basic_args_holder& args_holder_trans(args_holder_ref_wrap<T>& holder)
{
	return static_cast<basic_args_holder&>(holder);
}

template<typename T> inline
const basic_args_holder& args_holder_trans(const args_holder_ref_wrap<T>& holder)
{
	return static_cast<const basic_args_holder&>(holder);
}

template<typename T> inline
T* args_holder_ref_wrap_dynamic_cast(const basic_args_holder& holder)
{
	typedef args_holder_ref_wrap<T> args_holder_type;
	const args_holder_type* p = dynamic_cast<const args_holder_type*>(boost::addressof(holder));
	return p? p->get_pointer() : 0;
}

template<typename T> inline
T* args_holder_ref_wrap_dynamic_cast_throw_if_nil(const basic_args_holder& holder)
{
	typedef args_holder_ref_wrap<T> args_holder_type;
	const args_holder_type* p = dynamic_cast<const args_holder_type*>(boost::addressof(holder));
	if(!p)
	{
		throw bad_args_holder_cast();
	}

	T* pval = p->get_pointer();

	if(!pval)
	{
		throw bad_args_holder_cast();
	}

	return pval;
}

template<typename T> inline
bool args_holder_ref_wrap_check(const basic_args_holder& holder)
{
	typedef args_holder_ref_wrap<T> args_holder_type;
	return !!dynamic_cast<const args_holder_type*>(boost::addressof(holder));
}

template<typename T> inline
T* args_holder_ref_wrap_static_cast(const basic_args_holder& holder)
{
	typedef args_holder_ref_wrap<T> args_holder_type;
	assert((args_holder_ref_wrap_check<T>(holder)));
	const args_holder_type* p = reinterpret_cast<const args_holder_type*>(boost::addressof(holder));
	assert(p);
	return p? p->get_pointer() : 0;
}

// args_holder

template<typename T>
class args_holder
	: public basic_args_holder
{
public:
	typedef T value_type;
	typedef basic_args_holder base_holder_type;

private:
	typedef args_holder this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	explicit args_holder(BOOST_RV_REF(value_type) val)
		: _val(boost::move(val))
	{
	}

	explicit args_holder(const value_type& val)
		: _val(val)
	{
	}

	args_holder(BOOST_RV_REF(this_type) right)
		: _val(boost::move(right._val))
	{
	}

	args_holder(const this_type& right)
		: _val(right._val)
	{
	}


	virtual ~args_holder(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(_val, boost::move(right_ref._val));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_val = right._val;
		return *this;
	}

	// don't need swap !!!

public:
	inline value_type* get_pointer(void)
	{
		return boost::addressof(_val);
	}

	inline const value_type* get_pointer(void) const
	{
		return boost::addressof(_val);
	}

private:
	value_type _val;
};

template<>
class args_holder<void>
	: public basic_args_holder
{
public:
	typedef void value_type;
	typedef basic_args_holder base_holder_type;

private:
	typedef args_holder this_type;

public:
	args_holder(void)
	{
	}

	template<typename T>
	explicit args_holder(const T&)
	{
	}

	virtual ~args_holder(void)
	{
	}

public:

	inline void* get_pointer(void) const
	{
		return 0;
	}
};

template<typename T> inline
basic_args_holder& args_holder_trans(args_holder<T>& holder)
{
	return static_cast<basic_args_holder&>(holder);
}

template<typename T> inline
const basic_args_holder& args_holder_trans(const args_holder<T>& holder)
{
	return static_cast<const basic_args_holder&>(holder);
}

template<typename T> inline
T* args_holder_dynamic_cast(basic_args_holder& holder)
{
	typedef args_holder<T> args_holder_type;
	args_holder_type* p = dynamic_cast<args_holder_type*>(boost::addressof(holder));
	return p? p->get_pointer() : 0;
}

template<typename T> inline
const T* args_holder_dynamic_cast(const basic_args_holder& holder)
{
	typedef args_holder<T> args_holder_type;
	const args_holder_type* p = dynamic_cast<const args_holder_type*>(boost::addressof(holder));
	return p? p->get_pointer() : 0;
}

template<typename T> inline
T* args_holder_dynamic_cast_throw_if_nil(basic_args_holder& holder)
{
	typedef args_holder<T> args_holder_type;
	args_holder_type* p = dynamic_cast<args_holder_type*>(boost::addressof(holder));
	if(!p)
	{
		throw bad_args_holder_cast();
	}

	T* pval = p->get_pointer();

	if(!pval)
	{
		throw bad_args_holder_cast();
	}

	return pval;
}

template<typename T> inline
const T* args_holder_dynamic_cast_throw_if_nil(const basic_args_holder& holder)
{
	typedef args_holder<T> args_holder_type;
	const args_holder_type* p = dynamic_cast<const args_holder_type*>(boost::addressof(holder));
	if(!p)
	{
		throw bad_args_holder_cast();
	}

	const T* pval = p->get_pointer();

	if(!pval)
	{
		throw bad_args_holder_cast();
	}

	return pval;
}

template<typename T> inline
bool args_holder_check(const basic_args_holder& holder)
{
	typedef args_holder<T> args_holder_type;
	return !!dynamic_cast<const args_holder_type*>(boost::addressof(holder));
}

template<typename T> inline
T* args_holder_static_cast(basic_args_holder& holder)
{
	typedef args_holder<T> args_holder_type;
	assert((args_holder_check<T>(holder)));
	args_holder_type* p = reinterpret_cast<args_holder_type*>(boost::addressof(holder));
	assert(p);
	return p? p->get_pointer() : 0;
}


template<typename T> inline
const T* args_holder_static_cast(const basic_args_holder& holder)
{
	typedef args_holder<T> args_holder_type;
	assert((args_holder_check<T>(holder)));
	const args_holder_type* p = reinterpret_cast<const args_holder_type*>(boost::addressof(holder));
	assert(p);
	return p? p->get_pointer() : 0;
}


} // namespace utility
} // namespace yggr

#endif // __YGGR_UTILITY_ARGS_HOLDER_HPP__
