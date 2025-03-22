//mongoc_gridfs_file_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_gridfs_file_native_ex.hpp>
#include <yggr/nsql_database_system/mongoc_gridfs_native_ex.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>

#define MONGOC_COMPILATION
#	include <mongoc-gridfs-file-private.h>
#undef MONGOC_COMPILATION

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

struct yggr_mongoc_gridfs_file_t
{
   mongoc_gridfs_t           *gridfs;
   bson_t                     bson;
   mongoc_gridfs_file_page_t *page;
   uint64_t                   pos;
   int32_t                    n;
   bson_error_t               error;
   mongoc_cursor_t           *cursor;
   uint32_t                   cursor_range[2]; /* current chunk, # of chunks */
   bool                       is_dirty;

   bson_value_t               files_id;
   int64_t                    length;
   int32_t                    chunk_size;
   int64_t                    upload_date;

   char                      *md5;
   char                      *filename;
   char                      *content_type;
   bson_t                     aliases;
   bson_t                     metadata;
   const char                *bson_md5;
   const char                *bson_filename;
   const char                *bson_content_type;
   bson_t                     bson_aliases;
   bson_t                     bson_metadata;
};

/*static*/ 
mongoc_gridfs_file_native_ex::org_gridfs_type* 
	mongoc_gridfs_file_native_ex::s_get_mongoc_gridfs(const mongoc_gridfs_file_t* pgridfs_file)
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(yggr_mongoc_gridfs_file_t)== sizeof(mongoc_gridfs_file_t))>));
	const yggr_mongoc_gridfs_file_t* ptmp = reinterpret_cast<const yggr_mongoc_gridfs_file_t*>(pgridfs_file);

	return ptmp? ptmp->gridfs : 0;
}

/*static*/ 
mongoc_gridfs_file_native_ex::org_client_type* 
	mongoc_gridfs_file_native_ex::s_get_mongoc_client(const mongoc_gridfs_file_t* pgridfs_file)
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(yggr_mongoc_gridfs_file_t)== sizeof(mongoc_gridfs_file_t))>));
	const org_gridfs_type* pgridfs = this_type::s_get_mongoc_gridfs(pgridfs_file);
	return pgridfs? mongoc_gridfs_native_ex::s_get_mongoc_client(pgridfs) : 0;
}

} // namespace nsql_database_system
} // namespace yggr

