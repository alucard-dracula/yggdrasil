// bson_json_opts.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_JSON_OPTS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_JSON_OPTS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/type_traits/number_t.hpp>

#include <yggr/nsql_database_system/bson_config.hpp>

namespace yggr
{
namespace nsql_database_system
{

/*
struct _bson_json_opts_t {
   bson_json_mode_t mode;
   int32_t max_len;
};
*/

typedef struct 
{
	YGGR_PP_UINTEGER_T_TYPE(sizeof(bson_json_mode_t)) mode;
	int32_t max_len;
} native_bson_json_opts_t;

struct bson_json_mode_def
{
public:
	typedef YGGR_PP_UINTEGER_T_TYPE(sizeof(bson_json_mode_t)) mode_type;

public:
	YGGR_STATIC_CONSTANT(mode_type, E_BSON_JSON_MODE_LEGACY = BSON_JSON_MODE_LEGACY); // 0
	YGGR_STATIC_CONSTANT(mode_type, E_BSON_JSON_MODE_CANONICAL = BSON_JSON_MODE_CANONICAL); // 1
	YGGR_STATIC_CONSTANT(mode_type, E_BSON_JSON_MODE_RELAXED = BSON_JSON_MODE_RELAXED); // 2

	YGGR_STATIC_CONSTANT(int32_t, E_BSON_MAX_LEN_UNLIMITED = BSON_MAX_LEN_UNLIMITED); // -1

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_JSON_OPTS_HPP__