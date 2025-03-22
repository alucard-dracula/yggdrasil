// c_bson_timestamp.cpp

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

#include <yggr/nsql_database_system/c_bson_timestamp.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <yggr/utility/swap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_bson_timestamp::c_bson_timestamp(void)
{
	base_type::tms = 0;
}

c_bson_timestamp::c_bson_timestamp(u32 time, u32 inc)
{
	base_type::t = time;
	base_type::i = inc;
}

c_bson_timestamp::c_bson_timestamp(const bson_value_t& val)
{
	base_type::tms = 0;
	this_type::pro_assign_bson_value_t(val);
}

c_bson_timestamp::c_bson_timestamp(const base_type& right)
{
	base_type::tms = right.tms;
}

c_bson_timestamp::c_bson_timestamp(const this_type& right)
{
	base_type::tms = right.tms;
}

c_bson_timestamp::~c_bson_timestamp(void)
{
}

c_bson_timestamp::this_type& c_bson_timestamp::operator=(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	base_type::tms = right.tms;
	return *this;
}

void c_bson_timestamp::pro_assign_bson_value_t(const bson_value_t& val)
{
	bool bsuccess = bson_value_native_ex::s_value_to_timestamp(base_type::t, base_type::i, &val);
	assert(bsuccess);
}

} // namespace nsql_database_system
} // namespace yggr

