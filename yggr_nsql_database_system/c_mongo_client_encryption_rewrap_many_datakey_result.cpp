// c_mongo_client_encryption_rewrap_many_datakey_result.cpp

//c_mongo_apm_callbacks.hpp

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

#include <yggr/nsql_database_system/c_mongo_client_encryption_rewrap_many_datakey_result.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_client_encryption_rewrap_many_datakey_result::c_mongo_client_encryption_rewrap_many_datakey_result(void)
{
	this_type::pro_init();
}

c_mongo_client_encryption_rewrap_many_datakey_result::c_mongo_client_encryption_rewrap_many_datakey_result(const bson_t& bs_data)
{
	this_type::pro_init();

	bson_native_ex::s_bson_copy(
		boost::addressof(base_type::bulk_write_result),
		boost::addressof(bs_data));
}

c_mongo_client_encryption_rewrap_many_datakey_result::c_mongo_client_encryption_rewrap_many_datakey_result(const org_type* ptr)
{
	this_type::pro_init();
	this_type::pro_copy(ptr);
}

c_mongo_client_encryption_rewrap_many_datakey_result::c_mongo_client_encryption_rewrap_many_datakey_result(const org_type& right)
{
	this_type::pro_init();
	this_type::pro_copy(boost::addressof(right));
}

c_mongo_client_encryption_rewrap_many_datakey_result::c_mongo_client_encryption_rewrap_many_datakey_result(const this_type& right)
{
	this_type::pro_init();
	this_type::pro_copy(right);
}

c_mongo_client_encryption_rewrap_many_datakey_result::~c_mongo_client_encryption_rewrap_many_datakey_result(void)
{
	if(base_type::bulk_write_result.len)
	{
		bson_destroy(boost::addressof(base_type::bulk_write_result));
	}
}

//public:
c_mongo_client_encryption_rewrap_many_datakey_result::this_type& 
	c_mongo_client_encryption_rewrap_many_datakey_result::operator=(const bson_t& right)
{
	bson_native_ex::s_bson_copy(
		boost::addressof(base_type::bulk_write_result), 
		boost::addressof(right) );

	return *this;
}

c_mongo_client_encryption_rewrap_many_datakey_result::this_type& 
	c_mongo_client_encryption_rewrap_many_datakey_result::operator=(const org_type& right)
{
	return this_type::pro_copy(boost::addressof(right));
}

c_mongo_client_encryption_rewrap_many_datakey_result::this_type& 
	c_mongo_client_encryption_rewrap_many_datakey_result::operator=(const base_type& right)
{
	return this_type::pro_copy(reinterpret_cast<const org_type*>(boost::addressof(right)));
}

c_mongo_client_encryption_rewrap_many_datakey_result::this_type& 
	c_mongo_client_encryption_rewrap_many_datakey_result::operator=(const this_type& right)
{
	return this_type::pro_copy(right);
}

//public:
void c_mongo_client_encryption_rewrap_many_datakey_result::swap(this_type& right)
{
	this_type::pro_swap(right);
}

//protected:
void c_mongo_client_encryption_rewrap_many_datakey_result::pro_init(void)
{
	memset(boost::addressof(base_type::bulk_write_result), 0, sizeof(base_type));
	::bson_init(boost::addressof(base_type::bulk_write_result));
}

void c_mongo_client_encryption_rewrap_many_datakey_result::pro_clear(void)
{
	this_type::result().clear();
}

c_mongo_client_encryption_rewrap_many_datakey_result::this_type& 
	c_mongo_client_encryption_rewrap_many_datakey_result::pro_copy(const org_type* ptr)
{
	if(!ptr || ptr == reinterpret_cast<org_type*>(this))
	{
		return *this;
	}

	const base_type* prbase = reinterpret_cast<const base_type*>(ptr);

	bson_native_ex::s_bson_copy(
		boost::addressof(base_type::bulk_write_result), 
		boost::addressof(prbase->bulk_write_result) );

	return *this;
}

c_mongo_client_encryption_rewrap_many_datakey_result::this_type& 
	c_mongo_client_encryption_rewrap_many_datakey_result::pro_copy(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	bson_native_ex::s_bson_copy(
		boost::addressof(base_type::bulk_write_result), 
		boost::addressof(right.bulk_write_result) );

	return *this;
}

void c_mongo_client_encryption_rewrap_many_datakey_result::pro_swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	bson_native_ex::s_bson_swap(
		boost::addressof(base_type::bulk_write_result), 
		boost::addressof(right.bulk_write_result) );
}

c_mongo_client_encryption_rewrap_many_datakey_result::this_type& 
	c_mongo_client_encryption_rewrap_many_datakey_result::pro_move(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	bson_native_ex::s_bson_swap(
		boost::addressof(base_type::bulk_write_result), 
		boost::addressof(right.bulk_write_result) );

	return *this;
}

} // namespace nsql_database_system
} // namespace yggr
