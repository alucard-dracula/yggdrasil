// mongo_accesser_encryption_test_of_connect_pool.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <sstream>

#include <yggr/ids/id_n.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>
#include <yggr/nsql_database_system/c_bson_binary_buffer.hpp>
#include <yggr/nsql_database_system/c_bson_binary_buffer_ref.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/c_mongo_connection_pool.hpp>
#include <yggr/nsql_database_system/c_mongo_enc_connection.hpp>
#include <yggr/nsql_database_system/c_mongo_enc_connection_pool.hpp>
#include <yggr/nsql_database_system/c_mongo_connection_encryption.hpp>

#include <yggr/nsql_database_system/mongo_accesser.hpp>
#include <yggr/nsql_database_system/mongodb_kms_providers_maker.hpp>

#include <yggr/file_system/local_fsys_operators.hpp> // must after c_mongo_connection.hpp in win32 release

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

yggr::utf8_string g_master_key_fname = "./nsql_database_system/rs0_enc_master_key.key";

yggr::utf8_string g_scheam_fname = "./nsql_database_system/rs0_enc_schema_pool.json";


yggr::utf8_string g_keyvault_db = "yggr_enc_keyvalut_db_pool";
yggr::utf8_string g_keyvault_coll = "__yggr_enc_keyvalut_coll_pool";

yggr::utf8_string g_enc_data_db = "yggr_enc_data_db_pool";
yggr::utf8_string g_enc_data_coll = "yggr_enc_data_coll_pool";

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


template<typename ConnPool>
void test_client_encryption_insert(ConnPool& conn_pool)
{
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_error error_type;

	//typedef yggr::nsql_database_system::c_mongo_enc_connection mongo_enc_conn_type;
	typedef yggr::nsql_database_system::c_mongo_enc_connection_pool mongo_enc_conn_pool_type;
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

	mongo_enc_conn_pool_type enc_conn_pool;

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
						enc_conn_pool, conn_pool, auto_enc_opts, err);

		yggr_test_assert(bchk);
		yggr_test_assert(enc_conn_pool.validate());
	}

	bson_type ins_data;
	ins_data.save("enc_field_01", "enc_value_01");
	std::cout << ins_data << std::endl;

	error_type err;
	bson_type opts;
	bson_type reply;

	bool bchk = 
		mongo_accesser_type::s_collection_insert(
			enc_conn_pool, enc_data_db, enc_data_coll, ins_data, opts, reply, err);

	if(!bchk)
	{
		std::cout << err << std::endl;
	}

	yggr_test_assert(bchk);

	std::cout << reply << std::endl;

	std::cout << "------test_client_encryption_insert success------" << std::endl;
}

void test_client_encryption_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection_encryption conn_enc_type;

#if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_addr = "192.168.1.101"; // you host ip address
#else
	yggr::utf8_string host_addr = "127.0.0.1";
#endif // YGGR_AT_MOBILE


#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()

#		if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_port = "11798"; // you host ip address
#		else
	yggr::utf8_string host_port = "11398";
#		endif // YGGR_AT_MOBILE

	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://xy:123456abc@" + host_addr + ":" + host_port + "/?replicaSet=rs0&tls=true", //"mongodb://xy:123456abc@127.0.0.1:11398/?replicaSet=rs0&tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);

	yggr::nsql_database_system::c_mongo_connection_pool client_pool(
				"mongodb://xy:123456abc@" + host_addr + ":" + host_port + "/?replicaSet=rs0&tls=true", // "mongodb://xy:123456abc@127.0.0.1:11398/?replicaSet=rs0&tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else

#		if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_port = "11698"; // you host ip address
#		else
	yggr::utf8_string host_port = "11298";
#		endif // YGGR_AT_MOBILE

	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://" + host_addr + ":" + host_port + "/?replicaSet=rs0&tls=true", // "mongodb://127.0.0.1:11298/?replicaSet=rs0&tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);

	yggr::nsql_database_system::c_mongo_connection_pool client_pool(
				"mongodb://" + host_addr + ":" + host_port + "/?replicaSet=rs0&tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);

#	endif // MONGODB_USING_AUTH

#else

#	if MONGODB_USING_AUTH()

#		if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_port = "11598"; // you host ip address
#		else
	yggr::utf8_string host_port = "11198";
#		endif // YGGR_AT_MOBILE

	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://xy:123456abc@" + host_addr + ":" + host_port + "/?replicaSet=rs0&tls=false" //"mongodb://xy:123456abc@127.0.0.1:11198/?replicaSet=rs0&tls=false"	// str_uri
			);

	yggr::nsql_database_system::c_mongo_connection_pool client_pool(
				"mongodb://xy:123456abc@" + host_addr + ":" + host_port + "/?replicaSet=rs0&tls=false" //"mongodb://xy:123456abc@127.0.0.1:11198/?replicaSet=rs0&tls=false"	// str_uri
			);
#	else

#		if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_port = "11498"; // you host ip address
#		else
	yggr::utf8_string host_port = "11098";
#		endif // YGGR_AT_MOBILE

	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://" + host_addr + ":" + host_port + "/?replicaSet=rs0&tls=false" //"mongodb://127.0.0.1:11098/?replicaSet=rs0&tls=false"	// str_uri
			);

	yggr::nsql_database_system::c_mongo_connection_pool client_pool(
				"mongodb://" + host_addr + ":" + host_port + "/?replicaSet=rs0&tls=false" //"mongodb://127.0.0.1:11098/?replicaSet=rs0&tls=false"	// str_uri
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

	conn_enc_type conn_enc;
	yggr_test_assert(!conn_enc.validate());
	
	create_connection_encryption(conn_enc, client);
	
	yggr_test_assert(conn_enc.validate());

	test_client_encryption_create_datakey_and_schema(conn_enc);

	test_client_encryption_insert(client_pool);

	client.clear();
	client_pool.clear();

	std::cout << "---------test_other_foo end--------------" << std::endl;
}

int main(int argc, char *argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();

#if defined(YGGR_SYSTEM_64) && !(defined(YGGR_AT_MOBILE))
	test_client_encryption_foo();
#else
	std::cout << "current platform not supprot auto encryption, because mongo_crypt_v1.dll (so) not supported";
#endif // #if defined(YGGR_SYSTEM_64) && !(defined(YGGR_AT_MOBILE))

	yggr::nsql_database_system::mongodb_installer::uninstall();
	
	std::cout << "--------------all end-----------------" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}