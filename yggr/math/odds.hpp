//odds.hpp

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

#ifndef __YGGR_MATH_ODDS_HPP__
#define __YGGR_MATH_ODDS_HPP__

#include <yggr/type_traits/upper_types.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_unsigned.hpp>

#include <cassert>
#include <complex>

namespace yggr
{
namespace math
{

template<typename T> inline
T odds_to_floating_odds(const T& t)
{
	BOOST_MPL_ASSERT((boost::is_floating_point<T>));
	return t + T(1);
}

template<typename T> inline
T floating_odds_to_odds(const T& t)
{
	BOOST_MPL_ASSERT((boost::is_floating_point<T>));
	return t - T(1);
}

template<typename T, bool bchk = boost::is_floating_point<T>::value>
struct odds_to_winning_helper;

template<typename T>
struct odds_to_winning_helper<T, true >
{
	typedef T ret_type;

	inline ret_type operator()(const T& odds) const
	{
		return T(1) / odds_to_floating_odds(odds);
	}
};

template<typename T>
struct odds_to_winning_helper<T, false >
{
	typedef typename upper_float<T>::type ret_type;

	inline ret_type operator()(const T& odds) const
	{
		BOOST_MPL_ASSERT((boost::is_integral<T>));
		BOOST_MPL_ASSERT((boost::is_same<
											typename boost::is_unsigned<T>::type,
											boost::is_unsigned<int>::type
										>));
		assert( std::abs(odds) != 0 && 0 == std::abs(odds) % 100);

		ret_type num = odds < 0? ret_type(-100) : odds;
		ret_type den = odds < 0? odds : ret_type(100);

		return T(1) / odds_to_floating_odds(num / den);
	}
};

template<typename T> inline
typename upper_float<T>::type odds_to_winning(const T& odds)
{
	typedef odds_to_winning_helper<T> now_helper_type;
	now_helper_type helper;
	return helper(odds);
}

template<typename T> inline
typename upper_float<T>::type odds_to_winning(const T& odds_num, const T& odds_den)
{
	typedef typename upper_float<T>::type ret_type;
	assert(odds_den);
	return odds_den? odds_to_winning(ret_type(odds_num) / ret_type(odds_den)) : ret_type(1);
}

template<typename T> inline
typename upper_float<T>::type odds_to_failure_rate(const T& odds)
{
	typedef typename upper_float<T>::type ret_type;

	return ret_type(1) - odds_to_winning(odds);
}

template<typename T> inline
typename upper_float<T>::type odds_to_failure_rate(const T& odds_num, const T& odds_den)
{
	typedef typename upper_float<T>::type ret_type;

	return ret_type(1) - odds_to_winning(ret_type(odds_num) / ret_type(odds_den));
}

template<typename T> inline
T winning_to_odds(const T& winning)
{
	BOOST_MPL_ASSERT((boost::is_floating_point<T>));
	return winning? floating_odds_to_odds(T(1) / winning) : T(0);
}

template<typename T> inline
T winning_to_floating_odds(const T& winning)
{
	BOOST_MPL_ASSERT((boost::is_floating_point<T>));
	return winning? T(1) / winning : T(1);
}

template<typename T> inline
T failure_rate_to_odds(const T& failure_rate)
{
	BOOST_MPL_ASSERT((boost::is_floating_point<T>));
	T winning = T(1) - failure_rate;
	return winning? floating_odds_to_odds(T(1) / winning) : T(0);
}

template<typename T> inline
T failure_rate_to_floating_odds(const T& failure_rate)
{
	BOOST_MPL_ASSERT((boost::is_floating_point<T>));
	T winning = T(1) - failure_rate;
	assert(winning);
	return winning? T(1) / winning : T(1);
}

template<typename T> inline
typename upper_float<T>::type single_lose_odds(const T& odds)
{
	typedef typename yggr::upper_float<T>::type ret_type;
	BOOST_MPL_ASSERT((boost::is_floating_point<T>));
	return odds_to_floating_odds(ret_type(odds)) / ret_type(odds);
}

template<typename T> inline
typename upper_integer<T>::type
	limit_continuous_lose_count(const T& play_count, typename upper_float<T>::type& odds)
{
	typedef typename upper_float<T>::type float_type;
	return (std::log(static_cast<float_type>(play_count)) / std::log(single_lose_odds(odds)) + float_type(0.5));
}


} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_ODDS_HPP__
