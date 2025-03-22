// c_bson_value.cpp

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

#include <yggr/nsql_database_system/c_bson_value.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_bson_value::c_bson_value(void)
{
	this_type::pro_init();
}

c_bson_value::c_bson_value(s32 val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(int32), val);
}

c_bson_value::c_bson_value(s64 val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(int64), val);
}

c_bson_value::c_bson_value(bool val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(bool), val);
}

c_bson_value::c_bson_value(f32 val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(double), static_cast<f64>(val));
}

c_bson_value::c_bson_value(f64 val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(double), val);
}

c_bson_value::c_bson_value(const char* val)
{
	this_type::pro_init();this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), val);
}

#ifndef YGGR_NO_CWCHAR

c_bson_value::c_bson_value(const wchar_t* val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), val);
}

#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T

c_bson_value::c_bson_value(const c8* val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), val);
}

#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T

c_bson_value::c_bson_value(const c16* val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), val);
}

#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T

c_bson_value::c_bson_value(const c32* val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(utf8), val);
}

#endif // YGGR_NO_CHAR32_T

c_bson_value::c_bson_value(const c_bson_null&)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(null));
}

c_bson_value::c_bson_value(const c_bson_undefined&)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(undefined));
}

c_bson_value::c_bson_value(const c_bson_maxkey&)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(maxkey));
}

c_bson_value::c_bson_value(const c_bson_minkey&)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(minkey));
}

c_bson_value::c_bson_value(const c_bson_date& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(date_time), val);
}

c_bson_value::c_bson_value(const c_bson_time& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(time), val);
}

c_bson_value::c_bson_value(const timeval& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(timeval), val);
}

c_bson_value::c_bson_value(const c_bson_timeval& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(timeval), val);
}

c_bson_value::c_bson_value(const bson_oid_t& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(oid), val);
}

c_bson_value::c_bson_value(const c_bson_oid& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(oid), val);
}

c_bson_value::c_bson_value(const bson_decimal128_t& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), val);
}

c_bson_value::c_bson_value(const c_bson_decimal128& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(decimal128), val);
}

c_bson_value::c_bson_value(const c_bson_regex& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(regex), val);
}

c_bson_value::c_bson_value(const c_bson_code& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(code), val);
}

c_bson_value::c_bson_value(const c_bson_symbol& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(symbol), val);
}

c_bson_value::c_bson_value(const bson_timestamp_t& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), val);
}

c_bson_value::c_bson_value(const c_bson_timestamp& val)
{
	this_type::pro_init();
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(timestamp), val);
}

c_bson_value::c_bson_value(const bson_t& val)
{
	this_type::pro_init();
	this_type::assign(val);
}

c_bson_value::c_bson_value(const base_type& right)
{
	this_type::pro_init();
	// construct not alloc inner heap memory, so can using bson_value_copy
	//bson_value_copy(&right, this);; // linux failed BSON_ASSERT(false)
	bson_value_native_ex::s_bson_value_copy(static_cast<org_type*>(this), &right);
}

c_bson_value::c_bson_value(const this_type& right)
{
	this_type::pro_init();
	// construct not alloc inner heap memory, so can using bson_value_copy
	//bson_value_copy(&right, this); // linux failed BSON_ASSERT(false)
	bson_value_native_ex::s_bson_value_copy(static_cast<org_type*>(this), static_cast<const org_type*>(&right));
}

c_bson_value::~c_bson_value(void)
{
	bson_value_native_ex::s_bson_value_clear(this);
	//bson_value_destroy(this);
}

c_bson_value::this_type& c_bson_value::operator=(s32 val)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(int32), val);
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(s64 val)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(int64), val);
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(bool val)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(bool), val);
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(f32 val)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(double), static_cast<f64>(val));
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(f64 val)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(double), val);
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(const c_bson_null&)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(null));
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(const c_bson_undefined&)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(undefined));
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(const c_bson_maxkey&)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(maxkey));
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(const c_bson_minkey&)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(minkey));
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(const c_bson_date& val)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(date_time), val);
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(const c_bson_time& val)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(time), val);
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(const c_bson_regex& val)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(regex), val);
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(const c_bson_code& val)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(code), val);
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(const c_bson_symbol& val)
{
	this_type::assign(YGGR_BSON_VALUE_TAG_OBJECT(symbol), val);
	return *this;
}

c_bson_value::this_type& c_bson_value::operator=(const this_type& right)
{
	return this_type::pro_operator_set(static_cast<const base_type&>(right));
}

void c_bson_value::swap(this_type& right)
{
	this_type::pro_swap(static_cast<base_type&>(right));
}

c_bson_value::this_type& c_bson_value::pro_operator_set(const base_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	bson_value_native_ex::s_bson_value_copy(this, boost::addressof(right));
	return *this;
}

void c_bson_value::pro_swap(base_type& right)
{
	bson_value_native_ex::s_bson_value_swap(this, boost::addressof(right));
}

void c_bson_value::pro_init(void)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
}

} // namespace nsql_database_system
} // namespace yggr
