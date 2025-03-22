//args_holder_nv.hpp

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

#ifndef __YGGR_UTILITY_ARGS_HOLDER_NV_HPP__
#define __YGGR_UTILITY_ARGS_HOLDER_NV_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <boost/ref.hpp>
#include <cassert>

// nv <=> no virtual

#ifdef _DEBUG
#	define DEBUG_VIRTUAL() virtual
#else
#	define DEBUG_VIRTUAL()
#endif // _DEBUG

namespace yggr
{
namespace utility
{

// basic_args_holder_nv
// transable only
class basic_args_holder_nv
{
private:
	typedef basic_args_holder_nv this_type;

public:
	basic_args_holder_nv(void)
	{
	}

	DEBUG_VIRTUAL() ~basic_args_holder_nv(void)
	{
	}
};

// args_holder_nv_ref_wrap

template<typename T>
class args_holder_nv_ref_wrap
	: public basic_args_holder_nv,
		public boost::reference_wrapper<T>
{
public:
	typedef T value_type;
	typedef basic_args_holder_nv base_holder_type;
	typedef boost::reference_wrapper<value_type> base_wrapper_type;

private:
	typedef args_holder_nv_ref_wrap this_type;

public:
	explicit args_holder_nv_ref_wrap(value_type& val)
		: base_wrapper_type(val)
	{
	}

	DEBUG_VIRTUAL() ~args_holder_nv_ref_wrap(void)
	{
	}

public:
	using base_wrapper_type::get_pointer;
};

template<>
class args_holder_nv_ref_wrap<void>
	: public basic_args_holder_nv
{
public:
	typedef void value_type;
	typedef basic_args_holder_nv base_holder_type;

private:
	typedef args_holder_nv_ref_wrap this_type;

public:
	args_holder_nv_ref_wrap(void)
	{
	}

	template<typename T>
	explicit args_holder_nv_ref_wrap(const T&)
	{
	}

	DEBUG_VIRTUAL() ~args_holder_nv_ref_wrap(void)
	{
	}

public:
	inline void* get_pointer(void) const
	{
		return 0;
	}
};

template<typename T> inline
basic_args_holder_nv& args_holder_trans(args_holder_nv_ref_wrap<T>& holder)
{
	return static_cast<basic_args_holder_nv&>(holder);
}

template<typename T> inline
const basic_args_holder_nv& args_holder_trans(const args_holder_nv_ref_wrap<T>& holder)
{
	return static_cast<const basic_args_holder_nv&>(holder);
}

#ifdef _DEBUG

template<typename T> inline
bool args_holder_ref_wrap_check(const basic_args_holder_nv& holder)
{
	typedef args_holder_nv_ref_wrap<T> args_holder_type;
	return !!dynamic_cast<const args_holder_type*>(boost::addressof(holder));
}

#endif // _DEBUG

template<typename T> inline
T* args_holder_ref_wrap_static_cast(const basic_args_holder_nv& holder)
{
	typedef args_holder_nv_ref_wrap<T> args_holder_type;
	assert((args_holder_ref_wrap_check<T>(holder)));
	const args_holder_type* p = reinterpret_cast<const args_holder_type*>(boost::addressof(holder));
	assert(p);
	return p? p->get_pointer() : 0;
}



// args_holder_nv

template<typename T>
class args_holder_nv
	: public basic_args_holder_nv
{
public:
	typedef T value_type;
	typedef basic_args_holder_nv base_holder_type;

private:
	typedef args_holder_nv this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	explicit args_holder_nv(BOOST_RV_REF(value_type) val)
		: _val(boost::move(val))
	{
	}

	explicit args_holder_nv(const value_type& val)
		: _val(val)
	{
	}

	args_holder_nv(BOOST_RV_REF(this_type) right)
		: _val(boost::move(right._val))
	{
	}

	args_holder_nv(const this_type& right)
		: _val(right._val)
	{
	}


	DEBUG_VIRTUAL() ~args_holder_nv(void)
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
class args_holder_nv<void>
	: public basic_args_holder_nv
{
public:
	typedef void value_type;
	typedef basic_args_holder_nv base_holder_type;

private:
	typedef args_holder_nv this_type;

public:
	args_holder_nv(void)
	{
	}

	template<typename T>
	explicit args_holder_nv(const T&)
	{
	}

	DEBUG_VIRTUAL() ~args_holder_nv(void)
	{
	}

public:

	inline void* get_pointer(void) const
	{
		return 0;
	}
};

template<typename T> inline
basic_args_holder_nv& args_holder_trans(args_holder_nv<T>& holder)
{
	return static_cast<basic_args_holder_nv&>(holder);
}

template<typename T> inline
const basic_args_holder_nv& args_holder_trans(const args_holder_nv<T>& holder)
{
	return static_cast<const basic_args_holder_nv&>(holder);
}

#ifdef _DEBUG

template<typename T> inline
bool args_holder_check(const basic_args_holder_nv& holder)
{
	typedef args_holder_nv<T> args_holder_type;
	return !!dynamic_cast<const args_holder_type*>(boost::addressof(holder));
}

#endif // _DEBUG

template<typename T> inline
T* args_holder_static_cast(basic_args_holder_nv& holder)
{
	typedef args_holder_nv<T> args_holder_type;
	assert((args_holder_check<T>(holder)));
	args_holder_type* p = reinterpret_cast<args_holder_type*>(boost::addressof(holder));
	assert(p);
	return p? p->get_pointer() : 0;
}


template<typename T> inline
const T* args_holder_static_cast(const basic_args_holder_nv& holder)
{
	typedef args_holder_nv<T> args_holder_type;
	assert((args_holder_check<T>(holder)));
	const args_holder_type* p = reinterpret_cast<const args_holder_type*>(boost::addressof(holder));
	assert(p);
	return p? p->get_pointer() : 0;
}

} // namespace utility
} // namespace yggr


#ifdef DEBUG_VIRTUAL
#	undef DEBUG_VIRTUAL
#endif // DEBUG_VIRTUAL

#endif // __YGGR_UTILITY_ARGS_HOLDER_NV_HPP__
