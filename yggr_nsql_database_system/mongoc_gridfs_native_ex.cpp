//mongoc_gridfs_native_ex.cpp

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


#include <yggr/nsql_database_system/mongoc_gridfs_native_ex.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>

#define MONGOC_COMPILATION
#	include <mongoc-gridfs-private.h>
#undef MONGOC_COMPILATION

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

struct yggr_mongoc_gridfs_t
{
   mongoc_client_t     *client;
   mongoc_collection_t *files;
   mongoc_collection_t *chunks;
};

/*static*/ 
mongoc_gridfs_native_ex::org_client_type* 
	mongoc_gridfs_native_ex::s_get_mongoc_client(const org_gridfs_type* pgridfs)
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(yggr_mongoc_gridfs_t) == sizeof(org_gridfs_type))>));
	const yggr_mongoc_gridfs_t* ptmp = reinterpret_cast<const yggr_mongoc_gridfs_t*>(pgridfs);

	return ptmp? ptmp->client : 0;
}


} // namespace nsql_database_system
} // namespace yggr

