// mongo_const_strings_def.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_CONST_STRINGS_DEF_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_CONST_STRINGS_DEF_HPP__

#include <yggr/nsql_database_system/detail/const_string_make_function_macro.hpp>

namespace yggr
{
namespace nsql_database_system
{

#define YGGR_TMP_PP_MAKE_FUNCTION YGGR_TMP_PP_CONST_STRING_MAKE_FUNCTION

struct mongo_const_strings_def
{
	YGGR_TMP_PP_MAKE_FUNCTION(_id, "_id")

	YGGR_TMP_PP_MAKE_FUNCTION(files_id, "files_id")

	YGGR_TMP_PP_MAKE_FUNCTION(upsert, "upsert")
};

#undef YGGR_TMP_PP_MAKE_FUNCTION

typedef mongo_const_strings_def mongo_const_strings_def_type;

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGO_CONST_STRINGS_DEF_HPP__
