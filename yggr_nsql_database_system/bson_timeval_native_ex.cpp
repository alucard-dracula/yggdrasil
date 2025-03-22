// bson_timeval_native_ex.cpp

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

#include <yggr/nsql_database_system/bson_timeval_native_ex.hpp>
#include <yggr/nsql_database_system/c_bson_date.hpp>
#include <yggr/nsql_database_system/c_bson_time.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

// timeval <=> c_bson_date
bool operator==(const timeval& l, const yggr::nsql_database_system::c_bson_date& r)
{
	return r.compare_eq(l);
}

bool operator!=(const timeval& l, const yggr::nsql_database_system::c_bson_date& r)
{
	return !r.compare_eq(l);
}

bool operator<(const timeval& l, const yggr::nsql_database_system::c_bson_date& r)
{
	return r.compare(l) > 0;
}

bool operator<=(const timeval& l, const yggr::nsql_database_system::c_bson_date& r)
{
	return r.compare(l) >= 0;
}

bool operator>(const timeval& l, const yggr::nsql_database_system::c_bson_date& r)
{
	return r.compare(l) < 0;
}

bool operator>=(const timeval& l, const yggr::nsql_database_system::c_bson_date& r)
{
	return r.compare(l) <= 0;
}

// timeval <=> c_bson_time
bool operator==(const timeval& l, const yggr::nsql_database_system::c_bson_time& r)
{
	return r.compare_eq(l);
}

bool operator!=(const timeval& l, const yggr::nsql_database_system::c_bson_time& r)
{
	return !r.compare_eq(l);
}

bool operator<(const timeval& l, const yggr::nsql_database_system::c_bson_time& r)
{
	return r.compare(l) > 0;
}

bool operator<=(const timeval& l, const yggr::nsql_database_system::c_bson_time& r)
{
	return r.compare(l) >= 0;
}

bool operator>(const timeval& l, const yggr::nsql_database_system::c_bson_time& r)
{
	return r.compare(l) < 0;
}

bool operator>=(const timeval& l, const yggr::nsql_database_system::c_bson_time& r)
{
	return r.compare(l) <= 0;
}


namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(timeval& l, timeval& r)
{
	yggr::swap(l.tv_sec, r.tv_sec);
	yggr::swap(l.tv_usec, r.tv_usec);
}

} // namespace swap_support
} // namespace nsql_database_system
} // namespace yggr
