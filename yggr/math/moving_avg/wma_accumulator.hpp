//wma_accumulator.hpp

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

#ifndef __YGGR_MATH_MOVING_AVG_WMA_ACCUMULATOR_HPP__
#define __YGGR_MATH_MOVING_AVG_WMA_ACCUMULATOR_HPP__

#include <yggr/base/static_constant.hpp>

#include <yggr/tuple/tuple.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/move/move.hpp>
#include <yggr/math/moving_avg/moving_avg_cfg.hpp>
#include <yggr/math/moving_avg/moving_collector.hpp>

namespace yggr
{
namespace math
{
namespace moving_avg
{

template<typename Collector>
struct wma_accumulator
{
public:
	typedef Collector collector_type;
	typedef typename collector_type::value_type value_type;

	typedef 
		yggr::tuple
		<
			value_type, // MMA today 
			value_type, // MMA yesterday
			value_type, // p(n)
			std::pair<value_type, bool> // p(n - 1 - step + 1) // old p1
		> result_type;

private:
	BOOST_MPL_ASSERT((is_moving_collector<collector_type>));

	YGGR_STATIC_CONSTANT(s32, E_saturated_M = collector_type::E_step * (collector_type::E_step + 1));
	YGGR_STATIC_CONSTANT(s32, E_saturated_D = 2);

private:
	typedef wma_accumulator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	wma_accumulator(void)
		: _total(), _numerator()
	{
	}

	wma_accumulator(BOOST_RV_REF(this_type) right)
		: _total(right._total), _numerator(right._numerator)
	{
	}

	wma_accumulator(const this_type& right)
		: _total(right._total), _numerator(right._numerator)
	{
	}

	~wma_accumulator(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right)
		{
			return *this;
		}

		_total = right_ref._total;
		_numerator = right_ref._numerator;
		
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_total = right._total;
		_numerator = right._numerator;
		
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		::yggr::swap(_total, right._total);
		::yggr::swap(_numerator, right._numerator);
	}

public:
	inline result_type operator()(collector_type& collector, 
									value_type wma_yesterday, 
									value_type pn) const
	{
		value_type wma_today = value_type();
		std::pair<value_type, bool> rst = collector << pn;
		assert(collector.size() > 0);

		value_type m = value_type();

		if(!rst.second)
		{
			m = static_cast<value_type>(collector.size() * (collector.size() + 1));
			_numerator += pn * static_cast<value_type>(collector.size());
			_total += pn;
		}
		else
		{
			m = static_cast<value_type>(this_type::E_saturated_M);
			_numerator += pn * static_cast<value_type>(collector.size()) - _total;
			_total += pn - rst.first;
		}

		// wma_today = numerator / (m / d) = numerator * d / m 
		wma_today = _numerator * static_cast<value_type>(this_type::E_saturated_D) / m;
		return yggr::make_tuple(wma_today, wma_yesterday, pn, rst);
	}

private:
	mutable value_type _total;
	mutable value_type _numerator;
};


} // namespace moving_avg
} // namespace math
} // namepsace yggr

namespace yggr
{
namespace math
{
namespace moving_avg
{
namespace swap_support
{

template<typename Collector> inline 
void swap(wma_accumulator<Collector>& l, wma_accumulator<Collector>& r)
{ 
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace moving_avg
} // namespace math
} // namespace yggr

namespace std
{
	using ::yggr::math::moving_avg::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::math::moving_avg::swap_support::swap;
} // namespace boost

#endif // __YGGR_MATH_MOVING_AVG_WMA_ACCUMULATOR_HPP__
