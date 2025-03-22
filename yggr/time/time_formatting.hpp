// time_formatting.hpp

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

#ifndef __YGGR_TIME_TIME_FORMATTING_HPP__
#define __YGGR_TIME_TIME_FORMATTING_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/time/time_format.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/thread/xtime.hpp>

#include <boost/mpl/and.hpp>

// fix has_has_range_const_iterator bug in boost_156-160, see boost/range/const_iterator.hpp
#include <boost/range/as_literal.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

#include <sstream>
#include <cassert>

namespace yggr
{
namespace time
{

#define YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX( __foo_name__, __char_t__ ) \
	template<typename Traits> inline \
	std::basic_ostream< __char_t__, Traits >& \
		__foo_name__(std::basic_ostream<__char_t__, Traits>& os, const __char_t__* format, const boost::xtime& tm) { \
		return __foo_name__< __char_t__, Traits >(os, format, tm); }

// stream time formatting
template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	time_formatting_utc(std::basic_ostream<Char, Traits>& os, const Char* format, const boost::xtime& tm)
{
	typedef boost::date_time::time_facet<boost::posix_time::ptime, Char> now_time_facet_type;

	format = format? format : time_format::tm_fmt_default<Char>();
	// high version boost in win32 overloaded time_facet's new operator, so using new
	//now_time_facet_type* pfacet = yggr_nothrow_new now_time_facet_type(format);
	now_time_facet_type* pfacet = 0;
	try
	{
		pfacet = new now_time_facet_type(format);
	}
	catch(...)
	{
		pfacet = 0;
	}

	assert(pfacet);
	if(!pfacet)
	{
		return os;
	}

	boost::system_time now_tm = tm;

	std::locale old_loc(os.getloc());
	std::locale new_loc(old_loc, pfacet);

	os.imbue(new_loc);
	os << now_tm;
	os.imbue(old_loc);

	return os;
}

YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX(time_formatting_utc, char)

#if !defined(YGGR_NO_CWCHAR)
YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX(time_formatting_utc, wchar_t)
#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)
YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX(time_formatting_utc, c8)
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX(time_formatting_utc, c16)
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX(time_formatting_utc, c32)
#endif // YGGR_NO_CHAR32_T


template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	time_formatting_local(std::basic_ostream<Char, Traits>& os, const Char* format, const boost::xtime& tm)
{
	typedef boost::date_time::time_facet<boost::posix_time::ptime, Char> now_time_facet_type;
	typedef boost::date_time::c_local_adjustor<boost::system_time> local_adj;

	format = format? format : time_format::tm_fmt_default<Char>();
	// high version boost in win32 overloaded time_facet's new operator, so using new
	//now_time_facet_type* pfacet = yggr_nothrow_new now_time_facet_type(format);
	now_time_facet_type* pfacet = 0;
	try
	{
		pfacet = new now_time_facet_type(format);
	}
	catch(...)
	{
		pfacet = 0;
	}

	assert(pfacet);
	if(!pfacet)
	{
		return os;
	}

	boost::system_time now_tm = tm;
	boost::system_time now_local_tm = local_adj::utc_to_local(now_tm);

	std::locale old_loc(os.getloc());
	std::locale new_loc(old_loc, pfacet);

	os.imbue(new_loc);
	os << now_local_tm;
	os.imbue(old_loc);

	return os;
}

YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX(time_formatting_local, char)

#if !defined(YGGR_NO_CWCHAR)
YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX(time_formatting_local, wchar_t)
#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)
YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX(time_formatting_local, c8)
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX(time_formatting_local, c16)
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX(time_formatting_local, c32)
#endif // YGGR_NO_CHAR32_T

#undef YGGR_TMP_PP_STREAM_TIME_FORMATTING_INT_FMT_FIX


#define YGGR_TMP_PP_STRING_TIME_FORMATTING_INT_FMT_FIX( __foo_name__) \
	template<typename String> inline \
	typename boost::enable_if< charset::is_string_t<String>, String& >::type \
		__foo_name__(String& out, int format, const boost::xtime& tm, \
							const string& ret_charset_name = charset::charset_name_t< String >(), \
							const string& fmt_charset_name = \
								charset::charset_name_t< \
									typename \
										range_ex::range_value_ex< \
											typename charset::basic_string_getter<String>::type \
										>::type >() ) { \
		typedef typename charset::basic_string_getter<String>::type base_string_type; \
		typedef typename range_ex::range_value_ex<base_string_type>::type now_char_type; \
		assert(format == 0); \
		return \
			__foo_name__< String, const now_char_type* >( \
				out, static_cast<const now_char_type*>(0), tm, ret_charset_name, fmt_charset_name); } \

// string time_formatting
// time_formatting_utc
template<typename String, typename Fmt> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			charset::is_string_t<String>,
			charset::is_generalized_string_t<Fmt>
		>,
		String&
	>::type
	time_formatting_utc(String& out, const Fmt& format, const boost::xtime& tm,
							const string& ret_charset_name = charset::charset_name_t<String>(),
							const string& fmt_charset_name = charset::charset_name_t<Fmt>())
{
	typedef String out_str_type;
	typedef Fmt fmt_str_type;

	typedef typename range_ex::range_value_ex<fmt_str_type>::type fmt_str_char_cv_type;
	typedef typename boost::remove_cv<fmt_str_char_cv_type>::type fmt_str_char_type;
	typedef std::basic_string<fmt_str_char_type> stream_string_type;
	typedef std::basic_stringstream<fmt_str_char_type> stream_type;

	typedef charset::charset_error_def::error_type charset_err_type;

	stream_string_type str_fmt;
	try
	{
		charset::charset_foo::s_xchg(
			str_fmt, format, fmt_charset_name, charset::charset_name_t<fmt_str_char_type>());
	}
	catch(const charset_err_type&)
	{
		assert(false);
		return out;
	}
	catch(...)
	{
		assert(false);
		return out;
	}

	stream_type ss;
	time_formatting_utc(ss, str_fmt.empty()? static_cast<const fmt_str_char_type*>(0) : str_fmt.c_str(), tm);

	try
	{
		charset::charset_foo::s_xchg(
			charset::get_basic_string(out),
			ss.str(),
			charset::charset_name_t<fmt_str_char_type>(),
			ret_charset_name);
	}
	catch(const charset_err_type&)
	{
		assert(false);
	}
	catch(...)
	{
		assert(false);
	}
	charset::recount_length(out);
	return out;
}

YGGR_TMP_PP_STRING_TIME_FORMATTING_INT_FMT_FIX(time_formatting_utc)

// time_formatting_local
template<typename String, typename Fmt> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			charset::is_string_t<String>,
			charset::is_generalized_string_t<Fmt>
		>,
		String&
	>::type
	time_formatting_local(String& out, const Fmt& format, const boost::xtime& tm,
							const string& ret_charset_name = charset::charset_name_t<String>(),
							const string& fmt_charset_name = charset::charset_name_t<Fmt>())
{
	typedef String out_str_type;
	typedef Fmt fmt_str_type;

	typedef typename range_ex::range_value_ex<fmt_str_type>::type fmt_str_char_cv_type;
	typedef typename boost::remove_cv<fmt_str_char_cv_type>::type fmt_str_char_type;
	typedef std::basic_string<fmt_str_char_type> stream_string_type;
	typedef std::basic_stringstream<fmt_str_char_type> stream_type;

	typedef charset::charset_error_def::error_type charset_err_type;

	stream_string_type str_fmt;
	try
	{
		charset::charset_foo::s_xchg(
			str_fmt, format, fmt_charset_name, charset::charset_name_t<fmt_str_char_type>());
	}
	catch(const charset_err_type&)
	{
		assert(false);
		return out;
	}
	catch(...)
	{
		assert(false);
		return out;
	}

	stream_type ss;
	time_formatting_local(ss, str_fmt.empty()? static_cast<const fmt_str_char_type*>(0) : str_fmt.c_str(), tm);

	try
	{
		charset::charset_foo::s_xchg(
			charset::get_basic_string(out),
			ss.str(),
			charset::charset_name_t<fmt_str_char_type>(),
			ret_charset_name);
	}
	catch(const charset_err_type&)
	{
		assert(false);
	}
	catch(...)
	{
		assert(false);
	}

	charset::recount_length(out);
	return out;
}

YGGR_TMP_PP_STRING_TIME_FORMATTING_INT_FMT_FIX(time_formatting_local)

#undef YGGR_TMP_PP_STRING_TIME_FORMATTING_INT_FMT_FIX

} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_TIME_FORMATTING_HPP__

