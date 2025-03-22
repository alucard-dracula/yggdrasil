//basic_bson_inner_data.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BASIC_BSON_INNER_DATA_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BASIC_BSON_INNER_DATA_HPP__

#include <yggr/charset/utf8_char.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/nsql_database_system/bson_typeid.hpp>

YGGR_PP_BSON_TYPEID_IMPL(bool, bson_typeid_def::E_BSON_TYPE_BOOL)

YGGR_PP_BSON_TYPEID_IMPL(s8, bson_typeid_def::E_BSON_TYPE_INT32)
YGGR_PP_BSON_TYPEID_IMPL(char, bson_typeid_def::E_BSON_TYPE_INT32)

#if !defined(YGGR_NO_CWCHAR)
#	if !((defined _MSC_VER) && (_MSC_VER < 1300))
YGGR_PP_BSON_TYPEID_IMPL(wchar_t, bson_typeid_def::E_BSON_TYPE_INT32)
#	endif // (defined _MSC_VER) && (_MSC_VER < 1300)
#endif //YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)
YGGR_PP_BSON_TYPEID_IMPL(c8, bson_typeid_def::E_BSON_TYPE_INT32)
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
YGGR_PP_BSON_TYPEID_IMPL(c16, bson_typeid_def::E_BSON_TYPE_INT32)
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
YGGR_PP_BSON_TYPEID_IMPL(c32, bson_typeid_def::E_BSON_TYPE_INT32)
#endif // YGGR_NO_CHAR32_T

YGGR_PP_BSON_TYPEID_IMPL(u8, bson_typeid_def::E_BSON_TYPE_INT32)
YGGR_PP_BSON_TYPEID_IMPL(s16, bson_typeid_def::E_BSON_TYPE_INT32)
YGGR_PP_BSON_TYPEID_IMPL(u16, bson_typeid_def::E_BSON_TYPE_INT32)
YGGR_PP_BSON_TYPEID_IMPL(s32, bson_typeid_def::E_BSON_TYPE_INT32)
YGGR_PP_BSON_TYPEID_IMPL(u32, bson_typeid_def::E_BSON_TYPE_INT32)

//YGGR_PP_BSON_TYPEID_IMPL(sint, bson_typeid_def::E_BSON_TYPE_INT32)
//YGGR_PP_BSON_TYPEID_IMPL(uint, bson_typeid_def::E_BSON_TYPE_INT32)

YGGR_PP_BSON_TYPEID_IMPL(s64, bson_typeid_def::E_BSON_TYPE_INT64)
YGGR_PP_BSON_TYPEID_IMPL(u64, bson_typeid_def::E_BSON_TYPE_INT64)

YGGR_PP_BSON_TYPEID_IMPL(f32, bson_typeid_def::E_BSON_TYPE_DOUBLE)
YGGR_PP_BSON_TYPEID_IMPL(f64, bson_typeid_def::E_BSON_TYPE_DOUBLE)

// strings
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(3, std::basic_string, bson_typeid_def::E_BSON_TYPE_UTF8)
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(3, boost::container::basic_string, bson_typeid_def::E_BSON_TYPE_UTF8)
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(3, charset::utf8_string_impl, bson_typeid_def::E_BSON_TYPE_UTF8)

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(2, charset::string_charset_helper, bson_typeid_def::E_BSON_TYPE_UTF8)

// utf8_char
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(3, charset::utf8_char_impl, bson_typeid_def::E_BSON_TYPE_UTF8)

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BASIC_BSON_INNER_DATA_HPP__
