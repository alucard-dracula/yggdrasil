// factorial.hpp

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

#ifndef __YGGR_MATH_FACTORIAL_HPP__
#define __YGGR_MATH_FACTORIAL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/native_t.hpp>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/func/foo_input_arg_t.hpp>

#include <yggr/math/flaw_comparer.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/mpl/size_t.hpp>

#include <cassert>

namespace yggr
{
namespace math
{
namespace detail
{

template<typename T>
struct factorial_detail
{
private:
	typedef T input_value_type;

public:
	typedef typename native_t<input_value_type>::type type;
	typedef typename func::foo_input_arg_t<type>::type arg_type;

	type operator()(arg_type val) const
	{
		typedef flaw_comparer<type> cmp_type;
		typedef typename upper_integer<type>::type upper_type;

		assert((cmp_type(val - upper_type(val)) == type()
				&& cmp_type(val) >= type()));

		type rst = type();
		++rst;

		if(cmp_type(val) != type())
		{
			for(type i = val; cmp_type(i) != type(); rst *= i, --i);
		}

		return rst;
	}
};

} // namespace detail

template<typename T> inline
typename boost::enable_if<boost::is_class<T>, T>::type
	factorial(const T& val)
{
	typedef detail::factorial_detail<T> factorial_type;
	factorial_type f;
	return f(val);
}

template<typename T> inline
typename boost::disable_if<boost::is_class<T>, T>::type
	factorial(T val)
{
	typedef detail::factorial_detail<T> factorial_type;
	factorial_type f;
	return f(val);
}

// meta version

namespace detail
{
template<std::size_t N>
struct factorial_t_detail
	: public boost::mpl::size_t< N * factorial_t_detail<N - 1>::value >
{
};

template<>
struct factorial_t_detail<static_cast<std::size_t>(0)>
	: public boost::mpl::size_t<static_cast<std::size_t>(1)>
{
};

template<>
struct factorial_t_detail<static_cast<std::size_t>(1)>
	: public boost::mpl::size_t<static_cast<std::size_t>(1)>
{
};

} // namespace detail

template<std::size_t N>
struct factorial_t
	: public detail::factorial_t_detail<N>
{
};

} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_FACTORIAL_HPP__
