// mongo_accesser_encryption_test_of_connect.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <sstream>

#include <yggr/ids/id_n.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>
#include <yggr/nsql_database_system/c_bson_binary_buffer.hpp>
#include <yggr/nsql_database_system/c_bson_binary_buffer_ref.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/c_mongo_enc_connection.hpp>
#include <yggr/nsql_database_system/c_mongo_connection_encryption.hpp>

#include <yggr/nsql_database_system/mongo_accesser.hpp>
#include <yggr/nsql_database_system/mongodb_kms_providers_maker.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


// 0 not-using-ssl 1 using-ssl
#define MONGODB_USING_CA() 0

// 0 not-using-auth 1 using-auth
#define MONGODB_USING_AUTH() 0

typedef yggr::file_system::local_fsys_operators local_fsys_operators;

// test crtpt s

void test_client_get_crypt_shared_version(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	// s_client_get_crypt_shared_version return inner_string
	{
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version(conn);
		yggr_test_assert(!str_shared_ver.empty());
		std::cout << str_shared_ver << std::endl;
	}

	{
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version(conn, &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// s_client_get_crypt_shared_version return inner_string alloc
	{
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// s_client_get_crypt_shared_version return inner_string or string
	// inner_string
	{
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn);
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn, &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn, err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// string
	{
		yggr::string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn);
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// string allocator
	{
		yggr::string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	yggr_test_assert(mongo_accesser_type::s_client_is_crypt_enabled(conn));

	std::cout << "------test_client_get_crypt_shared_version success------" << std::endl;
}

yggr::utf8_string g_master_key_fname = "./nsql_database_system/rs0_enc_master_key.key";

yggr::utf8_string g_scheam_fname = "./nsql_database_system/rs0_enc_schema.json";


yggr::utf8_string g_keyvault_db = "yggr_enc_keyvalut_db";
yggr::utf8_string g_keyvault_coll = "__yggr_enc_keyvalut_coll";

yggr::utf8_string g_enc_data_db = "yggr_enc_data_db";
yggr::utf8_string g_enc_data_coll = "yggr_enc_data_coll";

void clear_encryption_keyvalut_db(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::utf8_string keyvalue_db(g_keyvault_db);

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess 
			= yggr::nsql_database_system::mongo_accesser::s_database_drop(
				conn, keyvalue_db, 0, err);

		yggr_test_assert(bsuccess);
	}
}

void remove_scheam_file(void)
{
	local_fsys_operators::remove_file(
		local_fsys_operators::make_path_object(g_scheam_fname));
}

void clear_encryption_datas_db(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::utf8_string keyvalue_db(g_enc_data_db);

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess 
			= yggr::nsql_database_system::mongo_accesser::s_database_drop(
				conn, keyvalue_db, 0, err);

		yggr_test_assert(bsuccess);
	}
}

void clear_encryption_all_datas(yggr::nsql_database_system::c_mongo_connection& conn)
{
	if(!(local_fsys_operators::is_exists(
			local_fsys_operators::make_path_object(g_master_key_fname))
			&& local_fsys_operators::is_exists(
			local_fsys_operators::make_path_object(g_scheam_fname))))
	{
		clear_encryption_keyvalut_db(conn); // if cleaed keyvalut_db need create keyid
	}
	
	clear_encryption_datas_db(conn);
}

void create_master_key_file(const yggr::utf8_string& fname, bool fource_write = false)
{
	typedef yggr::ids::id_n<96> master_key_type;
	typedef master_key_type::string_type master_key_buffer_type;
	typedef yggr::ids::id_generator<master_key_type> master_key_gen_type;

	typedef local_fsys_operators::path_type file_path_type;

	yggr_test_assert(fname.size());

	if(fname.empty())
	{
		return;
	}

	file_path_type fpath = local_fsys_operators::make_path_object(fname);

	if(!(!local_fsys_operators::is_exists(fpath) || fource_write))
	{
		return;
	}

	master_key_gen_type mk_gen;
	master_key_type mk = mk_gen();

	std::cout << mk << std::endl;

	local_fsys_operators::write_file_of_binary(fpath, mk.begin(), mk.end());
}

yggr::ids::id_n<96> load_master_key_file(const yggr::utf8_string& fname)
{
	typedef yggr::ids::id_n<96> master_key_type;
	typedef master_key_type::string_type master_key_buffer_type;
	typedef yggr::ids::id_generator<master_key_type> master_key_gen_type;

	typedef local_fsys_operators::path_type file_path_type;

	yggr_test_assert(fname.size());

	if(fname.empty())
	{
		return master_key_type();
	}

	file_path_type fpath = local_fsys_operators::make_path_object(fname);

	if(!local_fsys_operators::is_exists(fpath))
	{
		return master_key_type();
	}

	master_key_type master_key;
	master_key_buffer_type& buf_ref = master_key;

	local_fsys_operators::read_file_of_binary(
		local_fsys_operators::make_path_object(fname),
		buf_ref);

	yggr_test_assert(master_key.size() == 96);

	return master_key.size() == 96? master_key : master_key_type();

}

template<typename Connect>
void test_client_encryption_create_connection_encryption(Connect& conn)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	typedef mongo_accesser_type::client_encryption_opts_type client_encryption_opts_type;

	typedef yggr::nsql_database_system::c_mongo_connection_encryption conn_enc_type;
	typedef yggr::nsql_database_system::mongodb_kms_providers_maker kms_providers_maker_type;

	typedef yggr::ids::id_n<96> master_key_type;
	typedef master_key_type::string_type master_key_buffer_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, master_key_buffer_type> master_key_binary_buffer_ref_type;

	yggr::utf8_string keyvalue_db(g_keyvault_db);
	yggr::utf8_string keyvalue_col(g_keyvault_coll);

	master_key_type mk = load_master_key_file(g_master_key_fname);
	master_key_binary_buffer_ref_type mk_binbuf_ref(mk);
	bson_type bs_kms_providers = kms_providers_maker_type::s_make_local_kms_providers(mk_binbuf_ref);
	std::cout << bs_kms_providers << std::endl;

	{
		client_encryption_opts_type enc_opts(keyvalue_db, keyvalue_col, bs_kms_providers, bson_type());

		conn_enc_type conn_enc;
		mongo_accesser_type::s_client_encryption_create_connection_encryption(conn_enc, conn, enc_opts);
		yggr_test_assert(conn_enc.validate());
	}

	{
		client_encryption_opts_type enc_opts(keyvalue_db, keyvalue_col, bs_kms_providers, bson_type());

		conn_enc_type conn_enc;
		mongo_accesser_type::s_client_encryption_create_connection_encryption(conn_enc, conn, enc_opts, 0);
		yggr_test_assert(conn_enc.validate());
	}

	{
		error_type err;
		client_encryption_opts_type enc_opts(keyvalue_db, keyvalue_col, bs_kms_providers, bson_type());

		conn_enc_type conn_enc; 
		mongo_accesser_type::s_client_encryption_create_connection_encryption(conn_enc, conn, enc_opts, err.org_pointer());

		yggr_test_assert(conn_enc.validate());
		yggr_test_assert(!err);

		std::cout << err << std::endl;
	}

	{
		error_type err;
		client_encryption_opts_type enc_opts(keyvalue_db, keyvalue_col, bs_kms_providers, bson_type());

		conn_enc_type conn_enc; 
		mongo_accesser_type::s_client_encryption_create_connection_encryption(conn_enc, conn, enc_opts, *(err.org_pointer()));

		yggr_test_assert(conn_enc.validate());
		yggr_test_assert(!err);

		std::cout << err << std::endl;
	}

	{
		error_type err;
		client_encryption_opts_type enc_opts(keyvalue_db, keyvalue_col, bs_kms_providers, bson_type());

		conn_enc_type conn_enc; 
		mongo_accesser_type::s_client_encryption_create_connection_encryption(conn_enc, conn, enc_opts, err);

		yggr_test_assert(conn_enc.validate());
		yggr_test_assert(!err);

		std::cout << err << std::endl;
	}

	std::cout << "------test_client_encryption_create_connection_encryption success------" << std::endl;
}

template<typename ConnEnc, typename Connect>
void create_connection_encryption(ConnEnc& conn_enc, Connect& conn)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	typedef mongo_accesser_type::client_encryption_opts_type client_encryption_opts_type;

	typedef ConnEnc conn_enc_type;
	typedef yggr::nsql_database_system::mongodb_kms_providers_maker kms_providers_maker_type;

	typedef yggr::ids::id_n<96> master_key_type;
	typedef master_key_type::string_type master_key_buffer_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, master_key_buffer_type> master_key_binary_buffer_ref_type;

	yggr::utf8_string keyvalue_db(g_keyvault_db);
	yggr::utf8_string keyvalue_col(g_keyvault_coll);

	master_key_type mk = load_master_key_file(g_master_key_fname);
	master_key_binary_buffer_ref_type mk_binbuf_ref(mk);
	bson_type bs_kms_providers = kms_providers_maker_type::s_make_local_kms_providers(mk_binbuf_ref);
	std::cout << bs_kms_providers << std::endl;

	{
		error_type err;
		client_encryption_opts_type enc_opts(keyvalue_db, keyvalue_col, bs_kms_providers, bson_type());
		//enc_opts.

		mongo_accesser_type::s_client_encryption_create_connection_encryption(conn_enc, conn, enc_opts, err);

		yggr_test_assert(conn_enc.validate());
		yggr_test_assert(!err);

		std::cout << err << std::endl;
	}
}

template<typename ConnEnc, typename Connect>
void create_connection_encryption_for_encrypt_expression(ConnEnc& conn_enc, Connect& conn)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	typedef mongo_accesser_type::client_encryption_opts_type client_encryption_opts_type;

	typedef ConnEnc conn_enc_type;
	typedef yggr::nsql_database_system::mongodb_kms_providers_maker kms_providers_maker_type;

	typedef yggr::ids::id_n<96> master_key_type;
	typedef master_key_type::string_type master_key_buffer_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, master_key_buffer_type> master_key_binary_buffer_ref_type;

	yggr::utf8_string keyvalue_db(g_keyvault_db);
	yggr::utf8_string keyvalue_col(g_keyvault_coll);

	master_key_type mk = load_master_key_file(g_master_key_fname);
	master_key_binary_buffer_ref_type mk_binbuf_ref(mk);
	bson_type bs_kms_providers = kms_providers_maker_type::s_make_local_kms_providers(mk_binbuf_ref);
	std::cout << bs_kms_providers << std::endl;

	{
		error_type err;
		client_encryption_opts_type enc_opts(keyvalue_db, keyvalue_col, bs_kms_providers, bson_type());

		mongo_accesser_type::s_client_encryption_create_connection_encryption(conn_enc, conn, enc_opts, err);

		yggr_test_assert(conn_enc.validate());
		yggr_test_assert(!err);

		std::cout << err << std::endl;
	}
}

template<typename ConnEnc>
void test_client_encryption_create_encrypted_collection(ConnEnc& conn_enc)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	bson_type cc_opts("{ \"encryptedFields\" : { \"fields\" : [ { \"path\" : \"enc_field_01\", \"bsonType\" : \"string\", \"keyId\" : null } ] } }");

	// conn_version
	{
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0);
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				*out_opts.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts);
		yggr_test_assert(bchk);
	}

	{
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				err.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				*err.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				err);
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts.org_pointer(),
				err.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts,
				err.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts.org_pointer(),
				err);
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, (*conn_enc.get_connection_shared_ptr()), 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts,
				err);
		yggr_test_assert(bchk);
	}

	// no conn version
	{
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0);
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				*out_opts.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts);
		yggr_test_assert(bchk);
	}

	{
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				err.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				*err.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				err);
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts.org_pointer(),
				err.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts,
				err.org_pointer());
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts.org_pointer(),
				err);
		yggr_test_assert(bchk);
	}

	{
		bson_type out_opts;
		error_type err;
		yggr::nsql_database_system::mongo_accesser::s_database_drop(
				(*conn_enc.get_connection_shared_ptr()), "yggr_test_create_encrypted_collection_db", 0, 0);
		bool bchk = 
			mongo_accesser_type::s_client_encryption_create_encrypted_collection(
				conn_enc, 
				"yggr_test_create_encrypted_collection_db",
				"yggr_test_create_encrypted_collection_coll",
				cc_opts, 
				"local",
				0,
				out_opts,
				err);
		yggr_test_assert(bchk);
	}

	std::cout << "------test_client_encryption_create_encrypted_collection success------" << std::endl;
}

template<typename Bson, typename BsonVal> inline
Bson& create_schema(Bson& schema, const BsonVal& keyid)
{
	typedef yggr::nsql_database_system::bson_mark_splice bson_mark_splice_type;

	schema.clear();

	schema.save("bsonType", "object");
	bson_mark_splice_type sp_properties;
	schema.save_document_start("properties", sp_properties);
	{
		bson_mark_splice_type sp_field;
		schema.save_document_start("enc_field_01", sp_field);
		{
			bson_mark_splice_type sp_enc;
			schema.save_document_start("encrypt", sp_enc);
			{
				bson_mark_splice_type sp_keyid;
				schema.save_array_start("keyId", sp_keyid);
				{
					schema.save("0", keyid);
				}
				schema.save_array_end(sp_keyid);
				schema.save("bsonType", "string");
				schema.save("algorithm", MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC);
			}
			schema.save_document_end(sp_enc);
		}
		schema.save_document_end(sp_field);
	}
	schema.save_document_end(sp_properties);

	return schema;
}

template<typename Bson, typename BsonVal> inline
BsonVal& get_keyid(BsonVal& key_id, const Bson& schema)
{
	typedef Bson bson_type;
	typedef BsonVal bson_value_type;

	typedef typename bson_type::iterator bson_iter_type;

	bson_iter_type iter = schema.find("properties");
	if(iter == schema.end())
	{
		return key_id;
	}

	std::pair<bson_iter_type, bson_iter_type> iter_pair = iter.find_descendant("properties.enc_field_01.encrypt.keyId.0");

	if(iter_pair.first == iter_pair.second)
	{
		return key_id;
	}

	yggr_test_assert(iter_pair.first != schema.end());
	yggr_test_assert(iter_pair.second != schema.end());
	yggr_test_assert(iter_pair.second.get_key() == "0");

	iter_pair.second.load_value(key_id);

	return key_id;
}

template<typename ConnEnc>
void test_client_encryption_create_datakey_and_schema(ConnEnc& conn_enc)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	typedef mongo_accesser_type::client_encryption_datakey_opts_type client_encryption_datakey_opts_type;

	typedef yggr::nsql_database_system::mongodb_kms_providers_maker kms_providers_maker_type;

	typedef local_fsys_operators::path_type file_path_type;

	file_path_type schema_fpath = local_fsys_operators::make_path_object(g_scheam_fname);

	bson_type schema;

	if(local_fsys_operators::is_exists(schema_fpath))
	{
		yggr::utf8_string buf;
		local_fsys_operators::read_file_of_binary(schema_fpath, buf.org_str());

		yggr_test_assert(buf.size());
		yggr::charset::affix_length(buf, buf.size());
		schema.assign(buf);
	}
	else
	{
		error_type err;
		yggr::utf8_string keyaltname("yggr_conn_enc_keyvalut_1");

		bson_value_type keyid;
		client_encryption_datakey_opts_type datakey_opts(&keyaltname, (&keyaltname) + 1);

		if(!mongo_accesser_type::s_client_encryption_create_datakey(
				conn_enc,
				kms_providers_maker_type::s_kms_providers_mode_name_local(),
				datakey_opts, 
				keyid, err))
		{
			std::cout << err << std::endl;
			yggr_test_assert(false);
		}
		
		std::cout << keyid << std::endl;

		create_schema(schema, keyid);
		yggr_test_assert(!schema.empty());

		yggr::utf8_string js_schema = schema.to_json_canonical();
		local_fsys_operators::write_file_of_binary(schema_fpath, js_schema.org_str());
	}

	yggr_test_assert(!schema.empty());
	std::cout << schema << std::endl;

	std::cout << "------test_client_encryption_create_datakey_and_schema success------" << std::endl;
}


template<typename Conn>
void test_client_encryption_insert(Conn& conn)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;

	typedef yggr::nsql_database_system::c_mongo_enc_connection mongo_enc_conn_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	typedef yggr::nsql_database_system::c_mongo_auto_encryption_opts auto_encryption_opts_type;

	typedef yggr::ids::id_n<96> master_key_type;
	typedef master_key_type::string_type master_key_buffer_type;
	typedef yggr::nsql_database_system::c_bson_binary_buffer_ref<0, master_key_buffer_type> master_key_binary_buffer_ref_type;

	typedef yggr::nsql_database_system::mongodb_kms_providers_maker kms_providers_maker_type;

	typedef local_fsys_operators::path_type file_path_type;

	// load master_key and make bson_kms_providers
	bson_type bs_kms_providers;
	{
		master_key_type mk = load_master_key_file(g_master_key_fname);
		yggr_test_assert(mk.size());

		master_key_binary_buffer_ref_type mk_binbuf_ref(mk);
		bs_kms_providers = kms_providers_maker_type::s_make_local_kms_providers(mk_binbuf_ref);
		
		std::cout << bs_kms_providers << std::endl;
	}

	// load schema
	file_path_type schema_fpath = local_fsys_operators::make_path_object(g_scheam_fname);

	yggr_test_assert(local_fsys_operators::is_exists(schema_fpath));

	bson_type schema;

	{
		yggr::utf8_string buf;
		local_fsys_operators::read_file_of_binary(schema_fpath, buf.org_str());

		yggr_test_assert(buf.size());
		yggr::charset::affix_length(buf, buf.size());
		schema.assign(buf);
	}

	yggr_test_assert(!schema.empty());

	std::cout << schema << std::endl;

	yggr::utf8_string keyvalue_db(g_keyvault_db);
	yggr::utf8_string keyvalue_col(g_keyvault_coll);

	yggr::utf8_string enc_data_db(g_enc_data_db);
	yggr::utf8_string enc_data_coll(g_enc_data_coll);

	mongo_enc_conn_type enc_conn;

	{
		bson_type schema_map;
		schema_map.save(enc_data_db + "." + enc_data_coll, schema);

		std::cout << schema_map << std::endl;

		error_type err;
		auto_encryption_opts_type auto_enc_opts(keyvalue_db, 
												keyvalue_col,
												bs_kms_providers,
												bson_type(),
												schema_map);


		bool bchk = mongo_accesser_type::s_client_enable_auto_encryption(
						enc_conn, conn, auto_enc_opts, err);

		yggr_test_assert(bchk);
		yggr_test_assert(enc_conn.validate());
	}
	
	{
		error_type err;
		mongo_accesser_type::s_client_get_crypt_shared_version(conn, err);
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
		std::cout << str_shared_ver << std::endl;
	}

//	{
//		error_type err;
//		yggr::utf8_string str_shared_ver = 
//			mongo_accesser_type::s_client_get_crypt_shared_version(conn, err);
//		yggr_test_assert(!str_shared_ver.empty());
//		yggr_test_assert(!err);
//		std::cout << str_shared_ver << std::endl;
//	}

	bson_type ins_data;
	ins_data.save("enc_field_01", "enc_value_01");
	std::cout << ins_data << std::endl;

	error_type err;
	bson_type opts;
	bson_type reply;

	bool bchk = 
		mongo_accesser_type::s_collection_insert(
			enc_conn, enc_data_db, enc_data_coll, ins_data, opts, reply, err);

	if(!bchk)
	{
		std::cout << err << std::endl;
	}

	yggr_test_assert(bchk);

	std::cout << reply << std::endl;

	std::cout << "------test_client_encryption_insert success------" << std::endl;
}

template<typename ConnEnc>
void test_client_encryption_rewrap_many_datakey(ConnEnc& conn_enc)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	typedef yggr::nsql_database_system::bson_mark_splice bson_mark_splice_type;

	typedef mongo_accesser_type::client_encryption_rewrap_many_datakey_result_type rewrap_many_datakey_result_type;
	typedef mongo_accesser_type::client_encryption_datakey_opts_type client_encryption_datakey_opts_type;

	typedef yggr::nsql_database_system::mongodb_kms_providers_maker kms_providers_maker_type;

	bson_value_type keyid;

	yggr::utf8_string first_keyaltname("yggr_conn_enc_keyvalut_2");
	yggr::utf8_string keyaltname("yggr_conn_enc_keyvalut_2_ex");

	// create a new keyid
	{
		error_type err;

		client_encryption_datakey_opts_type datakey_opts(&first_keyaltname, (&first_keyaltname) + 1);

		if(!mongo_accesser_type::s_client_encryption_create_datakey(
				conn_enc,
				kms_providers_maker_type::s_kms_providers_mode_name_local(),
				datakey_opts, 
				keyid, err))
		{
			std::cout << err << std::endl;
			yggr_test_assert(false);
		}
	}

	std::cout << keyid << std::endl;

	// check keyid
	{
		error_type err;
		bson_type key_doc;
		bool bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}

	{
		error_type err;
		rewrap_many_datakey_result_type rewrap_rst;

		bson_type new_local_master_key;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_rewrap_many_datakey(
				conn_enc, 0, "", 0, rewrap_rst, err);

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << rewrap_rst << std::endl;
	}

	// check keyid
	{
		error_type err;
		bson_type key_doc;
		bool bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}

	{
		error_type err;
		rewrap_many_datakey_result_type rewrap_rst;

		bson_type bs_filter;

		bs_filter.save("_id", keyid);

		bson_type new_local_master_key;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_rewrap_many_datakey(
				conn_enc, bs_filter, "", 0, rewrap_rst, err);

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << rewrap_rst << std::endl;
	}

	// check keyid
	{
		error_type err;
		bson_type key_doc;
		bool bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}

	{
		error_type err;
		bson_type bs_filter;
		bson_type mk;
		rewrap_many_datakey_result_type rewrap_rst;

		bs_filter.save("_id", keyid);

		bool bchk = 
			mongo_accesser_type::s_client_encryption_rewrap_many_datakey(
				conn_enc, bs_filter.org_pointer(), "local", mk.org_pointer(), rewrap_rst.org_pointer(), err.org_pointer());

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << rewrap_rst << std::endl;
	}

	{
		error_type err;
		bson_type bs_filter;
		bson_type mk;
		rewrap_many_datakey_result_type rewrap_rst;

		bs_filter.save("_id", keyid);

		bool bchk = 
			mongo_accesser_type::s_client_encryption_rewrap_many_datakey(
				conn_enc, bs_filter.org_pointer(), "local", mk.org_pointer(), rewrap_rst, err.org_pointer());

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << rewrap_rst << std::endl;
	}

	{
		error_type err;
		bson_type bs_filter;
		bson_type mk;
		rewrap_many_datakey_result_type rewrap_rst;

		bs_filter.save("_id", keyid);

		bool bchk = 
			mongo_accesser_type::s_client_encryption_rewrap_many_datakey(
				conn_enc, bs_filter, "local", mk, rewrap_rst.org_pointer(), err);

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << rewrap_rst << std::endl;
	}

	{
		error_type err;
		bson_type bs_filter;
		bson_type mk;
		rewrap_many_datakey_result_type rewrap_rst;

		bs_filter.save("_id", keyid);

		bool bchk = 
			mongo_accesser_type::s_client_encryption_rewrap_many_datakey(
				conn_enc, bs_filter.org_pointer(), 0, 0, rewrap_rst.org_pointer(), err.org_pointer());

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << rewrap_rst << std::endl;
	}

	{
		error_type err;
		bson_type bs_filter;
		bson_type mk;
		rewrap_many_datakey_result_type rewrap_rst;

		bs_filter.save("_id", keyid);

		bool bchk = 
			mongo_accesser_type::s_client_encryption_rewrap_many_datakey(
				conn_enc, bs_filter, "local", mk, rewrap_rst, err);

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << rewrap_rst << std::endl;
	}

	// check keyid
	{
		error_type err;
		bson_type key_doc;
		bool bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}
	
	std::cout << "------test_client_encryption_rewrap_many_datakey success------" << std::endl;
}


template<typename ConnEnc>
void test_client_encryption_get_key(ConnEnc& conn_enc)
{
	typedef local_fsys_operators::path_type file_path_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	file_path_type schema_fpath = local_fsys_operators::make_path_object(g_scheam_fname);

	bson_type schema;

	yggr_test_assert(local_fsys_operators::is_exists(schema_fpath));

	yggr::utf8_string buf;
	local_fsys_operators::read_file_of_binary(schema_fpath, buf.org_str());

	yggr_test_assert(buf.size());
	yggr::charset::affix_length(buf, buf.size());
	schema.assign(buf);

	bson_value_type keyid;

	get_keyid(keyid, schema);

	std::cout << keyid << std::endl;

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyid.org_pointer());

		yggr_test_assert(bchk);
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyid);

		yggr_test_assert(bchk);
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyid,
				key_doc.org_pointer());

		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyid,
				key_doc);

		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyid,
				err.org_pointer());

		yggr_test_assert(bchk);
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyid,
				err);

		yggr_test_assert(bchk);
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyid.org_pointer(), 
				key_doc.org_pointer(),
				err.org_pointer());

		yggr_test_assert(bchk);
		yggr_test_assert(!err);

		std::cout << key_doc << std::endl;
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyid, 
				key_doc,
				err.org_pointer());

		yggr_test_assert(bchk);
		yggr_test_assert(!err);

		std::cout << key_doc << std::endl;
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyid, 
				key_doc.org_pointer(),
				err);

		yggr_test_assert(bchk);
		yggr_test_assert(!err);

		std::cout << key_doc << std::endl;
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyid, 
				key_doc,
				err);

		yggr_test_assert(bchk);
		yggr_test_assert(!err);

		std::cout << key_doc << std::endl;
	}

	std::cout << "------test_client_encryption_get_key success------" << std::endl;
}

template<typename ConnEnc>
void test_client_encryption_get_key_by_key_alt_name(ConnEnc& conn_enc)
{
	typedef local_fsys_operators::path_type file_path_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	yggr::utf8_string keyaltname("yggr_conn_enc_keyvalut_1");

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyaltname.c_str());

		yggr_test_assert(bchk);
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyaltname);

		yggr_test_assert(bchk);
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyaltname,
				key_doc.org_pointer());

		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyaltname,
				key_doc);

		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyaltname,
				err.org_pointer());

		yggr_test_assert(bchk);
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyaltname,
				err);

		yggr_test_assert(bchk);
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyaltname, 
				key_doc.org_pointer(),
				err.org_pointer());

		yggr_test_assert(bchk);
		yggr_test_assert(!err);

		std::cout << key_doc << std::endl;
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyaltname, 
				key_doc,
				err.org_pointer());

		yggr_test_assert(bchk);
		yggr_test_assert(!err);

		std::cout << key_doc << std::endl;
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyaltname, 
				key_doc.org_pointer(),
				err);

		yggr_test_assert(bchk);
		yggr_test_assert(!err);

		std::cout << key_doc << std::endl;
	}

	{
		bson_type key_doc;
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_key(
				conn_enc, 
				keyaltname, 
				key_doc,
				err);

		yggr_test_assert(bchk);
		yggr_test_assert(!err);

		std::cout << key_doc << std::endl;
	}

	std::cout << "------test_client_encryption_get_key_by_key_alt_name success------" << std::endl;
}

bool handler_get_keys(const yggr::nsql_database_system::c_bson& bs_doc)
{
	std::cout << bs_doc << std::endl;
	return true;
}

template<typename ConnEnc>
void test_client_encryption_get_keys(ConnEnc& conn_enc)
{
	typedef local_fsys_operators::path_type file_path_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	{
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_keys(
				conn_enc, 
				boost::bind(&handler_get_keys, _1));

		yggr_test_assert(bchk);
	}

	{
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_keys(
				conn_enc, 
				err.org_pointer(),
				boost::bind(&handler_get_keys, _1));

		yggr_test_assert(bchk);
	}

	{
		error_type err;

		bool bchk = 
			mongo_accesser_type::s_client_encryption_get_keys(
				conn_enc, 
				err,
				boost::bind(&handler_get_keys, _1));

		yggr_test_assert(bchk);
	}

	std::cout << "------test_client_encryption_get_keys success------" << std::endl;
}

template<typename ConnEnc>
void test_client_encryption_add_and_remove_key_alt_name(ConnEnc& conn_enc)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	typedef mongo_accesser_type::client_encryption_datakey_opts_type client_encryption_datakey_opts_type;

	typedef yggr::nsql_database_system::mongodb_kms_providers_maker kms_providers_maker_type;

	//typedef local_fsys_operators::path_type file_path_type;

	bson_value_type keyid;

	yggr::utf8_string first_keyaltname("yggr_conn_enc_keyvalut_2");
	yggr::utf8_string keyaltname("yggr_conn_enc_keyvalut_2_ex");

	// create a new keyid
	{
		error_type err;

		client_encryption_datakey_opts_type datakey_opts(&first_keyaltname, (&first_keyaltname) + 1);

		if(!mongo_accesser_type::s_client_encryption_create_datakey(
				conn_enc,
				kms_providers_maker_type::s_kms_providers_mode_name_local(),
				datakey_opts, 
				keyid, err))
		{
			std::cout << err << std::endl;
			yggr_test_assert(false);
		}
	}

	std::cout << keyid << std::endl;

	// check keyid
	{
		error_type err;
		bson_type key_doc;
		bool bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}

	// add_key_alt_name test

	{
		error_type err;
		bson_type key_doc;
		bool bchk = false;
		
		bchk = mongo_accesser_type::s_client_encryption_add_key_alt_name(conn_enc, keyid.org_pointer(), keyaltname.c_str());
		yggr_test_assert(bchk);

		bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();

		bchk = mongo_accesser_type::s_client_encryption_remove_key_alt_name(conn_enc, keyid.org_pointer(), keyaltname.c_str());
		yggr_test_assert(bchk);
	}

	{
		error_type err;
		bson_type key_doc; 
		bool bchk = false;
		
		bchk = mongo_accesser_type::s_client_encryption_add_key_alt_name(conn_enc, keyid, keyaltname);
		yggr_test_assert(bchk);

		bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();

		bchk = mongo_accesser_type::s_client_encryption_remove_key_alt_name(conn_enc, keyid, keyaltname);
		yggr_test_assert(bchk);
	}

	{
		error_type err;
		bson_type key_doc;
		bool bchk = false;
		
		bchk = mongo_accesser_type::s_client_encryption_add_key_alt_name(conn_enc, keyid, keyaltname, key_doc.org_pointer(), err.org_pointer());
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();

		bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();

		bchk = mongo_accesser_type::s_client_encryption_remove_key_alt_name(conn_enc, keyid, keyaltname, key_doc.org_pointer(), err.org_pointer());
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();
	}

	{
		error_type err;
		bson_type key_doc;
		bool bchk = false;
		
		bchk = mongo_accesser_type::s_client_encryption_add_key_alt_name(conn_enc, keyid, keyaltname, key_doc, err.org_pointer());
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();

		bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();

		bchk = mongo_accesser_type::s_client_encryption_remove_key_alt_name(conn_enc, keyid, keyaltname, key_doc, err.org_pointer());
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();
	}

	{
		error_type err;
		bson_type key_doc;
		bool bchk = false;
		
		bchk = mongo_accesser_type::s_client_encryption_add_key_alt_name(conn_enc, keyid, keyaltname, key_doc.org_pointer(), err);
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();

		bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();

		bchk = mongo_accesser_type::s_client_encryption_remove_key_alt_name(conn_enc, keyid, keyaltname, key_doc.org_pointer(), err);
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();
	}

	{
		error_type err;
		bson_type key_doc;
		bool bchk = false;
		
		bchk = mongo_accesser_type::s_client_encryption_add_key_alt_name(conn_enc, keyid, keyaltname, key_doc, err);
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();

		bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();

		bchk = mongo_accesser_type::s_client_encryption_remove_key_alt_name(conn_enc, keyid, keyaltname, key_doc, err);
		yggr_test_assert(bchk);
		std::cout << key_doc << std::endl;
		key_doc.clear();
	}

	std::cout << "------test_client_encryption_add_and_remove_key_alt_name success------" << std::endl;
}

template<typename ConnEnc>
void test_client_encryption_create_and_delete_key(ConnEnc& conn_enc)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	typedef mongo_accesser_type::client_encryption_datakey_opts_type client_encryption_datakey_opts_type;

	typedef yggr::nsql_database_system::mongodb_kms_providers_maker kms_providers_maker_type;


	yggr::utf8_string keyaltname("yggr_conn_enc_keyvalut_2");

	client_encryption_datakey_opts_type datakey_opts(&keyaltname, (&keyaltname) + 1);

	// create a new keyid
	{
		bool bchk = false;
		error_type err;
		bson_value_type keyid;
		bson_type reply;

		bchk = 
			mongo_accesser_type::s_client_encryption_create_datakey(
				conn_enc,
				kms_providers_maker_type::s_kms_providers_mode_name_local(),
				datakey_opts.org_pointer(), 
				keyid.org_pointer(), err.org_pointer());

		yggr_test_assert(bchk);


		bchk = 
			mongo_accesser_type::s_client_encryption_delete_key(
				conn_enc, keyid.org_pointer(), reply.org_pointer(), err.org_pointer());

		yggr_test_assert(true);

		std::cout << reply << std::endl;
	}

	{
		bool bchk = false;
		error_type err;
		bson_value_type keyid;
		bson_type reply;

		bchk = 
			mongo_accesser_type::s_client_encryption_create_datakey(
				conn_enc,
				kms_providers_maker_type::s_kms_providers_mode_name_local(),
				datakey_opts, 
				keyid.org_pointer(), err);

		yggr_test_assert(bchk);


		bchk = 
			mongo_accesser_type::s_client_encryption_delete_key(
				conn_enc, keyid, reply.org_pointer(), err);

		yggr_test_assert(true);

		std::cout << reply << std::endl;
	}

	{
		bool bchk = false;
		error_type err;
		bson_value_type keyid;
		bson_type reply;

		bchk = 
			mongo_accesser_type::s_client_encryption_create_datakey(
				conn_enc,
				kms_providers_maker_type::s_kms_providers_mode_name_local(),
				datakey_opts, 
				keyid, err.org_pointer());

		yggr_test_assert(bchk);


		bchk = 
			mongo_accesser_type::s_client_encryption_delete_key(
				conn_enc, keyid, reply, err.org_pointer());

		yggr_test_assert(true);

		std::cout << reply << std::endl;
	}

	{
		bool bchk = false;
		error_type err;
		bson_value_type keyid;
		bson_type reply;

		bchk = 
			mongo_accesser_type::s_client_encryption_create_datakey(
				conn_enc,
				kms_providers_maker_type::s_kms_providers_mode_name_local(),
				datakey_opts, 
				keyid, err);

		yggr_test_assert(bchk);


		bchk = 
			mongo_accesser_type::s_client_encryption_delete_key(
				conn_enc, keyid, reply, err);

		yggr_test_assert(true);

		std::cout << reply << std::endl;
	}

	std::cout << "------test_client_encryption_delete_key success------" << std::endl;
}


template<typename ConnEnc>
void test_client_encryption_get_crypt_shared_version(ConnEnc& conn_enc)
{
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	// s_client_encryption_get_crypt_shared_version return inner_string
	{
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc);
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// s_client_encryption_get_crypt_shared_version return inner_string alloc
	{
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, yggr::utf8_string().get_allocator());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, yggr::utf8_string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, yggr::utf8_string().get_allocator(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, yggr::utf8_string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, yggr::utf8_string().get_allocator(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version(conn_enc, yggr::utf8_string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// s_client_encryption_get_crypt_shared_version return inner_string or string
	// inner_string
	{
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::utf8_string>(conn_enc);
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::utf8_string>(conn_enc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::utf8_string>(conn_enc, &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::utf8_string>(conn_enc, YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::utf8_string>(conn_enc, err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::utf8_string>(conn_enc, YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// string
	{
		yggr::string str_shared_ver = mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc);
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::string str_shared_ver = mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// string allocator
	{
		yggr::string str_shared_ver = mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, yggr::string().get_allocator());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, yggr::string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(!str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::string str_shared_ver = mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, yggr::string().get_allocator(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, yggr::string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, yggr::string().get_allocator(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver = 
			mongo_accesser_type::s_client_encryption_get_crypt_shared_version<yggr::string>(conn_enc, yggr::string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(!str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	std::cout << "------test_client_encryption_get_crypt_shared_version success------" << std::endl;
}

template<typename ConnEnc>
void test_client_encryption_exception_encrypt_decrypt(ConnEnc& conn_enc)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	typedef mongo_accesser_type::client_encryption_datakey_opts_type client_encryption_datakey_opts_type;
	typedef mongo_accesser_type::client_encryption_encrypt_opts_type client_encryption_encrypt_opts_type;

	typedef yggr::nsql_database_system::mongodb_kms_providers_maker kms_providers_maker_type;

	bson_value_type keyid;

	yggr::utf8_string keyaltname("yggr_conn_enc_keyvalut_3");

	// create a new keyid
	{
		error_type err;

		client_encryption_datakey_opts_type datakey_opts(&keyaltname, (&keyaltname) + 1);

		if(!mongo_accesser_type::s_client_encryption_create_datakey(
				conn_enc,
				kms_providers_maker_type::s_kms_providers_mode_name_local(),
				datakey_opts, 
				keyid, err))
		{
			std::cout << err << std::endl;
			yggr_test_assert(false);
		}
	}

	std::cout << keyid << std::endl;

	// check keyid
	{
		error_type err;
		bson_type key_doc;
		bool bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}

	// ins_data.save("enc_field_01", "enc_value_01"); // test_data
	// encrypt decrypt

	{
		error_type err;

		bson_value_type data("enc_value_01");
		bson_value_type enc_data;
		bson_value_type dec_data;
		
		bool bchk = false;

		client_encryption_encrypt_opts_type enc_opts(keyid, 
														client_encryption_encrypt_opts_type
															::s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_deterministic());

		bchk = mongo_accesser_type::s_client_encryption_encrypt(conn_enc, data.org_pointer(), enc_opts.org_pointer(), enc_data.org_pointer());
		yggr_test_assert(bchk);

		std::cout << enc_data << std::endl;

		bchk = mongo_accesser_type::s_client_encryption_decrypt(conn_enc, enc_data.org_pointer(), dec_data.org_pointer());
		yggr_test_assert(bchk);

		std::cout << dec_data << std::endl;

		yggr_test_assert(data == dec_data);
	}

	{
		error_type err;

		bson_value_type data("enc_value_01");
		bson_value_type enc_data;
		bson_value_type dec_data;
		
		bool bchk = false;

		client_encryption_encrypt_opts_type enc_opts(keyid, 
														client_encryption_encrypt_opts_type
															::s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_deterministic());

		bchk = mongo_accesser_type::s_client_encryption_encrypt(conn_enc, data, enc_opts, enc_data);
		yggr_test_assert(bchk);

		std::cout << enc_data << std::endl;

		bchk = mongo_accesser_type::s_client_encryption_decrypt(conn_enc, enc_data, dec_data);
		yggr_test_assert(bchk);

		std::cout << dec_data << std::endl;

		yggr_test_assert(data == dec_data);
	}

	{
		error_type err;

		bson_value_type data("enc_value_01");
		bson_value_type enc_data;
		bson_value_type dec_data;
		
		bool bchk = false;

		client_encryption_encrypt_opts_type enc_opts(keyid, 
														client_encryption_encrypt_opts_type
															::s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_deterministic());

		bchk = mongo_accesser_type::s_client_encryption_encrypt(conn_enc, data, enc_opts, enc_data, err.org_pointer());
		yggr_test_assert(bchk);

		std::cout << enc_data << std::endl;

		bchk = mongo_accesser_type::s_client_encryption_decrypt(conn_enc, enc_data, dec_data, err.org_pointer());
		yggr_test_assert(bchk);

		std::cout << dec_data << std::endl;

		yggr_test_assert(data == dec_data);
	}

	{
		error_type err;

		bson_value_type data("enc_value_01");
		bson_value_type enc_data;
		bson_value_type dec_data;
		
		bool bchk = false;

		client_encryption_encrypt_opts_type enc_opts(keyid, 
														client_encryption_encrypt_opts_type
															::s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_deterministic());

		bchk = mongo_accesser_type::s_client_encryption_encrypt(conn_enc, data, enc_opts, enc_data, err);
		yggr_test_assert(bchk);

		std::cout << enc_data << std::endl;

		bchk = mongo_accesser_type::s_client_encryption_decrypt(conn_enc, enc_data, dec_data, err);
		yggr_test_assert(bchk);

		std::cout << dec_data << std::endl;

		yggr_test_assert(data == dec_data);
	}

	std::cout << "------test_client_encryption_exception_encrypt_decrypt success------" << std::endl;
}

template<typename ConnEnc>
void test_client_encryption_exception_encrypt_expression(ConnEnc& conn_enc)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	typedef mongo_accesser_type::client_encryption_datakey_opts_type client_encryption_datakey_opts_type;
	typedef mongo_accesser_type::client_encryption_encrypt_opts_type client_encryption_encrypt_opts_type;
	typedef yggr::nsql_database_system::c_mongo_client_encryption_encrypt_range_opts client_encryption_encrypt_range_opts_type;

	typedef yggr::nsql_database_system::mongodb_kms_providers_maker kms_providers_maker_type;

	bson_value_type keyid;

	yggr::utf8_string keyaltname("yggr_conn_enc_keyvalut_4");

	// create a new keyid
	{
		error_type err;

		client_encryption_datakey_opts_type datakey_opts(&keyaltname, (&keyaltname) + 1);

		if(!mongo_accesser_type::s_client_encryption_create_datakey(
				conn_enc,
				kms_providers_maker_type::s_kms_providers_mode_name_local(),
				datakey_opts, 
				keyid, err))
		{
			std::cout << err << std::endl;
			yggr_test_assert(false);
		}
	}

	std::cout << keyid << std::endl;

	// check keyid
	{
		error_type err;
		bson_type key_doc;
		bool bchk = mongo_accesser_type::s_client_encryption_get_key(conn_enc, keyid, key_doc, err);
		yggr_test_assert(bchk);

		std::cout << key_doc << std::endl;
	}

	// ins_data.save("enc_field_01", "enc_value_01"); // test_data
	// encrypt decrypt

	{
		error_type err;
		bson_type expr("{\"$and\" : [{\"enc_field_01\": {\"$gt\": 0}}, {\"enc_field_01\": {\"$lt\": 200 }}]}");
		bson_type enc_expr;

		client_encryption_encrypt_range_opts_type range_opts(bson_value_type(0), 
																bson_value_type(200), 
																1 );
		
		bool bchk = false;

		std::cout << expr << std::endl;

		client_encryption_encrypt_opts_type enc_opts(keyid,
														client_encryption_encrypt_opts_type::s_encrypt_algorithm_range_preview(),
														"",
														0,
														client_encryption_encrypt_opts_type::s_encrypt_query_mode_range_preview(),
														range_opts );

		bchk = mongo_accesser_type::s_client_encryption_encrypt(conn_enc, expr, enc_opts, enc_expr);

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << enc_expr << std::endl;
	}

	{
		error_type err;
		bson_type expr("{\"$and\" : [{\"enc_field_01\": {\"$gt\": 0}}, {\"enc_field_01\": {\"$lt\": 200 }}]}");
		bson_type enc_expr;

		client_encryption_encrypt_range_opts_type range_opts(bson_value_type(0), 
																bson_value_type(200), 
																1 );
		
		bool bchk = false;

		std::cout << expr << std::endl;

		client_encryption_encrypt_opts_type enc_opts(keyid,
														client_encryption_encrypt_opts_type::s_encrypt_algorithm_range_preview(),
														"",
														0,
														client_encryption_encrypt_opts_type::s_encrypt_query_mode_range_preview(),
														range_opts );

		bchk = mongo_accesser_type::s_client_encryption_encrypt(
				conn_enc, expr.org_pointer(), enc_opts.org_pointer(),
				enc_expr.org_pointer(), err.org_pointer());

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << enc_expr << std::endl;
	}

	{
		error_type err;
		bson_type expr("{\"$and\" : [{\"enc_field_01\": {\"$gt\": 0}}, {\"enc_field_01\": {\"$lt\": 200 }}]}");
		bson_type enc_expr;

		client_encryption_encrypt_range_opts_type range_opts(bson_value_type(0), 
																bson_value_type(200), 
																1 );
		
		bool bchk = false;

		std::cout << expr << std::endl;

		client_encryption_encrypt_opts_type enc_opts(keyid,
														client_encryption_encrypt_opts_type::s_encrypt_algorithm_range_preview(),
														"",
														0,
														client_encryption_encrypt_opts_type::s_encrypt_query_mode_range_preview(),
														range_opts );

		bchk = mongo_accesser_type::s_client_encryption_encrypt(conn_enc, expr.org_pointer(), enc_opts.org_pointer(), enc_expr.org_pointer(), err.org_pointer());

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << enc_expr << std::endl;
	}

	{
		error_type err;
		bson_type expr("{\"$and\" : [{\"enc_field_01\": {\"$gt\": 0}}, {\"enc_field_01\": {\"$lt\": 200 }}]}");
		bson_type enc_expr;

		client_encryption_encrypt_range_opts_type range_opts(bson_value_type(0), 
																bson_value_type(200), 
																1 );
		
		bool bchk = false;

		std::cout << expr << std::endl;

		client_encryption_encrypt_opts_type enc_opts(keyid,
														client_encryption_encrypt_opts_type::s_encrypt_algorithm_range_preview(),
														"",
														0,
														client_encryption_encrypt_opts_type::s_encrypt_query_mode_range_preview(),
														range_opts );

		bchk = mongo_accesser_type::s_client_encryption_encrypt(conn_enc, expr, enc_opts, enc_expr, err);

		if(!bchk)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bchk);

		std::cout << enc_expr << std::endl;
	}

	std::cout << "------test_client_encryption_exception_encrypt_expression success------" << std::endl;
}

// test crtpt e

void test_client_encryption_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection_encryption conn_enc_type;

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://xy:123456abc@127.0.0.1:11398/?replicaSet=rs0&tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://127.0.0.1:11298/?replicaSet=rs0&tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);

#	endif // MONGODB_USING_AUTH

#else

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://xy:123456abc@127.0.0.1:11198/?replicaSet=rs0&tls=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://127.0.0.1:11098/?replicaSet=rs0&tls=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		client.clear();
		return;
	}

	remove_scheam_file();
	clear_encryption_all_datas(client);

	create_master_key_file(g_master_key_fname/*, true*/);
	test_client_encryption_create_connection_encryption(client);

	conn_enc_type conn_enc;
	yggr_test_assert(!conn_enc.validate());
	
	create_connection_encryption(conn_enc, client);
	
	yggr_test_assert(conn_enc.validate());

	test_client_encryption_rewrap_many_datakey(conn_enc);
	test_client_encryption_create_and_delete_key(conn_enc);

	test_client_encryption_exception_encrypt_decrypt(conn_enc);
	test_client_encryption_exception_encrypt_expression(conn_enc);

	test_client_encryption_create_datakey_and_schema(conn_enc);
	test_client_encryption_create_encrypted_collection(conn_enc);

	test_client_encryption_get_key(conn_enc);
	test_client_encryption_get_key_by_key_alt_name(conn_enc);
	test_client_encryption_get_keys(conn_enc);
	test_client_encryption_add_and_remove_key_alt_name(conn_enc);

	test_client_encryption_insert(client);

	test_client_get_crypt_shared_version(client);
	test_client_encryption_get_crypt_shared_version(conn_enc);

	client.clear();

	std::cout << "---------test_other_foo end--------------" << std::endl;
}

void test_shared_ptr_client_encryption_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection_encryption conn_enc_type;

	typedef yggr::nsql_database_system::c_mongo_connection conn_type;
	typedef yggr::shared_ptr<conn_type> conn_ptr_type;

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	conn_ptr_type pclient(yggr_nothrow_new conn_type(
				"mongodb://xy:123456abc@127.0.0.1:11398/?replicaSet=rs0&tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			));
#	else
	conn_ptr_type pclient(yggr_nothrow_new conn_type(
				"mongodb://127.0.0.1:11298/?replicaSet=rs0&tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			));

#	endif // MONGODB_USING_AUTH

#else

#	if MONGODB_USING_AUTH()
	conn_ptr_type pclient(yggr_nothrow_new conn_type(
				"mongodb://xy:123456abc@127.0.0.1:11198/?replicaSet=rs0&tls=false"	// str_uri
			));
#	else
	conn_ptr_type pclient(yggr_nothrow_new conn_type(
				"mongodb://127.0.0.1:11098/?replicaSet=rs0&tls=false"	// str_uri
			));
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	if(!pclient)
	{
		std::cerr << "client alloc failed" << std::endl;
		return;
	}

	if(!(*pclient))
	{
		std::cerr << "Failed to parse URI." << std::endl;
		pclient->clear();
		return;
	}

	remove_scheam_file();
	clear_encryption_all_datas(*pclient);

	create_master_key_file(g_master_key_fname/*, true*/);
	test_client_encryption_create_connection_encryption(pclient);

	{
		conn_enc_type conn_enc;
		yggr_test_assert(!conn_enc.validate());
	
		create_connection_encryption_for_encrypt_expression(conn_enc, pclient);
	
		yggr_test_assert(conn_enc.validate());

		test_client_encryption_create_datakey_and_schema(conn_enc);

		//test_client_encryption_insert(client);

		//test_client_encryption_exception_encrypt_expression(conn_enc);
	}


	pclient->clear();

	std::cout << "---------test_other_foo end--------------" << std::endl;
}

int main(int argc, char *argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();

#if defined(YGGR_SYSTEM_64)
	test_client_encryption_foo();
	test_shared_ptr_client_encryption_foo();
#else
	std::cout << "32bit exe not supprot auto encryption, because mongo_crypt_v1.dll not has 32bit version";
#endif// YGGR_SYSTEM_64

	yggr::nsql_database_system::mongodb_installer::uninstall();
	
	std::cout << "--------------all end-----------------" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}
