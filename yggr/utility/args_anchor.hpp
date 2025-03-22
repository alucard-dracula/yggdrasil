// args_anchor_t.hpp

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

#ifndef __YGGR_UTILITY_ARGS_ANCHOR_T_HPP__
#define __YGGR_UTILITY_ARGS_ANCHOR_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/native_t.hpp>
#include <yggr/mplex/null_t.hpp>

#include <boost/ref.hpp>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace utility
{

// normal
template<typename T = void>
struct args_anchor_t
{
public:
	typedef T type;
private:
	typedef boost::reference_wrapper<type> ref_type;
private:
	typedef args_anchor_t this_type;

public:
	args_anchor_t(type& ref)
		: _ref(ref)
	{
	}

	~args_anchor_t(void)
	{
	}

public:
	inline operator type&(void) const 
	{
		return _ref.get();
	}

    inline type& get(void) const 
	{ 
		return _ref.get(); 
	}

    inline type* get_pointer(void) const 
	{ 
		return _ref.get_pointer; 
	}

private:
	ref_type _ref;
};

// pointer
template<typename T>
struct args_anchor_t<T*>
{
public:
	typedef T* type;

private:
	typedef args_anchor_t this_type;

public:
	args_anchor_t(type ptr)
		: _ptr(ptr)
	{
	}

	~args_anchor_t(void)
	{
	}

public:

	inline operator type(void) const 
	{
		return _ptr;
	}

    inline type get(void) const 
	{ 
		return _ptr; 
	}

    inline type get_pointer(void) const 
	{ 
		return _ptr; 
	}

private:
	type _ptr;
};

// void
template<>
struct args_anchor_t<void>
{
public:
	typedef void type;

private:
	typedef mplex::null_t compatible_type;

private:
	typedef args_anchor_t this_type;

public:
	args_anchor_t(void)
	{
	}

	~args_anchor_t(void)
	{
	}

public:

	inline operator compatible_type(void) const 
	{
		return compatible_type();
	}

    inline compatible_type get(void) const 
	{ 
		return compatible_type(); 
	}

    inline type* get_pointer(void) const 
	{ 
		return static_cast<type*>(0); 
	}
};

// meta is_args_anchor

namespace detail
{

template<typename T>
struct is_args_anchor_t_detail
	: public boost::mpl::false_
{
};

template<typename T>
struct is_args_anchor_t_detail<args_anchor_t<T> >
	: public boost::mpl::true_
{
};

} // namespace detail

template<typename T>
struct is_args_anchor_t
	: public 
		detail::is_args_anchor_t_detail
		<
			typename native_t<T>::type
		>
{
};

// foo

template<typename T> inline
bool is_args_anchor(void)
{
	return is_args_anchor_t<T>::value;
}

template<typename T> inline
bool is_args_anchor(const T&)
{
	return is_args_anchor_t<T>::value;
}

# if defined( __BORLANDC__ ) && BOOST_WORKAROUND( __BORLANDC__, BOOST_TESTED_AT(0x581) )
#  define YGGR_ANCHOR_CONST
# else
#  define YGGR_ANCHOR_CONST const
# endif // __BORLANDC__

// args_anchor
inline args_anchor_t<void> YGGR_ANCHOR_CONST args_anchor(void)
{ 
    return args_anchor_t<void>();
}

template<typename T> inline 
args_anchor_t<T> YGGR_ANCHOR_CONST args_anchor(T& t)
{ 
    return args_anchor_t<T>(t);
}

// const_args_anchor
inline args_anchor_t<void> YGGR_ANCHOR_CONST const_args_anchor(void)
{ 
    return args_anchor_t<void>();
}

template<typename T> inline 
args_anchor_t<T const> YGGR_ANCHOR_CONST const_args_anchor(const T& t)
{
    return args_anchor_t<T const>(t);
}

# undef YGGR_ANCHOR_CONST

} // namespace utility

using utility::args_anchor_t;
using utility::is_args_anchor_t;

using utility::is_args_anchor;
using utility::args_anchor;
using utility::const_args_anchor;

} // namespace yggr

#endif // __YGGR_UTILITY_ARGS_ANCHOR_T_HPP__
