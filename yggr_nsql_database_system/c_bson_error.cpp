//c_bosn_err.cpp

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

#include <yggr/nsql_database_system/c_bson_error.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_bson_error::c_bson_error(void)
{
	base_type& base = *this;
	bson_error_native_ex::s_bson_error_init(&base);
}

c_bson_error::c_bson_error(u32 domain, u32 code, const char* format, ...)
{
	base_type& base = *this;

	va_list args;
	va_start(args, format);
	bson_error_native_ex::s_bson_error_init_va_list(&base, domain, code, format, args);
    va_end(args);
}

c_bson_error::c_bson_error(const base_type& right)
{
	base_type& base = *this;
	bson_error_native_ex::s_bson_error_copy(&base, &right);
}

c_bson_error::c_bson_error(const this_type& right)
{
	base_type& l = *this;
	const base_type& r = right;
	bson_error_native_ex::s_bson_error_copy(&l, &r);
}

c_bson_error::~c_bson_error(void)
{
}

c_bson_error::this_type& c_bson_error::pro_operator_set(const base_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	base_type& base = *this;
	bson_error_native_ex::s_bson_error_copy(&base, &right);
	return *this;
}

c_bson_error::this_type& c_bson_error::operator=(const this_type& right)
{
	return this_type::pro_operator_set(static_cast<const base_type&>(right));
}

void c_bson_error::pro_swap(base_type& right)
{
	if(this == &right)
	{
		return;
	}

	base_type& base = *this;
	bson_error_native_ex::s_bson_error_swap(&base, &right);
}

void c_bson_error::swap(this_type& right)
{
	this_type::pro_swap(static_cast<base_type&>(right));
}

} // namespace nsql_database_system
} // namespace yggr

