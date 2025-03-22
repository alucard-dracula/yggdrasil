//bson_date_time_native_ex.cpp

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

#include <yggr/nsql_database_system/bson_date_time_native_ex.hpp>
#include <boost/date_time/c_time.hpp>
#include <cstring>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
char* bson_date_time_native_ex::s_bson_date_format(const char* format, s64 msec_since_epoch, char* str, std::size_t len)
{
	assert(format && str && 64 < len);

	if(!str)
	{
		return 0;
	}

	format = format? format : bson_date_time_format::tm_fmt_iso8601_utc_org();

	char buf[64] = {0};

	s64 msecs_part = msec_since_epoch % time::time_ratio_def::E_MS_AND_MCS_RATIO;
	std::time_t t = static_cast<time_t>(msec_since_epoch / time::time_ratio_def::E_MS_AND_MCS_RATIO);
	std::tm tm = {0};

	std::strftime(buf, sizeof(buf), format, boost::date_time::c_time::gmtime(&t, &tm));

	if(msecs_part) 
	{
#if defined(_DEBUG)
		s32 spf_len = 
#endif // defined(_DEBUG)
		std::sprintf(str, "%s.%03" PRId64 "Z", buf, msecs_part);

#if defined(_DEBUG)
		assert(0 <= spf_len && spf_len < len);
#endif // defined(_DEBUG)
	} 
	else 
	{
		std::size_t buf_size = strlen(buf);
		strcpy(str, buf);
		str[buf_size] = 'Z';
		str[buf_size + 1] = 0;
	}

	return str;
}

/*static*/ 
bson_string_t* bson_date_time_native_ex::s_bson_date_format(const char* format, s64 msec_since_epoch, bson_string_t* str)
{
	assert(format && str);

	if(!str)
	{
		return 0;
	}

	format = format? format : bson_date_time_format::tm_fmt_iso8601_utc_org();

	char buf[64] = {0};

	s64 msecs_part = msec_since_epoch % time::time_ratio_def::E_MS_AND_MCS_RATIO;
	std::time_t t = static_cast<time_t>(msec_since_epoch / time::time_ratio_def::E_MS_AND_MCS_RATIO);
	std::tm tm = {0};

	std::strftime(buf, sizeof(buf), format, boost::date_time::c_time::gmtime(&t, &tm));

	if (msecs_part) 
	{
		bson_string_append_printf(str, "%s.%03" PRId64 "Z", buf, msecs_part);
	} 
	else 
	{
		bson_string_append(str, buf);
		bson_string_append_c(str, 'Z');
	}

	return str;
}

} // namespace nsql_database_system
} // namespace yggr

