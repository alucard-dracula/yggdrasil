// c_bson_decimal128.cpp

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

#include <yggr/nsql_database_system/c_bson_decimal128.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

// construct
c_bson_decimal128::c_bson_decimal128(void)
{
	base_type::low = base_type::high = 0;
}

c_bson_decimal128::c_bson_decimal128(const char* str_num)
{
	base_type::low = base_type::high = 0;
	this_type::pro_init_from_string(str_num);
}

#ifndef YGGR_NO_CWCHAR

c_bson_decimal128::c_bson_decimal128(const wchar_t* str_num)
{
	base_type::low = base_type::high = 0;
	this_type::pro_init_from_string(str_num);
}

#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T

c_bson_decimal128::c_bson_decimal128(const c8* str_num)
{
	base_type::low = base_type::high = 0;
	this_type::pro_init_from_string(str_num);
}

#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T

c_bson_decimal128::c_bson_decimal128(const c16* str_num)
{
	base_type::low = base_type::high = 0;
	this_type::pro_init_from_string(str_num);
}

#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T

c_bson_decimal128::c_bson_decimal128(const c32* str_num)
{
	base_type::low = base_type::high = 0;
	this_type::pro_init_from_string(str_num);
}

#endif // YGGR_NO_CHAR32_T

c_bson_decimal128::c_bson_decimal128(u64 nlow)
{
	base_type::low = nlow;
	base_type::high = 0;
}

c_bson_decimal128::c_bson_decimal128(u64 nhigh, u64 nlow)
{
	base_type::low = nlow;
	base_type::high = nhigh;
}

c_bson_decimal128::c_bson_decimal128(const bson_value_t& val)
{
	this_type::pro_assign_bson_value_t(val);
}

c_bson_decimal128::c_bson_decimal128(const org_type& right)
{
	base_type::low = right.low;
	base_type::high = right.high;
}

c_bson_decimal128::c_bson_decimal128(const this_type& right)
{
	base_type::low = right.low;
	base_type::high = right.high;
}

// destruct
c_bson_decimal128::~c_bson_decimal128(void)
{
}

//public:
// operator=
c_bson_decimal128::this_type& c_bson_decimal128::operator=(u64 nlow)
{
	base_type::high = 0;
	base_type::low = nlow;
	return *this;
}

c_bson_decimal128::this_type& c_bson_decimal128::pro_operator_set(const base_type& right)
{
	if(static_cast<base_type*>(this) == boost::addressof(right))
	{
		return *this;
	}

	base_type::low = right.low;
	base_type::high = right.high;

	return *this;
}

c_bson_decimal128::this_type& c_bson_decimal128::operator=(const this_type& right)
{
	return this_type::pro_operator_set(static_cast<const base_type&>(right));
}

void c_bson_decimal128::pro_swap(base_type& right)
{
	if(static_cast<base_type*>(this) == boost::addressof(right))
	{
		return;
	}

	yggr::swap(base_type::low, right.low);
	yggr::swap(base_type::high, right.high);
}

void c_bson_decimal128::swap(this_type& right)
{
	this_type::pro_swap(static_cast<base_type&>(right));
}

void c_bson_decimal128::pro_assign_bson_value_t(const bson_value_t& val)
{
	bool bsuccess = 
		bson_value_native_ex::s_value_to_decimal128(
			static_cast<base_type*>(this),
			boost::addressof(val));
	assert(bsuccess);
}

c_bson_decimal128::inner_string_type 
	c_bson_decimal128::str(const string& ret_charset_name) const
{
	assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

	char out_str[bson_decimal128_native_ex::E_BSON_DECIMAL128_STRING] = {0};

	return 
		bson_decimal128_native_ex::s_bson_decimal128_to_string(this, out_str)?
			inner_string_type(out_str, YGGR_STR_UTF8_STRING_CHARSET_NAME())
			: inner_string_type();
}

} // namespace nsql_database_system
} // namesapce yggr
