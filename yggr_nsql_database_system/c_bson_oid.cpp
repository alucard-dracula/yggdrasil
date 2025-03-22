// c_bson_oid.cpp

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

#include <yggr/nsql_database_system/c_bson_oid.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp> // in cpp20 befor c_bson_oid.hpp, because bson_oid_native_ex.hpp recursion

#include <algorithm>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*explicit*/
c_bson_oid::c_bson_oid(bson_context_t* pcontext)
{
	base_type& base = *this;
	bson_oid_init(boost::addressof(base), pcontext);
}

/*explicit*/ 
c_bson_oid::c_bson_oid(const char* str_oid, std::size_t n)
{
	this_type::assign(str_oid, n);
}

c_bson_oid::c_bson_oid(const bson_value_t& val)
{
	base_type& base = *this;
	memset(boost::addressof(base), 0, sizeof(base_type));

	this_type::pro_assign_bson_value_t(val);
}

/*explicit*/ 
c_bson_oid::c_bson_oid(const base_type& right)
{
	this_type::assign(right);
}
	
c_bson_oid::c_bson_oid(const this_type& right)
{
	this_type::assign(right);
}

c_bson_oid::~c_bson_oid(void)
{
}

c_bson_oid::this_type& c_bson_oid::operator=(const this_type& right)
{
	this_type::assign(right);
	return *this;
}

void c_bson_oid::pro_swap(base_type& right)
{
	bson_oid_native_ex::s_bson_oid_swap(this, boost::addressof(right));
}

void c_bson_oid::swap(this_type& right)
{
	this_type::pro_swap(static_cast<base_type&>(right));
}

void c_bson_oid::pro_assign_bson_value_t(const bson_value_t& val)
{
	base_type& base = *this;
	bool bsuccess = 
		bson_value_native_ex::s_value_to_oid(
			boost::addressof(base), boost::addressof(val));
	assert(bsuccess);
}

void c_bson_oid::assign(const char* str_oid, std::size_t len)
{
	base_type& base = *this;
	if(!str_oid)
	{
		bson_oid_init(boost::addressof(base), 0);
		return;
	}

	if(len == this_type::E_bson_oid_string_size)
	{
		bson_oid_init_from_string(boost::addressof(base), str_oid);
	}
	else if(len == this_type::E_bson_oid_byte_size)
	{
		bson_oid_init_from_data(boost::addressof(base), reinterpret_cast<const uint8_t*>(str_oid));
	}
	else
	{
		assert(false);
		bson_oid_init(boost::addressof(base), 0);
	}
}

void c_bson_oid::assign(const char* str_oid)
{
	size_type len = str_oid? charset::string_byte_size(str_oid) : 0;
	this_type::assign(str_oid, len);
}



} // namespace nsql_database_system
} // namespace yggr
