//bson_typeid_def.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPEID_DEF_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPEID_DEF_HPP__

#include <yggr/base/static_constant.hpp>

#include <yggr/type_traits/number_t.hpp>
#include <yggr/nsql_database_system/bson_config.hpp>

namespace yggr
{
namespace nsql_database_system
{

struct bson_typeid_def 
{
public:
	// !!! native type is u8 compatible with org enum type, so using u32 !!!
	typedef YGGR_PP_UINTEGER_T_TYPE(sizeof(bson_type_t)) bson_typeid_type;

public:
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_EOD = BSON_TYPE_EOD); // 0
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_DOUBLE = BSON_TYPE_DOUBLE); // 1
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_UTF8 = BSON_TYPE_UTF8); // 2 //bson_string
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_DOCUMENT = BSON_TYPE_DOCUMENT); // 3
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_ARRAY = BSON_TYPE_ARRAY); // 4
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_BINARY = BSON_TYPE_BINARY); // 5
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_UNDEFINED = BSON_TYPE_UNDEFINED); // 6
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_OID = BSON_TYPE_OID); // 7
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_BOOL = BSON_TYPE_BOOL); // 8
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_DATE_TIME = BSON_TYPE_DATE_TIME); // 9
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_NULL = BSON_TYPE_NULL); // 10
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_REGEX = BSON_TYPE_REGEX); // 11
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_DBPOINTER = BSON_TYPE_DBPOINTER); // 12 // bson_dbref
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_CODE = BSON_TYPE_CODE); // 13
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_SYMBOL = BSON_TYPE_SYMBOL); // 14
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_CODEWSCOPE = BSON_TYPE_CODEWSCOPE); // 15
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_INT32 = BSON_TYPE_INT32); // 16 // bson_int
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_TIMESTAMP = BSON_TYPE_TIMESTAMP); // 17
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_INT64 = BSON_TYPE_INT64); // 18 // bson_long
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_DECIMAL128 = BSON_TYPE_DECIMAL128); // 19 // bson_decimal128_t

	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_MAXKEY = BSON_TYPE_MAXKEY);

	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_YGGR_EX_BEG = 0x80 + 2); // 0x82 mark
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_SPLICE_OBJECT = E_BSON_TYPE_YGGR_EX_BEG + 1); // 0x83
	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_ANY_VALUE = E_BSON_TYPE_YGGR_EX_BEG + 2); // 0x84

	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_YGGR_EX_END = E_BSON_TYPE_YGGR_EX_BEG + 10); // 0x8c mark

	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_USER = E_BSON_TYPE_YGGR_EX_END + 2); // 0x8e mark

	// can append new type

	YGGR_STATIC_CONSTANT(bson_typeid_type, E_BSON_TYPE_MINKEY = BSON_TYPE_MINKEY);
};

struct bson_binary_typeid_def
{
public:
	// !!! native type is u8 compatible with org enum type, so using u32 !!!
	typedef YGGR_PP_UINTEGER_T_TYPE(sizeof(bson_subtype_t)) bson_subtypeid_type;

public:
	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_BINARY = BSON_SUBTYPE_BINARY);
	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_FUNCTION = BSON_SUBTYPE_FUNCTION);
	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_BINARY_DEPRECATED = BSON_SUBTYPE_BINARY_DEPRECATED); // old binary buffer
	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_UUID_DEPRECATED = BSON_SUBTYPE_UUID_DEPRECATED); // old uuid binary buffer
	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_UUID = BSON_SUBTYPE_UUID);
	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_MD5 = BSON_SUBTYPE_MD5);

	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_YGGR_EX_BEG = BSON_SUBTYPE_USER + 2); // mark

	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_U8_BUFFER = E_BSON_BIN_TYPE_YGGR_EX_BEG + 1);
	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_PACKET = E_BSON_BIN_TYPE_YGGR_EX_BEG + 2);

	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_YGGR_EX_END = E_BSON_BIN_TYPE_YGGR_EX_BEG + 10); // mark

	YGGR_STATIC_CONSTANT(bson_subtypeid_type, E_BSON_BIN_TYPE_USER = E_BSON_BIN_TYPE_YGGR_EX_END + 2); // mark

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPEID_DEF_HPP__
