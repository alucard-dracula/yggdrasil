//mongoc_connection_pool_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>

#include <list>
#include <map>

#include <yggr/container/list.hpp>
#include <yggr/container/map.hpp>
#include <yggr/container/align_vector.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>
#include <yggr/nsql_database_system/c_mongo_connection_pool.hpp>
#include <yggr/nsql_database_system/c_mongo_index_description.hpp>
#include <yggr/nsql_database_system/c_mongo_find_and_modify_opts.hpp>
#include <yggr/nsql_database_system/mongo_bulk_operation_helper.hpp>
#include <yggr/nsql_database_system/mongo_utility.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>

#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/this_thread.hpp>

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

void test1(void)
{

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

	yggr::nsql_database_system::c_mongo_connection_pool client(client_init);

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		//return EXIT_FAILURE;
		return;
	}


	client.clear();

	std::cout << "---------test1 end--------------" << std::endl;
}

void test2(void)
{

#if defined(YGGR_AT_MOBILE)

#	if MONGODB_USING_CA()

#		if MONGODB_USING_AUTH()
		const char *uristr = "mongodb://xy:123456abc@192.168.1.101:10398/?ssl=true";
#		else
		const char *uristr = "mongodb://192.168.1.101:10298/?ssl=true";
#		endif // MONGODB_USING_AUTH

#	else

#		if MONGODB_USING_AUTH()
		const char *uristr = "mongodb://xy:123456abc@192.168.1.101:10198/?ssl=false";
#		else
		const char *uristr = "mongodb://192.168.1.101:10098/?ssl=false";
#		endif // MONGODB_USING_AUTH

#	endif // MONGODB_USING_CA

#else

#	if MONGODB_USING_CA()

#		if MONGODB_USING_AUTH()
		const char *uristr = "mongodb://xy:123456abc@127.0.0.1:10398/?ssl=true";
#		else
		const char *uristr = "mongodb://127.0.0.1:10298/?ssl=true";
#		endif // MONGODB_USING_AUTH

#	else

#		if MONGODB_USING_AUTH()
		const char *uristr = "mongodb://xy:123456abc@127.0.0.1:10198/?ssl=false";
#		else
		const char *uristr = "mongodb://127.0.0.1:10098/?ssl=false";
#		endif // MONGODB_USING_AUTH

#	endif // MONGODB_USING_CA

#endif // YGGR_AT_MOBILE

	mongoc_client_pool_t *client_pool = 0;
	
	mongoc_uri_t* puri = mongoc_uri_new(uristr);

	client_pool = mongoc_client_pool_new(puri);

	if (!client_pool) {
		fprintf (stderr, "Failed to parse URI.\n");
		return;
	}
	
#if MONGODB_USING_CA()
	mongoc_ssl_opt_t ssl_opts = { 0 };

	yggr::string str_pem_file = "./nsql_database_system/cert/client.pem";
	ssl_opts.pem_file = str_pem_file.c_str(); // warning this is not copy
	ssl_opts.pem_pwd = "";
	ssl_opts.ca_file = "./nsql_database_system/cert/ca.pem";
	ssl_opts.ca_dir = "./nsql_database_system/cert";
	ssl_opts.weak_cert_validation = true;

	mongoc_client_pool_set_ssl_opts(client_pool, &ssl_opts);
#endif // MONGODB_USING_CA

	mongoc_client_pool_destroy(client_pool);
	mongoc_uri_destroy(puri);

	std::cout << "---------test2 end--------------" << std::endl;
}

int main(int argc, char *argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();

	test1();
	test2();

	yggr::nsql_database_system::mongodb_installer::uninstall();

	std::cout << "--------------all end-----------------" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}