//mongoc_collection_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_COLLECTION_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_COLLECTION_NATIVE_EX_HPP__

#include <yggr/nsql_database_system/mongodb_config.hpp>

namespace yggr
{
namespace nsql_database_system
{


class mongoc_collection_native_ex
{
private:
	typedef mongoc_collection_native_ex this_type;
	
public:
	//static bool s_mongoc_collection_save(mongoc_collection_t *collection,
	//										const bson_t *document,
	//										const mongoc_write_concern_t *write_concern,
	//										bson_error_t *error);

	static bool s_mongoc_collection_stats(mongoc_collection_t *collection,
											const bson_t *options,
											bson_t *stats,
											bson_error_t *error);

	static bool s_mongoc_collection_validate(mongoc_collection_t* collection,
												const bson_t* options,
												bson_t* reply,
												bson_error_t* error);

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_COLLECTION_NATIVE_EX_HPP__
