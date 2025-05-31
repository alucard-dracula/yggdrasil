// mongoc_client_encryption_encrypt_opts_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/mongoc_client_encryption_encrypt_range_opts_native_ex.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_client_encryption_encrypt_opts_t {
//   bson_value_t keyid;
//   char *algorithm;
//   char *keyaltname;
//   struct {
//      int64_t value;
//      bool set;
//   } contention_factor;
//   char *query_type;
//   mongoc_client_encryption_encrypt_range_opts_t *range_opts;
//};

struct yggr_mongoc_client_encryption_encrypt_opts
{
	typedef struct 
	{
		int64_t value;
		bool set;
	} contention_factor_type;

	bson_value_t keyid;
	char *algorithm;
	char *keyaltname;
	contention_factor_type contention_factor;
	char *query_type;
	mongoc_client_encryption_encrypt_range_opts_t *range_opts;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class mongoc_client_encryption_encrypt_opts_native_ex
{
public:
	typedef mongoc_client_encryption_encrypt_opts_t org_client_encryption_encrypt_opts_type;
	typedef yggr_mongoc_client_encryption_encrypt_opts yggr_client_encryption_encrypt_range_type;

	typedef charset::utf8_string inner_string_type;
	typedef charset::utf8_string_view inner_string_view_type;

private:
	typedef mongoc_client_encryption_encrypt_opts_native_ex this_type;

public:
	inline static const inner_string_type& s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_random(void)
	{
		static const inner_string_type s_alog_name(MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM, 
													YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_alog_name;
	}

	inline static const inner_string_type& s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_deterministic(void)
	{
		static const inner_string_type s_alog_name(MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC, 
													YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_alog_name;
	}

public:
	inline static const inner_string_type& s_encrypt_algorithm_indexed(void)
	{
		static const inner_string_type s_alog_name(MONGOC_ENCRYPT_ALGORITHM_INDEXED, 
													YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_alog_name;
	}

	inline static const inner_string_type& s_encrypt_algorithm_unindexed(void)
	{
		static const inner_string_type s_alog_name(MONGOC_ENCRYPT_ALGORITHM_UNINDEXED, 
													YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_alog_name;
	}

#if YGGR_MONGOC_HEX() < 0x011C0000
	inline static const inner_string_type& s_encrypt_algorithm_range_preview(void)
	{
		static const inner_string_type s_alog_name(MONGOC_ENCRYPT_ALGORITHM_RANGEPREVIEW, 
													YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_alog_name;
	}

	inline static const inner_string_type& s_encrypt_algorithm_range(void)
	{
		return this_type::s_encrypt_algorithm_range_preview();
	}

#else
	inline static const inner_string_type& s_encrypt_algorithm_range(void)
	{
		static const inner_string_type s_alog_name(MONGOC_ENCRYPT_ALGORITHM_RANGE, 
													YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_alog_name;
	}

	inline static const inner_string_type& s_encrypt_algorithm_range_preview(void)
	{
		return this_type::s_encrypt_algorithm_range();
	}
#endif // #if YGGR_MONGOC_HEX() < 0x011C0000


public:
	inline static const inner_string_type& s_encrypt_query_mode_equality(void)
	{
		static const inner_string_type s_query_mode_name(MONGOC_ENCRYPT_QUERY_TYPE_EQUALITY, 
															YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_query_mode_name;
	}

#if YGGR_MONGOC_HEX() < 0x011C0000
	inline static const inner_string_type& s_encrypt_query_mode_range_preview(void)
	{
		static const inner_string_type s_query_mode_name(MONGOC_ENCRYPT_QUERY_TYPE_RANGEPREVIEW, 
															YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_query_mode_name;
	}

	inline static const inner_string_type& s_encrypt_query_mode_range(void)
	{
		return this_type::s_encrypt_query_mode_range_preview();
	}
#else
	inline static const inner_string_type& s_encrypt_query_mode_range(void)
	{
		static const inner_string_type s_query_mode_name(MONGOC_ENCRYPT_QUERY_TYPE_RANGE, 
															YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return s_query_mode_name;
	}

	inline static const inner_string_type& s_encrypt_query_mode_range_preview(void)
	{
		return this_type::s_encrypt_query_mode_range();
	}
#endif // #if YGGR_MONGOC_HEX() < 0x011C0000

public:
	static bool s_mongoc_client_encryption_encrypt_opts_init(void* popts);
	static void s_mongoc_client_encryption_encrypt_opts_clear(void* popts);
	static void s_mongoc_client_encryption_encrypt_opts_org_clear(void* popts);

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_CLIENT_ENCRYPTION_ENCRYPT_OPTS_NATIVE_EX_HPP__
