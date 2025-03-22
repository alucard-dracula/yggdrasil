//odd_even_number.hpp

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

#ifndef __YGGR_MATH_ODD_EVENT_NUMBER_HPP__
#define __YGGR_MATH_ODD_EVENT_NUMBER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/upper_types.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace yggr
{
namespace math
{
namespace detail
{

// odd
template<typename T, bool is_integral>
struct odd_checker_impl;

template<typename T>
struct odd_checker_impl<T, false>
	: public boost::reference_wrapper<T const>
{
public:
	typedef T value_type;
	typedef boost::reference_wrapper<T const> base_type;

public:
	odd_checker_impl(const value_type& t)
		: base_type(t)
	{
	}

public:
	template<typename R> inline
	operator R(void) const
	{
		typedef typename upper_integer<value_type>::type now_int_type;
		typedef boost::mpl::integral_c<now_int_type, 1> now_eins_type;
		return
			base_type::get() - static_cast<now_int_type>(base_type::get())?
			R()
			: R(static_cast<now_int_type>(base_type::get()) & now_eins_type::value);
	}
};

template<typename T>
struct odd_checker_impl<T, true>
	: public boost::reference_wrapper<T const>
{
public:
	typedef T value_type;
	typedef boost::reference_wrapper<T const> base_type;

public:
	odd_checker_impl(const value_type& t)
		: base_type(t)
	{
	}

	template<typename R> inline
	operator R(void) const
	{
		typedef boost::mpl::integral_c<value_type, 1> now_eins_type;

		return R(base_type::get() & now_eins_type::value);
	}
};

template<typename T, T N>
struct odd_t_checker_impl
	: public
		boost::mpl::if_
		<
			boost::is_integral<T>,
			boost::mpl::bool_<(N & static_cast<T>(1))>,
			boost::mpl::false_
		>::type
{
};

//even
template<typename T, bool is_integral>
struct even_checker_impl;

template<typename T>
struct even_checker_impl<T, false>
	: public boost::reference_wrapper<T const>
{
public:
	typedef T value_type;
	typedef boost::reference_wrapper<T const> base_type;

public:
	even_checker_impl(const value_type& t)
		: base_type(t)
	{
	}

public:
	template<typename R> inline
	operator R(void) const
	{
		typedef typename upper_integer<value_type>::type now_int_type;
		typedef boost::mpl::integral_c<now_int_type, 1> now_eins_type;

		return
			base_type::get() - static_cast<now_int_type>(base_type::get())?
			R()
			: R(now_eins_type::value - (static_cast<now_int_type>(base_type::get()) & now_eins_type::value));
	}
};

template<typename T>
struct even_checker_impl<T, true>
	: public boost::reference_wrapper<T const>
{
public:
	typedef T value_type;
	typedef boost::reference_wrapper<T const> base_type;

public:
	even_checker_impl(const value_type& t)
		: base_type(t)
	{
	}

public:
	template<typename R> inline
	operator R(void) const
	{
		typedef boost::mpl::integral_c<value_type, 1> now_eins_type;
		return R(now_eins_type::value - (base_type::get() & now_eins_type::value));
	}
};


template<typename T, T N>
struct even_t_checker_impl
	: public
		boost::mpl::if_
		<
			boost::is_integral<T>,
			boost::mpl::bool_<(1 - (N & static_cast<T>(1)))>,
			boost::mpl::false_
		>::type
{
};

} // namespace detail

// odd
template<typename T>
struct odd_checker
	: public detail::odd_checker_impl<T, boost::is_integral<T>::value>
{
public:
	typedef T value_type;
	typedef detail::odd_checker_impl<T, boost::is_integral<T>::value> base_type;

public:
	odd_checker(const value_type& t)
		: base_type(t)
	{
	}
};

template<typename T, T N>
struct odd_t_checker
	: public detail::odd_t_checker_impl<T, N>
{
};

// even
template<typename T>
struct even_checker
	: public detail::even_checker_impl<T, boost::is_integral<T>::value>
{
public:
	typedef T value_type;
	typedef detail::even_checker_impl<T, boost::is_integral<T>::value> base_type;

public:
	even_checker(const value_type& t)
		: base_type(t)
	{
	}
};

template<typename T, T N>
struct even_t_checker
	: public detail::even_t_checker_impl<T, N>
{
};


// odd
template<typename T> inline
bool is_odd_number(const T& val)
{
	typedef odd_checker<T> checker_type;
	checker_type chk(val);
	return chk;
}

template<typename R, typename T> inline
R is_odd_number(const T& val)
{
	typedef odd_checker<T> checker_type;
	checker_type chk(val);
	return chk;
}

template<typename T, T N>
struct is_odd_number_t
	: public odd_t_checker<T, N>
{
};


// even
template<typename T> inline
bool is_even_number(const T& val)
{
	typedef even_checker<T> checker_type;
	checker_type chk(val);
	return chk;
}

template<typename R, typename T> inline
R is_even_number(const T& val)
{
	typedef even_checker<T> checker_type;
	checker_type chk(val);
	return chk;
}

template<typename T, T N>
struct is_even_number_t
	: public even_t_checker<T, N>
{
};

} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_ODD_EVENT_NUMBER_HPP__
