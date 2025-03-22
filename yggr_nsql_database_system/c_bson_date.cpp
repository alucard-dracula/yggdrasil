// c_bson_date.cpp

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

#include <yggr/nsql_database_system/c_bson_date.hpp>
#include <yggr/nsql_database_system/c_bson_time.hpp>
#include <yggr/nsql_database_system/c_bson_timeval.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <yggr/utility/swap.hpp>

#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_bson_date::c_bson_date(void)
	: _date(0)
{
	boost::xtime xtm = {0};

#if BOOST_VERSION < 105300
	boost::xtime_get(&xtm, boost::TIME_UTC); 
#else
	boost::xtime_get(&xtm, boost::TIME_UTC_); 
#endif // BOOST_VERSION

	_date = static_cast<base_type>(xtm.sec) * this_type::E_S_AND_MS_RATIO 
				+ (static_cast<base_type>(xtm.nsec) + this_type::E_MS_AND_NS_HALF_RATIO) / this_type::E_MS_AND_NS_RATIO;
}

c_bson_date::c_bson_date(const bson_value_t& val)
	: _date(0)
{
	this_type::pro_assign_bson_value_t(val);
}

c_bson_date::c_bson_date(base_type right)
	: _date(right)
{
}

c_bson_date::c_bson_date(const c_bson_time& right)
	: _date(right.time() * this_type::E_S_AND_MS_RATIO)
{
}

c_bson_date::c_bson_date(const timeval& right)
	: _date(static_cast<base_type>(right.tv_sec) * this_type::E_S_AND_MS_RATIO 
			+ static_cast<base_type>(right.tv_usec) / this_type::E_MS_AND_MCS_RATIO)
{
}

c_bson_date::c_bson_date(const c_bson_timeval& right)
	: _date(static_cast<base_type>(right.var_tv_sec()) * this_type::E_S_AND_MS_RATIO 
			+ static_cast<base_type>(right.var_tv_usec()) / this_type::E_MS_AND_MCS_RATIO)
{
}

c_bson_date::c_bson_date(const bridge_time_type& right)
	: _date(static_cast<base_type>(right.sec) * this_type::E_S_AND_MS_RATIO 
			+ static_cast<base_type>(right.nsec) / this_type::E_MS_AND_NS_RATIO)
{
}

c_bson_date::c_bson_date(const this_type& right)
	: _date(right._date)
{
}

c_bson_date::~c_bson_date(void)
{
}

c_bson_date::this_type& c_bson_date::operator=(base_type right)
{
	_date = right;
	return *this;
}

c_bson_date::this_type& c_bson_date::operator=(const c_bson_time& right)
{
	_date = right.time() * this_type::E_S_AND_MS_RATIO;
	return *this;
}

c_bson_date::this_type& c_bson_date::operator=(const bridge_time_type& right)
{
	_date = static_cast<base_type>(right.sec) * this_type::E_S_AND_MS_RATIO 
				+ static_cast<base_type>(right.nsec) / this_type::E_MS_AND_NS_RATIO;
	return *this;
}

c_bson_date::this_type& c_bson_date::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_date = right._date;
	return *this;
}

void c_bson_date::swap(base_type& right)
{
	yggr::swap(_date, right);
}

void c_bson_date::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}
	yggr::swap(_date, right._date);
}

// compare_eq
bool c_bson_date::compare_eq(const c_bson_time& val) const
{
	return 
		bson_date_time_native_ex::date_time_compare_eq(
			_date, val.time());
}

// compare
s32 c_bson_date::compare(const c_bson_time& val) const
{
	return 
		bson_date_time_native_ex::date_time_compare(
			_date, val.time());
}

void c_bson_date::pro_assign_bson_value_t(const bson_value_t& val)
{
	bool bsuccess = bson_value_native_ex::s_value_to_date(_date, &val);
	assert(bsuccess);
}

std::tm c_bson_date::to_tm(void) const
{
	boost::system_time sys_time = this_type::bridge_time();
	return boost::posix_time::to_tm(sys_time);
}

std::tm c_bson_date::to_local_tm(void) const
{
	typedef boost::date_time::c_local_adjustor<boost::system_time> local_adj;

	boost::system_time sys_time = this_type::bridge_time();
	boost::system_time local_sys_time = local_adj::utc_to_local(sys_time);

	return boost::posix_time::to_tm(local_sys_time);
}

// c_bson_date <=> c_bson_time
bool operator==(const c_bson_date& l, const c_bson_time& r)
{
	return l.compare_eq(r);
}

bool operator!=(const c_bson_date& l, const c_bson_time& r)
{
	return !l.compare_eq(r);
}

bool operator<(const c_bson_date& l, const c_bson_time& r)
{
	return l.compare(r) < 0;
}

bool operator<=(const c_bson_date& l, const c_bson_time& r)
{
	return l.compare(r) <= 0;
}

bool operator>(const c_bson_date& l, const c_bson_time& r)
{
	return l.compare(r) > 0;
}

bool operator>=(const c_bson_date& l, const c_bson_time& r)
{
	return l.compare(r) >= 0;
}

} // namespace nsql_database_system
} // namespace yggr



