//c_bson_time.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_TIME_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_TIME_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/time/time_config.hpp>
#include <yggr/time/time_ratio_def.hpp>
#include <yggr/charset/generalized_string.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/container_ex/is_allocator.hpp>
#include <yggr/container_ex/allocator_traits.hpp>
#include <yggr/type_traits/native_t.hpp>

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

namespace yggr
{
namespace nsql_database_system
{

class c_bson_date;
class c_bson_time;
class c_bson_timeval;

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_TYPEID_IMPL(c_bson_time, bson_typeid_def::E_BSON_TYPE_DATE_TIME)

namespace yggr
{
namespace nsql_database_system
{

// !!! mongodb strand time unit is millisecond (c_bson_date) !!!

class c_bson_time
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
	typedef c_bson_time this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_bson_time(void);
	c_bson_time(base_type right);
	c_bson_time(const bson_value_t& val);

	c_bson_time(const c_bson_date& right);

	c_bson_time(const timeval& right);
	c_bson_time(const c_bson_timeval& right);

	c_bson_time(const bridge_time_type& right); // support time_cast

	c_bson_time(BOOST_RV_REF(this_type) right)
		: _time(right._time)
	{
	}

	c_bson_time(const this_type& right);
	~c_bson_time(void);

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	this_type& operator=(base_type right);

	this_type& operator=(const c_bson_date& right);

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, this_type&>::type
		operator=(const BsonTimeval& right)
	{
		_time = right.tv_sec;
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

		_time = right._time;
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
    // base_type is s64
#if defined(_MSC_VER) || (YGGR_CPP_VER_11 <= YGGR_CPP_VERSION)
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
		return _time;
	}

	inline operator base_type(void) const
	{
		return _time;
	}

	inline operator bridge_time_type(void) const // support time_cast
	{
		return this_type::bridge_time();
	}

public:
	inline base_type& time(void)
	{
		return _time;
	}

	inline base_type time(void) const
	{
		return _time;
	}

	inline base_type date(void) const
	{
		return _time * this_type::E_S_AND_MS_RATIO;
	}

	inline bridge_time_type bridge_time(void) const
	{
		bridge_time_type tm =
		{
			static_cast<bridge_time_type::xtime_sec_t>(_time),
			0
		};

		return tm;
	}

public:
	// compare_eq
	inline bool compare_eq(base_type right) const
	{
		return _time == right;
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& val) const
	{
		return bson_value_native_ex::s_value_cmp_eq_time(boost::addressof(val), _time);
	}

	bool compare_eq(const c_bson_date& val) const;

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
		compare_eq(const BsonTimeval& val) const
	{
		return
			bson_date_time_native_ex::time_timeval_compare_eq(
				_time, val.tv_sec, val.tv_usec);
	}

	inline bool compare_eq(const bridge_time_type& val) const
	{
		return
			bson_date_time_native_ex::time_xtime_compare_eq(
				_time, val.sec, val.nsec);
	}

	inline bool compare_eq(const this_type& right) const
	{
		return _time == right._time;
	}

	// compare
	inline s32 compare(base_type right) const
	{
		return _time == right? 0 : _time < right? -1 : 1;
	}

	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, s32>::type
		compare(const BsonValue& val) const
	{
		return -bson_value_native_ex::s_value_cmp_time(boost::addressof(val), _time);
	}

	s32 compare(const c_bson_date& val) const;

	template<typename BsonTimeval> inline
	typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, s32>::type
		compare(const BsonTimeval& val) const
	{
		return
			bson_date_time_native_ex::time_timeval_compare(
				_time, val.tv_sec, val.tv_usec);
	}

	inline s32 compare(const bridge_time_type& val) const
	{
		return
			bson_date_time_native_ex::time_xtime_compare(
				_time, val.sec, val.nsec);
	}

	inline s32 compare(const this_type& right) const
	{
		return _time == right._time? 0 : _time < right._time? -1 : 1;
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
		return bson_append_time_t(&bs, str_name.data(), str_name.size(), _time);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		u64 date = _time * this_type::E_S_AND_MS_RATIO;
		return bson_native_ex::s_bson_save_value_of_date(&bs, date);
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
		_time = bson_iter_time_t(&iter);
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		_time = bson_iter_time_t(&iter);
	}

private:
	base_type _time; // s
};

// non member operator foo
// operator==
inline bool operator==(const c_bson_time::base_type l, const c_bson_time& r)
{
	assert(((r.compare_eq(l)) == (l == r.time())));
	return l == r.time();
}

inline bool operator==(const c_bson_time& l, const c_bson_time::base_type r)
{
	assert(((l.compare_eq(r)) == (l.time() == r)));
	return l.time() == r;
}

inline bool operator==(const c_bson_time& l, const c_bson_time& r)
{
	assert(((l.compare_eq(r)) == (l.time() == r.time())));
	return l.time() == r.time();
}

// operator!=
inline bool operator!=(const c_bson_time::base_type l, const c_bson_time& r)
{
	assert(((!r.compare_eq(l)) == (l != r.time())));
	return l != r.time();
}

inline bool operator!=(const c_bson_time& l, const c_bson_time::base_type r)
{
	assert(((!l.compare_eq(r)) == (l.time() != r)));
	return l.time() != r;
}

inline bool operator!=(const c_bson_time& l, const c_bson_time& r)
{
	assert(((!l.compare_eq(r)) == (l.time() != r)));
	return l.time() != r.time();
}

// operator <
inline bool operator<(const c_bson_time::base_type l, const c_bson_time& r)
{
	assert(((0 < r.compare(l) ) == (l < r.time())));
	return l < r.time();
}

inline bool operator<(const c_bson_time& l, const c_bson_time::base_type r)
{
	assert(((l.compare(r) < 0) == (l.time() < r)));
	return l.time() < r;
}

inline bool operator<(const c_bson_time& l, const c_bson_time& r)
{
	assert(((l.compare(r) < 0) == (l.time() < r.time())));
	return l.time() < r.time();
}

// operator <=
inline bool operator<=(const c_bson_time::base_type l, const c_bson_time& r)
{
	assert(((r.compare(l) >= 0) == (l <= r.time())));
	return l <= r.time();
}

inline bool operator<=(const c_bson_time& l, const c_bson_time::base_type r)
{
	assert(((l.compare(r) <= 0) == (l.time() <= r)));
	return l.time() <= r;
}

inline bool operator<=(const c_bson_time& l, const c_bson_time& r)
{
	assert(((l.compare(r) <= 0) == (l.time() <= r.time())));
	return l.time() <= r.time();
}

// operator >
inline bool operator>(const c_bson_time::base_type l, const c_bson_time& r)
{
	assert(((r.compare(l) < 0) == (l > r.time())));
	return l > r.time();
}

inline bool operator>(const c_bson_time& l, const c_bson_time::base_type r)
{
	assert(((l.compare(r) > 0) == (l.time() > r)));
	return l.time() > r;
}

inline bool operator>(const c_bson_time& l, const c_bson_time& r)
{
	assert(((l.compare(r) > 0) == (l.time() > r.time())));
	return l.time() > r.time();
}

// operator >=
inline bool operator>=(const c_bson_time::base_type l, const c_bson_time& r)
{
	assert(((r.compare(l) <= 0) == (l >= r.time())));
	return l >= r.time();
}

inline bool operator>=(const c_bson_time& l, const c_bson_time::base_type r)
{
	assert(((l.compare(r) >= 0) == (l.time() >= r)));
	return l.time() >= r;
}

inline bool operator>=(const c_bson_time& l, const c_bson_time& r)
{
	assert(((l.compare(r) >= 0) == (l.time() >= r.time())));
	return l.time() >= r.time();
}

// c_bson_time <=> c_bson_date
bool operator==(const c_bson_time& l, const c_bson_date& r);
bool operator!=(const c_bson_time& l, const c_bson_date& r);
bool operator<(const c_bson_time& l, const c_bson_date& r);
bool operator<=(const c_bson_time& l, const c_bson_date& r);
bool operator>(const c_bson_time& l, const c_bson_date& r);
bool operator>=(const c_bson_time& l, const c_bson_date& r);

// c_bson_time <=> timeval
template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator==(const c_bson_time& l, const BsonTimeval& r)
{
	return l.compare_eq(r);
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator!=(const c_bson_time& l, const BsonTimeval& r)
{
	return !l.compare_eq(r);
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator<(const c_bson_time& l, const BsonTimeval& r)
{
	return l.compare(r) < 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator<=(const c_bson_time& l, const BsonTimeval& r)
{
	return l.compare(r) <= 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator>(const c_bson_time& l, const BsonTimeval& r)
{
	return l.compare(r) > 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator>=(const c_bson_time& l, const BsonTimeval& r)
{
	return l.compare(r) >= 0;
}

// c_bson_time <=> xtime
inline bool operator==(const c_bson_time& l, const c_bson_time::bridge_time_type& r)
{
	return l.compare_eq(r);
}

inline bool operator!=(const c_bson_time& l, const c_bson_time::bridge_time_type& r)
{
	return !l.compare_eq(r);
}

inline bool operator<(const c_bson_time& l, const c_bson_time::bridge_time_type& r)
{
	return l.compare(r) < 0;
}

inline bool operator<=(const c_bson_time& l, const c_bson_time::bridge_time_type& r)
{
	return l.compare(r) <= 0;
}

inline bool operator>(const c_bson_time& l, const c_bson_time::bridge_time_type& r)
{
	return l.compare(r) > 0;
}

inline bool operator>=(const c_bson_time& l, const c_bson_time::bridge_time_type& r)
{
	return l.compare(r) >= 0;
}

// xtime <=> c_bson_time
inline bool operator==(const c_bson_time::bridge_time_type& l, const c_bson_time& r)
{
	return r.compare_eq(l);
}

inline bool operator!=(const c_bson_time::bridge_time_type& l, const c_bson_time& r)
{
	return !r.compare_eq(l);
}

inline bool operator<(const c_bson_time::bridge_time_type& l, const c_bson_time& r)
{
	return r.compare(l) > 0;
}

inline bool operator<=(const c_bson_time::bridge_time_type& l, const c_bson_time& r)
{
	return r.compare(l) >= 0;
}

inline bool operator>(const c_bson_time::bridge_time_type& l, const c_bson_time& r)
{
	return r.compare(l) < 0;
}

inline bool operator>=(const c_bson_time::bridge_time_type& l, const c_bson_time& r)
{
	return r.compare(l) <= 0;
}

// c_bson_time <=> c_bson_value
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_time& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_time& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const c_bson_time& l, const BsonValue& r)
{
	return l.compare(r) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const c_bson_time& l, const BsonValue& r)
{
	return l.compare(r) <= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const c_bson_time& l, const BsonValue& r)
{
	return l.compare(r) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const c_bson_time& l, const BsonValue& r)
{
	return l.compare(r) >= 0;
}

// c_bson_value <=> c_bson_time

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_time& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_time& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const BsonValue& l, const c_bson_time& r)
{
	return r.compare(l) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const BsonValue& l, const c_bson_time& r)
{
	return r.compare(l) >= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const BsonValue& l, const c_bson_time& r)
{
	return r.compare(l) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const BsonValue& l, const c_bson_time& r)
{
	return r.compare(l) <= 0;
}

namespace detail
{

template<>
struct bson_operator_outter<c_bson_time>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_bson_time& val,
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
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_time& val)
{
	typedef detail::bson_operator_outter<c_bson_time> outter_type;

	outter_type outter;
	return outter(os, val, bson_json_mode_def::E_BSON_JSON_MODE_LEGACY);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_bson_time) val)
//{
//	const c_bson_time& right_cref = val;
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

void swap(c_bson_time::base_type& l, c_bson_time& r);
void swap(c_bson_time& l, c_bson_time::base_type& r);
void swap(c_bson_time& l, c_bson_time& r);

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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_TIME_HPP__
