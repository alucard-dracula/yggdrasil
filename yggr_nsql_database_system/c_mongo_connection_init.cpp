//c_mongo_connection_init.cpp

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

#include <yggr/nsql_database_system/c_mongo_connection_init.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_connection_init::c_mongo_connection_init(void)
{
}

c_mongo_connection_init::c_mongo_connection_init(const uri_type& uri)
	: uri_type(uri)
{
}
	
#ifdef MONGOC_ENABLE_SSL

c_mongo_connection_init::c_mongo_connection_init(const uri_type& uri,
						const ssl_opt_type& ssl_opt)
	: uri_type(uri), ssl_opt_type(ssl_opt)
{
}

#endif // MONGOC_ENABLE_SSL


c_mongo_connection_init::c_mongo_connection_init(const this_type& right)
	: uri_type(static_cast<const uri_type&>(right))
#ifdef MONGOC_ENABLE_SSL
	, ssl_opt_type(static_cast<const ssl_opt_type&>(right))
#endif // MONGOC_ENABLE_SSL
{
}

c_mongo_connection_init::~c_mongo_connection_init(void)
{
}

c_mongo_connection_init::this_type& c_mongo_connection_init::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	uri_type::operator=(right);
#if defined(MONGOC_ENABLE_SSL)
	ssl_opt_type::operator=(right);
#endif // MONGOC_ENABLE_SSL
	return *this;
}

void c_mongo_connection_init::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	uri_type::swap(right);
#if defined(MONGOC_ENABLE_SSL)
	ssl_opt_type::swap(right);
#endif // MONGOC_ENABLE_SSL
}

} // namespace nsql_database_system
} // namespace yggr

