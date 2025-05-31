// mongodbc_basic_connect_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>
#include <yggr/nsql_database_system/c_mongo_connection.hpp>

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

int main(int argc, char *argv[])
{
	mongoc_collection_t *collection = 0;
	mongoc_cursor_t *cursor = 0;
	yggr::nsql_database_system::c_bson_error error;
	const bson_t *doc = 0;
	const char* collection_name = "foo";
	char *str = 0;

	yggr::nsql_database_system::mongodb_installer::install();

#if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_addr = "192.168.1.101"; // you host ip address
#else
	yggr::utf8_string host_addr = "127.0.0.1";
#endif // YGGR_AT_MOBILE

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://xy:123456abc@" + host_addr + ":10398/?ssl=true", //"mongodb://xy:123456abc@127.0.0.1:10398/?ssl=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://" + host_addr + ":10298/?ssl=true", //"mongodb://127.0.0.1:10298/?ssl=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);

#	endif // MONGODB_USING_AUTH

#else

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://xy:123456abc@" + host_addr + ":10198/?ssl=false" //"mongodb://xy:123456abc@127.0.0.1:10198/?ssl=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://" + host_addr + ":10098/?ssl=false" // "mongodb://127.0.0.1:10098/?ssl=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	yggr::nsql_database_system::c_mongo_connection client(client_init);

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		return EXIT_FAILURE;
	}

	yggr::nsql_database_system::c_bson query;

	{
		yggr::nsql_database_system::c_mongo_connection::mongoc_client_ptr_type pclt = client.pop();
		collection = mongoc_client_get_collection (pclt.get(), "test", collection_name);

		// remove
		yggr::nsql_database_system::c_bson bsd;
		bson_error_t err_ins_d = {0};
		mongoc_collection_remove(collection, MONGOC_REMOVE_NONE, &bsd, 0, &err_ins_d);

		//insert single
		yggr::nsql_database_system::c_bson bs;
		bs.save("c", 3);
		bson_error_t err_ins = {0};
		mongoc_collection_insert(collection, MONGOC_INSERT_NONE, &bs, 0, &err_ins);

		// insert bulk
		yggr::nsql_database_system::c_bson bs2;
		bs2.save("e", 5);

		bson_error_t err_ins_b = {0};
		bson_t* bs_arr[3] = {&bs2, &bs2, &bs2};

		mongoc_collection_insert_bulk(collection, MONGOC_INSERT_NONE, const_cast<const bson_t**>(&bs_arr[0]), 3, 0, &err_ins_b);

		cursor = mongoc_collection_find (collection,
										MONGOC_QUERY_NONE,
										0,
										0,
										0,
										&query,
										NULL,  /* Fields, NULL for all. */
										NULL); /* Read Prefs, NULL for default */

		//std::cout << mongoc_cursor_is_alive(cursor) << std::endl;

		while (mongoc_cursor_next (cursor, &doc))
		{
			str = bson_as_json (doc, NULL);
			fprintf (stdout, "%s\n", str);
			bson_free (str);
		}

		if (mongoc_cursor_error (cursor, &error))
		{
			fprintf (stderr, "Cursor Failure: %s\n", error.what());

			/*bool bpushed = */client.push(pclt);
			goto end;
		}

		/*bool bpushed = */client.push(pclt);

	}

end:
	mongoc_cursor_destroy (cursor);
	mongoc_collection_destroy (collection);
	client.clear();

	yggr::nsql_database_system::mongodb_installer::uninstall();

	wait_any_key(argc, argv);
	return 0;
}
