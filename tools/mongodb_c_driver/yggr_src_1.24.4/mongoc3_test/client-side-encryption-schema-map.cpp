
#include <iostream>
#include <fstream>
#include <string>

#include <base64.h>

#include <mongoc/mongoc.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#ifdef _MSC_VER
#	if defined(_DEBUG)
#		if _MSC_VER == 1600
#			pragma comment(lib, "libbson-vc100-d.lib")
#			pragma comment(lib, "libcommon-vc100-d.lib")
#			pragma comment(lib, "libmongoc-vc100-d.lib")
#			pragma comment(lib, "libmongocrypt-vc100-d.lib")
#			pragma comment(lib, "libkms-message-vc100-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc100-d.lib")
#			pragma comment(lib, "libbase64-vc100-d.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libbson-vc110-d.lib")
#			pragma comment(lib, "libcommon-vc110-d.lib")
#			pragma comment(lib, "libmongoc-vc110-d.lib")
#			pragma comment(lib, "libmongocrypt-vc110-d.lib")
#			pragma comment(lib, "libkms-message-vc110-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc110-d.lib")
#			pragma comment(lib, "libbase64-vc110-d.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libbson-vc120-d.lib")
#			pragma comment(lib, "libcommon-vc120-d.lib")
#			pragma comment(lib, "libmongoc-vc120-d.lib")
#			pragma comment(lib, "libmongocrypt-vc120-d.lib")
#			pragma comment(lib, "libkms-message-vc120-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc120-d.lib")
#			pragma comment(lib, "libbase64-vc120-d.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libbson-vc140-d.lib")
#			pragma comment(lib, "libcommon-vc140-d.lib")
#			pragma comment(lib, "libmongoc-vc140-d.lib")
#			pragma comment(lib, "libmongocrypt-vc140-d.lib")
#			pragma comment(lib, "libkms-message-vc140-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc140-d.lib")
#			pragma comment(lib, "libbase64-vc140-d.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libbson-vc141-d.lib")
#			pragma comment(lib, "libcommon-vc141-d.lib")
#			pragma comment(lib, "libmongoc-vc141-d.lib")
#			pragma comment(lib, "libmongocrypt-vc141-d.lib")
#			pragma comment(lib, "libkms-message-vc141-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc141-d.lib")
#			pragma comment(lib, "libbase64-vc141-d.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libbson-vc142-d.lib")
#			pragma comment(lib, "libcommon-vc142-d.lib")
#			pragma comment(lib, "libmongoc-vc142-d.lib")
#			pragma comment(lib, "libmongocrypt-vc142-d.lib")
#			pragma comment(lib, "libkms-message-vc142-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc142-d.lib")
#			pragma comment(lib, "libbase64-vc142-d.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libbson-vc143-d.lib")
#			pragma comment(lib, "libcommon-vc143-d.lib")
#			pragma comment(lib, "libmongoc-vc143-d.lib")
#			pragma comment(lib, "libmongocrypt-vc143-d.lib")
#			pragma comment(lib, "libkms-message-vc143-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc143-d.lib")
#			pragma comment(lib, "libbase64-vc143-d.lib")
#		else
#			error "set libs"
#		endif // ( _MSC_VER == 1600)
#	else // _DEBUG
#		if _MSC_VER == 1600
#			pragma comment(lib, "libbson-vc100.lib")
#			pragma comment(lib, "libcommon-vc100.lib")
#			pragma comment(lib, "libmongoc-vc100.lib")
#			pragma comment(lib, "libmongocrypt-vc100.lib")
#			pragma comment(lib, "libkms-message-vc100.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc100.lib")
#			pragma comment(lib, "libbase64-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libbson-vc110.lib")
#			pragma comment(lib, "libcommon-vc110.lib")
#			pragma comment(lib, "libmongoc-vc110.lib")
#			pragma comment(lib, "libmongocrypt-vc110.lib")
#			pragma comment(lib, "libkms-message-vc110.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc110.lib")
#			pragma comment(lib, "libbase64-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libbson-vc120.lib")
#			pragma comment(lib, "libcommon-vc120.lib")
#			pragma comment(lib, "libmongoc-vc120.lib")
#			pragma comment(lib, "libmongocrypt-vc120.lib")
#			pragma comment(lib, "libkms-message-vc120.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc120.lib")
#			pragma comment(lib, "libbase64-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libbson-vc140.lib")
#			pragma comment(lib, "libcommon-vc140.lib")
#			pragma comment(lib, "libmongoc-vc140.lib")
#			pragma comment(lib, "libmongocrypt-vc140.lib")
#			pragma comment(lib, "libkms-message-vc140.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc140.lib")
#			pragma comment(lib, "libbase64-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libbson-vc141.lib")
#			pragma comment(lib, "libcommon-vc141.lib")
#			pragma comment(lib, "libmongoc-vc141.lib")
#			pragma comment(lib, "libmongocrypt-vc141.lib")
#			pragma comment(lib, "libkms-message-vc141.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc141.lib")
#			pragma comment(lib, "libbase64-vc141.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libbson-vc142.lib")
#			pragma comment(lib, "libcommon-vc142.lib")
#			pragma comment(lib, "libmongoc-vc142.lib")
#			pragma comment(lib, "libmongocrypt-vc142.lib")
#			pragma comment(lib, "libmongocrypt-vc142.lib")
#			pragma comment(lib, "libkms-message-vc142.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc142.lib")
#			pragma comment(lib, "libbase64-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libbson-vc143.lib")
#			pragma comment(lib, "libcommon-vc143.lib")
#			pragma comment(lib, "libmongoc-vc143.lib")
#			pragma comment(lib, "libmongocrypt-vc143.lib")
#			pragma comment(lib, "libmongocrypt-vc143.lib")
#			pragma comment(lib, "libkms-message-vc143.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc143.lib")
#			pragma comment(lib, "libbase64-vc143.lib")
#		else
#			error "set libs"
#		endif // ( _MSC_VER == 1600)
#	endif // _DEBUG
#endif // _MSC_VER

#ifdef _MSC_VER
//#	pragma comment(lib, "ssleay32.lib")
//#	pragma comment(lib, "libeay32.lib")

#	if defined(_DEBUG)
#		pragma comment(lib, "zlibstaticd.lib")
#		pragma comment(lib, "icuucd.lib")
#	else
#		pragma comment(lib, "zlibstatic.lib")
#		pragma comment(lib, "icuuc.lib")
#	endif // _DEBUG

#	pragma comment(lib, "ws2_32.lib")
#	pragma comment(lib, "Dnsapi.lib")
#	pragma comment(lib, "Bcrypt.lib")
#	pragma comment(lib, "Secur32.lib")
#	pragma comment(lib, "Crypt32.lib")
#endif // _MSC_VER

#ifdef _MSC_VER
#	if 1
#		if _MSC_VER == 1600
#			pragma comment(lib, "libcrypto-vc100.lib")
#			pragma comment(lib, "libssl-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libcrypto-vc110.lib")
#			pragma comment(lib, "libssl-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libcrypto-vc120.lib")
#			pragma comment(lib, "libssl-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libcrypto-vc140.lib")
#			pragma comment(lib, "libssl-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libcrypto-vc141.lib")
#			pragma comment(lib, "libssl-vc141.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libcrypto-vc142.lib")
#			pragma comment(lib, "libssl-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libcrypto-vc143.lib")
#			pragma comment(lib, "libssl-vc143.lib")
#		else
#		endif // ( _MSC_VER == 1600)
#	else
#		if _MSC_VER == 1600
#			pragma comment(lib, "libcrypto_static-vc100.lib")
#			pragma comment(lib, "libssl_static-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libcrypto_static-vc110.lib")
#			pragma comment(lib, "libssl_static-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libcrypto_static-vc120.lib")
#			pragma comment(lib, "libssl_static-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libcrypto_static-vc140.lib")
#			pragma comment(lib, "libssl_static-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libcrypto_static-vc141.lib")
#			pragma comment(lib, "libssl_static-vc141.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libcrypto_static-vc142.lib")
#			pragma comment(lib, "libssl_static-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libcrypto_static-vc143.lib")
#			pragma comment(lib, "libssl_static-vc143.lib")
#		else
#		endif // ( _MSC_VER == 1600)
#	endif // 0,1
#endif // _MSC_VER



uint8_t *
hex_to_bin (const char *hex, uint32_t *len)
{
   if (!hex) {
      *len = 0;
      return NULL;
   }
   const size_t hex_len = strlen (hex);
   if (hex_len % 2 != 0) {
      *len = 0;
      return NULL;
   }

   if (!bson_in_range_unsigned (uint32_t, hex_len / 2u)) {
      return NULL;
   }

   *len = (uint32_t) (hex_len / 2u);
   uint8_t *const out = static_cast<uint8_t*>(bson_malloc0 (*len));

   for (size_t i = 0u; i < hex_len; i += 2u) {
      uint32_t hex_char;

      if (1 != sscanf (hex + i, "%2x", &hex_char)) {
         bson_free (out);
         *len = 0;
         return NULL;
      }
      out[i / 2u] = (uint8_t) hex_char;
   }
   return out;
}

bool
print_one_document (mongoc_collection_t *coll, bson_error_t *error)
{
   bool ret = false;
   mongoc_cursor_t *cursor = NULL;
   const bson_t *found;
   bson_t *filter = NULL;
   char *as_string = NULL;

   filter = bson_new ();
   cursor = mongoc_collection_find_with_opts (
      coll, filter, NULL /* opts  */, NULL /* read prefs */);
   if (!mongoc_cursor_next (cursor, &found)) {
      fprintf (stderr, "error: did not find inserted document\n");
      goto fail;
   }
   if (mongoc_cursor_error (cursor, error)) {
      goto fail;
   }
   as_string = bson_as_canonical_extended_json (found, NULL);
   printf ("%s", as_string);

   ret = true;
fail:
   bson_destroy (filter);
   mongoc_cursor_destroy (cursor);
   bson_free (as_string);
   return ret;
}

void out_bson(bson_t& bs)
{
	char* str_json = bson_as_json(&bs, 0);

	std::cout << (str_json? str_json : "empty") << std::endl;

	if(str_json) bson_free(str_json);
}

void out_bson_value(bson_value_t& bs_val)
{
	bson_t out_bs = {0};
	bson_init(&out_bs);

	bson_append_value(&out_bs, "out_bson_value", -1, &bs_val);

	out_bson(out_bs);

	bson_destroy(&out_bs);
}


//#include "client-side-encryption-helpers.h"

/* Helper method to create a new data key in the key vault, a schema to use that
 * key, and writes the schema to a file for later use. */
static bool
create_schema_file (bson_t *kms_providers,
                    const char *keyvault_db,
                    const char *keyvault_coll,
                    mongoc_client_t *keyvault_client,
                    bson_error_t *error)
{
	
   mongoc_client_encryption_t *client_encryption = NULL;
   mongoc_client_encryption_opts_t *client_encryption_opts = NULL;
   mongoc_client_encryption_datakey_opts_t *datakey_opts = NULL;
   bson_value_t datakey_id = {static_cast<bson_type_t>(0)};
   const char *keyaltnames[] = {"mongoc_encryption_example_1"};
   bson_t *schema = NULL;
   char *schema_string = NULL;
   size_t schema_string_len;
   FILE *outfile = NULL;
   bool ret = false;

   client_encryption_opts = mongoc_client_encryption_opts_new ();
   mongoc_client_encryption_opts_set_kms_providers (client_encryption_opts,
                                                    kms_providers);
   mongoc_client_encryption_opts_set_keyvault_namespace (
      client_encryption_opts, keyvault_db, keyvault_coll);
   mongoc_client_encryption_opts_set_keyvault_client (client_encryption_opts,
                                                      keyvault_client);

   client_encryption =
      mongoc_client_encryption_new (client_encryption_opts, error);
   if (!client_encryption) {
      goto fail;
   }


  // {
		//mongoc_collection_t* test_coll = mongoc_client_get_collection(keyvault_client, "test_enc_conn", "foo");
		//assert(test_coll);

		//bson_t* now_to_insert = BCON_NEW ("no_encryptedField", "123456789");

		//out_bson(*now_to_insert);

		//bson_error_t error = {0};
		//ret = mongoc_collection_insert_one (
		//	test_coll, now_to_insert, NULL /* opts */, NULL /* reply */, &error);
		//assert(ret);
		//printf ("decrypted document: ");
		//if (!print_one_document (test_coll, &error)) 
		//{
		//	assert(false);
		//}
		//printf ("\n");

		//bson_destroy(now_to_insert);
		//mongoc_collection_destroy(test_coll);

  // }

   /* Create a new data key and json schema for the encryptedField.
    * https://dochub.mongodb.org/core/client-side-field-level-encryption-automatic-encryption-rules
    */
   datakey_opts = mongoc_client_encryption_datakey_opts_new ();
   mongoc_client_encryption_datakey_opts_set_keyaltnames (
      datakey_opts, (char**)&keyaltnames[0], 1);
   if (!mongoc_client_encryption_create_datakey (
          client_encryption, "local", datakey_opts, &datakey_id, error)) {
      goto fail;
   }

   out_bson_value(datakey_id);

   /* Create a schema describing that "encryptedField" is a string encrypted
    * with the newly created data key using deterministic encryption. */
   schema = BCON_NEW ("properties",
                      "{",
                      "encryptedField",
                      "{",
                      "encrypt",
                      "{",
                      "keyId",
                      "[",
                      BCON_BIN (datakey_id.value.v_binary.subtype,
                                datakey_id.value.v_binary.data,
                                datakey_id.value.v_binary.data_len),
                      "]",
                      "bsonType",
                      "string",
                      "algorithm",
                      MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC,
                      "}",
                      "}",
                      "}",
                      "bsonType",
                      "object");

   /* Use canonical JSON so that other drivers and tools will be
    * able to parse the MongoDB extended JSON file. */
   schema_string = bson_as_canonical_extended_json (schema, &schema_string_len);
   outfile = fopen ("jsonSchema.json", "w");
   if (0 == fwrite (schema_string, sizeof (char), schema_string_len, outfile)) {
      fprintf (stderr, "failed to write to file\n");
      goto fail;
   }

   ret = true;
fail:
   mongoc_client_encryption_destroy (client_encryption);
   mongoc_client_encryption_datakey_opts_destroy (datakey_opts);
   mongoc_client_encryption_opts_destroy (client_encryption_opts);
   bson_free (schema_string);
   bson_destroy (schema);
   bson_value_destroy (&datakey_id);
   if (outfile) {
      fclose (outfile);
   }


   return ret;
}

bson_t& make_kms_providers(bson_t& bs_kms_providers, std::string& master_key, const std::string& fpath)
{
	if(master_key.size() != 96)
	{
		std::string b64_master_key;
		std::ifstream ifs(fpath, std::ios_base::out | std::ios_base::binary);

		ifs.seekg(0, ifs.end);
		std::size_t master_key_size = ifs.tellg();

		ifs.seekg(0, 0);
		b64_master_key.resize(master_key_size);
		ifs.read(&b64_master_key[0], master_key_size);
		ifs.close();

		assert(b64_pton_byte_size(b64_master_key.size()) == 96);
		master_key.resize(96);

		b64_pton(b64_master_key.c_str(), reinterpret_cast<uint8_t*>(&master_key[0]), master_key.size());
	}


	//std::string str_bs_kms_providers = "{ \"local\" : { \"key\" : \"" + master_key +  "\" } }";

	bson_t bs_sub = {0};
	bson_init(&bs_kms_providers);
	bson_init(&bs_sub);

	bson_append_binary(&bs_sub, "key", -1, BSON_SUBTYPE_BINARY, reinterpret_cast<const uint8_t*>(master_key.data()), master_key.size());
	bson_append_document(&bs_kms_providers, "local", -1, &bs_sub);

	out_bson(bs_kms_providers);

	return bs_kms_providers;
}

#define TEST_CREATE_CONFIG_FILES() 1

/* This example demonstrates how to use automatic encryption with a client-side
 * schema map using the enterprise version of MongoDB */

//#define MONGOC_INSIDE
//
//#include "bson-dsl.h"
//
//#undef MONGOC_INSIDE

int
main (void)
{
	//bson_t ccOpts = {0};
	// bsonBuildDecl (ccOpts,
 //                 kv ("encryptedFields",
 //                     doc (kv ("fields",
 //                              array (doc (kv ("path", cstr ("ssn")),
 //                                          kv ("bsonType", cstr ("string")),
 //                                          kv ("keyId", null)))))));
	// out_bson(ccOpts);

/* The collection used to store the encryption data keys. */
#define KEYVAULT_DB "c_encryption"
#define KEYVAULT_COLL "__libmongocTestKeyVault"
/* The collection used to store the encrypted documents in this example. */
#define ENCRYPTED_DB "test"
#define ENCRYPTED_COLL "coll"

	const char *uristr = "mongodb://127.0.0.1:11098/?replicaSet=rs0&ssl=false";

   int exit_status = EXIT_FAILURE;
   bool ret;
   uint8_t *local_masterkey = NULL;
   uint32_t local_masterkey_len;
   bson_t *kms_providers = NULL;
   bson_error_t error = {0};
   bson_t *index_keys = NULL;
   bson_t *index_opts = NULL;
   mongoc_index_model_t *index_model = NULL;
   bson_json_reader_t *reader = NULL;
   bson_t schema = BSON_INITIALIZER;
   bson_t *schema_map = NULL;

   mongoc_collection_t* crypted_coll = NULL;

   /* The MongoClient used to access the key vault (keyvault_namespace). */
   mongoc_client_t *keyvault_client = NULL;
   mongoc_collection_t *keyvault_coll = NULL;
   mongoc_auto_encryption_opts_t *auto_encryption_opts = NULL;
   mongoc_client_t *client = NULL;
   mongoc_collection_t *coll = NULL;
   bson_t *to_insert = NULL;
   mongoc_client_t *unencrypted_client = NULL;
   mongoc_collection_t *unencrypted_coll = NULL;

   mongoc_init ();

   /* Configure the master key. This must be the same master key that was used
    * to create the encryption key. */
   //local_masterkey =
   //   hex_to_bin (getenv ("LOCAL_MASTERKEY"), &local_masterkey_len);
   //if (!local_masterkey || local_masterkey_len != 96) {
   //   fprintf (stderr,
   //            "Specify LOCAL_MASTERKEY environment variable as a "
   //            "secure random 96 byte hex value.\n");
   //   goto fail;
   //}

   //kms_providers = BCON_NEW ("local",
   //                          "{",
   //                          "key",
   //                          BCON_BIN (static_cast<bson_subtype_t>(0), local_masterkey, local_masterkey_len),
   //                          "}");

   std::string master_key;

   bson_t bs_kms_providers = {0};
   kms_providers = &(make_kms_providers(bs_kms_providers, master_key, "mongodb_client.key"));

   assert(master_key.size() == 96);

   local_masterkey = reinterpret_cast<uint8_t*>(&master_key[0]);
   local_masterkey_len = master_key.size();

#if TEST_CREATE_CONFIG_FILES()
   /* Set up the key vault for this example. */
   //keyvault_client = mongoc_client_new (
   //   "mongodb://localhost/?appname=client-side-encryption-keyvault");
   keyvault_client = mongoc_client_new (uristr);

   BSON_ASSERT (keyvault_client);

   keyvault_coll = mongoc_client_get_collection (
      keyvault_client, KEYVAULT_DB, KEYVAULT_COLL);
   mongoc_collection_drop (keyvault_coll, NULL);

   //{
	  // mongoc_collection_t* now_coll = mongoc_client_get_collection (keyvault_client, ENCRYPTED_DB, ENCRYPTED_COLL);

	  // /* Clear old data */
	  // mongoc_collection_drop (now_coll, NULL);
	  // mongoc_collection_destroy (now_coll);
   //}

   /* Create a unique index to ensure that two data keys cannot share the same
    * keyAltName. This is recommended practice for the key vault. */
   index_keys = BCON_NEW ("keyAltNames", BCON_INT32 (1));
   index_opts = BCON_NEW ("unique",
                          BCON_BOOL (true),
                          "partialFilterExpression",
                          "{",
                          "keyAltNames",
                          "{",
                          "$exists",
                          BCON_BOOL (true),
                          "}",
                          "}");
   index_model = mongoc_index_model_new (index_keys, index_opts);
   //ret = mongoc_collection_create_indexes_with_opts (keyvault_coll,
   //                                                  &index_model,
   //                                                  1,
   //                                                  NULL /* opts */,
   //                                                  NULL /* reply */,
   //                                                  &error);

   ret = true;

   if (!ret) {
      goto fail;
   }

   /* Create a new data key and a schema using it for encryption. Save the
    * schema to the file jsonSchema.json */

   ret = create_schema_file (
			kms_providers, KEYVAULT_DB, KEYVAULT_COLL, keyvault_client, &error);

    if (!ret) {
      goto fail;
   }


#endif // #if TEST_CREATE_CONFIG_FILES()

   /* Load the JSON Schema and construct the local schema_map option. */
   reader = bson_json_reader_new_from_file ("jsonSchema.json", &error);
   if (!reader) {
      goto fail;
   }

   bson_json_reader_read (reader, &schema, &error);

   out_bson(schema);

   /* Construct the schema map, mapping the namespace of the collection to the
    * schema describing encryption. */
   schema_map =
      BCON_NEW (ENCRYPTED_DB "." ENCRYPTED_COLL, BCON_DOCUMENT (&schema));

   out_bson(*schema_map);

   auto_encryption_opts = mongoc_auto_encryption_opts_new ();
   //mongoc_auto_encryption_opts_set_keyvault_client (auto_encryption_opts,
   //                                                 keyvault_client);

   mongoc_auto_encryption_opts_set_keyvault_namespace (
      auto_encryption_opts, KEYVAULT_DB, KEYVAULT_COLL);
   mongoc_auto_encryption_opts_set_kms_providers (auto_encryption_opts,
                                                  kms_providers);
   mongoc_auto_encryption_opts_set_schema_map (auto_encryption_opts,
                                               schema_map);

   //client = mongoc_client_new ("mongodb://localhost/?appname=client-side-encryption");
   client = mongoc_client_new(uristr);
   //client = keyvault_client;

   BSON_ASSERT (client);

   /* Enable automatic encryption. It will determine that encryption is
    * necessary from the schema map instead of relying on the server to provide
    * a schema. */
   ret = mongoc_client_enable_auto_encryption (
      client, auto_encryption_opts, &error);
   if (!ret) {
      goto fail;
   }

   coll = mongoc_client_get_collection (client, ENCRYPTED_DB, ENCRYPTED_COLL);

   /* Clear old data */
   mongoc_collection_drop (coll, NULL);

   to_insert = BCON_NEW ("encryptedField", "123456789");

   out_bson(*to_insert);

   ret = mongoc_collection_insert_one (
      coll, to_insert, NULL /* opts */, NULL /* reply */, &error);
   if (!ret) {
      goto fail;
   }
   printf ("decrypted document: ");
   if (!print_one_document (coll, &error)) {
      goto fail;
   }
   printf ("\n");

   //unencrypted_client = mongoc_client_new (
   //   "mongodb://localhost/?appname=client-side-encryption-unencrypted");
   unencrypted_client = mongoc_client_new(uristr);
   BSON_ASSERT (unencrypted_client);

   unencrypted_coll = mongoc_client_get_collection (
      unencrypted_client, ENCRYPTED_DB, ENCRYPTED_COLL);
   printf ("encrypted document: ");
   if (!print_one_document (unencrypted_coll, &error)) {
      goto fail;
   }
   printf ("\n");

   /*mongoc_collection_t **/crypted_coll = 
	   mongoc_client_get_collection (
		client, ENCRYPTED_DB, ENCRYPTED_COLL);

   if(crypted_coll)
   {
		printf ("unencrypted find document: ");
		print_one_document(crypted_coll, &error);
		mongoc_collection_destroy (crypted_coll);
   }
   else
   {
	   printf ("unencrypted find document failed");
   }
   printf ("\n");

   exit_status = EXIT_SUCCESS;
fail:
   if (error.code) {
      fprintf (stderr, "error: %s\n", error.message);
   }

   //bson_free (local_masterkey);
   bson_destroy (kms_providers);
   mongoc_collection_destroy (keyvault_coll);
   mongoc_index_model_destroy (index_model);
   bson_destroy (index_opts);
   bson_destroy (index_keys);
   bson_json_reader_destroy (reader);
   mongoc_auto_encryption_opts_destroy (auto_encryption_opts);
   mongoc_collection_destroy (coll);
   mongoc_client_destroy (client);
   bson_destroy (to_insert);
   mongoc_collection_destroy (unencrypted_coll);
   mongoc_client_destroy (unencrypted_client);
   mongoc_client_destroy (keyvault_client);
   bson_destroy (&schema);
   bson_destroy (schema_map);
   mongoc_cleanup ();

   char cc = 0;
   std::cin >> cc;

   return exit_status;
}
