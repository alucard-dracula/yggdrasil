// pascals_triangle.hpp

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

#ifndef __YGGR_MATH_PASCALS_TRIANGLE_HPP__
#define __YGGR_MATH_PASCALS_TRIANGLE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/native_t.hpp>
#include <yggr/func/foo_input_arg_t.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace math
{

namespace detail
{

// pascals_triangle_detail
template<typename T>
struct pascals_triangle_detail
{
private:
	typedef T input_value_type;

private:
	typedef pascals_triangle_detail this_type;

public:
	typedef typename native_t<input_value_type>::type type;
	typedef typename func::foo_input_arg_t<type>::type arg_type;

	inline type operator()(arg_type n, arg_type m) const
	{
		if(m == type() || n == m)
		{
			type t = type();
			return ++t;
		}
		else
		{
			return this_type::operator()(n - 1, m - 1) + this_type::operator()(n - 1, m);
		}
	}
};

} // namespace detail

template<typename T> inline
typename boost::enable_if<boost::is_class<T>, T>::type
	pascals_triangle(const T& n, const T& m)
{
	typedef detail::pascals_triangle_detail<T> cal_type;
	cal_type cal;
	return cal(n, m);
}

template<typename T> inline
typename boost::disable_if<boost::is_class<T>, T>::type
	pascals_triangle(T n, T m)
{
	typedef detail::pascals_triangle_detail<T> cal_type;
	cal_type cal;
	return cal(n, m);
}

// meta version
namespace detail
{

// pascals_triangle_t
template<std::size_t N, std::size_t M>
struct pascals_triangle_t
	: public boost::mpl::size_t<pascals_triangle_t<N - 1, M - 1>::value + pascals_triangle_t<N - 1, M>::value>
{
};

template<std::size_t N>
struct pascals_triangle_t<N, static_cast<std::size_t>(0)>
	: public boost::mpl::size_t<1>
{
};

template<std::size_t N>
struct pascals_triangle_t<N, N>
	: public boost::mpl::size_t<1>
{
};

template<>
struct pascals_triangle_t<static_cast<std::size_t>(0), static_cast<std::size_t>(0)>
	: public boost::mpl::size_t<1>
{
};

} // namespace detail

template<std::size_t N, std::size_t M>
struct pascals_triangle_t
	: public detail::pascals_triangle_t<N, M>
{
};

} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_PASCALS_TRIANGLE_HPP__
