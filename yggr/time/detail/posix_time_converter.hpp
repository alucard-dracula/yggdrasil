// posix_time_converter.hpp

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

#ifndef __YGGR_TIME_DETAIL_POSIX_TIME_CONVERTER_HPP__
#define __YGGR_TIME_DETAIL_POSIX_TIME_CONVERTER_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

namespace yggr
{
namespace time
{
namespace detail
{

struct posix_time_converter
{
private:
	typedef posix_time_converter this_type;

public:
	inline static std::tm& time_t_to_tm(std::tm& out_tm, std::time_t t)
	{
		return (out_tm = boost::posix_time::to_tm(boost::posix_time::from_time_t(t)));
	}

	inline static std::tm time_t_to_tm(std::time_t t)
	{
		std::tm out_tm = {0};
		time_t_to_tm(out_tm, t);
		return out_tm;
	}

	inline static std::time_t tm_to_time_t(const std::tm& in_tm)
	{
	
		return 
			(boost::posix_time::ptime_from_tm(in_tm) 
				- boost::posix_time::ptime(boost::gregorian::date(1970,1,1)))
			.total_seconds();
	}
};

} // namespace detail
} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_DETAIL_POSIX_TIME_CONVERTER_HPP__
