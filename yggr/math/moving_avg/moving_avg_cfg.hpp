//moving_avg_cfg.hpp

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

#ifndef __YGGR_MATH_MOVING_AVG_MOVING_AVG_CFG_HPP__
#define __YGGR_MATH_MOVING_AVG_MOVING_AVG_CFG_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits/is_floating_point.hpp>

namespace yggr
{
namespace math
{
namespace moving_avg
{

struct accumulator_config
{
public:
	typedef u32 value_type;

public:
	typedef boost::mpl::integral_c<value_type, 0> t_start_type;

	typedef boost::mpl::integral_c<value_type, 0x00010000> t_user_type;

	typedef boost::mpl::integral_c<value_type, 0x7fffffff> t_end_type;

public:
	YGGR_STATIC_CONSTANT(value_type, E_type_start = t_start_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_type_user = t_user_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_type_end = t_end_type::value);
};

namespace detail
{

template<typename Val, bool is_floating = boost::is_floating_point<Val>::value >
struct correct_impl;

template<typename Val>
struct correct_impl<Val, true>
{
	typedef Val value_type;

	inline value_type correct_value(void) const
	{
		return value_type(1.0f);
	}

	inline value_type round_value(void) const
	{
		return value_type(0.0f);
	}

	inline static value_type s_correct_value(void)
	{
		return value_type(1.0f);
	}

	inline static value_type s_round_value(void)
	{
		return value_type(0.0f);
	}
};

template<typename Val>
struct correct_impl<Val, false>
{
	typedef Val value_type;

	inline value_type correct_value(void) const
	{
		return value_type(64);
	}

	inline value_type round_value(void) const
	{
		return value_type(32);
	}

	inline static value_type s_correct_value(void)
	{
		return value_type(64);
	}

	inline static value_type s_round_value(void)
	{
		return value_type(32);
	}
};

} // namespace detail

template<typename Val>
struct correct
	: public detail::correct_impl<Val>
{
private:
	typedef detail::correct_impl<Val> base_type;

public:
	typedef typename base_type::value_type value_type;

public:
	using base_type::correct_value;
	using base_type::round_value;

	using base_type::s_correct_value;
	using base_type::s_round_value;

};

} // namespace moving_avg
} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_MOVING_AVG_MOVING_AVG_CFG_HPP__
