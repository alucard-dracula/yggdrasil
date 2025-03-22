//mongo_accesser_client_encryption.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_CLIENT_ENCRYPTION_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_CLIENT_ENCRYPTION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/native_t.hpp>

#include <yggr/nsql_database_system/basic_mongo_accesser_client_encryption.hpp>
#include <yggr/nsql_database_system/detail/to_pointer.hpp>

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace yggr
{
namespace nsql_database_system
{

class mongo_accesser_client_encryption
	: public basic_mongo_accesser_client_encryption
{
public:
	typedef basic_mongo_accesser_client_encryption base_type;

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
	typedef base_type::client_encryption_opts_type client_encryption_opts_type;
	typedef base_type::org_client_encryption_opts_type org_client_encryption_opts_type;

	typedef base_type::client_encryption_datakey_opts_type client_encryption_datakey_opts_type;
	typedef base_type::org_client_encryption_datakey_opts_type org_client_encryption_datakey_opts_type;

	typedef base_type::client_encryption_rewrap_many_datakey_result_type client_encryption_rewrap_many_datakey_result_type;
	typedef base_type::org_client_encryption_rewrap_many_datakey_result_type org_client_encryption_rewrap_many_datakey_result_type;

	typedef base_type::client_encryption_encrypt_opts_type client_encryption_encrypt_opts_type;
	typedef base_type::org_client_encryption_encrypt_opts_type org_client_encryption_encrypt_opts_type;

private:
	typedef mongo_accesser_client_encryption this_type;

public:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// don't coding get_param or set_params function, because that is call time used only,
	// example get_read_concern(...)
	// if you want to use that, please call foo "using_handler" execute you custom native operations
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//------------------------encryption s------------------------

	// s_client_encryption_create_connection_encryption connection shared_ptr version
	template<typename ConnEnc, typename Connection_T, typename CltEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			ConnEnc&
		>::type
		s_client_encryption_create_connection_encryption(ConnEnc& conn_enc,
															smart_ptr_ex::shared_ptr<Connection_T> pconn,
															const CltEncOpts& opts,
															org_bson_error_type* perr = 0)
	{
		client_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_encryption_create_connection_encryption(
				conn_enc, pconn, tmp_opts.org_pointer(), perr);
	}

	template<typename ConnEnc, typename Connection_T, typename CltEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			ConnEnc&
		>::type
		s_client_encryption_create_connection_encryption(ConnEnc& conn_enc,
															smart_ptr_ex::shared_ptr<Connection_T> pconn,
															const CltEncOpts& opts,
															org_bson_error_type& err)
	{
		client_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_encryption_create_connection_encryption(
				conn_enc, pconn, tmp_opts.org_pointer(), boost::addressof(err));
	}

	// s_client_encryption_create_connection_encryption connection reference version
	template<typename ConnEnc, typename Connection_T, typename CltEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			ConnEnc&
		>::type
		s_client_encryption_create_connection_encryption(ConnEnc& conn_enc,
															Connection_T& conn,
															const CltEncOpts& opts,
															org_bson_error_type* perr = 0)
	{
		client_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_encryption_create_connection_encryption(
				conn_enc, conn, tmp_opts.org_pointer(), perr);
	}

	template<typename ConnEnc, typename Connection_T, typename CltEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			ConnEnc&
		>::type
		s_client_encryption_create_connection_encryption(ConnEnc& conn_enc,
															Connection_T& conn,
															const CltEncOpts& opts,
															org_bson_error_type& err)
	{
		client_encryption_opts_type tmp_opts(opts);

		return
			base_type::s_org_client_encryption_create_connection_encryption(
				conn_enc, conn, tmp_opts.org_pointer(), boost::addressof(err));
	}

	//s_client_encryption_create_datakey
private:
	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_create_datakey_wrap(ConnEnc& conn_enc,
												const String1& str_kms_provides_mode_name,
												const org_client_encryption_datakey_opts_type* popts,
												org_bson_value_type* pkeyid,
												org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_kms_provides_mode_name((charset::string_charset_helper_data(str_kms_provides_mode_name)));

		return
			base_type::s_org_client_encryption_create_datakey(
				conn_enc,
				utf8_str_kms_provides_mode_name.data(),
				popts,
				pkeyid,
				perr);
	}

public:
	template<typename ConnEnc, typename String1, typename DatakeyOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_create_datakey(ConnEnc& conn_enc,
											const String1& str_kms_provides_mode_name,
											const DatakeyOpts& popts,
											org_bson_value_type* pkeyid,
											org_bson_error_type* perr = 0)
	{
		return
			this_type::s_client_encryption_create_datakey_wrap(
				conn_enc, str_kms_provides_mode_name,
				detail::to_const_pointer<org_client_encryption_datakey_opts_type>(popts),
				pkeyid, perr);
	}

	template<typename ConnEnc, typename String1, typename DatakeyOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_create_datakey(ConnEnc& conn_enc,
											const String1& str_kms_provides_mode_name,
											const DatakeyOpts& popts,
											org_bson_value_type& keyid,
											org_bson_error_type* perr = 0)
	{
		return
			this_type::s_client_encryption_create_datakey_wrap(
				conn_enc, str_kms_provides_mode_name,
				detail::to_const_pointer<org_client_encryption_datakey_opts_type>(popts),
				boost::addressof(keyid), perr);
	}

	template<typename ConnEnc, typename String1,
				typename DatakeyOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_create_datakey(ConnEnc& conn_enc,
											const String1& str_kms_provides_mode_name,
											const DatakeyOpts& popts,
											org_bson_value_type* pkeyid,
											org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_create_datakey_wrap(
				conn_enc, str_kms_provides_mode_name,
				detail::to_const_pointer<org_client_encryption_datakey_opts_type>(popts),
				pkeyid, boost::addressof(err));
	}

	template<typename ConnEnc, typename String1, typename DatakeyOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_create_datakey(ConnEnc& conn_enc,
											const String1& str_kms_provides_mode_name,
											const DatakeyOpts& popts,
											org_bson_value_type& keyid,
											org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_create_datakey_wrap(
				conn_enc, str_kms_provides_mode_name,
				detail::to_const_pointer<org_client_encryption_datakey_opts_type>(popts),
				boost::addressof(keyid), boost::addressof(err));
	}

	// s_client_encryption_create_encrypted_collection
private:
	template<typename ConnEnc,
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
		s_client_encryption_create_encrypted_collection_wrap(ConnEnc& conn_enc,
																const String1& str_db,
																const String2& str_coln,
																const org_bson_type* popts,
																const String3& kms_provider,
																const org_bson_type* popt_master_key,
																org_bson_type* pout_popts,
																org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));
		utf8_string3_type utf8_kms_provider((charset::string_charset_helper_data(kms_provider)));

		return
			base_type::s_org_client_encryption_create_encrypted_collection(
				conn_enc,
				utf8_str_db.data(), utf8_str_coln.data(),
				popts, utf8_kms_provider.data(), popt_master_key,
				pout_popts, perr);
	}

	template<typename ConnEnc, typename Connection_T,
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
		s_client_encryption_create_encrypted_collection_wrap(ConnEnc& conn_enc,
																Connection_T& conn,
																const String1& str_db,
																const String2& str_coln,
																const org_bson_type* popts,
																const String3& kms_provider,
																const org_bson_type* popt_master_key,
																org_bson_type* pout_popts,
																org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));
		utf8_string3_type utf8_kms_provider((charset::string_charset_helper_data(kms_provider)));

		return
			base_type::s_org_client_encryption_create_encrypted_collection(
				conn_enc, conn,
				utf8_str_db.data(), utf8_str_coln.data(),
				popts, utf8_kms_provider.data(), popt_master_key,
				pout_popts, perr);
	}

public:
	// no connection_t version
	template<typename ConnEnc,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				0,
				0);
	}

	template<typename ConnEnc,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type* pout_popts)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				pout_popts, 0);
	}

	template<typename ConnEnc,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type& out_popts)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				boost::addressof(out_popts),
				0);
	}

	template<typename ConnEnc,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				0,
				perr);
	}

	template<typename ConnEnc,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				0,
				boost::addressof(err));
	}

	template<typename ConnEnc,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type* pout_popts,
														org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				pout_popts,
				perr);
	}

	template<typename ConnEnc,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type& out_popts,
														org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				boost::addressof(out_popts),
				perr);
	}

	template<typename ConnEnc,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type* pout_popts,
														org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				pout_popts,
				boost::addressof(err));
	}

	template<typename ConnEnc,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type& out_popts,
														org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				boost::addressof(out_popts),
				boost::addressof(err));
	}

	// connection_t version
	template<typename ConnEnc, typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														Connection_T& conn,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc, conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				0,
				0);
	}

	template<typename ConnEnc, typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														Connection_T& conn,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type* pout_popts)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc, conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				pout_popts, 0);
	}

	template<typename ConnEnc, typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														Connection_T& conn,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type& out_popts)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc, conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				boost::addressof(out_popts),
				0);
	}

	template<typename ConnEnc, typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														Connection_T& conn,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc, conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				0,
				perr);
	}

	template<typename ConnEnc, typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														Connection_T& conn,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc, conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				0,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														Connection_T& conn,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type* pout_popts,
														org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc, conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				pout_popts,
				perr);
	}

	template<typename ConnEnc, typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														Connection_T& conn,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type& out_popts,
														org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc, conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				boost::addressof(out_popts),
				perr);
	}

	template<typename ConnEnc, typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														Connection_T& conn,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type* pout_popts,
														org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc, conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				pout_popts,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts, typename BsonOptMasterKey> inline
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
		s_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
														Connection_T& conn,
														const String1& str_db,
														const String2& str_coln,
														const BsonOpts& opts,
														const String3& kms_provider,
														const BsonOptMasterKey& opt_master_key,
														org_bson_type& out_popts,
														org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_create_encrypted_collection_wrap(
				conn_enc, conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(opt_master_key),
				boost::addressof(out_popts),
				boost::addressof(err));
	}

	// s_client_encryption_rewrap_many_datakey

private:
	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			boost::is_integral<String1>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey_wrap(ConnEnc& conn_enc,
														const org_bson_type* pfilter,
														const String1& kms_provider,
														const org_bson_type* pmaster_key,
														org_client_encryption_rewrap_many_datakey_result_type* presult,
														org_bson_error_type* perr)
	{
		assert(kms_provider == 0);

		return
			base_type::s_org_client_encryption_rewrap_many_datakey(
				conn_enc,
				pfilter,
				static_cast<const char*>(0),
				pmaster_key,
				presult,
				perr);
	}

	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_integral<String1> >,
				charset::utf8_string_constructable<String1>
			>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey_wrap(ConnEnc& conn_enc,
														const org_bson_type* pfilter,
														const String1& kms_provider,
														const org_bson_type* pmaster_key,
														org_client_encryption_rewrap_many_datakey_result_type* presult,
														org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_kms_provider((charset::string_charset_helper_data(kms_provider)));

		return
			base_type::s_org_client_encryption_rewrap_many_datakey(
				conn_enc,
				pfilter,
				(utf8_kms_provider.size()? utf8_kms_provider.data() : 0),
				pmaster_key,
				presult,
				perr);
	}

public:
	template<typename ConnEnc,
				typename BsonFilter,
				typename String1,
				typename BsonMasterKey> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc,
												const BsonFilter& bs_filter,
												const String1& kms_provider,
												const BsonMasterKey& bs_master_key)
	{
		return
			this_type::s_client_encryption_rewrap_many_datakey_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(bs_filter),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(bs_master_key),
				0,
				0);
	}

	template<typename ConnEnc,
				typename BsonFilter,
				typename String1,
				typename BsonMasterKey> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc,
												const BsonFilter& bs_filter,
												const String1& kms_provider,
												const BsonMasterKey& bs_master_key,
												org_client_encryption_rewrap_many_datakey_result_type* presult)
	{
		return
			this_type::s_client_encryption_rewrap_many_datakey_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(bs_filter),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(bs_master_key),
				presult,
				0);
	}

	template<typename ConnEnc,
				typename BsonFilter,
				typename String1,
				typename BsonMasterKey,
				typename RewrapManyDatakeyResult> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::mpl::not_< boost::is_base_of<org_bson_error_type, RewrapManyDatakeyResult> >
			>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc,
												const BsonFilter& bs_filter,
												const String1& kms_provider,
												const BsonMasterKey& bs_master_key,
												RewrapManyDatakeyResult& result)
	{
		return
			this_type::s_client_encryption_rewrap_many_datakey_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(bs_filter),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(bs_master_key),
				reinterpret_cast<org_client_encryption_rewrap_many_datakey_result_type*>(boost::addressof(result)),
				0);
	}

	template<typename ConnEnc,
				typename BsonFilter,
				typename String1,
				typename BsonMasterKey> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc,
												const BsonFilter& bs_filter,
												const String1& kms_provider,
												const BsonMasterKey& bs_master_key,
												org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_rewrap_many_datakey_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(bs_filter),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(bs_master_key),
				0,
				perr);
	}

	template<typename ConnEnc,
				typename BsonFilter,
				typename String1,
				typename BsonMasterKey> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc,
												const BsonFilter& bs_filter,
												const String1& kms_provider,
												const BsonMasterKey& bs_master_key,
												org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_rewrap_many_datakey_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(bs_filter),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(bs_master_key),
				0,
				boost::addressof(err));
	}

	template<typename ConnEnc,
				typename BsonFilter,
				typename String1,
				typename BsonMasterKey> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc,
												const BsonFilter& bs_filter,
												const String1& kms_provider,
												const BsonMasterKey& bs_master_key,
												org_client_encryption_rewrap_many_datakey_result_type* presult,
												org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_rewrap_many_datakey_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(bs_filter),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(bs_master_key),
				presult,
				perr);
	}

	template<typename ConnEnc,
				typename BsonFilter,
				typename String1,
				typename BsonMasterKey,
				typename RewrapManyDatakeyResult> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc,
												const BsonFilter& bs_filter,
												const String1& kms_provider,
												const BsonMasterKey& bs_master_key,
												RewrapManyDatakeyResult& result,
												org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_rewrap_many_datakey_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(bs_filter),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(bs_master_key),
				reinterpret_cast<org_client_encryption_rewrap_many_datakey_result_type*>(boost::addressof(result)),
				perr);
	}

	template<typename ConnEnc,
				typename BsonFilter,
				typename String1,
				typename BsonMasterKey> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc,
												const BsonFilter& bs_filter,
												const String1& kms_provider,
												const BsonMasterKey& bs_master_key,
												org_client_encryption_rewrap_many_datakey_result_type* presult,
												org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_rewrap_many_datakey_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(bs_filter),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(bs_master_key),
				presult,
				boost::addressof(err));
	}

	template<typename ConnEnc,
				typename BsonFilter,
				typename String1,
				typename BsonMasterKey,
				typename RewrapManyDatakeyResult> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc,
												const BsonFilter& bs_filter,
												const String1& kms_provider,
												const BsonMasterKey& bs_master_key,
												RewrapManyDatakeyResult& result,
												org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_rewrap_many_datakey_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(bs_filter),
				kms_provider,
				detail::to_const_pointer<org_bson_type>(bs_master_key),
				reinterpret_cast<org_client_encryption_rewrap_many_datakey_result_type*>(boost::addressof(result)),
				boost::addressof(err));
	}

	// s_client_encryption_delete_key
	template<typename ConnEnc, typename BsonValKeyID> inline
	static bool s_client_encryption_delete_key(ConnEnc& conn_enc,
												const BsonValKeyID& keyid)
	{
		return
			base_type::s_org_client_encryption_delete_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				0,
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static bool s_client_encryption_delete_key(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												org_bson_type* preply)
	{
		return
			base_type::s_org_client_encryption_delete_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				preply,
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static bool s_client_encryption_delete_key(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												org_bson_type& reply)
	{
		return
			base_type::s_org_client_encryption_delete_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				boost::addressof(reply),
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static bool s_client_encryption_delete_key(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												org_bson_error_type* perr)
	{
		return
			base_type::s_org_client_encryption_delete_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				0,
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static bool s_client_encryption_delete_key(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												org_bson_error_type& err)
	{
		return
			base_type::s_org_client_encryption_delete_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				0,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static bool s_client_encryption_delete_key(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												org_bson_type* preply,
												org_bson_error_type* perr)
	{
		return
			base_type::s_org_client_encryption_delete_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				preply,
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static bool s_client_encryption_delete_key(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												org_bson_type& reply,
												org_bson_error_type* perr)
	{
		return
			base_type::s_org_client_encryption_delete_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				boost::addressof(reply),
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static bool s_client_encryption_delete_key(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												org_bson_type* preply,
												org_bson_error_type& err)
	{
		return
			base_type::s_org_client_encryption_delete_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				preply,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static bool s_client_encryption_delete_key(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												org_bson_type& reply,
												org_bson_error_type& err)
	{
		return
			base_type::s_org_client_encryption_delete_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				boost::addressof(reply),
				boost::addressof(err));
	}

	// s_client_encryption_get_key // bson_value keyid version
	template<typename ConnEnc, typename BsonValKeyID> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValKeyID>::type >,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc, const BsonValKeyID& keyid)
	{
		return
			base_type::s_org_client_encryption_get_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				0,
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValKeyID>::type >,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const BsonValKeyID& keyid,
									org_bson_type* pkey_doc)
	{
		return
			base_type::s_org_client_encryption_get_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				pkey_doc,
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<org_bson_value_type, typename native_t<BsonValKeyID>::type >,
				boost::is_base_of<org_bson_type, BsonKeyDoc>
			>,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
											const BsonValKeyID& keyid,
											BsonKeyDoc& key_doc)
	{
		return
			base_type::s_org_client_encryption_get_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				boost::addressof(key_doc),
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValKeyID>::type >,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const BsonValKeyID& keyid,
									org_bson_error_type* perr)
	{
		return
			base_type::s_org_client_encryption_get_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				0,
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValKeyID>::type >,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const BsonValKeyID& keyid,
									org_bson_error_type& err)
	{
		return
			base_type::s_org_client_encryption_get_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				0,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValKeyID>::type >,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const BsonValKeyID& keyid,
									org_bson_type* pkey_doc,
									org_bson_error_type* perr)
	{
		return
			base_type::s_org_client_encryption_get_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				pkey_doc,
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValKeyID>::type >,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const BsonValKeyID& keyid,
									BsonKeyDoc& key_doc,
									org_bson_error_type* perr)
	{
		return
			base_type::s_org_client_encryption_get_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				boost::addressof(key_doc),
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValKeyID>::type >,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const BsonValKeyID& keyid,
									org_bson_type* pkey_doc,
									org_bson_error_type& err)
	{
		return
			base_type::s_org_client_encryption_get_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				pkey_doc,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonValKeyID, typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValKeyID>::type >,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const BsonValKeyID& keyid,
									BsonKeyDoc& key_doc,
									org_bson_error_type& err)
	{
		return
			base_type::s_org_client_encryption_get_key(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				boost::addressof(key_doc),
				boost::addressof(err));
	}

	// s_client_encryption_get_key // string keyaltname version
protected:
	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_get_key_by_alt_name_wrap(ConnEnc& conn_enc,
														const String1& keyaltname,
														org_bson_type* pkey_doc,
														org_bson_error_type* perr)
	{
		typedef String1 string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_keyaltname((charset::string_charset_helper_data(keyaltname)));

		return
			base_type::s_org_client_encryption_get_key_by_alt_name(
				conn_enc,
				utf8_keyaltname.data(),
				pkey_doc,
				perr);
	}

public:
	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc, const String1& keyaltname)
	{
		return
			this_type::s_client_encryption_get_key_by_alt_name_wrap(
				conn_enc,
				keyaltname,
				0,
				0);
	}

	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const String1& keyaltname,
									org_bson_type* pkey_doc)
	{
		return
			this_type::s_client_encryption_get_key_by_alt_name_wrap(
				conn_enc,
				keyaltname,
				pkey_doc,
				0);
	}

	template<typename ConnEnc, typename String1, typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, BsonKeyDoc>
			>,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
											const String1& keyaltname,
											BsonKeyDoc& key_doc)
	{
		return
			this_type::s_client_encryption_get_key_by_alt_name_wrap(
				conn_enc,
				keyaltname,
				boost::addressof(key_doc),
				0);
	}

	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const String1& keyaltname,
									org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_get_key_by_alt_name_wrap(
				conn_enc,
				keyaltname,
				0,
				perr);
	}

	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const String1& keyaltname,
									org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_get_key_by_alt_name_wrap(
				conn_enc,
				keyaltname,
				0,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const String1& keyaltname,
									org_bson_type* pkey_doc,
									org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_get_key_by_alt_name_wrap(
				conn_enc,
				keyaltname,
				pkey_doc,
				perr);
	}

	template<typename ConnEnc, typename String1, typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const String1& keyaltname,
									BsonKeyDoc& key_doc,
									org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_get_key_by_alt_name_wrap(
				conn_enc,
				keyaltname,
				boost::addressof(key_doc),
				perr);
	}

	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const String1& keyaltname,
									org_bson_type* pkey_doc,
									org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_get_key_by_alt_name_wrap(
				conn_enc,
				keyaltname,
				pkey_doc,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename String1, typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_get_key(ConnEnc& conn_enc,
									const String1& keyaltname,
									BsonKeyDoc& key_doc,
									org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_get_key_by_alt_name_wrap(
				conn_enc,
				keyaltname,
				boost::addressof(key_doc),
				boost::addressof(err));
	}

	// s_client_encryption_get_keys
	template<typename ConnEnc, typename HandlerCollector> inline
	static bool s_client_encryption_get_keys(ConnEnc& conn_enc,
												const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_encryption_get_keys(
				conn_enc, 0, handler_collector);
	}

	template<typename ConnEnc, typename HandlerCollector> inline
	static bool s_client_encryption_get_keys(ConnEnc& conn_enc,
												org_bson_error_type* perr,
												const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_encryption_get_keys(
				conn_enc, perr, handler_collector);
	}

	template<typename ConnEnc, typename HandlerCollector> inline
	static bool s_client_encryption_get_keys(ConnEnc& conn_enc,
												org_bson_error_type& err,
												const HandlerCollector& handler_collector)
	{
		return
			base_type::s_org_client_encryption_get_keys(
				conn_enc, boost::addressof(err), handler_collector);
	}

public:
	// s_client_encryption_get_crypt_shared_version return inner_string_type
	template<typename ConnEnc> inline
	static inner_string_type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc)
	{
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, 0);
	}

	template<typename ConnEnc> inline
	static inner_string_type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const string& ret_charset_name)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, 0);
	}

	template<typename ConnEnc> inline
	static inner_string_type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, org_bson_error_type* perr)
	{
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, perr);
	}

	template<typename ConnEnc> inline
	static inner_string_type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc,
											const string& ret_charset_name,
											org_bson_error_type* perr)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, perr);
	}

	template<typename ConnEnc> inline
	static inner_string_type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, org_bson_error_type& err)
	{
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, boost::addressof(err));
	}

	template<typename ConnEnc> inline
	static inner_string_type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc,
											const string& ret_charset_name,
											org_bson_error_type& err)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, boost::addressof(err));
	}

	// s_client_encryption_get_crypt_shared_version return inner_string_type allocator ver
	template<typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			inner_string_type
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const Alloc& alloc)
	{
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, alloc, 0);
	}

	template<typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			inner_string_type
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const Alloc& alloc, const string& ret_charset_name)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, alloc, 0);
	}

	template<typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			inner_string_type
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const Alloc& alloc, org_bson_error_type* perr)
	{
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, alloc, perr);
	}

	template<typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			inner_string_type
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc,
														const Alloc& alloc,
														const string& ret_charset_name,
														org_bson_error_type* perr)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, alloc, perr);
	}

	template<typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			inner_string_type
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const Alloc& alloc, org_bson_error_type& err)
	{
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, alloc, boost::addressof(err));
	}

	template<typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			inner_string_type
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc,
														const Alloc& alloc,
														const string& ret_charset_name,
														org_bson_error_type& err)
	{
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, alloc, boost::addressof(err));
	}

	// s_client_encryption_get_crypt_shared_version return other_string
	template<typename String, typename ConnEnc> inline
	static String
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc)
	{
		typedef String now_string_type;
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, 0).template str<now_string_type>();
	}

	template<typename String, typename ConnEnc> inline
	static String
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const string& ret_charset_name)
	{
		typedef String now_string_type;
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, 0).template str<now_string_type>(ret_charset_name);
	}

	template<typename String, typename ConnEnc> inline
	static String
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, org_bson_error_type* perr)
	{
		typedef String now_string_type;
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, perr).template str<now_string_type>();
	}

	template<typename String, typename ConnEnc> inline
	static String
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc,
														const string& ret_charset_name,
														org_bson_error_type* perr)
	{
		typedef String now_string_type;
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, perr).template str<now_string_type>(ret_charset_name);
	}

	template<typename String, typename ConnEnc> inline
	static String
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, org_bson_error_type& err)
	{
		typedef String now_string_type;
		return
			base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, boost::addressof(err))
				.template str<now_string_type>();
	}

	template<typename String, typename ConnEnc> inline
	static String
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc,
														const string& ret_charset_name,
														org_bson_error_type& err)
	{
		typedef String now_string_type;
		return
			base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, boost::addressof(err))
				.template str<now_string_type>(ret_charset_name);
	}

	// s_client_encryption_get_crypt_shared_version return other_string allocator ver
	template<typename String, typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			String
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const Alloc& alloc)
	{
		typedef String now_string_type;
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, 0).template str<now_string_type>(alloc);
	}

	template<typename String, typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			String
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const Alloc& alloc, const string& ret_charset_name)
	{
		typedef String now_string_type;
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, 0).template str<now_string_type>(alloc, ret_charset_name);
	}

	template<typename String, typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			String
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const Alloc& alloc, org_bson_error_type* perr)
	{
		typedef String now_string_type;
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, perr).template str<now_string_type>(alloc);
	}

	template<typename String, typename Alloc, typename ConnEnc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			String
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc,
														const Alloc& alloc,
														const string& ret_charset_name,
														org_bson_error_type* perr)
	{
		typedef String now_string_type;
		return base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, perr).template str<now_string_type>(alloc, ret_charset_name);
	}

	template<typename String, typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			String
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const Alloc& alloc, org_bson_error_type& err)
	{
		typedef String now_string_type;
		return
			base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, boost::addressof(err))
				.template str<now_string_type>(alloc);
	}

	template<typename String, typename ConnEnc, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			String
		>::type
		s_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc,
														const Alloc& alloc,
														const string& ret_charset_name,
														org_bson_error_type& err)
	{
		typedef String now_string_type;
		return
			base_type::s_org_client_encryption_get_crypt_shared_version(conn_enc, boost::addressof(err))
				.template str<now_string_type>(alloc, ret_charset_name);
	}

	// s_client_encryption_add_key_alt_name
protected:
	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_add_key_alt_name_wrap(ConnEnc& conn_enc,
													const org_bson_value_type* pkeyid,
													const String1& keyaltname,
													org_bson_type* pkey_doc,
													org_bson_error_type* perr)
	{
		typedef String1 string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_keyaltname((charset::string_charset_helper_data(keyaltname)));

		return
			base_type::s_org_client_encryption_add_key_alt_name(
				conn_enc, pkeyid, utf8_keyaltname.data(), pkey_doc, perr);

	}

public:
	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_add_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname)
	{
		return
			this_type::s_client_encryption_add_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				0,
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_add_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												org_bson_type* pkey_doc)
	{
		return
			this_type::s_client_encryption_add_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				pkey_doc,
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1,
				typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, BsonKeyDoc>
			>,
			bool
		>::type
		s_client_encryption_add_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												BsonKeyDoc& key_doc)
	{
		return
			this_type::s_client_encryption_add_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				boost::addressof(key_doc),
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_add_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_add_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				0,
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_add_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_add_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				0,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_add_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												org_bson_type* pkey_doc,
												org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_add_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				pkey_doc,
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1,
				typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_add_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												BsonKeyDoc& key_doc,
												org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_add_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				boost::addressof(key_doc),
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_add_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												org_bson_type* pkey_doc,
												org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_add_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				pkey_doc,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1,
				typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_add_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												BsonKeyDoc& key_doc,
												org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_add_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				boost::addressof(key_doc),
				boost::addressof(err));
	}

	// s_client_encryption_remove_key_alt_name
protected:
	template<typename ConnEnc, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_remove_key_alt_name_wrap(ConnEnc& conn_enc,
													const org_bson_value_type* pkeyid,
													const String1& keyaltname,
													org_bson_type* pkey_doc,
													org_bson_error_type* perr)
	{
		typedef String1 string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_keyaltname((charset::string_charset_helper_data(keyaltname)));

		return
			base_type::s_org_client_encryption_remove_key_alt_name(
				conn_enc, pkeyid, utf8_keyaltname.data(), pkey_doc, perr);

	}

public:
	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_remove_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname)
	{
		return
			this_type::s_client_encryption_remove_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				0,
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_remove_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												org_bson_type* pkey_doc)
	{
		return
			this_type::s_client_encryption_remove_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				pkey_doc,
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1,
				typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				boost::is_base_of<org_bson_type, BsonKeyDoc>
			>,
			bool
		>::type
		s_client_encryption_remove_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												BsonKeyDoc& key_doc)
	{
		return
			this_type::s_client_encryption_remove_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				boost::addressof(key_doc),
				0);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_remove_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_remove_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				0,
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_remove_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_remove_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				0,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_remove_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												org_bson_type* pkey_doc,
												org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_remove_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				pkey_doc,
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1,
				typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_remove_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												BsonKeyDoc& key_doc,
												org_bson_error_type* perr)
	{
		return
			this_type::s_client_encryption_remove_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				boost::addressof(key_doc),
				perr);
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_remove_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												org_bson_type* pkey_doc,
												org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_remove_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				pkey_doc,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonValKeyID, typename String1,
				typename BsonKeyDoc> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_client_encryption_remove_key_alt_name(ConnEnc& conn_enc,
												const BsonValKeyID& keyid,
												const String1& keyaltname,
												BsonKeyDoc& key_doc,
												org_bson_error_type& err)
	{
		return
			this_type::s_client_encryption_remove_key_alt_name_wrap(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(keyid),
				keyaltname,
				boost::addressof(key_doc),
				boost::addressof(err));
	}

	// s_client_encryption_encrypt
	template<typename ConnEnc, typename BsonVal, typename CltEncEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonVal>::type>,
			bool
		>::type
		s_client_encryption_encrypt(ConnEnc& conn_enc,
									const BsonVal& val,
									const CltEncEncOpts& opts,
									org_bson_value_type* pciphertext, // null "required 'ciphertext' unset"
									org_bson_error_type* perr = 0)
	{
		client_encryption_encrypt_opts_type arg_opts(opts);

		return
			base_type::s_org_client_encryption_encrypt(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(val),
				arg_opts.org_pointer(),
				pciphertext,
				perr);
	}

	template<typename ConnEnc, typename BsonVal, typename CltEncEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonVal>::type>,
			bool
		>::type
		s_client_encryption_encrypt(ConnEnc& conn_enc,
									const BsonVal& val,
									const CltEncEncOpts& opts,
									org_bson_value_type& ciphertext,
									org_bson_error_type* perr = 0)
	{
		client_encryption_encrypt_opts_type arg_opts(opts);

		return
			base_type::s_org_client_encryption_encrypt(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(val),
				arg_opts.org_pointer(),
				boost::addressof(ciphertext),
				perr);
	}

	template<typename ConnEnc, typename BsonVal, typename CltEncEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonVal>::type>,
			bool
		>::type
		s_client_encryption_encrypt(ConnEnc& conn_enc,
									const BsonVal& val,
									const CltEncEncOpts& opts,
									org_bson_value_type* pciphertext,
									org_bson_error_type& err)
	{
		client_encryption_encrypt_opts_type arg_opts(opts);

		return
			base_type::s_org_client_encryption_encrypt(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(val),
				arg_opts.org_pointer(),
				pciphertext,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonVal, typename CltEncEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonVal>::type>,
			bool
		>::type
		s_client_encryption_encrypt(ConnEnc& conn_enc,
									const BsonVal& val,
									const CltEncEncOpts& opts,
									org_bson_value_type& ciphertext,
									org_bson_error_type& err)
	{
		client_encryption_encrypt_opts_type arg_opts(opts);

		return
			base_type::s_org_client_encryption_encrypt(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(val),
				arg_opts.org_pointer(),
				boost::addressof(ciphertext),
				boost::addressof(err));
	}

	// s_client_encryption_encrypt_expression
	template<typename ConnEnc, typename BsonExpr, typename CltEncEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_type, typename native_t<BsonExpr>::type>,
			bool
		>::type
		s_client_encryption_encrypt(ConnEnc& conn_enc,
									const BsonExpr& expr,
									const CltEncEncOpts& opts,
									org_bson_type* pexpr_out,
									org_bson_error_type* perr = 0)
	{
		client_encryption_encrypt_opts_type arg_opts(opts);

		return
			base_type::s_org_client_encryption_encrypt_expression(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(expr),
				arg_opts.org_pointer(),
				pexpr_out,
				perr);
	}

	template<typename ConnEnc, typename BsonExpr, typename CltEncEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_type, typename native_t<BsonExpr>::type>,
			bool
		>::type
		s_client_encryption_encrypt(ConnEnc& conn_enc,
									const BsonExpr& expr,
									const CltEncEncOpts& opts,
									org_bson_type& expr_out,
									org_bson_error_type* perr = 0)
	{
		client_encryption_encrypt_opts_type arg_opts(opts);

		return
			base_type::s_org_client_encryption_encrypt_expression(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(expr),
				arg_opts.org_pointer(),
				boost::addressof(expr_out),
				perr);
	}

	template<typename ConnEnc, typename BsonExpr, typename CltEncEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_type, typename native_t<BsonExpr>::type>,
			bool
		>::type
		s_client_encryption_encrypt(ConnEnc& conn_enc,
									const BsonExpr& expr,
									const CltEncEncOpts& opts,
									org_bson_type* pexpr_out,
									org_bson_error_type& err)
	{
		client_encryption_encrypt_opts_type arg_opts(opts);

		return
			base_type::s_org_client_encryption_encrypt_expression(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(expr),
				arg_opts.org_pointer(),
				pexpr_out,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonExpr, typename CltEncEncOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_type, typename native_t<BsonExpr>::type>,
			bool
		>::type
		s_client_encryption_encrypt(ConnEnc& conn_enc,
									const BsonExpr& expr,
									const CltEncEncOpts& opts,
									org_bson_type& expr_out,
									org_bson_error_type& err)
	{
		client_encryption_encrypt_opts_type arg_opts(opts);

		return
			base_type::s_org_client_encryption_encrypt_expression(
				conn_enc,
				detail::to_const_pointer<org_bson_type>(expr),
				arg_opts.org_pointer(),
				boost::addressof(expr_out),
				boost::addressof(err));
	}

	// s_client_encryption_decrypt
	template<typename ConnEnc, typename BsonValCipherText> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValCipherText>::type>,
			bool
		>::type
		s_client_encryption_decrypt(ConnEnc& conn_enc,
									const BsonValCipherText& ciphertext,
									org_bson_value_type* pval, // null "required 'value' unset"
									org_bson_error_type* perr = 0)
	{
		return
			base_type::s_org_client_encryption_decrypt(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(ciphertext),
				pval,
				perr);
	}

	template<typename ConnEnc, typename BsonValCipherText> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValCipherText>::type>,
			bool
		>::type
		s_client_encryption_decrypt(ConnEnc& conn_enc,
									const BsonValCipherText& ciphertext,
									org_bson_value_type& val,
									org_bson_error_type* perr = 0)
	{
		return
			base_type::s_org_client_encryption_decrypt(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(ciphertext),
				boost::addressof(val),
				perr);
	}

	template<typename ConnEnc, typename BsonValCipherText> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValCipherText>::type>,
			bool
		>::type
		s_client_encryption_decrypt(ConnEnc& conn_enc,
									const BsonValCipherText& ciphertext,
									org_bson_value_type* pval, // null "required 'value' unset"
									org_bson_error_type& err)
	{
		return
			base_type::s_org_client_encryption_decrypt(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(ciphertext),
				pval,
				boost::addressof(err));
	}

	template<typename ConnEnc, typename BsonValCipherText> inline
	static
	typename
		boost::enable_if
		<
			boost::is_base_of<org_bson_value_type, typename native_t<BsonValCipherText>::type>,
			bool
		>::type
		s_client_encryption_decrypt(ConnEnc& conn_enc,
									const BsonValCipherText& ciphertext,
									org_bson_value_type& val,
									org_bson_error_type& err)
	{
		return
			base_type::s_org_client_encryption_decrypt(
				conn_enc,
				detail::to_const_pointer<org_bson_value_type>(ciphertext),
				boost::addressof(val),
				boost::addressof(err));
	}

	//------------------------encryption e------------------------
};

} // namespace nsql_database_system
} // namespace yggr

#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_CLIENT_ENCRYPTION_HPP__
