// mongoc_uri_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_URI_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_URI_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/nsql_database_system/mongodb_config.hpp>

#include <boost/ref.hpp>

#include <cassert>
#include <sstream>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_uri_t {
//   char *str;
//   bool is_srv;
//   char srv[BSON_HOST_NAME_MAX + 1];
//   mongoc_host_list_t *hosts;
//   char *username;
//   char *password;
//   char *database;
//   bson_t raw;     /* Unparsed options, see mongoc_uri_parse_options */
//   bson_t options; /* Type-coerced and canonicalized options */
//   bson_t credentials;
//   bson_t compressors;
//   mongoc_read_prefs_t *read_prefs;
//   mongoc_read_concern_t *read_concern;
//   mongoc_write_concern_t *write_concern;
//};

struct yggr_mongoc_uri_t
{
   char						*str;
   bool						is_srv;
   char						srv[BSON_HOST_NAME_MAX + 1];
   mongoc_host_list_t		*hosts;
   char						*username;
   char						*password;
   char						*database;
   bson_t					raw;     /* Unparsed options, see mongoc_uri_parse_options */
   bson_t					options; /* Type-coerced and canonicalized options */
   bson_t					credentials;
   bson_t					compressors;
   mongoc_read_prefs_t		*read_prefs;
   mongoc_read_concern_t	*read_concern;
   mongoc_write_concern_t	*write_concern;
};

class mongoc_uri_native_ex
{
public:
	typedef mongoc_uri_t org_uri_type;
	typedef yggr_mongoc_uri_t yggr_uri_type;

	typedef utf8_string inner_string_type;

private:
	typedef mongoc_uri_native_ex this_type;

public:
	YGGR_CONSTEXPR_OR_INLINE static const char* s_mongoc_uri_scheme(void)
	{
		return "mongodb://";
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* s_mongoc_uri_scheme_srv_record(void)
	{
		return "mongodb+srv://";
	}
	YGGR_CONSTEXPR_OR_INLINE static const char* s_mongoc_default_uri(void)
	{
		return "mongodb://127.0.0.1/";
	}

	YGGR_CONSTEXPR_OR_INLINE static const char* s_mongoc_default_uri_srv_record(void)
	{
		return "mongodb+srv://127.0.0.1/";
	}

	inline static bool s_mongoc_uri_init(void* puri, bson_error_t* perr)
	{
		assert(puri);
		if(!puri)
		{
			return false;
		}

		memset(puri, 0, sizeof(yggr_uri_type));
		return this_type::s_prv_mongoc_uri_init(puri, 0, perr);
	}

	inline static bool s_mongoc_uri_init(void* puri, const char* host_name, u16 port, bson_error_t* perr)
	{
		assert((host_name && port));
		if(!(host_name && port))
		{
			return false;
		}
		
		if(!puri)
		{
			return false;
		}
         memset(puri, 0, sizeof(yggr_uri_type));
         
		std::stringstream ss;
		ss << this_type::s_mongoc_uri_scheme() << host_name << ":" << port << "/";
		return this_type::s_prv_mongoc_uri_init(puri, ss.str().data(), perr);
	}

	inline static bool s_mongoc_uri_init(void* puri, const char* uri_scheme, const char* host_name, u16 port, bson_error_t* perr)
	{
		assert((host_name && port));
		if(!(host_name && port))
		{
			return false;
		}
		
		if(!puri)
		{
			return false;
		}
		
		memset(puri, 0, sizeof(yggr_uri_type));

		if(!uri_scheme)
		{
			uri_scheme =  s_mongoc_uri_scheme();
		}

		std::stringstream ss;
		ss << uri_scheme << host_name << ":" << port << "/";
		return this_type::s_prv_mongoc_uri_init(puri, ss.str().data(), perr);
	}

	inline static bool s_mongoc_uri_init(void* puri, const char* uri_str, bson_error_t* perr)
	{
		assert(puri);
		if(!puri)
		{
			return false;
		}

		memset(puri, 0, sizeof(yggr_uri_type));

		return this_type::s_prv_mongoc_uri_init(puri, uri_str, perr);
	}

	inline static bool s_mongoc_uri_copy(void* pdst, const void* psrc)
	{
		return this_type::s_prv_mongoc_uri_copy(pdst, psrc);
	}

	inline static void s_mongoc_uri_swap(void* pl, void* pr)
	{
		this_type::s_prv_mongoc_uri_swap(pl, pr);
	}

	inline static void s_mongoc_uri_destroy(void* puri)
	{
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		mongoc_uri_destroy(porg_uri);
	}

	inline static void s_mongoc_uri_clear(void* puri)
	{
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		this_type::s_prv_mongoc_uri_clear(porg_uri);
	}

	inline static bool s_mongoc_uri_empty(const void* puri)
	{
		assert(puri);
		const yggr_uri_type* pyggr_uri = reinterpret_cast<const yggr_uri_type*>(puri);
		return !(pyggr_uri && pyggr_uri->str);
	}

	// mongoc_uri_get_hosts
	inline static const mongoc_host_list_t* s_mongoc_uri_get_hosts(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_hosts(porg_uri) : 0;
	}

	// mongoc_uri_get_srv_hostname
	inline static const char* s_mongoc_uri_get_srv_hostname(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_srv_hostname(porg_uri) : 0;
	}

	// mongoc_uri_get_srv_service_name
	inline static const char* s_mongoc_uri_get_srv_service_name(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_srv_service_name(porg_uri) : 0;
	}

	// get set database
	inline static const char* s_mongoc_uri_get_database(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_database(porg_uri) : 0;
	}

	inline static bool s_mongoc_uri_set_database(void* puri, const char* database)
	{
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return mongoc_uri_set_database(porg_uri, database);
	}

	// mongoc_uri_get_compressors
	inline static const bson_t* s_mongoc_uri_get_compressors(const void* puri)
	{
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return mongoc_uri_get_compressors(porg_uri);
	}

	// mongoc_uri_get_options
	inline static const bson_t* s_mongoc_uri_get_options(const void* puri)
	{
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return mongoc_uri_get_options(porg_uri);
	}

	// get set username
	inline static const char* s_mongoc_uri_get_username(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_username(porg_uri) : 0;
	}

	inline static bool s_mongoc_uri_set_username(void* puri, const char* username)
	{
		assert((puri && username));
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && username && mongoc_uri_set_username(porg_uri, username);
	}

	// get set password
	inline static const char* s_mongoc_uri_get_password(const void* puri)
	{
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return mongoc_uri_get_password(porg_uri);
	}

	inline static bool s_mongoc_uri_set_password(void* puri, const char* passwd)
	{
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return mongoc_uri_set_password(porg_uri, passwd);
	}

	// options operator
	inline static bool s_mongoc_uri_has_option(const void* puri, const char* key)
	{
		assert((puri && key));
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri && key && mongoc_uri_has_option(porg_uri, key);
	}

	// mongoc_uri_option_is_xxx
	inline static bool s_mongoc_uri_option_is_int32(const char* key)
	{
		assert(key);
		return key && mongoc_uri_option_is_int32(key);
	}

	inline static bool s_mongoc_uri_option_is_int64(const char* key)
	{
		assert(key);
		return key && mongoc_uri_option_is_int64(key);
	}

	inline static bool s_mongoc_uri_option_is_bool(const char* key)
	{
		assert(key);
		return key && mongoc_uri_option_is_bool(key);
	}

	inline static bool s_mongoc_uri_option_is_utf8(const char* key)
	{
		assert(key);
		return key && mongoc_uri_option_is_utf8(key);
	}

	// mongoc_uri_get_option_as_xxx

	inline static int32_t s_mongoc_uri_get_option_as_int32(const void* puri, const char* option, int32_t fallback)
	{
		assert(puri && option);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri && option? mongoc_uri_get_option_as_int32(porg_uri, option, fallback) : fallback;
	}

	inline static int64_t s_mongoc_uri_get_option_as_int64(const void* puri, const char* option, int64_t fallback)
	{
		assert(puri && option);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri && option? mongoc_uri_get_option_as_int64(porg_uri, option, fallback) : fallback;
	}

	inline static bool s_mongoc_uri_get_option_as_bool(const void* puri, const char* option, bool fallback)
	{
		assert(puri && option);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri && option? mongoc_uri_get_option_as_bool(porg_uri, option, fallback) : fallback;
	}

	inline static const char* s_mongoc_uri_get_option_as_utf8(const void* puri, const char* option, const char* fallback)
	{
		assert(puri && option);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri && option? mongoc_uri_get_option_as_utf8(porg_uri, option, fallback) : fallback;
	}

	// mongoc_uri_set_option_as_xxx
	inline static bool s_mongoc_uri_set_option_as_int32(void* puri, const char* option, int32_t value)
	{
		assert(puri && option);
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && option && mongoc_uri_set_option_as_int32(porg_uri, option, value);
	}

	inline static bool s_mongoc_uri_set_option_as_int64(void* puri, const char* option, int64_t value)
	{
		assert(puri && option);
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && option && mongoc_uri_set_option_as_int64(porg_uri, option, value);
	}

	inline static bool s_mongoc_uri_set_option_as_bool(void* puri, const char* option, bool value)
	{
		assert(puri && option);
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && option && mongoc_uri_set_option_as_bool(porg_uri, option, value);
	}

	inline static bool s_mongoc_uri_set_option_as_utf8(void* puri, const char* option, const char* value)
	{
		assert(puri && option);
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && option && mongoc_uri_set_option_as_utf8(porg_uri, option, value);
	}

	// mongoc_uri_get_replica_set
	inline static const char* s_mongoc_uri_get_replica_set(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_replica_set(porg_uri) : 0;
	}

	// mongoc_uri_get_string
	inline static const char* s_mongoc_uri_get_string(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_string(porg_uri) : 0;
	}

	// mongoc_uri_get_credentials
	inline static const bson_t* s_mongoc_uri_get_credentials(const void* puri)
	{
		assert((puri));
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_credentials(porg_uri) : 0;
	}

	// get set auth_source
	inline static const char* s_mongoc_uri_get_auth_source(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_auth_source(porg_uri) : 0;
	}

	inline static bool s_mongoc_uri_set_auth_source(void* puri, const char* value)
	{
		assert((puri && value));
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && value && mongoc_uri_set_auth_source(porg_uri, value);
	}

	// get set mongoc_uri_get_appname
	inline static const char* s_mongoc_uri_get_appname(void* puri)
	{
		assert((puri));
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_appname(porg_uri) : 0;
	}

	inline static bool s_mongoc_uri_set_appname(void* puri, const char* value)
	{
		assert((puri));
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && value && mongoc_uri_set_appname(porg_uri, value);
	}

	// mongoc_uri_set_appname
	inline static bool s_mongoc_uri_set_compressors(void* puri, const char* value)
	{
		assert((puri && value));
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && value && mongoc_uri_set_compressors(porg_uri, value);
	}

	// get set auth_mechanism
	inline static const char* s_mongoc_uri_get_auth_mechanism(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_auth_mechanism(porg_uri) : 0;
	}

	inline static bool s_mongoc_uri_set_auth_mechanism(void* puri, const char* value)
	{
		assert(puri && value);
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && value && mongoc_uri_set_auth_mechanism(porg_uri, value);
	}

	// get set mechanism_properties
	inline static bool s_mongoc_uri_get_mechanism_properties(const void* puri, bson_t* properties)
	{
		assert(puri && properties);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri && properties && mongoc_uri_get_mechanism_properties(porg_uri, properties);
	}

	inline static bool s_mongoc_uri_set_mechanism_properties(void* puri, const bson_t* properties)
	{
		assert(puri && properties);
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && properties && mongoc_uri_set_mechanism_properties(porg_uri, properties);
	}

	// s_mongoc_uri_get_tls older

	// mongoc_uri_get_tls
	inline static bool s_mongoc_uri_get_tls(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri && mongoc_uri_get_tls(porg_uri);
	}

	// mongoc_uri_unescape
	inline static inner_string_type s_mongoc_uri_unescape(const char* escaped_string)
	{
		assert(escaped_string);
		char* ptr = mongoc_uri_unescape(escaped_string);
		if(!ptr)
		{
			return inner_string_type();
		}
		else
		{
			inner_string_type ret(ptr, YGGR_STR_UTF8_STRING_CHARSET_NAME());

			bson_free(ptr);
			return ret;
		}
	}

	inline static const bson_t* s_mongoc_uri_get_read_prefs(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_read_prefs(porg_uri) : static_cast<bson_t*>(0);
	}

	inline static const mongoc_read_prefs_t* s_mongoc_uri_get_read_prefs_t(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_read_prefs_t(porg_uri) : 0;
	}

	inline static bool s_mongoc_uri_set_read_prefs_t(void* puri, const mongoc_read_prefs_t* prefs)
	{
		assert(puri);
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);

		return porg_uri && (mongoc_uri_set_read_prefs_t(porg_uri, prefs), true);
	}

	// get set write_concern
	inline static const mongoc_write_concern_t* s_mongoc_uri_get_write_concern(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return mongoc_uri_get_write_concern(porg_uri);
	}

	inline static bool s_mongoc_uri_set_write_concern(void* puri, const mongoc_write_concern_t* wc)
	{
		assert(puri);
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && (mongoc_uri_set_write_concern(porg_uri, wc), true);
	}

	// mongoc_uri_get_read_concern
	inline static const mongoc_read_concern_t* s_mongoc_uri_get_read_concern(const void* puri)
	{
		assert(puri);
		const org_uri_type* porg_uri = reinterpret_cast<const org_uri_type*>(puri);
		return porg_uri? mongoc_uri_get_read_concern(porg_uri) : 0;
	}

	inline static bool s_mongoc_uri_set_read_concern(void* puri, const mongoc_read_concern_t* rc)
	{
		assert(puri);
		org_uri_type* porg_uri = reinterpret_cast<org_uri_type*>(puri);
		return porg_uri && (mongoc_uri_set_read_concern(porg_uri, rc), true);
	}

private:
	YGGR_CONSTEXPR_OR_INLINE static const char* s_prv_escape_instructions(void)
	{
		return "Percent-encode username and password according to RFC 3986";
	}

private:
	// copy from mongodb_c_driver and mend it
	inline static void s_prv_mongoc_uri_do_unescape(char** str)
	{
		char* tmp = 0;

		if((tmp = *str))
		{
			*str = mongoc_uri_unescape(tmp);
			bson_free(tmp);
		}
	}

	static char* s_prv_scan_to_unichar(const char* str, bson_unichar_t match,
										const char* terminators, const char** end);

	static bool s_prv_mongoc_uri_parse_host(org_uri_type* uri, const char* host_and_port_in);
	static bool s_prv_mongoc_uri_parse_hosts(org_uri_type* uri, const char* hosts);
	static bool s_prv_mongoc_uri_parse_database(org_uri_type* uri, const char* str, const char** end);
	static bool s_prv_mongoc_uri_parse_scheme(org_uri_type* uri, const char* str, const char** end);

	static bool s_prv_mongoc_uri_has_unescaped_chars(const char* str, const char* chars);
	static bool s_prv_mongoc_uri_parse_userpass(org_uri_type* uri, const char* str, bson_error_t* error);

	static std::size_t s_prv_count_dots(const char *s);
	static bool s_prv_mongoc_uri_parse_before_slash(org_uri_type *uri, const char *before_slash, bson_error_t* error);
	inline static bool s_prv_valid_hostname(const char* s)
	{
		size_t len = strlen(s);
		return len > 1 && s[0] != '.';
	}

	static bool s_prv_mongoc_uri_parse_srv(org_uri_type* uri, const char* str, bson_error_t* error);
	static bool s_prv_mongoc_uri_parse(org_uri_type* uri, const char* str, bson_error_t* error);
	static bool s_prv_mongoc_uri_assign_read_prefs_mode(org_uri_type* uri, bson_error_t *error);
	static bool s_prv_mongoc_uri_build_write_concern(org_uri_type* uri, bson_error_t* error);

	static bool s_prv_mongoc_uri_init(void* puri, const char* uri_string, bson_error_t* error);

	static bool s_prv_mongoc_uri_copy(void* pdst, const void* psrc);
	static void s_prv_mongoc_uri_swap(void* pl, void* pr);
	static void s_prv_mongoc_uri_clear(org_uri_type* uri);
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGOC_URI_NATIVE_EX_HPP__
