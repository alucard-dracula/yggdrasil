// bson_error_code_ex_def.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_CODE_EX_DEF_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_CODE_EX_DEF_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

//typedef enum {
//   BSON_VALIDATE_NONE = 0,
//   BSON_VALIDATE_UTF8 = (1 << 0),
//   BSON_VALIDATE_DOLLAR_KEYS = (1 << 1),
//   BSON_VALIDATE_DOT_KEYS = (1 << 2),
//   BSON_VALIDATE_UTF8_ALLOW_NULL = (1 << 3),
//   BSON_VALIDATE_EMPTY_KEYS = (1 << 4),
//} bson_validate_flags_t;

namespace yggr
{
namespace nsql_database_system
{

struct bson_error_code_ex_def
{
	YGGR_STATIC_CONSTANT(enum_value_t, BSON_VALIDATE_CUSTOMER_MARK = 1 << 8);

	YGGR_STATIC_CONSTANT(enum_value_t, BSON_VALIDATE_DIFFERENT_TYPE = 1 << 9);
	YGGR_STATIC_CONSTANT(enum_value_t, BSON_VALIDATE_ONLY_SUPPORT_COMPARE_EQUAL_TYPE = 1 << 10);

	YGGR_CONSTEXPR_OR_INLINE static const char* msg_bson_validate_different_type(void)
	{
		return "bson diffent type [%d, %d]";
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* msg_bson_validate_only_support_compare_equal_type(void)
	{
		return "bson only support compare queal type [%d]";
	}
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_CODE_EX_HPP__
