// mongo_gridfs_org_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>
#include <fstream>

#if !defined(BUILD_MONGODB_DRIVER_YGGR_VER)
#   define BUILD_MONGODB_DRIVER_YGGR_VER
#endif // BUILD_MONGODB_DRIVER_YGGR_VER

#if !defined(BSON_STATIC)
#	define BSON_STATIC
#endif //BSON_STATIC

#if !defined(MONGOC_STATIC)
#	define MONGOC_STATIC
#endif //MONGOC_STATIC

#if !defined(KMS_MSG_STATIC)
#	define KMS_MSG_STATIC
#endif //KMS_MSG_STATIC

#if !defined(MLIB_USER)
#	define MLIB_USER
#endif //MLIB_USER

#if !defined(MONGOCRYPT_STATIC_DEFINE)
#	define MONGOCRYPT_STATIC_DEFINE
#endif //MONGOCRYPT_STATIC_DEFINE

#if !defined(MONGOCRYPT_LITTLE_ENDIAN)
#	define MONGOCRYPT_LITTLE_ENDIAN
#endif //MONGOCRYPT_LITTLE_ENDIAN

#if !defined(MONGOCRYPT_INTELDFP)
#	define MONGOCRYPT_INTELDFP
#endif //MONGOCRYPT_INTELDFP

#if !defined(OPENSSL_USING_STATIC_LIB)
#	define OPENSSL_USING_STATIC_LIB
#endif // OPENSSL_USING_STATIC_LIB

#include <mongoc.h>
//
//#if !(defined(MONGOC_ENABLE_CRYPTO_CNG) && MONGOC_ENABLE_CRYPTO_CNG == 1)
//#	error "aaa"
//#endif // aa

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/conf.h>
#include <openssl/bio.h>


#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

// this example is native test, so using "#pragma comment", don't modify to "YGGR_PP_LINK_LIB"

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
#			pragma comment(lib, "libzlibstatic-vc100-d.lib")
#			pragma comment(lib, "icuuc-vc100-d.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libbson-vc110-d.lib")
#			pragma comment(lib, "libcommon-vc110-d.lib")
#			pragma comment(lib, "libmongoc-vc110-d.lib")
#			pragma comment(lib, "libmongocrypt-vc110-d.lib")
#			pragma comment(lib, "libkms-message-vc110-d.lib")
#			pragma comment(lib, "libutf8proc-vc110-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc110-d.lib")
#			pragma comment(lib, "libbase64-vc110-d.lib")
#			pragma comment(lib, "libzlibstatic-vc110-d.lib")
#			pragma comment(lib, "icuuc-vc110-d.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libbson-vc120-d.lib")
#			pragma comment(lib, "libcommon-vc120-d.lib")
#			pragma comment(lib, "libmongoc-vc120-d.lib")
#			pragma comment(lib, "libmongocrypt-vc120-d.lib")
#			pragma comment(lib, "libkms-message-vc120-d.lib")
#			pragma comment(lib, "libutf8proc-vc120-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc120-d.lib")
#			pragma comment(lib, "libbase64-vc120-d.lib")
#			pragma comment(lib, "libzlibstatic-vc120-d.lib")
#			pragma comment(lib, "icuuc-vc120-d.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libbson-vc140-d.lib")
#			pragma comment(lib, "libcommon-vc140-d.lib")
#			pragma comment(lib, "libmongoc-vc140-d.lib")
#			pragma comment(lib, "libmongocrypt-vc140-d.lib")
#			pragma comment(lib, "libkms-message-vc140-d.lib")
#			pragma comment(lib, "libutf8proc-vc140-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc140-d.lib")
#			pragma comment(lib, "libbase64-vc140-d.lib")
#			pragma comment(lib, "libzlibstatic-vc140-d.lib")
#			pragma comment(lib, "icuuc-vc140-d.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libbson-vc141-d.lib")
#			pragma comment(lib, "libcommon-vc141-d.lib")
#			pragma comment(lib, "libmongoc-vc141-d.lib")
#			pragma comment(lib, "libmongocrypt-vc141-d.lib")
#			pragma comment(lib, "libkms-message-vc141-d.lib")
#			pragma comment(lib, "libutf8proc-vc141-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc141-d.lib")
#			pragma comment(lib, "libbase64-vc141-d.lib")
#			pragma comment(lib, "libzlibstatic-vc141-d.lib")
#			pragma comment(lib, "icuuc-vc141-d.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libbson-vc142-d.lib")
#			pragma comment(lib, "libcommon-vc142-d.lib")
#			pragma comment(lib, "libmongoc-vc142-d.lib")
#			pragma comment(lib, "libmongocrypt-vc142-d.lib")
#			pragma comment(lib, "libkms-message-vc142-d.lib")
#			pragma comment(lib, "libutf8proc-vc142-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc142-d.lib")
#			pragma comment(lib, "libbase64-vc142-d.lib")
#			pragma comment(lib, "libzlibstatic-vc142-d.lib")
#			pragma comment(lib, "icuuc-vc142-d.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 2000
#			pragma comment(lib, "libbson-vc143-d.lib")
#			pragma comment(lib, "libcommon-vc143-d.lib")
#			pragma comment(lib, "libmongoc-vc143-d.lib")
#			pragma comment(lib, "libmongocrypt-vc143-d.lib")
#			pragma comment(lib, "libkms-message-vc143-d.lib")
#			pragma comment(lib, "libutf8proc-vc143-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc143-d.lib")
#			pragma comment(lib, "libbase64-vc143-d.lib")
#			pragma comment(lib, "libzlibstatic-vc143-d.lib")
#			pragma comment(lib, "icuuc-vc143-d.lib")
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
#			pragma comment(lib, "libzlibstatic-vc100.lib")
#			pragma comment(lib, "icuuc-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libbson-vc110.lib")
#			pragma comment(lib, "libcommon-vc110.lib")
#			pragma comment(lib, "libmongoc-vc110.lib")
#			pragma comment(lib, "libmongocrypt-vc110.lib")
#			pragma comment(lib, "libkms-message-vc110.lib")
#			pragma comment(lib, "libutf8proc-vc110.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc110.lib")
#			pragma comment(lib, "libbase64-vc110.lib")
#			pragma comment(lib, "libzlibstatic-vc110.lib")
#			pragma comment(lib, "icuuc-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libbson-vc120.lib")
#			pragma comment(lib, "libcommon-vc120.lib")
#			pragma comment(lib, "libmongoc-vc120.lib")
#			pragma comment(lib, "libmongocrypt-vc120.lib")
#			pragma comment(lib, "libkms-message-vc120.lib")
#			pragma comment(lib, "libutf8proc-vc120.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc120.lib")
#			pragma comment(lib, "libbase64-vc120.lib")
#			pragma comment(lib, "libzlibstatic-vc120.lib")
#			pragma comment(lib, "icuuc-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libbson-vc140.lib")
#			pragma comment(lib, "libcommon-vc140.lib")
#			pragma comment(lib, "libmongoc-vc140.lib")
#			pragma comment(lib, "libmongocrypt-vc140.lib")
#			pragma comment(lib, "libkms-message-vc140.lib")
#			pragma comment(lib, "libutf8proc-vc140.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc140.lib")
#			pragma comment(lib, "libbase64-vc140.lib")
#			pragma comment(lib, "libzlibstatic-vc140.lib")
#			pragma comment(lib, "icuuc-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libbson-vc141.lib")
#			pragma comment(lib, "libcommon-vc141.lib")
#			pragma comment(lib, "libmongoc-vc141.lib")
#			pragma comment(lib, "libmongocrypt-vc141.lib")
#			pragma comment(lib, "libkms-message-vc141.lib")
#			pragma comment(lib, "libutf8proc-vc141.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc141.lib")
#			pragma comment(lib, "libbase64-vc141.lib")
#			pragma comment(lib, "libzlibstatic-vc141.lib")
#			pragma comment(lib, "icuuc-vc141.lib")
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
#			pragma comment(lib, "libzlibstatic-vc142.lib")
#			pragma comment(lib, "icuuc-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 2000
#			pragma comment(lib, "libbson-vc143.lib")
#			pragma comment(lib, "libcommon-vc143.lib")
#			pragma comment(lib, "libmongoc-vc143.lib")
#			pragma comment(lib, "libmongocrypt-vc143.lib")
#			pragma comment(lib, "libmongocrypt-vc143.lib")
#			pragma comment(lib, "libkms-message-vc143.lib")
#			pragma comment(lib, "libutf8proc-vc143.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc143.lib")
#			pragma comment(lib, "libbase64-vc143.lib")
#			pragma comment(lib, "libzlibstatic-vc143.lib")
#			pragma comment(lib, "icuuc-vc143.lib")
#		else
#			error "set libs"
#		endif // ( _MSC_VER == 1600)
#	endif // _DEBUG
#endif // _MSC_VER

#ifdef _MSC_VER
//#	pragma comment(lib, "ssleay32.lib")
//#	pragma comment(lib, "libeay32.lib")

//#	if defined(_DEBUG)
//#		pragma comment(lib, "icuucd.lib")
//#	else
//#		pragma comment(lib, "icuuc.lib")
//#	endif // _DEBUG

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
#		elif 1930 <= _MSC_VER && _MSC_VER <= 2000
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
#		elif 1930 <= _MSC_VER && _MSC_VER <= 2000
#			pragma comment(lib, "libcrypto_static-vc143.lib")
#			pragma comment(lib, "libssl_static-vc143.lib")
#		else
#		endif // ( _MSC_VER == 1600)
#	endif // 0,1
#endif // _MSC_VER

// 0 not-using-ssl 1 using-ssl
#define MONGODB_USING_CA() 0

// 0 not-using-auth 1 using-auth
#define MONGODB_USING_AUTH() 0

const char* g_db_name = "test_org_gridfs";
const char* g_gridfs_name = "fs";
const char* g_fname = "gridfs_test_file.txt";
//const char* g_fname = "test.mp3";

#if defined(YGGR_AT_MOBILE)

#	if MONGODB_USING_CA()
#		if MONGODB_USING_AUTH()

	const char* g_uristr = "mongodb://xy@123456abc@192.168.1.101:10098/?ssl=true";
	const char* g_pem_file = "./nsql_database_system/cert/client.pem";
	const char* g_pem_pwd = "";
	const char* g_ca_file = "./nsql_database_system/cert/ca.pem";
	const char* g_ca_dir = "./nsql_database_system/cert";

#		else

	const char* g_uristr = "mongodb://192.168.1.101:10098/?ssl=true";
	const char* g_pem_file = "./nsql_database_system/cert/client.pem";
	const char* g_pem_pwd = "";
	const char* g_ca_file = "./nsql_database_system/cert/ca.pem";
	const char* g_ca_dir = "./nsql_database_system/cert";

#		endif // MONGODB_USING_AUTH
#	else
#		if MONGODB_USING_AUTH()
	const char* g_uristr = "mongodb://xy@123456abc@192.168.1.101:10098/?ssl=false";
	const char* g_pem_file = "";
	const char* g_pem_pwd = "";
	const char* g_ca_file = "";
	const char* g_ca_dir = "";
#		else
	const char* g_uristr = "mongodb://192.168.1.101:10098/?ssl=false";
	const char* g_pem_file = "";
	const char* g_pem_pwd = "";
	const char* g_ca_file = "";
	const char* g_ca_dir = "";
#		endif // MONGODB_USING_AUTH
#	endif // MONGODB_USING_CA

#else

#	if MONGODB_USING_CA()
#		if MONGODB_USING_AUTH()

	const char* g_uristr = "mongodb://xy@123456abc@127.0.0.1:10098/?ssl=true";
	const char* g_pem_file = "./nsql_database_system/cert/client.pem";
	const char* g_pem_pwd = "";
	const char* g_ca_file = "./nsql_database_system/cert/ca.pem";
	const char* g_ca_dir = "./nsql_database_system/cert";

#		else

	const char* g_uristr = "mongodb://127.0.0.1:10098/?ssl=true";
	const char* g_pem_file = "./nsql_database_system/cert/client.pem";
	const char* g_pem_pwd = "";
	const char* g_ca_file = "./nsql_database_system/cert/ca.pem";
	const char* g_ca_dir = "./nsql_database_system/cert";

#		endif // MONGODB_USING_AUTH
#	else
#		if MONGODB_USING_AUTH()
	const char* g_uristr = "mongodb://xy@123456abc@127.0.0.1:10098/?ssl=false";
	const char* g_pem_file = "";
	const char* g_pem_pwd = "";
	const char* g_ca_file = "";
	const char* g_ca_dir = "";
#		else
	const char* g_uristr = "mongodb://127.0.0.1:10098/?ssl=false";
	const char* g_pem_file = "";
	const char* g_pem_pwd = "";
	const char* g_ca_file = "";
	const char* g_ca_dir = "";
#		endif // MONGODB_USING_AUTH
#	endif // MONGODB_USING_CA

#endif //#if defined(YGGR_AT_MOBILE)

void init_ssl(void)
{
	SSL_library_init ();
	SSL_load_error_strings ();
#if OPENSSL_VERSION_MAJOR < 2
	ERR_load_BIO_strings ();
#endif // OPENSSL_VERSION_MAJOR < 2
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
#if OPENSSL_VERSION_MAJOR < 2
	ERR_remove_state(0);
#endif // OPENSSL_VERSION_MAJOR < 2
}

void test_gridfs_read(void)
{
	mongoc_client_t *client = 0;
	bson_error_t error;

	const char* db_name = g_db_name;
	const char* gridfs_name = g_gridfs_name;
	const char* fname = g_fname;
	const char* out_fname = "./nsql_database_system/out_gridfs_test_file.txt";
	//const char* out_fname = "./nsql_database_system/out_test.mp3";

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

	client = mongoc_client_new(g_uristr);

	if (!client)
	{
		fprintf (stderr, "Failed to parse URI.\n");
		return;
	}


#if MONGODB_USING_CA()
	mongoc_ssl_opt_t ssl_opts = { 0 };
	ssl_opts.pem_file = g_pem_file; // warning this is not copy
	ssl_opts.pem_pwd = g_pem_pwd;
	ssl_opts.ca_file = g_ca_file;
	ssl_opts.ca_dir = g_ca_dir;
	ssl_opts.weak_cert_validation = true;
	mongoc_client_set_ssl_opts(client, &ssl_opts);
#endif // MONGODB_USING_CA()

	mongoc_gridfs_t* gridfs = 0;
	mongoc_gridfs_file_t* file = 0;
	mongoc_stream_t* stream = 0;


	gridfs = mongoc_client_get_gridfs(client, db_name, gridfs_name, &error); // gridfs_t

	if(!gridfs)
	{
		std::cout << "test_gridfs_read !gridfs" << std::endl;
		std::cerr << error.message << std::endl;
		goto end;
	}

	file = mongoc_gridfs_find_one_by_filename(gridfs, fname, &error); // gridfs_file_t

	if(!file)
	{
		std::cout << "test_gridfs_read !file" << std::endl;
		mongoc_gridfs_destroy(gridfs);
		goto end;
	}

	stream = mongoc_stream_gridfs_new(file); // stream
	if(!stream)
	{
		std::cout << "test_gridfs_read !stream" << std::endl;
		mongoc_gridfs_file_destroy(file);
		mongoc_gridfs_destroy(gridfs);
		goto end;
	}

	/*
	ssize_t
		mongoc_gridfs_file_readv (mongoc_gridfs_file_t *file,
								  mongoc_iovec_t       *iov,
								  size_t                iovcnt,
								  size_t                min_bytes,
								  uint32_t              timeout_msec);
	*/
	{
		std::string str_buf(mongoc_gridfs_file_get_length(file), 0);

		mongoc_iovec_t iov = {0};
		iov.iov_base = &str_buf[0];
		iov.iov_len = str_buf.size();

		ssize_t size = mongoc_stream_readv(stream, &iov, 1, -1, 0);
		yggr_test_assert(size == str_buf.size());

		std::ofstream ofs;
		ofs.open(out_fname, std::ios_base::out | std::ios_base::binary);
		ofs.write(str_buf.data(), str_buf.size());
		ofs.flush();
		ofs.close();
	}

	mongoc_stream_destroy(stream);
	mongoc_gridfs_file_destroy(file);
	mongoc_gridfs_destroy(gridfs);

	std::cout << "test_gridfs_read success" << std::endl;

end:
	mongoc_client_destroy (client);

#ifdef _WIN32
   WSACleanup ();
#endif
}

void test_gridfs_write(void)
{
	mongoc_client_t *client = 0;
	bson_error_t error;

	const char* db_name = g_db_name;
	const char* gridfs_name = g_gridfs_name;
	const char* fname = g_fname;
	const char* local_fname = "./nsql_database_system/gridfs_test_file.txt";
	//const char* local_fname = "./nsql_database_system/test.mp3";

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

	client = mongoc_client_new(g_uristr);

	if (!client)
	{
		fprintf (stderr, "Failed to parse URI.\n");
		return;
	}

	{
		mongoc_database_t* pdb = mongoc_client_get_database(client, db_name);
		if(pdb)
		{
			mongoc_database_drop(pdb, 0);
			mongoc_database_destroy(pdb);
		}
	}

#if MONGODB_USING_CA()
	mongoc_ssl_opt_t ssl_opts = { 0 };
	ssl_opts.pem_file = g_pem_file; // warning this is not copy
	ssl_opts.pem_pwd = g_pem_pwd;
	ssl_opts.ca_file = g_ca_file;
	ssl_opts.ca_dir = g_ca_dir;
	ssl_opts.weak_cert_validation = true;
	mongoc_client_set_ssl_opts(client, &ssl_opts);
#endif // MONGODB_USING_CA

	mongoc_gridfs_t* gridfs = 0;
	mongoc_stream_t* stream = 0;
	mongoc_gridfs_file_t* file = 0;

	gridfs = mongoc_client_get_gridfs(client, db_name, gridfs_name, &error);

	if(!gridfs)
	{
		std::cout << "test_gridfs_write !gridfs" << std::endl;
		std::cerr << error.message << std::endl;
		goto end;
	}

	stream = mongoc_stream_file_new_for_path(local_fname, O_RDONLY, 0);
	if(!stream)
	{
		std::cout << "test_gridfs_write !stream" << std::endl;
		mongoc_gridfs_destroy(gridfs);
		goto end;
	}


	{
		mongoc_gridfs_file_opt_t opt = { 0 };
		opt.filename = fname;
		file = mongoc_gridfs_create_file_from_stream (gridfs, stream, &opt);
	}

	if(!file)
	{
		std::cout << "test_gridfs_write !file" << std::endl;
		mongoc_stream_destroy(stream);
		mongoc_gridfs_destroy(gridfs);
		goto end;
	}

	mongoc_gridfs_file_save(file); // 1 stream, 2 stram to file 3 save

	mongoc_gridfs_file_destroy(file);
	stream = 0; // if mongoc_gridfs_create_file_from_stream success stream is freed
	//mongoc_stream_destroy(stream); // don't destory stream
	mongoc_gridfs_destroy(gridfs);

	std::cout << "test_gridfs_write success" << std::endl;

end:
	mongoc_client_destroy (client);

#ifdef _WIN32
   WSACleanup ();
#endif
}

int main(int argc, char *argv[])
{
	init_ssl();
	mongoc_init();

	test_gridfs_write();
	test_gridfs_read();

	mongoc_cleanup ();
	clean_ssl();

	wait_any_key(argc, argv);
	return 0;
}
