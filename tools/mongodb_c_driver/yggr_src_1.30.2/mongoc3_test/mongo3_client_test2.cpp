// mongo_gridfs_org_test.cpp

#include <iostream>

#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>
#include <openssl/bio.h> 

#ifdef _MSC_VER
#	if defined(_DEBUG)
#		if _MSC_VER == 1600
#			pragma comment(lib, "libbson-vc100-d.lib")
#			pragma comment(lib, "libcommon-vc100-d.lib")
#			pragma comment(lib, "libmongoc-vc100-d.lib")
#			pragma comment(lib, "libmongocrypt-vc100-d.lib")
#			pragma comment(lib, "libkms-message-vc100-d.lib")
#			pragma comment(lib, "libutf8proc-vc100-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc100-d.lib")
#			pragma comment(lib, "libbase64-vc100-d.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libbson-vc110-d.lib")
#			pragma comment(lib, "libcommon-vc110-d.lib")
#			pragma comment(lib, "libmongoc-vc110-d.lib")
#			pragma comment(lib, "libmongocrypt-vc110-d.lib")
#			pragma comment(lib, "libkms-message-vc110-d.lib")
#			pragma comment(lib, "libutf8proc-vc110-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc110-d.lib")
#			pragma comment(lib, "libbase64-vc110-d.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libbson-vc120-d.lib")
#			pragma comment(lib, "libcommon-vc120-d.lib")
#			pragma comment(lib, "libmongoc-vc120-d.lib")
#			pragma comment(lib, "libmongocrypt-vc120-d.lib")
#			pragma comment(lib, "libkms-message-vc120-d.lib")
#			pragma comment(lib, "libutf8proc-vc120-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc120-d.lib")
#			pragma comment(lib, "libbase64-vc120-d.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libbson-vc140-d.lib")
#			pragma comment(lib, "libcommon-vc140-d.lib")
#			pragma comment(lib, "libmongoc-vc140-d.lib")
#			pragma comment(lib, "libmongocrypt-vc140-d.lib")
#			pragma comment(lib, "libkms-message-vc140-d.lib")
#			pragma comment(lib, "libutf8proc-vc140-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc140-d.lib")
#			pragma comment(lib, "libbase64-vc140-d.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libbson-vc141-d.lib")
#			pragma comment(lib, "libcommon-vc141-d.lib")
#			pragma comment(lib, "libmongoc-vc141-d.lib")
#			pragma comment(lib, "libmongocrypt-vc141-d.lib")
#			pragma comment(lib, "libkms-message-vc141-d.lib")
#			pragma comment(lib, "libutf8proc-vc141-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc141-d.lib")
#			pragma comment(lib, "libbase64-vc141-d.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libbson-vc142-d.lib")
#			pragma comment(lib, "libcommon-vc142-d.lib")
#			pragma comment(lib, "libmongoc-vc142-d.lib")
#			pragma comment(lib, "libmongocrypt-vc142-d.lib")
#			pragma comment(lib, "libkms-message-vc142-d.lib")
#			pragma comment(lib, "libutf8proc-vc142-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc142-d.lib")
#			pragma comment(lib, "libbase64-vc142-d.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libbson-vc143-d.lib")
#			pragma comment(lib, "libcommon-vc143-d.lib")
#			pragma comment(lib, "libmongoc-vc143-d.lib")
#			pragma comment(lib, "libmongocrypt-vc143-d.lib")
#			pragma comment(lib, "libkms-message-vc143-d.lib")
#			pragma comment(lib, "libutf8proc-vc143-d.lib")
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
#			pragma comment(lib, "libutf8proc-vc100.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc100.lib")
#			pragma comment(lib, "libbase64-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libbson-vc110.lib")
#			pragma comment(lib, "libcommon-vc110.lib")
#			pragma comment(lib, "libmongoc-vc110.lib")
#			pragma comment(lib, "libmongocrypt-vc110.lib")
#			pragma comment(lib, "libkms-message-vc110.lib")
#			pragma comment(lib, "libutf8proc-vc110.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc110.lib")
#			pragma comment(lib, "libbase64-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libbson-vc120.lib")
#			pragma comment(lib, "libcommon-vc120.lib")
#			pragma comment(lib, "libmongoc-vc120.lib")
#			pragma comment(lib, "libmongocrypt-vc120.lib")
#			pragma comment(lib, "libkms-message-vc120.lib")
#			pragma comment(lib, "libutf8proc-vc120.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc120.lib")
#			pragma comment(lib, "libbase64-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libbson-vc140.lib")
#			pragma comment(lib, "libcommon-vc140.lib")
#			pragma comment(lib, "libmongoc-vc140.lib")
#			pragma comment(lib, "libmongocrypt-vc140.lib")
#			pragma comment(lib, "libkms-message-vc140.lib")
#			pragma comment(lib, "libutf8proc-vc140.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc140.lib")
#			pragma comment(lib, "libbase64-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libbson-vc141.lib")
#			pragma comment(lib, "libcommon-vc141.lib")
#			pragma comment(lib, "libmongoc-vc141.lib")
#			pragma comment(lib, "libmongocrypt-vc141.lib")
#			pragma comment(lib, "libkms-message-vc141.lib")
#			pragma comment(lib, "libutf8proc-vc141.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc141.lib")
#			pragma comment(lib, "libbase64-vc141.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libbson-vc142.lib")
#			pragma comment(lib, "libcommon-vc142.lib")
#			pragma comment(lib, "libmongoc-vc142.lib")
#			pragma comment(lib, "libmongocrypt-vc142.lib")
#			pragma comment(lib, "libmongocrypt-vc142.lib")
#			pragma comment(lib, "libkms-message-vc142.lib")
#			pragma comment(lib, "libutf8proc-vc142.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc142.lib")
#			pragma comment(lib, "libbase64-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libbson-vc143.lib")
#			pragma comment(lib, "libcommon-vc143.lib")
#			pragma comment(lib, "libmongoc-vc143.lib")
#			pragma comment(lib, "libmongocrypt-vc143.lib")
#			pragma comment(lib, "libmongocrypt-vc143.lib")
#			pragma comment(lib, "libkms-message-vc143.lib")
#			pragma comment(lib, "libutf8proc-vc143.lib")
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


#define MONGODB_USING_OLD_LEGACY_STDIO_DEFINITIONS


#ifdef _MSC_VER
#	if defined(MONGODB_USING_OLD_LEGACY_STDIO_DEFINITIONS)
#		if 1800 < _MSC_VER
#			pragma comment(lib, "legacy_stdio_definitions.lib")
#		endif // 1800 < _MSC_VER

#		if defined(_DEBUG)
#			if _MSC_VER == 1900
#				pragma comment(lib, "liblegacy_stdio_iob_func-vc140-d.lib")
#			elif _MSC_VER == 1910
#				pragma comment(lib, "liblegacy_stdio_iob_func-vc141-d.lib")
#			elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#				pragma comment(lib, "liblegacy_stdio_iob_func-vc142-d.lib")
#			elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#				pragma comment(lib, "liblegacy_stdio_iob_func-vc143-d.lib")
#			else
#			endif // ( _MSC_VER == 1600)
#		else // _DEBUG
#			if _MSC_VER == 1900
#				pragma comment(lib, "liblegacy_stdio_iob_func-vc140.lib")
#			elif _MSC_VER == 1910
#				pragma comment(lib, "liblegacy_stdio_iob_func-vc141.lib")
#			elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#				pragma comment(lib, "liblegacy_stdio_iob_func-vc142.lib")
#			elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#				pragma comment(lib, "liblegacy_stdio_iob_func-vc143.lib")
#			else
#			endif // ( _MSC_VER == 1600)
#		endif // _DEBUG
#	endif // MONGODB_USING_OLD_LEGACY_STDIO_DEFINITIONS
#endif // _MSC_VER

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void init_ssl(void)
{
	SSL_library_init ();
	SSL_load_error_strings ();
	ERR_load_BIO_strings ();
	OpenSSL_add_all_algorithms ();
}

void clean_ssl(void)
{
	ENGINE_cleanup();
	CONF_modules_unload(1);
	EVP_cleanup();
	SSL_COMP_free_compression_methods();
	ERR_free_strings();
	CRYPTO_cleanup_all_ex_data();
	ERR_remove_state(0);
}

int main (int argc, char *argv[])
{
	mongoc_client_t *client = 0;
	mongoc_collection_t *collection = 0;
	mongoc_cursor_t *cursor = 0;
	bson_error_t error;
	const bson_t *doc = 0;
	//const char *uristr = "mongodb://127.0.0.1:10098/?ssl=true";
	const char *uristr = "mongodb://xy_root:123456abc@127.0.0.1:10098/?ssl=true";
	const char *collection_name = "foo";
	bson_t query;
	char *str = 0;

#ifdef _WIN32
   {
      WORD wVersionRequested;
      WSADATA wsaData;
      int err;

      wVersionRequested = MAKEWORD (2, 2);

      err = WSAStartup (wVersionRequested, &wsaData);

      /* check the version perhaps? */

      BSON_ASSERT (err == 0);
   }
#endif //_WIN32

	init_ssl();
	mongoc_init ();

	client = mongoc_client_new (uristr);

	if (!client) {
		fprintf (stderr, "Failed to parse URI.\n");
		return EXIT_FAILURE;
	}

	bson_init (&query);

	
	mongoc_ssl_opt_t ssl_opts = { 0 };


	std::string str_pem_file = "mongo.pem";
	ssl_opts.pem_file = str_pem_file.c_str(); // warning this is not copy
	ssl_opts.pem_pwd = "";
	ssl_opts.ca_file = "camongodb.pem";
	ssl_opts.ca_dir = "./";
	ssl_opts.weak_cert_validation = true;
	mongoc_client_set_ssl_opts(client, &ssl_opts);

	collection = mongoc_client_get_collection (client, "test", collection_name);

   

	cursor = mongoc_collection_find (collection,
									MONGOC_QUERY_NONE,
									0,
									0,
									0,
									&query,
									NULL,  /* Fields, NULL for all. */
									NULL); /* Read Prefs, NULL for default */

	std::cout << mongoc_cursor_is_alive(cursor) << std::endl;

	bson_destroy (&query);

	while (mongoc_cursor_next (cursor, &doc)) 
	{
		str = bson_as_json (doc, NULL);
		fprintf (stdout, "%s\n", str);
		bson_free (str);
	}

	if (mongoc_cursor_error (cursor, &error))
	{
		fprintf (stderr, "Cursor Failure: %s\n", error.message);

		mongoc_cursor_destroy (cursor);
		mongoc_collection_destroy (collection);
		mongoc_client_destroy (client);

		mongoc_cleanup ();
		clean_ssl();

#	ifdef _WIN32
		WSACleanup ();
#	endif

		char cc = 0;
		std::cin >> cc;
		return EXIT_FAILURE;
	}

	mongoc_cursor_destroy (cursor);
	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);

	mongoc_cleanup ();
	clean_ssl();

	//ENGINE_cleanup();
	//CONF_modules_unload(1);
	//EVP_cleanup();
	//SSL_COMP_free_compression_methods();
	//ERR_free_strings();
	//CRYPTO_cleanup_all_ex_data();
	//ERR_remove_state(0);

	
#ifdef _WIN32
   WSACleanup ();
#endif

	char cc = 0;
	std::cin >> cc;

	return EXIT_SUCCESS;
}