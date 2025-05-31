//c_mongo_gridfs_file_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>

#include <yggr/charset/string.hpp>
#include <yggr/nsql_database_system/c_bson_date.hpp>
#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/c_mongo_stream.hpp>
#include <yggr/nsql_database_system/c_mongo_gridfs_file.hpp>
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
std::string g_fname_buffer("gridfs_stream_test_file_buffer");

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

	yggr::nsql_database_system::c_mongo_gridfs_file file(
			mongoc_gridfs_find_one_by_filename(pgridfs, fname.c_str(), &error));
	yggr_test_assert(file);

	if(!file)
	{
		std::cerr << "test_gridfs_read !file" << std::endl;
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	yggr::nsql_database_system::c_mongo_gridfs_stream stream(file);

	if(!stream)
	{
		std::cerr << "test_gridfs_read !stream" << std::endl;
		file.clear();
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	{
		std::string str_buf(file.size(), 0);
		size_t size = stream.read(str_buf);

		yggr_test_assert(size == str_buf.size());

		std::ofstream ofs;
		ofs.open(out_path.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(str_buf.data(), str_buf.size());
		ofs.flush();
		ofs.close();
	}

	stream.clear();
	file.clear();
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
		std::cerr << "test_gridfs_write !pclient" << std::endl;
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

	yggr::nsql_database_system::c_mongo_gridfs_file file(pgridfs, stream,
		yggr::nsql_database_system::c_mongo_gridfs_file::file_opt_type(fname));

	yggr_test_assert(file);
	if(!file)
	{
		std::cout << "test_gridfs_write !file" << std::endl;
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	stream.need_delete() = false;

	bool b = file.save();
	yggr_test_assert(b);

	file.clear();
	mongoc_gridfs_destroy(pgridfs);

	std::cout << "test_gridfs_write success" << std::endl;

	bool bsuccess = conn.push(pclient);
	yggr_test_assert(bsuccess);
}

void test_gridfs_file_of_gridfs_rw(void)
{

#if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_addr = "192.168.1.101"; // you host ip address
#else
	yggr::utf8_string host_addr = "127.0.0.1";
#endif // YGGR_AT_MOBILE

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://xy:123456abc@" + host_addr + ":10398/?tls=true", //"mongodb://xy:123456abc@127.0.0.1:10398/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://" + host_addr + ":10298/?tls=true", //"mongodb://127.0.0.1:10298/?tls=true",	// str_uri
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
				"mongodb://xy:123456abc@" + host_addr + ":10198/?tls=false" //"mongodb://xy:123456abc@127.0.0.1:10198/?tls=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://" + host_addr + ":10098/?tls=false" // "mongodb://127.0.0.1:10098/?tls=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	yggr::nsql_database_system::c_mongo_connection client(client_init);

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		return;
	}

	yggr::nsql_database_system::mongo_accesser::s_database_drop(client, g_db_name.c_str(), 0);

	test_gridfs_write(client);
	test_gridfs_read(client);

	client.clear();

	std::cout << "test_gridfs_file_of_gridfs_rw success" << std::endl;
}

//------------------------rw_buffer------------------

void test_gridfs_read_buffer(yggr::nsql_database_system::c_mongo_connection& conn)
{
	bson_error_t error;

	std::string db_name(g_db_name);
	std::string gridfs_name(g_gridfs_name);
	std::string fname(g_fname_buffer);
	std::string out_path("./nsql_database_system/out_gridfs_buffer_test_file.txt");

	yggr::nsql_database_system::c_mongo_connection::mongoc_client_ptr_type pclient = conn.pop();
	yggr_test_assert(pclient);

	if(!pclient)
	{
		std::cerr << "test_gridfs_read_buffer !pclient" << std::endl;
		return;
	}

	mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(pclient.get(), db_name.c_str(), gridfs_name.c_str(), &error); // gridfs_t
	yggr_test_assert(pgridfs);

	if(!pgridfs)
	{
		std::cerr << "test_gridfs_read_buffer !pgridfs" << std::endl;
		conn.push(pclient);
		return;
	}

	yggr::nsql_database_system::c_mongo_gridfs_file file(
			mongoc_gridfs_find_one_by_filename(pgridfs, fname.c_str(), &error));
	yggr_test_assert(file);

	if(!file)
	{
		std::cerr << "test_gridfs_read_buffer !file" << std::endl;
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	yggr::nsql_database_system::c_mongo_gridfs_stream stream(file);

	if(!stream)
	{
		std::cerr << "test_gridfs_read_buffer !stream" << std::endl;
		file.clear();
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	{
		std::string str_buf(file.size(), 0);
		size_t size = stream.read(str_buf);

		yggr_test_assert(size == str_buf.size());

		std::ofstream ofs;
		ofs.open(out_path.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(str_buf.data(), str_buf.size());
		ofs.flush();
		ofs.close();
	}

	stream.clear();
	file.clear();
	mongoc_gridfs_destroy(pgridfs);

	std::cout << "test_gridfs_read_buffer success" << std::endl;

	bool bsuccess = conn.push(pclient);
	yggr_test_assert(bsuccess);
}

void test_gridfs_write_buffer(yggr::nsql_database_system::c_mongo_connection& conn)
{
	bson_error_t error;

	std::string db_name(g_db_name);
	std::string gridfs_name(g_gridfs_name);
	std::string fname(g_fname_buffer);
	std::string buffer("this_is_a_buffer_data test_gridfs");

	yggr::nsql_database_system::c_mongo_connection::mongoc_client_ptr_type pclient = conn.pop();
	yggr_test_assert(pclient);

	if(!pclient)
	{
		std::cerr << "test_gridfs_write_buffer !pclient" << std::endl;
		return;
	}

	mongoc_gridfs_t* pgridfs = mongoc_client_get_gridfs(pclient.get(), db_name.c_str(), gridfs_name.c_str(), &error);
	yggr_test_assert(pgridfs);

	if(!pgridfs)
	{
		std::cout << "test_gridfs_write_buffer !pgridfs" << std::endl;
		conn.push(pclient);
		return;
	}

	yggr::nsql_database_system::c_mongo_gridfs_file file(pgridfs,
		yggr::nsql_database_system::c_mongo_gridfs_file::file_opt_type(fname));

	yggr_test_assert(file);
	if(!file)
	{
		std::cout << "test_gridfs_write_buffer !file" << std::endl;
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	yggr::nsql_database_system::c_mongo_iovec iovec(buffer);
	file.write(iovec);

	//stream.need_delete() = false;

	bool b = file.save();
	yggr_test_assert(b);

	file.clear();
	mongoc_gridfs_destroy(pgridfs);

	std::cout << "test_gridfs_write_buffer success" << std::endl;

	bool bsuccess = conn.push(pclient);
	yggr_test_assert(bsuccess);
}

void test_gridfs_file_of_gridfs_rw_buffer(void)
{

#if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_addr = "192.168.1.101"; // you host ip address
#else
	yggr::utf8_string host_addr = "127.0.0.1";
#endif // YGGR_AT_MOBILE

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://xy:123456abc@" + host_addr + ":10398/?tls=true", //"mongodb://xy:123456abc@127.0.0.1:10398/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://" + host_addr + ":10298/?tls=true", // "mongodb://127.0.0.1:10298/?tls=true",	// str_uri
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
				"mongodb://xy:123456abc@" + host_addr + ":10198/?tls=false" //"mongodb://xy:123456abc@127.0.0.1:10198/?tls=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://" + host_addr + ":10098/?tls=false" //"mongodb://127.0.0.1:10098/?tls=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	yggr::nsql_database_system::c_mongo_connection client(client_init);

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		return;
	}

	yggr::nsql_database_system::mongo_accesser::s_database_drop(client, g_db_name.c_str(), 0);

	test_gridfs_write_buffer(client);
	test_gridfs_read_buffer(client);

	client.clear();

	std::cout << "test_gridfs_file_of_gridfs_rw_buffer success" << std::endl;
}

// test_cmongo_grids_file_member

void test_mongo_gridfs_file_construct(mongoc_gridfs_t* pgridfs,
										yggr::nsql_database_system::c_mongo_gridfs_file& file_sample)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file mongo_gridfs_file_type;
	typedef yggr::nsql_database_system::c_mongo_gridfs_stream mongo_gridfs_stream_type;
	typedef mongo_gridfs_file_type::file_opt_type file_opt_type;

	{
		mongo_gridfs_file_type file;
		yggr_test_assert(file.empty());
	}

	{
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(pgridfs, boost::addressof(org_file_opt_ref));

		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5_pointer() == 0 || !(file.md5_pointer() == org_file_opt_ref.md5));
		yggr_test_assert(file.file_name_pointer() == 0 || !(file.file_name_pointer() == org_file_opt_ref.filename));
		yggr_test_assert(file.content_pointer() == 0 || !(file.content_pointer() == org_file_opt_ref.content_type));
		yggr_test_assert(file.aliases_pointer() == 0 || !(file.aliases_pointer() == org_file_opt_ref.aliases));
		yggr_test_assert(file.metadata_pointer() == 0 || !(file.metadata_pointer() == org_file_opt_ref.metadata));

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(pgridfs, org_file_opt_ref);

		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5_pointer() == 0 || !(file.md5_pointer() == org_file_opt_ref.md5));
		yggr_test_assert(file.file_name_pointer() == 0 || !(file.file_name_pointer() == org_file_opt_ref.filename));
		yggr_test_assert(file.content_pointer() == 0 || !(file.content_pointer() == org_file_opt_ref.content_type));
		yggr_test_assert(file.aliases_pointer() == 0 || !(file.aliases_pointer() == org_file_opt_ref.aliases));
		yggr_test_assert(file.metadata_pointer() == 0 || !(file.metadata_pointer() == org_file_opt_ref.metadata));

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		file_opt_type chk_file_opt(file_sample.file_opt());
		mongo_gridfs_file_type file(pgridfs, file_sample.file_opt());

		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		mongo_gridfs_stream_type stream(file_sample);
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(pgridfs, stream.org_pointer(), boost::addressof(org_file_opt_ref));
		stream.release();

		yggr_test_assert(!stream);
		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5_pointer() == 0 || !(file.md5_pointer() == org_file_opt_ref.md5));
		yggr_test_assert(file.file_name_pointer() == 0 || !(file.file_name_pointer() == org_file_opt_ref.filename));
		yggr_test_assert(file.content_pointer() == 0 || !(file.content_pointer() == org_file_opt_ref.content_type));
		yggr_test_assert(file.aliases_pointer() == 0 || !(file.aliases_pointer() == org_file_opt_ref.aliases));
		yggr_test_assert(file.metadata_pointer() == 0 || !(file.metadata_pointer() == org_file_opt_ref.metadata));

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		mongo_gridfs_stream_type stream(file_sample);
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(pgridfs, stream.org_pointer(), org_file_opt_ref);
		stream.release();

		yggr_test_assert(!stream);
		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5_pointer() == 0 || !(file.md5_pointer() == org_file_opt_ref.md5));
		yggr_test_assert(file.file_name_pointer() == 0 || !(file.file_name_pointer() == org_file_opt_ref.filename));
		yggr_test_assert(file.content_pointer() == 0 || !(file.content_pointer() == org_file_opt_ref.content_type));
		yggr_test_assert(file.aliases_pointer() == 0 || !(file.aliases_pointer() == org_file_opt_ref.aliases));
		yggr_test_assert(file.metadata_pointer() == 0 || !(file.metadata_pointer() == org_file_opt_ref.metadata));

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		mongo_gridfs_stream_type stream(file_sample);
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(pgridfs, stream.org_pointer(), file_sample.file_opt());
		stream.release();

		yggr_test_assert(!stream);
		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5_pointer() == 0 || !(file.md5_pointer() == org_file_opt_ref.md5));
		yggr_test_assert(file.file_name_pointer() == 0 || !(file.file_name_pointer() == org_file_opt_ref.filename));
		yggr_test_assert(file.content_pointer() == 0 || !(file.content_pointer() == org_file_opt_ref.content_type));
		yggr_test_assert(file.aliases_pointer() == 0 || !(file.aliases_pointer() == org_file_opt_ref.aliases));
		yggr_test_assert(file.metadata_pointer() == 0 || !(file.metadata_pointer() == org_file_opt_ref.metadata));

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		mongo_gridfs_stream_type stream(file_sample);
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(pgridfs, stream, boost::addressof(org_file_opt_ref));

		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5_pointer() == 0 || !(file.md5_pointer() == org_file_opt_ref.md5));
		yggr_test_assert(file.file_name_pointer() == 0 || !(file.file_name_pointer() == org_file_opt_ref.filename));
		yggr_test_assert(file.content_pointer() == 0 || !(file.content_pointer() == org_file_opt_ref.content_type));
		yggr_test_assert(file.aliases_pointer() == 0 || !(file.aliases_pointer() == org_file_opt_ref.aliases));
		yggr_test_assert(file.metadata_pointer() == 0 || !(file.metadata_pointer() == org_file_opt_ref.metadata));

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		mongo_gridfs_stream_type stream(file_sample);
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(pgridfs, stream, org_file_opt_ref);

		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5_pointer() == 0 || !(file.md5_pointer() == org_file_opt_ref.md5));
		yggr_test_assert(file.file_name_pointer() == 0 || !(file.file_name_pointer() == org_file_opt_ref.filename));
		yggr_test_assert(file.content_pointer() == 0 || !(file.content_pointer() == org_file_opt_ref.content_type));
		yggr_test_assert(file.aliases_pointer() == 0 || !(file.aliases_pointer() == org_file_opt_ref.aliases));
		yggr_test_assert(file.metadata_pointer() == 0 || !(file.metadata_pointer() == org_file_opt_ref.metadata));

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		mongo_gridfs_stream_type stream(file_sample);
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(pgridfs, stream, file_sample.file_opt());

		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5_pointer() == 0 || !(file.md5_pointer() == org_file_opt_ref.md5));
		yggr_test_assert(file.file_name_pointer() == 0 || !(file.file_name_pointer() == org_file_opt_ref.filename));
		yggr_test_assert(file.content_pointer() == 0 || !(file.content_pointer() == org_file_opt_ref.content_type));
		yggr_test_assert(file.aliases_pointer() == 0 || !(file.aliases_pointer() == org_file_opt_ref.aliases));
		yggr_test_assert(file.metadata_pointer() == 0 || !(file.metadata_pointer() == org_file_opt_ref.metadata));

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	// don't wrapper twice
	//{
	//	file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
	//	file_opt_type chk_file_opt(org_file_opt_ref);
	//	mongo_gridfs_file_type file(file_sample.org_pointer());
	//
	//	yggr_test_assert(!file.empty());

	//	std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
	//	std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
	//	std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
	//	std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
	//	std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
	//	std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

	//	yggr_test_assert(file.md5() == chk_file_opt.var_md5());
	//	yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
	//	yggr_test_assert(file.content() == chk_file_opt.var_content());
	//	yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
	//	yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
	//	yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	//}

	{
		mongo_gridfs_file_type file_tmp(file_sample);
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(boost::move(file_tmp));

		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(file_sample);

		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	std::cout << "----------------test_mongo_gridfs_file_construct end------------" << std::endl;
}

void test_mongo_gridfs_file_operator_set(mongoc_gridfs_t* pgridfs,
											yggr::nsql_database_system::c_mongo_gridfs_file& file_sample)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file mongo_gridfs_file_type;
	typedef yggr::nsql_database_system::c_mongo_gridfs_stream mongo_gridfs_stream_type;
	typedef mongo_gridfs_file_type::file_opt_type file_opt_type;

	{
		mongo_gridfs_file_type file;
		yggr_test_assert(file.empty());
	}

	{
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type tmp_file(file_sample);
		mongo_gridfs_file_type file;
		file = boost::move(tmp_file);


		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file;
		file = file_sample;

		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	std::cout << "----------------test_mongo_gridfs_file_operator_set end------------" << std::endl;
}

void test_mongo_gridfs_file_swap(mongoc_gridfs_t* pgridfs,
									yggr::nsql_database_system::c_mongo_gridfs_file& file_sample)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file mongo_gridfs_file_type;
	typedef yggr::nsql_database_system::c_mongo_gridfs_stream mongo_gridfs_stream_type;
	typedef mongo_gridfs_file_type::file_opt_type file_opt_type;

	{
		mongo_gridfs_file_type file;
		yggr_test_assert(file.empty());
	}

	{
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type tmp_file(file_sample);
		mongo_gridfs_file_type file;
		file.swap(boost::move(tmp_file));


		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	{
		file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type tmp_file(file_sample);
		mongo_gridfs_file_type file;
		file.swap(tmp_file);

		yggr_test_assert(!file.empty());

		std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
		std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
		std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
		std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
		std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
		std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

		yggr_test_assert(file.md5() == chk_file_opt.var_md5());
		yggr_test_assert(file.file_name() == chk_file_opt.var_file_name());
		yggr_test_assert(file.content() == chk_file_opt.var_content());
		yggr_test_assert(file.aliases() == chk_file_opt.var_aliases());
		yggr_test_assert(file.metadata() == chk_file_opt.var_metadata());
		yggr_test_assert(file.chunk_size() == chk_file_opt.var_chunk_size());
	}

	std::cout << "----------------test_mongo_gridfs_file_swap end------------" << std::endl;
}

void test_mongo_gridfs_file_member_foo(mongoc_gridfs_t* pgridfs,
										yggr::nsql_database_system::c_mongo_gridfs_file& file_sample)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_mongo_gridfs_file mongo_gridfs_file_type;
	typedef yggr::nsql_database_system::c_mongo_gridfs_stream mongo_gridfs_stream_type;
	typedef mongo_gridfs_file_type::file_opt_type file_opt_type;

	{
		mongo_gridfs_file_type file;
		yggr_test_assert(file.empty());
	}

	{
		//file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
		//file_opt_type chk_file_opt(org_file_opt_ref);
		mongo_gridfs_file_type file(file_sample);

		//mongo_gridfs_file_type& file_ref = file;
		const mongo_gridfs_file_type& file_cref = file;

		{
			std::cout << file.aliases() << std::endl;
			std::cout << file_cref.aliases() << std::endl;

			bson_type aliases_obj("{\"aliases\" : [\"aaa\", \"bbb\", \"ccc\"]}");
			file.aliases() = aliases_obj;
			std::cout << file_cref.aliases() << std::endl;
			yggr_test_assert(file_cref.aliases() == aliases_obj);
			yggr_test_assert(file_sample.aliases() == aliases_obj);
		}

		{
			std::cout << file.metadata() << std::endl;
			std::cout << file_cref.metadata() << std::endl;

			bson_type metadata_obj("{ \"metadata\" : { \"tag1\" : 1, \"tag2\" : 2 } }");
			file.metadata() = metadata_obj;
			std::cout << file_cref.metadata() << std::endl;
			yggr_test_assert(file_cref.metadata() == metadata_obj);
			yggr_test_assert(file_sample.metadata() == metadata_obj);
		}

		{
			std::cout << file.content() << std::endl;
			std::cout << file_cref.content() << std::endl;

			file.content() = "contentType";
			std::cout << file_cref.content() << std::endl;
			yggr_test_assert(file_cref.content() == "contentType");
			yggr_test_assert(file_sample.content() == "contentType");

			yggr_test_assert(file_cref.content<std::string>() == "contentType");
			yggr_test_assert(file_cref.content<boost::container::string>() == "contentType");
			yggr_test_assert(file_cref.content<yggr::string>() == "contentType");

			yggr_test_assert(file_cref.content<std::string>(file_cref.content<std::string>().get_allocator()) == "contentType");
			yggr_test_assert(file_cref.content<boost::container::string>(file_cref.content<boost::container::string>().get_allocator()) == "contentType");
			yggr_test_assert(file_cref.content<yggr::string>(file_cref.content<yggr::string>().get_allocator()) == "contentType");
		}

		{
			std::cout << file.file_name() << std::endl;
			std::cout << file_cref.file_name() << std::endl;

			file.file_name() = "gridfs_stream_test_file2";
			std::cout << file_cref.file_name() << std::endl;
			yggr_test_assert(file_cref.file_name() == "gridfs_stream_test_file2");
			yggr_test_assert(file_sample.file_name() == "gridfs_stream_test_file2");

			yggr_test_assert(file_cref.file_name<std::string>() == "gridfs_stream_test_file2");
			yggr_test_assert(file_cref.file_name<boost::container::string>() == "gridfs_stream_test_file2");
			yggr_test_assert(file_cref.file_name<yggr::string>() == "gridfs_stream_test_file2");

			yggr_test_assert(file_cref.file_name<std::string>(file_cref.file_name<std::string>().get_allocator()) == "gridfs_stream_test_file2");
			yggr_test_assert(file_cref.file_name<boost::container::string>(file_cref.file_name<boost::container::string>().get_allocator()) == "gridfs_stream_test_file2");
			yggr_test_assert(file_cref.file_name<yggr::string>(file_cref.file_name<yggr::string>().get_allocator()) == "gridfs_stream_test_file2");
		}

		{
			std::cout << file.md5() << std::endl;
			std::cout << file_cref.md5() << std::endl;

			file.md5() = "md5_string";
			std::cout << file_cref.md5() << std::endl;
			yggr_test_assert(file_cref.md5() == "md5_string");
			yggr_test_assert(file_sample.md5() == "md5_string");

			yggr_test_assert(file_cref.md5<std::string>() == "md5_string");
			yggr_test_assert(file_cref.md5<boost::container::string>() == "md5_string");
			yggr_test_assert(file_cref.md5<yggr::string>() == "md5_string");

			yggr_test_assert(file_cref.md5<std::string>(file_cref.md5<std::string>().get_allocator()) == "md5_string");
			yggr_test_assert(file_cref.md5<boost::container::string>(file_cref.md5<boost::container::string>().get_allocator()) == "md5_string");
			yggr_test_assert(file_cref.md5<yggr::string>(file_cref.md5<yggr::string>().get_allocator()) == "md5_string");
		}

		{
			std::cout << file_sample.chunk_size() << std::endl;
			yggr_test_assert(file_sample.chunk_size() == 261120);
		}

		{
			file_opt_type::org_type org_file_opt_ref = file_sample.file_opt_reference();
			file_opt_type chk_file_opt(org_file_opt_ref);

			std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
			std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
			std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
			std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
			std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
			std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

			yggr_test_assert(file_sample.md5_pointer() == org_file_opt_ref.md5);
			yggr_test_assert(file_sample.file_name_pointer() == org_file_opt_ref.filename);
			yggr_test_assert(file_sample.content_pointer() == org_file_opt_ref.content_type);
			yggr_test_assert(file_sample.aliases_pointer() == org_file_opt_ref.aliases);
			yggr_test_assert(file_sample.metadata_pointer() == org_file_opt_ref.metadata);

			yggr_test_assert(file_sample.md5() == chk_file_opt.var_md5());
			yggr_test_assert(file_sample.file_name() == chk_file_opt.var_file_name());
			yggr_test_assert(file_sample.content() == chk_file_opt.var_content());
			yggr_test_assert(file_sample.aliases() == chk_file_opt.var_aliases());
			yggr_test_assert(file_sample.metadata() == chk_file_opt.var_metadata());
			yggr_test_assert(file_sample.chunk_size() == chk_file_opt.var_chunk_size());
		}

		{
			file_opt_type chk_file_opt = file_sample.file_opt();

			std::cout << "var_md5: " << chk_file_opt.var_md5() << std::endl;
			std::cout << "var_file_name: " << chk_file_opt.var_file_name() << std::endl;
			std::cout << "var_content: " << chk_file_opt.var_content() << std::endl;
			std::cout << "var_aliases: " << chk_file_opt.var_aliases() << std::endl;
			std::cout << "var_metadata: " << chk_file_opt.var_metadata() << std::endl;
			std::cout << "var_chunk_size: " << chk_file_opt.var_chunk_size() << std::endl;

			yggr_test_assert(file_sample.md5() == chk_file_opt.var_md5());
			yggr_test_assert(file_sample.file_name() == chk_file_opt.var_file_name());
			yggr_test_assert(file_sample.content() == chk_file_opt.var_content());
			yggr_test_assert(file_sample.aliases() == chk_file_opt.var_aliases());
			yggr_test_assert(file_sample.metadata() == chk_file_opt.var_metadata());
			yggr_test_assert(file_sample.chunk_size() == chk_file_opt.var_chunk_size());
		}

		{
			yggr_test_assert(!file_sample.empty());

			std::cout << file_sample.length() << std::endl;

#if defined(YGGR_AT_WINDOWS)
			yggr_test_assert(file_sample.length() == 41);
#elif defined(YGGR_AT_LINUX)
#	if defined(YGGR_AT_ANDROID)
            yggr_test_assert(file_sample.length() == 41);
#	else
			yggr_test_assert(file_sample.length() == 40);
#	endif // YGGR_AT_ANDROID
#elif defined(YGGR_AT_DARWIN)
            yggr_test_assert(file_sample.length() == 40);
#else
#   error "append other platfrom test code"
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(file_sample.length() == file_sample.size());
		}

		{
			std::cout << file_sample.var_id() << std::endl;
			std::cout << file_sample.id() << std::endl;

			const yggr::nsql_database_system::c_mongo_gridfs_file& file_sample_cref = file_sample;
			yggr::nsql_database_system::c_bson_oid chk_oid = file_sample_cref.var_id(); //ok
			//yggr::nsql_database_system::c_bson_oid chk_oid = file_sample.var_id().view(); //ok
			//yggr::nsql_database_system::c_bson_oid chk_oid = file_sample.var_id(); // not ok, msvc built-in operator+=(const T&, T) conflict
			yggr_test_assert(chk_oid != chk_oid.s_gen_empty_oid());

			yggr_test_assert(file_sample.var_id() == chk_oid);
			yggr_test_assert(file_sample.id() == chk_oid);

			mongo_gridfs_file_type file;
			const mongo_gridfs_file_type& file_cref = file;
			yggr_test_assert(!file_cref);
			yggr_test_assert(file_cref.var_id().value_typeid() == BSON_TYPE_EOD);
			yggr_test_assert(file_cref.id() == yggr::nsql_database_system::c_bson_oid::s_gen_empty_oid());
		}

		{
			std::cout << file_sample.upload_date() << std::endl;
			yggr::nsql_database_system::c_bson_date upload_date(file_sample.upload_date());
			std::cout << upload_date.to_local_string() << std::endl;
		}

		{
			std::cout << file_sample.error() << std::endl;
			std::cout << file_sample.error<bson_error_t>() << std::endl;

			{
				yggr::nsql_database_system::c_bson_error err;
				bool berr = file_sample.error(err);

				yggr_test_assert(!berr);
				std::cout << err << std::endl;
			}

			{
				yggr::nsql_database_system::c_bson_error err;
				bool berr = file_sample.error(boost::addressof(err));

				yggr_test_assert(!berr);
				std::cout << err << std::endl;
			}

			{
				yggr::nsql_database_system::c_bson_error::org_type err = {0};
				bool berr = file_sample.error(err);

				yggr_test_assert(!berr);
				std::cout << err << std::endl;
			}

			{
				yggr::nsql_database_system::c_bson_error::org_type err = {0};
				bool berr = file_sample.error(boost::addressof(err));

				yggr_test_assert(!berr);
				std::cout << err << std::endl;
			}
		}
	}

	std::cout << "----------------test_mongo_gridfs_file_member_foo end------------" << std::endl;
}

void test_gridfs_member_detail(yggr::nsql_database_system::c_mongo_connection& conn)
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

	yggr::nsql_database_system::c_mongo_gridfs_file file(
			mongoc_gridfs_find_one_by_filename(pgridfs, fname.c_str(), &error));
	yggr_test_assert(file);

	if(!file)
	{
		std::cerr << "test_gridfs_read !file" << std::endl;
		mongoc_gridfs_destroy(pgridfs);
		conn.push(pclient);
		return;
	}

	test_mongo_gridfs_file_construct(pgridfs, file);
	test_mongo_gridfs_file_operator_set(pgridfs, file);
	test_mongo_gridfs_file_swap(pgridfs, file);
	test_mongo_gridfs_file_member_foo(pgridfs, file);

	file.clear();
	mongoc_gridfs_destroy(pgridfs);

	std::cout << "test_gridfs_member_detail success" << std::endl;

	bool bsuccess = conn.push(pclient);
	yggr_test_assert(bsuccess);
}

void test_gridfs_file_members(void)
{

#if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_addr = "192.168.1.101"; // you host ip address
#else
	yggr::utf8_string host_addr = "127.0.0.1";
#endif // YGGR_AT_MOBILE

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://xy:123456abc@" + host_addr + ":10398/?tls=true", //"mongodb://xy:123456abc@127.0.0.1:10398/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://" + host_addr + ":10298/?tls=true", // "mongodb://127.0.0.1:10298/?tls=true",	// str_uri
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
				"mongodb://xy:123456abc@" + host_addr + ":10198/?tls=false" //"mongodb://xy:123456abc@127.0.0.1:10198/?tls=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://" + host_addr + ":10098/?tls=false" //"mongodb://127.0.0.1:10098/?tls=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	yggr::nsql_database_system::c_mongo_connection client(client_init);

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		return;
	}

	yggr::nsql_database_system::mongo_accesser::s_database_drop(client, g_db_name.c_str(), 0);

	test_gridfs_write(client);
	test_gridfs_member_detail(client);

	client.clear();

	std::cout << "test_gridfs_file_members success" << std::endl;
}

int main(int argc, char* argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();

	test_gridfs_file_of_gridfs_rw();
	test_gridfs_file_of_gridfs_rw_buffer();

	test_gridfs_file_members();

	yggr::nsql_database_system::mongodb_installer::uninstall();

	wait_any_key(argc, argv);
	return 0;
}
