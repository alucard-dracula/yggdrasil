// c_mongo_client_encryption_encrypt_range_opts.cpp

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

#include <yggr/nsql_database_system/c_mongo_client_encryption_encrypt_range_opts.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

//public:
c_mongo_client_encryption_encrypt_range_opts::c_mongo_client_encryption_encrypt_range_opts(void)
{
	this_type::pro_init();
}

c_mongo_client_encryption_encrypt_range_opts::c_mongo_client_encryption_encrypt_range_opts(s64 arg_sparsity)
{
	this_type::pro_init();

	base_type::sparsity = arg_sparsity;
}

c_mongo_client_encryption_encrypt_range_opts::c_mongo_client_encryption_encrypt_range_opts(const org_type* ptr)
{
	this_type::pro_init();
	this_type::pro_copy(ptr);
}

c_mongo_client_encryption_encrypt_range_opts::c_mongo_client_encryption_encrypt_range_opts(const org_type& right)
{
	this_type::pro_init();
	this_type::pro_copy(boost::addressof(right));
}

c_mongo_client_encryption_encrypt_range_opts::c_mongo_client_encryption_encrypt_range_opts(const this_type& right)
{
	this_type::pro_init();
	this_type::pro_copy(right);
}

c_mongo_client_encryption_encrypt_range_opts::~c_mongo_client_encryption_encrypt_range_opts(void)
{
	this_type::pro_clear();
}

//public:
c_mongo_client_encryption_encrypt_range_opts::this_type&
	c_mongo_client_encryption_encrypt_range_opts::operator=(const org_type& right)
{
	return this_type::pro_copy(boost::addressof(right));
}

c_mongo_client_encryption_encrypt_range_opts::this_type&
	c_mongo_client_encryption_encrypt_range_opts::operator=(const base_type& right)
{
	return this_type::pro_copy(reinterpret_cast<const org_type*>(boost::addressof(right)));
}

c_mongo_client_encryption_encrypt_range_opts::this_type&
	c_mongo_client_encryption_encrypt_range_opts::operator=(const this_type& right)
{
	return this_type::pro_copy(right);
}

//public:
void c_mongo_client_encryption_encrypt_range_opts::swap(this_type& right)
{
	this_type::pro_swap(right);
}

//protected:
void c_mongo_client_encryption_encrypt_range_opts::pro_init(void)
{
	base_type& base_ref = *this;
	native_ex_type::s_mongoc_client_encryption_encrypt_range_opts_init(boost::addressof(base_ref));
}

void c_mongo_client_encryption_encrypt_range_opts::pro_clear(void)
{
	native_ex_type::s_mongoc_client_encryption_encrypt_range_opts_clear(
		this_type::org_pointer());
}

c_mongo_client_encryption_encrypt_range_opts::this_type&
	c_mongo_client_encryption_encrypt_range_opts::pro_copy(const org_type* psrc)
{
	//base_type& base_ref = *this;
	native_ex_type::s_mongoc_client_encryption_encrypt_range_opts_copy(
		this_type::org_pointer(), psrc);
	return *this;
}

c_mongo_client_encryption_encrypt_range_opts::this_type&
	c_mongo_client_encryption_encrypt_range_opts::pro_copy(const this_type& right)
{
	native_ex_type::s_mongoc_client_encryption_encrypt_range_opts_copy(
		this_type::org_pointer(), right.org_pointer());
	return *this;
}

void c_mongo_client_encryption_encrypt_range_opts::pro_swap(this_type& right)
{
	native_ex_type::s_mongoc_client_encryption_encrypt_range_opts_swap(
		this_type::org_pointer(), right.org_pointer());
}

c_mongo_client_encryption_encrypt_range_opts::this_type&
	c_mongo_client_encryption_encrypt_range_opts::pro_move(this_type& right)
{
	native_ex_type::s_mongoc_client_encryption_encrypt_range_opts_move(
		this_type::org_pointer(), right.org_pointer());

	return *this;
}

} // namespace nsql_database_system
} // namespace yggr

