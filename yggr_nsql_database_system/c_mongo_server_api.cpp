//c_mongo_server_api.cpp

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

#include <yggr/nsql_database_system/c_mongo_server_api.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//public:
c_mongo_server_api::c_mongo_server_api(void)
{
	this_type::prv_init();
	base_type::version = MONGOC_SERVER_API_V1;
}

c_mongo_server_api::c_mongo_server_api(version_type ver, bool bstrict, bool berrors)
{
	this_type::prv_init();
	base_type::version = static_cast<mongoc_server_api_version_t>(ver);
	mongoc_optional_set_value(boost::addressof(base_type::strict), bstrict);
	mongoc_optional_set_value(boost::addressof(base_type::deprecation_errors), berrors);
}

c_mongo_server_api::c_mongo_server_api(const org_type* right)
{
	this_type::pro_copy(*reinterpret_cast<const base_type*>(right));
}

c_mongo_server_api::c_mongo_server_api(const org_type& right)
{
	this_type::pro_copy(reinterpret_cast<const base_type&>(right));
}

c_mongo_server_api::c_mongo_server_api(const this_type& right)
{
	this_type::pro_copy(right);
}

c_mongo_server_api::~c_mongo_server_api(void)
{
}

//public:
c_mongo_server_api::this_type& c_mongo_server_api::operator=(const org_type& right)
{
	this_type::pro_copy(reinterpret_cast<const base_type&>(right));
	return *this;
}

c_mongo_server_api::this_type& c_mongo_server_api::operator=(const this_type& right)
{
	this_type::pro_copy(right);
	return *this;
}

//public:
void c_mongo_server_api::swap(org_type& right)
{
	this_type::pro_swap(reinterpret_cast<base_type&>(right));
}

void c_mongo_server_api::swap(this_type& right)
{
	this_type::pro_swap(right);
}

//private:
void c_mongo_server_api::prv_init(void)
{
	memset(static_cast<base_type*>(this), 0, sizeof(base_type));
}

void c_mongo_server_api::pro_copy(const base_type& right)
{
	if(static_cast<base_type*>(this) == boost::addressof(right))
	{
		return;
	}

	memcpy(static_cast<base_type*>(this), boost::addressof(right), sizeof(base_type));
}

void c_mongo_server_api::pro_copy(const this_type& right)
{
	this_type::pro_copy(static_cast<const base_type&>(right));
}

void c_mongo_server_api::pro_swap(base_type& right)
{
	if(static_cast<base_type*>(this) == boost::addressof(right))
	{
		return;
	}

	base_type tmp = {static_cast<org_version_type>(0)};
	memcpy(boost::addressof(tmp), static_cast<base_type*>(this), sizeof(base_type));
	memcpy(static_cast<base_type*>(this), boost::addressof(right), sizeof(base_type));
	memcpy(boost::addressof(right), boost::addressof(tmp), sizeof(base_type));
}

void c_mongo_server_api::pro_swap(this_type& right)
{
	this_type::pro_swap(static_cast<base_type&>(right));
}

} // namespace nsql_database_system
} // namespace yggr
