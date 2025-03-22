//c_mongo_stream_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>

//#include <stdio.h>
//#include <stdlib.h>

#include <yggr/charset/string.hpp>

#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/c_mongo_stream.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>

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

std::string g_db_name("test_stream_gridfs");
std::string g_gridfs_name("fs");
std::string g_fname("gridfs_stream_test_file");

void test_gridfs_read(yggr::nsql_database_system::c_mongo_connection& conn)
{
	bson_error_t error;

	std::string db_name(g_db_name);
	std::string gridfs_name(g_gridfs_name);
	std::string fname(g_fname);
	std::string out_path("./nsql_database_system/out_gridfs_test_file.txt");

	yggr::nsql_database_system::c_mongo_connection::mongoc_client_ptr_type pclient = conn.pop();
	yggr_test_assert(pclient);

	if(!pclient)
	{
		std::cerr << "test_gridfs_read !pclient" << std::endl;
		return;
	}

	mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(pclient.get(), db_name.c_str(), gridfs_name.c_str(), &error); // gridfs_t
	yggr_test_assert(pgridfs);

	if(!pgridfs)
	{
		std::cerr << "test_gridfs_read !pgridfs" << std::endl;
		conn.push(pclient);
		return;
	}

	mongoc_gridfs_file_t* pfile = mongoc_gridfs_find_one_by_filename(pgridfs, fname.c_str(), &error); // gridfs_file_t
	yggr_test_assert(pfile);

	if(!pfile)
	{
		std::cerr << "test_gridfs_read !pfile" << std::endl;
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	yggr::nsql_database_system::c_mongo_gridfs_stream stream(pfile);

	if(!stream)
	{
		std::cerr << "test_gridfs_read !stream" << std::endl;
		mongoc_gridfs_file_destroy(pfile);
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	{
		std::string str_buf(mongoc_gridfs_file_get_length(pfile), 0);
		size_t size = stream.read(str_buf);

		yggr_test_assert(size == str_buf.size());

		std::ofstream ofs;
		ofs.open(out_path.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(str_buf.data(), str_buf.size());
		ofs.flush();
		ofs.close();
	}

	stream.clear();
	mongoc_gridfs_file_destroy(pfile);
	mongoc_gridfs_destroy(pgridfs);

	std::cout << "test_gridfs_read success" << std::endl;

	bool bsuccess = conn.push(pclient);
	yggr_test_assert(bsuccess);
}

void test_gridfs_write(yggr::nsql_database_system::c_mongo_connection& conn)
{
	bson_error_t error;
	
	std::string db_name(g_db_name);
	std::string gridfs_name(g_gridfs_name);
	std::string fname(g_fname);
	std::string local_path("./nsql_database_system/gridfs_test_file.txt");
	
	yggr::nsql_database_system::c_mongo_connection::mongoc_client_ptr_type pclient = conn.pop();
	yggr_test_assert(pclient);

	if(!pclient)
	{
		std::cerr << "test_gridfs_read !pclient" << std::endl;
		return;
	}

	mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(pclient.get(), db_name.c_str(), gridfs_name.c_str(), &error);
	yggr_test_assert(pgridfs);

	if(!pgridfs)
	{
		std::cout << "test_gridfs_write !pgridfs" << std::endl;
		conn.push(pclient);
		return;
	}

	yggr::nsql_database_system::c_mongo_file_stream stream(local_path);
	if(!stream)
	{
		std::cout << "test_gridfs_write !stream" << std::endl;
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	mongoc_gridfs_file_t* pfile = 0;
	{
		mongoc_gridfs_file_opt_t opt = { 0 };
		opt.filename = fname.c_str();
		pfile = mongoc_gridfs_create_file_from_stream(pgridfs, stream, &opt);
	}

	yggr_test_assert(pfile);
	if(!pfile)
	{
		std::cout << "test_gridfs_write !file" << std::endl;
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	stream.need_delete() = false;
	mongoc_gridfs_file_save(pfile); // 1 stream, 2 stram to file 3 save

	mongoc_gridfs_file_destroy(pfile);
	mongoc_gridfs_destroy(pgridfs);

	std::cout << "test_gridfs_write success" << std::endl;

	bool bsuccess = conn.push(pclient);
	yggr_test_assert(bsuccess);
}

void test_stream_of_gridfs_rw(void)
{
	yggr::nsql_database_system::mongodb_installer::install();

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://xy:123456abc@127.0.0.1:10398/?ssl=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://127.0.0.1:10298/?ssl=true",	// str_uri
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
				"mongodb://xy:123456abc@127.0.0.1:10198/?ssl=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://127.0.0.1:10098/?ssl=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	yggr::nsql_database_system::c_mongo_connection client(client_init);

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		//return EXIT_FAILURE;
		return;
	}

	yggr::nsql_database_system::mongo_accesser::s_database_drop(client, g_db_name.c_str(), 0);

	test_gridfs_write(client);
	test_gridfs_read(client);

	client.clear();

	yggr::nsql_database_system::mongodb_installer::uninstall();

	std::cout << "test_stream_of_gridfs_rw success" << std::endl;
}

int main(int argc, char* argv[])
{
	test_stream_of_gridfs_rw();

	wait_any_key(argc, argv);
	return 0;
}