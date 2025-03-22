//mongoc_read_concern_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_READ_CONCERN_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_READ_CONCERN_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>
#include <yggr/nsql_database_system/mongodb_config.hpp>

namespace yggr
{
namespace nsql_database_system
{

//
//struct _mongoc_read_concern_t
//{
//   char   *level;
//   bool    frozen;
//   bson_t  compiled;
//};


struct yggr_mongoc_read_concern
{
	char* level;
	bool frozen;
	bson_t compiled;
};

class mongoc_read_concern_native_ex
{
private:
	typedef mongoc_read_concern_native_ex this_type;


public:
	YGGR_CONSTEXPR_OR_INLINE static const char* s_level_local(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return MONGOC_READ_CONCERN_LEVEL_LOCAL;
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* s_level_majority(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return MONGOC_READ_CONCERN_LEVEL_MAJORITY;
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* s_level_available(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return MONGOC_READ_CONCERN_LEVEL_AVAILABLE;
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* s_level_linearizable(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return MONGOC_READ_CONCERN_LEVEL_LINEARIZABLE;
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* s_level_snapshot(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return MONGOC_READ_CONCERN_LEVEL_SNAPSHOT;
	}

	YGGR_CONSTEXPR_OR_INLINE static bool s_level_check(const char* str_lv) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return 
			str_lv == s_level_local() ||  0 == strcmp(str_lv, s_level_local())
			|| str_lv == s_level_majority() || 0 == strcmp(str_lv, s_level_majority())
			|| str_lv == s_level_available() || 0 == strcmp(str_lv, s_level_available())
			|| str_lv == s_level_linearizable() || 0 == strcmp(str_lv, s_level_linearizable())
			|| str_lv == s_level_snapshot() || 0 == strcmp(str_lv, s_level_snapshot());
	}

	template<typename String> YGGR_CONSTEXPR_OR_INLINE
	static 
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String>,
			bool
		>::type 
		s_level_check(const String& str_lv) YGGR_NOEXCEPT_OR_NOTHROW
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_utf8_string_type);

		now_utf8_string_type utf8_str_lv((charset::string_charset_helper_data(str_lv)));

		return 
			utf8_str_lv == s_level_local()
			|| utf8_str_lv == s_level_majority()
			|| utf8_str_lv == s_level_available()
			|| utf8_str_lv == s_level_linearizable()
			|| utf8_str_lv == s_level_snapshot();
	}


public:
	static void* s_mongoc_read_concern_init(void* pmrc);
	static void* s_mongoc_read_concern_move(void* pdst, void* psrc);
	static void* s_mongoc_read_concern_copy(void* pdst, const void* psrc);
	static void* s_mongoc_read_concern_unfreeze_copy(void* pdst, const void* psrc);
	static void s_mongoc_read_concern_swap(void* pl, void* pr);
	static bool s_mongoc_read_concern_static_destroy(void* pmrc);
public:
	static const char* s_mongoc_read_concern_get_level(const void* pmrc);
	static bool s_mongoc_read_concern_set_level(void* pmrc, const char* level, std::size_t n = -1);

	static bool s_mongoc_read_concern_is_default(const void* pmrc);

	static bool s_mongoc_read_concern_append(void* pmrc, bson_t* command);

	static bool s_mongoc_read_concern_freeze (void* pmrc);
	
	inline static bool s_mongoc_read_concern_unfreeze(void* pmrc)
	{
		return this_type::prv_s_mongoc_read_concern_unfreeze(reinterpret_cast<yggr_mongoc_read_concern*>(pmrc));
	}

private:
	static bool prv_s_mongoc_read_concern_unfreeze(yggr_mongoc_read_concern* preal_mrc);
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_READ_CONCERN_NATIVE_EX_HPP__

