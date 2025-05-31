//mongo_accesser_test_of_connection_pool_mt.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>

#include <boost/atomic.hpp>
#include <boost/function.hpp>

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

#include <yggr/utility/to_pointer.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

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

boost::atomic<bool> g_is_run;

void test_insert(yggr::nsql_database_system::c_mongo_connection_pool& conn,
					const boost::function0<void>& handler)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson bs;

	bs.save("test_insert_1_2", 2);

	for(;g_is_run.load();)
	{
		yggr::nsql_database_system::c_mongo_write_concern wcon;
		yggr::nsql_database_system::c_bson_error err_ins;

		yggr::nsql_database_system::mongo_accesser::s_collection_insert(
			conn, db_name, coln_name, bs, opts, reply, err_ins);
	}

	handler();
}

void test_find(yggr::nsql_database_system::c_mongo_connection_pool& conn,
				const boost::function0<void>& handler)
{
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

	for(;g_is_run.load();)
	{
		yggr::nsql_database_system::c_bson filter;
		yggr::nsql_database_system::c_bson_error err_ins;

		bson_list_type bs_list;

		yggr::nsql_database_system::mongo_accesser::s_collection_find(
			conn, db_name, coln_name,
			filter, 0, 0, 
			err_ins,
			boost::bind(&collector::collecting, boost::ref(bs_list), _1));


		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	handler();
}

void test_drop(yggr::nsql_database_system::c_mongo_connection_pool& conn,
				const boost::function0<void>& handler)
{
	typedef yggr::thread::this_thread<boost::thread> this_thread_type;
	typedef yggr::thread::boost_thread_config_type thread_config_type;
	
	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	for(;g_is_run.load();)
	{
		yggr::nsql_database_system::c_bson_error err;
	
		yggr::nsql_database_system::mongo_accesser::s_collection_drop(
			conn, db_name, coln_name, err);

		
		this_thread_type::yield();
		this_thread_type::sleep(thread_config_type::time_type(0, 33000000));
	}

	handler();
}

void test_collections(int argc, char *argv[])
{
	typedef yggr::thread::this_thread<boost::thread> this_thread_type;
	typedef yggr::thread::boost_thread_config_type thread_config_type;

	thread_config_type::thread_group_type trd_group;

	g_is_run = true;

#if defined(YGGR_AT_MOBILE)
	yggr::utf8_string host_addr = "192.168.1.101"; // you host ip address
#else
	yggr::utf8_string host_addr = "127.0.0.1";
#endif // YGGR_AT_MOBILE


#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection_pool::init_type client_init(
				"mongodb://xy:123456abc@" + host_addr + ":10398/?ssl=true",	 //"mongodb://xy:123456abc@127.0.0.1:10398/?ssl=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection_pool::init_type client_init(
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
	yggr::nsql_database_system::c_mongo_connection_pool::init_type client_init(
				"mongodb://xy:123456abc@" + host_addr + ":10198/?ssl=false" //"mongodb://xy:123456abc@127.0.0.1:10198/?ssl=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection_pool::init_type client_init(
				"mongodb://" + host_addr + ":10098/?ssl=false" //"mongodb://127.0.0.1:10098/?ssl=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	yggr::nsql_database_system::c_mongo_connection_pool client(client_init);

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		client.clear();
		return;
	}

	//boost::function0<void> foo(boost::bind(&yggr::nsql_database_system::mongodb_installer::this_thread_clear));

	trd_group.create_thread(
		boost::bind(&test_insert, boost::ref(client), 
					boost::function0<void>(
						boost::bind(&yggr::nsql_database_system::mongodb_installer::this_thread_clear))));
	
	trd_group.create_thread(
		boost::bind(&test_find, boost::ref(client), 
					boost::function0<void>(
						boost::bind(&yggr::nsql_database_system::mongodb_installer::this_thread_clear))));
	
	trd_group.create_thread(
		boost::bind(&test_drop, boost::ref(client), 
					boost::function0<void>(
						boost::bind(&yggr::nsql_database_system::mongodb_installer::this_thread_clear))));

	if(!detail::wait_any_key_detail(argc, argv))
	{
		this_thread_type::sleep(thread_config_type::time_type(10, 0));
	}
	g_is_run = false;
	trd_group.join_all();

	client.clear();

	std::cout << "---------test_collections end--------------" << std::endl;
}

int main(int argc, char *argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();

	test_collections(argc, argv);

	yggr::nsql_database_system::mongodb_installer::uninstall();
	
	std::cout << "--------------all end-----------------" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}