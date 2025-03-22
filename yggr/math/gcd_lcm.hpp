//gcd_lcm.hpp

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

#ifndef __YGGR_MATH_GCD_LCM_HPP__
#define __YGGR_MATH_GCD_LCM_HPP__

#include <yggr/type_traits/native_t.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/func/foo_input_arg_t.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace yggr
{
namespace math
{

namespace detail
{

// gcd detail
template<typename T>
struct gcd_detail
{
private:
	typedef T input_value_type;

public:
	typedef typename native_t<input_value_type>::type type;
	typedef typename func::foo_input_arg_t<type>::type arg_type;

	type operator()(arg_type num1, arg_type num2) const
	{
		BOOST_MPL_ASSERT((boost::is_integral<type>));
		if(num1 < type() || num2 < type())
		{
			return type();
		}

		type dividend = (std::max)(num1, num2);
		type divisor = (std::min)(num1, num2);
		type residue_num = T();

		if(!divisor)
		{
			return type();
		}

		do
		{
			residue_num = dividend % divisor;
			dividend = divisor;
			divisor = residue_num;
		}while(residue_num);

		return dividend;
	}
};

} // namespace detail

template<typename T> inline
typename boost::enable_if<boost::is_class<T>, T>::type
	gcd(const T& num1, const T& num2)
{
	typedef detail::gcd_detail<T> gcd_type;
	gcd_type gcd;
	return gcd(num1, num2);
}

template<typename T> inline
typename boost::disable_if<boost::is_class<T>, T>::type
	gcd(T num1, T num2)
{
	typedef detail::gcd_detail<T> gcd_type;
	gcd_type gcd;
	return gcd(num1, num2);
}

template<typename T> inline
typename boost::enable_if<boost::is_class<T>, T>::type
	lcm(const T& num1, const T& num2)
{
	typedef detail::gcd_detail<T> gcd_type;
	gcd_type gcd;
	T num_gcd = gcd(num1, num2);

	return num_gcd? num1 * num2 / num_gcd : T();
}

template<typename T> inline
typename boost::disable_if<boost::is_class<T>, T>::type
	lcm(T num1, T num2)
{
	typedef detail::gcd_detail<T> gcd_type;
	gcd_type gcd;
	T num_gcd = gcd(num1, num2);

	return num_gcd? num1 * num2 / num_gcd : T();
}

namespace detail
{

template<int dividend, int divisor>
struct gcd_t_detail
	: public boost::mpl::int_<gcd_t_detail<divisor, dividend % divisor>::value>
{
};

template< int dividend>
struct gcd_t_detail<dividend, 0>
	: public boost::mpl::int_<dividend>
{
};

} // namespace detail

template<int dividend, int divisor>
struct gcd_t 
	: public 
		boost::mpl::int_
		<
			static_cast<int>
			(
				detail::gcd_t_detail
				<
					(dividend > divisor? dividend : divisor), 
					(dividend > divisor? divisor : dividend) 
				>::value
			)
		>
{
};

template<int dividend>
struct gcd_t<dividend, 0>
	: public
		boost::mpl::int_
		<
			static_cast<int>(0)
		>
{
};

template<int dividend, int divisor>
struct lcm_t
	: public
		boost::mpl::int_
		<
			static_cast<int>
			(
				dividend * divisor / gcd_t<dividend, divisor>::value
			)
		>
{
};

template<int dividend>
struct lcm_t<dividend, 0>
	: public
		boost::mpl::int_
		<
			static_cast<int>(0)
		>
{
};

} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_GCD_LCM_HPP__