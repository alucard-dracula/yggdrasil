// const_string_make_function_macro.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_CONST_STRING_MAKE_FUNCTION_MACRO_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_CONST_STRING_MAKE_FUNCTION_MACRO_HPP__

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/const_strings.hpp>
#include <yggr/range_ex/range_value_ex.hpp>

#define YGGR_TMP_PP_CONST_STRING_MAKE_FUNCTION(__name__, __key__) \
	inline static const utf8_string& __name__(void) { \
		static const ::yggr::utf8_string tmp(__key__, YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return tmp; } \
	\
	template<typename String> inline static \
	const String& __name__(void) { \
		typedef String now_string_type; \
		typedef typename ::yggr::range_ex::range_value_ex<now_string_type>::type string_val_type; \
		YGGR_PP_CONST_STRING_LOCAL_DEF(now_string, __key__) \
		static const now_string_type tmp(YGGR_PP_CONST_STRING_LOCAL_GET(now_string, string_val_type)); \
		return tmp; }

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{
} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_CONST_STRING_MAKE_FUNCTION_MACRO_HPP__
