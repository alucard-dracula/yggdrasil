//mongoc_uri_native_ex.cpp

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

#include <yggr/nsql_database_system/mongoc_uri_native_ex.hpp>

#include <yggr/nsql_database_system/mongoc_read_prefs_native_ex.hpp>
#include <yggr/nsql_database_system/mongoc_write_concern_native_ex.hpp>
#include <yggr/nsql_database_system/mongoc_read_concern_native_ex.hpp>
#include <yggr/nsql_database_system/mongoc_host_list_native_ex.hpp>

#include <yggr/nsql_database_system/bson_string_native_ex.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>

#include <memory>

#define MONGOC_COMPILATION

#	include <mongoc-uri-private.h>
#	include <mongoc-read-prefs.h>

#undef MONGOC_COMPILATION

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#ifdef _WIN32
# define strcasecmp  _stricmp
# define strncasecmp _strnicmp
#endif // _WIN32


#define YGGR_MONGOC_URI_ERROR(error, format, ...)     \
	bson_set_error (error,                            \
					MONGOC_ERROR_COMMAND,             \
					MONGOC_ERROR_COMMAND_INVALID_ARG, \
					format,                           \
					__VA_ARGS__);

namespace yggr
{
namespace nsql_database_system
{

//private:
// copy from mongodb_c_driver and mend it

/*static*/
char* mongoc_uri_native_ex::s_prv_scan_to_unichar(const char* str, bson_unichar_t match,
													const char* terminators, const char** end)
{
	bson_unichar_t c = 0;
	const char* iter = 0;

	for(iter = str;
			iter && *iter &&(c = bson_utf8_get_char(iter));
			iter = bson_utf8_next_char(iter))
	{
		if(c == match)
		{
			*end = iter;
			return bson_strndup(str, iter - str);
		}
		else
		{
			if(c == '\\')
			{
				iter = bson_utf8_next_char(iter);
				if(!bson_utf8_get_char(iter))
				{
					break;
				}
			}
			else
			{
				const char* term_iter;
				for(term_iter = terminators; *term_iter; term_iter++)
				{
					if(c == *term_iter)
					{
						return 0;
					}
				}
			}
		}
	}

	return 0;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_parse_hosts(org_uri_type* uri, const char* hosts)
{
	assert((uri && hosts));
	if(!(uri && hosts))
	{
		return false;
	}

	const char* next = 0;
	const char* end_hostport = 0;
	char* s = 0;
	BSON_ASSERT (hosts);
	/*
	* Parsing the series of hosts is a lot more complicated than you might
	* imagine. This is due to some characters being both separators as well as
	* valid characters within the "hostname". In particularly, we can have file
	* paths to specify paths to UNIX domain sockets. We impose the restriction
	* that they must be suffixed with ".sock" to simplify the parsing.
	*
	* You can separate hosts and file system paths to UNIX domain sockets with
	* ",".
	*/
	s = this_type::s_prv_scan_to_unichar(hosts, '?', "", &end_hostport);
	if(s)
	{
		MONGOC_WARNING("%s", "A '/' is required between the host list and any options.");
		goto error_fix;
	}

	next = hosts;

	do
	{
		/* makes a copy of the section of the string */
		s = this_type::s_prv_scan_to_unichar(next, ',', "", &end_hostport);
		if(s)
		{
			next = end_hostport + 1;
		}
		else
		{
			s = bson_string_native_ex::s_bson_strdup(next);
			next = 0;
		}

		if(!mongoc_uri_parse_host(uri, s))
		{
			goto error_fix;
		}
		bson_free(s);
	} while(next);

	return true;

error_fix:
	bson_free(s);
	return false;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_parse_database(org_uri_type* uri, const char* str, const char** end)
{
	assert((uri && str && end));
	if(!(uri && str && end))
	{
		return false;
	}

	const char* end_database = 0;
	//const char* c = 0;
	char* invalid_c = 0;
	const char* tmp = 0;

	yggr_uri_type* pyggr_uri = reinterpret_cast<yggr_uri_type*>(uri);
	if((pyggr_uri->database = this_type::s_prv_scan_to_unichar(str, '?', "", &end_database)))
	{
		if(strcmp(pyggr_uri->database, "") == 0)
		{
			/* no database is found, don't store the empty string. */
			bson_free(pyggr_uri->database);
			pyggr_uri->database = 0;
			/* but it is valid to have an empty database. */
			return true;
		}
		*end = end_database;
	}
	else if(*str)
	{
		pyggr_uri->database = bson_string_native_ex::s_bson_strdup(str);
		*end = str + strlen(str);
	}

	this_type::s_prv_mongoc_uri_do_unescape(&pyggr_uri->database);
	if(!pyggr_uri->database)
	{
		/* invalid */
		return false;
	}

	/* invalid characters in database name */
	for(const char* c = "/\\. \"$"; *c; c++)
	{
		invalid_c = this_type::s_prv_scan_to_unichar(pyggr_uri->database, static_cast<bson_unichar_t>(*c), "", &tmp);
		if(invalid_c)
		{
			bson_free(invalid_c);
			return false;
		}
	}

	return true;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_parse_scheme(org_uri_type* uri, const char* str, const char** end)
{
	assert((uri && str && end));
	if(!(uri && str && end))
	{
		return false;
	}

	yggr_uri_type* pyggr_uri = reinterpret_cast<yggr_uri_type*>(uri);

	if(!strncmp(str, this_type::s_mongoc_uri_scheme_srv_record(), 14))
	{
		pyggr_uri->is_srv = true;
		*end = str + 14;
		return true;
	}

	if(!strncmp(str, this_type::s_mongoc_uri_scheme(), 10))
	{
		pyggr_uri->is_srv = false;
		*end = str + 10;
		return true;
	}

	return false;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_has_unescaped_chars(const char* str, const char* chars)
{
	const char* tmp = 0;
	char* s = 0;

	for(const char* c = chars; *c; ++c)
	{
		s = this_type::s_prv_scan_to_unichar(str, static_cast<bson_unichar_t>(*c), "", &tmp);
		if(s)
		{
			bson_free(s);
			return true;
		}
	}

	return false;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_parse_userpass(org_uri_type* uri,
															const char* str,
															bson_error_t* error)
{
	assert((uri && str));
	if(!(uri && str))
	{
		return false;
	}

	const char* prohibited = "@:/";
	const char* end_user = 0;

	BSON_ASSERT (str);
	BSON_ASSERT (uri);

	yggr_uri_type* pyggr_uri = reinterpret_cast<yggr_uri_type*>(uri);
	assert(pyggr_uri);

	if((pyggr_uri->username = this_type::s_prv_scan_to_unichar(str, ':', "", &end_user)))
	{
		pyggr_uri->password = bson_string_native_ex::s_bson_strdup(end_user + 1);
	}
	else
	{
		pyggr_uri->username = bson_string_native_ex::s_bson_strdup(str);
		pyggr_uri->password = 0;
	}

	if(this_type::s_prv_mongoc_uri_has_unescaped_chars(pyggr_uri->username, prohibited))
	{
		YGGR_MONGOC_URI_ERROR(
			error,
			"Username \"%s\" must not have unescaped chars. %s",
			pyggr_uri->username,
			this_type::s_prv_escape_instructions());
		return false;
	}

	this_type::s_prv_mongoc_uri_do_unescape(&pyggr_uri->username);
	if(!pyggr_uri->username)
	{
		YGGR_MONGOC_URI_ERROR(
			error,
			"Incorrect URI escapes in username. %s",
			this_type::s_prv_escape_instructions());
		return false;
	}

	/* Providing password at all is optional */
	if(pyggr_uri->password)
	{
		if(this_type::s_prv_mongoc_uri_has_unescaped_chars(pyggr_uri->password, prohibited))
		{
			YGGR_MONGOC_URI_ERROR(
				error,
				"Password \"%s\" must not have unescaped chars. %s",
				pyggr_uri->password,
				this_type::s_prv_escape_instructions());
			return false;
		}

		this_type::s_prv_mongoc_uri_do_unescape(&pyggr_uri->password);
		if(!pyggr_uri->password)
		{
			YGGR_MONGOC_URI_ERROR(error, "%s", "Incorrect URI escapes in password");
			return false;
		}
	}

	return true;
}

/*static*/
std::size_t mongoc_uri_native_ex::s_prv_count_dots(const char* s)
{
   std::size_t n = 0;
   const char *dot = s;

   while((dot = strchr(dot + 1, '.')))
   {
      ++n;
   }

   return n;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_parse_srv(org_uri_type* uri, const char* str, bson_error_t* error)
{
	assert((uri && str));
	if(!(uri && str))
	{
		return false;
	}

	if(*str == '\0')
	{
		YGGR_MONGOC_URI_ERROR(error, "%s", "Missing service name in SRV URI");
		return false;
	}

	yggr_uri_type* pyggr_uri = reinterpret_cast<yggr_uri_type*>(uri);

	{
		char* service = bson_string_native_ex::s_bson_strdup(str);

		this_type::s_prv_mongoc_uri_do_unescape(&service);

		if(!service
			|| !this_type::s_prv_valid_hostname(service)
			|| this_type::s_prv_count_dots(service) < 2)
		{
			YGGR_MONGOC_URI_ERROR(error, "%s", "Invalid service name in URI");
			bson_free(service);
			return false;
		}

		bson_strncpy(pyggr_uri->srv, service, sizeof(pyggr_uri->srv));

		bson_free(service);
	}

	if(strchr(pyggr_uri->srv, ','))
	{
		YGGR_MONGOC_URI_ERROR(error, "%s", "Multiple service names are prohibited in an SRV URI");
		return false;
	}

	if(strchr(pyggr_uri->srv, ':'))
	{
		YGGR_MONGOC_URI_ERROR(error, "%s", "Port numbers are prohibited in an SRV URI");
		return false;
	}

	return true;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_parse_before_slash(mongoc_uri_t* uri,
																const char* before_slash,
																bson_error_t* error)
{
	assert((uri && before_slash));
	if(!(uri && before_slash))
	{
		return false;
	}

	char* userpass = 0;
	const char* hosts = 0;
	yggr_uri_type* pyggr_uri = 0;

	userpass = this_type::s_prv_scan_to_unichar(before_slash, '@', "", &hosts);
	if(userpass)
	{
		if(!this_type::s_prv_mongoc_uri_parse_userpass(uri, userpass, error))
		{
			goto error_fix;
		}

		++hosts; /* advance past "@" */
		if(*hosts == '@')
		{
			/* special case: "mongodb://alice@@localhost" */
			YGGR_MONGOC_URI_ERROR(
				error,
				"Invalid username or password. %s",
				this_type::s_prv_escape_instructions());
			goto error_fix;
		}
	}
	else
	{
		hosts = before_slash;
	}

	pyggr_uri = reinterpret_cast<yggr_uri_type*>(uri);

	if(pyggr_uri->is_srv)
	{
		if(!this_type::s_prv_mongoc_uri_parse_srv(uri, hosts, error))
		{
			goto error_fix;
		}
	}
	else
	{
		if(!this_type::s_prv_mongoc_uri_parse_hosts(uri, hosts))
		{
			YGGR_MONGOC_URI_ERROR(error, "%s", "Invalid host string in URI");
			goto error_fix;
		}
	}

	bson_free(userpass);
	return true;

error_fix:
	bson_free(userpass);
	return false;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_parse(org_uri_type* uri, const char* str, bson_error_t* error)
{
	assert((uri && str));
	if(!(uri && str))
	{
		return false;
	}

	char* before_slash = 0;
	const char* tmp = 0;

	if(!bson_utf8_validate(str, strlen (str), false /* allow_null */))
	{
		YGGR_MONGOC_URI_ERROR (error, "%s", "Invalid UTF-8 in URI");
		goto error_fix;
	}

	if(!this_type::s_prv_mongoc_uri_parse_scheme(uri, str, &str))
	{
		YGGR_MONGOC_URI_ERROR (
			error,
			"%s",
			"Invalid URI Schema, expecting 'mongodb://' or 'mongodb+srv://'");
		goto error_fix;
	}

	before_slash = this_type::s_prv_scan_to_unichar(str, '/', "", &tmp);
	if(!before_slash)
	{
		before_slash = bson_string_native_ex::s_bson_strdup(str);
		str += strlen(before_slash);
	}
	else
	{
		str = tmp;
	}

	if(!this_type::s_prv_mongoc_uri_parse_before_slash(uri, before_slash, error))
	{
		goto error_fix;
	}

	if(*str)
	{
		if(*str == '/')
		{
			++str;
			if(*str)
			{
				if(!this_type::s_prv_mongoc_uri_parse_database(uri, str, &str))
				{
					YGGR_MONGOC_URI_ERROR(error, "%s", "Invalid database name in URI");
					goto error_fix;
				}
			}

			if(*str == '?')
			{
				++str;
				if(*str)
				{
					if(!mongoc_uri_parse_options(uri, str, false /* from DNS */, error))
					{
						goto error_fix;
					}
				}
			}
		}
		else
		{
			YGGR_MONGOC_URI_ERROR (error, "%s", "Expected end of hostname delimiter");
			goto error_fix;
		}
	}

	if(!mongoc_uri_finalize (uri, error))
	{
		goto error_fix;
	}

	bson_free(before_slash);
	return true;

error_fix:
	bson_free(before_slash);
	return false;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_assign_read_prefs_mode(org_uri_type* uri, bson_error_t* error)
{
	assert(uri);
	if(!uri)
	{
		return false;
	}

	const char* str = 0;
	bson_iter_t iter = {0};

	BSON_ASSERT (uri);

	yggr_uri_type* pyggr_uri = reinterpret_cast<yggr_uri_type*>(uri);

	if(bson_iter_init_find_case(
		&iter,
		&pyggr_uri->options,
		MONGOC_URI_READPREFERENCE) && BSON_ITER_HOLDS_UTF8(&iter))
	{
		str = bson_iter_utf8(&iter, NULL);

		if(0 == strcasecmp("primary", str))
		{
			mongoc_read_prefs_set_mode(pyggr_uri->read_prefs, MONGOC_READ_PRIMARY);
		}
		else if(0 == strcasecmp("primarypreferred", str))
		{
			mongoc_read_prefs_set_mode(pyggr_uri->read_prefs, MONGOC_READ_PRIMARY_PREFERRED);
		}
		else if(0 == strcasecmp("secondary", str))
		{
			mongoc_read_prefs_set_mode(pyggr_uri->read_prefs, MONGOC_READ_SECONDARY);
		}
		else if(0 == strcasecmp("secondarypreferred", str))
		{
			mongoc_read_prefs_set_mode(pyggr_uri->read_prefs, MONGOC_READ_SECONDARY_PREFERRED);
		}
		else if(0 == strcasecmp("nearest", str))
		{
			mongoc_read_prefs_set_mode(pyggr_uri->read_prefs, MONGOC_READ_NEAREST);
		}
		else
		{
			YGGR_MONGOC_URI_ERROR(error, "Unsupported readPreference value [readPreference=%s]", str);
			return false;
		}
	}
	return true;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_build_write_concern(org_uri_type* uri, bson_error_t* error)
{
	assert(uri);
	if(!uri)
	{
		return false;
	}

	mongoc_write_concern_t* write_concern = 0;
	const char* str;
	bson_iter_t iter = {0};
	int64_t wtimeoutms = 0;
	int value = 0;

	BSON_ASSERT(uri);
	yggr_uri_type* pyggr_uri = reinterpret_cast<yggr_uri_type*>(uri);

	write_concern = mongoc_write_concern_new();
	pyggr_uri->write_concern = write_concern;

	if(bson_iter_init_find_case(&iter, &pyggr_uri->options, MONGOC_URI_SAFE) &&
		BSON_ITER_HOLDS_BOOL(&iter))
	{
		mongoc_write_concern_set_w (
			write_concern,
			bson_iter_bool(&iter)? 1 : MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED);
	}

	wtimeoutms = mongoc_uri_get_option_as_int64 (uri, MONGOC_URI_WTIMEOUTMS, 0);
	if(wtimeoutms < 0)
	{
		YGGR_MONGOC_URI_ERROR(error, "Unsupported wtimeoutMS value [w=%" PRId64 "]", wtimeoutms);
		return false;
	}
	else if(wtimeoutms > 0)
	{
		mongoc_write_concern_set_wtimeout_int64(write_concern, wtimeoutms);
	}

	if(bson_iter_init_find_case(&iter, &pyggr_uri->options, MONGOC_URI_JOURNAL) &&
		BSON_ITER_HOLDS_BOOL(&iter))
	{
		mongoc_write_concern_set_journal(write_concern, bson_iter_bool (&iter));
	}

	if(bson_iter_init_find_case(&iter, &pyggr_uri->options, MONGOC_URI_W))
	{
		if(BSON_ITER_HOLDS_INT32(&iter))
		{
			value = bson_iter_int32(&iter);

			switch (value)
			{
			case MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED:
			case MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED:
				if(mongoc_write_concern_get_journal(write_concern))
				{
					YGGR_MONGOC_URI_ERROR(error, "Journal conflicts with w value [w=%d]", value);
					return false;
				}
				mongoc_write_concern_set_w(write_concern, value);
				break;
			default:
				if(value > 0)
				{
					mongoc_write_concern_set_w(write_concern, value);
					break;
				}
				YGGR_MONGOC_URI_ERROR(error, "Unsupported w value [w=%d]", value);
				return false;
			}
		}
		else if(BSON_ITER_HOLDS_UTF8(&iter))
		{
			str = bson_iter_utf8(&iter, NULL);

			if(0 == strcasecmp ("majority", str))
			{
				mongoc_write_concern_set_w(
					write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);
			}
			else
			{
				mongoc_write_concern_set_wtag(write_concern, str);
			}
		}
		else
		{
			BSON_ASSERT (false);
			return false;
		}
	}

	return true;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_init(void* puri, const char* uri_str, bson_error_t* error)
{
	assert(puri);
	if(!puri)
	{
		return false;
	}

	org_uri_type* uri = reinterpret_cast<org_uri_type*>(puri);
	yggr_uri_type* pyggr_uri = reinterpret_cast<yggr_uri_type*>(uri);

#ifdef _DEBUG
	{
		yggr_uri_type chk = {0};
		assert(( 0 == memcmp(uri, &chk, sizeof(yggr_uri_type))));
	}
#endif // _DEBUG

	bson_init(boost::addressof(pyggr_uri->raw));
	bson_init(boost::addressof(pyggr_uri->options));
	bson_init(boost::addressof(pyggr_uri->credentials));
	bson_init(boost::addressof(pyggr_uri->compressors));

	/* Initialize read_prefs since tag parsing may add to it */
	pyggr_uri->read_prefs = mongoc_read_prefs_new(MONGOC_READ_PRIMARY);

	/* Initialize empty read_concern */
	pyggr_uri->read_concern = mongoc_read_concern_new();

	if(!uri_str)
	{
		//uri_str = "mongodb://127.0.0.1/";
		uri_str = this_type::s_mongoc_default_uri();
	}

	if(!this_type::s_prv_mongoc_uri_parse(uri, uri_str, error))
	{
		this_type::s_prv_mongoc_uri_clear(uri);
		return false;
	}

	pyggr_uri->str = bson_string_native_ex::s_bson_strdup(uri_str);

	this_type::s_prv_mongoc_uri_assign_read_prefs_mode(uri, error);

	if(!mongoc_read_prefs_is_valid(pyggr_uri->read_prefs))
	{
		YGGR_MONGOC_URI_ERROR(error, "%s", "Invalid readPreferences");
		this_type::s_prv_mongoc_uri_clear(uri);
		return false;
	}

	this_type::s_prv_mongoc_uri_build_write_concern(uri, error);

	if(!mongoc_write_concern_native_ex
			::s_mongoc_write_concern_is_valid(pyggr_uri->write_concern))
	{
		YGGR_MONGOC_URI_ERROR (error, "%s", "Invalid writeConcern");
		this_type::s_prv_mongoc_uri_clear(uri);
		return false;
	}

	return true;
}

/*static*/
bool mongoc_uri_native_ex::s_prv_mongoc_uri_copy(void* pdst, const void* psrc)
{
	assert((pdst && psrc));

	if(!(pdst && psrc))
	{
		return false;
	}

	//const org_uri_type* porg_src = reinterpret_cast<const org_uri_type*>(psrc);
	const yggr_uri_type* pyggr_src = reinterpret_cast<const yggr_uri_type*>(psrc);

	org_uri_type* porg_dst = reinterpret_cast<org_uri_type*>(pdst);
	yggr_uri_type* pyggr_dst = reinterpret_cast<yggr_uri_type*>(pdst);

	this_type::s_prv_mongoc_uri_clear(porg_dst);

	//mongoc_host_list_t* iter = 0;

	pyggr_dst->str      = bson_string_native_ex::s_bson_strdup(pyggr_src->str);
	pyggr_dst->is_srv	= pyggr_src->is_srv;

	memcpy(&(pyggr_dst->srv), &(pyggr_src->is_srv), sizeof(pyggr_src->is_srv));

	//for(iter = pyggr_src->hosts; iter; iter = iter->next)
	//{
	//	s_prv_mongoc_uri_append_host(porg_dst, iter->host, iter->port);
	//}
	pyggr_dst->hosts = mongoc_host_list_native_ex::s_mongoc_host_list_copy_all(pyggr_dst->hosts);

	pyggr_dst->username = bson_string_native_ex::s_bson_strdup(pyggr_src->username);
	pyggr_dst->password = bson_string_native_ex::s_bson_strdup(pyggr_src->password);
	pyggr_dst->database = bson_string_native_ex::s_bson_strdup(pyggr_src->database);

	bson_native_ex::s_bson_copy(&(pyggr_dst->raw), &(pyggr_src->raw));
	bson_native_ex::s_bson_copy(&(pyggr_dst->options), &(pyggr_src->options));
	bson_native_ex::s_bson_copy(&(pyggr_dst->credentials), &(pyggr_src->credentials));
	bson_native_ex::s_bson_copy(&(pyggr_dst->compressors), &(pyggr_src->compressors));

	pyggr_dst->read_prefs    = mongoc_read_prefs_copy(pyggr_src->read_prefs);
	pyggr_dst->read_concern  = mongoc_read_concern_copy(pyggr_src->read_concern);
	pyggr_dst->write_concern = mongoc_write_concern_copy(pyggr_src->write_concern);

	return true;
}

/*static*/
void mongoc_uri_native_ex::s_prv_mongoc_uri_swap(void* pl, void* pr)
{
	assert((pl && pr));
	if(!(pl && pr))
	{
		return;
	}

	if(pl == pr)
	{
		return;
	}

	yggr_uri_type tmp = {0};
	memcpy(boost::addressof(tmp), pl, sizeof(yggr_uri_type));
	memcpy(pl, pr, sizeof(yggr_uri_type));
	memcpy(pr, boost::addressof(tmp), sizeof(yggr_uri_type));
}

/*static*/
void mongoc_uri_native_ex::s_prv_mongoc_uri_clear(org_uri_type* uri)
{
	yggr_uri_type* pyggr_uri = reinterpret_cast<yggr_uri_type*>(uri);
	if(pyggr_uri)
	{
		if(pyggr_uri->str) bson_free(pyggr_uri->str);
		// is_srv
		// srv
		//s_prv_mongoc_host_list_destroy_all(pyggr_uri->hosts);
		mongoc_host_list_native_ex::s_mongoc_host_list_destroy_all(pyggr_uri->hosts);

		if(pyggr_uri->username) bson_free(pyggr_uri->username);
		if(pyggr_uri->password) bson_zero_free(pyggr_uri->password, strlen(pyggr_uri->password));
		if(pyggr_uri->database) bson_free(pyggr_uri->database);

		if(pyggr_uri->raw.len) bson_destroy(&(pyggr_uri->raw));
		if(pyggr_uri->options.len) bson_destroy(&(pyggr_uri->options));
		if(pyggr_uri->credentials.len) bson_destroy(&(pyggr_uri->credentials));
		if(pyggr_uri->compressors.len) bson_destroy(&(pyggr_uri->compressors));

		if(pyggr_uri->read_prefs) mongoc_read_prefs_destroy(pyggr_uri->read_prefs);
		if(pyggr_uri->read_concern) mongoc_read_concern_destroy(pyggr_uri->read_concern);
		if(pyggr_uri->write_concern) mongoc_write_concern_destroy(pyggr_uri->write_concern);

		memset(pyggr_uri, 0, sizeof(yggr_uri_type));
	}
}

} // namespace nsql_database_system
} // namespace yggr
