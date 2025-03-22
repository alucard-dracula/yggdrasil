//to_pointer.hpp

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

#ifndef __YGGR_UTILITY_TO_POINTER_HPP__
#define __YGGR_UTILITY_TO_POINTER_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/is_pointer.hpp>

namespace yggr
{
namespace utility
{

namespace detail
{

template<typename T, bool is_ptr = boost::is_pointer<T>::value >
struct to_pointer_helper;

template<typename T>
struct to_pointer_helper<T, true>
{
public:
	typedef T type;
public:
	inline type conv(type t) const
	{
		return t;
	}

	template<typename R> inline
	typename boost::enable_if<boost::is_same<type, R>, type>::type
		conv(type t) const
	{
		return t;
	}

	template<typename R> inline
	typename boost::disable_if<boost::is_same<type, R>, R*>::type
		conv(type t) const
	{
		return reinterpret_cast<R*>(t);
	}
};

template<typename T>
struct to_pointer_helper<T, false>
{
public:
	typedef T type;

public:
	inline type* conv(type& t) const
	{
		return boost::addressof(t);
	}

	template<typename R> inline
	typename boost::enable_if<boost::is_same<type, R>, type*>::type
		conv(type& t) const
	{
		return boost::addressof(t);
	}

	template<typename R> inline
	typename boost::disable_if<boost::is_same<type, R>, R*>::type
		conv(type& t) const
	{
		return reinterpret_cast<R*>(boost::addressof(t));
	}
};

template<typename T, bool is_ptr = boost::is_pointer<T>::value>
struct to_const_pointer_helper;

template<typename T>
struct to_const_pointer_helper<T, true>
{
public:
	typedef T type;

public:
	inline const type conv(const type t) const
	{
		return t;
	}

	template<typename R> inline
	typename boost::enable_if<boost::is_same<type, R>, const type>::type
		conv(const type t) const
	{
		return t;
	}

	template<typename R> inline
	typename boost::disable_if<boost::is_same<type, R>, const R*>::type
		conv(const type t) const
	{
		return reinterpret_cast<const R*>(t);
	}
};

template<typename T>
struct to_const_pointer_helper<T, false>
{
public:
	typedef T type;

public:
	inline const type* conv(const type& t) const
	{
		return boost::addressof(t);
	}

	template<typename R> inline
	typename boost::enable_if<boost::is_same<type, R>, const type*>::type
		conv(const type& t) const
	{
		return boost::addressof(t);
	}

	template<typename R> inline
	typename boost::disable_if<boost::is_same<type, R>, const R*>::type
		conv(const type& t) const
	{
		return reinterpret_cast<const R*>(boost::addressof(t));
	}
};

} // namespace detail

// to_pointer
template<typename T> inline
T* to_pointer(T* t)
{
	return t;
}

template<typename T1, typename T2> inline
typename boost::disable_if<boost::is_same<T1, T2>, T1*>::type
	to_pointer(T2* t)
{
	return reinterpret_cast<T1*>(t);
}

template<typename T> inline
T* to_pointer(T& t)
{
	typedef detail::to_pointer_helper<T> h_type;
	h_type h;
	return h.conv(t);
}

template<typename T1, typename T2> inline
typename boost::disable_if<boost::is_same<T1, T2>, T1*>::type
	to_pointer(T2& t)
{
	typedef detail::to_pointer_helper<T2> h_type;
	h_type h;
	return h.template conv<T1>(t);
}

// to_const_pointer
template<typename T> inline
const T* to_const_pointer(const T* t)
{
	return t;
}

template<typename T1, typename T2> inline
typename boost::disable_if<boost::is_same<T1, T2>, const T1*>::type
	to_const_pointer(const T2* t)
{
	return reinterpret_cast<const T1*>(t);
}

template<typename T> inline
const T* to_const_pointer(const T& t)
{
	typedef detail::to_const_pointer_helper<T> h_type;
	h_type h;
	return h.conv(t);
}

template<typename T1, typename T2> inline
typename boost::disable_if<boost::is_same<T1, T2>, const T1*>::type
	to_const_pointer(const T2& t)
{
	typedef detail::to_const_pointer_helper<T2> h_type;
	h_type h;
	return h.template conv<T1>(t);
}


} // namespace utility
} // namespace yggr

#endif // __YGGR_UTILITY_TO_POINTER_HPP__
