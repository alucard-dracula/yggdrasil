//time_diff.hpp

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

#ifndef __YGGR_TIME_DETAIL_TIME_DIFF_HPP__
#define __YGGR_TIME_DETAIL_TIME_DIFF_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/date_time/c_time.hpp>

namespace yggr
{
namespace time
{
namespace detail
{

// std::timediff(calendar, utc)
inline std::time_t gen_utc_calendar_time_diff(void)
{
	std::tm tmp_tm = {0};
	std::time_t cnd_tm = std::time(0);
	if(cnd_tm == static_cast<std::time_t>(-1))
	{
		return static_cast<std::time_t>(-1);
	}

	std::tm* putc_tm = boost::date_time::c_time::gmtime(&cnd_tm, &tmp_tm);
	if(!putc_tm)
	{
		return static_cast<std::time_t>(-1);
	}

	std::time_t utc_tm = std::mktime(putc_tm);
	if(utc_tm == static_cast<std::time_t>(-1))
	{
		return static_cast<std::time_t>(-1);
	}

	return static_cast<std::time_t>(std::difftime(cnd_tm, utc_tm));
}

// std::timediff(local, utc)
inline std::time_t gen_utc_local_time_diff(void)
{
	std::tm tmp_tm = {0};
	std::time_t cnd_tm = std::time(0);
	if(cnd_tm == static_cast<std::time_t>(-1))
	{
		return static_cast<std::time_t>(-1);
	}
	
	std::tm* putc_tm = boost::date_time::c_time::gmtime(&cnd_tm, &tmp_tm);
	if(!putc_tm)
	{
		return static_cast<std::time_t>(-1);
	}

	/*	
		!!!std::mktime must be used after std::localtime or std::gmtime!!!
		!!!if run after std::tm* plcl_tm = std::localtime(&cnd_tm);!!!
		!!!putc_tm is same plcl_tm, so utm_tm == lcl_tm !!!
	*/
	std::time_t utc_tm = std::mktime(putc_tm); 
	if(utc_tm == static_cast<std::time_t>(-1))
	{
		return static_cast<std::time_t>(-1);
	}

	std::tm* plcl_tm = boost::date_time::c_time::localtime(&cnd_tm, &tmp_tm);
	if(!plcl_tm)
	{
		return static_cast<std::time_t>(-1);
	}

	std::time_t lcl_tm = std::mktime(plcl_tm);
	if(lcl_tm == static_cast<std::time_t>(-1))
	{
		return static_cast<std::time_t>(-1);
	}

	return static_cast<std::time_t>(std::difftime(lcl_tm, utc_tm));
}

// std::timediff(calendar, local)
inline std::time_t gen_local_calendar_time_diff(void)
{
	std::tm tmp_tm = {0};
	std::time_t cnd_tm = std::time(0);

	if(cnd_tm == static_cast<std::time_t>(-1))
	{
		return static_cast<std::time_t>(-1);
	}

	std::tm* plcl_tm = boost::date_time::c_time::localtime(&cnd_tm, &tmp_tm);
	if(!plcl_tm)
	{
		return static_cast<std::time_t>(-1);
	}

	std::time_t lcl_tm = std::mktime(plcl_tm);
	if(lcl_tm == static_cast<std::time_t>(-1))
	{
		return static_cast<std::time_t>(-1);
	}

	return static_cast<std::time_t>(std::difftime(lcl_tm, cnd_tm));
}

} // namespace detail
} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_DETAIL_TIME_DIFF_HPP__
