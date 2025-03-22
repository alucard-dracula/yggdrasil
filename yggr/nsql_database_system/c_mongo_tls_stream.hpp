//c_mongo_tls_stream.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_TLS_STREAM_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_TLS_STREAM_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/nsql_database_system/c_mongo_basic_stream.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace yggr
{
namespace nsql_database_system
{

// mongodb_c_driver some initialization method does not provide any public
class c_mongo_tls_stream
	: public c_mongo_basic_stream
{
public:
	typedef c_mongo_basic_stream base_type;
	typedef base_type::org_type org_type;
	typedef base_type::org_type base_org_type;
	typedef base_type::stream_ptr_type stream_ptr_type;

protected:
	typedef bson_t org_bson_type;
	typedef bson_error_t org_bson_error_type;

private:
	typedef c_mongo_tls_stream this_type;
	BOOST_MOVABLE_BUT_NOT_COPYABLE(c_mongo_tls_stream) // not using this_type cpp20 has issue

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	c_mongo_tls_stream(void);

	c_mongo_tls_stream(base_org_type* pstream);
	c_mongo_tls_stream(base_org_type* pstream, bool need_del);

	c_mongo_tls_stream(base_org_type* porg_base_stream, const char* str_host,
						mongoc_ssl_opt_t* pssl_opt, int client);

	template<typename String, typename MongocSSLOpt>
	c_mongo_tls_stream(base_org_type* porg_base_stream, const String& str_host,
						const MongocSSLOpt& ssl_opt, int client,
						typename
							boost::enable_if
							<
								charset::is_convertible_utf8_string<String>,
								sfinae_type
							>::type sfinae = 0)
	{
		typedef String now_string_type;
		// don't change YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type inner_str_host((charset::string_charset_helper_data(str_host)));
		mongoc_ssl_opt_t* pssl_opt = const_cast<mongoc_ssl_opt_t*>(ssl_opt);

		base_type::pro_init(
			mongoc_stream_tls_new_with_hostname(
				porg_base_stream, inner_str_host.c_str(), pssl_opt, client));
	}

	c_mongo_tls_stream(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	~c_mongo_tls_stream(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::pro_move(right_ref);
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	// handshake
	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::is_convertible_utf8_string<String>,
			bool
		>::type
		handshake(const String& str_host, int32_t timeout_msec, int& events, org_bson_error_type* perr = 0)
	{
		typedef String now_string_type;
		// don't change YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type inner_str_host((charset::string_charset_helper_data(str_host)));

		return
			mongoc_stream_tls_handshake(
				base_type::org_pointer(),
				str_host.c_str(), timeout_msec, boost::addressof(events), perr);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::is_convertible_utf8_string<String>,
			bool
		>::type
		handshake(const String& host, int32_t timeout_msec, int& events, org_bson_error_type& err)
	{
		return this_type::handshake(host, timeout_msec, events, boost::addressof(err));
	}

	// handshake_block
	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::is_convertible_utf8_string<String>,
			bool
		>::type
		handshake_block(const String& str_host, int32_t timeout_msec, org_bson_error_type* perr = 0)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, now_inner_string_type);

		now_inner_string_type inner_str_host((charset::string_charset_helper_data(str_host)));

		return
			mongoc_stream_tls_handshake_block(
				base_type::org_pointer(), str_host.c_str(), timeout_msec, perr);
	}

	template<typename String> inline
	typename
		boost::enable_if
		<
			charset::is_convertible_utf8_string<String>,
			bool
		>::type
		handshake_block(const String& host, int32_t timeout_msec, org_bson_error_type& err)
	{
		return this_type::handshake_block(host, timeout_msec, boost::addressof(err));
	}
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_tls_stream)
} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_TLS_STREAM_HPP__
