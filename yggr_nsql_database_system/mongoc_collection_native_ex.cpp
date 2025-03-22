//mongoc_collection_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_collection_native_ex.hpp>

#define BSON_COMPILATION
#	include <bson/bson-private.h>
#undef BSON_COMPILATION

#define MONGOC_COMPILATION
#	include <mongoc-util-private.h>
#	include <mongoc-collection-private.h>
#undef MONGOC_COMPILATION

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

/*static*/ 
bool mongoc_collection_native_ex::s_mongoc_collection_stats(mongoc_collection_t *collection,
															const bson_t *options,
															bson_t *stats,
															bson_error_t *error)
{
	bson_iter_t iter;
	bson_t cmd = BSON_INITIALIZER;
	bool ret;

	BSON_ASSERT_PARAM (collection);

	if (options && bson_iter_init_find (&iter, options, "scale") &&
		!BSON_ITER_HOLDS_INT32 (&iter)) {
		bson_set_error (error,
						MONGOC_ERROR_BSON,
						MONGOC_ERROR_BSON_INVALID,
						"'scale' must be an int32 value.");
		return false;
	}

	BSON_APPEND_UTF8 (&cmd, "collStats", collection->collection);

	if (options) {
		bson_concat (&cmd, options);
	}

	/* Server Selection Spec: "may-use-secondary" commands SHOULD take a read
	* preference argument and otherwise MUST use the default read preference
	* from client, database or collection configuration. */
	ret = mongoc_collection_command_simple (
		collection, &cmd, collection->read_prefs, stats, error);

	bson_destroy (&cmd);

	return ret;
}

/*static*/ 
bool mongoc_collection_native_ex::s_mongoc_collection_validate(mongoc_collection_t* collection, /* IN */
																const bson_t* options,           /* IN */
																bson_t* reply,                   /* OUT */
																bson_error_t* error)             /* OUT */
{
	bson_iter_t iter;
	bson_t cmd = BSON_INITIALIZER;
	bool ret = false;
	bool reply_initialized = false;

	BSON_ASSERT_PARAM (collection);

	if (options && bson_iter_init_find (&iter, options, "full") &&
		!BSON_ITER_HOLDS_BOOL (&iter)) {
		bson_set_error (error,
						MONGOC_ERROR_BSON,
						MONGOC_ERROR_BSON_INVALID,
						"'full' must be a boolean value.");
		goto cleanup;
	}

	bson_append_utf8 (
		&cmd, "validate", 8, collection->collection, collection->collectionlen);

	if (options) {
		bson_concat (&cmd, options);
	}

	ret =
		mongoc_collection_command_simple (collection, &cmd, NULL, reply, error);
	reply_initialized = true;

cleanup:
	bson_destroy (&cmd);

	if (reply && !reply_initialized) {
		bson_init (reply);
	}

	return ret;
}

} // namespace nsql_database_system
} // namespace yggr

