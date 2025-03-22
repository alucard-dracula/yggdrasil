// c_bson_symbol.cpp

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

#include <yggr/nsql_database_system/c_bson_symbol.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>

#include <algorithm>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


namespace yggr
{
namespace nsql_database_system
{

c_bson_symbol::c_bson_symbol(void)
{
}

c_bson_symbol::c_bson_symbol(const bson_value_t& val)
{
	this_type::pro_assign_bson_value_t(val);
}

c_bson_symbol::c_bson_symbol(const char* symbol_str)
	: base_type(symbol_str)
{
}

#ifndef YGGR_NO_CWCHAR

c_bson_symbol::c_bson_symbol(const wchar_t* symbol_str)
	: base_type(symbol_str)
{
}

#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T

c_bson_symbol::c_bson_symbol(const c8* symbol_str)
	: base_type(symbol_str)
{
}

#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T

c_bson_symbol::c_bson_symbol(const c16* symbol_str)
	: base_type(symbol_str)
{
}

#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T

c_bson_symbol::c_bson_symbol(const c32* symbol_str)
	: base_type(symbol_str)
{
}

#endif // YGGR_NO_CHAR32_T

c_bson_symbol::c_bson_symbol(const this_type& right)
	: base_type(right)
{
}

c_bson_symbol::~c_bson_symbol(void)
{
}

c_bson_symbol::this_type& c_bson_symbol::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	base_type::operator=(right);
	return *this;
}

void c_bson_symbol::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	base_type::swap(right);
}

void c_bson_symbol::pro_assign_bson_value_t(const bson_value_t& val)
{
	bool bsuccess = bson_value_native_ex::s_value_to_symbol(base_type::_str, &val);
	assert(bsuccess);
}

} // namespace nsql_database_system
} // namespace yggr


