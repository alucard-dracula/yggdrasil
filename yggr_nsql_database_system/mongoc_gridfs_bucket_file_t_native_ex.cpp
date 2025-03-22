//mongoc_gridfs_bucket_file_t_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_gridfs_bucket_file_t_native_ex.hpp>

#include <boost/ref.hpp>

#include <cassert>

#define MONGOC_COMPILATION
#	include <mongoc-gridfs-bucket-file-private.h>
#undef MONGOC_COMPILATION

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

inline mongoc_gridfs_bucket_file_t* to_mongoc_gridfs_bucket_file_t_pointer(void* ptr)
{
	return reinterpret_cast<mongoc_gridfs_bucket_file_t*>(ptr);
}

} // namespace nsql_database_system
} // namespace yggr
namespace yggr
{
namespace nsql_database_system
{

/* corresponding bucket */
/*static*/ 
mongoc_gridfs_bucket_t* mongoc_gridfs_bucket_file_t_native_ex::var_bucket(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->bucket
			: 0;
}

/* file data */
/*static*/  
char* mongoc_gridfs_bucket_file_t_native_ex::var_filename(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->filename
			: 0;
}

/*static*/  
bson_value_t* mongoc_gridfs_bucket_file_t_native_ex::var_file_id(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->file_id
			: 0;
}

/*static*/  
bson_t* mongoc_gridfs_bucket_file_t_native_ex::var_metadata(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->metadata
			: 0;
}

/*static*/  
int32_t mongoc_gridfs_bucket_file_t_native_ex::var_chunk_size(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->chunk_size
			: 0;
}

/*static*/  
int64_t mongoc_gridfs_bucket_file_t_native_ex::var_length(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->length
			: 0;
}

//   /* fields for reading and writing */
/*static*/  
uint8_t* mongoc_gridfs_bucket_file_t_native_ex::var_buffer(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->buffer
			: 0;
}

/*static*/  
size_t mongoc_gridfs_bucket_file_t_native_ex::var_in_buffer(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->in_buffer
			: 0;
}

/*static*/  
int32_t mongoc_gridfs_bucket_file_t_native_ex::var_curr_chunk(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->curr_chunk
			: 0;
}

//   /* for writing */
/*static*/ 
bool mongoc_gridfs_bucket_file_t_native_ex::var_saved(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->saved
			: false;
}

//   /* for reading */
/*static*/  
mongoc_cursor_t* mongoc_gridfs_bucket_file_t_native_ex::var_cursor(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->cursor
			: 0;
}

/*static*/  
size_t mongoc_gridfs_bucket_file_t_native_ex::var_bytes_read(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->bytes_read
			: 0;
}

/*static*/  
bool mongoc_gridfs_bucket_file_t_native_ex::var_finished(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->finished
			: false;
}

//   /* Error */
/*static*/  
bson_error_t* mongoc_gridfs_bucket_file_t_native_ex::var_err(void* pbucket_file)
{
	assert(pbucket_file);
	return 
		pbucket_file?
			boost::addressof(to_mongoc_gridfs_bucket_file_t_pointer(pbucket_file)->err)
			: 0;
}

} // namespace nsql_database_system
} // namespace yggr

