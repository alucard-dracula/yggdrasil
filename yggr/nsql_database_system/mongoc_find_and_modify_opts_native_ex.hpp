//mongoc_find_and_modify_opts_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_FIND_AND_MODIFY_OPTS_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_FIND_AND_MODIFY_OPTS_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_find_and_modify_opts_t {
//   bson_t *sort;
//   bson_t *update;
//   bson_t *fields;
//   mongoc_find_and_modify_flags_t flags;
//   bool bypass_document_validation;
//   uint32_t max_time_ms;
//   bson_t extra;
//};

struct yggr_mongoc_find_and_modify_opts
{
	bson_t* sort;
	bson_t* update;
	bson_t* fields;
	u32  flags;
	u32  bypass_document_validation;
	uint32_t max_time_ms;
	bson_t extra;
};

class mongoc_find_and_modify_opts_native_ex
{
private:
	typedef mongoc_find_and_modify_opts_native_ex this_type;

public:
	// bypass document
	YGGR_STATIC_CONSTANT(u32, E_MONGOC_BYPASS_DOCUMENT_VALIDATION_FALSE = 0);
	YGGR_STATIC_CONSTANT(u32, E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE = (1 << 0));
	//YGGR_STATIC_CONSTANT(u32, E_MONGOC_BYPASS_DOCUMENT_VALIDATION_DEFAULT = (1 << 1));
	YGGR_STATIC_CONSTANT(u32, E_MONGOC_BYPASS_DOCUMENT_VALIDATION_DEFAULT = E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE);

public:
	// flag
	YGGR_STATIC_CONSTANT(u32, E_MONGOC_FIND_AND_MODIFY_NONE = MONGOC_FIND_AND_MODIFY_NONE);
	YGGR_STATIC_CONSTANT(u32, E_MONGOC_FIND_AND_MODIFY_REMOVE = MONGOC_FIND_AND_MODIFY_REMOVE);
	YGGR_STATIC_CONSTANT(u32, E_MONGOC_FIND_AND_MODIFY_UPSERT = MONGOC_FIND_AND_MODIFY_UPSERT);
	YGGR_STATIC_CONSTANT(u32, E_MONGOC_FIND_AND_MODIFY_RETURN_NEW = MONGOC_FIND_AND_MODIFY_RETURN_NEW);
	
public:
	static void* s_mongoc_find_and_modify_opts_init(void* popts);

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_FIND_AND_MODIFY_OPTS_NATIVE_EX_HPP__
