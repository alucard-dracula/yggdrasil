//log_change_checker.hpp

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

#ifndef __YGGR_LOG_LOG_OP_LOG_CHANGE_CHECKER_HPP__
#define __YGGR_LOG_LOG_OP_LOG_CHANGE_CHECKER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/time/time.hpp>
#include <yggr/charset/utf8_string.hpp>

namespace yggr
{
namespace log
{
namespace log_op
{

struct log_file_day_checker
{
public:
	typedef time::time time_type;

private:
	typedef log_file_day_checker this_type;

public:
	inline bool operator()(const time_type& old) const
	{
		time_type now;

		std::tm old_tm = old.to_local_tm();
		std::tm now_tm = now.to_local_tm();

		return 
			(old_tm.tm_year != now_tm.tm_year
			|| old_tm.tm_mon != now_tm.tm_mon
			|| old_tm.tm_mday != now_tm.tm_mday);
	}

	inline static const char* format(void)
	{
		return ".%Y%m%d";
	}
};

struct log_file_non_checker
{
public:
	typedef time::time time_type;

private:
	typedef log_file_non_checker this_type;

public:
	inline bool operator()(const time_type&) const
	{
		return false;
	}

	inline static const char* format(void)
	{
		return ".%Y%m%d";
	}
};

} // namespace log_op
} // namespace log
} // namespace yggr

#endif // __YGGR_LOG_LOG_OP_LOG_CHANGE_CHECKER_HPP__
