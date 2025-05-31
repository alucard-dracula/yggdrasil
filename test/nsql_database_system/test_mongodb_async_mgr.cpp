//mongo_accesser_test_of_connection.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>

#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>

#include <yggr/container/list.hpp>
#include <yggr/container/map.hpp>
#include <yggr/container/align_vector.hpp>
#include <yggr/utility/to_pointer.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>
#include <yggr/nsql_database_system/c_mongo_connection_pool.hpp>
#include <yggr/nsql_database_system/c_mongo_index_description.hpp>
#include <yggr/nsql_database_system/c_mongo_find_and_modify_opts.hpp>
#include <yggr/nsql_database_system/mongo_bulk_operation_helper.hpp>
#include <yggr/nsql_database_system/mongo_utility.hpp>
#include <yggr/nsql_database_system/mongo_accesser.hpp>

#include <yggr/nsql_database_system/mongodb_async_mgr.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(base)

// 0 not-using-ssl 1 using-ssl
#define MONGODB_USING_CA() 0

// 0 not-using-auth 1 using-auth
#define MONGODB_USING_AUTH() 0

// ------------------collections test----------------------------
// insert

//#define USE_AUTH() 0

template<typename Conn, typename Accesser>
bool test_insert(Conn& conn, /*const Accesser&,*/ const yggr::any&, yggr::any&)
{
	typedef Accesser accesser_type;

	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs;
	bs.save("test_insert_1_2", 2);

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err_ins;

	accesser_type::s_collection_insert(
		conn, db_name, coln_name, bs, opts, reply, err_ins);


	//std::cout << "write_concern" << wcon << std::endl;
	std::cout << "bson_error" << err_ins << std::endl;

	return true;
	
}

template<typename Conn, typename Accesser>
bool test_find(Conn& conn, /*const Accesser&,*/ const yggr::any&, yggr::any&)
{
	typedef Accesser accesser_type;
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;
	
	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	struct collector
	{
		static bool collecting(bson_list_type& list, const yggr::nsql_database_system::c_bson& data)
		{
			list.push_back(data);
			return true;
		}
	};

	yggr::nsql_database_system::c_bson filter;

	yggr::nsql_database_system::c_bson_error err;

	bson_list_type bs_list;

	accesser_type::s_collection_find(
			conn, db_name, coln_name,
			&filter, 0, 0, 
			&err,
			boost::bind(&collector::collecting, boost::ref(bs_list), _1));

	for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
	{
		std::cout << (*i) << std::endl;
	}

	return true;
}

template<typename Conn, typename Accesser>
bool test_drop(Conn& conn, /*const Accesser&,*/ const yggr::any&, yggr::any&)
{
	typedef Accesser accesser_type;
	
	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson_error err;
	accesser_type::s_collection_drop(conn, db_name, coln_name, 0);
	return true;
}

template<typename OutData, typename DB_Mgr>
void stop_test(bool bsuccess, OutData& out, DB_Mgr& db_mgr)
{
	db_mgr.stop();
}

void test_mongodb_mgr_async_execute_of_connection(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection conn_type;
	typedef yggr::nsql_database_system::mongo_accesser accesser_type;

	typedef yggr::nsql_database_system::mongodb_async_mgr<
				yggr::string, conn_type, accesser_type> db_mgr_type;

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
				"mongodb://" + host_addr + ":10098/?ssl=false" //"mongodb://127.0.0.1:10098/?ssl=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

#if 0
	db_mgr_type db_mgr(client_init);
#else

	db_mgr_type db_mgr;
	db_mgr.init(client_init);
#endif // 0, 1

	db_mgr.register_command("test_insert", 
								boost::bind(&test_insert<conn_type, accesser_type>, _1, _2, _3));

	db_mgr.register_command("test_find", 
								boost::bind(&test_find<conn_type, accesser_type>, _1, _2, _3));

	db_mgr.register_command("test_drop", 
								boost::bind(&test_drop<conn_type, accesser_type>, _1, _2, _3));


	db_mgr.start(1);

	db_mgr.async_execute("test_insert");
	db_mgr.async_execute("test_find");
	db_mgr.async_execute("test_drop", boost::bind(&stop_test<yggr::any, db_mgr_type>, 
													_1, _2, boost::ref(db_mgr)));

	std::cout << "post end" << std::endl;
	db_mgr.join();
	db_mgr.clear();

	std::cout << "---------test_mongodb_mgr_async_execute_of_connection end--------------" << std::endl;
}

void test_mongodb_mgr_async_run_command_of_connection(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection conn_type;
	typedef yggr::nsql_database_system::mongo_accesser accesser_type;

	typedef yggr::nsql_database_system::mongodb_async_mgr<
				yggr::string, conn_type, accesser_type> db_mgr_type;

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
				"mongodb://" + host_addr + ":10098/?ssl=false" //"mongodb://127.0.0.1:10098/?ssl=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	db_mgr_type db_mgr(client_init);

	db_mgr.start(1);

	db_mgr.async_run_command(boost::bind(&test_insert<conn_type, accesser_type>, _1, _2, _3));
	db_mgr.async_run_command(boost::bind(&test_find<conn_type, accesser_type>, _1, _2, _3));
	db_mgr.async_run_command(boost::bind(&test_drop<conn_type, accesser_type>, _1, _2, _3), 
								boost::bind(&stop_test<yggr::any, db_mgr_type>, _1, _2, boost::ref(db_mgr)));

	std::cout << "post end" << std::endl;
	db_mgr.join();
	db_mgr.clear();

	std::cout << "---------test_mongodb_mgr_async_run_command_of_connection end--------------" << std::endl;
}

void test_mongodb_mgr_async_execute_of_connection_pool(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection_pool conn_type;
	typedef yggr::nsql_database_system::mongo_accesser accesser_type;

	typedef yggr::nsql_database_system::mongodb_async_mgr<
				yggr::string, conn_type, accesser_type> db_mgr_type;

#if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_addr = "192.168.1.101"; // you host ip address
#else
	yggr::utf8_string host_addr = "127.0.0.1";
#endif // YGGR_AT_MOBILE

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
		db_mgr_type db_mgr(
						"mongodb://xy:123456abc@" + host_addr + ":10398/?ssl=true", //"mongodb://xy:123456abc@127.0.0.1:10398/?ssl=true",	// str_uri
						"./nsql_database_system/cert/client.pem",		// pem_file
						"",										// pem_pwd
						"./nsql_database_system/cert/ca.pem",	// ca_file
						"./nsql_database_system/cert",				// ca_dir
						""										// crl_file
					);
#	else
		db_mgr_type db_mgr(
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
		db_mgr_type db_mgr(
						"mongodb://xy:123456abc@" + host_addr + ":10198/?ssl=false"	 //"mongodb://xy:123456abc@127.0.0.1:10198/?ssl=false"	// str_uri
					);
#	else
		db_mgr_type db_mgr(
						"mongodb://" + host_addr + ":10098/?ssl=false" //"mongodb://127.0.0.1:10098/?ssl=false"	// str_uri
					);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA


	db_mgr.register_command("test_insert", 
								boost::bind(&test_insert<conn_type, accesser_type>, _1, _2, _3));

	db_mgr.register_command("test_find", 
								boost::bind(&test_find<conn_type, accesser_type>, _1, _2, _3));

	db_mgr.register_command("test_drop", 
								boost::bind(&test_drop<conn_type, accesser_type>, _1, _2, _3));

	db_mgr.start(1);

	db_mgr.async_execute("test_insert");
	db_mgr.async_execute("test_find");
	db_mgr.async_execute("test_drop", boost::bind(&stop_test<yggr::any, db_mgr_type>, 
													_1, _2, boost::ref(db_mgr)));
	std::cout << "post end" << std::endl;
	db_mgr.join();
	db_mgr.clear();

	std::cout << "---------test_mongodb_mgr_async_execute_of_connection_pool end--------------" << std::endl;
}

void test_mongodb_mgr_async_run_command_of_connection_pool(void)
{
	typedef yggr::nsql_database_system::c_mongo_connection_pool conn_type;
	typedef yggr::nsql_database_system::mongo_accesser accesser_type;

	typedef yggr::nsql_database_system::mongodb_async_mgr<
				yggr::string, conn_type, accesser_type> db_mgr_type;

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
				"mongodb://" + host_addr + ":10098/?ssl=false" //"mongodb://127.0.0.1:10098/?ssl=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	db_mgr_type db_mgr(client_init);

	db_mgr.start(2);

	db_mgr.async_run_command(boost::bind(&test_insert<conn_type, accesser_type>, _1, _2, _3));
	db_mgr.async_run_command(boost::bind(&test_find<conn_type, accesser_type>, _1, _2, _3));
	db_mgr.async_run_command(boost::bind(&test_drop<conn_type, accesser_type>, _1, _2, _3), 
								boost::bind(&stop_test<yggr::any, db_mgr_type>, _1, _2, boost::ref(db_mgr)));

	std::cout << "post end" << std::endl;
	db_mgr.join();
	db_mgr.clear();

	std::cout << "---------test_mongodb_mgr_async_run_command_of_connection_pool end--------------" << std::endl;
}

int main(int argc, char *argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();

	test_mongodb_mgr_async_execute_of_connection();
	test_mongodb_mgr_async_execute_of_connection_pool();

	test_mongodb_mgr_async_run_command_of_connection();
	test_mongodb_mgr_async_run_command_of_connection_pool();

	yggr::nsql_database_system::mongodb_installer::uninstall();
	
	std::cout << "--------------all end-----------------" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}