//mongoc_client_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_client_native_ex.hpp>

#define MONGOC_COMPILATION
#	include <mongoc-client-private.h>
#undef MONGOC_COMPILATION

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
mongoc_read_prefs_t*
	mongoc_client_native_ex::s_set_read_prefs_pointer(mongoc_client_t* pclt, mongoc_read_prefs_t* pread_prefs)
{
	if((pclt && pread_prefs))
	{
		mongoc_read_prefs_t* tmp_ptr = pclt->read_prefs;
		pclt->read_prefs = pread_prefs;
		return tmp_ptr;
	}
	else
	{
		return pread_prefs;
	}
}

/*static*/ 
mongoc_read_concern_t*
	mongoc_client_native_ex::s_set_read_concern_pointer(mongoc_client_t* pclt, mongoc_read_concern_t* pread_concern)
{
	if((pclt && pread_concern))
	{
		mongoc_read_concern_t* tmp_ptr = pclt->read_concern;
		pclt->read_concern = pread_concern;
		return tmp_ptr;
	}
	else
	{
		return pread_concern;
	}
}

/*static*/ 
mongoc_write_concern_t*
	mongoc_client_native_ex::s_set_write_concern_pointer(mongoc_client_t* pclt, mongoc_write_concern_t* pwrite_concern)
{
	if((pclt && pwrite_concern))
	{
		mongoc_write_concern_t* tmp_ptr = pclt->write_concern;
		pclt->write_concern = pwrite_concern;
		return tmp_ptr;
	}
	else
	{
		return pwrite_concern;
	}
}

} // namespace nsql_database_system
} // namespace yggr
