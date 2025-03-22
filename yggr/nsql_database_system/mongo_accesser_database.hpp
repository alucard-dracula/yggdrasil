//mongo_accesser_database.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_DATABASE_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_DATABASE_HPP__

#include <yggr/nsql_database_system/basic_mongo_accesser_database.hpp>
#include <yggr/nsql_database_system/detail/to_pointer.hpp>

namespace yggr
{
namespace nsql_database_system
{

class mongo_accesser_database
	: public basic_mongo_accesser_database
{

public:
	typedef basic_mongo_accesser_database base_type;

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

private:
	typedef mongo_accesser_database this_type;

public:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// don't coding get_param or set_params function, because that is call time used only, 
	// example get_read_concern(...)
	// if you want to use that, please call foo "using_handler" execute you custom native operations
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//------------------------database s------------------------
private:
	//database_aggregate
	template<typename Connection_T, 
				typename String1,
				typename HandlerCollector> inline
	static bool s_database_aggregate_wrap(Connection_T& conn, 
											const String1& str_db,
											const org_bson_type* ppipeline, 
											const org_bson_type* popts,
											const org_read_prefs_type* pread_prefs,
											org_bson_error_type* perr, 
											const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return 
			base_type::s_org_database_aggregate(
				conn, utf8_str_db.data(), 
				ppipeline, popts, pread_prefs, 
				perr, 
				handler_collector);
	}

public:
	template<typename Connection_T, 
				typename String1,
				typename BsonPipeline, typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
	static bool s_database_aggregate(Connection_T& conn, 
										const String1& str_db,
										const BsonPipeline& pipeline, 
										const BsonOpts& opts,
										const ReadPrefs& read_prefs,
										org_bson_error_type* perr, 
										const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_aggregate_wrap(
				conn, 
				str_db,
				detail::to_const_pointer<org_bson_type>(pipeline), 
				detail::to_const_pointer<org_bson_type>(opts), 
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				perr, 
				handler_collector);
	}

	template<typename Connection_T, 
				typename String1,
				typename BsonPipeline, typename BsonOpts, typename ReadPrefs,
				typename HandlerCollector> inline
	static bool s_database_aggregate(Connection_T& conn, 
										const String1& str_db,
										const BsonPipeline& pipeline, 
										const BsonOpts& opts,
										const ReadPrefs& read_prefs,
										org_bson_error_type& err, 
										const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_aggregate_wrap(
				conn, 
				str_db,
				detail::to_const_pointer<org_bson_type>(pipeline), 
				detail::to_const_pointer<org_bson_type>(opts), 
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(err), 
				handler_collector);
	}


	// database_create_collection
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_database_create_collecion_wrap(Connection_T& conn, 
											const String1& str_db, 
											const String2& str_coln,
											const org_bson_type* popts, 
											org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return base_type::s_org_database_create_collecion(conn, utf8_str_db.data(), utf8_str_coln.data(), popts, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_database_create_collecion(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonOpts& opts, 
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_database_create_collecion_wrap(
				conn, 
				str_db, 
				str_coln, 
				detail::to_const_pointer<org_bson_type>(opts), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts > inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_database_create_collecion(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonOpts& opts, 
									org_bson_error_type& err)
	{
		return 
			this_type::s_database_create_collecion_wrap(
				conn, 
				str_db, 
				str_coln, 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(err));
	}

	// find_collections
private:
	template<typename Connection_T,
				typename String1,
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_find_collections_wrap(Connection_T& conn, 
											const String1& str_db,
											const org_bson_type* popts, 
											org_bson_error_type* perr,
											const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return 
			base_type::s_org_database_find_collections(
				conn, utf8_str_db.data(), 
				popts, perr, handler_collector);
	}

public:
	template<typename Connection_T, 
				typename String1,
				typename BsonOpts, 
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_find_collections(Connection_T& conn, 
									const String1& str_db,
									const BsonOpts& opts, 
									org_bson_error_type* perr,
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_find_collections_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(opts), 
				perr, 
				handler_collector);
	}

	template<typename Connection_T, 
				typename String1,
				typename BsonOpts, 
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_find_collections(Connection_T& conn, 
									const String1& str_db,
									const BsonOpts& opts, 
									org_bson_error_type& err,
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_find_collections_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(err), 
				handler_collector);
	}

	//has_collection
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_database_has_collection_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_database_has_collection(
				conn, utf8_str_db.data(), utf8_str_coln.data(), perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_database_has_collection(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_database_has_collection_wrap(
				conn, str_db, str_coln, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_database_has_collection(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									org_bson_error_type& err)
	{
		return 
			this_type::s_database_has_collection_wrap(
				conn, str_db, str_coln, boost::addressof(err));
	}

	// get_collection_names
private:
	template<typename Connection_T, 
				typename String1,
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_get_collection_names_wrap(Connection_T& conn, 
												const String1& str_db,
												const org_bson_type* popts,
												org_bson_error_type* perr,
												const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return 
			base_type::s_org_database_get_collection_names(
				conn, utf8_str_db.data(), popts, perr, handler_collector);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonOpts,
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_get_collection_names(Connection_T& conn, 
											const String1& str_db,
											const BsonOpts& opts,
											org_bson_error_type* perr,
											const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_get_collection_names_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(opts),
				perr, 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonOpts,
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_get_collection_names(Connection_T& conn, 
											const String1& str_db,
											const BsonOpts& opts,
											org_bson_error_type& err,
											const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_get_collection_names_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err), 
				handler_collector);
	}

	// database_drop
private:
	template<typename Connection_T, typename String1> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_drop_wrap(Connection_T& conn, 
								const String1& str_db,
								const org_bson_type* popts,
								org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		return base_type::s_org_database_drop(conn, utf8_str_db.data(), popts, perr);
	}

public:
	template<typename Connection_T, 
				typename String1,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::mpl::or_
				<
					boost::is_base_of<org_bson_type, typename native_t<BsonOpts>::type>,
					boost::is_same<int, BsonOpts>
				>
			>,
			bool
		>::type
		s_database_drop(Connection_T& conn, 
						const String1& str_db, 
						const BsonOpts& opts,
						org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_database_drop_wrap(
				conn, 
				str_db,
				detail::to_const_pointer<org_bson_type>(opts),
				perr);
	}

	template<typename Connection_T, 
				typename String1,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::mpl::or_
				<
					boost::is_base_of<org_bson_type, typename native_t<BsonOpts>::type>,
					boost::is_same<int, BsonOpts>
				>
			>,
			bool
		>::type
		s_database_drop(Connection_T& conn, 
						const String1& str_db, 
						const BsonOpts& opts,
						org_bson_error_type& err)
	{
		return 
			this_type::s_database_drop_wrap(
				conn, 
				str_db,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err) );
	}

	template<typename Connection_T, 
				typename String1> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_drop(Connection_T& conn, 
						const String1& str_db, 
						org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_database_drop_wrap(
				conn, str_db, 0, perr);
	}

	template<typename Connection_T, 
				typename String1> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_drop(Connection_T& conn, 
						const String1& str_db, 
						org_bson_error_type& err)
	{
		return 
			this_type::s_database_drop_wrap(
				conn, str_db, 0, boost::addressof(err) );
	}

	// add_user
private:
	template<typename Connection_T,
				typename String1, typename String2, typename String3> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_database_add_user_wrap(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_uname, 
									const String3& str_pwd, 
									const org_bson_type* proles,
									const org_bson_type* pcustom_data,
									org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_uname((charset::string_charset_helper_data(str_uname)));
		utf8_string3_type utf8_str_pwd((charset::string_charset_helper_data(str_pwd)));

		return 
			base_type::s_org_database_add_user(
				conn, utf8_str_db.data(), 
				utf8_str_uname.data(), utf8_str_pwd.data(),
				proles, pcustom_data, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonRoles, typename BsonCustomData> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_database_add_user(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_uname, 
							const String3& str_pwd, 
							const BsonRoles& roles,
							const BsonCustomData& custom_data,
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_database_add_user_wrap(
				conn, 
				str_db, str_uname, str_pwd,
				detail::to_const_pointer<org_bson_type>(roles), 
				detail::to_const_pointer<org_bson_type>(custom_data), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonRoles, typename BsonCustomData> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_database_add_user(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_uname, 
							const String3& str_pwd, 
							const BsonRoles& roles,
							const BsonCustomData& custom_data,
							org_bson_error_type& err)
	{
		return 
			this_type::s_database_add_user_wrap(
				conn, 
				str_db, str_uname, str_pwd,
				detail::to_const_pointer<org_bson_type>(roles), 
				detail::to_const_pointer<org_bson_type>(custom_data), 
				boost::addressof(err));
	}

	// remove_user
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_database_remove_user_wrap(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_uname, 
									org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_uname((charset::string_charset_helper_data(str_uname)));

		return 
			base_type::s_org_database_remove_user(
				conn, utf8_str_db.data(), utf8_str_uname.data(), perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_database_remove_user(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_uname, 
								org_bson_error_type* perr = 0)
	{
		return this_type::s_database_remove_user_wrap(conn, str_db, str_uname, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_database_remove_user(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_uname, 
								org_bson_error_type& err)
	{
		return this_type::s_database_remove_user_wrap(conn, str_db, str_uname, boost::addressof(err));
	}

	// remove_all_users
private:
	template<typename Connection_T, typename String1> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_remove_all_users_wrap(Connection_T& conn, 
											const String1& str_db, 
											org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		return base_type::s_org_database_remove_all_users(conn, utf8_str_db.data(), perr);
	}

public:
	template<typename Connection_T, typename String1> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_remove_all_users(Connection_T& conn, 
									const String1& str_db, 
									org_bson_error_type* perr = 0)
	{
		return this_type::s_database_remove_all_users_wrap(conn, str_db, perr);
	}

	template<typename Connection_T, typename String1> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_database_remove_all_users(Connection_T& conn, 
									const String1& str_db, 
									org_bson_error_type& err)
	{
		return this_type::s_database_remove_all_users_wrap(conn, str_db, boost::addressof(err));
	}

	// database_watch
private:
	template<typename Connection_T,
				typename String1,
				typename HandlerCollector> inline
	static bool s_database_watch_wrap(Connection_T& conn,
										const String1& str_db,
										const org_bson_type* ppipeline,
										const org_bson_type* popts, 
										org_bson_type* preply,
										org_bson_error_type* perr,
										const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return 
			base_type::s_org_database_watch(
				conn, utf8_str_db.data(), 
				ppipeline, popts, 
				preply, perr, 
				handler_collector);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_database_watch(Connection_T& conn,
									const String1& str_db,
									const BsonPipeline& pipeline,
									const BsonOpts& opts, 
									org_bson_type* preply,
									org_bson_error_type* perr,
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_watch_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, perr, 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_database_watch(Connection_T& conn,
									const String1& str_db,
									const BsonPipeline& pipeline,
									const BsonOpts& opts, 
									org_bson_type& reply,
									org_bson_error_type* perr,
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_watch_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				perr, 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_database_watch(Connection_T& conn,
									const String1& str_db,
									const BsonPipeline& pipeline,
									const BsonOpts& opts, 
									org_bson_type* preply,
									org_bson_error_type& err,
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_watch_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, 
				boost::addressof(err), 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_database_watch(Connection_T& conn,
									const String1& str_db,
									const BsonPipeline& pipeline,
									const BsonOpts& opts, 
									org_bson_type& reply,
									org_bson_error_type& err,
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_watch_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				boost::addressof(err), 
				handler_collector);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_watch(Connection_T& conn,
						const String1& str_db,
						const BsonPipeline& pipeline,
						const BsonOpts& opts, 
						org_bson_type* preply,
						org_bson_error_type* perr,
						const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_watch_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, perr, 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_watch(Connection_T& conn,
						const String1& str_db,
						const BsonPipeline& pipeline,
						const BsonOpts& opts, 
						org_bson_type& reply,
						org_bson_error_type* perr,
						const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_watch_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				perr, 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_watch(Connection_T& conn,
						const String1& str_db,
						const BsonPipeline& pipeline,
						const BsonOpts& opts, 
						org_bson_type* preply,
						org_bson_error_type& err,
						const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_watch_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, 
				boost::addressof(err), 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_watch(Connection_T& conn,
						const String1& str_db,
						const BsonPipeline& pipeline,
						const BsonOpts& opts, 
						org_bson_type& reply,
						org_bson_error_type& err,
						const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_database_watch_wrap(
				conn, 
				str_db, 
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				boost::addressof(err), 
				handler_collector);
	}

	// database_using_handler
private:
	template<typename Connection_T, 
				typename String1,
				typename Handler > inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_database_using_handler_wrap(Connection_T& conn, 
										const String1& str_db, 
										org_bson_error_type* perr, 
										const Handler& handler)
	{
		typedef typename native_t<String1>::type string1_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return base_type::s_org_database_using_handler(conn, utf8_str_db.data(), perr, handler);
	}

public:
	template<typename Connection_T, 
				typename String1,
				typename Handler> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_database_using_handler(Connection_T& conn, 
									const String1& str_db, 
									org_bson_error_type* perr, 
									const Handler& handler)
	{
		return this_type::s_database_using_handler_wrap(conn, str_db, perr, handler);
	}

	template<typename Connection_T, 
				typename String1,
				typename Handler> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_database_using_handler(Connection_T& conn, 
									const String1& str_db, 
									org_bson_error_type& err, 
									const Handler& handler)
	{
		return this_type::s_database_using_handler_wrap(conn, str_db, boost::addressof(err), handler);
	}

public:
	template<typename Connection_T, 
				typename String1, 
				typename Handler > inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_using_handler(Connection_T& conn, 
						const String1& str_db, 
						org_bson_error_type* perr, 
						const Handler& handler)
	{
		return this_type::s_database_using_handler(conn, str_db, perr, handler);
	}

	template<typename Connection_T, 
				typename String1,
				typename Handler> inline
	static
	typename 
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_using_handler(Connection_T& conn, 
						const String1& str_db,
						org_bson_error_type& err, 
						const Handler& handler)
	{
		return this_type::s_database_using_handler(conn, str_db, err, handler);
	}

	//------------------------database e------------------------
};

} // namespace nsql_database_system
} // namespace yggr

#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_DATABASE_HPP__
