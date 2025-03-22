//mongo_accesser_client.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_CLIENT_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_CLIENT_HPP__

#include <yggr/nsql_database_system/basic_mongo_accesser_client.hpp>
#include <yggr/nsql_database_system/detail/to_pointer.hpp>

namespace yggr
{
namespace nsql_database_system
{

class mongo_accesser_client
	: public basic_mongo_accesser_client
{
public:
	typedef basic_mongo_accesser_client base_type;

	typedef base_type::bson_type bson_type;
	typedef base_type::org_bson_type org_bson_type;

	typedef base_type::bson_value_type bson_value_type;
	typedef base_type::org_bson_value_type org_bson_value_type;

	typedef base_type::bson_error_type bson_error_type;
	typedef base_type::org_bson_error_type org_bson_error_type;

	typedef base_type::write_concern_type write_concern_type;
	typedef base_type::org_write_concern_type org_write_concern_type;

	typedef base_type::read_prefs_type read_prefs_type;
	typedef base_type::org_read_prefs_type org_read_prefs_type;

	typedef base_type::index_model_type index_model_type;
	typedef base_type::org_index_model_type org_index_model_type;

	typedef base_type::find_and_modify_opts_type find_and_modify_opts_type;
	typedef base_type::org_find_and_modify_opts_type org_find_and_modify_opts_type;

	typedef base_type::read_concern_type read_concern_type;
	typedef base_type::org_read_concern_type org_read_concern_type;

	typedef base_type::mongoc_error_domain_type mongoc_error_domain_type;
	typedef base_type::mongoc_error_code_type mongoc_error_code_type;

public:
	typedef base_type::uri_type uri_type;
	typedef base_type::org_uri_type org_uri_type;

	typedef base_type::ssl_opt_type ssl_opt_type;
	typedef base_type::org_ssl_opt_type org_ssl_opt_type;

	typedef base_type::server_description_type server_description_type;
	typedef base_type::org_server_description_type org_server_description_type;

	typedef base_type::stream_initiator_type stream_initiator_type;
	typedef base_type::org_stream_initiator_type org_stream_initiator_type;

	typedef base_type::apm_callbacks_type apm_callbacks_type;
	typedef base_type::org_apm_callbacks_type org_apm_callbacks_type;

	typedef base_type::server_api_type server_api_type;
	typedef base_type::org_server_api_type org_server_api_type;

	typedef base_type::auto_encryption_opts_type auto_encryption_opts_type;
	typedef base_type::org_auto_encryption_opts_type org_auto_encryption_opts_type;

private:
	typedef mongo_accesser_client this_type;

public:
	//------------------------client s---------------------------

	// client_command_simple
private:
	template<typename Connection_T, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_simple_wrap(Connection_T& conn,
										const String1& str_db,
										const org_bson_type* pcmd,
										const org_read_prefs_type* pread_prefs,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_client_command_simple(
				conn,
				utf8_str_db.data(),
				pcmd, pread_prefs,
				preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs > inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								org_bson_type* preply,
								org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_simple_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								org_bson_type& reply,
								org_bson_error_type* perr = 0)
	{
		return
			this_type::s_client_command_simple_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								org_bson_type* preply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_command_simple_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								org_bson_type& reply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_command_simple_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(reply),
				boost::addressof(err));
	}

	// integrated collection_command_simple to overload run_command_simple
public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								org_bson_type* preply,
								org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_simple(
				conn,
				str_db,
				cmd, read_prefs,
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								org_bson_type& reply,
								org_bson_error_type* perr = 0)
	{
		return
			this_type::s_client_command_simple(
				conn,
				str_db,
				cmd, read_prefs,
				reply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								org_bson_type* preply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_command_simple(
				conn,
				str_db,
				cmd, read_prefs,
				preply, err);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								org_bson_type& reply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_command_simple(
				conn,
				str_db,
				cmd, read_prefs,
				reply, err);
	}

	// client_command_simple
private:
	template<typename Connection_T, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_simple_with_server_id_wrap(Connection_T& conn,
													const String1& str_db,
													const org_bson_type* pcmd,
													const org_read_prefs_type* pread_prefs,
													u32 server_id,
													org_bson_type* preply,
													org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_client_command_simple_with_server_id(
				conn,
				utf8_str_db.data(),
				pcmd, pread_prefs, server_id,
				preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs > inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								u32 server_id,
								org_bson_type* preply,
								org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_simple_with_server_id_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				server_id,
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								u32 server_id,
								org_bson_type& reply,
								org_bson_error_type* perr = 0)
	{
		return
			this_type::s_client_command_simple_with_server_id_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				server_id,
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								u32 server_id,
								org_bson_type* preply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_command_simple_with_server_id_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				server_id,
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								u32 server_id,
								org_bson_type& reply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_command_simple_with_server_id_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				server_id,
				boost::addressof(reply),
				boost::addressof(err));
	}

	// integrated collection_command_simple to overload run_command_simple
public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								u32 server_id,
								org_bson_type* preply,
								org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_simple(
				conn,
				str_db,
				cmd, read_prefs, server_id,
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								u32 server_id,
								org_bson_type& reply,
								org_bson_error_type* perr = 0)
	{
		return
			this_type::s_client_command_simple(
				conn,
				str_db,
				cmd, read_prefs, server_id,
				reply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								u32 server_id,
								org_bson_type* preply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_command_simple(
				conn,
				str_db,
				cmd, read_prefs, server_id,
				preply, err);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command_simple(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								u32 server_id,
								org_bson_type& reply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_command_simple(
				conn,
				str_db,
				cmd, read_prefs, server_id,
				reply, err);
	}

	// client_command_with_opts
private:
	template<typename Connection_T, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_with_opts_wrap(Connection_T& conn,
										const String1& str_db,
										const org_bson_type* pcmd,
										const org_read_prefs_type* pread_prefs,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_client_command_with_opts(
				conn, utf8_str_db.data(),
				pcmd, pread_prefs, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							const BsonCmd& cmd,
							const ReadPrefs& read_prefs,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							const BsonCmd& cmd,
							const ReadPrefs& read_prefs,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							const BsonCmd& cmd,
							const ReadPrefs& read_prefs,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type& err)
	{
		return
			this_type::s_client_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err) );
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							const BsonCmd& cmd,
							const ReadPrefs& read_prefs,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type& err)
	{
		return
			this_type::s_client_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err) );
	}

	// s_collection_command_with_opts no read_pref version
public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							const BsonCmd& cmd,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							const BsonCmd& cmd,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							const BsonCmd& cmd,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type& err)
	{
		return
			this_type::s_client_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err) );
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							const BsonCmd& cmd,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type& err)
	{
		return
			this_type::s_client_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err) );
	}

	// integrated collection_command_with_opts to overload run_command
public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						const BsonCmd& cmd,
						const ReadPrefs& read_prefs,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type* perr)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				cmd, read_prefs, opts,
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						const BsonCmd& cmd,
						const ReadPrefs& read_prefs,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type* perr)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				cmd, read_prefs, opts,
				reply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						const BsonCmd& cmd,
						const ReadPrefs& read_prefs,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type& err)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				cmd, read_prefs, opts,
				preply, err);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						const BsonCmd& cmd,
						const ReadPrefs& read_prefs,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type& err)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				cmd, read_prefs, opts,
				reply, err);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						const BsonCmd& cmd,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type* perr)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				cmd, opts,
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						const BsonCmd& cmd,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type* perr)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				cmd, opts,
				reply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						const BsonCmd& cmd,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type& err)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				cmd, opts,
				preply, err);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						const BsonCmd& cmd,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type& err)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				cmd, opts,
				reply, err);
	}

	// collection_read_command
private:
	template<typename Connection_T, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_command_with_opts_wrap(Connection_T& conn,
												const String1& str_db,
												const org_bson_type* pcmd,
												const org_read_prefs_type* pread_prefs,
												const org_bson_type* popts,
												org_bson_type* preply,
												org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_client_read_command_with_opts(
				conn, utf8_str_db.data(),
				pcmd, pread_prefs, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_command(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type* perr)
	{
		return
			this_type::s_client_read_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_command(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type* perr)
	{
		return
			this_type::s_client_read_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_command(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_read_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_command(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const ReadPrefs& read_prefs,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_read_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err));
	}

	// collection_read_write_command
private:
	template<typename Connection_T, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_write_command_with_opts_wrap(Connection_T& conn,
													const String1& str_db,
													const org_bson_type* pcmd,
													const org_read_prefs_type* pread_prefs,
													const org_bson_type* popts,
													org_bson_type* preply,
													org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_client_read_write_command_with_opts(
				conn, utf8_str_db.data(),
				pcmd, pread_prefs, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_write_command(Connection_T& conn,
									const String1& str_db,
									const BsonCmd& cmd,
									const ReadPrefs& read_prefs,
									const BsonOpts& opts,
									org_bson_type* preply,
									org_bson_error_type* perr)
	{
		return
			this_type::s_client_read_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_write_command(Connection_T& conn,
									const String1& str_db,
									const BsonCmd& cmd,
									const ReadPrefs& read_prefs,
									const BsonOpts& opts,
									org_bson_type& reply,
									org_bson_error_type* perr)
	{
		return
			this_type::s_client_read_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_write_command(Connection_T& conn,
									const String1& str_db,
									const BsonCmd& cmd,
									const ReadPrefs& read_prefs,
									const BsonOpts& opts,
									org_bson_type* preply,
									org_bson_error_type& err)
	{
		return
			this_type::s_client_read_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_write_command(Connection_T& conn,
									const String1& str_db,
									const BsonCmd& cmd,
									const ReadPrefs& read_prefs,
									const BsonOpts& opts,
									org_bson_type& reply,
									org_bson_error_type& err)
	{
		return
			this_type::s_client_read_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err));
	}

	// collection_read_write_command no read_prefs version
	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_write_command(Connection_T& conn,
									const String1& str_db,
									const BsonCmd& cmd,
									const BsonOpts& opts,
									org_bson_type* preply,
									org_bson_error_type* perr)
	{
		return
			this_type::s_client_read_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_write_command(Connection_T& conn,
									const String1& str_db,
									const BsonCmd& cmd,
									const BsonOpts& opts,
									org_bson_type& reply,
									org_bson_error_type* perr)
	{
		return
			this_type::s_client_read_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_write_command(Connection_T& conn,
									const String1& str_db,
									const BsonCmd& cmd,
									const BsonOpts& opts,
									org_bson_type* preply,
									org_bson_error_type& err)
	{
		return
			this_type::s_client_read_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_read_write_command(Connection_T& conn,
									const String1& str_db,
									const BsonCmd& cmd,
									const BsonOpts& opts,
									org_bson_type& reply,
									org_bson_error_type& err)
	{
		return
			this_type::s_client_read_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err));
	}

	// collection_write_command
private:
	template<typename Connection_T, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_write_command_with_opts_wrap(Connection_T& conn,
												const String1& str_db,
												const org_bson_type* pcmd,
												const org_bson_type* popts,
												org_bson_type* preply,
												org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_client_write_command_with_opts(
				conn, utf8_str_db.data(),
				pcmd, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_write_command(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type* perr)
	{
		return
			this_type::s_client_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_write_command(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type* perr)
	{
		return
			this_type::s_client_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_write_command(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd,
				typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_write_command(Connection_T& conn,
								const String1& str_db,
								const BsonCmd& cmd,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_write_command_with_opts_wrap(
				conn,
				str_db,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err));
	}

	//  client_command flags version
private:
	template<typename Connection_T, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_command_wrap(Connection_T& conn,
								const String1& str_db,
								u32 flags,
								const org_bson_type* pcmd,
								const org_read_prefs_type* pread_prefs,
								const org_bson_type* popts,
								org_bson_type* preply,
								org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_client_command(
				conn,
				utf8_str_db.data(),
				flags,
				pcmd, pread_prefs, popts,
				preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs, typename BsonOpts > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							u32 flags,
							const BsonCmd& cmd,
							const ReadPrefs& read_prefs,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_wrap(
				conn,
				str_db,
				flags,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							u32 flags,
							const BsonCmd& cmd,
							const ReadPrefs& read_prefs,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_wrap(
				conn,
				str_db,
				flags,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							u32 flags,
							const BsonCmd& cmd,
							const ReadPrefs& read_prefs,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type& err)
	{
		return
			this_type::s_client_command_wrap(
				conn,
				str_db,
				flags,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							u32 flags,
							const BsonCmd& cmd,
							const ReadPrefs& read_prefs,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type& err)
	{
		return
			this_type::s_client_command_wrap(
				conn,
				str_db,
				flags,
				detail::to_const_pointer<org_bson_type>(cmd),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err) );
	}

	// no read_prefs version
public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename BsonOpts > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							u32 flags,
							const BsonCmd& cmd,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_wrap(
				conn,
				str_db,
				flags,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							u32 flags,
							const BsonCmd& cmd,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type* perr)
	{
		return
			this_type::s_client_command_wrap(
				conn,
				str_db,
				flags,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
							const String1& str_db,
							u32 flags,
							const BsonCmd& cmd,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type& err)
	{
		return
			this_type::s_client_command_wrap(
				conn,
				str_db,
				flags,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_client_command(Connection_T& conn,
								const String1& str_db,
								u32 flags,
								const BsonCmd& cmd,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type& err)
	{
		return
			this_type::s_client_command_wrap(
				conn,
				str_db,
				flags,
				detail::to_const_pointer<org_bson_type>(cmd),
				0,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err) );
	}

	// integrated client_command flags version to collection_command
public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs, typename BsonOpts > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						u32 flags,
						const BsonCmd& cmd,
						const ReadPrefs& read_prefs,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type* perr)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				flags,
				cmd, read_prefs, opts,
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						u32 flags,
						const BsonCmd& cmd,
						const ReadPrefs& read_prefs,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type* perr)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				flags,
				cmd, read_prefs, opts,
				reply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						u32 flags,
						const BsonCmd& cmd,
						const ReadPrefs& read_prefs,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type& err)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				flags,
				cmd, read_prefs, opts,
				preply, err);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename ReadPrefs, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						u32 flags,
						const BsonCmd& cmd,
						const ReadPrefs& read_prefs,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type& err)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				flags,
				cmd, read_prefs, opts,
				reply, err);
	}

	// no read_prefs version
public:
	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename BsonOpts > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						u32 flags,
						const BsonCmd& cmd,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type* perr)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				flags,
				cmd, opts,
				preply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						u32 flags,
						const BsonCmd& cmd,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type* perr)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				flags,
				cmd, opts,
				reply, perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						u32 flags,
						const BsonCmd& cmd,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type& err)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				flags,
				cmd, opts,
				preply, err);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonCmd, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, typename native_t<BsonCmd>::type>
			>,
			bool
		>::type
		s_run_command(Connection_T& conn,
						const String1& str_db,
						u32 flags,
						const BsonCmd& cmd,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type& err)
	{
		return
			this_type::s_client_command(
				conn,
				str_db,
				flags,
				cmd, opts,
				reply, err);
	}

public:
	// s_client_get_crypt_shared_version return inner_string_type
	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn)
	{
		return base_type::s_org_client_get_crypt_shared_version(conn, 0);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, const string& ret_charset_name)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_get_crypt_shared_version(conn, 0);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, org_bson_error_type* perr)
	{
		return base_type::s_org_client_get_crypt_shared_version(conn, perr);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn,
											const string& ret_charset_name,
											org_bson_error_type* perr)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_get_crypt_shared_version(conn, perr);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, org_bson_error_type& err)
	{
		return base_type::s_org_client_get_crypt_shared_version(conn, boost::addressof(err));
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn,
											const string& ret_charset_name,
											org_bson_error_type& err)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_get_crypt_shared_version(conn, boost::addressof(err));
	}

	// s_client_get_crypt_shared_version return inner_string_type allocator ver
	template<typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, const Alloc& alloc)
	{
		return base_type::s_org_client_get_crypt_shared_version(conn, alloc, 0);
	}

	template<typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, const Alloc& alloc, const string& ret_charset_name)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_get_crypt_shared_version(conn, alloc, 0);
	}

	template<typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, const Alloc& alloc, org_bson_error_type* perr)
	{
		return base_type::s_org_client_get_crypt_shared_version(conn, alloc, perr);
	}

	template<typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn,
											const Alloc& alloc,
											const string& ret_charset_name,
											org_bson_error_type* perr)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_get_crypt_shared_version(conn, alloc, perr);
	}

	template<typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, const Alloc& alloc, org_bson_error_type& err)
	{
		return base_type::s_org_client_get_crypt_shared_version(conn, alloc, boost::addressof(err));
	}

	template<typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			inner_string_type
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn,
											const Alloc& alloc,
											const string& ret_charset_name,
											org_bson_error_type& err)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_get_crypt_shared_version(conn, alloc, boost::addressof(err));
	}

	// s_client_get_crypt_shared_version return other_string
	template<typename String, typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn)
	{
		typedef String now_string_type;
		return base_type::s_org_client_get_crypt_shared_version(conn, 0).template str<now_string_type>();
	}

	template<typename String, typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, const string& ret_charset_name)
	{
		typedef String now_string_type;
		return base_type::s_org_client_get_crypt_shared_version(conn, 0).template str<now_string_type>(ret_charset_name);
	}

	template<typename String, typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, org_bson_error_type* perr)
	{
		typedef String now_string_type;
		return base_type::s_org_client_get_crypt_shared_version(conn, perr).template str<now_string_type>();
	}

	template<typename String, typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn,
											const string& ret_charset_name,
											org_bson_error_type* perr)
	{
		typedef String now_string_type;
		return base_type::s_org_client_get_crypt_shared_version(conn, perr).template str<now_string_type>(ret_charset_name);
	}

	template<typename String, typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, org_bson_error_type& err)
	{
		typedef String now_string_type;
		return
			base_type::s_org_client_get_crypt_shared_version(conn, boost::addressof(err))
				.template str<now_string_type>();
	}

	template<typename String, typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn,
											const string& ret_charset_name,
											org_bson_error_type& err)
	{
		typedef String now_string_type;
		return
			base_type::s_org_client_get_crypt_shared_version(conn, boost::addressof(err))
				.template str<now_string_type>(ret_charset_name);
	}

	// s_client_get_crypt_shared_version return other_string allocator ver
	template<typename String, typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, const Alloc& alloc)
	{
		typedef String now_string_type;
		return base_type::s_org_client_get_crypt_shared_version(conn, 0).template str<now_string_type>(alloc);
	}

	template<typename String, typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, const Alloc& alloc, const string& ret_charset_name)
	{
		typedef String now_string_type;
		return base_type::s_org_client_get_crypt_shared_version(conn, 0).template str<now_string_type>(alloc, ret_charset_name);
	}

	template<typename String, typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, const Alloc& alloc, org_bson_error_type* perr)
	{
		typedef String now_string_type;
		return base_type::s_org_client_get_crypt_shared_version(conn, perr).template str<now_string_type>(alloc);
	}

	template<typename String, typename Alloc, typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn,
											const Alloc& alloc,
											const string& ret_charset_name,
											org_bson_error_type* perr)
	{
		typedef String now_string_type;
		return base_type::s_org_client_get_crypt_shared_version(conn, perr).template str<now_string_type>(alloc, ret_charset_name);
	}

	template<typename String, typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn, const Alloc& alloc, org_bson_error_type& err)
	{
		typedef String now_string_type;
		return
			base_type::s_org_client_get_crypt_shared_version(conn, boost::addressof(err))
				.template str<now_string_type>(alloc);
	}

	template<typename String, typename Connection_T, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		s_client_get_crypt_shared_version(Connection_T& conn,
											const Alloc& alloc,
											const string& ret_charset_name,
											org_bson_error_type& err)
	{
		typedef String now_string_type;
		return
			base_type::s_org_client_get_crypt_shared_version(conn, boost::addressof(err))
				.template str<now_string_type>(alloc, ret_charset_name);
	}

	// s_client_is_crypt_enabled
public:
	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_client_is_crypt_enabled(Connection_T& conn)
	{
		return base_type::s_org_client_is_crypt_enabled(conn);
	}

public:
	template<typename Connection_T,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector>
	static bool s_client_watch(Connection_T& conn,
								const BsonPipeline& pipeline,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type* perr,
								const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_watch(
				conn,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr,
				handler_collector);
	}

	template<typename Connection_T,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector>
	static bool s_client_watch(Connection_T& conn,
								const BsonPipeline& pipeline,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type* perr,
								const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_watch(
				conn,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				perr,
				handler_collector);
	}

	template<typename Connection_T,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector>
	static bool s_client_watch(Connection_T& conn,
								const BsonPipeline& pipeline,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type& err,
								const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_watch(
				conn,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err),
				handler_collector);
	}

	template<typename Connection_T,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector>
	static bool s_client_watch(Connection_T& conn,
								const BsonPipeline& pipeline,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type& err,
								const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_watch(
				conn,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err),
				handler_collector);
	}

public:
	template<typename Connection_T,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector>
	static bool s_watch(Connection_T& conn,
						const BsonPipeline& pipeline,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type* perr,
						const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_watch(
				conn,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr,
				handler_collector);
	}

	template<typename Connection_T,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector>
	static bool s_watch(Connection_T& conn,
						const BsonPipeline& pipeline,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type* perr,
						const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_watch(
				conn,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				perr,
				handler_collector);
	}

	template<typename Connection_T,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector>
	static bool s_watch(Connection_T& conn,
						const BsonPipeline& pipeline,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type& err,
						const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_watch(
				conn,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err),
				handler_collector);
	}

	template<typename Connection_T,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector>
	static bool s_watch(Connection_T& conn,
						const BsonPipeline& pipeline,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type& err,
						const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_watch(
				conn,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err),
				handler_collector);
	}

	// get_database_names
public:
	template<typename Connection_T, typename BsonOpts, typename HandlerCollector> inline
	static bool s_client_get_database_names(Connection_T& conn,
											const BsonOpts& opts,
											org_bson_error_type* perr,
											const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_get_database_names(
				conn,
				detail::to_const_pointer<org_bson_type>(opts),
				perr,
				handler_collector);
	}

	template<typename Connection_T, typename BsonOpts, typename HandlerCollector> inline
	static bool s_client_get_database_names(Connection_T& conn,
											const BsonOpts& opts,
											org_bson_error_type& err,
											const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_get_database_names(
				conn,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err),
				handler_collector);
	}

	template<typename Connection_T, typename HandlerCollector> inline
	static bool s_client_get_database_names(Connection_T& conn,
												org_bson_error_type* perr,
												const HandlerCollector& handler_collector)
	{
		return base_type::s_org_client_get_database_names(conn, 0, perr, handler_collector);
	}

	template<typename Connection_T, typename HandlerCollector> inline
	static bool s_client_get_database_names(Connection_T& conn,
											org_bson_error_type& err,
											const HandlerCollector& handler_collector)
	{
		return base_type::s_org_client_get_database_names(conn, 0, boost::addressof(err), handler_collector);
	}

	//get_server_description
public:
	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			server_description_type
		>::type
		s_client_get_server_description(Connection_T& conn, u32 svr_id, org_bson_error_type* perr = 0)
	{
		return base_type::s_org_client_get_server_description(conn, svr_id, perr);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			server_description_type
		>::type
		s_client_get_server_description(Connection_T& conn, u32 svr_id, org_bson_error_type& err)
	{
		return base_type::s_org_client_get_server_description(conn, svr_id, boost::addressof(err));
	}

	//get_server_descriptions
public:
	template<typename Connection_T, typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			std::size_t
		>::type
		s_client_get_server_descriptions(Connection_T& conn,
											const HandlerCollector& handler_collection)
	{
		return base_type::s_org_client_get_server_descriptions(conn, 0, handler_collection);
	}

	template<typename Connection_T, typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			std::size_t
		>::type
		s_client_get_server_descriptions(Connection_T& conn,
											org_bson_error_type* perr,
											const HandlerCollector& handler_collection)
	{
		return base_type::s_org_client_get_server_descriptions(conn, perr, handler_collection);
	}

	template<typename Connection_T, typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			std::size_t
		>::type
		s_client_get_server_descriptions(Connection_T& conn,
											org_bson_error_type& err,
											const HandlerCollector& handler_collection)
	{
		return
			base_type::s_org_client_get_server_descriptions(
				conn,
				boost::addressof(err),
				handler_collection);
	}

public:
	template<typename Connection_T, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			server_description_type
		>::type
		s_client_select_server(Connection_T& conn,
								bool for_writes,
								const ReadPrefs& read_prefs,
								org_bson_error_type* perr = 0)
	{
		return
			base_type::s_org_client_select_server(
				conn,
				for_writes,
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				perr);
	}

	template<typename Connection_T, typename ReadPrefs> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			server_description_type
		>::type
		s_client_select_server(Connection_T& conn,
								bool for_writes,
								const ReadPrefs& read_prefs,
								org_bson_error_type& err)
	{
		return
			base_type::s_org_client_select_server(
				conn,
				for_writes,
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(err));
	}

	//get_read_concern
public:
	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			read_concern_type
		>::type
		s_client_get_read_concern(Connection_T& conn, org_bson_error_type* perr = 0)
	{
		return base_type::s_org_client_get_read_concern(conn, perr);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			read_concern_type
		>::type
		s_client_get_read_concern(Connection_T& conn, org_bson_error_type& err)
	{
		return base_type::s_org_client_get_read_concern(conn, boost::addressof(err));
	}

	//get_read_prefs
public:
	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			read_prefs_type
		>::type
		s_client_get_read_prefs(Connection_T& conn, org_bson_error_type* perr = 0)
	{
		return base_type::s_org_client_get_read_prefs(conn, perr);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			read_prefs_type
		>::type
		s_client_get_read_prefs(Connection_T& conn, org_bson_error_type& err)
	{
		return base_type::s_org_client_get_read_prefs(conn, boost::addressof(err));
	}

	// get_server_status
private:
	template<typename Connection_T> inline
	static bool s_client_get_server_status_wrap(Connection_T& conn,
												const org_read_prefs_type* pread_prefs,
												org_bson_type* preply,
												org_bson_error_type* perr)
	{
		static const bson_type cmd(charset::utf8_string("{\"serverStatus\" : 1 }", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		static const charset::utf8_string db("admin", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		return
			base_type::s_org_client_command_simple(
				conn, db.c_str(), boost::addressof(cmd), pread_prefs,
				preply, perr);
	}

public:
	template<typename Connection_T, typename ReadPrefs> inline
	static bool s_client_get_server_status(Connection_T& conn,
											const ReadPrefs& read_prefs,
											org_bson_type* preply,
											org_bson_error_type* perr = 0)
	{
		return
			this_type::s_client_get_server_status_wrap(
				conn,
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				preply, perr);
	}

	template<typename Connection_T, typename ReadPrefs> inline
	static bool s_client_get_server_status(Connection_T& conn,
											const ReadPrefs& read_prefs,
											org_bson_type& reply,
											org_bson_error_type* perr = 0)
	{
		return
			this_type::s_client_get_server_status_wrap(
				conn,
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T, typename ReadPrefs> inline
	static bool s_client_get_server_status(Connection_T& conn,
											const ReadPrefs& read_prefs,
											org_bson_type* preply,
											org_bson_error_type& err)
	{
		return
			this_type::s_client_get_server_status_wrap(
				conn,
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T, typename ReadPrefs> inline
	static bool s_client_get_server_status(Connection_T& conn,
											const ReadPrefs& read_prefs,
											org_bson_type& reply,
											org_bson_error_type& err)
	{
		return
			this_type::s_client_get_server_status_wrap(
				conn,
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(reply),
				boost::addressof(err));
	}

	// get_uri
public:
	template<typename Connection_T> inline
	static uri_type s_client_get_uri(Connection_T& conn)
	{
		return conn.uri();
	}

	// get_write_concern
public:
	template<typename Connection_T>
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			write_concern_type
		>::type
		s_client_get_write_concern(Connection_T& conn, org_bson_error_type* perr = 0)
	{
		return base_type::s_org_client_get_write_concern(conn, perr);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			write_concern_type
		>::type
		s_client_get_write_concern(Connection_T& conn, org_bson_error_type& err)
	{
		return base_type::s_org_client_get_write_concern(conn, boost::addressof(err));
	}

	// set_read_conecrn
public:
	template<typename Connection_T, typename ReadConcern > inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_client_set_read_concern(Connection_T& conn,
									const ReadConcern& read_concern,
									org_bson_error_type* perr = 0)
	{
		return
			base_type::s_org_client_set_read_concern(
				conn,
				detail::to_const_pointer<org_read_concern_type>(read_concern),
				perr);
	}

	template<typename Connection_T, typename ReadConcern > inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_client_set_read_concern(Connection_T& conn,
									const ReadConcern& read_concern,
									org_bson_error_type& err)
	{
		return
			base_type::s_org_client_set_read_concern(
				conn,
				detail::to_const_pointer<org_read_concern_type>(read_concern),
				boost::addressof(err));
	}

	//  set_read_prefs
public:
	template<typename Connection_T, typename ReadPrefs > inline
	static bool s_client_set_read_prefs(Connection_T& conn,
										const ReadPrefs& read_prefs,
										org_bson_error_type* perr = 0)
	{
		return
			base_type::s_org_client_set_read_prefs(
				conn,
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				perr);
	}

	template<typename Connection_T, typename ReadPrefs > inline
	static bool s_client_set_read_prefs(Connection_T& conn,
										const ReadPrefs& read_prefs,
										org_bson_error_type& err)
	{
		return
			base_type::s_org_client_set_read_prefs(
				conn,
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(err));
	}

	// get_set_ssl_opts
#ifdef MONGOC_ENABLE_SSL

public:
	template<typename Connection_T> inline
	static ssl_opt_type s_client_get_ssl_opts(Connection_T& conn)
	{
		return conn.ssl_opt();
	}

	template<typename Connection_T> inline
	static bool s_org_client_set_ssl_opts(Connection_T& conn, const org_ssl_opt_type* pssl_opt)
	{
		return conn.ssl_opt(pssl_opt);
	}

	template<typename Connection_T, typename SSL_OPT> inline
	static bool s_client_set_ssl_opts(Connection_T& conn, const SSL_OPT& ssl_opt)
	{
		return conn.ssl_opt(ssl_opt);
	}

#endif // MONGOC_ENABLE_SSL

	//set_write_concern
public:
	template<typename Connection_T, typename WriteConcern> inline
	static bool s_client_set_write_concern(Connection_T& conn,
											const WriteConcern& write_concern,
											org_bson_error_type* perr = 0)
	{
		return
			base_type::s_org_client_set_write_concern(
				conn,
				detail::to_const_pointer<org_write_concern_type>(write_concern),
				perr);
	}

	template<typename Connection_T, typename WriteConcern> inline
	static bool s_client_set_write_concern(Connection_T& conn,
											const WriteConcern& write_concern,
											org_bson_error_type& err)
	{
		return
			base_type::s_org_client_set_write_concern(
				conn,
				detail::to_const_pointer<org_write_concern_type>(write_concern),
				boost::addressof(err));
	}


	//set_stream_initiator()
public:
	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_client_set_stream_initiator(Connection_T& conn,
										stream_initiator_type pfoo_initator = 0,
										void* puser_data = 0,
										org_bson_error_type* perr = 0)
	{
		return base_type::s_org_client_set_stream_initiator(conn, pfoo_initator, puser_data, perr);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_client_set_stream_initiator(Connection_T& conn,
										stream_initiator_type pfoo_initator,
										void* puser_data,
										org_bson_error_type& err)
	{
		return base_type::s_org_client_set_stream_initiator(conn, pfoo_initator, puser_data, boost::addressof(err));
	}

	template<typename Connection_T, typename Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				boost::is_same<void*, typename func::foo_t_info<Handler>::result_type>
			>,
			bool
		>::type
		s_client_set_stream_initiator(Connection_T& conn,
										stream_initiator_type pfoo_initator,
										const Handler& handler_user_data,
										org_bson_error_type* perr = 0)
	{
		return base_type::s_org_client_set_stream_initiator(conn, pfoo_initator, handler_user_data, perr);
	}

	template<typename Connection_T, typename Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
				boost::is_same<void*, typename func::foo_t_info<Handler>::result_type>
			>,
			bool
		>::type
		s_client_set_stream_initiator(Connection_T& conn,
										stream_initiator_type pfoo_initator,
										const Handler& handler_user_data,
										org_bson_error_type& err)
	{
		return base_type::s_org_client_set_stream_initiator(conn, pfoo_initator, handler_user_data, boost::addressof(err));
	}

public:
	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_client_reset(Connection_T& conn, org_bson_error_type* perr = 0)
	{
		return base_type::s_org_client_reset(conn, perr);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			bool
		>::type
		s_client_reset(Connection_T& conn, org_bson_error_type& err)
	{
		return base_type::s_org_client_reset(conn, boost::addressof(err));
	}

	// client_set_appname
	template<typename Connection_T, typename String> inline
	static bool s_client_set_appname(Connection_T& conn, const String& str_appname)
	{
		return conn.set_appname(str_appname);
	}

	// client_set_apm_callbacks
	template<typename Connection_T, typename ApmCallBacks> inline
	static bool s_client_set_apm_callbacks(Connection_T& conn, const ApmCallBacks& cbs, void* contex = 0)
	{
		return conn.set_apm_callbacks(cbs, contex);
	}

	template<typename Connection_T, typename ApmCallBacks, typename Context> inline
	static bool s_client_set_apm_callbacks(Connection_T& conn, const ApmCallBacks& cbs, Context& contex)
	{
		return conn.set_apm_callbacks(cbs, contex);
	}

	// client_set_error_api
	template<typename Connection_T> inline
	static bool s_client_set_error_api(Connection_T& conn, s32 version)
	{
		return conn.set_error_api(version);
	}

	// s_client_get_handshake_description
	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			server_description_type
		>::type
		s_client_get_handshake_description(Connection_T& conn,
											u32 server_id,
											org_bson_error_type* perr = 0)
	{
		return
			base_type::s_org_client_get_handshake_description(
				conn, server_id, 0, perr);
	}

	template<typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			server_description_type
		>::type
		s_client_get_handshake_description(Connection_T& conn,
											u32 server_id,
											org_bson_error_type& err)
	{
		return
			base_type::s_org_client_get_handshake_description(
				conn, server_id, 0, boost::addressof(err));
	}

	template<typename Connection_T, typename ServerApi> inline
	static bool s_client_set_server_api(Connection_T& conn, const ServerApi& svr_api, org_bson_error_type* perr = 0)
	{
		return conn.set_server_api(svr_api, perr);
	}

	template<typename Connection_T, typename ServerApi> inline
	static bool s_client_set_server_api(Connection_T& conn, const ServerApi& svr_api, org_bson_error_type& err)
	{
		return conn.set_server_api(svr_api, err);
	}

public:
	// no pkeyvalut_conn version
	// shared_ptr_ver
	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												smart_ptr_ex::shared_ptr<Connection_T> pconn,
												const AutoEncOpts& opts,
												org_bson_error_type* perr = 0)
	{
		typedef Connection_T connection_type;
		typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;

		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, pconn, connection_ptr_type(), tmp_opts.org_pointer(), perr);
	}

	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												smart_ptr_ex::shared_ptr<Connection_T> pconn,
												const AutoEncOpts& opts,
												org_bson_error_type& err)
	{
		typedef Connection_T connection_type;
		typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;

		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, pconn, connection_ptr_type(), tmp_opts.org_pointer(), boost::addressof(err));
	}

	// conn_t version

	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												Connection_T& conn,
												const AutoEncOpts& opts,
												org_bson_error_type* perr = 0)
	{
		typedef Connection_T connection_type;
		typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;

		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, conn, connection_ptr_type(), tmp_opts.org_pointer(), perr);
	}

	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												Connection_T& conn,
												const AutoEncOpts& opts,
												org_bson_error_type& err)
	{
		typedef Connection_T connection_type;
		typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;

		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, conn, connection_ptr_type(), tmp_opts.org_pointer(), boost::addressof(err));
	}

	// pkeyvalut_conn version
	// pconn, pkeyvault_conn
	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												smart_ptr_ex::shared_ptr<Connection_T> pconn,
												smart_ptr_ex::shared_ptr<Connection_T> pkeyvalut_conn,
												const AutoEncOpts& opts,
												org_bson_error_type* perr = 0)
	{
		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, pconn, pkeyvalut_conn, tmp_opts.org_pointer(), perr);
	}

	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												smart_ptr_ex::shared_ptr<Connection_T> pconn,
												smart_ptr_ex::shared_ptr<Connection_T> pkeyvalut_conn,
												const AutoEncOpts& opts,
												org_bson_error_type& err)
	{
		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, pconn, pkeyvalut_conn, tmp_opts.org_pointer(), boost::addressof(err));
	}

	// conn, pkeyvalut_conn
	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												Connection_T& conn,
												smart_ptr_ex::shared_ptr<Connection_T> pkeyvalut_conn,
												const AutoEncOpts& opts,
												org_bson_error_type* perr = 0)
	{
		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, conn, pkeyvalut_conn, tmp_opts.org_pointer(), perr);
	}

	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												Connection_T& conn,
												smart_ptr_ex::shared_ptr<Connection_T> pkeyvalut_conn,
												const AutoEncOpts& opts,
												org_bson_error_type& err)
	{
		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, conn, pkeyvalut_conn, tmp_opts.org_pointer(), boost::addressof(err));
	}

	// pconn, keyvalut_conn
	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												smart_ptr_ex::shared_ptr<Connection_T> pconn,
												Connection_T& keyvalut_conn,
												const AutoEncOpts& opts,
												org_bson_error_type* perr = 0)
	{
		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, pconn, keyvalut_conn, tmp_opts.org_pointer(), perr);
	}

	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												smart_ptr_ex::shared_ptr<Connection_T> pconn,
												Connection_T& keyvalut_conn,
												const AutoEncOpts& opts,
												org_bson_error_type& err)
	{
		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, pconn, keyvalut_conn, tmp_opts.org_pointer(), boost::addressof(err));
	}

	// conn, keyvalut_conn,
	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												Connection_T& conn,
												Connection_T& keyvalut_conn,
												const AutoEncOpts& opts,
												org_bson_error_type* perr = 0)
	{
		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, conn, keyvalut_conn, tmp_opts.org_pointer(), perr);
	}

	template<typename EncConnect, typename Connection_T, typename AutoEncOpts> inline
	static bool s_client_enable_auto_encryption(EncConnect& enc_conn,
												Connection_T& conn,
												Connection_T& keyvalut_conn,
												const AutoEncOpts& opts,
												org_bson_error_type& err)
	{
		auto_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_enable_auto_encryption(
				enc_conn, conn, keyvalut_conn, tmp_opts.org_pointer(), boost::addressof(err));
	}

	// client_using_handler
public:
	template<typename Connection_T, typename Handler> inline
	static typename func::foo_t_info<Handler>::result_type
		s_client_using_handler(Connection_T& conn, const Handler& handler, org_bson_error_type* perr = 0)
	{
		return base_type::s_org_client_using_handler(conn, handler, perr);
	}

	template<typename Connection_T, typename Handler> inline
	static typename func::foo_t_info<Handler>::result_type
		s_client_using_handler(Connection_T& conn, const Handler& handler, org_bson_error_type& err)
	{
		return base_type::s_org_client_using_handler(conn, handler, boost::addressof(err));
	}

	//------------------------client e---------------------------
};

} // namespace nsql_database_system
} // namespace yggr

#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_CLIENT_HPP__
