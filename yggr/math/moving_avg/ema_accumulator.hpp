//ema_accumulator.hpp

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

#ifndef __YGGR_MATH_MOVING_AVG_EMA_ACCUMULATOR_HPP__
#define __YGGR_MATH_MOVING_AVG_EMA_ACCUMULATOR_HPP__

#include <yggr/base/static_constant.hpp>

#include <yggr/tuple/tuple.hpp>

#include <yggr/math/moving_avg/moving_avg_cfg.hpp>
#include <yggr/math/moving_avg/moving_collector.hpp>

namespace yggr
{
namespace math
{
namespace moving_avg
{

// MD:alpha's molecular correction value
// DD:alpha's denominator correction value 
template<typename Collector, s32 MD, s32 DD>
struct ema_accumulator_N
{
public:
	YGGR_STATIC_CONSTANT(s32, E_MD = MD);
	YGGR_STATIC_CONSTANT(s32, E_DD = DD);
	
public:
	typedef Collector collector_type;
	typedef typename collector_type::value_type value_type;

private:
	typedef ema_accumulator_N this_type;

public:
	typedef 
		yggr::tuple
		<
			value_type, // EMA today 
			value_type, // EMA yesterday
			value_type, // p(n)
			std::pair<value_type, bool> // p(n - 1 - step + 1) // old p1
		> result_type;

	inline result_type operator()(collector_type& collector, 
									value_type ema_yesterday, 
									value_type pn) const
	{
		value_type ema_today = value_type();
		std::pair<value_type, bool> rst = collector << pn;
		assert(collector.size() > 0);

		ema_today 
			= ema_yesterday 
				+ ((pn - ema_yesterday) * (value_type(1) + static_cast<value_type>(E_MD)))
					/ ((static_cast<value_type>(collector.size()) + static_cast<value_type>(E_DD)));

		return yggr::make_tuple(ema_today, ema_yesterday, pn, rst);
	}
};

template<typename Collector, s32 M, s32 D>
struct ema_accumulator_alpha
{
public:
	YGGR_STATIC_CONSTANT(s32, E_M = M);
	YGGR_STATIC_CONSTANT(s32, E_D = D);
	
private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(E_D != 0)>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(E_M < E_D)>));

private:
	typedef ema_accumulator_alpha this_type;

public:
	typedef Collector collector_type;
	typedef typename collector_type::value_type value_type;

	typedef 
		yggr::tuple
		<
			value_type, // EMA today 
			value_type, // EMA yesterday
			value_type, // p(n)
			std::pair<value_type, bool> // p(n - 1 - step + 1) // old p1
		> result_type;

public:
	inline result_type operator()(collector_type& collector, 
									value_type ema_yesterday, 
									value_type pn) const
	{
		value_type ema_today = value_type();
		std::pair<value_type, bool> rst = collector << pn;
		assert(collector.size() > 0);

		ema_today = 
			rst.second?
				ema_yesterday 
					+ (pn - ema_yesterday) * static_cast<value_type>(E_M)
						/  static_cast<value_type>(E_D)
				: (ema_yesterday * (collector.size() - 1) + pn) 
					/ static_cast<value_type>(collector.size());

		return yggr::make_tuple(ema_today, ema_yesterday, pn, rst);
	}
};

} // namespace moving_avg
} // namespace math
} //namepsace yggr

#endif // __YGGR_MATH_MOVING_AVG_EMA_ACCUMULATOR_HPP__
