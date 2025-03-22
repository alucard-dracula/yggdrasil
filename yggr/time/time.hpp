//time.hpp

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

#ifndef __YGGR_TIME_TIME_HPP__
#define __YGGR_TIME_TIME_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/error_make.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/support/operator_t.hpp>

#include <yggr/type_traits/native_t.hpp>
#include <yggr/type_traits/operator_result.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/mplex/limits.hpp>
#include <yggr/math/modulus.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>
#include <yggr/charset/generalized_string.hpp>
#include <yggr/charset/make_utf8_string_t_from_allocator.hpp>

#include <yggr/time/time_config.hpp>
#include <yggr/time/time_ratio_def.hpp>
#include <yggr/time/xtime_basic_foo.hpp>
#include <yggr/time/time_arithmetic_ref.hpp>
#include <yggr/time/time_formatting.hpp>

#include <yggr/time/detail/time_to_string_macro.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/functional/hash/hash.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

#include <ctime>
#include <cassert>

namespace yggr
{
namespace time
{

class time
	: public boost::xtime
{
public:
	typedef time_ratio_def time_ratio_def_type;
	typedef time_ratio_def_type::value_type time_ratio_type;

	YGGR_STATIC_CONSTANT(time_ratio_type, E_S_AND_MS_RATIO = time_ratio_def_type::E_S_AND_MS_RATIO); // 1000
	YGGR_STATIC_CONSTANT(time_ratio_type, E_S_AND_MS_HALF_RATIO = time_ratio_def_type::E_S_AND_MS_HALF_RATIO); // 500

	YGGR_STATIC_CONSTANT(time_ratio_type, E_S_AND_MCS_RATIO = time_ratio_def_type::E_S_AND_MCS_RATIO); // 1000000
	YGGR_STATIC_CONSTANT(time_ratio_type, E_S_AND_MCS_HALF_RATIO = time_ratio_def_type::E_S_AND_MCS_HALF_RATIO); // 500000

	YGGR_STATIC_CONSTANT(time_ratio_type, E_S_AND_NS_RATIO = time_ratio_def_type::E_S_AND_NS_RATIO); // 1000000000
	YGGR_STATIC_CONSTANT(time_ratio_type, E_S_AND_NS_HALF_RATIO = time_ratio_def_type::E_S_AND_NS_HALF_RATIO); // 500000000

	YGGR_STATIC_CONSTANT(time_ratio_type, E_MS_AND_MCS_RATIO = time_ratio_def_type::E_MS_AND_MCS_RATIO); // 1000
	YGGR_STATIC_CONSTANT(time_ratio_type, E_MS_AND_MCS_HALF_RATIO = time_ratio_def_type::E_MS_AND_MCS_HALF_RATIO); // 500

	YGGR_STATIC_CONSTANT(time_ratio_type, E_MS_AND_NS_RATIO = time_ratio_def_type::E_MS_AND_NS_RATIO); // 1000000
	YGGR_STATIC_CONSTANT(time_ratio_type, E_MS_AND_NS_HALF_RATIO = time_ratio_def_type::E_MS_AND_NS_HALF_RATIO); // 500000

	YGGR_STATIC_CONSTANT(time_ratio_type, E_MCS_AND_NS_RATIO = time_ratio_def_type::E_MCS_AND_NS_RATIO); // 1000
	YGGR_STATIC_CONSTANT(time_ratio_type, E_MCS_AND_NS_HALF_RATIO = time_ratio_def_type::E_MCS_AND_NS_HALF_RATIO); // 500

public:
	typedef boost::system_time system_type;

	typedef boost::xtime base_type;
	typedef base_type bridge_time_type;
	typedef base_type::xtime_sec_t xtime_sec_t; // sys32 32bit sys64 64bit
	typedef base_type::xtime_nsec_t xtime_nsec_t; // win 32bit linux 64bit

private:
	typedef mplex::tag_sfinae_type tag_sfinae_type;

private:
	typedef time this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	time(void);

	template<typename T>
	explicit time(T tsec,
					typename
						boost::enable_if
						<
							detail::is_time_second<T>,
							tag_sfinae_type
						>::type sfinae = tag_sfinae_type())
	{
		xtime_from_second(*this, tsec);
	}

	time(xtime_sec_t tsec, xtime_nsec_t tnsec);

	time(BOOST_RV_REF(base_type) right)
	{
		base_type::sec = boost::move(right.sec);
		base_type::nsec = boost::move(right.nsec);
		this_type::rebuild();
	}

	time(const base_type& right);

	time(BOOST_RV_REF(this_type) right)
	{
		base_type::sec = boost::move(right.sec);
		base_type::nsec = boost::move(right.nsec);
	}

	time(const this_type& right);
	~time(void);

public:
	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type&>::type
		operator=(T tsec)
	{
		xtime_from_second(*this, tsec);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type::sec = boost::move(right.sec);
		base_type::nsec = boost::move(right.nsec);
		this_type::rebuild();

        return *this;
	}

	inline this_type& operator=(const base_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type::sec = right.sec;
		base_type::nsec = right.nsec;

		this_type::rebuild();
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::sec = boost::move(right.sec);
		base_type::nsec = boost::move(right.nsec);
        return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type::sec = right.sec;
		base_type::nsec = right.nsec;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(base_type& right)
	{
		if(this == &right)
		{
			return;
		}

		yggr::swap(base_type::sec, right.sec);
		yggr::swap(base_type::nsec, right.nsec);

		this_type::rebuild();
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		yggr::swap(base_type::sec, right.sec);
		yggr::swap(base_type::nsec, right.nsec);
	}

public:
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return base_type::sec || base_type::nsec;
	}

public:
	inline bridge_time_type& bridge_time(void)
	{
		return static_cast<bridge_time_type&>(*this);
	}

	inline const bridge_time_type& bridge_time(void) const
	{
		return static_cast<const bridge_time_type&>(*this);
	}

public:
	inline bool decomposable(void) const
	{
		return xtime_decomposable(*this);
	}

	inline this_type& decompose(void)
	{
		return (xtime_decompose(*this), *this);
	}

	inline bool formatable(void) const
	{
		return xtime_formatable(*this);
	}

	inline this_type& format(void)
	{
		return (xtime_format(*this), *this);
	}

	inline this_type& rebuild(void)
	{
		return (xtime_format(*this), xtime_decompose(*this), *this);
	}

public:
	// +a
	inline const this_type& positive(void) const
	{
		return *this;
	}

	// -a
	inline this_type negative(void) const
	{
		return this_type(-base_type::sec, -base_type::nsec);
	}

	// a + b, a += b
	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type&>::type
		plus_set(const T& right)
	{
		xtime_plus_set(*this, right);
		return *this;
	}

	inline this_type& plus_set(const base_type& right)
	{
		xtime_plus_set(*this, right);
		return *this;
	}

	inline this_type& plus_set(const this_type& right)
	{
		xtime_plus_set(*this, right);
		return *this;
	}

	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type>::type
		plus(const T& right) const
	{
		this_type tmp = *this;
		xtime_plus_set(tmp, right);
		return tmp;
	}

	inline this_type plus(const base_type& right) const
	{
		this_type tmp = *this;
		xtime_plus_set(tmp, right);
		return tmp;
	}

	inline this_type plus(const this_type& right) const
	{
		this_type tmp = *this;
		xtime_plus_set(tmp, right);
		return tmp;
	}

	// a - b, a -= b
	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type&>::type
		minus_set(const T& right)
	{
		xtime_minus_set(*this, right);
		return *this;
	}

	inline this_type& minus_set(const base_type& right)
	{
		xtime_minus_set(*this, right);
		return *this;
	}

	inline this_type& minus_set(const this_type& right)
	{
		xtime_minus_set(*this, right);
		return *this;
	}

	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type>::type
		minus(const T& right) const
	{
		this_type tmp = *this;
		xtime_minus_set(tmp, right);
		return tmp;
	}

	inline this_type minus_set(const base_type& right) const
	{
		this_type tmp = *this;
		xtime_minus_set(tmp, right);
		return tmp;
	}

	inline this_type minus(const this_type& right) const
	{
		this_type tmp = *this;
		xtime_minus_set(tmp, right);
		return tmp;
	}

	// a * b, a *= b
	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type&>::type
		multiplies_set(const T& right)
	{
		xtime_multiplies_set(*this, right);
		return *this;
	}

	inline this_type& multiplies_set(const base_type& right)
	{
		xtime_multiplies_set(*this, right);
		return *this;
	}

	inline this_type& multiplies_set(const this_type& right)
	{
		xtime_multiplies_set(*this, right);
		return *this;
	}

	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type>::type
		multiplies(const T& right) const
	{
		this_type tmp = *this;
		xtime_multiplies_set(tmp, right);
		return tmp;
	}

	inline this_type multiplies(const base_type& right) const
	{
		this_type tmp = *this;
		xtime_multiplies_set(tmp, right);
		return tmp;
	}

	inline this_type multiplies(const this_type& right) const
	{
		this_type tmp = *this;
		xtime_multiplies_set(tmp, right);
		return tmp;
	}

	// a / b, a /= b

	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type&>::type
		divides_set(const T& right)
	{
		xtime_divides_set(*this, right);
		return *this;
	}

	inline this_type& divides_set(const base_type& right)
	{
		xtime_divides_set(*this, right);
		return *this;
	}

	inline this_type& divides_set(const this_type& right)
	{
		xtime_divides_set(*this, right);
		return *this;
	}

	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type>::type
		divides(const T& right) const
	{
		this_type tmp = *this;
		xtime_divides_set(tmp, right);
		return tmp;
	}

	inline this_type divides(const base_type& right) const
	{
		this_type tmp = *this;
		xtime_divides_set(tmp, right);
		return tmp;
	}

	inline this_type divides(const this_type& right) const
	{
		this_type tmp = *this;
		xtime_divides_set(tmp, right);
		return tmp;
	}

	template<typename T> inline
	T divides(const base_type& right) const
	{
		return xtime_divides<T>(*this, right);
	}

	template<typename T> inline
	T divides(const this_type& right) const
	{
		return xtime_divides<T>(*this, right);
	}

	// a % b, a %= b

	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type&>::type
		modulus_set(const T& right)
	{
		xtime_modulus_set(*this, right);
		return *this;
	}

	inline this_type& modulus_set(const base_type& right)
	{
		xtime_modulus_set(*this, right);
		return *this;
	}

	inline this_type& modulus_set(const this_type& right)
	{
		xtime_modulus_set(*this, right);
		return *this;
	}

	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, this_type>::type
		modulus(const T& right) const
	{
		this_type tmp = *this;
		xtime_modulus_set(tmp, right);
		return tmp;
	}

	inline this_type modulus(const base_type& right) const
	{
		this_type tmp = *this;
		xtime_modulus_set(tmp, right);
		return tmp;
	}

	inline this_type modulus(const this_type& right) const
	{
		this_type tmp = *this;
		xtime_modulus_set(tmp, right);
		return tmp;
	}

public:
	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, s32>::type
		compare(const T& right) const
	{
		return xtime_compare(*this, right);
	}

	template<typename T> inline
	typename boost::enable_if<detail::is_time_second<T>, bool>::type
		compare_eq(const T& right) const
	{
		return xtime_compare_eq(*this, right);
	}

	inline s32 compare(const base_type& right) const
	{
		return xtime_compare(*this, right);
	}

	inline bool compare_eq(const base_type& right) const
	{
		return xtime_compare_eq(*this, right);
	}

	inline s32 compare(const this_type& right) const
	{
		return xtime_compare(*this, right);
	}

	inline bool compare_eq(const this_type& right) const
	{
		return xtime_compare_eq(*this, right);
	}

public:
	inline bool validate(void) const
	{
		return xtime_validate(*this);
	}

public:
	inline f64 ex_to_second(void) const
	{
		typedef f64 ret_type;
		return
			static_cast<ret_type>(base_type::sec)
			+ static_cast<ret_type>(base_type::nsec)
			/ static_cast<ret_type>(this_type::E_S_AND_NS_RATIO);
	}

	template<typename T> inline
	T ex_to_second(void) const
	{
		typedef T ret_type;
		return
			static_cast<ret_type>(base_type::sec)
			+ static_cast<ret_type>(base_type::nsec)
			/ static_cast<ret_type>(this_type::E_S_AND_NS_RATIO);
	}

	inline f64 ex_to_millisecond(void) const
	{
		typedef f64 ret_type;
		typedef s64 cal_type;

		cal_type sec = base_type::sec;
		cal_type nsec = base_type::nsec;

		return
			static_cast<ret_type>(sec * this_type::E_S_AND_MS_RATIO)
			+ static_cast<ret_type>(nsec + this_type::E_MS_AND_NS_HALF_RATIO)
			/ static_cast<ret_type>(E_MS_AND_NS_RATIO);
	}

	template<typename T> inline
	T ex_to_millisecond(void) const
	{
		typedef T ret_type;
		typedef s64 cal_type;

		cal_type sec = base_type::sec;
		cal_type nsec = base_type::nsec;

		return
			static_cast<ret_type>(sec * this_type::E_S_AND_MS_RATIO )
			+ static_cast<ret_type>(nsec + this_type::E_MS_AND_NS_HALF_RATIO)
			/ static_cast<ret_type>(this_type::E_MS_AND_NS_RATIO);
	}

public:
	inline void reset(void)
	{
		base_type::sec = 0;
		base_type::nsec = 0;
	}

	inline void update(void)
	{
		base_type& base = *this;
#if BOOST_VERSION < 105300
	boost::xtime_get(&base, boost::TIME_UTC);
#else
	boost::xtime_get(&base, boost::TIME_UTC_);
#endif // BOOST_VERSION
	}

public:
	std::tm to_tm(void) const;
	std::tm to_local_tm(void) const;

	// to_string
protected:
	//template<typename String, typename Fmt> inline
	//String pro_to_string(const Fmt& format,
	//						const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt)) const;

	//template<typename String, typename Fmt, typename Alloc> inline
	//String pro_to_string(const Fmt& format, const Alloc& alloc,
	//					const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt)) const;

	YGGR_PP_TIME_TIME_TO_STRING_PRO_MEMBER_FOO(
		::yggr::time::time_config::default_time_format,
		::yggr::time::time_formatting_utc,
		pro_to_string )

public:
	//// return utf8_string verson
	//inline utf8_string to_string(void) const;

	//template<typename Alloc> inline
	//utf8_string to_string(const Alloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const;

	//template<typename Char> inline
	//utf8_string to_string(const Char* format,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;

	//template<typename Char, typename Alloc> inline
	//utf8_string to_string(const Char* format, const Alloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;

	//template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
	//			typename Char, typename Traits, typename Alloc> inline
	//utf8_string to_string(const BasicString<Char, Traits, Alloc>& format,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name =
	//							::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >()) const;

	//template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
	//			typename Char, typename Traits, typename Alloc,
	//			typename RetAlloc> inline
	//utf8_string to_string(const BasicString<Char, Traits, Alloc>& format, const RetAlloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name =
	//							::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >()) const;

	//template<typename T1, typename T2> inline
	//utf8_string to_string(const charset::string_charset_helper<T1, T2>& format,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename T1, typename T2, typename Alloc> inline
	//utf8_string to_string(const charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//// return custom string
	//String to_string(void) const;

	//template<typename String, typename Alloc> inline
	//String to_string(const Alloc& alloc,
	//					const yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String::value_type) ) const;

	//template<typename String, typename Char> inline
	//String to_string(const Char* format,
	//					const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;

	//template<typename String, typename Char, typename Alloc> inline
	//String to_string(const Char* format, const Alloc& alloc,
	//					const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;

	//template<typename String, typename Fmt> inline
	//String to_string(const Fmt& format,
	//					const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt)) const;

	//template<typename String, typename Fmt, typename Alloc> inline
	//String to_string(const Fmt& format, const Alloc& alloc,
	//					const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt) ) const;

	//template<typename String, typename T1, typename T2> inline
	//String to_string(const charset::string_charset_helper<T1, T2>& format,
	//					const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String)) const;

	//template<typename String, typename T1, typename T2, typename Alloc> inline
	//String to_string(const charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//					const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	YGGR_PP_TIME_TIME_TO_STRING_MEMBER_FOO(this_type::pro_to_string, to_string)

	// to_local_string
protected:
	//template<typename String, typename Fmt> inline
	//String pro_to_local_string(const Fmt& format,
	//								const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//								const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt)) const;

	//template<typename String, typename Fmt, typename Alloc> inline
	//String pro_to_local_string(const Fmt& format, const Alloc& alloc,
	//								const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//								const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt)) const;

	YGGR_PP_TIME_TIME_TO_STRING_PRO_MEMBER_FOO(
		::yggr::time::time_config::default_time_format,
		::yggr::time::time_formatting_local,
		pro_to_local_string )

public:
	//// return utf8_string verson
	//inline utf8_string to_local_string(void) const;

	//template<typename Alloc> inline
	//utf8_string to_local_string(const Alloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const;

	//template<typename Char> inline
	//utf8_string to_local_string(const Char* format,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;

	//template<typename Char, typename Alloc> inline
	//utf8_string to_local_string(const Char* format, const Alloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;

	//template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
	//			typename Char, typename Traits, typename Alloc> inline
	//utf8_string to_local_string(const BasicString<Char, Traits, Alloc>& format,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name =
	//							::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >()) const;

	//template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
	//			typename Char, typename Traits, typename Alloc,
	//			typename RetAlloc> inline
	//utf8_string to_local_string(const BasicString<Char, Traits, Alloc>& format, const RetAlloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name =
	//							::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >()) const;

	//template<typename T1, typename T2> inline
	//utf8_string to_local_string(const charset::string_charset_helper<T1, T2>& format,
	//								const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename T1, typename T2, typename Alloc> inline
	//utf8_string to_local_string(const charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//								const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//// return custom string
	//String to_local_string(void) const;

	//template<typename String, typename Alloc> inline
	//String to_local_string(const Alloc& alloc,
	//							const yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String::value_type) ) const;

	//template<typename String, typename Char> inline
	//String to_local_string(const Char* format,
	//							const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;

	//template<typename String, typename Char, typename Alloc> inline
	//String to_local_string(const Char* format, const Alloc& alloc,
	//							const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;

	//template<typename String, typename Fmt> inline
	//String to_local_string(const Fmt& format,
	//							const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt)) const;

	//template<typename String, typename Fmt, typename Alloc> inline
	//String to_local_string(const Fmt& format, const Alloc& alloc,
	//							const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt) ) const;

	//template<typename String, typename T1, typename T2> inline
	//String to_local_string(const charset::string_charset_helper<T1, T2>& format,
	//							const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String)) const;

	//template<typename String, typename T1, typename T2, typename Alloc> inline
	//String to_local_string(const charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	YGGR_PP_TIME_TIME_TO_STRING_MEMBER_FOO(this_type::pro_to_local_string, to_local_string)

public:
	inline std::size_t hash(void) const
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, base_type::sec);
		boost::hash_combine(seed, base_type::nsec);
		return seed;
	}

public:
	template<typename T> inline
	static T time_second(void)
	{
		this_type tmp;
		return tmp.ex_to_second<T>();
	}

	template<typename T> inline
	static T time_millisecond(void)
	{
		this_type tmp;
		return tmp.ex_to_millisecond<T>();
	}

public:
	// time_string
	//// return utf8_string verson
	//inline static utf8_string time_string(void);

	//template<typename Alloc> inline static
	//utf8_string time_string(const Alloc& alloc,
	//							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char));

	//template<typename Char> inline static
	//utf8_string time_string(const Char* format,
	//							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));

	//template<typename Char, typename Alloc> inline static
	//utf8_string time_string(const Char* format, const Alloc& alloc,
	//							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));

	//template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
	//			typename Char, typename Traits, typename Alloc> inline static
	//utf8_string time_string(const BasicString<Char, Traits, Alloc>& format,
	//							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//							const string& fmt_charset_name =
	//								::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >());

	//template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
	//			typename Char, typename Traits, typename Alloc,
	//			typename RetAlloc> inline static
	//utf8_string time_string(const BasicString<Char, Traits, Alloc>& format, const RetAlloc& alloc,
	//							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//							const string& fmt_charset_name =
	//								::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >());

	//template<typename T1, typename T2> inline static
	//utf8_string time_string(const charset::string_charset_helper<T1, T2>& format,
	//							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME());

	//template<typename T1, typename T2, typename Alloc> inline static
	//utf8_string time_string(const charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME());

	//// return custom string
	//String time_string(void);

	//template<typename String, typename Alloc> inline static
	//String time_string(const Alloc& alloc,
	//						const yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String::value_type) );

	//template<typename String, typename Char> inline static
	//String time_string(const Char* format,
	//						const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));

	//template<typename String, typename Char, typename Alloc> inline static
	//String time_string(const Char* format, const Alloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));

	//template<typename String, typename Fmt> inline static
	//String time_string(const Fmt& format,
	//						const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt));

	//template<typename String, typename Fmt, typename Alloc> inline static
	//String time_string(const Fmt& format, const Alloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt) );

	//template<typename String, typename T1, typename T2> inline static
	//String time_string(const charset::string_charset_helper<T1, T2>& format,
	//						const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String));

	//template<typename String, typename T1, typename T2, typename Alloc> inline static
	//String time_string(const charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME());

	YGGR_PP_TIME_TIME_TO_STRING_STATIC_MEMBER_FOO(to_string, time_string)

public:
	// local_time_string
	//// return utf8_string verson
	//inline static utf8_string local_time_string(void);

	//template<typename Alloc> inline static
	//utf8_string local_time_string(const Alloc& alloc,
	//								const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//								const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char));

	//template<typename Char> inline static
	//utf8_string local_time_string(const Char* format,
	//								const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//								const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));

	//template<typename Char, typename Alloc> inline static
	//utf8_string local_time_string(const Char* format, const Alloc& alloc,
	//								const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//								const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));

	//template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
	//			typename Char, typename Traits, typename Alloc> inline static
	//utf8_string local_time_string(const BasicString<Char, Traits, Alloc>& format,
	//								const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//								const string& fmt_charset_name =
	//									::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >());

	//template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
	//			typename Char, typename Traits, typename Alloc,
	//			typename RetAlloc> inline static
	//utf8_string local_time_string(const BasicString<Char, Traits, Alloc>& format, const RetAlloc& alloc,
	//								const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//								const string& fmt_charset_name =
	//									::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >());

	//template<typename T1, typename T2> inline static
	//utf8_string local_time_string(const charset::string_charset_helper<T1, T2>& format,
	//								const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME());

	//template<typename T1, typename T2, typename Alloc> inline static
	//utf8_string local_time_string(const charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//								const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME());

	//// return custom string
	//String local_time_string(void);

	//template<typename String, typename Alloc> inline static
	//String local_time_string(const Alloc& alloc,
	//							const yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String::value_type) );

	//template<typename String, typename Char> inline static
	//String local_time_string(const Char* format,
	//							const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));

	//template<typename String, typename Char, typename Alloc> inline static
	//String local_time_string(const Char* format, const Alloc& alloc,
	//							const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));

	//template<typename String, typename Fmt> inline static
	//String local_time_string(const Fmt& format,
	//							const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt));

	//template<typename String, typename Fmt, typename Alloc> inline static
	//String local_time_string(const Fmt& format, const Alloc& alloc,
	//							const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//							const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt) );

	//template<typename String, typename T1, typename T2> inline static
	//String local_time_string(const charset::string_charset_helper<T1, T2>& format,
	//							const string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String));

	//template<typename String, typename T1, typename T2, typename Alloc> inline static
	//String local_time_string(const charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//							const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME());

	YGGR_PP_TIME_TIME_TO_STRING_STATIC_MEMBER_FOO(to_local_string, local_time_string)

public:
	inline static const this_type& min_time_step(void)
	{
		static const this_type val(static_cast<time::xtime_sec_t>(0), static_cast<xtime_nsec_t>(1));
		return val;
	}

	inline static const this_type& max_time_step(void)
	{
		return this_type::max_value();
	}

	inline static const this_type& zero_value(void)
	{
		static const this_type val(static_cast<time::xtime_sec_t>(0));
		return val;
	}

	inline static const this_type& min_value(void)
	{
		static const this_type val(
			mplex::numeric_limits<xtime_sec_t>::min_type::value,
			static_cast<xtime_nsec_t>(0));

		return val;
	}

	inline static const this_type& max_value(void)
	{
		static const this_type val(
			mplex::numeric_limits<xtime_sec_t>::max_type::value,
			static_cast<xtime_nsec_t>(this_type::E_S_AND_NS_RATIO - 1));

		return val;
	}

	template<typename T> inline
	static typename boost::enable_if_c<(sizeof(T) < 8), const this_type&>::type
		default_precision(void)
	{
		static const this_type val(0, 10000);
		return val;
	}

	template<typename T> inline
	static typename boost::disable_if_c<(sizeof(T) < 8), const this_type&>::type
		default_precision(void)
	{
		static const this_type val(0, 10);
		return val;
	}
};

// non-member foo
//==
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator==(const time& l, const T& r)
{
	return l.compare_eq(r);
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator==(const T& l, const time& r)
{
	return r.compare_eq(l);
}

inline bool operator==(const time& l, const time::base_type& r)
{
	return l.compare_eq(r);
}

inline bool operator==(const time::base_type& l, const time& r)
{
	return r.compare_eq(l);
}

inline bool operator==(const time& l, const time& r)
{
	return l.compare_eq(r);
}

// !=
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator!=(const time& l, const T& r)
{
	return !l.compare_eq(r);
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator!=(const T& l, const time& r)
{
	return !r.compare_eq(l);
}

inline bool operator!=(const time& l, const time::base_type& r)
{
	return !l.compare_eq(r);
}

inline bool operator!=(const time::base_type& l, const time& r)
{
	return !r.compare_eq(l);
}

inline bool operator!=(const time& l, const time& r)
{
	return !l.compare_eq(r);
}

// <
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator<(const time& l, const T& r)
{
	return l.compare(r) < 0;
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator<(const T& l, const time& r)
{
	return 0 < r.compare(l);
}

inline bool operator<(const time& l, const time::base_type& r)
{
	return l.compare(r) < 0;
}

inline bool operator<(const time::base_type& l, const time& r)
{
	return 0 < r.compare(l);
}

inline bool operator<(const time& l, const time& r)
{
	return l.compare(r) < 0;
}

// <=
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator<=(const time& l, const T& r)
{
	return l.compare(r) <= 0;
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator<=(const T& l, const time& r)
{
	return 0 <= r.compare(l);
}

inline bool operator<=(const time& l, const time::base_type& r)
{
	return l.compare(r) <= 0;
}

inline bool operator<=(const time::base_type& l, const time& r)
{
	return 0 <= r.compare(l);
}

inline bool operator<=(const time& l, const time& r)
{
	return l.compare(r) <= 0;
}

// >

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator>(const time& l, const T& r)
{
	return 0 < l.compare(r);
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator>(const T& l, const time& r)
{
	return r.compare(l) < 0;
}

inline bool operator>(const time& l, const time::base_type& r)
{
	return 0 < l.compare(r);
}

inline bool operator>(const time::base_type& l, const time& r)
{
	return r.compare(l) < 0;
}

inline bool operator>(const time& l, const time& r)
{
	return 0 < l.compare(r);
}

// >=

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator>=(const time& l, const T& r)
{
	return 0 <= l.compare(r);
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, bool>::type
	operator>=(const T& l, const time& r)
{
	return r.compare(l) <= 0;
}

inline bool operator>=(const time& l, const time::base_type& r)
{
	return 0 <= l.compare(r);
}

inline bool operator>=(const time::base_type& l, const time& r)
{
	return r.compare(l) <= 0;
}

inline bool operator>=(const time& l, const time& r)
{
	return 0 <= l.compare(r);
}

// positive
inline const time& operator+(const time& val)
{
	return val.positive();
}

// negative
inline time operator-(const time& val)
{
	return val.negative();
}

// plus_set
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time&>::type
	operator+=(time& l, const T& r)
{
	return l.plus_set(r);
}

inline time& operator+=(time& l, const time::base_type& r)
{
	return l.plus_set(r);
}

inline time& operator+=(time& l, const time& r)
{
	return l.plus_set(r);
}

// plus
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time>::type
	operator+(const time& l, const T& r)
{
	return l.plus(r);
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time>::type
	operator+(const T& l, const time& r)
{
	return r.plus(l);
}

inline time operator+(const time& l, const time::base_type& r)
{
	return l.plus(r);
}

inline time operator+(const time::base_type& l, const time& r)
{
	return r.plus(l);
}

inline time operator+(const time& l, const time& r)
{
	return l.plus(r);
}

// minus_set
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time&>::type
	operator-=(time& l, const T& r)
{
	return l.minus_set(r);
}

inline time& operator-=(time& l, const time::base_type& r)
{
	return l.minus_set(r);
}

inline time& operator-=(time& l, const time& r)
{
	return l.minus_set(r);
}

// minus
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time>::type
	operator-(const time& l, const T& r)
{
	return l.minus(r);
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time>::type
	operator-(const T& l, const time& r)
{
	return (-r).plus(l);
}

inline time operator-(const time& l, const time::base_type& r)
{
	return l.minus(r);
}

inline time operator-(const time::base_type& l, const time& r)
{
	return (-r).plus(l);
}

inline time operator-(const time& l, const time& r)
{
	return l.minus(r);
}

// mul_set
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time&>::type
	operator*=(time& l, const T& r)
{
	return l.multiplies_set(r);
}

inline time& operator*=(time& l, const time::base_type& r)
{
	return l.multiplies_set(r);
}

inline time& operator*=(time& l, const time& r)
{
	return l.multiplies_set(r);
}

// mul
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time>::type
	operator*(const time& l, const T& r)
{
	return l.multiplies(r);
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time>::type
	operator*(const T& l, const time& r)
{
	return r.multiplies(l);
}

inline time operator*(const time& l, const time::base_type& r)
{
	return l.multiplies(r);
}

inline time operator*(const time::base_type& l, const time& r)
{
	return r.multiplies(l);
}

inline time operator*(const time& l, const time& r)
{
	return l.multiplies(r);
}

// div_set
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time&>::type
	operator/=(time& l, const T& r)
{
	return l.divides_set(r);
}

inline time& operator/=(time& l, const time::base_type& r)
{
	return l.divides_set(r);
}

inline time& operator/=(time& l, const time& r)
{
	return l.divides_set(r);
}

template<typename R, typename T> inline
typename boost::enable_if<boost::is_base_of<time::base_type, T>, time&>::type
	operator/=(time& l,
				math::arith_reference_wrapper
				<
					tag_float_divides_type,
					R,
					T const
				> const& r)
{
	const T& rref = r;
	return (l = l.divides<R>(rref));
}

template<typename R, typename T> inline
typename boost::enable_if<boost::is_same<time, T>, time&>::type
	operator/=(math::arith_reference_wrapper
				<
					tag_float_divides_type,
					R,
					T
				> const& l,
				const time::base_type& r)
{
	T& lref = l;
	return (lref = lref.template divides<R>(r));
}

template<typename R, typename T> inline
typename boost::enable_if<boost::is_same<time, T>, time&>::type
	operator/=(math::arith_reference_wrapper
				<
					tag_float_divides_type,
					R,
					T
				> const& l,
				const time& r)
{
	T& lref = l;
	return (lref = lref.template divides<R>(r));
}

template<typename RL, typename TL, typename RR, typename TR> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::is_same<time, TL>,
			boost::is_base_of<time::base_type, TR>
		>,
		time&
	>::type
	operator/=(math::arith_reference_wrapper
				<
					tag_float_divides_type,
					RL,
					TL
				> const& l,
				math::arith_reference_wrapper
				<
					tag_float_divides_type,
					RR,
					TR const
				> const& r)
{
	typedef typename divides_operator_result<RL, RR>::type ret_type;

	TL& lref = l;
	const TR& rref = r;
	return (lref = lref.template divides<ret_type>(rref));
}


// div
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time>::type
	operator/(const time& l, const T& r)
{
	return l.divides(r);
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time>::type
	operator/(const T& l, const time& r)
{
	time t(l);
	return t.divides_set(r);
}

inline time operator/(const time& l, const time::base_type& r)
{
	return l.divides(r);
}

inline time operator/(const time::base_type& l, const time& r)
{
	time t(l);
	return t.divides_set(r);
}

inline time operator/(const time& l, const time& r)
{
	return l.divides(r);
}

template<typename R, typename T> inline
typename boost::enable_if<boost::is_base_of<time::base_type, T>, R>::type
	operator/(const time& l,
				const
					math::arith_reference_wrapper
					<
						tag_float_divides_type,
						R,
						T const
					>& r)
{
	const T& rref = r;
	return l.divides<R>(rref);
}

template<typename R, typename T> inline
typename boost::enable_if<boost::is_base_of<time::base_type, T>, R>::type
	operator/(math::arith_reference_wrapper
				<
					tag_float_divides_type,
					R,
					T const
				> const& l,
				const time& r)
{
	const T& lref = l;
	return xtime_divides<R>(lref, r);
}

template<typename RL, typename TL, typename RR, typename TR> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::is_base_of<time::base_type, TL>,
			boost::is_base_of<time::base_type, TR>
		>,
		typename divides_operator_result<RL, RR>::type
	>::type
	operator/(math::arith_reference_wrapper
				<
					tag_float_divides_type,
					RL,
					TL const
				> const& l,
				math::arith_reference_wrapper
				<
					tag_float_divides_type,
					RR,
					TR const
				> const& r)
{
	typedef typename divides_operator_result<RL, RR>::type ret_type;
	const TL& lref = l;
	const TR& rref = r;

	return xtime_divides<ret_type>(lref, rref);
}

template<typename RL, typename RR> inline
typename divides_operator_result<RL, RR>::type
	operator/(math::arith_reference_wrapper
				<
					tag_float_divides_type,
					RL,
					time const
				> const& l,
				math::arith_reference_wrapper
				<
					tag_float_divides_type,
					RR,
					time const
				> const& r)
{
	typedef typename divides_operator_result<RL, RR>::type ret_type;

	const time& lref = l;
	const time& rref = r;

	return lref.divides<ret_type>(rref);
}

// mod_set
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time&>::type
	operator%=(time& l, const T& r)
{
	return l.modulus_set(r);
}

inline time& operator%=(time& l, const time::base_type& r)
{
	return l.modulus_set(r);
}

inline time& operator%=(time& l, const time& r)
{
	return l.modulus_set(r);
}

// mod
template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time>::type
	operator%(const time& l, const T& r)
{
	return l.modulus(r);
}

template<typename T> inline
typename boost::enable_if<detail::is_time_second<T>, time>::type
	operator%(const T& l, const time& r)
{
	time t(l);
	return t.modulus_set(r);
}

inline time operator%(const time& l, const time::base_type& r)
{
	return l.modulus(r);
}

inline time operator%(const time::base_type& l, const time& r)
{
	time t(l);
	return t.modulus_set(r);
}

inline time operator%(const time& l, const time& r)
{
	return l.modulus(r);
}

inline time abs(const time& t)
{
	return t < time::zero_value()? -t : t;
}

} //namespace time
} //namespace yggr

// abs
namespace std
{
	using ::yggr::time::abs;
} // namespace std


// swap
namespace yggr
{
namespace time
{
namespace swap_support
{

inline void swap(time::base_type& left, time& right)
{
	right.swap(left);
}

inline void swap(time& left, time::base_type& right)
{
	left.swap(right);
}

inline void swap(time& left, time& right)
{
	left.swap(right);
}

} // namespace swap_support

using swap_support::swap;

} // namespace time
} // namespace yggr

namespace std
{
	using ::yggr::time::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::time::swap_support::swap;
} // namespace boost

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace time
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

inline std::size_t hash_value(const yggr::time::time& tm)
{
	return tm.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace time
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

template<>
struct hash<yggr::time::time >
{
	typedef yggr::time::time type;

	inline std::size_t operator()(const type& tm) const
	{
		return tm.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#endif //_YGGR_TIME_TIME_HPP__
