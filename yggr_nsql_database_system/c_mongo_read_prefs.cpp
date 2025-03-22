//c_mongo_read_prefs.cpp

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

#include <yggr/nsql_database_system/c_mongo_read_prefs.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_read_prefs::c_mongo_read_prefs(void)
{
	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_init(this);
}

c_mongo_read_prefs::c_mongo_read_prefs(mode_type mode)
{
	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_init(this, mode);
}

c_mongo_read_prefs::c_mongo_read_prefs(const org_type* pread_prefs)
{
	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_init(this);
	if(!pread_prefs)
	{
		return;
	}
	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_copy(this, pread_prefs);
}

c_mongo_read_prefs::c_mongo_read_prefs(const org_type& right)
{
	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_copy(this, &right);
}

c_mongo_read_prefs::c_mongo_read_prefs(const this_type& right)
{
	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_copy(this, &right);
}

c_mongo_read_prefs::~c_mongo_read_prefs(void)
{
	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_static_destroy(this);
}

c_mongo_read_prefs::this_type& c_mongo_read_prefs::operator=(const org_type& right)
{
	if(reinterpret_cast<void*>(this) == reinterpret_cast<const void*>(&right))
	{
		return *this;
	}

	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_copy(this, &right);
	return *this;
}

c_mongo_read_prefs::this_type& c_mongo_read_prefs::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_copy(this, &right);
	return *this;
}

void c_mongo_read_prefs::swap(org_type& right)
{
	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_swap(this, &right);
}

void c_mongo_read_prefs::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	mongoc_read_prefs_native_ex::s_mongoc_read_prefs_swap(this, &right);
}

} // namespace nsql_database_system
} // namespace yggr

