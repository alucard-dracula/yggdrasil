//bson_date_time_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_DATA_TIME_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_DATA_TIME_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/time/time_config.hpp>
#include <yggr/time/time_ratio_def.hpp>
#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/bson_date_time_format.hpp>

#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>

#include <boost/thread/xtime.hpp>

namespace yggr
{
namespace nsql_database_system
{

class bson_date_time_native_ex
{
private:
	typedef bson_date_time_native_ex this_type;

public:
	static char* s_bson_date_format(const char* format, s64 msec_since_epoch, char* str, std::size_t len);
	static bson_string_t* s_bson_date_format(const char* format, s64 msec_since_epoch, bson_string_t* str);

public:
	inline static char* s_bson_date_format_iso8601(s64 msec_since_epoch, char* str, std::size_t len)
	{
		return this_type::s_bson_date_format(bson_date_time_format::tm_fmt_iso8601_utc_org(), msec_since_epoch, str, len);
	}

	inline static bson_string_t* s_bson_date_format_iso8601(s64 msec_since_epoch, bson_string_t* str)
	{
		return this_type::s_bson_date_format(bson_date_time_format::tm_fmt_iso8601_utc_org(), msec_since_epoch, str);
	}

public:
	inline static s64 date_to_msec_since_epoch(s64 date_val)
	{
		return date_val;
	}

	inline static s64 time_to_msec_since_epoch(s64 time_val)
	{
		return time_val * time::time_ratio_def::E_S_AND_MS_RATIO;
	}

	inline static s64 timeval_to_msec_since_epoch(s64 tv_sec, s64 tv_usec)
	{
		return 
			tv_sec * time::time_ratio_def::E_S_AND_MS_RATIO
			+ tv_usec / time::time_ratio_def::E_MS_AND_MCS_RATIO;
	}

public:
	// date - date
	// compare_eq
	inline static bool date_date_compare_eq(s64 date1, s64 date2)
	{
		return date1 == date2;
	}

	// compare
	inline static s32 date_date_compare(s64 date1, s64 date2)
	{
		return date1 == date2? 0 : date1 < date2 ? -1 : 1;
	}

	// date - time
	// compare_eq
	inline static bool date_time_compare_eq(s64 date, s64 tm)
	{
		return this_type::date_date_compare_eq(date, tm * time::time_ratio_def::E_S_AND_MS_RATIO);
	}

	// compare
	inline static s32 date_time_compare(s64 date, s64 tm)
	{
		return this_type::date_date_compare(date, tm * time::time_ratio_def::E_S_AND_MS_RATIO);
	}

	// date - timeval
	// compare_eq
	inline static bool date_timeval_compare_eq(s64 date, s64 tv_sec, s64 tv_usec)
	{
		return 
			this_type::date_date_compare_eq(
				date,
				(static_cast<s64>(tv_sec) * time::time_ratio_def::E_S_AND_MS_RATIO
				+ static_cast<s64>(tv_usec) / time::time_ratio_def::E_MS_AND_MCS_RATIO) );
	}

	// compare
	inline static s32 date_timeval_compare(s64 date, s64 tv_sec, s64 tv_usec)
	{
		return 
			this_type::date_date_compare(
				date,
				(static_cast<s64>(tv_sec) * time::time_ratio_def::E_S_AND_MS_RATIO
				+ static_cast<s64>(tv_usec) / time::time_ratio_def::E_MS_AND_MCS_RATIO) );
	}

	// date - xtime
	// compare_eq
	inline static bool date_xtime_compare_eq(s64 date, s64 nsec, s64 nnssec)
	{
		return 
			this_type::date_date_compare_eq(
				date,
				(static_cast<s64>(nsec) * time::time_ratio_def::E_S_AND_MS_RATIO
				+ static_cast<s64>(nnssec) / time::time_ratio_def::E_MS_AND_NS_RATIO) );
	}

	// compare
	inline static s32 date_xtime_compare(s64 date, s64 tv_sec, s64 tv_usec)
	{
		return 
			this_type::date_date_compare(
				date,
				(static_cast<s64>(tv_sec) * time::time_ratio_def::E_S_AND_MS_RATIO
				+ static_cast<s64>(tv_usec) / time::time_ratio_def::E_MS_AND_NS_RATIO) );
	}

	// time - date
	// compare_eq
	inline static bool time_date_compare_eq(s64 tm, s64 date)
	{
		return this_type::date_time_compare_eq(date, tm);
	}

	// compare
	inline static s32 time_date_compare(s64 tm, s64 date)
	{
		return -this_type::date_time_compare(date, tm);
	}

	// time - timeval
	// compare_eq
	inline static bool time_timeval_compare_eq(s64 tm, s64 tv_sec, s64 tv_usec)
	{
		return (tm == tv_sec) && (tv_usec == 0);
	}

	// compare
	inline static s32 time_timeval_compare(s64 tm, s64 tv_sec, s64 tv_usec)
	{
		return 
			date_date_compare(
				tm * time::time_ratio_def::E_S_AND_MS_RATIO,
				(static_cast<s64>(tv_sec) * time::time_ratio_def::E_S_AND_MS_RATIO
					+ static_cast<s64>(tv_usec) / time::time_ratio_def::E_MS_AND_MCS_RATIO) );
	}

	// time - xtime
	// compare_eq
	inline static bool time_xtime_compare_eq(s64 tm, s64 nsec, s64 nnssec)
	{
		return (tm == nsec) && (nnssec == 0);
	}

	// compare
	inline static s32 time_xtime_compare(s64 tm, s64 nsec, s64 nnssec)
	{
		return 
			this_type::date_date_compare(
				tm * time::time_ratio_def::E_S_AND_MS_RATIO,
				(static_cast<s64>(nsec) * time::time_ratio_def::E_S_AND_MS_RATIO
				+ static_cast<s64>(nnssec) / time::time_ratio_def::E_MS_AND_NS_RATIO) );
	}

	// timeval - date
	// compare_eq
	inline static bool timeval_date_compare_eq(s64 tv_sec, s64 tv_usec, s64 date)
	{
		return date_timeval_compare_eq(date, tv_sec, tv_usec);
	}

	// compare
	inline static s32 timeval_date_compare(s64 tv_sec, s64 tv_usec, s64 date)
	{
		return -date_timeval_compare(date, tv_sec, tv_usec);
	}

	// timeval - time
	// compare_eq
	inline static bool timeval_time_compare_eq(s64 tv_sec, s64 tv_usec, s64 date)
	{
		return time_timeval_compare_eq(date, tv_sec, tv_usec);
	}

	// compare
	inline static s32 timeval_time_compare(s64 tv_sec, s64 tv_usec, s64 date)
	{
		return -time_timeval_compare(date, tv_sec, tv_usec);
	}

	// timeval - xtime
	// compare_eq
	inline static bool timeval_xtime_compare_eq(s64 tv_sec, s64 tv_usec, s64 nsec, s64 nnssec)
	{
		return 
			this_type::date_date_compare_eq(
				(static_cast<s64>(tv_sec) * time::time_ratio_def::E_S_AND_MS_RATIO
				+ static_cast<s64>(tv_usec) / time::time_ratio_def::E_MS_AND_MCS_RATIO),
				(static_cast<s64>(nsec) * time::time_ratio_def::E_S_AND_MS_RATIO
				+ static_cast<s64>(nnssec) / time::time_ratio_def::E_MS_AND_NS_RATIO) );
	}

	// compare
	inline static s32 timeval_xtime_compare(s64 tv_sec, s64 tv_usec, s64 nsec, s64 nnssec)
	{
		return 
			this_type::date_date_compare(
				(static_cast<s64>(tv_sec) * time::time_ratio_def::E_S_AND_MS_RATIO
				+ static_cast<s64>(tv_usec) / time::time_ratio_def::E_MS_AND_MCS_RATIO),
				(static_cast<s64>(nsec) * time::time_ratio_def::E_S_AND_MS_RATIO
				+ static_cast<s64>(nnssec) / time::time_ratio_def::E_MS_AND_NS_RATIO) );
	}

	// xtime - date
	// compare_eq
	inline static bool xtime_date_compare_eq(s64 nsec, s64 nnssec, s64 date)
	{
		return date_xtime_compare_eq(date, nsec, nnssec);
	}

	// compare
	inline static s32 xtime_date_compare(s64 nsec, s64 nnssec, s64 date)
	{
		return -date_xtime_compare(date, nsec, nnssec);
	}

	// xtime - time
	// compare_eq
	inline static bool xtime_time_compare_eq(s64 nsec, s64 nnssec, s64 date)
	{
		return time_xtime_compare_eq(date, nsec, nnssec);
	}

	// compare
	inline static s32 xtime_time_compare(s64 nsec, s64 nnssec, s64 date)
	{
		return -time_xtime_compare(date, nsec, nnssec);
	}

	// xtime - timeval
	// compare_eq
	inline static bool xtime_timeval_compare_eq(s64 nsec, s64 nnssec, s64 tv_sec, s64 tv_usec)
	{
		return timeval_xtime_compare_eq(tv_sec, tv_usec, nsec, nnssec);
	}

	// compare
	inline static s32 xtime_timeval_compare(s64 nsec, s64 nnssec, s64 tv_sec, s64 tv_usec)
	{
		return -timeval_xtime_compare(tv_sec, tv_usec, nsec, nnssec);
	}
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_DATA_TIME_NATIVE_EX_HPP__
