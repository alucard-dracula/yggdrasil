// bson_timeval_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_TIMEVAL_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_TIMEVAL_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>

#include <yggr/nsql_database_system/bson_date_time_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/thread/xtime.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_bson_date;
class c_bson_time;

class bson_value_native_ex;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class bson_timeval_native_ex
{
public:
	inline static bool timeval_compare_eq(const timeval& l, const timeval& r)
	{
		return (l.tv_sec == r.tv_sec) && (l.tv_usec == r.tv_usec);
	}

	inline static s32 timeval_compare(const timeval& l, const timeval& r)
	{
		return
			l.tv_sec == r.tv_sec?
				l.tv_usec == r.tv_usec?
				0
				: l.tv_usec < r.tv_usec? -1 : 1
			: l.tv_sec < r.tv_sec? -1 : 1;
	}
};

} // namespace nsql_database_system
} // namespace yggr

// timeval <=> BsonTimeval
template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator==(const timeval& l, const BsonTimeval& r)
{
	return yggr::nsql_database_system::bson_timeval_native_ex::timeval_compare_eq(l, r);
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator!=(const timeval& l, const BsonTimeval& r)
{
	return !yggr::nsql_database_system::bson_timeval_native_ex::timeval_compare_eq(l, r);
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator<(const timeval& l, const BsonTimeval& r)
{
	return yggr::nsql_database_system::bson_timeval_native_ex::timeval_compare(l, r) < 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator<=(const timeval& l, const BsonTimeval& r)
{
	return yggr::nsql_database_system::bson_timeval_native_ex::timeval_compare(l, r) <= 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator>(const timeval& l, const BsonTimeval& r)
{
	return yggr::nsql_database_system::bson_timeval_native_ex::timeval_compare(l, r) > 0;
}

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, bool>::type
	operator>=(const timeval& l, const BsonTimeval& r)
{
	return yggr::nsql_database_system::bson_timeval_native_ex::timeval_compare(l, r) >= 0;
}

// timeval <=> c_bson_date
bool operator==(const timeval& l, const yggr::nsql_database_system::c_bson_date& r);
bool operator!=(const timeval& l, const yggr::nsql_database_system::c_bson_date& r);
bool operator<(const timeval& l, const yggr::nsql_database_system::c_bson_date& r);
bool operator<=(const timeval& l, const yggr::nsql_database_system::c_bson_date& r);
bool operator>(const timeval& l, const yggr::nsql_database_system::c_bson_date& r);
bool operator>=(const timeval& l, const yggr::nsql_database_system::c_bson_date& r);

// timeval <=> c_bson_time
bool operator==(const timeval& l, const yggr::nsql_database_system::c_bson_time& r);
bool operator!=(const timeval& l, const yggr::nsql_database_system::c_bson_time& r);
bool operator<(const timeval& l, const yggr::nsql_database_system::c_bson_time& r);
bool operator<=(const timeval& l, const yggr::nsql_database_system::c_bson_time& r);
bool operator>(const timeval& l, const yggr::nsql_database_system::c_bson_time& r);
bool operator>=(const timeval& l, const yggr::nsql_database_system::c_bson_time& r);

// timeval <=> boost::xtime
inline bool operator==(const timeval& l, const boost::xtime& r)
{
	return
		yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare_eq(
			l.tv_sec, l.tv_usec, r.sec, r.nsec);
}

inline bool operator!=(const timeval& l, const boost::xtime& r)
{
	return
		!yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare_eq(
			l.tv_sec, l.tv_usec, r.sec, r.nsec);
}

inline bool operator<(const timeval& l, const boost::xtime& r)
{
	return
		yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare(
			l.tv_sec, l.tv_usec, r.sec, r.nsec) < 0;
}

inline bool operator<=(const timeval& l, const boost::xtime& r)
{
	return
		yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare(
			l.tv_sec, l.tv_usec, r.sec, r.nsec) <= 0;
}

inline bool operator>(const timeval& l, const boost::xtime& r)
{
	return
		yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare(
			l.tv_sec, l.tv_usec, r.sec, r.nsec) > 0;
}

inline bool operator>=(const timeval& l, const boost::xtime& r)
{
	return
		yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare(
			l.tv_sec, l.tv_usec, r.sec, r.nsec) >= 0;
}

// boost::xtime <=> timeval
inline bool operator==(const boost::xtime& l, const timeval& r)
{
	return
		yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare_eq(
			r.tv_sec, r.tv_usec, l.sec, l.nsec);
}

inline bool operator!=(const boost::xtime& l, const timeval& r)
{
	return
		!yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare_eq(
			r.tv_sec, r.tv_usec, l.sec, l.nsec);
}

inline bool operator<(const boost::xtime& l, const timeval& r)
{
	return
		yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare(
			r.tv_sec, r.tv_usec, l.sec, l.nsec) > 0;
}

inline bool operator<=(const boost::xtime& l, const timeval& r)
{
	return
		yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare(
			r.tv_sec, r.tv_usec, l.sec, l.nsec) >= 0;
}

inline bool operator>(const boost::xtime& l, const timeval& r)
{
	return
		yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare(
			r.tv_sec, r.tv_usec, l.sec, l.nsec) < 0;
}

inline bool operator>=(const boost::xtime& l, const timeval& r)
{
	return
		yggr::nsql_database_system::bson_date_time_native_ex::timeval_xtime_compare(
			r.tv_sec, r.tv_usec, l.sec, l.nsec) <= 0;
}

// timeval <=> bson_value
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const timeval& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_timeval(
			boost::addressof(r), boost::addressof(l));
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const timeval& l, const BsonValue& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_timeval(
			boost::addressof(r), boost::addressof(l));
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const timeval& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_timeval(
			boost::addressof(r), boost::addressof(l)) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const timeval& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_timeval(
			boost::addressof(r), boost::addressof(l)) >= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const timeval& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_timeval(
			boost::addressof(r), boost::addressof(l)) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const timeval& l, const BsonValue& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_timeval(
			boost::addressof(r), boost::addressof(l)) <= 0;
}

// bson_value <=> timeval
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const timeval& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_timeval(
			boost::addressof(l), boost::addressof(r));
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const timeval& r)
{
	return
		!yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_eq_timeval(
			boost::addressof(l), boost::addressof(r));
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<(const BsonValue& l, const timeval& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_timeval(
			boost::addressof(l), boost::addressof(r)) < 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator<=(const BsonValue& l, const timeval& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_timeval(
			boost::addressof(l), boost::addressof(r)) <= 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>(const BsonValue& l, const timeval& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_timeval(
			boost::addressof(l), boost::addressof(r)) > 0;
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator>=(const BsonValue& l, const timeval& r)
{
	return
		yggr::nsql_database_system::bson_value_native_ex::s_value_cmp_timeval(
			boost::addressof(l), boost::addressof(r)) >= 0;
}

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<>
struct bson_operator_outter<timeval>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const timeval& val,
					bson_json_mode_def::mode_type mode) const
	{
		u64 date_val = bson_date_time_native_ex::timeval_to_msec_since_epoch(val.tv_sec, val.tv_usec);
		char buf[time::time_config::default_string_buffer_length] = {0};

		switch(mode)
		{
		case bson_json_mode_def::E_BSON_JSON_MODE_RELAXED:
			if(!(date_val < 0))
			{
				os << "{ \"$date\" : \""
					<< bson_date_time_native_ex::s_bson_date_format_iso8601(
						date_val, buf, time::time_config::default_string_buffer_length)
					<< "\" }";
				break;
			}
		case bson_json_mode_def::E_BSON_JSON_MODE_CANONICAL:
			os << "{ \"$date\" : { \"$numberLong\" : \"" << date_val << "\" } }";
			break;
		default:
			os << "{ \"$date\" : " << date_val << " }";
		}

		return os;
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

// <<
template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const timeval& val)
{
	typedef yggr::nsql_database_system::detail::bson_operator_outter<timeval> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(timeval& l, timeval& r);

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, void>::type
	swap(timeval& l, BsonTimeval& r)
{
	yggr::swap(l.tv_sec, r.tv_sec);
	yggr::swap(l.tv_usec, r.tv_usec);
}

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename BsonTimeval> inline
typename boost::enable_if<boost::is_base_of<timeval, BsonTimeval>, void>::type
	swap(timeval& l, BOOST_RV_REF(BsonTimeval) r)
{
    BsonTimeval& right_ref = r;
	yggr::swap(l.tv_sec, right_ref.tv_sec);
	yggr::swap(l.tv_usec, right_ref.tv_usec);
}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_TIMEVAL_NATIVE_EX_HPP__
