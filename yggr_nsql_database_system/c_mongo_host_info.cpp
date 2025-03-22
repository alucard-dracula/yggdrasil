// c_mongo_host_info.hpp

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

#include <yggr/nsql_database_system/c_mongo_host_info.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_host_info::c_mongo_host_info(void)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));
}

c_mongo_host_info::c_mongo_host_info(const org_type& right)
{
	base_type& base = *this;

	memset(&base, 0, sizeof(base_type));
	memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right), E_copy_size);
}

c_mongo_host_info::c_mongo_host_info(const org_type* right)
{
	base_type& base = *this;
	memset(&base, 0, sizeof(base_type));

	if(!right)
	{
		return;
	}

	memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(right), E_copy_size);
}

c_mongo_host_info::c_mongo_host_info(const base_type& right)
{
	base_type& base = *this;

	memset(&base, 0, sizeof(base_type));
	memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right), E_copy_size);
}

c_mongo_host_info::c_mongo_host_info(const this_type& right)
{
	base_type& base = *this;

	memset(&base, 0, sizeof(base_type));
	memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right), E_copy_size);
}

c_mongo_host_info::~c_mongo_host_info(void)
{
}

c_mongo_host_info::this_type& c_mongo_host_info::operator=(const org_type& right)
{
	if(reinterpret_cast<void*>(this) 
		== reinterpret_cast<const void*>(&right))
	{
		return *this;
	}

	base_type& base = *this;
	memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right), E_copy_size);
	return *this;
}


c_mongo_host_info::this_type& c_mongo_host_info::operator=(const base_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	base_type& base = *this;
	memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right), E_copy_size);
	return *this;
}


c_mongo_host_info::this_type& c_mongo_host_info::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	base_type& base = *this;
	memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right), E_copy_size);
	return *this;
}

void c_mongo_host_info::swap(org_type& right)
{
	if(reinterpret_cast<void*>(this) 
		== reinterpret_cast<void*>(&right))
	{
		return;
	}

	base_type& base = *this;
	base_type tmp = {0};

	memcpy(this_type::pro_get_copy_begin(&tmp), this_type::pro_get_copy_begin(&base), E_copy_size);
	memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right), E_copy_size);
	memcpy(this_type::pro_get_copy_begin(&right), this_type::pro_get_copy_begin(&tmp), E_copy_size);
}

void c_mongo_host_info::swap(base_type& right)
{
	if(this == &right)
	{
		return;
	}

	base_type& base = *this;
	base_type tmp = {0};

	memcpy(this_type::pro_get_copy_begin(&tmp), this_type::pro_get_copy_begin(&base), E_copy_size);
	memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right), E_copy_size);
	memcpy(this_type::pro_get_copy_begin(&right), this_type::pro_get_copy_begin(&tmp), E_copy_size);
}

void c_mongo_host_info::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	base_type& base = *this;
	base_type tmp = {0};

	memcpy(this_type::pro_get_copy_begin(&tmp), this_type::pro_get_copy_begin(&base), E_copy_size);
	memcpy(this_type::pro_get_copy_begin(&base), this_type::pro_get_copy_begin(&right), E_copy_size);
	memcpy(this_type::pro_get_copy_begin(&right), this_type::pro_get_copy_begin(&tmp), E_copy_size);
}

} // namespace nsql_database_system
} // namespace yggr
