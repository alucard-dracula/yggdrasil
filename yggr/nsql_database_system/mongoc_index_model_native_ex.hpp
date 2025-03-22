//mongoc_index_model_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_INDEX_MODEL_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_INDEX_MODEL_NATIVE_EX_HPP__

#include <yggr/nsql_database_system/mongodb_config.hpp>

namespace yggr
{
namespace nsql_database_system
{

struct yggr_mongoc_index_model_t
{
	bson_t* keys;
	bson_t* opts;
};

class mongoc_index_model_native_ex
{
public:
	typedef mongoc_index_model_t org_index_model_type;
	typedef yggr_mongoc_index_model_t yggr_index_model_type;

private:
	typedef mongoc_index_model_native_ex this_type;

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_INDEX_MODEL_NATIVE_EX_HPP__
