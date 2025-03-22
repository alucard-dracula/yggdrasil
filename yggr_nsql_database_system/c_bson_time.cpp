// c_bson_time.cpp

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

#include <yggr/nsql_database_system/c_bson_time.hpp>
#include <yggr/nsql_database_system/c_bson_date.hpp>
#include <yggr/nsql_database_system/c_bson_timeval.hpp>
#include <yggr/nsql_database_system/bson_date_time_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <yggr/time/time_basic_foo.hpp>
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

c_bson_time::c_bson_time(void)
	: _time(0)
{
	boost::xtime xtm = {0};

#if BOOST_VERSION < 105300
	boost::xtime_get(&xtm, boost::TIME_UTC); 
#else
	boost::xtime_get(&xtm, boost::TIME_UTC_); 
#endif // BOOST_VERSION

	_time = xtm.sec;
}

c_bson_time::c_bson_time(const bson_value_t& val)
	: _time(0)
{
	this_type::pro_assign_bson_value_t(val);
}

c_bson_time::c_bson_time(base_type right)
	: _time(right)
{
}

c_bson_time::c_bson_time(const c_bson_date& right)
	: _time(right.date() / this_type::E_S_AND_MS_RATIO)
{
}

c_bson_time::c_bson_time(const timeval& right)
	: _time(static_cast<base_type>(right.tv_sec))
{
}

c_bson_time::c_bson_time(const c_bson_timeval& right)
	: _time(static_cast<base_type>(right.var_tv_sec()))
{
}

c_bson_time::c_bson_time(const bridge_time_type& right)
	: _time(static_cast<base_type>(right.sec))
{
}

c_bson_time::c_bson_time(const this_type& right)
	:_time(right._time)
{
}

c_bson_time::~c_bson_time(void)
{
}

c_bson_time::this_type& c_bson_time::operator=(base_type right)
{
	_time = right;
	return *this;
}

c_bson_time::this_type& c_bson_time::operator=(const c_bson_date& right)
{
	_time = right.date() / this_type::E_S_AND_MS_RATIO;
	return *this;
}

c_bson_time::this_type& c_bson_time::operator=(const bridge_time_type& right)
{
	_time = static_cast<base_type>(right.sec);
	return *this;
}

c_bson_time::this_type& c_bson_time::operator=(const this_type& right)
{
	if(this == &right) 
	{
		return *this;
	}
	_time = right._time;
	return *this;
}

void c_bson_time::swap(base_type& right)
{
	yggr::swap(_time, right);
}

void c_bson_time::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}
	yggr::swap(_time, right._time);
}

// compare_eq
bool c_bson_time::compare_eq(const c_bson_date& val) const
{
	return 
		bson_date_time_native_ex::time_date_compare_eq(
			_time, val.date());
}

// compare
s32 c_bson_time::compare(const c_bson_date& val) const
{
	return 
		bson_date_time_native_ex::time_date_compare(
			_time, val.date());
}

void c_bson_time::pro_assign_bson_value_t(const bson_value_t& val)
{
	bool bsuccess = bson_value_native_ex::s_value_to_time(_time, &val);
	assert(bsuccess);
}

std::tm c_bson_time::to_tm(void) const
{
	static const std::tm empty_tm = {0};
	std::tm tmp_tm = {0};
	std::time_t time = _time;
	std::tm* ptm = 0;
	return (ptm = time::utc_time_to_utc_tm(time, tmp_tm))? *ptm : empty_tm;
}

std::tm c_bson_time::to_local_tm(void) const
{
	static const std::tm empty_tm = {0};
	std::tm tmp_tm = {0};
	std::time_t time = _time;
	std::tm* ptm = 0;
	return (ptm = time::utc_time_to_local_tm(time, tmp_tm))? *ptm : empty_tm;
}

// c_bson_time <=> c_bson_date
bool operator==(const c_bson_time& l, const c_bson_date& r)
{
	return l.compare_eq(r);
}

bool operator!=(const c_bson_time& l, const c_bson_date& r)
{
	return !l.compare_eq(r);
}

bool operator<(const c_bson_time& l, const c_bson_date& r)
{
	return l.compare(r) < 0;
}

bool operator<=(const c_bson_time& l, const c_bson_date& r)
{
	return l.compare(r) <= 0;
}

bool operator>(const c_bson_time& l, const c_bson_date& r)
{
	return l.compare(r) > 0;
}

bool operator>=(const c_bson_time& l, const c_bson_date& r)
{
	return l.compare(r) >= 0;
}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(c_bson_time::base_type& l, c_bson_time& r) 
{ 
	r.swap(l); 
} 
	
void swap(c_bson_time& l, c_bson_time::base_type& r) 
{ 
	l.swap(r); 
} 
	
void swap(c_bson_time& l, c_bson_time& r) 
{ 
	l.swap(r);
}

} // namespace swap_support
} // namespace nsql_database_system
} // namespace yggr
