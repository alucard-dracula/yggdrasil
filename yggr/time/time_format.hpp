//time_format.hpp

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

#ifndef __YGGR_TIME_TIME_FORMAT_HPP__
#define __YGGR_TIME_TIME_FORMAT_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/mpl/string.hpp>
#include <boost/preprocessor/cat.hpp>


// !!! time format escape characters use the standard of boost::date_time::time_facet !!!
namespace yggr
{
namespace time
{
namespace detail
{

#define YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C( __fmt__ ) \
	YGGR_CONSTEXPR_OR_INLINE operator const char*(void) const YGGR_NOEXCEPT_OR_NOTHROW { \
			return __fmt__; }

#if !defined(YGGR_NO_CWCHAR)
#	define YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_WC( __fmt__ ) \
		YGGR_CONSTEXPR_OR_INLINE operator const wchar_t*(void) const YGGR_NOEXCEPT_OR_NOTHROW { \
				return BOOST_PP_CAT(L, __fmt__); }
#else
#	define YGGR_TMP_PP_MAKE_STR_TM_FMT_OPERATOR_WC( __fmt__ )
#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)
#define YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C8( __fmt__ ) \
	YGGR_CONSTEXPR_OR_INLINE operator const c8*(void) const YGGR_NOEXCEPT_OR_NOTHROW { \
			return BOOST_PP_CAT(u8, __fmt__); }
#else
#	define YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C8( __fmt__ )
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
#define YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C16( __fmt__ ) \
	YGGR_CONSTEXPR_OR_INLINE operator const c16*(void) const YGGR_NOEXCEPT_OR_NOTHROW { \
			return BOOST_PP_CAT(u, __fmt__); }
#else
#	define YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C16( __fmt__ )
#endif // YGGR_NO_CHAR16_T


#if !defined(YGGR_NO_CHAR32_T)
#define YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C32( __fmt__ ) \
	YGGR_CONSTEXPR_OR_INLINE operator const c32*(void) const YGGR_NOEXCEPT_OR_NOTHROW { \
			return BOOST_PP_CAT(U, __fmt__); }
#else
#	define YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C32( __fmt__ )
#endif // YGGR_NO_CHAR32_T


#define YGGR_PP_MAKE_STR_TM_FMT(__name__, __fmt__) \
	struct __name__ { \
	public: \
		YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C(__fmt__) \
		YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_WC(__fmt__) \
		YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C8(__fmt__) \
		YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C16(__fmt__) \
		YGGR_PP_MAKE_STR_TM_FMT_OPERATOR_C32(__fmt__) };


YGGR_PP_MAKE_STR_TM_FMT(str_tm_fmt_default, "%Y%m%d%H%M%S")
YGGR_PP_MAKE_STR_TM_FMT(str_tm_utc_fmt_iso8601_default, "%Y-%m-%dT%H:%M:%S%F%Q")

} // namespace detail

struct time_format
{
public:
	typedef boost::mpl::string<'%Y-', '%m-', '%dT', '%H:', '%M:', '%S', '%F%Q'> tm_utc_fmt_iso8601_type;
	typedef boost::mpl::string<'%Y%m', '%d%H', '%M%S'> tm_fmt_default_type;

public:
	template<typename T> YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC
	const char* tm_fmt(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		typedef T tm_fmt_type;
		return boost::mpl::c_str<tm_fmt_type>::value;
	}

public:
	// tm_fmt_default
	YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC
	const char* tm_fmt_default(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC_CONST detail::str_tm_fmt_default now_fmt;
		return now_fmt;
	}

	template<typename Char> YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC
	const Char* tm_fmt_default(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC_CONST detail::str_tm_fmt_default now_fmt;
		return now_fmt;
	}

	//tm_fmt_iso8601
	YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC
	const char* tm_fmt_iso8601(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC_CONST detail::str_tm_utc_fmt_iso8601_default now_fmt;
		return now_fmt;
	}

	template<typename Char> YGGR_CXX14_STATIC_CONSTEXPR_OR_INLINE_STATIC
	const Char* tm_fmt_iso8601(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		YGGR_CXX14_CONSTEXPR_OR_STATIC_CONST detail::str_tm_utc_fmt_iso8601_default now_fmt;
		return now_fmt;
	}
};

} // namespace time
} // namespace yggr

#define YGGR_PP_TIME_FORMAT_STRING( __format_t__ ) \
	boost::mpl::c_str< __format_t__ >::value


#endif // __YGGR_TIME_TIME_FORMAT_HPP__
