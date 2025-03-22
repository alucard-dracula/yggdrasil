// c_bson_timeval.cpp

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

#include <yggr/nsql_database_system/c_bson_timeval.hpp>
#include <yggr/nsql_database_system/c_bson_date.hpp>
#include <yggr/nsql_database_system/c_bson_time.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <yggr/utility/swap.hpp>

#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

#include <ctime>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_bson_timeval::c_bson_timeval(void)
{
	boost::xtime xtm = {0};

#if BOOST_VERSION < 105300
	boost::xtime_get(&xtm, boost::TIME_UTC); 
#else
	boost::xtime_get(&xtm, boost::TIME_UTC_); 
#endif // BOOST_VERSION

	base_type::tv_sec = static_cast<base_sec_val_type>(xtm.sec);
	this_type::set_var_tv_usec_of_ns(xtm.nsec);
}

c_bson_timeval::c_bson_timeval(const bson_value_t& val)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
	this_type::pro_assign_bson_value_t(val);
}

c_bson_timeval::c_bson_timeval(base_sec_val_type sec, base_usec_val_type usec)
{
	base_type::tv_sec = sec;
	base_type::tv_usec = usec;
}

c_bson_timeval::c_bson_timeval(const c_bson_date& right)
{
	c_bson_date::base_type date = right.date();
	base_type::tv_sec = static_cast<base_sec_val_type>(date / this_type::E_S_AND_MS_RATIO);

	u64 tmp = date % this_type::E_S_AND_MS_RATIO;
	this_type::set_var_tv_usec_of_ms(tmp);
}

c_bson_timeval::c_bson_timeval(const c_bson_time& right)
{
	c_bson_time::base_type time = right.time();
	base_type::tv_sec = static_cast<base_sec_val_type>(time);
	base_type::tv_usec = 0;
}

c_bson_timeval::c_bson_timeval(const bridge_time_type& right)
{
	base_type::tv_sec = static_cast<base_sec_val_type>(right.sec);
	this_type::set_var_tv_usec_of_ns(right.nsec);
}

c_bson_timeval::c_bson_timeval(const base_type& right)
{
	base_type::tv_sec = right.tv_sec;
	base_type::tv_usec = right.tv_usec;
}

c_bson_timeval::c_bson_timeval(const this_type& right)
{
	base_type::tv_sec = right.tv_sec;
	base_type::tv_usec = right.tv_usec;
}

c_bson_timeval::~c_bson_timeval(void)
{
}

c_bson_timeval::this_type& c_bson_timeval::operator=(const c_bson_date& right)
{
	c_bson_date::base_type date = right.date();
	base_type::tv_sec = static_cast<base_sec_val_type>(date / this_type::E_S_AND_MS_RATIO);
	this_type::set_var_tv_usec_of_ms(date % this_type::E_S_AND_MS_RATIO);
	return *this;
}

c_bson_timeval::this_type& c_bson_timeval::operator=(const c_bson_time& right)
{
	c_bson_time::base_type time = right.time();
	base_type::tv_sec = static_cast<base_sec_val_type>(time);
	base_type::tv_usec = 0;
	return *this;
}

c_bson_timeval::this_type& c_bson_timeval::operator=(const bridge_time_type& right)
{
	base_type::tv_sec = static_cast<base_sec_val_type>(right.sec);
	this_type::set_var_tv_usec_of_ns(right.nsec);
	return *this;
}

c_bson_timeval::this_type& c_bson_timeval::operator=(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	base_type::tv_sec = right.tv_sec;
	base_type::tv_usec = right.tv_usec;
	return *this;
}

void c_bson_timeval::swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	yggr::swap(base_type::tv_sec, right.tv_sec);
	yggr::swap(base_type::tv_usec, right.tv_usec);
}

void c_bson_timeval::pro_assign_bson_value_t(const bson_value_t& val)
{
	base_type& base = *this;
	bool bsuccess = 
		bson_value_native_ex::s_value_to_timeval(
			boost::addressof(base), boost::addressof(val));
	assert(bsuccess);
}

std::tm c_bson_timeval::to_tm(void) const
{
	boost::system_time sys_time = this_type::bridge_time();
	return boost::posix_time::to_tm(sys_time);
}

std::tm c_bson_timeval::to_local_tm(void) const
{
	typedef boost::date_time::c_local_adjustor<boost::system_time> local_adj;

	boost::system_time sys_time = this_type::bridge_time();
	boost::system_time local_sys_time = local_adj::utc_to_local(sys_time);

	return boost::posix_time::to_tm(local_sys_time);
}

// compare_eq
bool c_bson_timeval::compare_eq(const c_bson_date& val) const
{
	return 
		bson_date_time_native_ex::timeval_date_compare_eq(
			base_type::tv_sec, base_type::tv_usec, val.date());
}

bool c_bson_timeval::compare_eq(const c_bson_time& val) const
{
	return 
		bson_date_time_native_ex::timeval_time_compare_eq(
			base_type::tv_sec, base_type::tv_usec, val.time());
}

// compare
s32 c_bson_timeval::compare(const c_bson_date& val) const
{
	return 
		bson_date_time_native_ex::timeval_date_compare(
			base_type::tv_sec, base_type::tv_usec, val.date());
}

s32 c_bson_timeval::compare(const c_bson_time& val) const
{
	return 
		bson_date_time_native_ex::timeval_time_compare(
			base_type::tv_sec, base_type::tv_usec, val.time());
}

// c_bson_timeval <=> c_bson_timeval
bool operator==(const c_bson_timeval& l, const c_bson_timeval& r)
{
	return l.compare_eq(r);
}

bool operator!=(const c_bson_timeval& l, const c_bson_timeval& r)
{
	return !l.compare_eq(r);
}

bool operator<(const c_bson_timeval& l, const c_bson_timeval& r)
{
	return l.compare(r) < 0;
}

bool operator<=(const c_bson_timeval& l, const c_bson_timeval& r)
{
	return l.compare(r) <= 0;
}

bool operator>(const c_bson_timeval& l, const c_bson_timeval& r)
{
	return l.compare(r) > 0;
}

bool operator>=(const c_bson_timeval& l, const c_bson_timeval& r)
{
	return l.compare(r) >= 0;
}

// c_bson_timeval <=> c_bson_date
bool operator==(const c_bson_timeval& l, const c_bson_date& r)
{
	return l.compare_eq(r);
}

bool operator!=(const c_bson_timeval& l, const c_bson_date& r)
{
	return !l.compare_eq(r);
}

bool operator<(const c_bson_timeval& l, const c_bson_date& r)
{
	return l.compare(r) < 0;
}

bool operator<=(const c_bson_timeval& l, const c_bson_date& r)
{
	return l.compare(r) <= 0;
}

bool operator>(const c_bson_timeval& l, const c_bson_date& r)
{
	return l.compare(r) > 0;
}

bool operator>=(const c_bson_timeval& l, const c_bson_date& r)
{
	return l.compare(r) >= 0;
}

// c_bson_timeval <=> c_bson_time
bool operator==(const c_bson_timeval& l, const c_bson_time& r)
{
	return l.compare_eq(r);
}

bool operator!=(const c_bson_timeval& l, const c_bson_time& r)
{
	return !l.compare_eq(r);
}

bool operator<(const c_bson_timeval& l, const c_bson_time& r)
{
	return l.compare(r) < 0;
}

bool operator<=(const c_bson_timeval& l, const c_bson_time& r)
{
	return l.compare(r) <= 0;
}

bool operator>(const c_bson_timeval& l, const c_bson_time& r)
{
	return l.compare(r) > 0;
}

bool operator>=(const c_bson_timeval& l, const c_bson_time& r)
{
	return l.compare(r) >= 0;
}

} // namespace nsql_database_system
} // namespace yggr
