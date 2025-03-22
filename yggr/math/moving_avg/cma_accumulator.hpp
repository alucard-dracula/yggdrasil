//cma_accumulator.hpp

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

#ifndef __YGGR_MATH_MOVING_AVG_CMA_ACCUMULATOR_HPP__
#define __YGGR_MATH_MOVING_AVG_CMA_ACCUMULATOR_HPP__

#include <yggr/tuple/tuple.hpp>

#include <yggr/math/moving_avg/moving_avg_cfg.hpp>
#include <yggr/math/moving_avg/moving_collector.hpp>

namespace yggr
{
namespace math
{
namespace moving_avg
{

template<typename Collector>
struct cma_accumulator
{
public:
	typedef Collector collector_type;
	typedef typename collector_type::value_type value_type;

	typedef 
		yggr::tuple
		<
			value_type, // CMA today 
			value_type, // CMA yesterday
			value_type, // p(n)
			std::pair<value_type, bool> // p(n - 1 - step + 1) // old p1
		> result_type;

private:
	typedef cma_accumulator this_type;

public:
	inline result_type operator()(collector_type& collector, 
									value_type cma_yesterday, 
									value_type pn) const
	{
		value_type cma_today = value_type();
		std::pair<value_type, bool> rst = collector << pn;
		assert(collector.size() > 0);

		cma_today = 
			rst.second?
				cma_yesterday + (pn - cma_yesterday) 
					/ static_cast<value_type>(collector.size())
				: (cma_yesterday * (collector.size() - 1) + pn) 
					/ static_cast<value_type>(collector.size());

		return yggr::make_tuple(cma_today, cma_yesterday, pn, rst);
	}
};


} // namespace moving_avg
} // namespace math
} //namepsace yggr

#endif // __YGGR_MATH_MOVING_AVG_CMA_ACCUMULATOR_HPP__
