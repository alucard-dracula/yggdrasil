// basic_mongo_accesser_client_encryption.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_CLIENT_ENCRYPTION_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_CLIENT_ENCRYPTION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/mplex/pointer_to_value_t.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/smart_ptr_ex/nonfree_deleter.hpp>

#include <yggr/range_ex/range_value_ex.hpp>

#include <yggr/nsql_database_system/mongo_accesser_types_decl.hpp>
#include <yggr/nsql_database_system/c_mongo_client_encryption_opts.hpp>
#include <yggr/nsql_database_system/c_mongo_client_encryption_datakey_opts.hpp>
#include <yggr/nsql_database_system/c_mongo_client_encryption_encrypt_opts.hpp>
#include <yggr/nsql_database_system/c_mongo_client_encryption_rewrap_many_datakey_result.hpp>

#include <yggr/nsql_database_system/detail/get_org_t.hpp>
#include <yggr/nsql_database_system/detail/mongoc_client_encryption_deleter.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace nsql_database_system
{

class basic_mongo_accesser_client_encryption
{
public:
	typedef mongo_accesser_types_decl types_decl_type;

public:
	typedef types_decl_type::inner_string_type inner_string_type;
	typedef types_decl_type::inner_string_view_type inner_string_view_type;

	typedef types_decl_type::bson_type bson_type;
	typedef types_decl_type::org_bson_type org_bson_type;

	typedef types_decl_type::bson_value_type bson_value_type;
	typedef types_decl_type::org_bson_value_type org_bson_value_type;

	typedef types_decl_type::bson_error_type bson_error_type;
	typedef types_decl_type::org_bson_error_type org_bson_error_type;

	typedef types_decl_type::write_concern_type write_concern_type;
	typedef types_decl_type::org_write_concern_type org_write_concern_type;

	typedef types_decl_type::read_prefs_type read_prefs_type;
	typedef types_decl_type::org_read_prefs_type org_read_prefs_type;

	typedef types_decl_type::index_model_type index_model_type;
	typedef types_decl_type::org_index_model_type org_index_model_type;

	typedef types_decl_type::find_and_modify_opts_type find_and_modify_opts_type;
	typedef types_decl_type::org_find_and_modify_opts_type org_find_and_modify_opts_type;

	typedef types_decl_type::read_concern_type read_concern_type;
	typedef types_decl_type::org_read_concern_type org_read_concern_type;

	typedef types_decl_type::mongoc_error_domain_type mongoc_error_domain_type;
	typedef types_decl_type::mongoc_error_code_type mongoc_error_code_type;

public:
	typedef c_mongo_client_encryption_opts client_encryption_opts_type;
	typedef client_encryption_opts_type::org_type org_client_encryption_opts_type;

	typedef c_mongo_client_encryption_datakey_opts client_encryption_datakey_opts_type;
	typedef client_encryption_datakey_opts_type::org_type org_client_encryption_datakey_opts_type;

	typedef c_mongo_client_encryption_rewrap_many_datakey_result client_encryption_rewrap_many_datakey_result_type;
	typedef client_encryption_rewrap_many_datakey_result_type::org_type org_client_encryption_rewrap_many_datakey_result_type;

	typedef c_mongo_client_encryption_encrypt_opts client_encryption_encrypt_opts_type;
	typedef client_encryption_encrypt_opts_type::org_type org_client_encryption_encrypt_opts_type;

private:
	typedef basic_mongo_accesser_client_encryption this_type;

	//------------------------encryption s------------------------

	// s_org_client_encryption_create_connection_encryption
protected:
	template<typename ConnSharedPtr, typename CltEncShardPtr>
	static bool pro_s_handler_encryption_create_connect_encryption(ConnSharedPtr& pconn_out,
																	CltEncShardPtr& pclt_enc_out,
																	ConnSharedPtr pconn,
																	org_client_encryption_opts_type* popts,
																	org_bson_error_type* perr)
	{
		typedef CltEncShardPtr mongoc_client_encryption_ptr_type;
		typedef typename mplex::pointer_to_value_t<mongoc_client_encryption_ptr_type>::type org_mongoc_client_encryption_type;
		typedef detail::mongoc_client_encryption_deleter<org_mongoc_client_encryption_type> mongoc_client_encryption_deleter_type;

		assert(pconn && popts);
		if(!(pconn && popts))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_create_connection_encryption, command invalid args");
			}

			return false;
		}

		if((pconn_out || pclt_enc_out))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_create_connection_encryption, mongo connection was used");
			}

			return false;
		}

		org_mongoc_client_encryption_type* penc = mongoc_client_encryption_new(popts, perr);

		if(!penc)
		{
			return false;
		}

		mongoc_client_encryption_ptr_type ptmp_clt_enc(penc, mongoc_client_encryption_deleter_type());

		assert(ptmp_clt_enc);

		pclt_enc_out.swap(ptmp_clt_enc);
		pconn_out = pconn;

		return (pconn_out && pclt_enc_out);
	}

public:
	template<typename ConnEnc, typename Connection_T>
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			ConnEnc&
		>::type
		s_org_client_encryption_create_connection_encryption(ConnEnc& conn_enc,
																smart_ptr_ex::shared_ptr<Connection_T> pconn,
																org_client_encryption_opts_type* popts,
																org_bson_error_type* perr)
	{
		typedef ConnEnc connection_encryption_type;
		typedef smart_ptr_ex::shared_ptr<mongoc_client_encryption_t> mongoc_client_encryption_ptr_type;

		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type client_ptr_type;
		typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(pconn && popts))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_create_connection_encryption, command invalid args");
			}

			return conn_enc;
		}

		client_ptr_type pclt = pconn->pop();

		if(!pclt)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_create_connection_encryption, mongoc_client is not ready");
			}

			return conn_enc;
		}

		mongoc_client_encryption_opts_set_keyvault_client(popts, pclt.get());

		if(!conn_enc.init(
				boost::bind(
					&this_type::pro_s_handler_encryption_create_connect_encryption<connection_ptr_type, mongoc_client_encryption_ptr_type>,
					_1, _2, pconn, popts, perr)))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_create_connection_encryption, connection_encryption alreday used");
			}

			assert(!conn_enc.validate());
		}

		bool bpush = pconn->push(pclt);
		assert(bpush);

		return conn_enc;
	}

	template<typename ConnEnc, typename Connection_T> inline
	static
	typename
		boost::enable_if
		<
			boost::is_same<mongoc_client_t, typename detail::get_org_t<Connection_T>::type>,
			ConnEnc&
		>::type
		s_org_client_encryption_create_connection_encryption(ConnEnc& conn_enc,
																Connection_T& conn,
																org_client_encryption_opts_type* popts,
																org_bson_error_type* perr)
	{
		typedef Connection_T connection_type;
		typedef smart_ptr_ex::shared_ptr<connection_type> connection_ptr_type;
		typedef smart_ptr_ex::nonfree_deleter<connection_type> now_connection_deleter_type;

		connection_ptr_type pconn(boost::addressof(conn), now_connection_deleter_type());

		return
			this_type::s_org_client_encryption_create_connection_encryption(
				conn_enc, pconn, popts, perr);
	}

	// s_org_client_encryption_create_datakey
	template<typename ConnEnc>
	static bool s_org_client_encryption_create_datakey(ConnEnc& conn_enc,
														const char* str_kms_provides_mode_name,
														const org_client_encryption_datakey_opts_type* popts,
														org_bson_value_type* pkeyid,
														org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_kms_provides_mode_name && popts && pkeyid))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_create_datakey, command invalid args");
			}
			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_create_datakey, mongoc_client_encryption is not ready");
			}

			return false;
		}

		bool bsuccess =
			::mongoc_client_encryption_create_datakey(
				pclt_enc.get(), str_kms_provides_mode_name, popts, pkeyid, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	// s_org_client_encryption_create_encrypted_collection
protected:
	inline static bool pro_s_org_client_encryption_create_encrypted_collection(mongoc_client_encryption_t* pclt_enc,
																				mongoc_client_t* pclt,
																				const char* str_db,
																				const char* str_coln,
																				const org_bson_type* popts,
																				const char* kms_provider,
																				const org_bson_type* popt_master_key,
																				org_bson_type* pout_popts,
																				org_bson_error_type* perr)
	{
		assert(pclt_enc && pclt && str_db && str_coln && popts && kms_provider);
		mongoc_database_t* pdb = ::mongoc_client_get_database(pclt, str_db);

		if(!pdb)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST,
					"mongo_accesser client_encryption_create_encrypted_collection, not found %s", str_db);
			}

			return false;
		}

		mongoc_collection_t* pcoln =
			mongoc_client_encryption_create_encrypted_collection(
				pclt_enc, pdb, str_coln, popts, pout_popts, kms_provider, popt_master_key, perr);

		bool bsuccess = !!pcoln;

		if(pcoln)
		{
			::mongoc_collection_destroy(pcoln);
		}

		if(pdb)
		{
			::mongoc_database_destroy(pdb);
		}

		return bsuccess;
	}

public:
	template<typename ConnEnc>
	static bool s_org_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
																	const char* str_db,
																	const char* str_coln,
																	const org_bson_type* popts,
																	const char* kms_provider,
																	const org_bson_type* popt_master_key,
																	org_bson_type* pout_popts,
																	org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;

		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;
		typedef typename conn_enc_type::mongoc_client_ptr_type mongoc_client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && popts && kms_provider))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_create_encrypted_collection, command invalid args");
			}
			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_create_encrypted_collection, mongoc_client_encryption is not ready");
			}

			return false;
		}

		mongoc_client_ptr_type pclt = conn_enc.get_client_shared_ptr(pclt_enc);
		assert(pclt);

		if(!pclt)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_create_encrypted_collection, mongoc_client_encryption panic");
			}

			bool bpush = conn_enc.push(pclt_enc);
			assert(bpush);

			return false;
		}

		bool bsuccess =
			this_type::pro_s_org_client_encryption_create_encrypted_collection(
				pclt_enc.get(), pclt.get(),
				str_db, str_coln,
				popts, kms_provider, popt_master_key,
				pout_popts, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	template<typename ConnEnc, typename Connection_T>
	static bool s_org_client_encryption_create_encrypted_collection(ConnEnc& conn_enc,
																	Connection_T& conn,
																	const char* str_db,
																	const char* str_coln,
																	const org_bson_type* popts,
																	const char* kms_provider,
																	const org_bson_type* popt_master_key,
																	org_bson_type* pout_popts,
																	org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::connection_id_type connection_id_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		typedef Connection_T connection_type;
		typedef typename connection_type::mongoc_client_ptr_type mongoc_client_ptr_type;

		{
			connection_id_type id_conn_enc = conn_enc.connection_id();
			if(!id_conn_enc)
			{
				return false;
			}

			if(conn.id() == id_conn_enc)
			{
				return
					this_type::s_org_client_encryption_create_encrypted_collection(
						conn_enc, str_db, str_coln, popts, kms_provider, popt_master_key, pout_popts, perr);
			}
		}

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(str_db && str_coln && popts && kms_provider))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_create_encrypted_collection, command invalid args");
			}
			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_create_encrypted_collection, mongoc_client_encryption is not ready");
			}

			return false;
		}

		mongoc_client_ptr_type pclt = conn.pop();
		assert(pclt);

		if(!pclt)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_create_encrypted_collection, mongoc_client_encryption panic");
			}

			bool bpush = conn_enc.push(pclt_enc);
			assert(bpush);

			return false;
		}

		bool bsuccess =
			this_type::pro_s_org_client_encryption_create_encrypted_collection(
				pclt_enc.get(), pclt.get(),
				str_db, str_coln,
				popts, kms_provider, popt_master_key,
				pout_popts, perr);

		bool bpush = conn.push(pclt);
		assert(bpush);

		bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	// s_org_client_encryption_rewrap_many_datakey
	template<typename ConnEnc>
	static bool s_org_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc,
															const org_bson_type* pfilter,
															const char* kms_provider,
															const org_bson_type* pmaster_key,
															org_client_encryption_rewrap_many_datakey_result_type* presult,
															org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_rewrap_many_datakey, mongoc_client_encryption is not ready");
			}

			return false;
		}

		bool bsuccess =
			::mongoc_client_encryption_rewrap_many_datakey(
				pclt_enc.get(), pfilter, kms_provider, pmaster_key, presult, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	// s_org_client_encryption_delete_key
	template<typename ConnEnc>
	static bool s_org_client_encryption_delete_key(ConnEnc& conn_enc,
													const org_bson_value_type* pkeyid,
													org_bson_type* preply,
													org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(pkeyid))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_delete_key, command invalid args");
			}

			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_delete_key, mongoc_client_encryption is not ready");
			}

			return false;
		}

		bool bsuccess = ::mongoc_client_encryption_delete_key(pclt_enc.get(), pkeyid, preply, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	// s_org_client_encryption_get_key
	// bson_value version // mongoc_client_encryption_get_key
	template<typename ConnEnc>
	static bool s_org_client_encryption_get_key(ConnEnc& conn_enc,
												const org_bson_value_type* pkeyid,
												org_bson_type* pkey_doc,
												org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(pkeyid))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_get_key, command invalid args");
			}

			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_get_key, mongoc_client_encryption is not ready");
			}

			return false;
		}

		bool bsuccess = ::mongoc_client_encryption_get_key(pclt_enc.get(), pkeyid, pkey_doc, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	// const char* version // mongoc_client_encryption_get_key_by_alt_name
	template<typename ConnEnc>
	static bool s_org_client_encryption_get_key_by_alt_name(ConnEnc& conn_enc,
															const char* pkeyaltname,
															org_bson_type* pkey_doc,
															org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(pkeyaltname))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_get_key, command invalid args");
			}

			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_get_key, mongoc_client_encryption is not ready");
			}

			return false;
		}

		bool bsuccess = ::mongoc_client_encryption_get_key_by_alt_name(pclt_enc.get(), pkeyaltname, pkey_doc, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	// s_org_client_encryption_get_keys
	template<typename ConnEnc, typename HandlerCollector>
	static bool s_org_client_encryption_get_keys(ConnEnc& conn_enc,
													org_bson_error_type* perr,
													const HandlerCollector& handler_collector)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_get_key, mongoc_client_encryption is not ready");
			}

			return false;
		}

		mongoc_cursor_t* pcur = mongoc_client_encryption_get_keys(pclt_enc.get(), perr);

		if(!pcur)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_QUERY,
					mongoc_error_code_type::E_MONGOC_ERROR_QUERY_FAILURE,
					"mongo_accesser client_encryption_get_key, query failed");
			}

			bool bpush = conn_enc.push(pclt_enc);
			assert(bpush);
			return false;
		}

		const org_bson_type* pdoc = 0;
		for(;mongoc_cursor_next(pcur, boost::addressof(pdoc));)
		{
			if(!handler_collector(*reinterpret_cast<const bson_type*>(pdoc)))
			{
				break;
			}
		}

		//bool bsuccess = !mongoc_cursor_error(pcur, perr);
		mongoc_cursor_error(pcur, perr);

		::mongoc_cursor_destroy(pcur);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return true;
	}

	// s_org_client_encryption_get_crypt_shared_version
protected:
	template<typename ConnEnc>
	static inner_string_type&
		pro_s_org_client_encryption_get_crypt_shared_version(inner_string_type& out_str_ver,
																ConnEnc& conn_enc,
																org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;
		typedef typename conn_enc_type::mongoc_client_ptr_type mongoc_client_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_get_crypt_shared_version, mongoc_client_encryption is not ready");
			}

			return out_str_ver;
		}

		const char* pstr_ver = mongoc_client_encryption_get_crypt_shared_version(pclt_enc.get());

		if(!pstr_ver)
		{
			mongoc_client_ptr_type pclt = conn_enc.get_client_shared_ptr(pclt_enc);
			assert(pclt);
			pstr_ver = mongoc_client_get_crypt_shared_version(pclt.get());
		}

		out_str_ver.assign((pstr_ver? pstr_ver : ""), YGGR_STR_UTF8_STRING_CHARSET_NAME());

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return out_str_ver;
	}

public:
	template<typename ConnEnc> inline
	static inner_string_type
		s_org_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, org_bson_error_type* perr)
	{
		inner_string_type str_ver;
		this_type::pro_s_org_client_encryption_get_crypt_shared_version(str_ver, conn_enc, perr);
		return str_ver;
	}

	template<typename ConnEnc, typename Alloc> inline
	static inner_string_type
		s_org_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc, const Alloc& alloc, org_bson_error_type* perr)
	{
		inner_string_type str_ver(alloc);
		this_type::pro_s_org_client_encryption_get_crypt_shared_version(str_ver, conn_enc, perr);
		return str_ver;
	}

	// s_org_client_encryption_add_key_alt_name
	template<typename ConnEnc>
	static bool s_org_client_encryption_add_key_alt_name(ConnEnc& conn_enc,
															const org_bson_value_type* pkeyid,
															const char* pkeyaltname,
															org_bson_type* pkey_doc,
															org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(pkeyid && pkeyaltname))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_add_key_alt_name, command invalid args");
			}

			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_add_key_alt_name, mongoc_client_encryption is not ready");
			}

			return false;
		}

		bool bsuccess =
			mongoc_client_encryption_add_key_alt_name(
				pclt_enc.get(), pkeyid, pkeyaltname, pkey_doc, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	//s_org_client_encryption_remove_key_alt_name
	template<typename ConnEnc>
	static bool s_org_client_encryption_remove_key_alt_name(ConnEnc& conn_enc,
															const org_bson_value_type* pkeyid,
															const char* pkeyaltname,
															org_bson_type* pkey_doc,
															org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(pkeyid && pkeyaltname))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_remove_key_alt_name, command invalid args");
			}

			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_remove_key_alt_name, mongoc_client_encryption is not ready");
			}

			return false;
		}

		bool bsuccess =
			mongoc_client_encryption_remove_key_alt_name(
				pclt_enc.get(), pkeyid, pkeyaltname, pkey_doc, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	// s_org_client_encryption_encrypt
	template<typename ConnEnc>
	static bool s_org_client_encryption_encrypt(ConnEnc& conn_enc,
												const org_bson_value_type* pval,
												org_client_encryption_encrypt_opts_type* popts,
												org_bson_value_type* pciphertext, // null "required 'ciphertext' unset"
												org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!pval)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_encrypt, command invalid args");
			}

			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_encrypt, mongoc_client_encryption is not ready");
			}

			return false;
		}

		bool bsuccess = mongoc_client_encryption_encrypt(pclt_enc.get(), pval, popts, pciphertext, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	// s_org_client_encryption_encrypt_expression
	template<typename ConnEnc>
	static bool s_org_client_encryption_encrypt_expression(ConnEnc& conn_enc,
															const org_bson_type* pexpr,
															org_client_encryption_encrypt_opts_type* popts,
															org_bson_type* pexpr_out,
															org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(pexpr && popts && pexpr_out))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_encrypt_expression, command invalid args");
			}

			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_encrypt_expression, mongoc_client_encryption is not ready");
			}

			return false;
		}

		bool bsuccess = mongoc_client_encryption_encrypt_expression(pclt_enc.get(), pexpr, popts, pexpr_out, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	// s_org_client_encryption_decrypt
	template<typename ConnEnc>
	static bool s_org_client_encryption_decrypt(ConnEnc& conn_enc,
												const org_bson_value_type* pciphertext,
												org_bson_value_type* pval, // null "required 'value' unset"
												org_bson_error_type* perr)
	{
		typedef ConnEnc conn_enc_type;
		typedef typename conn_enc_type::mongoc_client_encryption_ptr_type mongoc_client_encryption_ptr_type;

		bson_error_native_ex::s_bson_error_init_non_check(perr);

		if(!(pciphertext))
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND,
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser client_encryption_decrypt, command invalid args");
			}

			return false;
		}

		mongoc_client_encryption_ptr_type pclt_enc = conn_enc.pop();

		if(!pclt_enc)
		{
			if(perr)
			{
				bson_set_error(
					perr,
					mongoc_error_domain_type::E_MONGOC_ERROR_CLIENT,
					mongoc_error_code_type::E_MONGOC_ERROR_CLIENT_NOT_READY,
					"mongo_accesser client_encryption_decrypt, mongoc_client_encryption is not ready");
			}

			return false;
		}

		bool bsuccess = mongoc_client_encryption_decrypt(pclt_enc.get(), pciphertext, pval, perr);

		bool bpush = conn_enc.push(pclt_enc);
		assert(bpush);

		return bsuccess;
	}

	//------------------------encryption e------------------------

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BASIC_MONGO_ACCESSER_CLIENT_ENCRYPTION_HPP__
