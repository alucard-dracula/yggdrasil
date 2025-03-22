//c_mongo_write_concern.cpp

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

#include <yggr/nsql_database_system/c_mongo_write_concern.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/utility/swap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_write_concern::c_mongo_write_concern(void)
{
	mongoc_write_concern_native_ex::s_mongoc_write_concern_init(this);
}

c_mongo_write_concern::c_mongo_write_concern(s32 w_count)
{
	mongoc_write_concern_native_ex::s_mongoc_write_concern_init(this);
	mongoc_write_concern_native_ex::s_mongoc_write_concern_set_w(this, w_count);
}

c_mongo_write_concern::c_mongo_write_concern(const org_type* ptr)
{
	mongoc_write_concern_native_ex::s_mongoc_write_concern_init(this);
	if(ptr)
	{
		mongoc_write_concern_native_ex::s_mongoc_write_concern_copy(this, ptr);
	}
}

c_mongo_write_concern::c_mongo_write_concern(const org_type& right)
{
	mongoc_write_concern_native_ex::s_mongoc_write_concern_init(this);
	mongoc_write_concern_native_ex::s_mongoc_write_concern_copy(this, boost::addressof(right));
}

c_mongo_write_concern::c_mongo_write_concern(const this_type& right)
{
	mongoc_write_concern_native_ex::s_mongoc_write_concern_init(this);
	mongoc_write_concern_native_ex::s_mongoc_write_concern_copy(this, boost::addressof(right));
}

c_mongo_write_concern::~c_mongo_write_concern(void)
{
	mongoc_write_concern_native_ex::s_mongoc_write_concern_static_destroy(this);
}

c_mongo_write_concern::this_type& c_mongo_write_concern::operator=(const org_type& right)
{
	mongoc_write_concern_native_ex::s_mongoc_write_concern_copy(this, boost::addressof(right));
	return *this;
}

c_mongo_write_concern::this_type& c_mongo_write_concern::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	mongoc_write_concern_native_ex::s_mongoc_write_concern_copy(this, boost::addressof(right));
	return *this;
}

void c_mongo_write_concern::swap(org_type& right)
{
	mongoc_write_concern_native_ex::s_mongoc_write_concern_swap(this, &right);
}

void c_mongo_write_concern::swap(this_type& right)
{
	mongoc_write_concern_native_ex::s_mongoc_write_concern_swap(this, &right);
}

} // namespace nsql_database_system
} // namespace yggr
