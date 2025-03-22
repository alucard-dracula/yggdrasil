// c_bson_dbpointer_test.cpp

#include <iostream>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson_oid.hpp>
#include <yggr/nsql_database_system/c_bson_dbpointer.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
	typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> bson_dbpointer_type;
	typedef bson_oid_type::base_type bson_base_oid_type;
	typedef yggr::nsql_database_system::c_bson_dbpointer<bson_base_oid_type> bson_base_dbpointer_type;
	
	{
		{
			bson_dbpointer_type data;

			std::cout << data << std::endl;

			yggr_test_assert(data.collection() == "");
			yggr_test_assert(data.oid());
		}

		{
			bson_base_dbpointer_type data;

			std::cout << data << std::endl;

			yggr_test_assert(data.collection() == "");
			yggr_test_assert(data.oid<bson_oid_type>());
		}
	}

	{
		{
			bson_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
			bson_value_type org_value(org_data);
			bson_dbpointer_type data(org_value);

			std::cout << data << std::endl;

			yggr_test_assert(data.collection() == "dbpointer");
			yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900aa"));
			yggr_test_assert(data == org_data);
		}

		{
			bson_base_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
			bson_value_type org_value(org_data);
			bson_base_dbpointer_type data(org_value);

			std::cout << data << std::endl;

			yggr_test_assert(data.collection() == "dbpointer");
			yggr_test_assert(data.oid<bson_oid_type>() == bson_oid_type("0011223344556677889900aa"));
			yggr_test_assert(data == org_data);
		}
	}

	{
		{
			bson_base_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
			bson_dbpointer_type data(org_data);

			std::cout << data << std::endl;

			yggr_test_assert(data.collection() == "dbpointer");
			yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900aa"));
			yggr_test_assert(data == org_data);
		}

		{
			bson_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
			bson_base_dbpointer_type data(org_data);

			std::cout << data << std::endl;

			yggr_test_assert(data.collection() == "dbpointer");
			yggr_test_assert(data.oid<bson_oid_type>() == bson_oid_type("0011223344556677889900aa"));
			yggr_test_assert(data == org_data);
		}
	}

	{
		bson_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
		bson_dbpointer_type data(boost::move(org_data));

		std::cout << data << std::endl;

		yggr_test_assert(data.collection() == "dbpointer");
		yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900aa"));
		//yggr_test_assert(data != org_data); // only using move test
	}

	{
		bson_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
		bson_dbpointer_type data(org_data);

		std::cout << data << std::endl;

		yggr_test_assert(data.collection() == "dbpointer");
		yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900aa"));
		yggr_test_assert(data == org_data);
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
	typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> bson_dbpointer_type;
	typedef bson_oid_type::base_type bson_base_oid_type;
	typedef yggr::nsql_database_system::c_bson_dbpointer<bson_base_oid_type> bson_base_dbpointer_type;

	{
		{
			bson_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
			bson_value_type org_value(org_data);
			bson_dbpointer_type data;
			data = org_value;

			std::cout << data << std::endl;

			yggr_test_assert(data.collection() == "dbpointer");
			yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900aa"));
			yggr_test_assert(data == org_data);
		}

		{
			bson_base_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
			bson_value_type org_value(org_data);
			bson_base_dbpointer_type data;
			data = org_value;

			std::cout << data << std::endl;

			yggr_test_assert(data.collection() == "dbpointer");
			yggr_test_assert(data.oid<bson_oid_type>() == bson_oid_type("0011223344556677889900aa"));
			yggr_test_assert(data == org_data);
		}
	}

	{
		{
			bson_base_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
			bson_dbpointer_type data;
			data = org_data;

			std::cout << data << std::endl;

			yggr_test_assert(data.collection() == "dbpointer");
			yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900aa"));
			yggr_test_assert(data == org_data);
		}

		{
			bson_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
			bson_base_dbpointer_type data;
			data = org_data;

			std::cout << data << std::endl;

			yggr_test_assert(data.collection() == "dbpointer");
			yggr_test_assert(data.oid<bson_oid_type>() == bson_oid_type("0011223344556677889900aa"));
			yggr_test_assert(data == org_data);
		}
	}

	{
		bson_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
		bson_dbpointer_type data;
		data = boost::move(org_data);

		std::cout << data << std::endl;

		yggr_test_assert(data.collection() == "dbpointer");
		yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900aa"));
		//yggr_test_assert(data != org_data); // only using move test
	}

	{
		bson_dbpointer_type org_data("dbpointer", bson_oid_type("0011223344556677889900aa"));
		bson_dbpointer_type data;
		data = org_data;

		std::cout << data << std::endl;

		yggr_test_assert(data.collection() == "dbpointer");
		yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900aa"));
		yggr_test_assert(data == org_data);
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
	typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> bson_dbpointer_type;
	typedef bson_oid_type::base_type bson_base_oid_type;
	typedef yggr::nsql_database_system::c_bson_dbpointer<bson_base_oid_type> bson_base_dbpointer_type;

	bson_dbpointer_type org_data1("dbpointer", bson_oid_type("0011223344556677889900aa"));
	bson_dbpointer_type org_data2("dbpointer2", bson_oid_type("0011223344556677889900bb"));

	{
		bson_dbpointer_type data1(org_data1);
		bson_dbpointer_type data2(org_data2);

		data1.swap(data2);
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		std::swap(data1, data2);
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		boost::swap(data1, data2);
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		yggr::swap(data1, data2);
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);
	}

	{
		bson_base_dbpointer_type data1(org_data1);
		bson_base_dbpointer_type data2(org_data2);

		data1.swap(data2);
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		std::swap(data1, data2);
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		boost::swap(data1, data2);
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		yggr::swap(data1, data2);
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);
	}

	{
		bson_dbpointer_type data1(org_data1);
		bson_base_dbpointer_type data2(org_data2);

		data1.swap(data2);
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		std::swap(data1, data2);
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		boost::swap(data1, data2);
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		yggr::swap(data1, data2);
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);
	}

	{
		bson_base_dbpointer_type data1(org_data1);
		bson_dbpointer_type data2(org_data2);

		data1.swap(data2);
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		std::swap(data1, data2);
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		boost::swap(data1, data2);
		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		yggr::swap(data1, data2);
		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;
	typedef yggr::nsql_database_system::c_bson_oid bson_oid_type;
	typedef yggr::nsql_database_system::c_bson_dbpointer<bson_oid_type> bson_dbpointer_type;
	typedef bson_oid_type::base_type bson_base_oid_type;
	typedef yggr::nsql_database_system::c_bson_dbpointer<bson_base_oid_type> bson_base_dbpointer_type;

	// value_typeid
	{
		bson_dbpointer_type data;

		yggr_test_assert(bson_dbpointer_type::value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_DBPOINTER);
		yggr_test_assert(bson_dbpointer_type::s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_DBPOINTER);
		yggr_test_assert(data.value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_DBPOINTER);
		yggr_test_assert(data.s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_DBPOINTER);
	}

	// collection
	{
		bson_dbpointer_type data("dbpointer", bson_oid_type("0011223344556677889900aa"));
	
		std::cout << data.collection() << std::endl;
		
		yggr_test_assert(data.collection() == "dbpointer");
		yggr_test_assert(data.collection<yggr::utf8_string>() == "dbpointer");

		yggr_test_assert(data.collection<yggr::string>() == "dbpointer");
		yggr_test_assert(data.collection<boost::container::string>() == "dbpointer");
		yggr_test_assert(data.collection<std::string>() == "dbpointer");

		yggr_test_assert(data.collection<yggr::wstring>() == L"dbpointer");
		yggr_test_assert(data.collection<boost::container::wstring>() == L"dbpointer");
		yggr_test_assert(data.collection<std::wstring>() == L"dbpointer");

		data.collection() = "dbpointer2";
		yggr_test_assert(data.collection() == "dbpointer2");
	}

	// oid
	{
		{
			bson_dbpointer_type data("dbpointer", bson_oid_type("0011223344556677889900aa"));

			yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900aa"));

			data.oid() = bson_oid_type("0011223344556677889900bb");

			yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900bb"));
			yggr_test_assert(data.oid<bson_base_oid_type>() == bson_oid_type("0011223344556677889900bb"));
		}

		{
			bson_base_dbpointer_type data("dbpointer", bson_oid_type("0011223344556677889900aa"));

			yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900aa"));

			data.oid() = bson_oid_type("0011223344556677889900bb");

			yggr_test_assert(data.oid() == bson_oid_type("0011223344556677889900bb"));
			yggr_test_assert(data.oid<bson_oid_type>() == bson_oid_type("0011223344556677889900bb"));
		}
	}

	// == !=
	{
		bson_dbpointer_type data("dbpointer", bson_oid_type("0011223344556677889900aa"));
		bson_dbpointer_type data_same = data;
		bson_dbpointer_type data_diff("dbpointer2", bson_oid_type("0011223344556677889900bb"));

		bson_value_type bs_val_same(data_same);
		bson_value_type bs_val_diff(data_diff);

		bson_value_type::base_type& base_bs_val_same(bs_val_same);
		bson_value_type::base_type& base_bs_val_diff(bs_val_diff);

		// ==
		yggr_test_assert((data == data));
		yggr_test_assert((data == data_same));
		yggr_test_assert(!(data == data_diff));

		yggr_test_assert((data == bs_val_same));
		yggr_test_assert((bs_val_same == data));
		yggr_test_assert(!(data == bs_val_diff));
		yggr_test_assert(!(bs_val_diff == data));

		yggr_test_assert((data == base_bs_val_same));
		yggr_test_assert((base_bs_val_same == data));
		yggr_test_assert(!(data == base_bs_val_diff));
		yggr_test_assert(!(base_bs_val_diff == data));

		// !=
		yggr_test_assert(!(data != data));
		yggr_test_assert(!(data != data_same));
		yggr_test_assert((data != data_diff));

		yggr_test_assert(!(data != bs_val_same));
		yggr_test_assert(!(bs_val_same != data));
		yggr_test_assert((data != bs_val_diff));
		yggr_test_assert((bs_val_diff != data));

		yggr_test_assert(!(data != base_bs_val_same));
		yggr_test_assert(!(base_bs_val_same != data));
		yggr_test_assert((data != base_bs_val_diff));
		yggr_test_assert((base_bs_val_diff != data));
	}

	// output
	{
		bson_dbpointer_type data("dbpointer", bson_oid_type("0011223344556677889900aa"));

		std::cout << data << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data) << std::endl;
	}

	{
		bson_dbpointer_type data("dbpointer", bson_oid_type("0011223344556677889900aa"));

		std::string chk_str1 = "{ \"$ref\" : \"dbpointer\", \"$id\" : \"0011223344556677889900aa\" }";
		std::string chk_str2 = "{ \"$dbPointer\" : { \"$ref\" : \"dbpointer\", \"$id\" : \"0011223344556677889900aa\" } }";

		{
			std::stringstream ss;
			ss << data;
			yggr_test_assert(ss.str() == chk_str1);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data);
			yggr_test_assert(ss.str() == chk_str1);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data);
			yggr_test_assert(ss.str() == chk_str2);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data);
			yggr_test_assert(ss.str() == chk_str2);
		}
	}

	{
		bson_type bs;
		bson_dbpointer_type data("dbpointer", bson_oid_type("0011223344556677889900aa"));
		bs.save("c_bson_dbpointer", data);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		bson_dbpointer_type ld_data;
		bson_type::iterator bs_iter(bs, "c_bson_dbpointer");
		bs_iter.load("c_bson_dbpointer", ld_data);

		yggr_test_assert(ld_data == data);
	}

	std::cout << "-----------------test_member_foo end-----------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_member_foo();

	wait_any_key(argc, argv);
	return 0;
}