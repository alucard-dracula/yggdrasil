//c_bson_timeval.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_TIMEVAL_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_TIMEVAL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/charset/generalized_string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/ppex/friend.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>
#include <yggr/nsql_database_system/bson_timeval_native_ex.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>
#include <yggr/nsql_database_system/detail/bson_date_time_to_string_macro.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/thread/xtime.hpp>

#include <cassert>

namespace yggr
{
namespace nsql_database_system
{

class c_bson_date;
class c_bson_time;
class c_bson_timeval;

} // namespace nsql_database_system
} // namespace yggr


YGGR_PP_BSON_WRAP_TYPEID_IMPL(timeval, c_bson_timeval, bson_typeid_def::E_BSON_TYPE_DATE_TIME)
YGGR_PP_BSON_TYPEID_IMPL(c_bson_timeval, bson_typeid_def::E_BSON_TYPE_DATE_TIME)

namespace yggr
{
namespace nsql_database_system
{

//struct timeval
//{
//	long    tv_sec;         /* seconds */
//	long    tv_usec;        /* and microseconds */
//};

///************* c_bson_timeval accurate to the millisecond *************/
// !!! mongodb strand time unit is millisecond (c_bson_date) !!!
// !!! so using save load operation will lose data precision !!!
class c_bson_timeval
	: public timeval
{
public:
	typedef timeval base_type;
	typedef base_type org_type;
	typedef boost::xtime bridge_time_type;

#if defined(BSON_OS_WIN32)
	typedef long base_sec_val_type;
	typedef long base_usec_val_type;
#elif defined(YGGR_AT_DARWIN)
	typedef long base_sec_val_type;
	typedef int base_usec_val_type;
#else
	typedef std::time_t base_sec_val_type;
	typedef std::time_t base_usec_val_type;
#endif // BSON_OS_WIN32

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
	typedef c_bson_timeval this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef
		utility::member_var_modify_helper
		<
			this_type,
			base_usec_val_type,
			base_usec_val_type,
			base_usec_val_type
		> tv_usec_modify_helper_type;

public:
	c_bson_timeval(void);
	c_bson_timeval(base_sec_val_type sec, base_usec_val_type usec);
	c_bson_timeval(const bson_value_t& val);

	c_bson_timeval(const c_bson_date& right);
	c_bson_timeval(const c_bson_time& right);
	c_bson_timeval(const bridge_time_type& right); // support time_cast

	c_bson_timeval(BOOST_RV_REF(base_type) right)
	{
		base_type::tv_sec = right.tv_sec;
		base_type::tv_usec = right.tv_usec;
	}

	c_bson_timeval(const base_type& right);

	c_bson_timeval(BOOST_RV_REF(this_type) right)
	{
		base_type::tv_sec = right.tv_sec;
		base_type::tv_usec = right.tv_usec;
	}

	c_bson_timeval(const this_type& right);
	~c_bson_timeval(void);

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	this_type& operator=(const c_bson_date& right);
	this_type& operator=(const c_bson_time& right);
	this_type& operator=(const bridge_time_type& right); // support time_cast

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimeval>, this_type&>::type
		operator=(BOOST_RV_REF(BsonTimeval) right)
	{
		BsonTimeval& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::tv_sec = right_ref.tv_sec;
		base_type::tv_usec = right_ref.tv_usec;
		return *this;
	}

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimeval>, this_type&>::type
		operator=(const BsonTimeval& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		base_type::tv_sec = right.tv_sec;
		base_type::tv_usec = right.tv_usec;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::tv_sec = right_ref.tv_sec;
		base_type::tv_usec = right_ref.tv_usec;
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimeval>, void>::type
		swap(BOOST_RV_REF(BsonTimeval) right)
	{
		BsonTimeval& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return;
		}

		yggr::swap(base_type::tv_sec, right_ref.tv_sec);
		yggr::swap(base_type::tv_usec, right_ref.tv_usec);
	}

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimeval>, void>::type
		swap(BsonTimeval& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		yggr::swap(base_type::tv_sec, right.tv_sec);
		yggr::swap(base_type::tv_usec, right.tv_usec);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return;
		}

		yggr::swap(base_type::tv_sec, right_ref.tv_sec);
		yggr::swap(base_type::tv_usec, right_ref.tv_usec);
	}

	void swap(this_type& right);

protected:
	void pro_assign_bson_value_t(const bson_value_t& val);

public:
	inline org_type* org_pointer(void)
	{
		return static_cast<org_type*>(this);
	}

	inline const org_type* org_pointer(void) const
	{
		return static_cast<const org_type*>(this);
	}

public:
	inline operator bridge_time_type(void) const // support time_cast
	{
		return this_type::bridge_time();
	}

public:
	inline u64 time(void) const
	{
		return static_cast<u64>(base_type::tv_sec);
	}

	inline u64 date(void) const
	{
		return
			static_cast<u64>(base_type::tv_sec) * this_type::E_S_AND_MS_RATIO
			+ static_cast<u64>(base_type::tv_usec) / this_type::E_MS_AND_MCS_RATIO;
	}

	inline bridge_time_type bridge_time(void) const
	{
		bridge_time_type tm =
		{
			static_cast<bridge_time_type::xtime_sec_t>(base_type::tv_sec),
			static_cast<bridge_time_type::xtime_nsec_t>(base_type::tv_usec * this_type::E_MCS_AND_NS_RATIO)
		};

		return tm;
	}

public:
	// compare_eq
	inline bool compare_eq(base_sec_val_type ntv_sec, base_usec_val_type ntv_usec) const
	{
		return (base_type::tv_sec == ntv_sec) && (base_type::tv_usec == ntv_usec);
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& val) const
	{
		return bson_value_native_ex::s_value_cmp_eq_timeval(boost::addressof(val), this);
	}

	bool compare_eq(const c_bson_date& val) const;
	bool compare_eq(const c_bson_time& val) const;

	inline bool compare_eq(const bridge_time_type& val) const
	{
		return
			bson_date_time_native_ex::timeval_xtime_compare_eq(
				base_type::tv_sec, base_type::tv_usec, val.sec, val.nsec);
	}

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimeval>, bool>::type
		compare_eq(const BsonTimeval& right) const
	{
		return this_type::compare_eq(right.tv_sec, right.tv_usec);
	}

	// compare
	inline s32 compare(base_sec_val_type ntv_sec, base_usec_val_type ntv_usec) const
	{
		return
			base_type::tv_sec == ntv_sec?
				base_type::tv_usec == ntv_sec?
					0
					: base_type::tv_usec < ntv_usec? -1 : 1
				: base_type::tv_sec < ntv_sec? -1 : 1;
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, s32>::type
		compare(const BsonValue& val) const
	{
		return -bson_value_native_ex::s_value_cmp_timeval(boost::addressof(val), this);
	}

	s32 compare(const c_bson_date& val) const;
	s32 compare(const c_bson_time& val) const;

	inline s32 compare(const bridge_time_type& val) const
	{
		return
			bson_date_time_native_ex::timeval_xtime_compare(
				base_type::tv_sec, base_type::tv_usec, val.sec, val.nsec);
	}

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimeval>, s32>::type
		compare(const BsonTimeval& right) const
	{
		return this_type::compare(right.tv_sec, right.tv_usec);
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

public:
	inline base_sec_val_type& var_tv_sec(void)
	{
		return base_type::tv_sec;
	}

	inline base_sec_val_type var_tv_sec(void) const
	{
		return base_type::tv_sec;
	}

	inline tv_usec_modify_helper_type var_tv_usec(void)
	{
		return
			tv_usec_modify_helper_type(
				*this, base_type::tv_usec,
				&this_type::set_var_tv_usec,
				&this_type::get_var_tv_usec);
	}

	inline base_usec_val_type var_tv_usec(void) const
	{
		return this_type::get_var_tv_usec();
	}

protected:
	inline base_usec_val_type get_var_tv_usec(void) const
	{
		return base_type::tv_usec;
	}

	inline void set_var_tv_usec(base_usec_val_type usec)
	{
		base_type::tv_usec = usec;
	}

	inline void set_var_tv_usec_of_ms(std::time_t ms)
	{
		u64 tmp = ms * this_type::E_MS_AND_MCS_RATIO;
		base_type::tv_usec = static_cast<base_usec_val_type>(tmp);
	}

	inline void set_var_tv_usec_of_ns(std::time_t ns)
	{
		u64 tmp = ns / this_type::E_MCS_AND_NS_RATIO;
		base_type::tv_usec = static_cast<base_usec_val_type>(tmp);
	}

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

		base_type& base = const_cast<this_type&>(*this);
		now_inner_string_type str_name((charset::string_charset_helper_data(name)));

		return bson_append_timeval(&bs, str_name.data(), str_name.size(), &base);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		u64 date = static_cast<u64>(base_type::tv_sec) * this_type::E_S_AND_MS_RATIO
					+ static_cast<u64>(base_type::tv_usec) / this_type::E_MS_AND_MCS_RATIO;
		return bson_native_ex::s_bson_save_value_of_date(&bs, date);
	}

	template<typename BsonIter, typename String > inline
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

		base_type& base = *this;
		bson_iter_timeval(&iter, &base);
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		base_type& base = *this;
		bson_iter_timeval(&iter, &base);
	}
};

// non member operator foo
// c_bson_timeval <=> BsonTimeval
template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator==(const c_bson_timeval& l, const BsonTimeval& r)
{
	return l.compare_eq(r);
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator!=(const c_bson_timeval& l, const BsonTimeval& r)
{
	return !l.compare_eq(r);
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator<(const c_bson_timeval& l, const BsonTimeval& r)
{
	return l.compare(r) < 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator<=(const c_bson_timeval& l, const BsonTimeval& r)
{
	return l.compare(r) <= 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator>(const c_bson_timeval& l, const BsonTimeval& r)
{
	return l.compare(r) > 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator>=(const c_bson_timeval& l, const BsonTimeval& r)
{
	return l.compare(r) >= 0;
}


// c_bson_timeval <=> c_bson_time
bool operator==(const c_bson_timeval& l, const c_bson_date& r);
bool operator!=(const c_bson_timeval& l, const c_bson_date& r);
bool operator<(const c_bson_timeval& l, const c_bson_date& r);
bool operator<=(const c_bson_timeval& l, const c_bson_date& r);
bool operator>(const c_bson_timeval& l, const c_bson_date& r);
bool operator>=(const c_bson_timeval& l, const c_bson_date& r);

// c_bson_timeval <=> c_bson_time
bool operator==(const c_bson_timeval& l, const c_bson_time& r);
bool operator!=(const c_bson_timeval& l, const c_bson_time& r);
bool operator<(const c_bson_timeval& l, const c_bson_time& r);
bool operator<=(const c_bson_timeval& l, const c_bson_time& r);
bool operator>(const c_bson_timeval& l, const c_bson_time& r);
bool operator>=(const c_bson_timeval& l, const c_bson_time& r);

// c_bson_timeval <=> xtime
inline bool operator==(const c_bson_timeval& l, const c_bson_timeval::bridge_time_type& r)
{
	return l.compare_eq(r);
}

inline bool operator!=(const c_bson_timeval& l, const c_bson_timeval::bridge_time_type& r)
{
	return !l.compare_eq(r);
}

inline bool operator<(const c_bson_timeval& l, const c_bson_timeval::bridge_time_type& r)
{
	return l.compare(r) < 0;
}

inline bool operator<=(const c_bson_timeval& l, const c_bson_timeval::bridge_time_type& r)
{
	return l.compare(r) <= 0;
}

inline bool operator>(const c_bson_timeval& l, const c_bson_timeval::bridge_time_type& r)
{
	return l.compare(r) > 0;
}

inline bool operator>=(const c_bson_timeval& l, const c_bson_timeval::bridge_time_type& r)
{
	return l.compare(r) >= 0;
}

// xtime <=> c_bson_timeval
inline bool operator==(const c_bson_timeval::bridge_time_type& l, const c_bson_timeval& r)
{
	return r.compare_eq(l);
}

inline bool operator!=(const c_bson_timeval::bridge_time_type& l, const c_bson_timeval& r)
{
	return !r.compare_eq(l);
}

inline bool operator<(const c_bson_timeval::bridge_time_type& l, const c_bson_timeval& r)
{
	return r.compare(l) > 0;
}

inline bool operator<=(const c_bson_timeval::bridge_time_type& l, const c_bson_timeval& r)
{
	return r.compare(l) >= 0;
}

inline bool operator>(const c_bson_timeval::bridge_time_type& l, const c_bson_timeval& r)
{
	return r.compare(l) < 0;
}

inline bool operator>=(const c_bson_timeval::bridge_time_type& l, const c_bson_timeval& r)
{
	return r.compare(l) <= 0;
}

// bson_value <=> c_bson_timeval
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_timeval& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_timeval& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const BsonValue& l, const c_bson_timeval& r)
{
	return r.compare(l) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const BsonValue& l, const c_bson_timeval& r)
{
	return r.compare(l) >= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const BsonValue& l, const c_bson_timeval& r)
{
	return r.compare(l) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const BsonValue& l, const c_bson_timeval& r)
{
	return r.compare(l) <= 0;
}

// c_bson_timeval <=> bson_value
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_timeval& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_timeval& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const c_bson_timeval& l, const BsonValue& r)
{
	return l.compare(r) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const c_bson_timeval& l, const BsonValue& r)
{
	return l.compare(r) <= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const c_bson_timeval& l, const BsonValue& r)
{
	return l.compare(r) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const c_bson_timeval& l, const BsonValue& r)
{
	return l.compare(r) >= 0;
}

namespace detail
{

template<>
struct bson_operator_outter<c_bson_timeval>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_bson_timeval& val,
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

// os << val
template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_timeval& val)
{
	typedef detail::bson_operator_outter<c_bson_timeval> outter_type;

	outter_type outter;
	return outter(os, val, bson_json_mode_def::E_BSON_JSON_MODE_LEGACY);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_bson_timeval) right)
//{
//	const c_bson_timeval& right_cref = right;
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

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_timeval& l, timeval& r)
{
	l.swap(r);
}

#else

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, void>::type
	swap(c_bson_timeval& l, BsonTimeval& r)
{
	l.swap(r);
}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_timeval& l, c_bson_timeval& r)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_TIMEVAL_HPP__
