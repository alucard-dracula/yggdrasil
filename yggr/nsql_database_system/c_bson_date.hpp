//c_bson_date.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_DATE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_DATE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/time/time_config.hpp>
#include <yggr/time/time_ratio_def.hpp>
#include <yggr/charset/generalized_string.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/type_traits/native_t.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>
#include <yggr/nsql_database_system/bson_date_time_native_ex.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>
#include <yggr/nsql_database_system/detail/bson_date_time_to_string_macro.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/thread/xtime.hpp>

#include <cassert>
#include <ctime>
#include <ostream>

namespace yggr
{
namespace nsql_database_system
{

class c_bson_date;
class c_bson_time;
class c_bson_timeval;

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_TYPEID_IMPL(c_bson_date, bson_typeid_def::E_BSON_TYPE_DATE_TIME)

namespace yggr
{
namespace nsql_database_system
{

// !!! mongodb strand time unit is millisecond (c_bson_date) !!!

class c_bson_date
{
public:
	typedef s64 base_type;
	typedef boost::xtime bridge_time_type;

private:
	typedef time::time_ratio_def time_ratio_def_type;
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

private:
	typedef c_bson_date this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_bson_date(void);
	c_bson_date(base_type right);
	c_bson_date(const bson_value_t& val);

	c_bson_date(BOOST_RV_REF(this_type) right)
		: _date(right._date)
	{
	}

	c_bson_date(const c_bson_time& right);

	c_bson_date(const timeval& right);
	c_bson_date(const c_bson_timeval& right);

	c_bson_date(const bridge_time_type& right); // support time_cast

	c_bson_date(const this_type& right);
	~c_bson_date(void);

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	this_type& operator=(base_type date);

	this_type& operator=(const c_bson_time& right);

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, this_type&>::type
		operator=(const BsonTimeval& right)
	{
		_date =
			static_cast<base_type>(right.tv_sec) * this_type::E_S_AND_MS_RATIO
			+ static_cast<base_type>(right.tv_usec) / this_type::E_MS_AND_MCS_RATIO;
		return *this;
	}

	this_type& operator=(const bridge_time_type& right); // support time_cast

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		_date = right._date;
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
    // base_type is s64
#if defined(_MSC_VER) || (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION) // gcc cpp03 not support move(basic type)
	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = static_cast<base_type&>(right);
		this_type::swap(right_ref);
	}
#endif // #if defined(_MSC_VER) || (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION)

	void swap(base_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

protected:
	void pro_assign_bson_value_t(const bson_value_t& val);

public:
	inline operator base_type&(void)
	{
		return _date;
	}

	inline operator base_type(void) const
	{
		return _date;
	}

	inline operator bridge_time_type(void) const // support time_cast
	{
		return this_type::bridge_time();
	}

public:
	inline base_type& date(void)
	{
		return _date;
	}

	inline base_type date(void) const
	{
		return _date;
	}

	inline base_type time(void) const
	{
		return _date / this_type::E_S_AND_MS_RATIO;
	}

	inline bridge_time_type bridge_time(void) const
	{
		bridge_time_type bridge =
		{
			static_cast<bridge_time_type::xtime_sec_t>(_date)
				/ this_type::E_S_AND_MS_RATIO,

			static_cast<bridge_time_type::xtime_nsec_t>(_date)
				% this_type::E_S_AND_MS_RATIO
				* this_type::E_MS_AND_NS_RATIO
		};

		return bridge;
	}

public:

	// compare_eq
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& val) const
	{
		return bson_value_native_ex::s_value_cmp_eq_date(boost::addressof(val), _date);
	}

	bool compare_eq(const c_bson_time& val) const;

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
		compare_eq(const BsonTimeval& val) const
	{
		return bson_date_time_native_ex::date_timeval_compare_eq(_date, val.tv_sec, val.tv_usec);
	}

	inline bool compare_eq(const bridge_time_type& val) const
	{
		return
			bson_date_time_native_ex::date_xtime_compare_eq(
				_date, val.sec, val.nsec);
	}

	inline bool compare_eq(base_type right) const
	{
		return _date == right;
	}

	inline bool compare_eq(const this_type& right) const
	{
		return _date == right._date;
	}

	// compare
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, s32>::type
		compare(const BsonValue& val) const
	{
		return -bson_value_native_ex::s_value_cmp_date(boost::addressof(val), _date);
	}

	s32 compare(const c_bson_time& val) const;

	//s32 compare(const c_bson_timeval& val) const;
	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, s32>::type
		compare(const BsonTimeval& val) const
	{
		return bson_date_time_native_ex::date_timeval_compare(_date, val.tv_sec, val.tv_usec);
	}

	inline s32 compare(const bridge_time_type& val) const
	{
		return
			bson_date_time_native_ex::date_xtime_compare(
				_date, val.sec, val.nsec);
	}

	inline s32 compare(base_type right) const
	{
		return _date == right? 0 : _date < right? -1 : 1;
	}

	inline s32 compare(const this_type& right) const
	{
		return _date == right._date? 0 : _date < right._date? -1 : 1;
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_DATE_TIME;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_DATE_TIME;
	}

// to_string
protected:
	//inline utf8_string pro_to_string(const char* format = 0) const;

	//template<typename Alloc> inline
	//utf8_string pro_to_string(const char* format, const Alloc& alloc) const;

	YGGR_PP_BSON_DATE_TIME_TO_STRING_PRO_MEMBER_FOO(bson_date_time_format::tm_fmt_iso8601_utc_org, pro_to_string)

public:
	//// return utf8_string verson
	//inline utf8_string to_string(void) const;

	//template<typename Alloc> inline
	//utf8_string to_string(const Alloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME() ) const;

	//template<typename Char> inline
	//utf8_string to_string(const Char* format,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;
	//
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
	//			typename Char, typename Traits, typename Alloc, typename RetAlloc> inline
	//utf8_string to_string(const BasicString<Char, Traits, Alloc>& format, const RetAlloc& alloc,
	//						const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//						const string& fmt_charset_name =
	//							::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >()) const;

	//template<typename T1, typename T2> inline
	//utf8_string to_string(const ::yggr::charset::string_charset_helper<T1, T2>& format,
	//						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename T1, typename T2, typename Alloc> inline
	//utf8_string to_string(const ::yggr::charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String> inline
	//utf8_string to_string(void) const;

	//template<typename String, typename Alloc> inline
	//String to_string(const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	//template<typename String, typename Char> inline
	//String to_string(const Char* format,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;

	//template<typename String, typename Char, typename Alloc> inline
	//String to_string(const Char* format, const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const;

	//template<typename String, typename Fmt> inline
	//String to_string(const Fmt& format,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt)) const;

	//template<typename String, typename Fmt, typename Alloc> inline
	//String to_string(const Fmt& format, const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt) ) const;

	//template<typename String, typename T1, typename T2> inline
	//String to_string(const ::yggr::charset::string_charset_helper<T1, T2>& format,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String)) const;

	//template<typename String, typename T1, typename T2, typename Alloc> inline
	//String to_string(const ::yggr::charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const;

	YGGR_PP_BSON_DATE_TIME_TO_STRING_MEMBER_FOO(this_type::pro_to_string, to_string)

	////debug

	//inline utf8_string to_string(void) const { return this_type::pro_to_string(); }
	//
	//inline utf8_string to_string(const char* format,
	//								const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//								const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const {
	//	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	return
	//		fmt_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()?
	//			this_type::pro_to_string(format)
	//			: this_type::pro_to_string(format? utf8_string(format, fmt_charset_name).data() : format); }
	//
	//template<typename Alloc> inline
	//typename
	//	boost::enable_if<
	//		::yggr::container_ex::is_allocator<Alloc>,
	//		typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type
	//	to_string(const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME() ) const {
	//	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	return this_type::pro_to_string(static_cast<const char*>(0), alloc); }
	//
	//template<typename Alloc> inline
	//typename
	//	boost::enable_if<
	//		::yggr::container_ex::is_allocator<Alloc>,
	//		typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type
	//	to_string(const char* format, const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const {
	//	return
	//		fmt_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()?
	//			this_type::pro_to_string(format, alloc)
	//			: this_type::pro_to_string(
	//				format? utf8_string(format, fmt_charset_name).data() : format, alloc); }
	//
	//template<typename Char> inline
	//utf8_string to_string(const Char* format,
	//							const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//							const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const {
	//	typedef Char* now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	if(format) {
	//		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//		return this_type::pro_to_string(str_fmt.data()); }
	//	else {
	//		return this_type::pro_to_string(); } }
	//
	//template<typename Char, typename Alloc> inline
	//typename
	//	boost::enable_if<
	//		::yggr::container_ex::is_allocator<Alloc>,
	//		typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type
	//	to_string(const Char* format, const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const {
	//	typedef Char* now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	if(format) {
	//		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//		return this_type::pro_to_string(str_fmt.data(), alloc); }
	//	else {
	//		return this_type::pro_to_string(static_cast<const char*>(0), alloc); } }
	//
	//template<template<typename _Char, typename _Trtaits, typename _Alloc> class Basic_String,
	//			typename Char, typename Traits, typename Alloc> inline
	//typename
	//	boost::enable_if<
	//		::yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >,
	//		utf8_string >::type
	//	to_string(const Basic_String<Char, Traits, Alloc>& format,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//					const ::yggr::string& fmt_charset_name =
	//						::yggr::charset::charset_name_t< Basic_String<Char, Traits, Alloc> >() ) const {
	//	typedef Basic_String<Char, Traits, Alloc> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//	return this_type::pro_to_string(str_fmt.data()); }
	//
	//template<template<typename _Char, typename _Trtaits, typename _Alloc> class Basic_String,
	//			typename Char, typename Traits, typename Alloc, typename RetAlloc> inline
	//typename
	//	boost::enable_if<
	//		boost::mpl::and_<
	//			::yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >,
	//			::yggr::container_ex::is_allocator<RetAlloc> >,
	//		typename ::yggr::charset::make_utf8_string_t_from_allocator<RetAlloc>::type >::type
	//	to_string(const Basic_String<Char, Traits, Alloc>& format, const RetAlloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//					const ::yggr::string& fmt_charset_name =
	//						::yggr::charset::charset_name_t< Basic_String<Char, Traits, Alloc> >()) const {
	//	typedef Basic_String<Char, Traits, Alloc> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//	return this_type::pro_to_string(str_fmt.data(), alloc); }
	//
	//template<template<typename _Char, typename _Trtaits> class BasicStringView,
	//			typename Char, typename Traits> inline
	//typename
	//	boost::enable_if<
	//		::yggr::charset::is_string_view_t< BasicStringView<Char, Traits> >,
	//		utf8_string >::type
	//	to_string(const BasicStringView<Char, Traits>& format,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//					const ::yggr::string& fmt_charset_name =
	//						::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >() ) const {
	//	typedef BasicStringView<Char, Traits> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//	return this_type::pro_to_string(str_fmt.data()); }
	//
	//template<template<typename _Char, typename _Trtaits> class BasicStringView,
	//			typename Char, typename Traits, typename RetAlloc> inline
	//typename
	//	boost::enable_if<
	//		boost::mpl::and_<
	//			::yggr::charset::is_string_view_t< BasicStringView<Char, Traits> >,
	//			::yggr::container_ex::is_allocator<RetAlloc> >,
	//		typename ::yggr::charset::make_utf8_string_t_from_allocator<RetAlloc>::type >::type
	//	to_string(const BasicStringView<Char, Traits>& format, const RetAlloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(),
	//					const ::yggr::string& fmt_charset_name =
	//						::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >()) const {
	//	typedef BasicStringView<Char, Traits> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//	return this_type::pro_to_string(str_fmt.data(), alloc); }
	//
	//template<typename T1, typename T2> inline
	//utf8_string to_string(const ::yggr::charset::string_charset_helper<T1, T2>& format,
	//							const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const {
	//	typedef ::yggr::charset::string_charset_helper<T1, T2> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format)));
	//	return this_type::pro_to_string(str_fmt.data()); }
	//
	//template<typename T1, typename T2, typename Alloc> inline
	//typename
	//	boost::enable_if<
	//		::yggr::container_ex::is_allocator<Alloc>,
	//		typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type
	//	to_string(const ::yggr::charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const {
	//	typedef ::yggr::charset::string_charset_helper<T1, T2> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format)));
	//	return this_type::pro_to_string(str_fmt.data(), alloc); }
	//
	//template<typename String> inline
	//typename
	//	boost::enable_if<
	//		::yggr::charset::is_string_t<String>,
	//		String >::type
	//	to_string(void) const {
	//	typedef String ret_type;
	//	return this_type::pro_to_string(0).str<ret_type>(); }
	//
	//template<typename String> inline
	//typename
	//	boost::enable_if<
	//		::yggr::charset::is_string_t<String>,
	//		String >::type
	//	to_string(const char* format,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const {
	//	typedef String ret_type;
	//	return
	//		fmt_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()?
	//			this_type::pro_to_string(format).str<ret_type>(ret_charset_name)
	//			: this_type::pro_to_string(format? utf8_string(format, fmt_charset_name).data() : format).str<ret_type>(ret_charset_name); }
	//
	//template<typename String, typename Alloc> inline
	//typename
	//	boost::enable_if<
	//		boost::mpl::and_<
	//			::yggr::charset::is_string_t<String>,
	//			::yggr::container_ex::is_allocator<Alloc> >,
	//		String >::type
	//	to_string(const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const {
	//	typedef String ret_type;
	//	return this_type::pro_to_string(0).str<ret_type>(alloc, ret_charset_name); }
	//
	//template<typename String, typename Alloc> inline
	//typename
	//	boost::enable_if<
	//		boost::mpl::and_<
	//			::yggr::charset::is_string_t<String>,
	//			::yggr::container_ex::is_allocator<Alloc> >,
	//		String >::type
	//	to_string(const char* format, const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const {
	//	typedef String ret_type;
	//	return
	//		fmt_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()?
	//			this_type::pro_to_string(format).str<ret_type>(alloc, ret_charset_name)
	//			: this_type::pro_to_string(format? utf8_string(format, fmt_charset_name).data() : format).str<ret_type>(alloc, ret_charset_name); }
	//
	//template<typename String, typename Char> inline
	//typename
	//	boost::enable_if<
	//		::yggr::charset::is_string_t<String>,
	//		String >::type
	//	to_string(const Char* format,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const {
	//	typedef String ret_type;
	//	typedef Char* now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//	return this_type::pro_to_string(str_fmt.data()).str<ret_type>(ret_charset_name); }
	//
	//template<typename String, typename Char, typename Alloc> inline
	//typename
	//	boost::enable_if<
	//		boost::mpl::and_<
	//			::yggr::charset::is_string_t<String>,
	//			::yggr::container_ex::is_allocator<Alloc> >,
	//		String >::type
	//	to_string(const Char* format, const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const {
	//	typedef String ret_type;
	//	typedef Char* now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	format_string_type str_fmt(::yggr::charset::string_charset_helper_data(format, fmt_charset_name));
	//	//return this_type::pro_to_string(str_fmt.data()).str<ret_type>(alloc, ret_charset_name)));
	//	return ret_type(alloc); }
	//
	//template<typename String,
	//			template<typename _Char, typename _Trtaits, typename _Alloc> class Basic_String,
	//			typename Char, typename Traits, typename Alloc> inline
	//typename
	//	boost::enable_if<
	//		boost::mpl::and_<
	//			::yggr::charset::is_string_t< String >,
	//			::yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> > >,
 //			String >::type
	//	to_string(const Basic_String<Char, Traits, Alloc>& format,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name =
	//						::yggr::charset::charset_name_t< Basic_String<Char, Traits, Alloc> >() ) const {
	//	typedef String ret_type;
	//	typedef Basic_String<Char, Traits, Alloc> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//	return this_type::pro_to_string(str_fmt.data()).str<ret_type>(ret_charset_name); }
	//
	//template<typename String,
	//			template<typename _Char, typename _Trtaits, typename _Alloc> class Basic_String,
	//			typename Char, typename Traits, typename Alloc, typename RetAlloc> inline
	//typename
	//	boost::enable_if<
	//		boost::mpl::and_<
	//			::yggr::charset::is_string_t<String>,
	//			::yggr::charset::is_string_t<Basic_String<Char, Traits, Alloc> >,
	//			::yggr::container_ex::is_allocator<Alloc> >,
	//		String >::type
	//	to_string(const Basic_String<Char, Traits, Alloc>& format, const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name =
	//						::yggr::charset::charset_name_t< Basic_String<Char, Traits, Alloc> >() ) const {
	//	typedef String ret_type;
	//	typedef Basic_String<Char, Traits, Alloc> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//	return this_type::pro_to_string(str_fmt.data()).str<ret_type>(alloc, ret_charset_name); }
	//
	//template<typename String,
	//			template<typename _Char, typename _Trtaits> class BasicStringView,
	//			typename Char, typename Traits> inline
	//typename
	//	boost::enable_if<
	//		boost::mpl::and_<
	//			::yggr::charset::is_string_t< String >,
	//			::yggr::charset::is_string_view_t< BasicStringView<Char, Traits> > >,
 //			String >::type
	//	to_string(const BasicStringView<Char, Traits>& format,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name =
	//						::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >() ) const {
	//	typedef String ret_type;
	//	typedef BasicStringView<Char, Traits> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//	return this_type::pro_to_string(str_fmt.data()).str<ret_type>(ret_charset_name); }
	//
	//template<typename String,
	//			template<typename _Char, typename _Trtaits> class BasicStringView,
	//			typename Char, typename Traits, typename RetAlloc> inline
	//typename
	//	boost::enable_if<
	//		boost::mpl::and_<
	//			::yggr::charset::is_string_t<String>,
	//			::yggr::charset::is_string_view_t<BasicStringView<Char, Traits> >,
	//			::yggr::container_ex::is_allocator<RetAlloc> >,
	//		String >::type
	//	to_string(const BasicStringView<Char, Traits>& format, const RetAlloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String),
	//					const ::yggr::string& fmt_charset_name =
	//						::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >() ) const {
	//	typedef String ret_type;
	//	typedef BasicStringView<Char, Traits> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name)));
	//	return this_type::pro_to_string(str_fmt.data()).str<ret_type>(alloc, ret_charset_name); }
	//
	//template<typename String, typename T1, typename T2> inline
	//typename boost::enable_if< ::yggr::charset::is_string_t<String>, String>::type
	//	to_string(const ::yggr::charset::string_charset_helper<T1, T2>& format,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String)) const {
	//	typedef String ret_type;
	//	typedef ::yggr::charset::string_charset_helper<T1, T2> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format)));
	//	return this_type::pro_to_string(str_fmt.data()).str<ret_type>(ret_charset_name); }
	//
	//template<typename String, typename T1, typename T2, typename Alloc> inline
	//typename
	//	boost::enable_if<
	//		boost::mpl::and_<
	//			::yggr::charset::is_string_t<String>,
	//			::yggr::container_ex::is_allocator<Alloc> >,
	//		String >::type
	//	to_string(const ::yggr::charset::string_charset_helper<T1, T2>& format, const Alloc& alloc,
	//					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const {
	//	typedef String ret_type;
	//	typedef ::yggr::charset::string_charset_helper<T1, T2> now_fmt_string_type;
	//	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type);
	//	format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format)));
	//	return this_type::pro_to_string(str_fmt.data()).str<ret_type>(alloc, ret_charset_name); }

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
		::yggr::nsql_database_system::bson_date_time_format::tm_fmt_iso8601_local,
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
	std::tm to_tm(void) const;
	std::tm to_local_tm(void) const;

private:
	YGGR_PP_FRIEND_CLASS( bson_inner_data_accesser<true> );

	//save and load def
	template<typename Bson, typename String > inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson>,
				charset::utf8_string_constructable< String >
			>,
			bool
		>::type
		save(Bson& bs, const String& name) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(bson_native_ex::s_bson_is_nil(&bs))
		{
			bson_init(&bs);
		}

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		return bson_append_date_time(&bs, str_name.data(), str_name.size(), _date);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		return bson_native_ex::s_bson_save_value_of_date(&bs, _date);
	}

	template<typename BsonIter, typename String> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				charset::utf8_string_constructable< String >
			>,
			void
		>::type
		load(const BsonIter& iter, const String& name)
	{
		assert(iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name));

		if(!iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		_date = bson_iter_date_time(&iter);
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		_date = bson_iter_date_time(&iter);
	}

private:
	base_type _date; // ms
};

// non member operator foo
// operator==
inline bool operator==(c_bson_date::base_type l, const c_bson_date& r)
{
	assert(((r.compare_eq(l)) == (l == r.date())));
	return l == r.date();
}

inline bool operator==(const c_bson_date& l, c_bson_date::base_type r)
{
	assert(((l.compare_eq(r)) == (l.date() == r)));
	return l.date() == r;
}

inline bool operator==(const c_bson_date& l, const c_bson_date& r)
{
	assert(((l.compare_eq(r)) == (l.date() == r.date())));
	return l.date() == r.date();
}

// operator!=
inline bool operator!=(const c_bson_date::base_type l, const c_bson_date& r)
{
	assert(((!r.compare_eq(l)) == (l != r.date())));
	return l != r.date();
}

inline bool operator!=(const c_bson_date& l, const c_bson_date::base_type r)
{
	assert(((!l.compare_eq(r)) == (l.date() != r)));
	return l.date() != r;
}

inline bool operator!=(const c_bson_date& l, const c_bson_date& r)
{
	assert(((!l.compare_eq(r)) == (l.date() != r)));
	return l.date() != r.date();
}

// operator <
inline bool operator<(const c_bson_date::base_type l, const c_bson_date& r)
{
	assert((( 0 < r.compare(l) ) == (l < r.date())));
	return l < r.date();
}

inline bool operator<(const c_bson_date& l, const c_bson_date::base_type r)
{
	assert(((l.compare(r) < 0) == (l.date() < r)));
	return l.date() < r;
}

inline bool operator<(const c_bson_date& l, const c_bson_date& r)
{
	assert(((l.compare(r) < 0) == (l.date() < r.date())));
	return l.date() < r.date();
}

// operator <=
inline bool operator<=(const c_bson_date::base_type l, const c_bson_date& r)
{
	assert(((r.compare(l) >= 0) == (l <= r.date())));
	return l <= r.date();
}

inline bool operator<=(const c_bson_date& l, const c_bson_date::base_type r)
{
	assert(((l.compare(r) <= 0) == (l.date() <= r)));
	return l.date() <= r;
}

inline bool operator<=(const c_bson_date& l, const c_bson_date& r)
{
	assert(((l.compare(r) <= 0) == (l.date() <= r.date())));
	return l.date() <= r.date();
}

// operator >
inline bool operator>(const c_bson_date::base_type l, const c_bson_date& r)
{
	assert(((r.compare(l) < 0) == (l > r.date())));
	return l > r.date();
}

inline bool operator>(const c_bson_date& l, const c_bson_date::base_type r)
{
	assert(((l.compare(r) > 0) == (l.date() > r)));
	return l.date() > r;
}

inline bool operator>(const c_bson_date& l, const c_bson_date& r)
{
	assert(((l.compare(r) > 0) == (l.date() > r.date())));
	return l.date() > r.date();
}

// operator >=
inline bool operator>=(const c_bson_date::base_type l, const c_bson_date& r)
{
	assert(((r.compare(l) <= 0) == (l >= r.date())));
	return l >= r.date();
}

inline bool operator>=(const c_bson_date& l, const c_bson_date::base_type r)
{
	assert(((l.compare(r) >= 0) == (l.date() >= r)));
	return l.date() >= r;
}

inline bool operator>=(const c_bson_date& l, const c_bson_date& r)
{
	assert(((l.compare(r) >= 0) == (l.date() >= r.date())));
	return l.date() >= r.date();
}

// c_bson_date <=> c_bson_time
bool operator==(const c_bson_date& l, const c_bson_time& r);
bool operator!=(const c_bson_date& l, const c_bson_time& r);
bool operator<(const c_bson_date& l, const c_bson_time& r);
bool operator<=(const c_bson_date& l, const c_bson_time& r);
bool operator>(const c_bson_date& l, const c_bson_time& r);
bool operator>=(const c_bson_date& l, const c_bson_time& r);

// c_bson_date <=> c_bson_timeval
template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator==(const c_bson_date& l, const BsonTimeval& r)
{
	return l.compare_eq(r);
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator!=(const c_bson_date& l, const BsonTimeval& r)
{
	return !l.compare_eq(r);
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator<(const c_bson_date& l, const BsonTimeval& r)
{
	return l.compare(r) < 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator<=(const c_bson_date& l, const BsonTimeval& r)
{
	return l.compare(r) <= 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator>(const c_bson_date& l, const BsonTimeval& r)
{
	return l.compare(r) > 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator>=(const c_bson_date& l, const BsonTimeval& r)
{
	return l.compare(r) >= 0;
}

// c_bson_date <=> xtime
inline bool operator==(const c_bson_date& l, const c_bson_date::bridge_time_type& r)
{
	return l.compare_eq(r);
}

inline bool operator!=(const c_bson_date& l, const c_bson_date::bridge_time_type& r)
{
	return !l.compare_eq(r);
}

inline bool operator<(const c_bson_date& l, const c_bson_date::bridge_time_type& r)
{
	return l.compare(r) < 0;
}

inline bool operator<=(const c_bson_date& l, const c_bson_date::bridge_time_type& r)
{
	return l.compare(r) <= 0;
}

inline bool operator>(const c_bson_date& l, const c_bson_date::bridge_time_type& r)
{
	return l.compare(r) > 0;
}

inline bool operator>=(const c_bson_date& l, const c_bson_date::bridge_time_type& r)
{
	return l.compare(r) >= 0;
}

// xtime <=> c_bson_date
inline bool operator==(const c_bson_date::bridge_time_type& l, const c_bson_date& r)
{
	return r.compare_eq(l);
}

inline bool operator!=(const c_bson_date::bridge_time_type& l, const c_bson_date& r)
{
	return !r.compare_eq(l);
}

inline bool operator<(const c_bson_date::bridge_time_type& l, const c_bson_date& r)
{
	return r.compare(l) > 0;
}

inline bool operator<=(const c_bson_date::bridge_time_type& l, const c_bson_date& r)
{
	return r.compare(l) >= 0;
}

inline bool operator>(const c_bson_date::bridge_time_type& l, const c_bson_date& r)
{
	return r.compare(l) < 0;
}

inline bool operator>=(const c_bson_date::bridge_time_type& l, const c_bson_date& r)
{
	return r.compare(l) <= 0;
}

// c_bson_date <=> c_bson_value
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_date& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_date& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const c_bson_date& l, const BsonValue& r)
{
	return l.compare(r) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const c_bson_date& l, const BsonValue& r)
{
	return l.compare(r) <= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const c_bson_date& l, const BsonValue& r)
{
	return l.compare(r) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const c_bson_date& l, const BsonValue& r)
{
	return l.compare(r) >= 0;
}

// c_bson_value <=> c_bson_date
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_date& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_date& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const BsonValue& l, const c_bson_date& r)
{
	return r.compare(l) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const BsonValue& l, const c_bson_date& r)
{
	return r.compare(l) >= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const BsonValue& l, const c_bson_date& r)
{
	return r.compare(l) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const BsonValue& l, const c_bson_date& r)
{
	return r.compare(l) <= 0;
}

// os << val
namespace detail
{

template<>
struct bson_operator_outter<c_bson_date>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_bson_date& val,
					bson_json_mode_def::mode_type mode) const
	{
		switch(mode)
		{
		case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
			if(!(val.date() < 0))
			{
				os << "{ \"$date\" : \""
					<< val.to_string()
					<< "\" }";
				break;
			}
		case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
			os << "{ \"$date\" : { \"$numberLong\" : \"" << val.date() << "\" } }";
			break;
		default:
			os << "{ \"$date\" : " << val.date() << " }";
		}

		return os;
	}
};


} // namespace detail

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_date& val)
{
	typedef detail::bson_operator_outter<c_bson_date> outter_type;

	outter_type outter;
	return outter(os, val, bson_json_mode_def::E_BSON_JSON_MODE_LEGACY);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_bson_date) val)
//{
//	const c_bson_date& right_cref = val;
//	return yggr::nsql_database_system::operator<<(os, right_cref);
//}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

inline void swap(c_bson_date::base_type& l, c_bson_date& r)
{
	r.swap(l);
}

inline void swap(c_bson_date& l, c_bson_date::base_type& r)
{
	l.swap(r);
}

inline void swap(c_bson_date& l, c_bson_date& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_DATE_HPP__
