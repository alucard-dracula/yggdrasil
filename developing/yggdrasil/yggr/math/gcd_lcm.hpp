//gcd_lcm.hpp

#ifndef __YGGR_MATH_GCD_LCM_HPP__
#define __YGGR_MATH_GCD_LCM_HPP__

#include <yggr/support/max_min_undef.ipp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <numeric>

namespace yggr
{
namespace math
{

template<typename T>
T gcd(T num1, T num2)
{
	BOOST_MPL_ASSERT((boost::is_integral<T>));
	if(num1 < T() || num2 < T())
	{
		return T();
	}

	T dividend = std::max(num1, num2);
	T divisor = std::min(num1, num2);
	T residue_num = T();

	if(!divisor)
	{
		return T();
	}

	do
	{
		residue_num = dividend % divisor;
		dividend = divisor;
		divisor = residue_num;
	}while(residue_num);

	return dividend;
}

template<typename T>
T lcm(T num1, T num2)
{
	T num_gcd = gcd(num1, num2);

	return num_gcd? num1 * num2 / num_gcd : T();
}

} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_GCD_LCM_HPP__