// mongoc_gridfs_bucket_t_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_gridfs_bucket_t_native_ex.hpp>

#include <boost/ref.hpp>

#define MONGOC_COMPILATION
#	include <mongoc-gridfs-bucket-private.h>
#undef MONGOC_COMPILATION

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
mongoc_collection_t* mongoc_gridfs_bucket_t_native_ex::var_chunks(mongoc_gridfs_bucket_t* pbucket)
{
	return pbucket? pbucket->chunks : 0;
}

/*static*/ 
mongoc_collection_t* mongoc_gridfs_bucket_t_native_ex::var_files(mongoc_gridfs_bucket_t* pbucket)
{
	return pbucket? pbucket->files : 0;
}

/*static*/ 
int32_t mongoc_gridfs_bucket_t_native_ex::var_chunk_size(mongoc_gridfs_bucket_t* pbucket)
{
	return pbucket? pbucket->chunk_size : 0;
}

/*static*/ 
char* mongoc_gridfs_bucket_t_native_ex::var_bucket_name(mongoc_gridfs_bucket_t* pbucket)
{
	return pbucket? pbucket->bucket_name : 0;
}

/*static*/ 
bool mongoc_gridfs_bucket_t_native_ex::var_indexed(mongoc_gridfs_bucket_t* pbucket)
{
	//return pbucket? pbucket->indexed : false;
	return pbucket && pbucket->indexed;
}

} // namespace nsql_database_system
} // namespace yggr
