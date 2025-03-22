// c_mongo_auto_encryption_opts_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_auto_encryption_opts.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

struct kms_credentials_provider_userdata
{
	int val;
};

bool kms_credentials_provider_callback(void *userdata, const bson_t *params, bson_t *out, bson_error_t *error)
{
	return false;
}

// test
void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_auto_encryption_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	{
		opts_type opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client_pool);

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_db);
		yggr_test_assert(opts_cref.var_keyvault_db().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_coll);
		yggr_test_assert(opts_cref.var_keyvault_coll().empty());

		yggr_test_assert(opts_cref.keyvault_namespace().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->kms_providers);
		yggr_test_assert(opts_cref.var_kms_providers().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->tls_opts);
		yggr_test_assert(opts_cref.var_tls_opts().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->schema_map);
		yggr_test_assert(opts_cref.var_schema_map().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->encrypted_fields_map);
		yggr_test_assert(opts_cref.var_encrypted_fields_map().empty());

		yggr_test_assert(!opts.var_bypass_auto_encryption());
		yggr_test_assert(!opts.var_bypass_query_analysis());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->creds_cb.fn);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->creds_cb.userdata);

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->extra);
		yggr_test_assert(opts_cref.var_extra().empty());
	}

	{
		opts_type opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } } }"),
			bson_type(),
			bson_type(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\" }"),
			bson_type(),
			false,
			false,
			bson_type("{\"extra_filed\" : \"extra_value\"}") );

		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_db);
		yggr_test_assert(!opts_cref.var_keyvault_db().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_coll);
		yggr_test_assert(!opts_cref.var_keyvault_coll().empty());

		yggr_test_assert(!opts_cref.keyvault_namespace().empty());

		std::cout << opts_cref.keyvault_namespace() << std::endl;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->kms_providers);
		yggr_test_assert(!opts_cref.var_kms_providers().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->tls_opts);
		yggr_test_assert(opts_cref.var_tls_opts().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->schema_map);
		yggr_test_assert(!opts_cref.var_schema_map().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->encrypted_fields_map);
		yggr_test_assert(opts_cref.var_encrypted_fields_map().empty());

		yggr_test_assert(!opts.var_bypass_auto_encryption());
		yggr_test_assert(!opts.var_bypass_query_analysis());

		yggr_test_assert(!opts.creds_cb_fn());
		yggr_test_assert(!opts.creds_cb_userdata());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->extra);
		yggr_test_assert(!opts_cref.var_extra().empty());
	}

	{
		kms_credentials_provider_userdata userdata;

		opts_type opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(),
			bson_type(),
			bson_type(),
			bson_type(),
			false,
			false,
			bson_type(),
			&kms_credentials_provider_callback,
			&userdata);

		yggr_test_assert(opts.creds_cb_fn());
		yggr_test_assert(opts.creds_cb_userdata());
	}

	{
		kms_credentials_provider_userdata userdata;

		opts_type opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(),
			bson_type(),
			bson_type(),
			bson_type(),
			false,
			false,
			bson_type(),
			&kms_credentials_provider_callback,
			userdata);

		yggr_test_assert(opts.creds_cb_fn());
		yggr_test_assert(opts.creds_cb_userdata());
	}

	{
		opts_type sample_opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } } }"),
			bson_type(),
			bson_type(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\" }"),
			bson_type(),
			false,
			false,
			bson_type("{\"extra_filed\" : \"extra_value\"}") );

		opts_type opts(sample_opts.org_pointer());

		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_db);
		yggr_test_assert(!opts_cref.var_keyvault_db().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_coll);
		yggr_test_assert(!opts_cref.var_keyvault_coll().empty());

		yggr_test_assert(!opts_cref.keyvault_namespace().empty());

		std::cout << opts_cref.keyvault_namespace() << std::endl;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->kms_providers);
		yggr_test_assert(!opts_cref.var_kms_providers().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->tls_opts);
		yggr_test_assert(opts_cref.var_tls_opts().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->schema_map);
		yggr_test_assert(!opts_cref.var_schema_map().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->encrypted_fields_map);
		yggr_test_assert(opts_cref.var_encrypted_fields_map().empty());

		yggr_test_assert(!opts.var_bypass_auto_encryption());
		yggr_test_assert(!opts.var_bypass_query_analysis());

		yggr_test_assert(!opts.creds_cb_fn());
		yggr_test_assert(!opts.creds_cb_userdata());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->extra);
		yggr_test_assert(!opts_cref.var_extra().empty());
	}

	{
		opts_type sample_opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } } }"),
			bson_type(),
			bson_type(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\" }"),
			bson_type(),
			false,
			false,
			bson_type("{\"extra_filed\" : \"extra_value\"}") );

		opts_type opts(*sample_opts.org_pointer());

		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_db);
		yggr_test_assert(!opts_cref.var_keyvault_db().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_coll);
		yggr_test_assert(!opts_cref.var_keyvault_coll().empty());

		yggr_test_assert(!opts_cref.keyvault_namespace().empty());

		std::cout << opts_cref.keyvault_namespace() << std::endl;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->kms_providers);
		yggr_test_assert(!opts_cref.var_kms_providers().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->tls_opts);
		yggr_test_assert(opts_cref.var_tls_opts().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->schema_map);
		yggr_test_assert(!opts_cref.var_schema_map().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->encrypted_fields_map);
		yggr_test_assert(opts_cref.var_encrypted_fields_map().empty());

		yggr_test_assert(!opts.var_bypass_auto_encryption());
		yggr_test_assert(!opts.var_bypass_query_analysis());

		yggr_test_assert(!opts.creds_cb_fn());
		yggr_test_assert(!opts.creds_cb_userdata());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->extra);
		yggr_test_assert(!opts_cref.var_extra().empty());
	}

	{
		opts_type sample_opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } } }"),
			bson_type(),
			bson_type(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\" }"),
			bson_type(),
			false,
			false,
			bson_type("{\"extra_filed\" : \"extra_value\"}") );

		opts_type opts(boost::move(sample_opts));

		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_db);
		yggr_test_assert(!opts_cref.var_keyvault_db().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_coll);
		yggr_test_assert(!opts_cref.var_keyvault_coll().empty());

		yggr_test_assert(!opts_cref.keyvault_namespace().empty());

		std::cout << opts_cref.keyvault_namespace() << std::endl;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->kms_providers);
		yggr_test_assert(!opts_cref.var_kms_providers().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->tls_opts);
		yggr_test_assert(opts_cref.var_tls_opts().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->schema_map);
		yggr_test_assert(!opts_cref.var_schema_map().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->encrypted_fields_map);
		yggr_test_assert(opts_cref.var_encrypted_fields_map().empty());

		yggr_test_assert(!opts.var_bypass_auto_encryption());
		yggr_test_assert(!opts.var_bypass_query_analysis());

		yggr_test_assert(!opts.creds_cb_fn());
		yggr_test_assert(!opts.creds_cb_userdata());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->extra);
		yggr_test_assert(!opts_cref.var_extra().empty());
	}

	{
		opts_type sample_opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } } }"),
			bson_type(),
			bson_type(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\" }"),
			bson_type(),
			false,
			false,
			bson_type("{\"extra_filed\" : \"extra_value\"}") );

		opts_type opts(sample_opts);

		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_db);
		yggr_test_assert(!opts_cref.var_keyvault_db().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_coll);
		yggr_test_assert(!opts_cref.var_keyvault_coll().empty());

		yggr_test_assert(!opts_cref.keyvault_namespace().empty());

		std::cout << opts_cref.keyvault_namespace() << std::endl;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->kms_providers);
		yggr_test_assert(!opts_cref.var_kms_providers().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->tls_opts);
		yggr_test_assert(opts_cref.var_tls_opts().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->schema_map);
		yggr_test_assert(!opts_cref.var_schema_map().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->encrypted_fields_map);
		yggr_test_assert(opts_cref.var_encrypted_fields_map().empty());

		yggr_test_assert(!opts.var_bypass_auto_encryption());
		yggr_test_assert(!opts.var_bypass_query_analysis());

		yggr_test_assert(!opts.creds_cb_fn());
		yggr_test_assert(!opts.creds_cb_userdata());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->extra);
		yggr_test_assert(!opts_cref.var_extra().empty());
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_auto_encryption_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	{
		opts_type sample_opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } } }"),
			bson_type(),
			bson_type(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\" }"),
			bson_type(),
			false,
			false,
			bson_type("{\"extra_filed\" : \"extra_value\"}") );

		opts_type opts;
		opts = (*sample_opts.org_pointer());

		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_db);
		yggr_test_assert(!opts_cref.var_keyvault_db().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_coll);
		yggr_test_assert(!opts_cref.var_keyvault_coll().empty());

		yggr_test_assert(!opts_cref.keyvault_namespace().empty());

		std::cout << opts_cref.keyvault_namespace() << std::endl;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->kms_providers);
		yggr_test_assert(!opts_cref.var_kms_providers().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->tls_opts);
		yggr_test_assert(opts_cref.var_tls_opts().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->schema_map);
		yggr_test_assert(!opts_cref.var_schema_map().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->encrypted_fields_map);
		yggr_test_assert(opts_cref.var_encrypted_fields_map().empty());

		yggr_test_assert(!opts.var_bypass_auto_encryption());
		yggr_test_assert(!opts.var_bypass_query_analysis());

		yggr_test_assert(!opts.creds_cb_fn());
		yggr_test_assert(!opts.creds_cb_userdata());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->extra);
		yggr_test_assert(!opts_cref.var_extra().empty());
	}

	{
		opts_type sample_opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } } }"),
			bson_type(),
			bson_type(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\" }"),
			bson_type(),
			false,
			false,
			bson_type("{\"extra_filed\" : \"extra_value\"}") );

		opts_type opts;
		opts = (*reinterpret_cast<base_opts_type*>(sample_opts.org_pointer()));

		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_db);
		yggr_test_assert(!opts_cref.var_keyvault_db().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_coll);
		yggr_test_assert(!opts_cref.var_keyvault_coll().empty());

		yggr_test_assert(!opts_cref.keyvault_namespace().empty());

		std::cout << opts_cref.keyvault_namespace() << std::endl;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->kms_providers);
		yggr_test_assert(!opts_cref.var_kms_providers().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->tls_opts);
		yggr_test_assert(opts_cref.var_tls_opts().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->schema_map);
		yggr_test_assert(!opts_cref.var_schema_map().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->encrypted_fields_map);
		yggr_test_assert(opts_cref.var_encrypted_fields_map().empty());

		yggr_test_assert(!opts.var_bypass_auto_encryption());
		yggr_test_assert(!opts.var_bypass_query_analysis());

		yggr_test_assert(!opts.creds_cb_fn());
		yggr_test_assert(!opts.creds_cb_userdata());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->extra);
		yggr_test_assert(!opts_cref.var_extra().empty());
	}

	{
		opts_type sample_opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } } }"),
			bson_type(),
			bson_type(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\" }"),
			bson_type(),
			false,
			false,
			bson_type("{\"extra_filed\" : \"extra_value\"}") );

		opts_type opts;
		opts = (boost::move(sample_opts));

		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_db);
		yggr_test_assert(!opts_cref.var_keyvault_db().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_coll);
		yggr_test_assert(!opts_cref.var_keyvault_coll().empty());

		yggr_test_assert(!opts_cref.keyvault_namespace().empty());

		std::cout << opts_cref.keyvault_namespace() << std::endl;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->kms_providers);
		yggr_test_assert(!opts_cref.var_kms_providers().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->tls_opts);
		yggr_test_assert(opts_cref.var_tls_opts().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->schema_map);
		yggr_test_assert(!opts_cref.var_schema_map().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->encrypted_fields_map);
		yggr_test_assert(opts_cref.var_encrypted_fields_map().empty());

		yggr_test_assert(!opts.var_bypass_auto_encryption());
		yggr_test_assert(!opts.var_bypass_query_analysis());

		yggr_test_assert(!opts.creds_cb_fn());
		yggr_test_assert(!opts.creds_cb_userdata());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->extra);
		yggr_test_assert(!opts_cref.var_extra().empty());
	}

	{
		opts_type sample_opts(
			"keyvault_db",
			"keyvault_coll",
			bson_type(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } } }"),
			bson_type(),
			bson_type(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\" }"),
			bson_type(),
			false,
			false,
			bson_type("{\"extra_filed\" : \"extra_value\"}") );

		opts_type opts;
		opts = (sample_opts);

		const opts_type& opts_cref = opts;

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client);
		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_db);
		yggr_test_assert(!opts_cref.var_keyvault_db().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->keyvault_coll);
		yggr_test_assert(!opts_cref.var_keyvault_coll().empty());

		yggr_test_assert(!opts_cref.keyvault_namespace().empty());

		std::cout << opts_cref.keyvault_namespace() << std::endl;

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->kms_providers);
		yggr_test_assert(!opts_cref.var_kms_providers().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->tls_opts);
		yggr_test_assert(opts_cref.var_tls_opts().empty());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->schema_map);
		yggr_test_assert(!opts_cref.var_schema_map().empty());

		yggr_test_assert(!reinterpret_cast<base_opts_type*>(boost::addressof(opts))->encrypted_fields_map);
		yggr_test_assert(opts_cref.var_encrypted_fields_map().empty());

		yggr_test_assert(!opts.var_bypass_auto_encryption());
		yggr_test_assert(!opts.var_bypass_query_analysis());

		yggr_test_assert(!opts.creds_cb_fn());
		yggr_test_assert(!opts.creds_cb_userdata());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(boost::addressof(opts))->extra);
		yggr_test_assert(!opts_cref.var_extra().empty());
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

bool verify_auto_encryption_opts(const yggr::nsql_database_system::c_mongo_auto_encryption_opts& opts)
{
	typedef yggr::nsql_database_system::c_mongo_auto_encryption_opts opts_type;
	typedef opts_type::base_type base_opts_type;

	const base_opts_type* pbase = reinterpret_cast<const base_opts_type*>(boost::addressof(opts));

	bool bchk = true;

	bchk =
		bchk
		&& ((pbase->keyvault_db && opts.var_keyvault_db().size() && opts.var_keyvault_db().data() == pbase->keyvault_db )
			|| (pbase->keyvault_db == 0 && opts.var_keyvault_db().empty()) );

	yggr_test_assert(bchk);

	bchk =
		bchk
		&& ((pbase->keyvault_coll && opts.var_keyvault_coll().size() && opts.var_keyvault_coll().data() == pbase->keyvault_coll)
			|| (pbase->keyvault_coll == 0 && opts.var_keyvault_coll().empty()) );

	yggr_test_assert(bchk);

	bchk =
		bchk
		&& ((pbase->kms_providers && opts.var_kms_providers().size() && opts.var_kms_providers().org_pointer() == pbase->kms_providers)
			|| (pbase->kms_providers == 0 && opts.var_kms_providers().empty()) );

	yggr_test_assert(bchk);

	bchk =
		bchk
		&& ((pbase->tls_opts && opts.var_tls_opts().size() && opts.var_tls_opts().org_pointer() == pbase->tls_opts)
			|| (pbase->tls_opts == 0 && opts.var_tls_opts().empty()) );

	yggr_test_assert(bchk);

	bchk =
		bchk
		&& ((pbase->schema_map && opts.var_schema_map().size() && opts.var_schema_map().org_pointer() == pbase->schema_map)
			|| (pbase->schema_map == 0 && opts.var_schema_map().empty()) );

	yggr_test_assert(bchk);

	bchk =
		bchk
		&& ((pbase->encrypted_fields_map && opts.var_encrypted_fields_map().size() && opts.var_encrypted_fields_map().org_pointer() == pbase->encrypted_fields_map)
			|| (pbase->encrypted_fields_map == 0 && opts.var_encrypted_fields_map().empty()) );

	yggr_test_assert(bchk);

	bchk =
		bchk
		&& ((pbase->extra && opts.var_extra().size() && opts.var_extra().org_pointer() == pbase->extra)
			|| (pbase->extra == 0 && opts.var_extra().empty()) );

	yggr_test_assert(bchk);

	return bchk;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_auto_encryption_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;

	yggr::utf8_string str_keyvault_db_1 = "keyvault_db_1";
	yggr::utf8_string str_keyvault_db_2 = "keyvault_db_2";

	yggr::utf8_string str_keyvault_coll_1 = "keyvault_coll_1";
	yggr::utf8_string str_keyvault_coll_2 = "keyvault_coll_2";

	bson_type bs_kms_providers_1(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } }, "
					"\"test_mark\" : 1 }");

	bson_type bs_kms_providers_2(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } }, "
					"\"test_mark\" : 2 }");

	bson_type bs_schema_map_1(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\", "
				" \"test_mark\" : 1 }");

	bson_type bs_schema_map_2(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\", "
				" \"test_mark\" : 2 }");

	bson_type bs_extra_1("{\"extra_filed\" : \"extra_value_1\"}");
	bson_type bs_extra_2("{\"extra_filed\" : \"extra_value_2\"}");

	opts_type sample_opts_1(
				str_keyvault_db_1,
				str_keyvault_coll_1,
				bs_kms_providers_1,
				bson_type(),
				bs_schema_map_1,
				bson_type(),
				false,
				false,
				bs_extra_1 );

	opts_type sample_opts_2(
				str_keyvault_db_2,
				str_keyvault_coll_2,
				bs_kms_providers_2,
				bson_type(),
				bs_schema_map_2,
				bson_type(),
				true,
				true,
				bs_extra_2 );

	//const opts_type& opts_sample_cref1 = sample_opts_1;
	const opts_type& opts_sample_cref2 = sample_opts_2;

	// .swap(move)
	{
		opts_type opts1(sample_opts_1);
		opts_type opts2(sample_opts_2);

		const opts_type& opts_cref1 = opts1;
		//const opts_type& opts_cref2 = opts2;

		opts1.swap(boost::move(opts2));

		yggr_test_assert(verify_auto_encryption_opts(opts1));
		yggr_test_assert(verify_auto_encryption_opts(opts2));

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_client
				== reinterpret_cast<base_opts_type*>(sample_opts_2.org_pointer())->keyvault_client);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_client_pool
				== reinterpret_cast<base_opts_type*>(sample_opts_2.org_pointer())->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_db == opts_sample_cref2.var_keyvault_db());
		yggr_test_assert(opts_cref1.var_keyvault_db() == opts_sample_cref2.var_keyvault_db());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_coll == opts_sample_cref2.var_keyvault_coll());
		yggr_test_assert(opts_cref1.var_keyvault_coll() == opts_sample_cref2.var_keyvault_coll());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->kms_providers == opts_sample_cref2.var_kms_providers());
		yggr_test_assert(opts_cref1.var_kms_providers() == opts_sample_cref2.var_kms_providers());

		yggr_test_assert(opts_cref1.var_tls_opts() == opts_sample_cref2.var_tls_opts());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->schema_map == opts_sample_cref2.var_schema_map());
		yggr_test_assert(opts_cref1.var_schema_map() == opts_sample_cref2.var_schema_map());

		yggr_test_assert(opts_cref1.var_encrypted_fields_map() == opts_sample_cref2.var_encrypted_fields_map());

		yggr_test_assert(opts_cref1.var_bypass_auto_encryption() == opts_sample_cref2.var_bypass_auto_encryption());
		yggr_test_assert(opts_cref1.var_bypass_query_analysis() == opts_sample_cref2.var_bypass_query_analysis());

		yggr_test_assert(opts_cref1.creds_cb_fn() == opts_sample_cref2.creds_cb_fn());
		yggr_test_assert(opts_cref1.creds_cb_userdata() == opts_sample_cref2.creds_cb_userdata());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->extra == opts_sample_cref2.var_extra());
		yggr_test_assert(opts_cref1.var_extra() == opts_sample_cref2.var_extra());
	}

	//.swap
	{
		opts_type opts1(sample_opts_1);
		opts_type opts2(sample_opts_2);

		const opts_type& opts_cref1 = opts1;
		//const opts_type& opts_cref2 = opts2;

		opts1.swap(opts2);

		yggr_test_assert(verify_auto_encryption_opts(opts1));
		yggr_test_assert(verify_auto_encryption_opts(opts2));

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_client
				== reinterpret_cast<base_opts_type*>(sample_opts_2.org_pointer())->keyvault_client);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_client_pool
				== reinterpret_cast<base_opts_type*>(sample_opts_2.org_pointer())->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_db == opts_sample_cref2.var_keyvault_db());
		yggr_test_assert(opts_cref1.var_keyvault_db() == opts_sample_cref2.var_keyvault_db());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_coll == opts_sample_cref2.var_keyvault_coll());
		yggr_test_assert(opts_cref1.var_keyvault_coll() == opts_sample_cref2.var_keyvault_coll());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->kms_providers == opts_sample_cref2.var_kms_providers());
		yggr_test_assert(opts_cref1.var_kms_providers() == opts_sample_cref2.var_kms_providers());

		yggr_test_assert(opts_cref1.var_tls_opts() == opts_sample_cref2.var_tls_opts());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->schema_map == opts_sample_cref2.var_schema_map());
		yggr_test_assert(opts_cref1.var_schema_map() == opts_sample_cref2.var_schema_map());

		yggr_test_assert(opts_cref1.var_encrypted_fields_map() == opts_sample_cref2.var_encrypted_fields_map());

		yggr_test_assert(opts_cref1.var_bypass_auto_encryption() == opts_sample_cref2.var_bypass_auto_encryption());
		yggr_test_assert(opts_cref1.var_bypass_query_analysis() == opts_sample_cref2.var_bypass_query_analysis());

		yggr_test_assert(opts_cref1.creds_cb_fn() == opts_sample_cref2.creds_cb_fn());
		yggr_test_assert(opts_cref1.creds_cb_userdata() == opts_sample_cref2.creds_cb_userdata());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->extra == opts_sample_cref2.var_extra());
		yggr_test_assert(opts_cref1.var_extra() == opts_sample_cref2.var_extra());
	}

	//std::swap
	{
		opts_type opts1(sample_opts_1);
		opts_type opts2(sample_opts_2);

		const opts_type& opts_cref1 = opts1;
		//const opts_type& opts_cref2 = opts2;

		std::swap(opts1, opts2);
		std::swap(boost::move(opts1), opts2);
		std::swap(opts1, boost::move(opts2));

		yggr_test_assert(verify_auto_encryption_opts(opts1));
		yggr_test_assert(verify_auto_encryption_opts(opts2));

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_client
				== reinterpret_cast<base_opts_type*>(sample_opts_2.org_pointer())->keyvault_client);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_client_pool
				== reinterpret_cast<base_opts_type*>(sample_opts_2.org_pointer())->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_db == opts_sample_cref2.var_keyvault_db());
		yggr_test_assert(opts_cref1.var_keyvault_db() == opts_sample_cref2.var_keyvault_db());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_coll == opts_sample_cref2.var_keyvault_coll());
		yggr_test_assert(opts_cref1.var_keyvault_coll() == opts_sample_cref2.var_keyvault_coll());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->kms_providers == opts_sample_cref2.var_kms_providers());
		yggr_test_assert(opts_cref1.var_kms_providers() == opts_sample_cref2.var_kms_providers());

		yggr_test_assert(opts_cref1.var_tls_opts() == opts_sample_cref2.var_tls_opts());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->schema_map == opts_sample_cref2.var_schema_map());
		yggr_test_assert(opts_cref1.var_schema_map() == opts_sample_cref2.var_schema_map());

		yggr_test_assert(opts_cref1.var_encrypted_fields_map() == opts_sample_cref2.var_encrypted_fields_map());

		yggr_test_assert(opts_cref1.var_bypass_auto_encryption() == opts_sample_cref2.var_bypass_auto_encryption());
		yggr_test_assert(opts_cref1.var_bypass_query_analysis() == opts_sample_cref2.var_bypass_query_analysis());

		yggr_test_assert(opts_cref1.creds_cb_fn() == opts_sample_cref2.creds_cb_fn());
		yggr_test_assert(opts_cref1.creds_cb_userdata() == opts_sample_cref2.creds_cb_userdata());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->extra == opts_sample_cref2.var_extra());
		yggr_test_assert(opts_cref1.var_extra() == opts_sample_cref2.var_extra());
	}

	//boost::swap
	{
		opts_type opts1(sample_opts_1);
		opts_type opts2(sample_opts_2);

		const opts_type& opts_cref1 = opts1;
		//const opts_type& opts_cref2 = opts2;

		boost::swap(opts1, opts2);
		boost::swap(boost::move(opts1), opts2);
		boost::swap(opts1, boost::move(opts2));

		yggr_test_assert(verify_auto_encryption_opts(opts1));
		yggr_test_assert(verify_auto_encryption_opts(opts2));

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_client
				== reinterpret_cast<base_opts_type*>(sample_opts_2.org_pointer())->keyvault_client);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_client_pool
				== reinterpret_cast<base_opts_type*>(sample_opts_2.org_pointer())->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_db == opts_sample_cref2.var_keyvault_db());
		yggr_test_assert(opts_cref1.var_keyvault_db() == opts_sample_cref2.var_keyvault_db());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_coll == opts_sample_cref2.var_keyvault_coll());
		yggr_test_assert(opts_cref1.var_keyvault_coll() == opts_sample_cref2.var_keyvault_coll());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->kms_providers == opts_sample_cref2.var_kms_providers());
		yggr_test_assert(opts_cref1.var_kms_providers() == opts_sample_cref2.var_kms_providers());

		yggr_test_assert(opts_cref1.var_tls_opts() == opts_sample_cref2.var_tls_opts());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->schema_map == opts_sample_cref2.var_schema_map());
		yggr_test_assert(opts_cref1.var_schema_map() == opts_sample_cref2.var_schema_map());

		yggr_test_assert(opts_cref1.var_encrypted_fields_map() == opts_sample_cref2.var_encrypted_fields_map());

		yggr_test_assert(opts_cref1.var_bypass_auto_encryption() == opts_sample_cref2.var_bypass_auto_encryption());
		yggr_test_assert(opts_cref1.var_bypass_query_analysis() == opts_sample_cref2.var_bypass_query_analysis());

		yggr_test_assert(opts_cref1.creds_cb_fn() == opts_sample_cref2.creds_cb_fn());
		yggr_test_assert(opts_cref1.creds_cb_userdata() == opts_sample_cref2.creds_cb_userdata());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->extra == opts_sample_cref2.var_extra());
		yggr_test_assert(opts_cref1.var_extra() == opts_sample_cref2.var_extra());
	}

	//yggr::swap
	{
		opts_type opts1(sample_opts_1);
		opts_type opts2(sample_opts_2);

		const opts_type& opts_cref1 = opts1;
		//const opts_type& opts_cref2 = opts2;

		yggr::swap(opts1, opts2);
		yggr::swap(boost::move(opts1), opts2);
		yggr::swap(opts1, boost::move(opts2));

		yggr_test_assert(verify_auto_encryption_opts(opts1));
		yggr_test_assert(verify_auto_encryption_opts(opts2));

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_client
				== reinterpret_cast<base_opts_type*>(sample_opts_2.org_pointer())->keyvault_client);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_client_pool
				== reinterpret_cast<base_opts_type*>(sample_opts_2.org_pointer())->keyvault_client_pool);

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_db == opts_sample_cref2.var_keyvault_db());
		yggr_test_assert(opts_cref1.var_keyvault_db() == opts_sample_cref2.var_keyvault_db());

		yggr_test_assert(reinterpret_cast<base_opts_type*>(opts1.org_pointer())->keyvault_coll == opts_sample_cref2.var_keyvault_coll());
		yggr_test_assert(opts_cref1.var_keyvault_coll() == opts_sample_cref2.var_keyvault_coll());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->kms_providers == opts_sample_cref2.var_kms_providers());
		yggr_test_assert(opts_cref1.var_kms_providers() == opts_sample_cref2.var_kms_providers());

		yggr_test_assert(opts_cref1.var_tls_opts() == opts_sample_cref2.var_tls_opts());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->schema_map == opts_sample_cref2.var_schema_map());
		yggr_test_assert(opts_cref1.var_schema_map() == opts_sample_cref2.var_schema_map());

		yggr_test_assert(opts_cref1.var_encrypted_fields_map() == opts_sample_cref2.var_encrypted_fields_map());

		yggr_test_assert(opts_cref1.var_bypass_auto_encryption() == opts_sample_cref2.var_bypass_auto_encryption());
		yggr_test_assert(opts_cref1.var_bypass_query_analysis() == opts_sample_cref2.var_bypass_query_analysis());

		yggr_test_assert(opts_cref1.creds_cb_fn() == opts_sample_cref2.creds_cb_fn());
		yggr_test_assert(opts_cref1.creds_cb_userdata() == opts_sample_cref2.creds_cb_userdata());

		yggr_test_assert(*reinterpret_cast<base_opts_type*>(opts1.org_pointer())->extra == opts_sample_cref2.var_extra());
		yggr_test_assert(opts_cref1.var_extra() == opts_sample_cref2.var_extra());
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;

	typedef yggr::nsql_database_system::c_mongo_auto_encryption_opts opts_type;
	typedef opts_type::base_type base_opts_type;
	typedef opts_type::org_type org_opts_type;
	typedef opts_type::mc_kms_credentials_callback_type mc_kms_credentials_callback_type;

	yggr::utf8_string str_keyvault_db_1 = "keyvault_db_1";
	yggr::utf8_string str_keyvault_db_2 = "keyvault_db_2";

	yggr::utf8_string str_keyvault_coll_1 = "keyvault_coll_1";
	yggr::utf8_string str_keyvault_coll_2 = "keyvault_coll_2";

	yggr::utf8_string str_keyvault_namespace_1 = str_keyvault_db_1 + "." + str_keyvault_coll_1;
	yggr::utf8_string str_keyvault_namespace_2 = str_keyvault_db_2 + "." + str_keyvault_coll_2;

	bson_type bs_kms_providers_1(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } }, "
					"\"test_mark\" : 1 }");

	bson_type bs_kms_providers_2(
				"{ \"local\" : "
					"{ \"key\" : "
						"{ \"$binary\" : "
								"\"ZjU0MWY3NWZhYjQwNGJmMTVmMzJlOWEyMDIyZDk1ZGY3NWUxNGE5Mjg3OTE3OThiODYxYzhjZTA0MzYwMzE4YTA4ZmVhYTE5MzgwNmViYmZhMWFjZmI4YThkOTAwYzM4\", "
							"\"$type\" : \"00\" } }, "
					"\"test_mark\" : 2 }");

	bson_type bs_schema_map_1(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\", "
				" \"test_mark\" : 1 }");

	bson_type bs_schema_map_2(
				"{ \"properties\" : "
					"{ \"encryptedField\" : "
						"{ \"encrypt\" : "
							"{ \"keyId\" : "
									"[ { \"$binary\" : { \"base64\" : \"a3/VifP/RdquD0x56og8bQ==\", \"subType\" : \"04\" } } ], "
								"\"bsonType\" : \"string\", "
								"\"algorithm\" : \"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic\" } } }, "
							"\"bsonType\" : \"object\", "
				" \"test_mark\" : 2 }");

	bson_type bs_extra_1("{\"extra_filed\" : \"extra_value_1\"}");
	bson_type bs_extra_2("{\"extra_filed\" : \"extra_value_2\"}");

	opts_type sample_opts(
				str_keyvault_db_1,
				str_keyvault_coll_1,
				bs_kms_providers_1,
				bson_type(),
				bs_schema_map_1,
				bson_type(),
				false,
				false,
				bs_extra_1);

	// var_keyvault_db
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_ref.var_keyvault_db() == str_keyvault_db_1);

		opts_ref.var_keyvault_db() = str_keyvault_db_2;

		yggr_test_assert(opts_cref.var_keyvault_db() == str_keyvault_db_2);
		yggr_test_assert(opts_cref.var_keyvault_db<std::string>() == str_keyvault_db_2);
		yggr_test_assert(opts_cref.var_keyvault_db<std::string>(std::string().get_allocator()) == str_keyvault_db_2);

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// keyvault_namespace
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_ref.keyvault_namespace() == str_keyvault_namespace_1);

		opts_ref.keyvault_namespace() = str_keyvault_namespace_2;

		yggr_test_assert(opts_cref.keyvault_namespace() == str_keyvault_namespace_2);
		yggr_test_assert(opts_cref.keyvault_namespace<std::string>() == str_keyvault_namespace_2);
		yggr_test_assert(opts_cref.keyvault_namespace<std::string>(std::string().get_allocator()) == str_keyvault_namespace_2);

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// var_keyvault_coll
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_ref.var_keyvault_coll() == str_keyvault_coll_1);

		opts_ref.var_keyvault_coll() = str_keyvault_coll_2;

		yggr_test_assert(opts_cref.var_keyvault_coll() == str_keyvault_coll_2);
		yggr_test_assert(opts_cref.var_keyvault_coll<std::string>() == str_keyvault_coll_2);
		yggr_test_assert(opts_cref.var_keyvault_coll<std::string>(std::string().get_allocator()) == str_keyvault_coll_2);

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// var_kms_providers
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_ref.var_kms_providers() == bs_kms_providers_1);

		opts_ref.var_kms_providers() = *(bs_kms_providers_2.org_pointer());
		opts_ref.var_kms_providers() = bs_kms_providers_2;

		yggr_test_assert(opts_cref.var_kms_providers() == bs_kms_providers_2);

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// var_tls_opts
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_ref.var_tls_opts() == bson_type());
		yggr_test_assert(!(reinterpret_cast<base_opts_type*>(opts_ref.org_pointer())->tls_opts));

		opts_ref.var_tls_opts() = bson_type();

		yggr_test_assert(opts_cref.var_tls_opts() == bson_type());
		yggr_test_assert(!(reinterpret_cast<base_opts_type*>(opts_ref.org_pointer())->tls_opts));

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// var_schema_map
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_ref.var_schema_map() == bs_schema_map_1);

		opts_ref.var_schema_map() = *(bs_schema_map_2.org_pointer());
		opts_ref.var_schema_map() = bs_schema_map_2;

		yggr_test_assert(opts_cref.var_schema_map() == bs_schema_map_2);

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// var_encrypted_fields_map
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_ref.var_encrypted_fields_map() == bson_type());
		yggr_test_assert(!(reinterpret_cast<base_opts_type*>(opts_ref.org_pointer())->encrypted_fields_map));

		opts_ref.var_encrypted_fields_map() = bson_type();

		yggr_test_assert(opts_cref.var_encrypted_fields_map() == bson_type());
		yggr_test_assert(!(reinterpret_cast<base_opts_type*>(opts_ref.org_pointer())->encrypted_fields_map));

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// var_bypass_auto_encryption
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(!opts_ref.var_bypass_auto_encryption());

		opts_ref.var_bypass_auto_encryption() = true;

		yggr_test_assert(opts_cref.var_bypass_auto_encryption());

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// var_bypass_query_analysis
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(!opts_ref.var_bypass_query_analysis());

		opts_ref.var_bypass_query_analysis() = true;

		yggr_test_assert(opts_cref.var_bypass_query_analysis());

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// var_creds_cb
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(!(opts_ref.var_creds_cb().var_fn()));
		yggr_test_assert(!(opts_ref.var_creds_cb().var_userdata()));

		kms_credentials_provider_userdata userdata = {10};

		opts_ref.var_creds_cb() = mc_kms_credentials_callback_type(&kms_credentials_provider_callback, &userdata);
		opts_ref.var_creds_cb() = mc_kms_credentials_callback_type(&kms_credentials_provider_callback, userdata);

		yggr_test_assert((opts_cref.var_creds_cb().var_fn()));
		yggr_test_assert((opts_cref.var_creds_cb().var_userdata()));

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// creds_cb_fn
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(!(opts_ref.creds_cb_fn()));

		opts_ref.creds_cb_fn() = &kms_credentials_provider_callback;

		yggr_test_assert((opts_cref.creds_cb_fn()));

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// creds_cb_userdata
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(!(opts_ref.creds_cb_userdata()));

		kms_credentials_provider_userdata userdata = {10};
		kms_credentials_provider_userdata userdata2 = {11};

		opts_ref.creds_cb_userdata() = &userdata;

		yggr_test_assert((opts_ref.creds_cb_userdata() == reinterpret_cast<void*>(&userdata)));
		yggr_test_assert((opts_ref.creds_cb_userdata<kms_credentials_provider_userdata>() == &userdata));

		yggr_test_assert((opts_cref.creds_cb_userdata() == reinterpret_cast<void*>(&userdata)));
		yggr_test_assert((opts_cref.creds_cb_userdata<kms_credentials_provider_userdata>() == &userdata));

		opts_ref.creds_cb_userdata<kms_credentials_provider_userdata>() = &userdata2;

		yggr_test_assert((opts_ref.creds_cb_userdata() == reinterpret_cast<void*>(&userdata2)));
		yggr_test_assert((opts_ref.creds_cb_userdata<kms_credentials_provider_userdata>() == &userdata2));

		yggr_test_assert((opts_cref.creds_cb_userdata() == reinterpret_cast<void*>(&userdata2)));
		yggr_test_assert((opts_cref.creds_cb_userdata<kms_credentials_provider_userdata>() == &userdata2));

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	// var_extra
	{
		opts_type opts(sample_opts);

		opts_type& opts_ref = opts;
		const opts_type& opts_cref = opts;

		yggr_test_assert(opts_ref.var_extra() == bs_extra_1);

		opts_ref.var_extra() = *(bs_extra_2.org_pointer());
		opts_ref.var_extra() = bs_extra_2;

		yggr_test_assert(opts_cref.var_extra() == bs_extra_2);

		yggr_test_assert(verify_auto_encryption_opts(opts));
	}

	std::cout << "-----------------test_member_foo end-----------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_member_foo();

	wait_any_key(argc, argv);
	return 0;
}
