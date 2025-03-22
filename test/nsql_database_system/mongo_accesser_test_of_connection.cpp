//mongo_accesser_test_of_connection.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <fstream>

#include <yggr/bind/bind.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/map.hpp>
#include <yggr/container/align_vector.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>
#include <yggr/nsql_database_system/c_mongo_connection.hpp>
#include <yggr/nsql_database_system/c_mongo_index_description.hpp>
#include <yggr/nsql_database_system/c_mongo_find_and_modify_opts.hpp>
#include <yggr/nsql_database_system/c_mongo_index_model.hpp>
#include <yggr/nsql_database_system/mongo_bulk_operation_helper.hpp>
#include <yggr/nsql_database_system/mongo_utility.hpp>

#include <yggr/nsql_database_system/mongo_accesser.hpp>

#include <yggr/utility/to_pointer.hpp>

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

// ------------------collections test----------------------------
// insert one
void test_insert_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	{
		yggr::string db_name = "test";
		yggr::string coln_name = "foo";
		yggr::nsql_database_system::c_bson bs;
		bs.save("test_insert_1_1", 1);
		bson_error_t err_ins = {0};
		yggr::nsql_database_system::mongo_accesser::s_collection_insert(
			conn, db_name.c_str(), coln_name.c_str(),
			&bs, 0, 0, &err_ins);
	}

	{
		yggr::string db_name = "test";
		yggr::string coln_name = "foo";
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson bs;
		bs.save("test_insert_1_1", 1);
		//bson_error_t err_ins = {0};
		yggr::nsql_database_system::mongo_accesser::s_collection_insert(
			conn, db_name.c_str(), coln_name.c_str(),
			&bs, &opts, 0, 0);
	}

	{
		yggr::string db_name = "test";
		yggr::utf8_string coln_name = "foo";
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson bs;
		bs.save("test_insert_1_1", 1);
		bson_error_t err_ins = {0};
		yggr::nsql_database_system::mongo_accesser::s_collection_insert(
			conn, db_name.c_str(), coln_name,
			&bs, opts, &reply, err_ins);

		std::cout << reply << std::endl;
	}

	{
		yggr::string db_name = "test";
		std::wstring coln_name = L"foo";
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson bs;
		bs.save("test_insert_1_1", 1);
		bson_error_t err_ins = {0};
		yggr::nsql_database_system::mongo_accesser::s_collection_insert(
			conn, db_name.c_str(), coln_name,
			bs, opts, reply, &err_ins);

		std::cout << reply << std::endl;
	}
}


void test_insert_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs;
	bs.save("test_insert_1_2", 2);

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err_ins;

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn, db_name, coln_name,
		bs, opts, reply, err_ins);

	std::cout << reply << std::endl;
}

void test_insert_1_3(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs;
	bs.save("test_insert_1_3", 3);

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err_ins;

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn, db_name, coln_name,
		bs, opts, reply, err_ins);

	std::cout << reply << std::endl;
}

// insert many

void test_insert_2_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs2;
	bs2.save("test_insert_2_1", 1);

	bson_error_t err_ins_b = {0};
	bson_t* bs_arr[3] = {&bs2, &bs2, &bs2};

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn,
		db_name.c_str(), coln_name.c_str(),
		const_cast<const bson_t**>(&bs_arr[0]), 3,
		0,
		0, &err_ins_b);
}

void test_insert_2_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs;
	bs.save("test_insert_2_2", 2);

	std::YGGR_ALIGN_VECTOR(
		yggr::nsql_database_system::c_bson,
		std::allocator<yggr::nsql_database_system::c_bson>) bson_vt;

	bson_vt.push_back(bs);
	bson_vt.push_back(bs);
	bson_vt.push_back(bs);

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err_ins;

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn, db_name, coln_name,
		bson_vt, opts, reply, err_ins);

	std::cout << reply << std::endl;
}

void test_insert_2_3(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::map<int, yggr::nsql_database_system::c_bson> bson_map_type;
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs;
	bs.save("test_insert_2_3", 3);

	bson_map_type bson_map;

	bson_map.insert(bson_map_type::value_type(1, bs));
	bson_map.insert(bson_map_type::value_type(2, bs));
	bson_map.insert(bson_map_type::value_type(3, bs));

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err_ins;

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn, db_name, coln_name,
		bson_map, opts, reply, err_ins);

	std::cout << reply << std::endl;
}

const bson_t* get_bson_ptr(const yggr::nsql_database_system::c_bson& right)
{
	return right.org_pointer();
}

void test_insert_2_4(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs;
	bs.save("test_insert_2_4", 4);

	std::list<yggr::nsql_database_system::c_bson> bson_list;

	bson_list.push_back(bs);
	bson_list.push_back(bs);
	bson_list.push_back(bs);

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err_ins;

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn, db_name, coln_name,
		bson_list,
		boost::bind(&get_bson_ptr, _1),
		opts, reply, err_ins);

	std::cout << reply << std::endl;
}

void test_insert_2_5(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs2;
	bs2.save("test_insert_2_5", 1);

	//bson_error_t err_ins_b = {0};
	bson_t* bs_arr[3] = {&bs2, &bs2, &bs2};

	yggr::nsql_database_system::c_bson opts;

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn,
		db_name.c_str(), coln_name.c_str(),
		&bs_arr[0], 3,
		opts,
		0, 0);
}

// remove
void test_remove_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	{
		yggr::string db_name = "test";
		std::wstring coln_name = L"foo";

		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson bs;
		bson_error_t err_ins = {0};
		yggr::nsql_database_system::mongo_accesser::s_collection_remove(
			conn, db_name.c_str(), coln_name.c_str(),
			MONGOC_REMOVE_SINGLE_REMOVE, &bs, 0, reply, &err_ins);

		std::cout << reply << std::endl;
	}

	{
		yggr::string db_name = "test";
		yggr::string coln_name = "foo";

		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson bs;
		bson_error_t err_ins = {0};
		yggr::nsql_database_system::mongo_accesser::s_collection_delete_one(
			conn, db_name.c_str(), coln_name,
			bs, 0, reply, err_ins);

		std::cout << reply << std::endl;
	}

	{
		yggr::string db_name = "test";
		yggr::string coln_name = "foo";

		yggr::nsql_database_system::c_bson bs;
		//bson_error_t err_ins = {0};
		yggr::nsql_database_system::mongo_accesser::s_collection_remove(
			conn, db_name.c_str(),coln_name,
			MONGOC_REMOVE_NONE, bs, 0, 0, 0);
	}

	{
		yggr::string db_name = "test";
		yggr::string coln_name = "foo";

		yggr::nsql_database_system::c_bson bs;
		//bson_error_t err_ins = {0};
		yggr::nsql_database_system::mongo_accesser::s_collection_delete_many(
			conn, db_name.c_str(),coln_name,
			bs, 0, 0, 0);
	}
}

void test_remove_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	{
		yggr::utf8_string db_name = "test";
		yggr::utf8_string coln_name = "foo";

		yggr::nsql_database_system::c_bson bs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err_ins;

		yggr::nsql_database_system::mongo_accesser::s_collection_remove(
			conn, db_name, coln_name,
			MONGOC_REMOVE_SINGLE_REMOVE, bs, opts, reply, err_ins);

		std::cout << reply << std::endl;
	}

	{
		yggr::utf8_string db_name = "test";
		yggr::utf8_string coln_name = "foo";

		yggr::nsql_database_system::c_bson bs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err_ins;

		yggr::nsql_database_system::mongo_accesser::s_collection_remove(
			conn, db_name, coln_name,
			MONGOC_REMOVE_NONE, bs, opts, reply, &err_ins);

		std::cout << reply << std::endl;
	}
}

struct collector_bson_list
{
	static bool handler_collecting(std::list<yggr::nsql_database_system::c_bson>& list, 
									const yggr::nsql_database_system::c_bson& data)
	{
		list.push_back(data);
		return true;
	}
};

// find
void test_find_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;

	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson filter;
	bson_error_t err_ins = {0};

	//struct collector
	//{
	//	static bool collecting(bson_list_type& list, const yggr::nsql_database_system::c_bson& data)
	//	{
	//		list.push_back(data);
	//		return true;
	//	}
	//};

	{
		bson_list_type bs_list;
		yggr::nsql_database_system::mongo_accesser::s_collection_find(
			conn, db_name.c_str(), coln_name.c_str(),
			&filter, 0, 0,
			&err_ins,
			boost::bind(&collector_bson_list::handler_collecting, boost::ref(bs_list), _1));

		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			char* str = bson_as_json(boost::addressof(*i), 0);
			std::cout << str << std::endl;
			bson_free (str);
		}
	}

	{
		bson_list_type bs_list;
		yggr::nsql_database_system::c_bson opts("{\"projection\" : { \"_id\" : false }, \"limit\" : 1 }");
		yggr::nsql_database_system::mongo_accesser::s_collection_find(
			conn, db_name, coln_name,
			filter, opts, 0,
			err_ins,
			boost::bind(&collector_bson_list::handler_collecting, boost::ref(bs_list), _1));

		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			char* str = bson_as_json(boost::addressof(*i), 0);
			std::cout << str << std::endl;
			bson_free (str);
		}
	}
}

void test_find_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;
	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson filter;
	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_mongo_read_prefs read_prefs;

	yggr::nsql_database_system::c_bson_error err;

	bson_list_type bs_list;

	//struct collector
	//{
	//	static bool collecting(bson_list_type& list, const yggr::nsql_database_system::c_bson& data)
	//	{
	//		list.push_back(data);
	//		return true;
	//	}
	//};

	yggr::nsql_database_system::mongo_accesser::s_collection_find(
		conn, db_name, coln_name,
		filter, opts, read_prefs, err,
		boost::bind(&collector_bson_list::handler_collecting, boost::ref(bs_list), _1));

	for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
	{
		char* str = bson_as_json (boost::addressof(*i), 0);
		std::cout << str << std::endl;
		bson_free (str);
	}
}

// update
void test_update_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";
	yggr::nsql_database_system::c_bson bs1;
	bs1.save("test_update_1_1", 1);
	yggr::nsql_database_system::c_bson bs2;
	bs2.save("test_update_1_1", 11);
	bson_error_t err_ins = {0};
	bson_error_t err_update = {0};

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn, db_name.c_str(), coln_name.c_str(),
		&bs1, 0, 0, &err_ins);

	{
		yggr::nsql_database_system::c_bson reply;

		yggr::nsql_database_system::mongo_accesser::s_collection_update(
			conn, db_name.c_str(), coln_name.c_str(),
			MONGOC_UPDATE_NONE, &bs1, &bs2, 0, reply, &err_update);

		std::cout << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;

		yggr::nsql_database_system::mongo_accesser::s_collection_update(
			conn, db_name.c_str(), coln_name,
			MONGOC_UPDATE_NONE, &bs1, &bs2, 0, reply, 0);

		std::cout << reply << std::endl;
	}
}

void test_update_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs1;
	bs1.save("test_update_1_2", 2);

	yggr::nsql_database_system::c_bson bs2;
	bs2.save("test_update_1_2", 22);

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson replay;
	yggr::nsql_database_system::c_bson_error err_ins;
	yggr::nsql_database_system::c_bson_error err_update;

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn, db_name, coln_name,
		bs1, 0, 0, err_ins);

	yggr::nsql_database_system::mongo_accesser::s_collection_update(
		conn, db_name, coln_name,
		MONGOC_INSERT_NONE, bs1, bs2, opts, replay, err_update);

	std::cout << replay << std::endl;
}

void test_update_1_3(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs_val;
	bs_val.save("test_update_1_3", 3);

	yggr::nsql_database_system::c_bson bs_sel("{ \"test_update_1_3\" : 3 }");
	//yggr::nsql_database_system::c_bson bs_sel("{ \"test_update_1_3\" : { \"$gte\" : 3 } }");
	yggr::nsql_database_system::c_bson bs_up("{ \"$set\" : { \"test_update_1_3\" : 33 } }");

	std::cout << "insert_value: " << bs_val << std::endl;
	std::cout << "selector: "<< bs_sel << std::endl;
	std::cout << "update: " << bs_up << std::endl;

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson replay;
	yggr::nsql_database_system::c_bson_error err_ins;
	yggr::nsql_database_system::c_bson_error err_update;

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn, db_name, coln_name,
		bs_val, 0, 0, err_ins);

	yggr::nsql_database_system::mongo_accesser::s_collection_update(
		conn, db_name, coln_name,
		MONGOC_INSERT_NONE, bs_sel, bs_up, 0, replay, err_update);

	std::cout << replay << std::endl;
}

void test_update_1_4(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs_val;
	bs_val.save("test_update_1_4", 4);

	yggr::nsql_database_system::c_bson bs_sel("{ \"test_update_1_4\" : 4 }");
	yggr::nsql_database_system::c_bson bs_up("{ \"$set\" : { \"test_update_1_4\" : 44 } }");

	std::cout << "insert_value: " << bs_val << std::endl;
	std::cout << "selector: "<< bs_sel << std::endl;
	std::cout << "update: " << bs_up << std::endl;

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson replay;
	yggr::nsql_database_system::c_bson_error err_ins;
	yggr::nsql_database_system::c_bson_error err_update;

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn, db_name, coln_name,
		bs_val, 0, 0, err_ins);

	yggr::nsql_database_system::mongo_accesser::s_collection_insert(
		conn, db_name, coln_name,
		bs_val, 0, 0, err_ins);

	yggr::nsql_database_system::mongo_accesser::s_collection_update(
		conn, db_name, coln_name,
		MONGOC_UPDATE_MULTI_UPDATE, bs_sel, bs_up, 0, replay, err_update);

	std::cout << replay << std::endl;
}

// count
void test_count_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson filter;
	bson_error_t err = {0};

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collection_count(
				conn, db_name.c_str(), coln_name.c_str(),
				&filter, 0, 0, 0, &err) << std::endl;
	}

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collection_count(
				conn, db_name.c_str(), coln_name,
				&filter, 0, 0, 0, 0) << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collection_count(
				conn, db_name.c_str(), coln_name,
				&filter, 0, 0, &reply, &err) << std::endl;

		std::cout << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collection_count(
				conn, db_name.c_str(), coln_name,
				&filter, 0, 0, &reply, 0) << std::endl;

		std::cout << reply << std::endl;
	}
}

void test_count_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson filter;
	yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
	yggr::nsql_database_system::c_bson_error err;

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collection_count(
				conn, db_name, coln_name,
				filter, 0, read_prefs, 0, err) << std::endl;
	}

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collection_count(
				conn, db_name.c_str(), coln_name,
				filter, 0, read_prefs, 0, 0) << std::endl;
	}
}

void test_count_1_3(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson filter;
	yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
	yggr::nsql_database_system::c_bson_error err;

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collection_count(
				conn, db_name, coln_name,
				filter, 0, read_prefs, 0, err) << std::endl;
	}

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collection_count(
				conn, db_name, coln_name,
				filter, 0, read_prefs, 0, err) << std::endl;
	}
}

void test_count_1_4(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson filter("{\"test_insert_1_1\" : {\"$in\" : [0, 1, 2]} }");
	yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
	yggr::nsql_database_system::c_bson_error err;

	std::cout << filter << std::endl;

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collection_count(
				conn, db_name, coln_name,
				filter, 0, read_prefs, 0, err) << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collection_count(
				conn, db_name, coln_name,
				filter, 0, read_prefs, reply, err) << std::endl;

		std::cout << reply << std::endl;
	}
}

// estimated_count
void test_estimated_count_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson filter;
	bson_error_t err = {0};

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collecion_estimated_count(
				conn, db_name.c_str(), coln_name.c_str(),
				0, 0, 0, &err) << std::endl;
	}

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collecion_estimated_count(
				conn, db_name.c_str(), coln_name,
				0, 0, 0, 0) << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collecion_estimated_count(
				conn, db_name.c_str(), coln_name,
				0, 0, &reply, &err) << std::endl;

		std::cout << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collecion_estimated_count(
				conn, db_name.c_str(), coln_name,
				0, 0, &reply, 0) << std::endl;

		std::cout << reply << std::endl;
	}
}

void test_estimated_count_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson filter;
	yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
	yggr::nsql_database_system::c_bson_error err;

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collecion_estimated_count(
				conn, db_name, coln_name,
				0, read_prefs, 0, err) << std::endl;
	}

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collecion_estimated_count(
				conn, db_name.c_str(), coln_name,
				0, read_prefs, 0, 0) << std::endl;
	}
}

void test_estimated_count_1_3(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson filter;
	yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
	yggr::nsql_database_system::c_bson_error err;

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collecion_estimated_count(
				conn, db_name, coln_name,
				0, read_prefs, 0, err) << std::endl;
	}

	{
		std::cout <<
			yggr::nsql_database_system::mongo_accesser::s_collecion_estimated_count(
				conn, db_name, coln_name,
				0, read_prefs, 0, err) << std::endl;
	}
}

// drop

void test_drop_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	bson_error_t err = {0};

	yggr::nsql_database_system::mongo_accesser::s_collection_drop(
		conn, db_name.c_str(), coln_name.c_str(), &err);

	yggr::nsql_database_system::mongo_accesser::s_collection_drop(
		conn, db_name.c_str(), coln_name, 0);

}

void test_drop_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::utf8_string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_drop(
		conn, db_name, coln_name, opts, err);
}

// rename

void test_rename_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";
	yggr::string new_db_name = "test_1_1";
	yggr::string new_coln_name = "foo_1_1";

	bson_error_t err = {0};

	{
		bool bright =
			yggr::nsql_database_system::mongo_accesser::s_collection_rename(
				conn, db_name.c_str(), coln_name.c_str(),
				new_db_name.c_str(), new_coln_name.c_str(),
				true, 0, &err);

		yggr_test_assert(bright);
	}

	{
		bool bright =
			yggr::nsql_database_system::mongo_accesser::s_collection_rename(
				conn, new_db_name.c_str(), new_coln_name.c_str(),
				db_name.c_str(), coln_name.c_str(),
				true, 0, err);

		yggr_test_assert(bright);
	}

	{
		yggr::nsql_database_system::c_bson opts;
		bool bright =
			yggr::nsql_database_system::mongo_accesser::s_collection_rename(
				conn, db_name.c_str(), coln_name.c_str(),
				new_db_name.c_str(), new_coln_name.c_str(),
				true, opts, err);

		yggr_test_assert(bright);
	}
}

void test_rename_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_1_1";
	yggr::utf8_string coln_name = "foo_1_1";
	yggr::utf8_string new_db_name = "test_1_2";
	yggr::string new_coln_name = "foo_1_2";

	yggr::nsql_database_system::c_bson_error err;

	{
		bool bright =
			yggr::nsql_database_system::mongo_accesser::s_collection_rename(
				conn, db_name, coln_name,
				new_db_name, new_coln_name,
				true, err);
		yggr_test_assert(bright);
	}

	{
		bool bright =
			yggr::nsql_database_system::mongo_accesser::s_collection_rename(
				conn, new_db_name, new_coln_name,
				db_name, coln_name,
				true, err);
		yggr_test_assert(bright);
	}

	{
		yggr::nsql_database_system::c_bson opts;
		bool bright =
			yggr::nsql_database_system::mongo_accesser::s_collection_rename(
				conn, db_name, coln_name,
				new_db_name, new_coln_name,
				true, opts, err);
		yggr_test_assert(bright);
	}
}

//save
void test_save_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs;

	bs.save("test_save_1_1", 1);

	bson_error_t err = {0};

	yggr::nsql_database_system::c_bson reply;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name.c_str(), coln_name.c_str(),
		&bs, 0, &reply, &err);

	std::cout << reply << std::endl;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name.c_str(), coln_name,
		&bs, 0, 0, 0);
}

void test_save_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs1;
	yggr::nsql_database_system::c_bson bs2;

	bs1.save("test_save_1_2", 1);
	bs1.save("_id", 1);
	bs2.save("test_save_1_2", 11);
	bs2.save("_id", 1);

	{
		yggr::nsql_database_system::c_bson reply;
		bson_error_t err = {0};

		yggr::nsql_database_system::mongo_accesser::s_collection_save(
			conn, db_name.c_str(), coln_name.c_str(),
			&bs1, 0, reply, &err);

		std::cout << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		bson_error_t err = {0};

		yggr::nsql_database_system::mongo_accesser::s_collection_save(
			conn, db_name.c_str(), coln_name.c_str(),
			&bs2, 0, reply, &err);

		std::cout << reply << std::endl;
	}
}

void test_save_2_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs;
	bs.save("test_save_2_1", 2);

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name,
		bs, opts, reply, err);

	std::cout << reply << std::endl;
}

void test_save_2_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs1;
	yggr::nsql_database_system::c_bson bs2;

	bs1.save("test_save_2_2", 2);
	bs1.save("_id", 2);
	bs2.save("test_save_2_2", 22);
	bs2.save("_id", 2);

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		yggr::nsql_database_system::mongo_accesser::s_collection_save(
			conn, db_name, coln_name, bs1, 0, reply, err);

		std::cout << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		yggr::nsql_database_system::mongo_accesser::s_collection_save(
			conn, db_name, coln_name, bs2, 0, reply, err);

		std::cout << reply << std::endl;
	}
}

// stats
void test_stats_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;

	bson_error_t err = {0};

	yggr::nsql_database_system::mongo_accesser::s_collection_stats(
		conn, db_name.c_str(), coln_name.c_str(),
		&opts, &reply, &err);

	std::cout << reply << std::endl;

	yggr::nsql_database_system::c_bson::org_type reply2 = {0};
	yggr::nsql_database_system::mongo_accesser::s_collection_stats(
		conn, db_name.c_str(), coln_name,
		opts, reply2, 0);

	std::cout << reply2 << std::endl;

	bson_destroy(&reply2);


}

void test_stats_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_stats(
		conn, db_name, coln_name, opts, reply, err);

	std::cout << reply << std::endl;
}

// validate
void test_validate_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;

	bson_error_t err = {0};


	yggr::nsql_database_system::mongo_accesser::s_collection_validate(
		conn, db_name.c_str(), coln_name.c_str(),
		&opts, &reply, &err);

	yggr::nsql_database_system::c_bson::org_type reply2 = {0};
	yggr::nsql_database_system::mongo_accesser::s_collection_validate(
		conn, db_name.c_str(), coln_name,
		opts, reply2, 0);

	bson_destroy(&reply2);
	std::cout << reply << std::endl;
}

void test_validate_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_validate(
		conn, db_name, coln_name, opts, reply, err);

	std::cout << reply << std::endl;
}

// index
// create_index
void test_create_index_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";
	yggr::string index_name = "test_create_index_1_1_idx_name";

	yggr::nsql_database_system::c_bson bs1;
	yggr::nsql_database_system::c_bson bs2;

	bs1.save("test_create_index_1_1", 1);
	bs2.save("test_create_index_1_1", 11);

	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs1, 0, 0, err);

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs2, 0, 0, err);

	yggr::nsql_database_system::c_bson keys;
	keys.save("test_create_index_1_1", 1);

	yggr::nsql_database_system::c_bson opts;
	opts.save("name", index_name);

	yggr::nsql_database_system::c_mongo_index_model model(keys, opts);

	yggr::vector<yggr::nsql_database_system::c_mongo_index_model> models_vt;
	models_vt.push_back(model);

	yggr::nsql_database_system::c_bson_error err_idx;

	bool bsuccess = false;

	bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_collection_create_indexes(
			conn, db_name, coln_name,
			models_vt,
			0,
			0,
			&err_idx);

	yggr_test_assert(bsuccess);

	bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_collection_create_indexes(
			conn, db_name, coln_name,
			models_vt,
			0,
			0,
			0);

	yggr_test_assert(bsuccess);

	bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_collection_create_indexes(
			conn, db_name, coln_name,
			models_vt.begin(), models_vt.end(),
			0,
			0,
			&err_idx);

	yggr_test_assert(bsuccess);

	bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_collection_create_indexes(
			conn, db_name, coln_name,
			models_vt.begin(), models_vt.end(),
			0,
			0,
			0);

	yggr_test_assert(bsuccess);
}

void test_create_index_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";
	yggr::string index_name = "test_create_index_1_2_idx_name";

	yggr::nsql_database_system::c_bson bs1;
	yggr::nsql_database_system::c_bson bs2;

	bs1.save("test_create_index_1_2", 1);
	bs2.save("test_create_index_1_2", 11);

	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs1, 0, 0, err);

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs2, 0, 0, err);

	yggr::nsql_database_system::c_bson keys;
	keys.save("test_create_index_1_2", 1);

	yggr::nsql_database_system::c_bson opts;
	opts.save("name", index_name);

	yggr::nsql_database_system::c_mongo_index_model model(keys, opts);

	yggr::vector<yggr::nsql_database_system::c_mongo_index_model> models_vt;
	models_vt.push_back(model);

	yggr::nsql_database_system::c_bson_error err_idx;

	bool bsuccess = false;

	bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_collection_create_indexes(
			conn, db_name, coln_name,
			models_vt,
			0,
			0,
			err_idx);

	yggr_test_assert(bsuccess);

	bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_collection_create_indexes(
			conn, db_name, coln_name,
			models_vt.begin(), models_vt.end(),
			0,
			0,
			err_idx);

	yggr_test_assert(bsuccess);
}

void test_create_index_1_3(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";
	yggr::string index_name = "test_create_index_1_3_idx_name";

	yggr::nsql_database_system::c_bson bs1;
	yggr::nsql_database_system::c_bson bs2;

	bs1.save("test_create_index_1_3", 1);
	bs2.save("test_create_index_1_3", 11);

	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs1, 0, 0, err);

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs2, 0, 0, err);

	yggr::nsql_database_system::c_bson keys;
	keys.save("test_create_index_1_3", 1);

	yggr::nsql_database_system::c_bson opts;
	opts.save("name", index_name);

	yggr::nsql_database_system::c_mongo_index_model model(keys, opts);
	yggr::nsql_database_system::c_mongo_index_model::org_type* pmodel_org = model;

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err_idx;

		bool bsuccess =
			yggr::nsql_database_system::mongo_accesser::s_collection_create_indexes(
				conn, db_name, coln_name,
				pmodel_org,
				0,
				reply,
				err_idx);

		yggr_test_assert(bsuccess);
		std::cout << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err_idx;

		bool bsuccess =
			yggr::nsql_database_system::mongo_accesser::s_collection_create_indexes(
				conn, db_name, coln_name,
				&model,
				0,
				reply,
				err_idx);

		yggr_test_assert(bsuccess);
		std::cout << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err_idx;

		bool bsuccess =
			yggr::nsql_database_system::mongo_accesser::s_collection_create_indexes(
				conn, db_name, coln_name,
				model,
				0,
				reply,
				err_idx);

		yggr_test_assert(bsuccess);
		std::cout << reply << std::endl;
	}
}

// drop_index
void test_drop_index_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";
	yggr::string index_name = "test_create_index_1_1_idx_name";

	yggr::nsql_database_system::c_bson_error err_idx;

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_drop_index(
				conn, db_name.c_str(), coln_name.c_str(),
				index_name.c_str(), 0, &err_idx);
		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_drop_index(
				conn, db_name.c_str(), coln_name,
				index_name.c_str(), 0, 0);
		yggr_test_assert(!bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_drop_index(
				conn, db_name.c_str(), coln_name,
				index_name.c_str(), 0);
		yggr_test_assert(!bsuccess);
	}
}

void test_drop_index_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";
	yggr::utf8_string index_name = "test_create_index_1_2_idx_name";

	yggr::nsql_database_system::c_bson_error err_idx;

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_drop_index(
				conn, db_name, coln_name,
				index_name, 0, err_idx);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson opts;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_drop_index(
				conn, db_name, coln_name,
				index_name, opts, err_idx);

		yggr_test_assert(!bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson opts;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_drop_index(
				conn, db_name, coln_name,
				index_name, err_idx);

		yggr_test_assert(!bsuccess);
	}
}

//find_indexes
void test_mongoc_collection_find_indexes(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson_error err_idx;

	yggr::nsql_database_system
		::c_mongo_connection
		::mongoc_client_ptr_type pclt = conn.pop();

	mongoc_collection_t* pcoln = mongoc_client_get_collection(pclt.get(), db_name.c_str(), coln_name.c_str());

	//mongoc_cursor_t* pcur = mongoc_collection_find_indexes(pcoln, &err_idx); // discard
	mongoc_cursor_t* pcur = mongoc_collection_find_indexes_with_opts(pcoln, 0);

	const bson_t* pdoc = 0;
	char* str = 0;
	for(;mongoc_cursor_next(pcur, &pdoc);)
	{
		str = bson_as_json (pdoc, NULL);
		std::cout << str << std::endl;
		bson_free (str);
	}

	//bool bsuccess = mongoc_cursor_error(pcur, &err_idx);
	mongoc_cursor_error(pcur, &err_idx);
	mongoc_cursor_destroy(pcur);
	mongoc_collection_destroy(pcoln);
	conn.push(pclt);
}

bool handler_collector(std::list<yggr::nsql_database_system::c_bson>& bs_list, const yggr::nsql_database_system::c_bson& data)
{
	bs_list.push_back(data);
	return true;
}

void test_find_indexes_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson_error err_idx;

	bson_list_type bs_list;

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_indexes(
				conn,
				db_name.c_str(), coln_name.c_str(),
				0, &err_idx,
				boost::bind(&handler_collector, boost::ref(bs_list), _1));

		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_indexes(
				conn,
				db_name.c_str(), coln_name,
				0, 0,
				boost::bind(&handler_collector, boost::ref(bs_list), _1));

		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}

		yggr_test_assert(bsuccess);
	}
}

void test_find_indexes_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	{
		yggr::nsql_database_system::c_bson_error err_idx;

		bson_list_type bs_list;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_indexes(
				conn,
				db_name, coln_name,
				0, err_idx,
				boost::bind(&handler_collector, boost::ref(bs_list), _1));

		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson_error err_idx;

		bson_list_type bs_list;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_indexes(
				conn,
				db_name, coln_name,
				opts, err_idx,
				boost::bind(&handler_collector, boost::ref(bs_list), _1));

		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}

		yggr_test_assert(bsuccess);
	}
}

void test_c_mongo_index_description(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;
	typedef yggr::nsql_database_system::c_mongo_index_description index_description_type;
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	{
		yggr::nsql_database_system::c_bson_error err_idx;

		bson_list_type bs_list;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_indexes(
				conn,
				db_name, coln_name,
				0, err_idx,
				boost::bind(&handler_collector, boost::ref(bs_list), _1));

		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			index_description_type des;
			des = (*i);
			std::cout << des.index_key_name() << ", " << des.index_key_order() << std::endl;
		}

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson_error err_idx;

		bson_list_type bs_list;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_indexes(
				conn,
				db_name, coln_name,
				0, err_idx,
				boost::bind(&handler_collector, boost::ref(bs_list), _1));

		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			index_description_type des;
			des = (*i);
			std::cout << des.index_key_name() << ", " << des.index_key_order() << std::endl;
		}

		yggr_test_assert(bsuccess);
	}
}

// find and modify
void test_find_and_modify_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs1;
	bs1.save("test_find_and_modify_1_1", 1);

	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs1, 0, 0, err);

	yggr::nsql_database_system::c_bson query;

	yggr::nsql_database_system::c_bson update;
	update.save("test_find_and_modify_1_1", 11);



	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err_idx;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_and_modify(
				conn, db_name.c_str(), coln_name.c_str(),
				&query, 0, &update, 0,
				false, false, true,
				&reply, &err_idx);

		std::cout << reply << std::endl;

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_and_modify(
				conn, db_name.c_str(), coln_name.c_str(),
				&query, 0, &update, 0,
				false, false, true,
				0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_and_modify(
				conn, db_name.c_str(), coln_name.c_str(),
				&query, 0, &update, 0,
				false, false, true,
				reply, 0);

		std::cout << reply << std::endl;

		yggr_test_assert(bsuccess);
	}


}

void test_find_and_modify_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs1;
	bs1.save("test_find_and_modify_1_2", 2);

	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs1, 0, 0, err);

	yggr::nsql_database_system::c_bson query;
	yggr::nsql_database_system::c_bson sort;

	yggr::nsql_database_system::c_bson update;
	update.save("test_find_and_modify_1_2", 22);

	yggr::nsql_database_system::c_bson fields;

	yggr::nsql_database_system::c_bson reply;


	yggr::nsql_database_system::c_bson_error err_idx;


	bool bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_collection_find_and_modify(
			conn, db_name, coln_name,
			query, sort, update, fields,
			false, false, true,
			reply, err_idx);

	yggr_test_assert(bsuccess);

	std::cout << reply << std::endl;
}

void test_find_and_modify_2_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs1;
	bs1.save("test_find_and_modify_2_1", 2);

	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs1, 0, 0, err);

	yggr::nsql_database_system::c_bson query;

	yggr::nsql_database_system::c_bson update;
	update.save("test_find_and_modify_2_1", 21);

	yggr::nsql_database_system::c_mongo_find_and_modify_opts opts;

	opts.var_update() = update;

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err_idx;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_and_modify(
				conn, db_name.c_str(), coln_name.c_str(), &query, opts, &reply, &err_idx);

		yggr_test_assert(bsuccess);

		std::cout << reply << std::endl;
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_find_and_modify(
				conn, db_name.c_str(), coln_name, &query, opts, 0, 0);

		yggr_test_assert(bsuccess);
	}


}

void test_find_and_modify_2_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs1;
	bs1.save("test_find_and_modify_2_2", 2);

	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs1, 0, 0, err);

	yggr::nsql_database_system::c_bson query;

	yggr::nsql_database_system::c_bson update;
	update.save("test_find_and_modify_2_2", 22);

	yggr::nsql_database_system::c_mongo_find_and_modify_opts opts;

	opts.var_update() = update;

	yggr::nsql_database_system::c_bson reply;

	yggr::nsql_database_system::c_bson_error err_idx;


	bool bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_collection_find_and_modify(
			conn, db_name, coln_name,
			query, opts, reply, err_idx);

	yggr_test_assert(bsuccess);

	std::cout << reply << std::endl;
}

void test_mongoc_collection_keys_to_index_string(void)
{
	typedef yggr::nsql_database_system::mongo_utility mongo_utility_type;
	yggr::nsql_database_system::c_bson bs;
	bs.save("aaa", 1);
	std::cout << mongo_utility_type::keys_to_index_string(bs) << std::endl;
	std::cout << mongo_utility_type::keys_to_index_string<yggr::string>(bs) << std::endl;
	std::cout << mongo_utility_type::keys_to_index_string<yggr::utf8_string>(bs) << std::endl;

	yggr_test_assert(mongo_utility_type::keys_to_index_string(bs) == "aaa_1");
	yggr_test_assert(mongo_utility_type::keys_to_index_string<yggr::string>(bs) == "aaa_1");
	yggr_test_assert(mongo_utility_type::keys_to_index_string<yggr::utf8_string>(bs) == "aaa_1");
}

// aggregate
void test_collection_aggregate_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;

	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs1("{\"name\" : \"test_aggregate_1_1_u1\"}");

	yggr::nsql_database_system::c_bson bs2;
	bs2.save("name", "test_aggregate_1_1_u1");

	yggr::nsql_database_system::c_bson bs3;
	bs3.save("name", "test_aggregate_1_1_u2");

	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs1, 0, 0, err);

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs2, 0, 0, err);

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs3, 0, 0, err);

	yggr::nsql_database_system::c_bson bs_pipeline("[{\"$group\" : { \"_id\" : \"$name\", \"num_tutorial\" : {\"$sum\" : 1}}}]");
	std::cout << bs_pipeline << std::endl;

	yggr::nsql_database_system::c_bson_error pipe_err;

	bson_list_type bs_list;

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_aggregate(
				conn, db_name.c_str(), coln_name.c_str(), 0, &bs_pipeline, 0, 0, &pipe_err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1));


		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_aggregate(
				conn, db_name.c_str(), coln_name, 0, &bs_pipeline, 0, 0, 0,
				boost::bind(&handler_collector, boost::ref(bs_list), _1));


		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}

		yggr_test_assert(bsuccess);
	}
}

void test_collection_aggregate_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;

	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	yggr::nsql_database_system::c_bson bs1("{\"name\" : \"test_aggregate_1_2_u1\"}");

	yggr::nsql_database_system::c_bson bs2;
	bs2.save("name", "test_aggregate_1_2_u1");

	yggr::nsql_database_system::c_bson bs3;
	bs3.save("name", "test_aggregate_1_2_u2");

	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs1, 0, 0, err);

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs2, 0, 0, err);

	yggr::nsql_database_system::mongo_accesser::s_collection_save(
		conn, db_name, coln_name, bs3, 0, 0, err);

	yggr::nsql_database_system::c_bson bs_pipeline("[{\"$group\" : { \"_id\" : \"$name\", \"num_tutorial\" : {\"$sum\" : 1}}}]");
	std::cout << bs_pipeline << std::endl;

	yggr::nsql_database_system::c_bson options;
	yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
	yggr::nsql_database_system::c_bson_error pipe_err;

	bson_list_type bs_list;

	bool bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_collection_aggregate(
			conn, db_name, coln_name, 0, bs_pipeline, options, read_prefs, pipe_err,
			boost::bind(&handler_collector, boost::ref(bs_list), _1));


	for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	yggr_test_assert(bsuccess);
}

// !!!this function testing new_version s_collection_stats!!!
void test_aggregate_1_3(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;

	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	//yggr::nsql_database_system::c_bson bs_pipeline("[ { \"$collStats\" : { \"latencyStats\" : { \"histograms\" : true } } } ]");
	//yggr::nsql_database_system::c_bson bs_pipeline("[ { \"$collStats\" : { } } ]");
	yggr::nsql_database_system::c_bson bs_pipeline(
		"[ { \"$collStats\": { \"latencyStats\": { \"histograms\" : true }, \"storageStats\" : { }, \"count\" : { }, \"queryExecStats\" : { } } } ]");
	std::cout << bs_pipeline << std::endl;

	yggr::nsql_database_system::c_bson options;
	yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
	yggr::nsql_database_system::c_bson_error pipe_err;

	bson_list_type bs_list;

	bool bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_collection_aggregate(
			conn, db_name, coln_name, 0, bs_pipeline, options, read_prefs, pipe_err,
			boost::bind(&handler_collector, boost::ref(bs_list), _1));


	for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	yggr_test_assert(bsuccess);
}

// bulk_execute

void test_bulk_insert_execute_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	struct bulk_opt_maker
	{
		static bool make(mongoc_bulk_operation_t* pbulk_opt)
		{
			yggr::nsql_database_system::c_bson bs;
			for(yggr::u32 i = 0, isize = 1000; i != isize; ++i)
			{
				yggr_test_assert(pbulk_opt);
				bs.save("test_bulk_insert_execute_1_1", i);
				yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
				bs.clear();
			}

			return true;
		}
	};

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		yggr::u32 used_svr_id
			= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
				conn, db_name.c_str(), coln_name.c_str(), 0,
				boost::bind(&bulk_opt_maker::make, _1),
				&reply, &err);

		yggr_test_assert(used_svr_id);

		std::cout << "bulk_used_server_id = " << used_svr_id << std::endl;
		std::cout << "bulk_reply = " << reply << std::endl;
		std::cout << "bulk_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		yggr::u32 used_svr_id
			= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
				conn, db_name.c_str(), coln_name.c_str(), 0,
				boost::bind(&bulk_opt_maker::make, _1),
				&reply, &err);

		yggr_test_assert(used_svr_id);

		std::cout << "bulk_used_server_id = " << used_svr_id << std::endl;
		std::cout << "bulk_reply = " << reply << std::endl;
		std::cout << "bulk_err = " << err << std::endl;
	}

	{
		yggr::u32 used_svr_id
			= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
				conn, db_name.c_str(), coln_name, 0,
				boost::bind(&bulk_opt_maker::make, _1),
				0, 0);

		yggr_test_assert(used_svr_id);
	}
}

void test_bulk_insert_execute_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	struct bulk_opt_maker
	{
		static bool make(mongoc_bulk_operation_t* pbulk_opt)
		{
			yggr::nsql_database_system::c_bson bs;
			for(yggr::u32 i = 0, isize = 1000; i != isize; ++i)
			{
				yggr_test_assert(pbulk_opt);
				bs.save("test_bulk_insert_execute_1_2", i);
				yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
				bs.clear();
			}

			return true;
		}
	};

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err;

	yggr::u32 used_svr_id
		= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
			conn, db_name, coln_name, opts,
			boost::bind(&bulk_opt_maker::make, _1),
			reply, err);

	yggr_test_assert(used_svr_id);

	std::cout << "bulk_used_server_id = " << used_svr_id << std::endl;
	std::cout << "bulk_reply = " << reply << std::endl;
	std::cout << "bulk_err = " << err << std::endl;
}

void test_bulk_remove_execute_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	struct bulk_opt_maker
	{
		static bool make(mongoc_bulk_operation_t* pbulk_opt)
		{
			yggr::nsql_database_system::c_bson bs;
			for(yggr::u32 i = 0, isize = 1000; i != isize; ++i)
			{
				yggr_test_assert(pbulk_opt);
				bs.save("test_bulk_remove_execute_1_2", i);
				yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
				bs.clear();
			}

			yggr::nsql_database_system::c_bson bs_del("{\"test_bulk_remove_execute_1_2\" : { \"$gte\" : 300 } }");
			yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_remove(pbulk_opt, bs_del);

			return true;
		}
	};

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err;

	yggr::u32 used_svr_id
		= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
			conn, db_name, coln_name, opts,
			boost::bind(&bulk_opt_maker::make, _1),
			reply, err);

	yggr_test_assert(used_svr_id);

	std::cout << "bulk_used_server_id = " << used_svr_id << std::endl;
	std::cout << "bulk_reply = " << reply << std::endl;
	std::cout << "bulk_err = " << err << std::endl;
}

void test_bulk_remove_one_execute_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	struct bulk_opt_maker
	{
		static bool make(mongoc_bulk_operation_t* pbulk_opt)
		{
			yggr::nsql_database_system::c_bson bs;
			for(yggr::u32 i = 0, isize = 1000; i != isize; ++i)
			{
				yggr_test_assert(pbulk_opt);
				bs.save("test_bulk_remove_one_execute_1_2", i);
				yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
				bs.clear();
			}

			yggr::nsql_database_system::c_bson bs_del("{\"test_bulk_remove_one_execute_1_2\" : { \"$gte\" : 300 } }");
			yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_remove_one(pbulk_opt, bs_del);

			return true;
		}
	};

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		yggr::u32 used_svr_id
			= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
				conn, db_name, coln_name, opts,
				boost::bind(&bulk_opt_maker::make, _1),
				reply, err);

		yggr_test_assert(used_svr_id);

		std::cout << "bulk_used_server_id = " << used_svr_id << std::endl;
		std::cout << "bulk_reply = " << reply << std::endl;
		std::cout << "bulk_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;

		yggr::u32 used_svr_id
			= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
				conn, db_name, coln_name, 0,
				boost::bind(&bulk_opt_maker::make, _1),
				reply, 0);

		yggr_test_assert(used_svr_id);

		std::cout << "bulk_used_server_id = " << used_svr_id << std::endl;
		std::cout << "bulk_reply = " << reply << std::endl;
	}
}

void test_bulk_update_execute_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	struct bulk_opt_maker
	{
		static bool make(mongoc_bulk_operation_t* pbulk_opt)
		{
			yggr::nsql_database_system::c_bson bs;
			for(yggr::u32 i = 0, isize = 10; i != isize; ++i)
			{
				yggr_test_assert(pbulk_opt);
				bs.save("test_bulk_update_execute_1_2", i);
				yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
				bs.clear();
			}

			yggr::nsql_database_system::c_bson bs_query("{\"test_bulk_update_execute_1_2\" : { \"$gte\" : 5 } }");
			yggr::nsql_database_system::c_bson bs_doc("{ \"$set\" : {\"test_bulk_update_execute_1_2\" : 2000 } }");
			yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_update(pbulk_opt, bs_query, bs_doc, true);

			return true;
		}
	};

	yggr::nsql_database_system::c_mongo_write_concern wcon;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err;

	yggr::u32 used_svr_id
		= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
			conn, db_name, coln_name, wcon,
			boost::bind(&bulk_opt_maker::make, _1),
			reply, err);

	yggr_test_assert(used_svr_id);

	std::cout << "bulk_used_server_id = " << used_svr_id << std::endl;
	std::cout << "bulk_reply = " << reply << std::endl;
	std::cout << "bulk_err = " << err << std::endl;
}

void test_bulk_update_one_execute_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	struct bulk_opt_maker
	{
		static bool make(mongoc_bulk_operation_t* pbulk_opt)
		{
			yggr::nsql_database_system::c_bson bs;
			for(yggr::u32 i = 0, isize = 10; i != isize; ++i)
			{
				yggr_test_assert(pbulk_opt);
				bs.save("test_bulk_update_one_execute_1_2", i);
				yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
				bs.clear();
			}

			yggr::nsql_database_system::c_bson bs_query("{\"test_bulk_update_one_execute_1_2\" : { \"$gte\" : 5 } }");
			yggr::nsql_database_system::c_bson bs_doc("{ \"$set\" : {\"test_bulk_update_one_execute_1_2\" : 2000 } }");
			yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_update_one(pbulk_opt, bs_query, bs_doc, true);

			return true;
		}
	};

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err;

	yggr::u32 used_svr_id
		= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
			conn, db_name, coln_name, opts,
			boost::bind(&bulk_opt_maker::make, _1),
			reply, err);

	yggr_test_assert(used_svr_id);

	std::cout << "bulk_used_server_id = " << used_svr_id << std::endl;
	std::cout << "bulk_reply = " << reply << std::endl;
	std::cout << "bulk_err = " << err << std::endl;
}

void test_bulk_replace_one_execute_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::utf8_string coln_name = "foo";

	struct bulk_opt_maker
	{
		static bool make(mongoc_bulk_operation_t* pbulk_opt)
		{
			yggr::nsql_database_system::c_bson bs;
			for(yggr::u32 i = 0, isize = 10; i != isize; ++i)
			{
				yggr_test_assert(pbulk_opt);
				bs.save("test_bulk_replace_one_execute_1_2", i);
				yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
				bs.clear();
			}

			yggr::nsql_database_system::c_bson bs_query("{\"test_bulk_replace_one_execute_1_2\" : { \"$gte\" : 5 } }");
			yggr::nsql_database_system::c_bson bs_doc("{\"test_bulk_replace_one_execute_1_2\" : 2000 }");
			yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_replace_one(pbulk_opt, bs_query, bs_doc, true);

			return true;
		}
	};

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson reply;
	yggr::nsql_database_system::c_bson_error err;

	yggr::u32 used_svr_id
		= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
			conn, db_name, coln_name, opts,
			boost::bind(&bulk_opt_maker::make, _1),
			reply, err);

	yggr_test_assert(used_svr_id);

	std::cout << "bulk_used_server_id = " << used_svr_id << std::endl;
	std::cout << "bulk_reply = " << reply << std::endl;
	std::cout << "bulk_err = " << err << std::endl;
}

void test_collection_watch(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;

	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	//yggr::nsql_database_system::c_mongo_uri uri =
	//	yggr::nsql_database_system::mongo_accesser::s_client_get_uri(conn);

	yggr::nsql_database_system::c_mongo_uri uri = conn.uri();

	bool b_is_replic_sets = !uri.get_replica_set().empty();

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_watch(
				conn,
				db_name.c_str(), coln_name.c_str(),
				&pipeline, 0,
				0, 0,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "collection_watch_reply = " << reply << std::endl;
		std::cout << "collection_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_watch(
				conn,
				db_name.c_str(), coln_name.c_str(),
				&pipeline, 0,
				&reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "collection_watch_reply = " << reply << std::endl;
		std::cout << "collection_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_watch(
				conn,
				db_name.c_str(), coln_name.c_str(),
				&pipeline, 0,
				reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "collection_watch_reply = " << reply << std::endl;
		std::cout << "collection_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_watch(
				conn,
				db_name.c_str(), coln_name.c_str(),
				&pipeline, 0,
				&reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "collection_watch_reply = " << reply << std::endl;
		std::cout << "collection_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_collection_watch(
				conn,
				db_name, coln_name,
				pipeline, 0,
				reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "collection_watch_reply = " << reply << std::endl;
		std::cout << "collection_watch_err = " << err << std::endl;
	}

	// s_watch
	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				db_name.c_str(), coln_name.c_str(),
				&pipeline, 0,
				0, 0,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "collection_watch_reply = " << reply << std::endl;
		std::cout << "collection_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				db_name.c_str(), coln_name.c_str(),
				&pipeline, 0,
				&reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "collection_watch_reply = " << reply << std::endl;
		std::cout << "collection_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				db_name.c_str(), coln_name.c_str(),
				&pipeline, 0,
				reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "collection_watch_reply = " << reply << std::endl;
		std::cout << "collection_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				db_name.c_str(), coln_name.c_str(),
				&pipeline, 0,
				&reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "collection_watch_reply = " << reply << std::endl;
		std::cout << "collection_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				db_name, coln_name,
				pipeline, 0,
				reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "collection_watch_reply = " << reply << std::endl;
		std::cout << "collection_watch_err = " << err << std::endl;
	}
}

// run_collection_command_simple
void test_run_client_command_simple_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	{
		struct bulk_opt_maker
		{
			static bool make(mongoc_bulk_operation_t* pbulk_opt)
			{
				yggr::nsql_database_system::c_bson bs;
				for(yggr::u32 i = 0, isize = 10; i != isize; ++i)
				{
					yggr_test_assert(pbulk_opt);
					bs.save("test_run_client_command_simple_1_1", i);
					yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
					bs.clear();
				}

				return true;
			}
		};

		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		yggr::u32 used_svr_id
			= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
				conn, db_name, coln_name, opts,
				boost::bind(&bulk_opt_maker::make, _1),
				reply, err);

		yggr_test_assert(used_svr_id);
	}

	yggr::nsql_database_system::c_bson cmd("{\"count\":\"foo\"}");

	// s_client_command_simple
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command_simple(
					conn, db_name.c_str(), &cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command_simple(
				conn, db_name.c_str(),
				&cmd, 0, &reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command_simple(
					conn, db_name,
					cmd, 0, 0, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command_simple(
					conn, db_name,
					cmd, 0, &reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_reply = " << reply << std::endl;
		std::cout << "test_run_client_command_simple_1_1_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command_simple(
					conn, db_name,
					cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_reply = " << reply << std::endl;
		std::cout << "test_run_client_command_simple_1_1_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command_simple(
					conn, db_name,
					cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_reply = " << reply << std::endl;
		std::cout << "test_run_client_command_simple_1_1_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command_simple(
					conn, db_name,
					cmd, read_prefs, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_reply = " << reply << std::endl;
		std::cout << "test_run_client_command_simple_1_1_err = " << err << std::endl;
	}

	// s_run_command_simple
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command_simple(
					conn, db_name.c_str(),
					&cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command_simple(
				conn, db_name.c_str(),
				&cmd, 0, &reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command_simple(
					conn, db_name,
					cmd, 0, 0, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command_simple(
					conn, db_name,
					cmd, 0, &reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_reply = " << reply << std::endl;
		std::cout << "test_run_client_command_simple_1_1_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command_simple(
					conn, db_name,
					cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_reply = " << reply << std::endl;
		std::cout << "test_run_client_command_simple_1_1_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command_simple(
					conn, db_name,
					cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_reply = " << reply << std::endl;
		std::cout << "test_run_client_command_simple_1_1_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command_simple(
					conn, db_name,
					cmd, read_prefs, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_simple_1_1_reply = " << reply << std::endl;
		std::cout << "test_run_client_command_simple_1_1_err = " << err << std::endl;
	}
}

// run_collection_command_with_opts
void test_run_client_command_with_opts_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	{
		struct bulk_opt_maker
		{
			static bool make(mongoc_bulk_operation_t* pbulk_opt)
			{
				yggr::nsql_database_system::c_bson bs;
				for(yggr::u32 i = 0, isize = 10; i != isize; ++i)
				{
					yggr_test_assert(pbulk_opt);
					bs.save("test_run_client_command_with_opts_1_1", i);
					yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
					bs.clear();
				}

				return true;
			}
		};

		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		yggr::u32 used_svr_id
			= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
				conn, db_name, coln_name, opts,
				boost::bind(&bulk_opt_maker::make, _1),
				reply, err);

		yggr_test_assert(used_svr_id);
	}

	yggr::nsql_database_system::c_bson cmd("{\"count\":\"foo\"}");

	// s_client_command
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name.c_str(),
					&cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	// s_client_command no read_prefs version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name.c_str(),
					&cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	typedef yggr::nsql_database_system::mongoc_command_mode_def cmd_mode_def_type;

	// s_client_command flags version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_RAW, &cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	// s_client_command flags and no read_pref version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_RAW, &cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	// s_run_command
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name.c_str(),
					&cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	// s_run_command no read_prefs veraion
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name.c_str(),
					&cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	// s_run_command flags version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_RAW, &cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	// s_run_command flags and no read_pref version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_RAW, &cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RAW, cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_command_with_opts_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_command_with_opts_1_1 err = " << err << std::endl;
	}
}

// run_collection_read_command
void test_run_client_read_command_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	{
		struct bulk_opt_maker
		{
			static bool make(mongoc_bulk_operation_t* pbulk_opt)
			{
				yggr::nsql_database_system::c_bson bs;
				for(yggr::u32 i = 0, isize = 10; i != isize; ++i)
				{
					yggr_test_assert(pbulk_opt);
					bs.save("test_run_client_read_command_1_1", i);
					yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
					bs.clear();
				}

				return true;
			}
		};

		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		yggr::u32 used_svr_id
			= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
				conn, db_name, coln_name, opts,
				boost::bind(&bulk_opt_maker::make, _1),
				reply, err);

		yggr_test_assert(used_svr_id);
	}

	yggr::nsql_database_system::c_bson cmd("{\"count\":\"foo\"}");

	// s_client_read_command
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_command(
					conn, db_name.c_str(),
					&cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_command(
					conn, db_name,
					cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_command(
					conn, db_name,
					cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_command(
					conn, db_name,
					cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_command(
					conn, db_name,
					cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_command(
					conn, db_name,
					cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	typedef yggr::nsql_database_system::mongoc_command_mode_def cmd_mode_def_type;

	// s_client_command flags version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_READ, &cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	// s_client_command flags and no read_pref version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_READ, &cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	// s_run_command flags version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_READ, &cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	// s_run_command flags and no read_pref version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_READ, &cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_READ, cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_command_1_1 err = " << err << std::endl;
	}
}

// run_collection_read_write_command
void test_run_client_read_write_command_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	{
		struct bulk_opt_maker
		{
			static bool make(mongoc_bulk_operation_t* pbulk_opt)
			{
				yggr::nsql_database_system::c_bson bs;
				for(yggr::u32 i = 0, isize = 10; i != isize; ++i)
				{
					yggr_test_assert(pbulk_opt);
					bs.save("test_run_client_read_write_command_1_1", i);
					yggr::nsql_database_system::mongo_bulk_operation_helper::s_op_insert(pbulk_opt, bs);
					bs.clear();
				}

				return true;
			}
		};

		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		yggr::u32 used_svr_id
			= yggr::nsql_database_system::mongo_accesser::s_collection_bulk_execute(
				conn, db_name, coln_name, opts,
				boost::bind(&bulk_opt_maker::make, _1),
				reply, err);

		yggr_test_assert(used_svr_id);
	}

	yggr::nsql_database_system::c_bson cmd("{\"count\":\"foo\"}");

	// s_client_read_write_command
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name.c_str(),
					&cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name,
					cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name,
					cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name,
					cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name,
					cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name,
					cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	// s_client_read_write_command no read_prefs version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name.c_str(),
					&cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name,
					cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name,
					cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name,
					cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name,
					cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_read_write_command(
					conn, db_name,
					cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	typedef yggr::nsql_database_system::mongoc_command_mode_def cmd_mode_def_type;

	// s_client_command flags version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_RW, &cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	// s_client_command flags and no read_prefs version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_RW, &cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	// s_run_command flags version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_RW, &cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	// s_run_command flags and no read_prefs version
	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name.c_str(),
					cmd_mode_def_type::E_MONGOC_CMD_RW, &cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_RW, cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_read_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_read_write_command_1_1 err = " << err << std::endl;
	}
}

void test_run_client_write_command_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";

	// s_client_read_write_command
	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name.c_str(),
					&cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name,
					cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name,
					cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name,
					cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name,
					cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name,
					cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	// s_client_write_command no read_prefs version
	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name.c_str(),
					&cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name,
					cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name,
					cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name,
					cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name,
					cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_write_command(
					conn, db_name,
					cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	typedef yggr::nsql_database_system::mongoc_command_mode_def cmd_mode_def_type;

	// s_client_command flags version
	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, &cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	// s_client_command flags and no read_prefs version
	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, &cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	// s_run_command flags version
	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, &cmd, 0, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, read_prefs, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	// s_run_command flags and no read_prefs version
	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, &cmd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, reply, 0);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, 0, &reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson cmd("{ \"insert\" : \"foo\", \"documents\" : [ { \"a\" : 1 }, { \"b\" : 2 } ]}");
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}

	// using write command only?
	{
		yggr::nsql_database_system::c_bson cmd("{ \"count\" : \"foo\"}");
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_run_command(
					conn, db_name,
					cmd_mode_def_type::E_MONGOC_CMD_WRITE, cmd, opts, reply, err);

		yggr_test_assert(bsuccess);

		std::cout << "test_run_client_write_command_1_1 reply = " << reply << std::endl;
		std::cout << "test_run_client_write_command_1_1 err = " << err << std::endl;
	}
}

struct handler_check_collection_void
{
    inline void operator()(mongoc_collection_t* pcol, bson_error_t* perr) const
    {
        std::cout << "run void operator()" << std::endl;
        yggr_test_assert(pcol);
        if(perr)
        {
            std::cout << *perr << std::endl;
        }
        else
        {
            std::cout << "arg perr is null" << std::endl;
        }
    }
};


struct handler_check_collection_bool
{
    inline bool operator()(mongoc_collection_t* pcol, bson_error_t* perr) const
    {
        std::cout << "run bool operator()" << std::endl;
        yggr_test_assert(pcol);
        if(perr)
        {
            std::cout << *perr << std::endl;
        }
        else
        {
            std::cout << "arg perr is null" << std::endl;
        }
        return !!pcol;
    }
};

void test_collection_using_handler(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test";
	yggr::string coln_name = "foo";


	{
		handler_check_collection_void void_chk;
		handler_check_collection_bool bool_chk;

		{
			yggr::nsql_database_system::mongo_accesser::s_collection_using_handler(
					conn, db_name.c_str(), coln_name.c_str(), 0, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_collection_using_handler(
						conn, db_name.c_str(), coln_name.c_str(), 0, bool_chk);

			yggr_test_assert(bsuccess);
		}

		{
			yggr::nsql_database_system::mongo_accesser::s_using_handler(
					conn, db_name.c_str(), coln_name.c_str(), 0, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_using_handler(
						conn, db_name.c_str(), coln_name.c_str(), 0, bool_chk);

			yggr_test_assert(bsuccess);
		}
	}

	{
		handler_check_collection_void void_chk;
		handler_check_collection_bool bool_chk;
		yggr::nsql_database_system::c_bson_error err;

		{
			yggr::nsql_database_system::mongo_accesser::s_collection_using_handler(
					conn, db_name, coln_name.c_str(), &err, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_collection_using_handler(
						conn, db_name, coln_name, &err, bool_chk);

			yggr_test_assert(bsuccess);
		}

		{
			yggr::nsql_database_system::mongo_accesser::s_using_handler(
					conn, db_name, coln_name.c_str(), &err, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_using_handler(
						conn, db_name, coln_name, &err, bool_chk);

			yggr_test_assert(bsuccess);
		}
	}

	{
		handler_check_collection_void void_chk;
		handler_check_collection_bool bool_chk;
		yggr::nsql_database_system::c_bson_error err;

		{
			yggr::nsql_database_system::mongo_accesser::s_collection_using_handler(
					conn, db_name, coln_name, err, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_collection_using_handler(
						conn, db_name, coln_name, err, bool_chk);

			yggr_test_assert(bsuccess);
		}

		{
			yggr::nsql_database_system::mongo_accesser::s_using_handler(
					conn, db_name, coln_name, err, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_using_handler(
						conn, db_name, coln_name, err, bool_chk);

			yggr_test_assert(bsuccess);
		}
	}
}

void test_collections(void)
{

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://xy:123456abc@127.0.0.1:10398/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://127.0.0.1:10298/?tls=true",	// str_uri
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
				"mongodb://xy:123456abc@127.0.0.1:10198/?tls=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://127.0.0.1:10098/?tls=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	yggr::nsql_database_system::c_mongo_connection client(client_init);

#if MONGODB_USING_CA()
#endif // MONGODB_USING_CA

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		client.clear();
		return;
	}

	{
		// test clear all
		std::cout << "--------test_clear_all S-----------" << std::endl;
		test_remove_1_1(client);
		test_remove_1_2(client);
		test_drop_1_1(client);
		test_drop_1_2(client);
		std::cout << "--------test_clear_all E-----------" << std::endl;

		// test insert
		std::cout << "--------test_insert S-----------" << std::endl;
		test_insert_1_1(client); // 4
		test_insert_1_2(client); // 1
		test_insert_1_3(client); // 1

		// test insert many
		test_insert_2_1(client); // 3
		test_insert_2_2(client); // 3
		test_insert_2_3(client); // 3
		test_insert_2_4(client); // 3
		test_insert_2_5(client); // 3

		std::cout << "--------test_insert E-----------" << std::endl;

		// test update
		std::cout << "--------test_update S-----------" << std::endl;
		test_update_1_1(client); // 1
		test_update_1_2(client); // 1
		test_update_1_3(client); // 1
		test_update_1_4(client); // 2
		std::cout << "--------test_update E-----------" << std::endl;

		// test save
		std::cout << "--------test_save S-----------" << std::endl;
		test_save_1_1(client);
		test_save_1_2(client);

		test_save_2_1(client);
		test_save_2_2(client);
		std::cout << "--------test_save E-----------" << std::endl;

		// test find
		std::cout << "--------test_find S-----------" << std::endl;
		test_find_1_1(client);
		std::cout << "-------------------" << std::endl;
		test_find_1_2(client);
		std::cout << "--------test_find E-----------" << std::endl;

		// test count
		std::cout << "--------test_count S-----------" << std::endl;
		test_count_1_1(client);
		test_count_1_2(client);
		test_count_1_3(client);
		test_count_1_4(client);
		std::cout << "--------test_count E-----------" << std::endl;

		// test estimated_count
		std::cout << "--------test_estimated_count S-----------" << std::endl;
		test_estimated_count_1_1(client);
		test_estimated_count_1_2(client);
		test_estimated_count_1_3(client);
		std::cout << "--------test_estimated_count E-----------" << std::endl;

		// test stats
		std::cout << "--------test_stats S-----------" << std::endl;
		test_stats_1_1(client);
		test_stats_1_2(client);
		std::cout << "--------test_stats E-----------" << std::endl;

		// test validate
		std::cout << "--------test_validate S-----------" << std::endl;
		test_validate_1_1(client);
		test_validate_1_2(client);
		std::cout << "--------test_validate E-----------" << std::endl;

		// test rename
		std::cout << "--------test_rename S-----------" << std::endl;
		test_rename_1_1(client);
		test_rename_1_2(client);
		std::cout << "--------test_rename E-----------" << std::endl;

		// test create_index and drop_index
		std::cout << "--------test_create_index_and_drop_index S-----------" << std::endl;
		test_drop_1_2(client);
		test_create_index_1_1(client);
		test_drop_index_1_1(client);

		test_drop_1_2(client);
		test_create_index_1_3(client);

		test_drop_1_2(client);
		test_create_index_1_2(client);
		std::cout << "--------test_create_index_and_drop_index E-----------" << std::endl;

		std::cout << "--------test_mongoc_collection_find_indexes1 S-----------" << std::endl;
		test_mongoc_collection_find_indexes(client);
		std::cout << "-------------------" << std::endl;
		test_find_indexes_1_1(client);
		std::cout << "-------------------" << std::endl;
		test_find_indexes_1_2(client);
		std::cout << "-------------------" << std::endl;
		test_c_mongo_index_description(client);
		std::cout << "--------test_mongoc_collection_find_indexes1 E-----------" << std::endl;

		test_drop_index_1_2(client);

		std::cout << "--------test_mongoc_collection_find_indexes2 S-----------" << std::endl;
		test_mongoc_collection_find_indexes(client);
		std::cout << "-------------------" << std::endl;
		test_find_indexes_1_1(client);
		std::cout << "-------------------" << std::endl;
		test_find_indexes_1_2(client);
		std::cout << "-------------------" << std::endl;
		test_c_mongo_index_description(client);
		std::cout << "--------test_mongoc_collection_find_indexes2 E-----------" << std::endl;

		std::cout << "--------test_find_and_modify S-----------" << std::endl;
		test_drop_1_2(client);
		test_find_and_modify_1_1(client);

		test_drop_1_2(client);
		test_find_and_modify_1_2(client);

		test_drop_1_2(client);
		test_find_and_modify_2_1(client);

		test_drop_1_2(client);
		test_find_and_modify_2_2(client);
		std::cout << "--------test_find_and_modify E-----------" << std::endl;

		std::cout << "--------test_aggregate S-----------" << std::endl;
		test_drop_1_2(client);
		test_collection_aggregate_1_1(client);

		test_drop_1_2(client);
		test_collection_aggregate_1_2(client);
		test_aggregate_1_3(client);
		std::cout << "--------test_aggregate E-----------" << std::endl;

		std::cout << "--------test_bulk_execute S-----------" << std::endl;
		test_drop_1_2(client);
		test_bulk_insert_execute_1_1(client);

		test_drop_1_2(client);
		test_bulk_insert_execute_1_2(client);

		test_drop_1_2(client);
		test_bulk_remove_execute_1_2(client);

		test_drop_1_2(client);
		test_bulk_remove_one_execute_1_2(client);

		test_drop_1_2(client);
		test_bulk_update_execute_1_2(client);

		test_drop_1_2(client);
		test_bulk_update_one_execute_1_2(client);

		test_drop_1_2(client);
		test_bulk_replace_one_execute_1_2(client);

		std::cout << "--------test_bulk_execute E-----------" << std::endl;

		std::cout << "--------s_collection_watch S-----------" << std::endl;

		test_collection_watch(client);

		std::cout << "--------s_collection_watch E-----------" << std::endl;

		std::cout << "--------test_run_client_command_simple S-----------" << std::endl;

		test_drop_1_2(client);
		test_run_client_command_simple_1_1(client);

		std::cout << "--------test_run_client_command_simple E-----------" << std::endl;

		std::cout << "--------test_run_client_command_with_opts S-----------" << std::endl;

		test_drop_1_2(client);
		test_run_client_command_with_opts_1_1(client);

		std::cout << "--------test_run_client_command_with_opts E-----------" << std::endl;

		std::cout << "--------test_run_client_read_command S-----------" << std::endl;

		test_drop_1_2(client);
		test_run_client_read_command_1_1(client);

		std::cout << "--------test_run_client_read_command E-----------" << std::endl;

		std::cout << "--------test_run_client_read_write_command S-----------" << std::endl;

		test_drop_1_2(client);
		test_run_client_read_write_command_1_1(client);

		std::cout << "--------test_run_client_read_write_command E-----------" << std::endl;

		std::cout << "--------test_run_client_write_command S-----------" << std::endl;

		test_drop_1_2(client);
		test_run_client_write_command_1_1(client);

		std::cout << "--------test_run_client_write_command E-----------" << std::endl;

		std::cout << "--------test_collection_using_handler S-----------" << std::endl;

		test_collection_using_handler(client);

		std::cout << "--------test_collection_using_handler E-----------" << std::endl;

	}

	test_mongoc_collection_keys_to_index_string();

	client.clear();

	std::cout << "---------test_collections end--------------" << std::endl;
}

//-------------------database test---------------------------------------

//test database_aggregate
void test_database_aggregate(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;

	yggr::string db_name = "admin";



	yggr::nsql_database_system::c_bson bs_pipeline("{ \"pipeline\" : [ { \"$currentOp\" : {  } } ] }");
	std::cout << bs_pipeline << std::endl;

	{
		yggr::nsql_database_system::c_bson_error pipe_err;
		bson_list_type bs_list;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_aggregate(
				conn, db_name.c_str(), &bs_pipeline, 0, 0, &pipe_err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1));


		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson_error pipe_err;
		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_aggregate(
				conn, db_name, bs_pipeline, 0, 0, pipe_err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1));


		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}

		yggr_test_assert(bsuccess);
	}
}

//test create_collection
void test_database_create_collecion_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_db";
	yggr::string coln_name = "test_database_create_collecion_1_1";



	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_create_collecion(
				conn, db_name.c_str(), coln_name.c_str(), 0, &err);

		if(!bsuccess)
		{
			std::cout << err << std::endl;
		}

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_create_collecion(
				conn, db_name.c_str(), coln_name.c_str(), 0, 0);

		yggr_test_assert(bsuccess);
	}
}

void test_database_create_collecion_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_db";
	yggr::utf8_string coln_name = "test_database_create_collecion_1_2";

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson_error err;


	bool bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_database_create_collecion(
			conn, db_name, coln_name, opts, err);

	if(!bsuccess)
	{
		std::cout << err << std::endl;
	}
}

// test find_collections

void test_database_find_collections_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;
	bson_list_type bs_list;
	yggr::string db_name = "test_db";
	yggr::nsql_database_system::c_bson_error err;

	//struct collector
	//{
	//	static bool handler_collector(std::list<yggr::nsql_database_system::c_bson>& bs_list,
	//										const yggr::nsql_database_system::c_bson& bs)
	//	{
	//		bs_list.push_back(bs);
	//		return true;
	//	}
	//};

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_find_collections(
				conn, db_name.c_str(),
				0, &err,
				boost::bind(&collector_bson_list::handler_collecting, boost::ref(bs_list), _1));
		yggr_test_assert(bsuccess);

		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << (*i) << std::endl;
		}
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_find_collections(
				conn, db_name,
				0, 0,
				boost::bind(&collector_bson_list::handler_collecting, boost::ref(bs_list), _1));
		yggr_test_assert(bsuccess);

		for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
		{
			std::cout << (*i) << std::endl;
		}
	}
}

void test_database_find_collections_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;
	bson_list_type bs_list;
	yggr::utf8_string db_name = "test_db";

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson_error err;

	//struct collector
	//{
	//	static bool handler_collector(std::list<yggr::nsql_database_system::c_bson>& bs_list,
	//										const yggr::nsql_database_system::c_bson& bs)
	//	{
	//		bs_list.push_back(bs);
	//		return true;
	//	}
	//};

	bool bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_database_find_collections(
			conn, db_name,
			opts, err,
			boost::bind(&collector_bson_list::handler_collecting, boost::ref(bs_list), _1));
	yggr_test_assert(bsuccess);

	for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end(); i != isize; ++i)
	{
		std::cout << (*i) << std::endl;
	}
}

// has_collections
void test_database_has_collection_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;
	bson_list_type bs_list;
	yggr::string db_name = "test_db";
	yggr::string coln_name = "test_database_has_collection_1_1";

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_database_create_collecion(
		conn, db_name, coln_name, opts, err);

	{
		yggr::nsql_database_system::c_bson_error err_has;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_has_collection(
				conn, db_name.c_str(), coln_name.c_str(), &err_has);

		std::cout << "test_database_has_collection_1_1 result: " << err_has << std::endl;

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson_error err_has;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_has_collection(
				conn, db_name.c_str(), "not_exists_collection", &err_has);

		std::cout << "test_database_has_collection_1_1 result: " << err_has << std::endl;

		yggr_test_assert(!bsuccess);
	}
}

void test_database_has_collection_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;
	bson_list_type bs_list;
	yggr::string db_name = "test_db";
	yggr::utf8_string coln_name = "test_database_has_collection_1_2";

	yggr::nsql_database_system::c_bson opts;
	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::mongo_accesser::s_database_create_collecion(
		conn, db_name, coln_name, opts, err);

	yggr::nsql_database_system::c_bson_error err_has;

	bool bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_database_has_collection(
			conn, db_name, coln_name, err);

	std::cout << "test_database_has_collection_1_2 result: " << err << std::endl;
	yggr_test_assert(bsuccess);
}

struct collector_yggr_string
{
	static void handler_collector(std::list<yggr::string>& str_list, const char* name)
	{
		yggr::utf8_string str_name(name, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		str_list.push_back(str_name.str<yggr::string>());
	}
};
//get_collection_names
void test_database_get_collection_name_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::string> string_list_type;
	string_list_type str_list;
	yggr::string db_name = "test_db";
	yggr::nsql_database_system::c_bson_error err;

	//struct collector
	//{
	//	static void handler_collector(std::list<yggr::string>& str_list, const char* name)
	//	{
	//		yggr::utf8_string str_name(name, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//		str_list.push_back(str_name.str<yggr::string>());
	//	}
	//};

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_get_collection_names(
				conn, db_name.c_str(), 0, &err,
				boost::bind(&collector_yggr_string::handler_collector, boost::ref(str_list), _1));

		yggr_test_assert(bsuccess);

		for(string_list_type::const_iterator i = str_list.begin(), isize = str_list.end(); i != isize; ++i)
		{
			std::cout << (*i) << std::endl;
		}
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_get_collection_names(
				conn, db_name, 0, 0,
				boost::bind(&collector_yggr_string::handler_collector, boost::ref(str_list), _1));

		yggr_test_assert(bsuccess);

		for(string_list_type::const_iterator i = str_list.begin(), isize = str_list.end(); i != isize; ++i)
		{
			std::cout << (*i) << std::endl;
		}
	}
}

void test_database_get_collection_name_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::string> string_list_type;
	string_list_type str_list;
	yggr::utf8_string db_name = "test_db";
	yggr::nsql_database_system::c_bson_error err;

	//struct collector
	//{
	//	static void handler_collector(std::list<yggr::string>& str_list, const char* name)
	//	{
	//		yggr::utf8_string str_name(name, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//		str_list.push_back(str_name.str<yggr::string>());
	//	}
	//};

	yggr::nsql_database_system::c_bson opts;
	bool bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_database_get_collection_names(
			conn, db_name, opts, err,
			boost::bind(&collector_yggr_string::handler_collector, boost::ref(str_list), _1));
	yggr_test_assert(bsuccess);

	for(string_list_type::const_iterator i = str_list.begin(), isize = str_list.end(); i != isize; ++i)
	{
		std::cout << (*i) << std::endl;
	}
}

// drop_db
void test_database_drop_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_db";

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_drop(
				conn, db_name.c_str(), &err);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_drop(
				conn, db_name, 0);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_drop(
				conn, db_name.c_str(), opts, &err);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_drop(
				conn, db_name, opts, 0);

		yggr_test_assert(bsuccess);
	}
}

void test_database_drop_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::utf8_string db_name = "test_db";

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_drop(conn, db_name, err);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_drop(conn, db_name, err);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_drop(
				conn, db_name, opts, err);

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_drop(
				conn, db_name, opts, err);

		yggr_test_assert(bsuccess);
	}
}


// add_user
void test_database_add_user_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_db";
	yggr::string uname = "test_database_add_user_1_1";
	yggr::string pwd = "test_database_add_user_1_1";

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_add_user(
				conn, db_name.c_str(), uname.c_str(), pwd.c_str(), 0, 0, &err);

		if(!bsuccess)
		{
			std::cout << err << std::endl;
		}
		else
		{
			std::cout << "------test_database_add_user_1_1 success------" << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_add_user(
				conn, db_name.c_str(), uname.c_str(), pwd, 0, 0, 0);

		if(!bsuccess)
		{
			std::cout << "s_database_add_user failed" << std::endl;
		}
		else
		{
			std::cout << "------test_database_add_user_1_1 success------" << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_add_user(
				conn, db_name.c_str(), uname.c_str(), pwd.c_str(), 0, 0, err);

		if(!bsuccess)
		{
			std::cout << err << std::endl;
		}
		else
		{
			std::cout << "------test_database_add_user_1_1 success------" << std::endl;
		}
	}
}

void test_database_add_user_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_db";
	yggr::utf8_string uname = "test_database_add_user_1_2";
	yggr::utf8_string pwd = "test_database_add_user_1_2";

	yggr::nsql_database_system::c_bson roles;
	yggr::nsql_database_system::c_bson custom_data;
	yggr::nsql_database_system::c_bson_error err;

	bool bsuccess
		= yggr::nsql_database_system::mongo_accesser::s_database_add_user(
			conn, db_name, uname, pwd, roles, custom_data, err);

	if(!bsuccess)
	{
		std::cout << err << std::endl;
	}
	else
	{
		std::cout << "------test_database_add_user_1_2 success------" << std::endl;
	}
}

// remove_user
void test_database_remove_user_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_db";
	yggr::string uname = "test_database_remove_user_1_1";
	yggr::string pwd = "test_database_remove_user_1_1";

	yggr::nsql_database_system::c_bson_error err;
	yggr::nsql_database_system::c_bson_error rm_err;

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_add_user(
				conn, db_name.c_str(), uname.c_str(), pwd.c_str(), 0, 0, &err);

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_remove_user(
				conn, db_name.c_str(), uname.c_str(), &err);

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_add_user(
				conn, db_name.c_str(), uname.c_str(), pwd, 0, 0, 0);

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_remove_user(
				conn, db_name.c_str(), uname, 0);

		yggr_test_assert(bsuccess);
	}
}

void test_database_remove_user_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_db";
	yggr::utf8_string uname = "test_database_remove_user_1_2";
	yggr::utf8_string pwd = "test_database_remove_user_1_2";

	yggr::nsql_database_system::c_bson roles;
	yggr::nsql_database_system::c_bson custom_data;

	yggr::nsql_database_system::c_bson_error err;
	yggr::nsql_database_system::c_bson_error rm_err;

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_add_user(
				conn, db_name, uname, pwd, roles, custom_data, err);

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_remove_user(
				conn, db_name, uname, err);

		yggr_test_assert(bsuccess);
	}
}

// remove_all_users
void test_database_remove_all_users_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_db";
	yggr::string uname = "test_database_remove_all_users_1_1";
	yggr::string pwd = "test_database_remove_all_users_1_1";

	yggr::nsql_database_system::c_bson_error err;
	yggr::nsql_database_system::c_bson_error rm_err;

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_add_user(
				conn, db_name.c_str(), uname.c_str(), pwd.c_str(), 0, 0, &err);

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_remove_all_users(
				conn, db_name.c_str(), &err);

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_remove_all_users(
				conn, db_name, 0);

		yggr_test_assert(bsuccess);
	}
}

void test_database_remove_all_users_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_db";
	yggr::utf8_string uname = "test_database_remove_all_users_1_2";
	yggr::utf8_string pwd = "test_database_remove_all_users_1_2";

	yggr::nsql_database_system::c_bson roles;
	yggr::nsql_database_system::c_bson custom_data;

	yggr::nsql_database_system::c_bson_error err;
	yggr::nsql_database_system::c_bson_error rm_err;

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_add_user(
				conn, db_name, uname, pwd, roles, custom_data, err);

		yggr_test_assert(bsuccess);
	}

	{
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_remove_all_users(
				conn, db_name, err);

		yggr_test_assert(bsuccess);
	}
}

void test_database_watch(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;

	yggr::string db_name = "test";

	//yggr::nsql_database_system::c_mongo_uri uri =
	//	yggr::nsql_database_system::mongo_accesser::s_client_get_uri(conn);

	yggr::nsql_database_system::c_mongo_uri uri = conn.uri();

	bool b_is_replic_sets = !uri.get_replica_set().empty();

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_watch(
				conn,
				db_name.c_str(),
				&pipeline, 0,
				0, 0,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_watch(
				conn,
				db_name.c_str(),
				&pipeline, 0,
				&reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_watch(
				conn,
				db_name.c_str(),
				&pipeline, 0,
				reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_watch(
				conn,
				db_name.c_str(),
				&pipeline, 0,
				&reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_database_watch(
				conn,
				db_name,
				pipeline, 0,
				reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	// s_watch
	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				db_name.c_str(),
				&pipeline, 0,
				0, 0,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				db_name.c_str(),
				&pipeline, 0,
				&reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				db_name.c_str(),
				&pipeline, 0,
				reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				db_name.c_str(),
				&pipeline, 0,
				&reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				db_name,
				pipeline, 0,
				reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

}

struct handler_check_database_void
{
	inline void operator()(mongoc_database_t* pdb, bson_error_t* perr) const
	{
		std::cout << "run void operator()" << std::endl;
		yggr_test_assert(pdb);
		if(perr)
		{
			std::cout << *perr << std::endl;
		}
		else
		{
			std::cout << "arg perr is null" << std::endl;
		}
	}
};

struct handler_check_database_bool
{
	inline bool operator()(mongoc_database_t* pdb, bson_error_t* perr) const
	{
		std::cout << "run bool operator()" << std::endl;
		yggr_test_assert(pdb);
		if(perr)
		{
			std::cout << *perr << std::endl;
		}
		else
		{
			std::cout << "arg perr is null" << std::endl;
		}
		return !!pdb;
	}
};

void test_database_using_handler(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name = "test_db";

	{
		handler_check_database_void void_chk;
		handler_check_database_bool bool_chk;

		{
			yggr::nsql_database_system::mongo_accesser::s_database_using_handler(
					conn, db_name.c_str(), 0, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_database_using_handler(
						conn, db_name.c_str(), 0, bool_chk);

			yggr_test_assert(bsuccess);
		}

		{
			yggr::nsql_database_system::mongo_accesser::s_using_handler(
					conn, db_name.c_str(), 0, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_using_handler(
						conn, db_name.c_str(), 0, bool_chk);

			yggr_test_assert(bsuccess);
		}
	}

	{
		handler_check_database_void void_chk;
		handler_check_database_bool bool_chk;
		yggr::nsql_database_system::c_bson_error err;

		{
			yggr::nsql_database_system::mongo_accesser::s_database_using_handler(
					conn, db_name, &err, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_database_using_handler(
						conn, db_name, &err, bool_chk);

			yggr_test_assert(bsuccess);
		}

		{
			yggr::nsql_database_system::mongo_accesser::s_using_handler(
					conn, db_name, &err, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_using_handler(
						conn, db_name, &err, bool_chk);

			yggr_test_assert(bsuccess);
		}
	}

	{
		handler_check_database_void void_chk;
		handler_check_database_bool bool_chk;
		yggr::nsql_database_system::c_bson_error err;

		{
			yggr::nsql_database_system::mongo_accesser::s_database_using_handler(
					conn, db_name, err, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_database_using_handler(
						conn, db_name, err, bool_chk);

			yggr_test_assert(bsuccess);
		}

		{
			yggr::nsql_database_system::mongo_accesser::s_using_handler(
					conn, db_name, err, void_chk);

			bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_using_handler(
						conn, db_name, err, bool_chk);

			yggr_test_assert(bsuccess);
		}
	}
}

void test_database(void)
{

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://xy:123456abc@127.0.0.1:10398/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://127.0.0.1:10298/?tls=true",	// str_uri
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
				"mongodb://xy:123456abc@127.0.0.1:10198/?tls=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://127.0.0.1:10098/?tls=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	yggr::nsql_database_system::c_mongo_connection client(client_init);

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		client.clear();
		return;
	}

	{
		//test_drop_db
		std::cout << "--------test_drop_db s-----------" << std::endl;
		test_database_drop_1_1(client);
		test_database_drop_1_2(client);
		std::cout << "--------test_drop_db e-----------" << std::endl;

		// test_database_aggregate
		std::cout << "--------test_database_aggregate s-----------" << std::endl;
		test_database_aggregate(client);
		std::cout << "--------test_database_aggregate e-----------" << std::endl;

		// test_create_collections
		std::cout << "--------test_create_collection s-----------" << std::endl;
		test_database_create_collecion_1_1(client);
		test_database_create_collecion_1_2(client);
		std::cout << "--------test_create_collection e-----------" << std::endl;

		// test_find_collections
		std::cout << "--------test_find_collections s-----------" << std::endl;
		test_database_find_collections_1_1(client);
		test_database_find_collections_1_2(client);
		std::cout << "--------test_find_collections e-----------" << std::endl;

		// test_has_collection
		std::cout << "--------test_has_collection s-----------" << std::endl;
		test_database_has_collection_1_1(client);
		test_database_has_collection_1_2(client);
		std::cout << "--------test_has_collection e-----------" << std::endl;

		//test_get_collection_name
		std::cout << "--------test_get_collection_name s-----------" << std::endl;
		test_database_get_collection_name_1_1(client);
		test_database_get_collection_name_1_2(client);
		std::cout << "--------test_get_collection_name e-----------" << std::endl;


#if !(MONGODB_USING_AUTH())
		std::cout << "--------test_add_user s-----------" << std::endl;
		test_database_add_user_1_1(client);
		test_database_add_user_1_2(client);
		std::cout << "--------test_add_user e-----------" << std::endl;

		std::cout << "--------test_remove_user s-----------" << std::endl;
		test_database_remove_user_1_1(client);
		test_database_remove_user_1_2(client);
		std::cout << "--------test_remove_user e-----------" << std::endl;

		std::cout << "--------test_remove_all_user s-----------" << std::endl;
		test_database_remove_all_users_1_1(client);
		test_database_remove_all_users_1_2(client);
		std::cout << "--------test_remove_all_user e-----------" << std::endl;
#endif // MONGODB_USING_AUTH
	}

	std::cout << "--------test_database_watch s-----------" << std::endl;

	test_database_watch(client);

	std::cout << "--------test_database_watch e-----------" << std::endl;

	std::cout << "--------test_database_using_handler s-----------" << std::endl;

	test_database_using_handler(client);

	std::cout << "--------test_database_using_handler e-----------" << std::endl;

	client.clear();

	std::cout << "---------test_database end--------------" << std::endl;
}

// gridfs test
void test_gridfs_save_file_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fpath("./nsql_database_system/gridfs_test_file.txt");
	yggr::string fname("gridfs_test_file_1_1.txt");

	yggr::nsql_database_system::c_bson_error err;

	{
		mongoc_gridfs_file_opt_t opt = { 0 };
		opt.filename = fname.c_str();

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_save_file(conn, db_name.c_str(), gridfs_name.c_str(),
										fpath.c_str(), 0, 0, &opt, &err);

		yggr_test_assert(bsuccess);
	}

	{
		mongoc_gridfs_file_opt_t opt = { 0 };
		opt.filename = fname.c_str();

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_save_file(conn, db_name.c_str(), gridfs_name.c_str(),
										fpath, 0, 0, &opt, 0);

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_save_file_1_1 success------" << std::endl;
}

void test_gridfs_save_file_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::utf8_string gridfs_name("fs_test");
	yggr::string fpath("./nsql_database_system/gridfs_test_file.txt");
	yggr::string fname("gridfs_test_file_1_2.txt");

	yggr::nsql_database_system::c_bson_error err;

	yggr::nsql_database_system::c_mongo_gridfs_file_opt opt(fname);

	bool bsuccess
		= yggr::nsql_database_system
			::mongo_accesser
			::s_gridfs_save_file(conn, db_name, gridfs_name,
									fpath, 0, 0, opt, err);

	yggr_test_assert(bsuccess);

	std::cout << "------test_gridfs_save_file_1_2 success------" << std::endl;
}

//gridfs_load_file

void test_gridfs_load_file_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fpath("./nsql_database_system/out_gridfs_test_file_1_1.txt");
	yggr::string fname("gridfs_test_file_1_1.txt");

	yggr::nsql_database_system::c_bson_error err;

	{
		std::vector<int> buf;

		yggr::u64 read_byte_size
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_load_file(conn, db_name.c_str(), gridfs_name.c_str(),
										fname.c_str(), 0, buf, &err);

		yggr_test_assert(read_byte_size);
		yggr_test_assert(buf.size() * sizeof(int) >= read_byte_size);

		std::ofstream ofs;
		ofs.open(fpath.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(reinterpret_cast<char*>(&(*boost::begin(buf))), read_byte_size);
		ofs.flush();
		ofs.close();
	}

	{
		std::vector<int> buf;

		yggr::u64 read_byte_size
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_load_file(conn, db_name.c_str(), gridfs_name,
										fname.c_str(), 0, buf, 0);

		yggr_test_assert(read_byte_size);
		yggr_test_assert(buf.size() * sizeof(int) >= read_byte_size);

		std::ofstream ofs;
		ofs.open(fpath.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(reinterpret_cast<char*>(&(*boost::begin(buf))), read_byte_size);
		ofs.flush();
		ofs.close();
	}

	std::cout << "------test_gridfs_load_file_1_1 success------" << std::endl;
}

void test_gridfs_load_file_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::utf8_string gridfs_name("fs_test");
	yggr::string fpath("./nsql_database_system/out_gridfs_test_file_1_2.txt");
	yggr::string fname("gridfs_test_file_1_2.txt");

	yggr::nsql_database_system::c_bson_error err;

	std::vector<int> buf;

	yggr::u64 read_byte_size
		= yggr::nsql_database_system
			::mongo_accesser
			::s_gridfs_load_file(conn, db_name, gridfs_name,
									fname, 0, buf, err);

	yggr_test_assert(read_byte_size);
	yggr_test_assert(buf.size() * sizeof(int) >= read_byte_size);

	std::ofstream ofs;
	ofs.open(fpath.c_str(), std::ios_base::out | std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&(*boost::begin(buf))), read_byte_size);
	ofs.flush();
	ofs.close();

	std::cout << "------test_gridfs_load_file_1_2 success------" << std::endl;
}

void test_gridfs_save_file_buffer_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fname("gridfs_test_file_buffer_1_1.txt");
	yggr::string buffer("this is a gridfs file buffer test gridfs buffer 1_1");

	yggr::nsql_database_system::c_bson_error err;

	{
		mongoc_gridfs_file_opt_t opt = { 0 };
		opt.filename = fname.c_str();

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_save_file_of_buffer(conn, db_name.c_str(), gridfs_name.c_str(),
												0, &buffer[0], buffer.size(), &opt, &err);

		yggr_test_assert(bsuccess);
	}

	{
		mongoc_gridfs_file_opt_t opt = { 0 };
		opt.filename = fname.c_str();

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_save_file_of_buffer(conn, db_name.c_str(), gridfs_name,
												0, &buffer[0], buffer.size(), &opt, 0);

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_save_file_buffer_1_1 success------" << std::endl;
}

void test_gridfs_save_file_buffer_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fname("gridfs_test_file_buffer_1_2.txt");
	yggr::string buffer("this is a gridfs file buffer test gridfs buffer 1_2");

	yggr::nsql_database_system::c_bson_error err;

	{
		yggr::nsql_database_system::c_mongo_gridfs_file_opt opt(fname);

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_save_file_of_buffer(conn, db_name, gridfs_name,
												0, &buffer[0], buffer.size(), opt, err);

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_save_file_buffer_1_2 success------" << std::endl;
}

// load file buffer
void test_gridfs_load_file_buffer_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fpath("./nsql_database_system/out_gridfs_test_file_buffer_1_1.txt");
	yggr::string fname("gridfs_test_file_buffer_1_1.txt");

	yggr::nsql_database_system::c_bson_error err;

	{
		std::vector<int> buf;

		yggr::u64 read_byte_size
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_load_file(conn, db_name.c_str(), gridfs_name.c_str(),
										fname.c_str(), 0, buf, &err);

		yggr_test_assert(read_byte_size);
		yggr_test_assert(buf.size() * sizeof(int) >= read_byte_size);

		std::ofstream ofs;
		ofs.open(fpath.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(reinterpret_cast<char*>(&(*boost::begin(buf))), read_byte_size);
		ofs.flush();
		ofs.close();
	}

	std::cout << "------test_gridfs_load_file_buffer_1_1 success------" << std::endl;
}

void test_gridfs_load_file_buffer_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fpath("./nsql_database_system/out_gridfs_test_file_buffer_1_2.txt");
	yggr::string fname("gridfs_test_file_buffer_1_2.txt");

	yggr::nsql_database_system::c_bson_error err;

	{
		std::vector<int> buf;

		yggr::u64 read_byte_size
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_load_file(conn, db_name.c_str(), gridfs_name.c_str(),
										fname.c_str(), 0, buf, &err);

		yggr_test_assert(read_byte_size);
		yggr_test_assert(buf.size() * sizeof(int) >= read_byte_size);

		std::ofstream ofs;
		ofs.open(fpath.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(reinterpret_cast<char*>(&(*boost::begin(buf))), read_byte_size);
		ofs.flush();
		ofs.close();
	}

	std::cout << "------test_gridfs_load_file_buffer_1_2 success------" << std::endl;
}

void test_gridfs_save_file_buffer_2_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fname("gridfs_test_file_buffer_2_1.txt");
	yggr::string buffer("this is a gridfs file buffer test gridfs buffer 2_1");

	yggr::nsql_database_system::c_bson_error err;

	{
		mongoc_gridfs_file_opt_t opt = { 0 };
		opt.filename = fname.c_str();

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_save_file_of_buffer(conn, db_name, gridfs_name,
												0, buffer, &opt, &err);

		yggr_test_assert(bsuccess);
	}

	{
		mongoc_gridfs_file_opt_t opt = { 0 };
		opt.filename = fname.c_str();

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_save_file_of_buffer(conn, db_name, gridfs_name,
												0, buffer, &opt, 0);

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_save_file_buffer_2_1 success------" << std::endl;
}

void test_gridfs_save_file_buffer_2_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fname("gridfs_test_file_buffer_2_2.txt");
	yggr::string buffer("this is a gridfs file buffer test gridfs buffer 2_2");

	yggr::nsql_database_system::c_bson_error err;

	{
		yggr::nsql_database_system::c_mongo_gridfs_file_opt opt(fname);

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_save_file_of_buffer(conn, db_name, gridfs_name,
												0, buffer, opt, err);

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_save_file_buffer_2_2 success------" << std::endl;
}

void test_gridfs_load_file_buffer_2_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fpath("./nsql_database_system/out_gridfs_test_file_buffer_2_1.txt");
	yggr::string fname("gridfs_test_file_buffer_2_1.txt");

	yggr::nsql_database_system::c_bson_error err;

	{
		std::vector<int> buf;

		yggr::u64 read_byte_size
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_load_file(conn, db_name.c_str(), gridfs_name.c_str(),
										fname.c_str(), 0, buf, &err);

		yggr_test_assert(read_byte_size);
		yggr_test_assert(buf.size() * sizeof(int) >= read_byte_size);

		std::ofstream ofs;
		ofs.open(fpath.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(reinterpret_cast<char*>(&(*boost::begin(buf))), read_byte_size);
		ofs.flush();
		ofs.close();
	}

	std::cout << "------test_gridfs_load_file_buffer_2_1 success------" << std::endl;
}

void test_gridfs_load_file_buffer_2_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fpath("./nsql_database_system/out_gridfs_test_file_buffer_2_2.txt");
	yggr::string fname("gridfs_test_file_buffer_2_2.txt");

	yggr::nsql_database_system::c_bson_error err;

	{
		std::vector<int> buf;

		yggr::u64 read_byte_size
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_load_file(conn, db_name.c_str(), gridfs_name.c_str(),
										fname.c_str(), 0, buf, &err);

		yggr_test_assert(read_byte_size);
		yggr_test_assert(buf.size() * sizeof(int) >= read_byte_size);

		std::ofstream ofs;
		ofs.open(fpath.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(reinterpret_cast<char*>(&(*boost::begin(buf))), read_byte_size);
		ofs.flush();
		ofs.close();
	}

	std::cout << "------test_gridfs_load_file_buffer_2_2 success------" << std::endl;
}

void gridfs_find_test_save(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");
	yggr::string fname_tital("gridfs_test_find");
	yggr::string buffer_tital("this is a gridfs file buffer test gridfs buffer");

	yggr::nsql_database_system::c_bson_error err;

	for(int i = 0; i != 10; ++i)
	{
		std::stringstream ss;
		ss << (i + 1);

		yggr::string fname = fname_tital + yggr::string("_") + ss.str().c_str() + ".txt";
		yggr::string buffer = buffer_tital + yggr::string(" ") + ss.str().c_str();

		yggr::nsql_database_system::c_mongo_gridfs_file_opt opt(fname);

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_save_file_of_buffer(conn, db_name, gridfs_name,
												0, buffer, opt, err);

		yggr_test_assert(bsuccess);
	}

	std::cout << "------gridfs_find_test_save success------" << std::endl;
}

struct collector_gridfs
{
	bool operator()(const yggr::nsql_database_system::c_mongo_gridfs_file& file) const
	{
		std::cout << file.file_name() << std::endl;
		return true;
	}
};

void test_gridfs_find_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file gridfs_file_type;


	//struct collector
	//{
	//	bool operator()(const gridfs_file_type& file) const
	//	{
	//		std::cout << file.file_name() << std::endl;
	//		return true;
	//	}
	//};

	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	{
		yggr::nsql_database_system::c_bson query;
		yggr::nsql_database_system::c_bson_error err;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_find(
				conn, db_name.c_str(), gridfs_name.c_str(),
				&query, &err, collector_gridfs());

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson query;
		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_find(conn, db_name.c_str(), gridfs_name,
										query, 0, collector_gridfs());

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_find_1_1 success------" << std::endl;
}

void test_gridfs_find_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file gridfs_file_type;


	//struct collector
	//{
	//	bool operator()(const gridfs_file_type& file) const
	//	{
	//		std::cout << file.file_name() << std::endl;
	//		return true;
	//	}
	//};

	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	{
		yggr::nsql_database_system::c_bson query;
		yggr::nsql_database_system::c_bson_error err;
		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_find(conn, db_name, gridfs_name,
										query, err, collector_gridfs());

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_find_1_2 success------" << std::endl;
}

void test_gridfs_find_one_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file gridfs_file_type;


	//struct collector
	//{
	//	bool operator()(const gridfs_file_type& file) const
	//	{
	//		std::cout << file.file_name() << std::endl;
	//		return true;
	//	}
	//};

	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	{
		yggr::nsql_database_system::c_bson query;
		yggr::nsql_database_system::c_bson_error err;
		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_find_one(conn, db_name.c_str(), gridfs_name.c_str(),
										&query, &err, collector_gridfs());

		yggr_test_assert(bsuccess);
	}

	{
		yggr::nsql_database_system::c_bson query;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_find_one(
					conn, db_name.c_str(), gridfs_name,
					query, 0, collector_gridfs());

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_find_one_1_1 success------" << std::endl;
}

void test_gridfs_find_one_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file gridfs_file_type;

	//struct collector
	//{
	//	bool operator()(const gridfs_file_type& file) const
	//	{
	//		std::cout << file.file_name() << std::endl;
	//		return true;
	//	}
	//};

	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	{
		yggr::nsql_database_system::c_bson query("{\"filename\" : \"gridfs_test_find_9.txt\"}");
		yggr::nsql_database_system::c_bson_error err;
		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_find_one(conn, db_name, gridfs_name,
										query, err, collector_gridfs());

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_find_one_1_2 success------" << std::endl;
}

void test_gridfs_find_one_by_name_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file gridfs_file_type;


	//struct collector
	//{
	//	bool operator()(const gridfs_file_type& file) const
	//	{
	//		std::cout << file.file_name() << std::endl;
	//		return true;
	//	}
	//};

	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	{
		const char* fname = "gridfs_test_find_5.txt";
		yggr::nsql_database_system::c_bson_error err;
		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_find_one(conn, db_name.c_str(), gridfs_name.c_str(),
										fname, &err, collector_gridfs());

		yggr_test_assert(bsuccess);
	}

	{
		yggr::string fname("gridfs_test_find_5.txt");
		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_find_one(conn, db_name.c_str(), gridfs_name,
										fname, 0, collector_gridfs());

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_find_one_by_name_1_1 success------" << std::endl;
}

void test_gridfs_find_one_by_name_1_2(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file gridfs_file_type;

	//struct collector
	//{
	//	bool operator()(const gridfs_file_type& file) const
	//	{
	//		std::cout << file.file_name() << std::endl;
	//		return true;
	//	}
	//};

	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	{
		yggr::string fname("gridfs_test_find_6.txt");
		yggr::nsql_database_system::c_bson_error err;
		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_find_one(conn, db_name, gridfs_name,
										fname, err, collector_gridfs());

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_find_one_by_name_1_2 success------" << std::endl;
}

struct collector_chunk
{
	static bool handler_collect_chunks(const yggr::nsql_database_system::c_bson& bs, 
										std::list<yggr::nsql_database_system::c_bson>& bs_list)
	{
		bs_list.push_back(bs);
		return true;
	}
};

void test_gridfs_chunks_find_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file gridfs_file_type;
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;

	//struct collector
	//{
	//	static bool collect_chunks(const yggr::nsql_database_system::c_bson& bs, bson_list_type& bs_list)
	//	{
	//		bs_list.push_back(bs);
	//		return true;
	//	}
	//};

	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	{
		yggr::nsql_database_system::c_bson query;
		yggr::nsql_database_system::c_bson_error err;
		bson_list_type list;

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_chunks_find(
					conn, db_name, gridfs_name,
					0, 0, 0, 0,
					query, 0, 0, err,
					boost::bind(&collector_chunk::handler_collect_chunks, _1, boost::ref(list)));

		yggr_test_assert(bsuccess);

		for(bson_list_type::const_iterator i = list.begin(), isize = list.end(); i != isize; ++ i)
		{
			std::cout << *i << std::endl;
		}
	}

	std::cout << "------test_gridfs_chunks_find_1_1 success------" << std::endl;
}

void test_gridfs_files_find_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file gridfs_file_type;
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;

	//struct collector
	//{
	//	static bool collect_chunks(const yggr::nsql_database_system::c_bson& bs, bson_list_type& bs_list)
	//	{
	//		bs_list.push_back(bs);
	//		return true;
	//	}
	//};

	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	{
		yggr::nsql_database_system::c_bson query;
		yggr::nsql_database_system::c_bson_error err;
		bson_list_type list;

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_files_find(
					conn, db_name, gridfs_name,
					0, 0, 0, 0,
					query, 0, 0, err,
					boost::bind(&collector_chunk::handler_collect_chunks, _1, boost::ref(list)));

		yggr_test_assert(bsuccess);

		for(bson_list_type::const_iterator i = list.begin(), isize = list.end(); i != isize; ++ i)
		{
			std::cout << *i << std::endl;
		}
	}

	std::cout << "------test_gridfs_files_find_1_1 success------" << std::endl;
}

void test_gridfs_files_remove_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	yggr::string file_name("gridfs_test_find_6.txt");

	{
		yggr::nsql_database_system::c_bson query;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_remove_file(conn, db_name, gridfs_name,
										file_name, err);

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_files_remove_1_1 success------" << std::endl;
}

void test_gridfs_files_remove_1_1_validate(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file gridfs_file_type;

	//struct collector
	//{
	//	bool operator()(const gridfs_file_type& file) const
	//	{
	//		std::cout << file.file_name() << std::endl;
	//		return true;
	//	}
	//};

	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	{
		yggr::string fname("gridfs_test_find_6.txt");
		yggr::nsql_database_system::c_bson_error err;
		bool bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_find_one(conn, db_name, gridfs_name,
										fname, err, collector_gridfs());

		yggr_test_assert(!bsuccess);
	}

	std::cout << "------test_gridfs_files_remove_1_1_validate success------" << std::endl;
}

struct collector_gridfs_files
{
	static bool handler_collecting(const yggr::nsql_database_system::c_mongo_gridfs_file& file,
										std::list<yggr::nsql_database_system::c_mongo_gridfs_file>& list)
	{
		list.push_back(file);
		return true;
	}
};

void test_gridfs_files_begin_end_1_1(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file gridfs_file_type;
	typedef std::list<gridfs_file_type> list_type;

	//struct collector
	//{
	//	static bool collecting(const gridfs_file_type& file, list_type& list)
	//	{
	//		list.push_back(file);
	//		return true;
	//	}
	//};

	yggr::string db_name("gridfs_db");
	yggr::string gridfs_name("fs_test");

	{
		list_type list;
		yggr::nsql_database_system::c_bson query;
		yggr::nsql_database_system::c_bson_error err_begin;
		yggr::nsql_database_system::c_bson_error err_end;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_files_begin(
					conn, db_name, gridfs_name,
					query, err_begin,
					boost::bind(&collector_gridfs_files::handler_collecting, _1, boost::ref(list)));

		yggr_test_assert(bsuccess);

		for(list_type::const_iterator i = list.begin(), isize = list.end(); i != isize; ++i)
		{
			std::cout << (*i).file_name() << std::endl;
		}

		bsuccess
			= yggr::nsql_database_system
				::mongo_accesser
				::s_gridfs_files_end(conn, list, err_end);

		yggr_test_assert(bsuccess);
	}

	std::cout << "------test_gridfs_files_begin_end_1_1 success------" << std::endl;
}

void test_gridfs(void)
{

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://xy:123456abc@127.0.0.1:10398/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://127.0.0.1:10298/?tls=true",	// str_uri
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
				"mongodb://xy:123456abc@127.0.0.1:10198/?tls=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://127.0.0.1:10098/?tls=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

#if 0
	yggr::nsql_database_system::c_mongo_connection client(client_init);
#else
	yggr::nsql_database_system::c_mongo_connection client;
	client.reinit(client_init);
#endif // 0, 1

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		client.clear();
		return;
	}

	yggr::nsql_database_system::mongo_accesser::s_database_drop(client, "gridfs_db", 0);

	test_gridfs_save_file_1_1(client);
	test_gridfs_save_file_1_2(client);

	test_gridfs_load_file_1_1(client);
	test_gridfs_load_file_1_2(client);

	test_gridfs_save_file_buffer_1_1(client);
	test_gridfs_load_file_buffer_1_1(client);

	test_gridfs_save_file_buffer_1_2(client);
	test_gridfs_load_file_buffer_1_2(client);

	test_gridfs_save_file_buffer_2_1(client);
	test_gridfs_load_file_buffer_2_1(client);

	test_gridfs_save_file_buffer_2_2(client);
	test_gridfs_load_file_buffer_2_2(client);

	gridfs_find_test_save(client);
	test_gridfs_find_1_1(client);
	test_gridfs_find_1_2(client);

	test_gridfs_find_one_1_1(client);
	test_gridfs_find_one_1_2(client);

	test_gridfs_find_one_by_name_1_1(client);
	test_gridfs_find_one_by_name_1_2(client);

	test_gridfs_chunks_find_1_1(client);
	test_gridfs_files_find_1_1(client);

	test_gridfs_files_remove_1_1(client);
	test_gridfs_files_remove_1_1_validate(client);

	test_gridfs_files_begin_end_1_1(client);

	client.clear();

	std::cout << "---------test_gridfs end--------------" << std::endl;
}

// gridfs_bucket test
void test_gridfs_bucket_save_file(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;

	yggr::string db_name("gridfs_bucket_db");
	yggr::string gridfs_name("fs_bucket_test");
	yggr::string fpath("./nsql_database_system/gridfs_bucket_test_file.txt");
	yggr::string fname("gridfs_bucket_test_file.txt");
	yggr::string str_gridfs_opts = "{ \"bucketName\" : \"" + gridfs_name + "\" }";

	yggr_test_assert(str_gridfs_opts == "{ \"bucketName\" : \"fs_bucket_test\" }");

	yggr::nsql_database_system::c_bson_error err;

	{
		bson_value_t file_id;
		bson_type bucket_opt(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file\", \"test_number\" : 1}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file(
				conn, db_name.c_str(), fname.c_str(),
				fpath.c_str(), 0, 0, &bucket_opt, 0, stream_opts, &file_id, &err);

		yggr_test_assert(bsuccess);

		std::cout << file_id << std::endl;
	}

	{
		bson_value_t file_id;
		bson_type bucket_opt(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file\", \"test_number\" : 2}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file(
				conn, db_name, fname,
				fpath.c_str(), 0, 0, bucket_opt, 0, &stream_opts, &file_id, &err);

		yggr_test_assert(bsuccess);

		std::cout << file_id << std::endl;
	}

	{
		bson_value_t file_id;
		bson_type bucket_opt(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file\", \"test_number\" : 3}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file(
				conn, db_name, fname,
				fpath, 0, 0, bucket_opt, 0, stream_opts, file_id, err);

		yggr_test_assert(bsuccess);

		std::cout << file_id << std::endl;
	}

	{
		bson_value_t file_id;
		bson_type bucket_opt(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file\", \"test_number\" : 4}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file(
				conn, db_name, fname,
				fpath, 0, 0, bucket_opt, 0, stream_opts, file_id, 0);

		yggr_test_assert(bsuccess);

		std::cout << file_id << std::endl;
	}

	{
		bson_type bucket_opt(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file\", \"test_number\" : 5}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file(
				conn, db_name, fname,
				fpath, 0, 0, bucket_opt, 0, stream_opts, 0, err);

		yggr_test_assert(bsuccess);
	}

	{
		bson_value_type bs_val_file_id(bson_oid_type("00112233445566778899aabb"));
		bson_type bucket_opt(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file\", \"test_number\" : 6}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_with_id(
				conn, db_name, fname,
				fpath, 0, 0, bucket_opt, 0, stream_opts, bs_val_file_id, err);

		yggr_test_assert(bsuccess);

		std::cout << bs_val_file_id << std::endl;
	}

	{
		bson_value_type bs_val_file_id(bson_oid_type("00112233445566778899aabb"));
		bson_type bucket_opt(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file\", \"test_number\" : 7}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_with_id(
				conn, db_name, fname,
				"./nsql_database_system/gridfs_bucket_test_file2.txt", 0, 0, bucket_opt, 0, stream_opts, bs_val_file_id, err);

		yggr_test_assert(!bsuccess);

		std::cout << bs_val_file_id << std::endl;
		std::cout << err << std::endl;
	}

#if 1 // change1 test default fs_name
	{
		bson_value_type bs_val_file_id(bson_oid_type("00112233445566778899aabb"));
		//bson_type bucket_opt(str_gridfs_opts); // default name is "fs"
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file\", \"test_number\" : 7}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_with_id(
				conn, db_name, fname,
				"./nsql_database_system/gridfs_bucket_test_file2.txt", 0, 0, 0, 0, stream_opts, bs_val_file_id, err);

		yggr_test_assert(bsuccess);

		std::cout << bs_val_file_id << std::endl;
	}
#endif // 0, 1

	{
		bson_value_type bs_val_file_id(bson_oid_type("112233445566778899aabbcc"));
		bson_type bucket_opt(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file\", \"test_number\" : 8}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_with_id(
				conn, db_name, fname,
				"./nsql_database_system/gridfs_bucket_test_file2.txt", 0, 0, bucket_opt, 0, stream_opts, bs_val_file_id, err);

		yggr_test_assert(bsuccess);

		std::cout << bs_val_file_id << std::endl;
		std::cout << err << std::endl;
	}

	std::cout << "------test_gridfs_bucket_save_file success------" << std::endl;
}

void test_gridfs_bucket_save_file_buffer(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;

	yggr::string db_name("gridfs_bucket_db");
	yggr::string gridfs_name("fs_bucket_test");
	yggr::string buffer("this is a gridfs bucket_file buffer test gridfs buffer");
	yggr::string fname("gridfs_bucket_test_file_buffer.txt");
	yggr::string str_gridfs_opts = "{ \"bucketName\" : \"" + gridfs_name + "\" }";

	yggr_test_assert(str_gridfs_opts == "{ \"bucketName\" : \"fs_bucket_test\" }");

	yggr_test_assert(buffer.size() == 54);

	yggr::nsql_database_system::c_bson_error err;

	{
		bson_value_t file_id;
		bson_type bucket_opts(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file_buffer\", \"test_number\" : 1}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_of_buffer(
				conn, db_name.c_str(), fname.c_str(),
				0, buffer.data(), buffer.size(),
				&bucket_opts, 0, &stream_opts, &file_id, &err);

		yggr_test_assert(bsuccess);

		std::cout << file_id << std::endl;
	}

	{
		bson_value_t file_id;
		bson_type bucket_opts(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file_buffer\", \"test_number\" : 2}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_of_buffer(
				conn, db_name, fname,
				0, buffer.data(), buffer.size(),
				bucket_opts, 0, &stream_opts, &file_id, err);

		yggr_test_assert(bsuccess);

		std::cout << file_id << std::endl;
	}

	{
		bson_value_t file_id;
		bson_type bucket_opts(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file_buffer\", \"test_number\" : 3}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_of_buffer(
				conn, db_name, fname,
				0, buffer.data(), buffer.size(),
				bucket_opts, 0, stream_opts, file_id, err);

		yggr_test_assert(bsuccess);

		std::cout << file_id << std::endl;
	}

	{
		bson_value_t file_id;
		bson_type bucket_opts(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file_buffer\", \"test_number\" : 4}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_of_buffer(
				conn, db_name, fname,
				0, buffer,
				bucket_opts, 0, &stream_opts, file_id, &err);

		yggr_test_assert(bsuccess);

		std::cout << file_id << std::endl;
	}

	{
		//bson_value_t file_id = {static_cast<bson_type_t>(0)};
		bson_type bucket_opts(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file_buffer\", \"test_number\" : 5}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_of_buffer(
				conn, db_name, fname,
				0, buffer,
				bucket_opts, 0, &stream_opts, 0, &err);

		yggr_test_assert(bsuccess);
	}

	{
		bson_value_type bs_val_file_id(bson_oid_type("2233445566778899aabbccdd"));
		bson_type bucket_opts(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file_buffer\", \"test_number\" : 6}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_of_buffer_with_id(
				conn, db_name, fname,
				0, reinterpret_cast<const yggr::u8*>(buffer.data()), buffer.size(),
				bucket_opts, 0, &stream_opts, bs_val_file_id, &err);

		yggr_test_assert(bsuccess);

		std::cout << bs_val_file_id << std::endl;
		std::cout << err << std::endl;
	}

	{
		bson_value_type bs_val_file_id(bson_oid_type("2233445566778899aabbccdd"));
		bson_type bucket_opts(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file_buffer\", \"test_number\" : 7}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_of_buffer_with_id(
				conn, db_name, fname,
				0, buffer,
				bucket_opts, 0, stream_opts, bs_val_file_id, err);

		yggr_test_assert(!bsuccess);

		std::cout << bs_val_file_id << std::endl;
		std::cout << err << std::endl;
	}

	{
		bson_value_type bs_val_file_id(bson_oid_type("33445566778899aabbccddee"));
		bson_type bucket_opts(str_gridfs_opts);
		bson_type stream_opts("{\"metadata\" : {\"test_foo\" : \"test_gridfs_bucket_save_file_buffer\", \"test_number\" : 8}}");

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_of_buffer_with_id(
				conn, db_name, fname,
				0, buffer,
				bucket_opts, 0, &stream_opts, bs_val_file_id, &err);

		yggr_test_assert(bsuccess);

		std::cout << bs_val_file_id << std::endl;
		std::cout << err << std::endl;
	}

	std::cout << "------test_gridfs_bucket_save_file_buffer success------" << std::endl;
}

//gridfs_load_file
void test_gridfs_bucket_load_file(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;

	yggr::string db_name("gridfs_bucket_db");
	yggr::string gridfs_name("fs_bucket_test");
	yggr::string fpath("./nsql_database_system/out_gridfs_bucket_test_file.txt");

	yggr::string str_gridfs_opts = "{ \"bucketName\" : \"" + gridfs_name + "\" }";

	yggr_test_assert(str_gridfs_opts == "{ \"bucketName\" : \"fs_bucket_test\" }");

	yggr::nsql_database_system::c_bson_error err;

	{
		bson_value_type bs_val_file_id(bson_oid_type("00112233445566778899aabb"));
		bson_type bucket_opt(str_gridfs_opts);
		yggr::string buf;

		yggr::u64 read_byte_size
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_load_file(
				conn, db_name.c_str(),
				&bucket_opt, 0,
				boost::addressof(bs_val_file_id), 0,
				buf, &err);

		yggr_test_assert(read_byte_size);
		yggr_test_assert(buf.size() == read_byte_size);

		std::cout << "loaded data: " << buf << std::endl;
		yggr_test_assert(buf == "this file is test gridfs bucket write and read");

		std::ofstream ofs;
		ofs.open(fpath.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(reinterpret_cast<char*>(&(*boost::begin(buf))), read_byte_size);
		ofs.flush();
		ofs.close();
	}

	{
		bson_value_type bs_val_file_id(bson_oid_type("2233445566778899aabbccdd"));
		bson_type bucket_opt(str_gridfs_opts);
		yggr::string buf;

		yggr::u64 read_byte_size
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_load_file(
				conn, db_name,
				bucket_opt, 0,
				bs_val_file_id, 0,
				buf, err);

		yggr_test_assert(read_byte_size);
		yggr_test_assert(buf.size() == read_byte_size);

		std::cout << "loaded data: " << buf << std::endl;
		yggr_test_assert(buf == "this is a gridfs bucket_file buffer test gridfs buffer");

		std::ofstream ofs;
		ofs.open(fpath.c_str(), std::ios_base::out | std::ios_base::binary);
		ofs.write(reinterpret_cast<char*>(&(*boost::begin(buf))), read_byte_size);
		ofs.flush();
		ofs.close();
	}

	std::cout << "------test_gridfs_bucket_load_file success------" << std::endl;
}

void gridfs_bucket_find_test_save(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;

	yggr::string db_name("gridfs_bucket_db");
	yggr::string gridfs_name("fs_bucket_test");

	yggr::string fname("gridfs_bucket_test_find");
	yggr::string buffer_data("this is a gridfs_bucket file buffer test gridfs bucket find");

	yggr::string str_gridfs_opts = "{ \"bucketName\" : \"" + gridfs_name + "\" }";

	yggr_test_assert(str_gridfs_opts == "{ \"bucketName\" : \"fs_bucket_test\" }");

	yggr::string file_id_part1 = "33445566778899001122aa";

	yggr::nsql_database_system::c_bson_error err;

	for(int i = 0; i != 10; ++i)
	{
		std::stringstream ss;
		ss << "0" << i;

		//yggr::string fname = fname_title + "_" + ss.str().c_str() + ".txt";
		// "33445566778899001122aa00", "33445566778899001122aa01", ..., "33445566778899001122aa09"
		yggr::string str_file_id = file_id_part1 + ss.str().c_str();
		yggr::string buffer = buffer_data + yggr::string(" ") + ss.str().c_str();

		bson_type bucket_opt(str_gridfs_opts);
		bson_value_type bs_val_file_id((bson_oid_type(str_file_id)));
		//bson_value_type bs_val_file_id(bson_oid_type(str_file_id));// msvc bug link error
		bson_type stream_opts("{\"metadata\" : { \"test_foo\" : \"gridfs_bucket_find_test_save\" }}");

		bool bsuccess = true;
		bsuccess =
			yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_save_file_of_buffer_with_id(
				conn, db_name, fname,
				0, buffer,
				bucket_opt, 0,
				stream_opts,
				bs_val_file_id, err);

		if(!bsuccess)
		{
			std::cout << err << std::endl;
			break;
		}

		yggr_test_assert(bsuccess);
	}

	std::cout << "------gridfs_bucket_find_test_save success------" << std::endl;
}

struct collector_bucket
{
	bool operator()(const yggr::nsql_database_system::c_bson& file) const
	{
		std::cout << file << std::endl;
		return true;
	}
};

struct collector_bucket_one
{
	bool operator()(const yggr::nsql_database_system::c_bson& file) const
	{
		std::cout << file << std::endl;
		return false;
	}
};

void test_gridfs_bucket_find(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;

	yggr::string db_name("gridfs_bucket_db");
	yggr::string gridfs_name("fs_bucket_test");

	yggr::string str_gridfs_opts = "{ \"bucketName\" : \"" + gridfs_name + "\" }";

	yggr_test_assert(str_gridfs_opts == "{ \"bucketName\" : \"fs_bucket_test\" }");

	yggr::nsql_database_system::c_bson_error err;

	//struct collector
	//{
	//	bool operator()(const bson_type& file) const
	//	{
	//		std::cout << file << std::endl;
	//		return true;
	//	}
	//};

	{
		bson_type bucket_opt(str_gridfs_opts);
		bson_type filter;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_find(
				conn, db_name.c_str(),
				&bucket_opt, 0,
				&filter, 0,
				&err,
				collector_bucket());

		yggr_test_assert(bsuccess);
	}

	std::cout << "----------------" << std::endl;

	{
		bson_type bucket_opt(str_gridfs_opts);
		bson_type filter;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_find(
				conn, db_name,
				bucket_opt, 0,
				filter, 0,
				err,
				collector_bucket());
		yggr_test_assert(bsuccess);
	}

	std::cout << "----------------" << std::endl;

	{
		bson_type filter("{\"_id\" : { \"$oid\" : \"33445566778899001122aa00\" } }");
		bson_type bucket_opt(str_gridfs_opts);

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_find(
				conn, db_name,
				bucket_opt, 0,
				filter, 0,
				err,
				collector_bucket());

		yggr_test_assert(bsuccess);
	}

	std::cout << "----------------" << std::endl;

	{
		bson_type filter("{\"filename\" : \"gridfs_bucket_test_find\" }");
		bson_type bucket_opt(str_gridfs_opts);

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_find(
				conn, db_name,
				bucket_opt, 0,
				filter, 0,
				err,
				collector_bucket());

		yggr_test_assert(bsuccess);
	}

	std::cout << "----------------" << std::endl;

	//struct collector_one
	//{
	//	bool operator()(const bson_type& file) const
	//	{
	//		std::cout << file << std::endl;
	//		return false;
	//	}
	//};

	{
		bson_type filter("{\"filename\" : \"gridfs_bucket_test_find\" }");
		bson_type bucket_opt(str_gridfs_opts);

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_find(
				conn, db_name,
				bucket_opt, 0,
				filter, 0,
				err,
				collector_bucket_one());

		yggr_test_assert(bsuccess);
	}

	std::cout << "----------------" << std::endl;

	std::cout << "------test_gridfs_bucket_find success------" << std::endl;
}


void test_gridfs_files_remove_file(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;

	yggr::string db_name("gridfs_bucket_db");
	yggr::string gridfs_name("fs_bucket_test");

	yggr::string str_gridfs_opts = "{ \"bucketName\" : \"" + gridfs_name + "\" }";

	yggr_test_assert(str_gridfs_opts == "{ \"bucketName\" : \"fs_bucket_test\" }");

	yggr::nsql_database_system::c_bson_error err;

	// s_gridfs_bucket_remove_file_by_id
	{
		bson_type bucket_opt(str_gridfs_opts);
		bson_value_type bs_val_file_id((bson_oid_type("2233445566778899aabbccdd")));

		yggr::u64 rm_count
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_remove_file_by_id(
				conn, db_name.c_str(),
				&bucket_opt, 0,
				&bs_val_file_id,
				&err);

		yggr_test_assert(rm_count == 1);
	}

	{
		bson_type bucket_opt(str_gridfs_opts);
		bson_value_type bs_val_file_id((bson_oid_type("2233445566778899aabbccdd")));

		yggr::u64 rm_count
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_remove_file_by_id(
				conn, db_name,
				bucket_opt, 0,
				bs_val_file_id,
				err);

		yggr_test_assert(rm_count == 0);
	}

	// s_gridfs_bucket_remove_files
	{

		bson_type bucket_opt(str_gridfs_opts);
		bson_type filter("{\"filename\" : \"gridfs_bucket_test_find\" }");

		yggr::u64 rm_count
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_remove_files(
				conn, db_name,
				bucket_opt, 0,
				filter, 0,
				err);

		yggr_test_assert(rm_count == 10);
		std::cout << "remove files count = " << rm_count << std::endl;
	}

	{
		bson_type bucket_opt(str_gridfs_opts);
		bson_type filter;

		yggr::u64 rm_count
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_remove_files(
				conn, db_name.c_str(),
				&bucket_opt, 0,
				&filter, 0,
				&err);

		yggr_test_assert(rm_count != 0);
		std::cout << "remove files count = " << rm_count << std::endl;
	}

	{
		bson_type bucket_opt(str_gridfs_opts);
		bson_type filter;

		yggr::u64 rm_count
			= yggr::nsql_database_system::mongo_accesser::s_gridfs_bucket_remove_files(
				conn, db_name,
				bucket_opt, 0,
				filter, 0,
				err);

		yggr_test_assert(rm_count == 0);
	}

	std::cout << "------test_gridfs_files_remove_file success------" << std::endl;
}

void test_gridfs_bucket(void)
{

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://xy:123456abc@127.0.0.1:10398/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://127.0.0.1:10298/?tls=true",	// str_uri
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
				"mongodb://xy:123456abc@127.0.0.1:10198/?tls=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection::init_type client_init(
				"mongodb://127.0.0.1:10098/?tls=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

#if 0
	yggr::nsql_database_system::c_mongo_connection client(client_init);
#else
	yggr::nsql_database_system::c_mongo_connection client;
	client.reinit(client_init);
#endif // 0, 1

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		client.clear();
		return;
	}

	yggr::nsql_database_system::mongo_accesser::s_database_drop(client, "gridfs_bucket_db", 0);

	test_gridfs_bucket_save_file(client);
	test_gridfs_bucket_save_file_buffer(client);
	test_gridfs_bucket_load_file(client);

	gridfs_bucket_find_test_save(client);
	test_gridfs_bucket_find(client);

	test_gridfs_files_remove_file(client);

	client.clear();

	std::cout << "---------test_gridfs end--------------" << std::endl;
}

void test_client_watch(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::nsql_database_system::c_bson> bson_list_type;

	//yggr::string db_name = "test";

	//yggr::nsql_database_system::c_mongo_uri uri =
	//	yggr::nsql_database_system::mongo_accesser::s_client_get_uri(conn);

	yggr::nsql_database_system::c_mongo_uri uri = conn.uri();

	bool b_is_replic_sets = !uri.get_replica_set().empty();

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_watch(
				conn,
				&pipeline, 0,
				0, 0,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "client_watch_reply = " << reply << std::endl;
		std::cout << "client_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_watch(
				conn,
				&pipeline, 0,
				&reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_watch(
				conn,
				&pipeline, 0,
				reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_watch(
				conn,
				&pipeline, 0,
				&reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_watch(
				conn,
				pipeline, 0,
				reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	// s_watch
	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				&pipeline, 0,
				0, 0,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "client_watch_reply = " << reply << std::endl;
		std::cout << "client_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				&pipeline, 0,
				&reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				&pipeline, 0,
				reply, &err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				&pipeline, 0,
				&reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson pipeline;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bson_list_type bs_list;
		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_watch(
				conn,
				pipeline, 0,
				reply, err,
				boost::bind(&handler_collector, boost::ref(bs_list), _1) );

		if(b_is_replic_sets)
		{
			yggr_test_assert(bsuccess); // only supported on replica sets

			for(bson_list_type::const_iterator i = bs_list.begin(), isize = bs_list.end();
					i != isize; ++i)
			{
				std::cout << *i << std::endl;
			}
		}
		else
		{
			yggr_test_assert(!bsuccess); // only supported on replica sets
		}

		std::cout << "database_watch_reply = " << reply << std::endl;
		std::cout << "database_watch_err = " << err << std::endl;
	}
}

void test_client_foo(void)
{

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://xy:123456abc@127.0.0.1:10398/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://127.0.0.1:10298/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);

#	endif // MONGODB_USING_AUTH

#else

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://xy:123456abc@127.0.0.1:10198/?tls=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://127.0.0.1:10098/?tls=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		client.clear();
		return;
	}

	std::cout << "--------test_s_client_watch s-----------" << std::endl;

	test_client_watch(client);

	std::cout << "--------test_s_client_watch e-----------" << std::endl;

	client.clear();

	std::cout << "---------test_other_foo end--------------" << std::endl;
}

// test other function

struct collector_string_list
{
	static void handler_collector(const char* name, std::list<yggr::string>& str_list)
	{
		yggr::utf8_string str_name(name, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		str_list.push_back(str_name.str<yggr::string>());
	}
};

void test_get_database_names(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::string> str_list_type;

	//struct collector
	//{
	//	static void handler_collector(const char* name, std::list<yggr::string>& str_list)
	//	{
	//		yggr::utf8_string str_name(name, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//		str_list.push_back(str_name.str<yggr::string>());
	//	}
	//};

	{
		yggr::nsql_database_system::c_bson_error err;

		str_list_type str_list;
		bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
					conn, 0,
					boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

		yggr_test_assert(bsuccess);

		for(str_list_type::const_iterator i = str_list.begin(), isize = str_list.end(); i != isize; ++i)
		{
			std::cout<< *i << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		str_list_type str_list;
		bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
					conn, &err,
					boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

		yggr_test_assert(bsuccess);

		for(str_list_type::const_iterator i = str_list.begin(), isize = str_list.end(); i != isize; ++i)
		{
			std::cout<< *i << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		str_list_type str_list;
		bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
					conn, err,
					boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

		yggr_test_assert(bsuccess);

		for(str_list_type::const_iterator i = str_list.begin(), isize = str_list.end(); i != isize; ++i)
		{
			std::cout<< *i << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		str_list_type str_list;
		bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
					conn, 0, err,
					boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

		yggr_test_assert(bsuccess);

		for(str_list_type::const_iterator i = str_list.begin(), isize = str_list.end(); i != isize; ++i)
		{
			std::cout<< *i << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson_error err;

		str_list_type str_list;
		bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
					conn, &opts, err,
					boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

		yggr_test_assert(bsuccess);

		for(str_list_type::const_iterator i = str_list.begin(), isize = str_list.end(); i != isize; ++i)
		{
			std::cout<< *i << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson opts;
		yggr::nsql_database_system::c_bson_error err;

		str_list_type str_list;
		bool bsuccess
				= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
					conn, opts, err,
					boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

		yggr_test_assert(bsuccess);

		for(str_list_type::const_iterator i = str_list.begin(), isize = str_list.end(); i != isize; ++i)
		{
			std::cout<< *i << std::endl;
		}
	}

	std::cout << "------test_get_database_names success------" << std::endl;
}

void test_get_server_status(yggr::nsql_database_system::c_mongo_connection& conn)
{
	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_get_server_status(
				conn, 0, 0, 0);

		yggr_test_assert(bsuccess);

		std::cout << "read_prefs = " << read_prefs << std::endl;
		std::cout << "reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_get_server_status(
				conn, read_prefs, 0, err);

		yggr_test_assert(bsuccess);

		std::cout << "read_prefs = " << read_prefs << std::endl;
		std::cout << "reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_get_server_status(
				conn, &read_prefs, &reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "read_prefs = " << read_prefs << std::endl;
		std::cout << "reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson reply;
		yggr::nsql_database_system::c_bson_error err;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_get_server_status(
				conn, &read_prefs, reply, &err);

		yggr_test_assert(bsuccess);

		std::cout << "read_prefs = " << read_prefs << std::endl;
		std::cout << "reply = " << reply << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson reply;

		bool bsuccess
			= yggr::nsql_database_system::mongo_accesser::s_client_get_server_status(
				conn, read_prefs, reply);

		yggr_test_assert(bsuccess);

		std::cout << "read_prefs = " << read_prefs << std::endl;
		std::cout << "reply = " << reply << std::endl;
	}

	std::cout << "------test_get_read_prefs success------" << std::endl;
}

void test_get_server_description(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef yggr::nsql_database_system::c_mongo_server_description svr_desc_type;

	yggr::u32 svrid = 1;

	{
		yggr::nsql_database_system::c_bson_error err;

		svr_desc_type svr_desc
			= mongo_accesser_type::s_client_get_server_description(conn, 0);

		yggr_test_assert(svr_desc.empty());
		yggr_test_assert(svr_desc.id() == 0);
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		svr_desc_type svr_desc
			= mongo_accesser_type::s_client_get_server_description(conn, svrid);

		yggr_test_assert(!svr_desc.empty());
		yggr_test_assert(svr_desc.id() == svrid);

		std::cout << "id = " << svr_desc.id() << std::endl;
		std::cout << "last_update_time = " << svr_desc.last_update_time() << std::endl;
		std::cout << "round_trip_time = " << svr_desc.round_trip_time() << std::endl;
		std::cout << "hello_response = " << svr_desc.hello_response() << std::endl;
		std::cout << "typeid_name = " << svr_desc.typeid_name() << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		svr_desc_type svr_desc
			= mongo_accesser_type::s_client_get_server_description(conn, svrid, err.org_pointer());

		yggr_test_assert(!svr_desc.empty());
		yggr_test_assert(svr_desc.id() == svrid);
		yggr_test_assert(!err);

		std::cout << "id = " << svr_desc.id() << std::endl;
		std::cout << "last_update_time = " << svr_desc.last_update_time() << std::endl;
		std::cout << "round_trip_time = " << svr_desc.round_trip_time() << std::endl;
		std::cout << "hello_response = " << svr_desc.hello_response() << std::endl;
		std::cout << "typeid_name = " << svr_desc.typeid_name() << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		svr_desc_type svr_desc
			= mongo_accesser_type::s_client_get_server_description(conn, svrid, *(err.org_pointer()));

		yggr_test_assert(!svr_desc.empty());
		yggr_test_assert(svr_desc.id() == svrid);
		yggr_test_assert(!err);

		std::cout << "id = " << svr_desc.id() << std::endl;
		std::cout << "last_update_time = " << svr_desc.last_update_time() << std::endl;
		std::cout << "round_trip_time = " << svr_desc.round_trip_time() << std::endl;
		std::cout << "hello_response = " << svr_desc.hello_response() << std::endl;
		std::cout << "typeid_name = " << svr_desc.typeid_name() << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		svr_desc_type svr_desc
			= mongo_accesser_type::s_client_get_server_description(conn, svrid, err);

		yggr_test_assert(!svr_desc.empty());
		yggr_test_assert(svr_desc.id() == svrid);
		yggr_test_assert(!err);

		std::cout << "id = " << svr_desc.id() << std::endl;
		std::cout << "last_update_time = " << svr_desc.last_update_time() << std::endl;
		std::cout << "round_trip_time = " << svr_desc.round_trip_time() << std::endl;
		std::cout << "hello_response = " << svr_desc.hello_response() << std::endl;
		std::cout << "typeid_name = " << svr_desc.typeid_name() << std::endl;
	}

	std::cout << "------test_get_server_description success------" << std::endl;
}

void test_get_server_descriptions(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef yggr::nsql_database_system::c_mongo_server_description svr_desc_type;
	typedef yggr::vector<svr_desc_type> out_cont_type;

	struct svr_desc_collection
	{
		static bool server_desc_collection(out_cont_type& out_cont, svr_desc_type& svr_desc)
		{
			out_cont.push_back(boost::move(svr_desc));
			return true;
		}
	};

	//yggr::u32 svrid = 1;

	{
		yggr::nsql_database_system::c_bson_error err;

		out_cont_type out_cont;

		std::size_t svr_desc_count =
			mongo_accesser_type::s_client_get_server_descriptions(
				conn, boost::bind(&svr_desc_collection::server_desc_collection, boost::ref(out_cont), _1));

		yggr_test_assert(1 == svr_desc_count);

		for(out_cont_type::const_iterator i = out_cont.begin(), isize = out_cont.end(); i != isize; ++i)
		{
			yggr_test_assert(!(*i).empty());

			std::cout << "id = " << (*i).id() << std::endl;
			std::cout << "last_update_time = " << (*i).last_update_time() << std::endl;
			std::cout << "round_trip_time = " << (*i).round_trip_time() << std::endl;
			std::cout << "hello_response = " << (*i).hello_response() << std::endl;
			std::cout << "typeid_name = " << (*i).typeid_name() << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		out_cont_type out_cont;

		std::size_t svr_desc_count =
			mongo_accesser_type::s_client_get_server_descriptions(
				conn, 0, boost::bind(&svr_desc_collection::server_desc_collection, boost::ref(out_cont), _1));

		yggr_test_assert(1 == svr_desc_count);

		for(out_cont_type::const_iterator i = out_cont.begin(), isize = out_cont.end(); i != isize; ++i)
		{
			yggr_test_assert(!(*i).empty());

			std::cout << "id = " << (*i).id() << std::endl;
			std::cout << "last_update_time = " << (*i).last_update_time() << std::endl;
			std::cout << "round_trip_time = " << (*i).round_trip_time() << std::endl;
			std::cout << "hello_response = " << (*i).hello_response() << std::endl;
			std::cout << "typeid_name = " << (*i).typeid_name() << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		out_cont_type out_cont;

		std::size_t svr_desc_count =
			mongo_accesser_type::s_client_get_server_descriptions(
				conn, err.org_pointer(), boost::bind(&svr_desc_collection::server_desc_collection, boost::ref(out_cont), _1));

		yggr_test_assert(!err);
		yggr_test_assert(1 == svr_desc_count);

		for(out_cont_type::const_iterator i = out_cont.begin(), isize = out_cont.end(); i != isize; ++i)
		{
			yggr_test_assert(!(*i).empty());

			std::cout << "id = " << (*i).id() << std::endl;
			std::cout << "last_update_time = " << (*i).last_update_time() << std::endl;
			std::cout << "round_trip_time = " << (*i).round_trip_time() << std::endl;
			std::cout << "hello_response = " << (*i).hello_response() << std::endl;
			std::cout << "typeid_name = " << (*i).typeid_name() << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		out_cont_type out_cont;

		std::size_t svr_desc_count =
			mongo_accesser_type::s_client_get_server_descriptions(
				conn, *err.org_pointer(), boost::bind(&svr_desc_collection::server_desc_collection, boost::ref(out_cont), _1));

		yggr_test_assert(!err);
		yggr_test_assert(1 == svr_desc_count);

		for(out_cont_type::const_iterator i = out_cont.begin(), isize = out_cont.end(); i != isize; ++i)
		{
			yggr_test_assert(!(*i).empty());

			std::cout << "id = " << (*i).id() << std::endl;
			std::cout << "last_update_time = " << (*i).last_update_time() << std::endl;
			std::cout << "round_trip_time = " << (*i).round_trip_time() << std::endl;
			std::cout << "hello_response = " << (*i).hello_response() << std::endl;
			std::cout << "typeid_name = " << (*i).typeid_name() << std::endl;
		}
	}

	{
		yggr::nsql_database_system::c_bson_error err;

		out_cont_type out_cont;

		std::size_t svr_desc_count =
			mongo_accesser_type::s_client_get_server_descriptions(
				conn, err, boost::bind(&svr_desc_collection::server_desc_collection, boost::ref(out_cont), _1));

		yggr_test_assert(!err);
		yggr_test_assert(1 == svr_desc_count);

		for(out_cont_type::const_iterator i = out_cont.begin(), isize = out_cont.end(); i != isize; ++i)
		{
			yggr_test_assert(!(*i).empty());

			std::cout << "id = " << (*i).id() << std::endl;
			std::cout << "last_update_time = " << (*i).last_update_time() << std::endl;
			std::cout << "round_trip_time = " << (*i).round_trip_time() << std::endl;
			std::cout << "hello_response = " << (*i).hello_response() << std::endl;
			std::cout << "typeid_name = " << (*i).typeid_name() << std::endl;
		}
	}

	std::cout << "------test_get_server_descriptions success------" << std::endl;
}

void test_select_server(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef yggr::nsql_database_system::c_mongo_server_description svr_desc_type;

	{
		svr_desc_type svr_desc = mongo_accesser_type::s_client_select_server(conn, true, 0, 0);

		yggr_test_assert(!svr_desc.empty());

		std::cout << "id = " << svr_desc.id() << std::endl;
		std::cout << "last_update_time = " << svr_desc.last_update_time() << std::endl;
		std::cout << "round_trip_time = " << svr_desc.round_trip_time() << std::endl;
		std::cout << "hello_response = " << svr_desc.hello_response() << std::endl;
		std::cout << "typeid_name = " << svr_desc.typeid_name() << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson_error err;

		svr_desc_type svr_desc =
			mongo_accesser_type::s_client_select_server(
				conn, false, read_prefs.org_pointer(), 0);

		yggr_test_assert(!svr_desc.empty());

		std::cout << "id = " << svr_desc.id() << std::endl;
		std::cout << "last_update_time = " << svr_desc.last_update_time() << std::endl;
		std::cout << "round_trip_time = " << svr_desc.round_trip_time() << std::endl;
		std::cout << "hello_response = " << svr_desc.hello_response() << std::endl;
		std::cout << "typeid_name = " << svr_desc.typeid_name() << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson_error err;

		svr_desc_type svr_desc =
			mongo_accesser_type::s_client_select_server(
				conn, false, *(read_prefs.org_pointer()), err.org_pointer());

		yggr_test_assert(!svr_desc.empty());

		std::cout << "id = " << svr_desc.id() << std::endl;
		std::cout << "last_update_time = " << svr_desc.last_update_time() << std::endl;
		std::cout << "round_trip_time = " << svr_desc.round_trip_time() << std::endl;
		std::cout << "hello_response = " << svr_desc.hello_response() << std::endl;
		std::cout << "typeid_name = " << svr_desc.typeid_name() << std::endl;
	}

	{
		yggr::nsql_database_system::c_mongo_read_prefs read_prefs;
		yggr::nsql_database_system::c_bson_error err;

		svr_desc_type svr_desc =
			mongo_accesser_type::s_client_select_server(
				conn, false, read_prefs, err);

		yggr_test_assert(!svr_desc.empty());

		std::cout << "id = " << svr_desc.id() << std::endl;
		std::cout << "last_update_time = " << svr_desc.last_update_time() << std::endl;
		std::cout << "round_trip_time = " << svr_desc.round_trip_time() << std::endl;
		std::cout << "hello_response = " << svr_desc.hello_response() << std::endl;
		std::cout << "typeid_name = " << svr_desc.typeid_name() << std::endl;
	}

	std::cout << "------test_select_server success------" << std::endl;
}

void test_get_set_read_concern(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef yggr::nsql_database_system::c_mongo_server_description svr_desc_type;
	typedef yggr::nsql_database_system::c_mongo_read_concern read_concern_type;

	{
		read_concern_type read_concern =
			mongo_accesser_type::s_client_get_read_concern(conn);

		yggr_test_assert(read_concern.is_default());
		std::cout << read_concern << std::endl;
	}

	{
		read_concern_type read_concern =
			mongo_accesser_type::s_client_get_read_concern(conn, 0);

		yggr_test_assert(read_concern.is_default());
		std::cout << read_concern << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		read_concern_type read_concern =
			mongo_accesser_type::s_client_get_read_concern(conn, err.org_pointer());

		yggr_test_assert(!err);
		yggr_test_assert(read_concern.is_default());
		std::cout << read_concern << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		read_concern_type read_concern =
			mongo_accesser_type::s_client_get_read_concern(conn, err);

		yggr_test_assert(!err);
		yggr_test_assert(read_concern.is_default());
		std::cout << read_concern << std::endl;
	}

	{
		read_concern_type org_read_concern = mongo_accesser_type::s_client_get_read_concern(conn);
		read_concern_type new_read_concern(read_concern_type::s_level_majority());

		mongo_accesser_type::s_client_set_read_concern(conn, new_read_concern.org_pointer());

		read_concern_type now_read_concern = mongo_accesser_type::s_client_get_read_concern(conn);

		yggr_test_assert(now_read_concern.var_level() == new_read_concern.var_level());
		std::cout << now_read_concern << std::endl;

		mongo_accesser_type::s_client_set_read_concern(conn, org_read_concern);
	}

	{
		read_concern_type org_read_concern = mongo_accesser_type::s_client_get_read_concern(conn);
		read_concern_type new_read_concern(read_concern_type::s_level_majority());

		mongo_accesser_type::s_client_set_read_concern(conn, *new_read_concern.org_pointer());

		read_concern_type now_read_concern = mongo_accesser_type::s_client_get_read_concern(conn);

		yggr_test_assert(now_read_concern.var_level() == new_read_concern.var_level());
		std::cout << now_read_concern << std::endl;

		mongo_accesser_type::s_client_set_read_concern(conn, org_read_concern);
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		read_concern_type org_read_concern = mongo_accesser_type::s_client_get_read_concern(conn);
		read_concern_type new_read_concern(read_concern_type::s_level_majority());

		mongo_accesser_type::s_client_set_read_concern(conn, new_read_concern, err);

		yggr_test_assert(!err);

		read_concern_type now_read_concern = mongo_accesser_type::s_client_get_read_concern(conn);

		yggr_test_assert(now_read_concern.var_level() == new_read_concern.var_level());
		std::cout << now_read_concern << std::endl;

		mongo_accesser_type::s_client_set_read_concern(conn, org_read_concern);
	}

	std::cout << "------test_get_set_read_concern success------" << std::endl;
}

void test_get_set_read_prefs(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef yggr::nsql_database_system::c_mongo_server_description svr_desc_type;
	typedef yggr::nsql_database_system::c_mongo_read_prefs read_prefs_type;

	{
		read_prefs_type read_prefs = mongo_accesser_type::s_client_get_read_prefs(conn);

		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	{
		read_prefs_type read_prefs = mongo_accesser_type::s_client_get_read_prefs(conn, 0);

		yggr_test_assert(read_prefs.validate());
		yggr_test_assert(read_prefs.var_mode() == read_prefs_type::E_MONGOC_READ_PRIMARY);
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		read_prefs_type org_read_prefs = mongo_accesser_type::s_client_get_read_prefs(conn);
		read_prefs_type new_read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);

		mongo_accesser_type::s_client_set_read_prefs(conn, new_read_prefs, 0);

		read_prefs_type now_read_prefs = mongo_accesser_type::s_client_get_read_prefs(conn);

		yggr_test_assert(now_read_prefs.var_mode() == new_read_prefs.var_mode());

		std::cout << now_read_prefs << std::endl;

		mongo_accesser_type::s_client_set_read_prefs(conn, org_read_prefs);
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		read_prefs_type org_read_prefs = mongo_accesser_type::s_client_get_read_prefs(conn);
		read_prefs_type new_read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);

		mongo_accesser_type::s_client_set_read_prefs(conn, new_read_prefs, err.org_pointer());
		yggr_test_assert(!err);

		read_prefs_type now_read_prefs = mongo_accesser_type::s_client_get_read_prefs(conn);

		yggr_test_assert(now_read_prefs.var_mode() == new_read_prefs.var_mode());

		std::cout << now_read_prefs << std::endl;

		mongo_accesser_type::s_client_set_read_prefs(conn, org_read_prefs);
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		read_prefs_type org_read_prefs = mongo_accesser_type::s_client_get_read_prefs(conn);
		read_prefs_type new_read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);

		mongo_accesser_type::s_client_set_read_prefs(conn, new_read_prefs, *err.org_pointer());
		yggr_test_assert(!err);

		read_prefs_type now_read_prefs = mongo_accesser_type::s_client_get_read_prefs(conn);

		yggr_test_assert(now_read_prefs.var_mode() == new_read_prefs.var_mode());

		std::cout << now_read_prefs << std::endl;

		mongo_accesser_type::s_client_set_read_prefs(conn, org_read_prefs);
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		read_prefs_type org_read_prefs = mongo_accesser_type::s_client_get_read_prefs(conn);
		read_prefs_type new_read_prefs(read_prefs_type::E_MONGOC_READ_SECONDARY);

		mongo_accesser_type::s_client_set_read_prefs(conn, new_read_prefs, err);
		yggr_test_assert(!err);

		read_prefs_type now_read_prefs = mongo_accesser_type::s_client_get_read_prefs(conn);

		yggr_test_assert(now_read_prefs.var_mode() == new_read_prefs.var_mode());

		std::cout << now_read_prefs << std::endl;

		mongo_accesser_type::s_client_set_read_prefs(conn, org_read_prefs);
	}

	std::cout << "------test_get_set_read_prefs success------" << std::endl;
}

void test_get_set_write_concern(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef yggr::nsql_database_system::c_mongo_server_description svr_desc_type;
	typedef yggr::nsql_database_system::c_mongo_write_concern write_concern_type;

	{
		write_concern_type write_concern = mongo_accesser_type::s_client_get_write_concern(conn);

		yggr_test_assert(write_concern.is_default());
		std::cout << write_concern << std::endl;
	}

	{
		write_concern_type write_concern = mongo_accesser_type::s_client_get_write_concern(conn, 0);

		yggr_test_assert(write_concern.is_default());
		std::cout << write_concern << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		write_concern_type write_concern = mongo_accesser_type::s_client_get_write_concern(conn, err.org_pointer());

		yggr_test_assert(!err);

		yggr_test_assert(write_concern.is_default());
		std::cout << write_concern << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		write_concern_type write_concern = mongo_accesser_type::s_client_get_write_concern(conn, *err.org_pointer());

		yggr_test_assert(!err);

		yggr_test_assert(write_concern.is_default());
		std::cout << write_concern << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		write_concern_type write_concern = mongo_accesser_type::s_client_get_write_concern(conn, err);

		yggr_test_assert(!err);

		yggr_test_assert(write_concern.is_default());
		std::cout << write_concern << std::endl;
	}

	{
		write_concern_type org_write_concern = mongo_accesser_type::s_client_get_write_concern(conn);
		write_concern_type new_write_concern(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		mongo_accesser_type::s_client_set_write_concern(conn, new_write_concern);

		write_concern_type now_write_concern = mongo_accesser_type::s_client_get_write_concern(conn);

		yggr_test_assert(now_write_concern.var_w() == new_write_concern.var_w());
		std::cout << now_write_concern << std::endl;

		mongo_accesser_type::s_client_set_write_concern(conn, org_write_concern);
	}

	{
		write_concern_type org_write_concern = mongo_accesser_type::s_client_get_write_concern(conn);
		write_concern_type new_write_concern(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		mongo_accesser_type::s_client_set_write_concern(conn, new_write_concern, 0);

		write_concern_type now_write_concern = mongo_accesser_type::s_client_get_write_concern(conn);

		yggr_test_assert(now_write_concern.var_w() == new_write_concern.var_w());
		std::cout << now_write_concern << std::endl;

		mongo_accesser_type::s_client_set_write_concern(conn, org_write_concern);
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		write_concern_type org_write_concern = mongo_accesser_type::s_client_get_write_concern(conn);
		write_concern_type new_write_concern(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		mongo_accesser_type::s_client_set_write_concern(conn, new_write_concern, err.org_pointer());
		yggr_test_assert(!err);

		write_concern_type now_write_concern = mongo_accesser_type::s_client_get_write_concern(conn);

		yggr_test_assert(now_write_concern.var_w() == new_write_concern.var_w());
		std::cout << now_write_concern << std::endl;

		mongo_accesser_type::s_client_set_write_concern(conn, org_write_concern);
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		write_concern_type org_write_concern = mongo_accesser_type::s_client_get_write_concern(conn);
		write_concern_type new_write_concern(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		mongo_accesser_type::s_client_set_write_concern(conn, new_write_concern, *err.org_pointer());
		yggr_test_assert(!err);

		write_concern_type now_write_concern = mongo_accesser_type::s_client_get_write_concern(conn);

		yggr_test_assert(now_write_concern.var_w() == new_write_concern.var_w());
		std::cout << now_write_concern << std::endl;

		mongo_accesser_type::s_client_set_write_concern(conn, org_write_concern);
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		write_concern_type org_write_concern = mongo_accesser_type::s_client_get_write_concern(conn);
		write_concern_type new_write_concern(write_concern_type::E_MONGOC_WRITE_CONCERN_W_MAJORITY);

		mongo_accesser_type::s_client_set_write_concern(conn, new_write_concern, err);
		yggr_test_assert(!err);

		write_concern_type now_write_concern = mongo_accesser_type::s_client_get_write_concern(conn);

		yggr_test_assert(now_write_concern.var_w() == new_write_concern.var_w());
		std::cout << now_write_concern << std::endl;

		mongo_accesser_type::s_client_set_write_concern(conn, org_write_concern);
	}

	std::cout << "------test_get_set_write_concern success------" << std::endl;
}

void test_get_uri(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_mongo_host_info host_info_type;
	typedef std::list<host_info_type> host_info_list_type;

	{
		yggr::nsql_database_system::c_mongo_uri uri = mongo_accesser_type::s_client_get_uri(conn);

		yggr_test_assert(!uri.empty());

		std::cout << "uri = " << uri << std::endl;

		std::cout << "get_auth_mechanism = " << uri.auth_mechanism() << std::endl;
		std::cout << "get_auth_source = " << uri.auth_source() << std::endl;
		std::cout << "var_database = " << uri.var_database() << std::endl;
		std::cout << "var_options = " << uri.var_options() << std::endl;
		std::cout << "var_username = " << uri.var_username() << std::endl;
		std::cout << "var_passwd = " << uri.var_passwd() << std::endl;
		std::cout << "var_read_concern = " << uri.var_read_concern() << std::endl;
		std::cout << "var_read_prefs = " << uri.var_read_prefs() << std::endl;
		std::cout << "var_read_prefs = " << uri.var_read_prefs<bson_type>() << std::endl;
		std::cout << "get_replica_set = " << uri.get_replica_set() << std::endl;
		std::cout << "get_tls = " << uri.get_tls() << std::endl;
		std::cout << "var_uri_string = " << uri.var_uri_string() << std::endl;
		std::cout << "var_write_concern = " << uri.var_write_concern() << std::endl;

		host_info_list_type list;
		uri.var_hosts(list);

		std::cout << "host_info_list = " << std::endl;
		for(host_info_list_type::const_iterator i = list.begin(), isize = list.end();
			i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	std::cout << "------test_get_uri success------" << std::endl;
}

void test_get_set_ssl_opts(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef yggr::nsql_database_system::c_mongo_connection mongo_connection_type;
	typedef yggr::nsql_database_system::c_mongo_ssl_opt ssl_opt_type;
	typedef ssl_opt_type::org_type org_ssl_opt_type;
	typedef ssl_opt_type::internal_tls_opts_type internal_tls_opts_type;

	{
		ssl_opt_type ssl_opt("./nsql_database_system/cert/client.pem",
								"",
								"./nsql_database_system/cert/ca.pem",
								"./nsql_database_system/cert",
								"",
								true,
								true,
								internal_tls_opts_type(true, true));


		ssl_opt_type old_ssl_opt = mongo_accesser_type::s_client_get_ssl_opts(conn);

		yggr_test_assert(!old_ssl_opt.var_tls_disable_ocsp_endpoint_check());
		std::cout << old_ssl_opt << std::endl;

		bool bchk = false;

		bchk = mongo_accesser_type::s_client_set_ssl_opts(conn, ssl_opt.org_pointer());
		yggr_test_assert(bchk);

		ssl_opt_type new_ssl_opt = mongo_accesser_type::s_client_get_ssl_opts(conn);

		yggr_test_assert(new_ssl_opt.var_tls_disable_ocsp_endpoint_check());
		std::cout << new_ssl_opt << std::endl;

		bchk = mongo_accesser_type::s_client_set_ssl_opts(conn, old_ssl_opt);
		yggr_test_assert(bchk);
	}

	std::cout << "------test_get_set_ssl_opts success------" << std::endl;
}

//void test_set_write_concern(yggr::nsql_database_system::c_mongo_connection& conn)
//{
//	bool bsuccess
//		= yggr::nsql_database_system::mongo_accesser::s_client_set_write_concern(
//			conn, yggr::nsql_database_system::c_mongo_write_concern());
//
//
//	yggr_test_assert(bsuccess);
//	std::cout << "------test_set_write_concern success------" << std::endl;
//}

//---------

#if !defined(MONGOC_INSIDE)
#	define MONGOC_INSIDE
#endif // MONGOC_INSIDE

#include <mongoc-client-private.h>

#undef MONGOC_INSIDE

mongoc_stream_t* my_mongoc_client_stream_initiator(const mongoc_uri_t       *uri,
													const mongoc_host_list_t *host,
													void                     *user_data,
													bson_error_t             *error)
{
	std::cout << "my_mongoc_client_stream_initiator" << std::endl;
	void *ssl_opts_void = NULL;
	bool use_ssl = false;
#ifdef MONGOC_ENABLE_SSL
	mongoc_client_t *client = reinterpret_cast<mongoc_client_t*>(user_data);

	use_ssl = client->use_ssl;
	ssl_opts_void = static_cast<void*>(&client->ssl_opts);

#endif

	return
		mongoc_client_connect (
			true, use_ssl, ssl_opts_void, uri, host, error);
}

void* my_handler_user_data(yggr::nsql_database_system::c_mongo_connection::mongoc_client_ptr_type pclt, yggr::u64 n)
{
	std::cout << "my_handler_user_data " << n << std::endl;
	return static_cast<void*>(pclt.get());
}

void test_set_stream_initiator(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef std::list<yggr::string> str_list_type;
	typedef yggr::nsql_database_system::c_mongo_connection conn_type;
	typedef conn_type::mongoc_client_ptr_type client_ptr_type;

	//struct collector
	//{
	//	static void handler_collector(const char* name, std::list<yggr::string>& str_list)
	//	{
	//		yggr::utf8_string str_name(name, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//		str_list.push_back(str_name.str<yggr::string>());
	//	}
	//};

	bool bchk = false;

	{
		bchk = mongo_accesser_type::s_client_set_stream_initiator(conn, &my_mongoc_client_stream_initiator);
		yggr_test_assert(bchk);

		mongo_accesser_type::s_client_reset(conn);

		{
			str_list_type str_list;
			bool bsuccess
					= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
						conn, 0,
						boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

			yggr_test_assert(bsuccess);
		}

		bchk = mongo_accesser_type::s_client_set_stream_initiator(conn);
		yggr_test_assert(bchk);

		{
			str_list_type str_list;
			bool bsuccess
					= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
						conn, 0,
						boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

			yggr_test_assert(bsuccess);
		}
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		bchk = mongo_accesser_type::s_client_set_stream_initiator(conn, &my_mongoc_client_stream_initiator, 0, err);
		yggr_test_assert(bchk);
		yggr_test_assert(!err);

		mongo_accesser_type::s_client_reset(conn);

		{
			str_list_type str_list;
			bool bsuccess
					= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
						conn, 0,
						boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

			yggr_test_assert(bsuccess);
		}

		bchk = mongo_accesser_type::s_client_set_stream_initiator(conn);
		yggr_test_assert(bchk);

		{
			str_list_type str_list;
			bool bsuccess
					= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
						conn, 0,
						boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

			yggr_test_assert(bsuccess);
		}
	}

	{
		bchk = mongo_accesser_type::s_client_set_stream_initiator(
				conn, &my_mongoc_client_stream_initiator,
				boost::bind(&my_handler_user_data, _1, 333));
		yggr_test_assert(bchk);

		mongo_accesser_type::s_client_reset(conn);

		{
			str_list_type str_list;
			bool bsuccess
					= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
						conn, 0,
						boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

			yggr_test_assert(bsuccess);
		}

		bchk = mongo_accesser_type::s_client_set_stream_initiator(conn);
		yggr_test_assert(bchk);

		{
			str_list_type str_list;
			bool bsuccess
					= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
						conn, 0,
						boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

			yggr_test_assert(bsuccess);
		}
	}

	std::cout << "------test_set_stream_initiator success------" << std::endl;
}

//void test_set_stream_initiator(yggr::nsql_database_system::c_mongo_connection& conn)
//{
//	{
//		bool bsuccess
//			= yggr::nsql_database_system::mongo_accesser::s_client_set_stream_initiator(
//				conn, &my_mongoc_client_stream_initiator);
//
//		yggr_test_assert(bsuccess);
//	}
//
//	{
//		bool bsuccess
//			= yggr::nsql_database_system::mongo_accesser::s_client_set_stream_initiator(
//				conn, &my_mongoc_client_stream_initiator, 0);
//
//		yggr_test_assert(bsuccess);
//	}
//
//	{
//		yggr::nsql_database_system::c_bson data;
//		yggr::nsql_database_system::c_bson_error err;
//		bool bsuccess
//			= yggr::nsql_database_system::mongo_accesser::s_client_set_stream_initiator(
//				conn, &my_mongoc_client_stream_initiator, &data);
//
//		yggr_test_assert(bsuccess);
//	}
//
//	{
//		yggr::nsql_database_system::c_bson data;
//		yggr::nsql_database_system::c_bson_error err;
//		bool bsuccess
//			= yggr::nsql_database_system::mongo_accesser::s_client_set_stream_initiator(
//				conn, &my_mongoc_client_stream_initiator, data);
//
//		yggr_test_assert(bsuccess);
//	}
//
//	{
//		bool bsuccess
//			= yggr::nsql_database_system::mongo_accesser::s_client_set_stream_initiator(
//				conn, &my_mongoc_client_stream_initiator, 0, 0);
//
//		yggr_test_assert(bsuccess);
//	}
//
//	{
//		yggr::nsql_database_system::c_bson data;
//		yggr::nsql_database_system::c_bson_error err;
//		bool bsuccess
//			= yggr::nsql_database_system::mongo_accesser::s_client_set_stream_initiator(
//				conn, &my_mongoc_client_stream_initiator, &data, 0);
//
//		yggr_test_assert(bsuccess);
//	}
//
//	{
//		yggr::nsql_database_system::c_bson data;
//		yggr::nsql_database_system::c_bson_error err;
//		bool bsuccess
//			= yggr::nsql_database_system::mongo_accesser::s_client_set_stream_initiator(
//				conn, &my_mongoc_client_stream_initiator, 0, &err);
//
//		yggr_test_assert(bsuccess);
//	}
//
//	{
//		yggr::nsql_database_system::c_bson data;
//		yggr::nsql_database_system::c_bson_error err;
//		bool bsuccess
//			= yggr::nsql_database_system::mongo_accesser::s_client_set_stream_initiator(
//				conn, &my_mongoc_client_stream_initiator, &data, &err);
//
//		yggr_test_assert(bsuccess);
//	}
//
//	{
//		yggr::nsql_database_system::c_bson data;
//		yggr::nsql_database_system::c_bson_error err;
//		bool bsuccess
//			= yggr::nsql_database_system::mongo_accesser::s_client_set_stream_initiator(
//				conn, &my_mongoc_client_stream_initiator, &data, err);
//
//		yggr_test_assert(bsuccess);
//	}
//
//	{
//		yggr::nsql_database_system::c_bson data;
//		yggr::nsql_database_system::c_bson_error err;
//		bool bsuccess
//			= yggr::nsql_database_system::mongo_accesser::s_client_set_stream_initiator(
//				conn, &my_mongoc_client_stream_initiator, data, err);
//
//		yggr_test_assert(bsuccess);
//	}
//
//	std::cout << "------test_set_stream_initiator success------" << std::endl;
//}

void test_client_reset(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	{
		bool bchk = mongo_accesser_type::s_client_reset(conn);
		yggr_test_assert(bchk);
	}

	{
		bool bchk = mongo_accesser_type::s_client_reset(conn, 0);
		yggr_test_assert(bchk);
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		bool bchk = mongo_accesser_type::s_client_reset(conn, err);
		yggr_test_assert(bchk);
		yggr_test_assert(!err);
	}

	std::cout << "------test_client_reset success------" << std::endl;
}

void test_client_set_appname(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	const char* str_appname = "my_appname";

	{
		bool bchk = mongo_accesser_type::s_client_set_appname(conn, str_appname);
		yggr_test_assert(bchk);
	}

	{
		bool bchk = mongo_accesser_type::s_client_set_appname(conn, std::string(str_appname));
		yggr_test_assert(bchk);
	}

	{
		bool bchk = mongo_accesser_type::s_client_set_appname(conn, boost::container::string(str_appname));
		yggr_test_assert(bchk);
	}

	{
		bool bchk = mongo_accesser_type::s_client_set_appname(conn, yggr::string(str_appname));
		yggr_test_assert(bchk);
	}

	{
		bool bchk = mongo_accesser_type::s_client_set_appname(conn, yggr::utf8_string(str_appname));
		yggr_test_assert(bchk);
	}

	std::cout << "------test_client_set_appname success------" << std::endl;
}

void test_client_get_handshake_description(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef yggr::nsql_database_system::c_mongo_server_description svr_desc_type;

	{
		svr_desc_type svr_desc = mongo_accesser_type::s_client_get_handshake_description(conn, 3);
		yggr_test_assert(svr_desc.empty());
	}

	{
		svr_desc_type svr_desc = mongo_accesser_type::s_client_get_handshake_description(conn, 1);
		yggr_test_assert(!svr_desc.empty());
		yggr_test_assert(svr_desc.id() == 1);
		std::cout << svr_desc.id() << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		svr_desc_type svr_desc = mongo_accesser_type::s_client_get_handshake_description(conn, 1, 0);
		yggr_test_assert(!svr_desc.empty());
		yggr_test_assert(svr_desc.id() == 1);
		std::cout << svr_desc.id() << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		svr_desc_type svr_desc = mongo_accesser_type::s_client_get_handshake_description(conn, 1, err.org_pointer());
		yggr_test_assert(!svr_desc.empty());
		yggr_test_assert(svr_desc.id() == 1);
		std::cout << svr_desc.id() << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		svr_desc_type svr_desc = mongo_accesser_type::s_client_get_handshake_description(conn, 1, *err.org_pointer());
		yggr_test_assert(!svr_desc.empty());
		yggr_test_assert(svr_desc.id() == 1);
		std::cout << svr_desc.id() << std::endl;
	}

	{
		yggr::nsql_database_system::c_bson_error err;
		svr_desc_type svr_desc = mongo_accesser_type::s_client_get_handshake_description(conn, 1, err);
		yggr_test_assert(!svr_desc.empty());
		yggr_test_assert(svr_desc.id() == 1);
		std::cout << svr_desc.id() << std::endl;
	}

	std::cout << "------test_client_set_appname success------" << std::endl;
}

//typedef void (*mongoc_apm_command_started_cb_t) (
//   const mongoc_apm_command_started_t *event);
//typedef void (*mongoc_apm_command_succeeded_cb_t) (
//   const mongoc_apm_command_succeeded_t *event);
//typedef void (*mongoc_apm_command_failed_cb_t) (
//   const mongoc_apm_command_failed_t *event);
//typedef void (*mongoc_apm_server_changed_cb_t) (
//   const mongoc_apm_server_changed_t *event);
//typedef void (*mongoc_apm_server_opening_cb_t) (
//   const mongoc_apm_server_opening_t *event);
//typedef void (*mongoc_apm_server_closed_cb_t) (
//   const mongoc_apm_server_closed_t *event);
//typedef void (*mongoc_apm_topology_changed_cb_t) (
//   const mongoc_apm_topology_changed_t *event);
//typedef void (*mongoc_apm_topology_opening_cb_t) (
//   const mongoc_apm_topology_opening_t *event);
//typedef void (*mongoc_apm_topology_closed_cb_t) (
//   const mongoc_apm_topology_closed_t *event);
//typedef void (*mongoc_apm_server_heartbeat_started_cb_t) (
//   const mongoc_apm_server_heartbeat_started_t *event);
//typedef void (*mongoc_apm_server_heartbeat_succeeded_cb_t) (
//   const mongoc_apm_server_heartbeat_succeeded_t *event);
//typedef void (*mongoc_apm_server_heartbeat_failed_cb_t) (
//   const mongoc_apm_server_heartbeat_failed_t *event);

void my_mongoc_apm_command_succeeded(const mongoc_apm_command_succeeded_t* pevent)
{
	std::cout << "!!!my_mongoc_apm_command_succeeded!!!" << std::endl;
}

void my_mongoc_apm_server_opening(const mongoc_apm_server_opening_t* pevent)
{
	std::cout << "!!!my_mongoc_apm_server_opening!!!" << std::endl;
}

void my_mongoc_apm_topology_opening(const mongoc_apm_topology_opening_t* pevent)
{
	std::cout << "!!!my_mongoc_apm_topology_opening!!!" << std::endl;
}

void my_mongoc_apm_server_heartbeat_succeeded(const mongoc_apm_server_heartbeat_succeeded_t* pevent)
{
	std::cout << "!!!my_mongoc_apm_server_heartbeat_succeeded!!!" << std::endl;
}

void test_client_set_apm_callbacks(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef std::list<yggr::string> str_list_type;
	typedef yggr::nsql_database_system::c_mongo_connection conn_type;
	typedef conn_type::mongoc_client_ptr_type client_ptr_type;

	typedef yggr::nsql_database_system::c_mongo_apm_callbacks apm_callbacks_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	//struct collector
	//{
	//	static void handler_collector(const char* name, std::list<yggr::string>& str_list)
	//	{
	//		yggr::utf8_string str_name(name, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	//		str_list.push_back(str_name.str<yggr::string>());
	//	}
	//};

	{
		apm_callbacks_type apm_callbacks;
		apm_callbacks.cb_command_succeeded() = &my_mongoc_apm_command_succeeded;
		apm_callbacks.cb_server_opening() = &my_mongoc_apm_server_opening;
		apm_callbacks.cb_topology_opening() = &my_mongoc_apm_topology_opening;
		apm_callbacks.cb_server_heartbeat_succeeded() = &my_mongoc_apm_server_heartbeat_succeeded;

		{
			bool bchk = false;

			bchk = mongo_accesser_type::s_client_set_apm_callbacks(conn, apm_callbacks);
			yggr_test_assert(bchk);

			mongo_accesser_type::s_client_reset(conn);

			{
				str_list_type str_list;
				bool bsuccess
						= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
							conn, 0,
							boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

				yggr_test_assert(bsuccess);
				yggr_test_assert(str_list.size());
			}

			bchk = mongo_accesser_type::s_client_set_apm_callbacks(conn, 0, 0);
			yggr_test_assert(bchk);
			mongo_accesser_type::s_client_reset(conn);
		}

		{
			bool bchk = false;

			bchk = mongo_accesser_type::s_client_set_apm_callbacks(conn, apm_callbacks.org_pointer());
			yggr_test_assert(bchk);

			mongo_accesser_type::s_client_reset(conn);

			{
				str_list_type str_list;
				bool bsuccess
						= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
							conn, 0,
							boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

				yggr_test_assert(bsuccess);
				yggr_test_assert(str_list.size());
			}

			bchk = mongo_accesser_type::s_client_set_apm_callbacks(conn, 0, 0);
			yggr_test_assert(bchk);
			mongo_accesser_type::s_client_reset(conn);
		}

		{
			bool bchk = false;

			bchk = mongo_accesser_type::s_client_set_apm_callbacks(conn, *apm_callbacks.org_pointer());
			yggr_test_assert(bchk);

			mongo_accesser_type::s_client_reset(conn);

			{
				str_list_type str_list;
				bool bsuccess
						= yggr::nsql_database_system::mongo_accesser::s_client_get_database_names(
							conn, 0,
							boost::bind(&collector_string_list::handler_collector, _1, boost::ref(str_list)));

				yggr_test_assert(bsuccess);
				yggr_test_assert(str_list.size());
			}

			bchk = mongo_accesser_type::s_client_set_apm_callbacks(conn, 0, 0);
			yggr_test_assert(bchk);
			mongo_accesser_type::s_client_reset(conn);
		}
	}

	std::cout << "------test_client_set_apm_callbacks success------" << std::endl;
}

void test_client_set_error_api(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	{
		bool bchk = false;

		// not MONGOC_ERROR_API_VERSION_2 or MONGOC_ERROR_API_VERSION_LEGACY
		bchk = mongo_accesser_type::s_client_set_error_api(conn, 0);
		yggr_test_assert(!bchk);

		bchk = mongo_accesser_type::s_client_set_error_api(conn, MONGOC_ERROR_API_VERSION_2);
		yggr_test_assert(bchk);

		// only set once
		bchk = mongo_accesser_type::s_client_set_error_api(conn, MONGOC_ERROR_API_VERSION_LEGACY);
		yggr_test_assert(!bchk);
	}

	std::cout << "------test_client_set_error_api success------" << std::endl;
}

void test_client_set_server_api(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;
	typedef yggr::nsql_database_system::c_mongo_server_api server_api_type;
	typedef yggr::nsql_database_system::c_bson_error bson_error_type;

	{
		server_api_type svr_api;
		bson_error_type bs_err;
		svr_api.strict_value() = true;
		bool bchk = false;

		bchk = mongo_accesser_type::s_client_set_server_api(conn, svr_api, bs_err);
		yggr_test_assert(bchk);

		// set once only
		bchk = mongo_accesser_type::s_client_set_server_api(conn, svr_api.org_pointer());
		yggr_test_assert(!bchk);

		bchk = mongo_accesser_type::s_client_set_server_api(conn, *svr_api.org_pointer(), bs_err.org_pointer());
		yggr_test_assert(!bchk);

		bchk = mongo_accesser_type::s_client_set_server_api(conn, *svr_api.org_pointer(), *bs_err.org_pointer());
		yggr_test_assert(!bchk);

	}

	std::cout << "------test_client_set_server_api success------" << std::endl;
}

// test crtpt s

void test_client_get_crypt_shared_version(yggr::nsql_database_system::c_mongo_connection& conn)
{
	typedef yggr::nsql_database_system::c_bson_error error_type;
	typedef yggr::nsql_database_system::mongo_accesser mongo_accesser_type;

	// s_client_get_crypt_shared_version return inner_string_view
	{
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version(conn);
		yggr_test_assert(str_shared_ver.empty());
	}

	{
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version(conn, &err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// s_client_get_crypt_shared_version return inner_string alloc
	{
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator());
		yggr_test_assert(str_shared_ver.empty());
	}

	{
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator(), &err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator(), err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version(conn, yggr::utf8_string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// s_client_get_crypt_shared_version return inner_string or string
	// inner_string
	{
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn);
		yggr_test_assert(str_shared_ver.empty());
	}

	{
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn, &err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn, err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::utf8_string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::utf8_string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// string
	{
		yggr::string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn);
		yggr_test_assert(str_shared_ver.empty());
	}

	{
		yggr::string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, &err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	// string allocator
	{
		yggr::string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator());
		yggr_test_assert(str_shared_ver.empty());
	}

	{
		yggr::string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str_shared_ver.empty());
	}

	{
		error_type err;
		yggr::string str_shared_ver = mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator(), &err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), &err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator(), err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	{
		error_type err;
		yggr::string str_shared_ver =
			mongo_accesser_type::s_client_get_crypt_shared_version<yggr::string>(conn, yggr::string().get_allocator(), YGGR_STR_UTF8_STRING_CHARSET_NAME(), err);
		yggr_test_assert(str_shared_ver.empty());
		yggr_test_assert(!err);
	}

	yggr_test_assert(!mongo_accesser_type::s_client_is_crypt_enabled(conn));

	// not_test_end wait fix mongodb_crypt_shared

	std::cout << "------test_client_get_crypt_shared_version success------" << std::endl;
}

// test crtpt e

void test_other_foo(void)
{

#if MONGODB_USING_CA()

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://xy:123456abc@127.0.0.1:10398/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);
#	else
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://127.0.0.1:10298/?tls=true",	// str_uri
				"./nsql_database_system/cert/client.pem",		// pem_file
				"",										// pem_pwd
				"./nsql_database_system/cert/ca.pem",	// ca_file
				"./nsql_database_system/cert",				// ca_dir
				""										// crl_file
			);

#	endif // MONGODB_USING_AUTH

#else

#	if MONGODB_USING_AUTH()
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://xy:123456abc@127.0.0.1:10198/?tls=false"	// str_uri
			);
#	else
	yggr::nsql_database_system::c_mongo_connection client(
				"mongodb://127.0.0.1:10098/?tls=false"	// str_uri
			);
#	endif // MONGODB_USING_AUTH

#endif // MONGODB_USING_CA

	if(!client)
	{
		std::cerr << "Failed to parse URI." << std::endl;
		client.clear();
		return;
	}

	test_get_database_names(client);
	test_get_server_status(client);

	test_get_server_description(client);
	test_get_server_descriptions(client);
	test_select_server(client);

	test_get_set_read_concern(client);
	test_get_set_read_prefs(client);
	test_get_set_write_concern(client);

	test_get_uri(client);
	test_get_set_ssl_opts(client);

	test_set_stream_initiator(client);

	test_client_reset(client);
	test_get_database_names(client); // help test test_client_reset

	test_client_set_appname(client);
	test_client_get_handshake_description(client);

	test_client_set_apm_callbacks(client);

	test_client_get_crypt_shared_version(client);

	test_client_set_error_api(client);
	test_client_set_server_api(client);

	client.clear();

	std::cout << "---------test_other_foo end--------------" << std::endl;
}

int main(int argc, char *argv[])
{
	yggr::nsql_database_system::mongodb_installer::install();

	test_collections();
	test_database();
	test_client_foo();
	test_gridfs();
	test_gridfs_bucket();

	test_other_foo();

	yggr::nsql_database_system::mongodb_installer::uninstall();

	std::cout << "--------------all end-----------------" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
