// mongoc_host_list_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_host_list_native_ex.hpp>

#if !defined(MONGOC_INSIDE)
#	define MONGOC_INSIDE
#endif // MONGOC_INSIDE

#	include <mongoc-host-list-private.h>

#undef MONGOC_INSIDE

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
mongoc_host_list_t* mongoc_host_list_native_ex::s_mongoc_host_list_copy_all(const mongoc_host_list_t* src)
{
	return src? _mongoc_host_list_copy_all(src) : 0;
}

/*static*/ 
void mongoc_host_list_native_ex::s_mongoc_host_list_destroy_all(mongoc_host_list_t* src)
{
	if(src)
	{
		_mongoc_host_list_destroy_all(src);
	}
}

} // namespace nsql_database_system
} // namespace yggr
