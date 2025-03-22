//mongoc_gridfs_bucket_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_gridfs_bucket_native_ex.hpp>

#include <boost/ref.hpp>

#define MONGOC_COMPILATION
#	include <mongoc-stream-private.h>
#	include <mongoc-stream-gridfs-upload-private.h>
#	include <mongoc-stream-gridfs-download-private.h>
#undef MONGOC_COMPILATION

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
bool mongoc_gridfs_bucket_native_ex::s_mongoc_gridfs_bucket_upload_get_error(mongoc_stream_t* chk_stream, bson_error_t* error)
{
	BSON_ASSERT(chk_stream);
	if(!(chk_stream && chk_stream->type == MONGOC_STREAM_GRIDFS_UPLOAD))
	{
		return false;
	}

	mongoc_gridfs_upload_stream_t* pgridfs = 
		reinterpret_cast<mongoc_gridfs_upload_stream_t*>(chk_stream);
	if(error && pgridfs->file)
	{
		memcpy(error, boost::addressof(pgridfs->file->err), sizeof(bson_error_t));
	}

	return !!(pgridfs->file->err.code);
}

/*static*/ 
void* mongoc_gridfs_bucket_native_ex::s_mongoc_gridfs_bucket_upload_get_file(mongoc_stream_t* chk_stream)
{
	BSON_ASSERT(chk_stream);
	return 
		chk_stream && chk_stream->type == MONGOC_STREAM_GRIDFS_UPLOAD?
			static_cast<void*>(reinterpret_cast<mongoc_gridfs_upload_stream_t*>(chk_stream)->file)
			: 0;
}

/*static*/ 
bool mongoc_gridfs_bucket_native_ex::s_mongoc_gridfs_bucket_upload_from_stream_with_id(mongoc_gridfs_bucket_t *bucket,
																						const bson_value_t *file_id,
																						const char *filename,
																						mongoc_stream_t *source,
																						const bson_t *opts,
																						bson_error_t *error)
{
	mongoc_stream_t* upload_stream = 0;
	ssize_t bytes_read = 0;
	ssize_t bytes_written = 0;
	char buf[512] = {0};

	BSON_ASSERT (bucket);
	BSON_ASSERT (file_id);
	BSON_ASSERT (filename);
	BSON_ASSERT (source);

	if(error)
	{
		memset(error, 0, sizeof(bson_error_t));
	}

	upload_stream = mongoc_gridfs_bucket_open_upload_stream_with_id (
		bucket, file_id, filename, opts, error);

	if(!upload_stream)
	{
		return false;
	}

	while((bytes_read = mongoc_stream_read(source, buf, 512, 1, 0)) > 0) 
	{
		bytes_written = mongoc_stream_write(upload_stream, buf, bytes_read, 0);
		if(bytes_written < 0) 
		{
			BSON_ASSERT (mongoc_gridfs_bucket_stream_error (upload_stream, error));
			mongoc_gridfs_bucket_abort_upload (upload_stream);
			mongoc_stream_destroy (upload_stream);
			return false;
		}
	}

	if (bytes_read < 0) 
	{
		mongoc_gridfs_bucket_abort_upload(upload_stream);
		bson_set_error (error,
						MONGOC_ERROR_GRIDFS,
						MONGOC_ERROR_GRIDFS_BUCKET_STREAM,
						"Error occurred on the provided stream.");
		mongoc_stream_destroy (upload_stream);
		return false;
	} 
	else 
	{
		bool bhas_err = 
			(0 != mongoc_stream_close(upload_stream))
			&& s_mongoc_gridfs_bucket_upload_get_error(upload_stream, error);

		mongoc_stream_destroy(upload_stream);
		return !bhas_err;
	}
}

//public:

/*static*/ 
bool mongoc_gridfs_bucket_native_ex::s_mongoc_gridfs_bucket_download_get_error(mongoc_stream_t* chk_stream, bson_error_t* error)
{
	BSON_ASSERT(chk_stream);
	if(!(chk_stream && chk_stream->type == MONGOC_STREAM_GRIDFS_DOWNLOAD))
	{
		return false;
	}

	mongoc_gridfs_download_stream_t* pgridfs = 
		reinterpret_cast<mongoc_gridfs_download_stream_t*>(chk_stream);
	if(error && pgridfs->file)
	{
		memcpy(error, boost::addressof(pgridfs->file->err), sizeof(bson_error_t));
	}

	return !!(pgridfs->file->err.code);
}

/*static*/ 
void* mongoc_gridfs_bucket_native_ex::s_mongoc_gridfs_bucket_download_get_file(mongoc_stream_t* chk_stream)
{
	BSON_ASSERT(chk_stream);
	return 
		chk_stream && chk_stream->type == MONGOC_STREAM_GRIDFS_DOWNLOAD?
			static_cast<void*>(reinterpret_cast<mongoc_gridfs_download_stream_t*>(chk_stream)->file)
			: 0;
}

} // namespace nsql_database_system
} // namespace yggr

