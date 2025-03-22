//mongoc_gridfs_bucket_file_t_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_GRIDFS_BUCKET_FILE_T_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_GRIDFS_BUCKET_FILE_T_NATIVE_EX_HPP__

#include <yggr/nsql_database_system/mongodb_config.hpp>

//typedef struct {
//   /* corresponding bucket */
//   mongoc_gridfs_bucket_t *bucket;
//
//   /* file data */
//   char *filename;
//   bson_value_t *file_id;
//   bson_t *metadata;
//   int32_t chunk_size;
//   int64_t length;
//
//   /* fields for reading and writing */
//   uint8_t *buffer;
//   size_t in_buffer;
//   int32_t curr_chunk;
//
//   /* for writing */
//   bool saved;
//
//   /* for reading */
//   mongoc_cursor_t *cursor;
//   size_t bytes_read;
//   bool finished;
//
//   /* Error */
//   bson_error_t err;
//} mongoc_gridfs_bucket_file_t;

namespace yggr
{
namespace nsql_database_system
{

class mongoc_gridfs_bucket_file_t_native_ex
{
private:
	typedef mongoc_gridfs_bucket_file_t_native_ex this_type;
	
	// cpp file has grobal function to_mongoc_gridfs_bucket_file_t_pointer;

public:
	/* corresponding bucket */
	static mongoc_gridfs_bucket_t* var_bucket(void* pbucket_file);

	/* file data */
	static char* var_filename(void* pbucket_file);
	static bson_value_t* var_file_id(void* pbucket_file);
	static bson_t* var_metadata(void* pbucket_file);
	static int32_t var_chunk_size(void* pbucket_file);
	static int64_t var_length(void* pbucket_file);

	//   /* fields for reading and writing */
	static uint8_t* var_buffer(void* pbucket_file);
	static size_t var_in_buffer(void* pbucket_file);
	static int32_t var_curr_chunk(void* pbucket_file);

	//   /* for writing */
	static bool var_saved(void* pbucket_file);

	//   /* for reading */
	static mongoc_cursor_t* var_cursor(void* pbucket_file);
	static size_t var_bytes_read(void* pbucket_file);
	static bool var_finished(void* pbucket_file);

	//   /* Error */
	static bson_error_t* var_err(void* pbucket_file);
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_GRIDFS_BUCKET_FILE_T_NATIVE_EX_HPP__
