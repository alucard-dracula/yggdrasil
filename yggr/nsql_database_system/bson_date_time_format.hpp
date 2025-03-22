// bson_date_time_format.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_DATE_TIME_FORMAT_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_DATE_TIME_FORMAT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/time/time_format.hpp>

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

YGGR_PP_MAKE_STR_TM_FMT(str_bson_tm_utc_fmt_iso8601_org, "%Y-%m-%dT%H:%M:%S")

YGGR_PP_MAKE_STR_TM_FMT(str_bson_tm_utc_fmt_iso8601, "%Y-%m-%dT%H:%M:%S%FZ")
YGGR_PP_MAKE_STR_TM_FMT(str_bson_tm_local_fmt_iso8601, "%Y-%m-%dT%H:%M:%S%F")

} // namespace detail

struct bson_date_time_format
{
public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE
	static const char* tm_fmt_iso8601_utc_org(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC detail::str_bson_tm_utc_fmt_iso8601_org now_fmt;
		return now_fmt;
	}

	template<typename Char> YGGR_CXX14_CONSTEXPR_OR_INLINE
	static const Char* tm_fmt_iso8601_utc_org(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC detail::str_bson_tm_utc_fmt_iso8601_org now_fmt;
		return now_fmt;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	static const char* tm_fmt_iso8601_utc(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC detail::str_bson_tm_utc_fmt_iso8601 now_fmt;
		return now_fmt;
	}

	template<typename Char> YGGR_CXX14_CONSTEXPR_OR_INLINE
	static const Char* tm_fmt_iso8601_utc(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC detail::str_bson_tm_utc_fmt_iso8601 now_fmt;
		return now_fmt;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	static const char* tm_fmt_iso8601_local(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC detail::str_bson_tm_local_fmt_iso8601 now_fmt;
		return now_fmt;
	}

	template<typename Char> YGGR_CXX14_CONSTEXPR_OR_INLINE
	static const Char* tm_fmt_iso8601_local(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC detail::str_bson_tm_local_fmt_iso8601 now_fmt;
		return now_fmt;
	}
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_DATE_TIME_FORMAT_HPP__
