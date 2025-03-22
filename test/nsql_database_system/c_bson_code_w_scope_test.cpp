// c_bson_code_w_scope_test.cpp

#include <iostream>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson_code_w_scope.hpp>

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
	typedef yggr::nsql_database_system::c_bson_code_w_scope<bson_type> bson_code_w_scope_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_code_w_scope_type data;

		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "");
	}

	{
		bson_code_w_scope_type org_data("this is a const char* cws",
										yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

		bson_value_type org_value(org_data);

		bson_code_w_scope_type data(org_value);

		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a const char* cws");
		yggr_test_assert(data.scope().get_value<yggr::s32>("aaa") == 1);
	}

	{
		bson_code_w_scope_type data("this is a const char* cws",
										yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a const char* cws");
		yggr_test_assert(data.scope().get_value<yggr::s32>("aaa") == 1);
	}

	{
		yggr::utf8_string code = "this is a cws";
		yggr::nsql_database_system::c_bson scope = "{\"aaa\" : 1}";
		bson_code_w_scope_type data(boost::move(code), scope);

		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a cws");
		yggr_test_assert(data.scope().get_value<yggr::s32>("aaa") == 1);
	}

	{
		yggr::utf8_string code = "this is a cws";
		yggr::nsql_database_system::c_bson scope = "{\"aaa\" : 1}";
		bson_code_w_scope_type org_data(boost::move(code), scope);
		bson_code_w_scope_type data(boost::move(org_data));

		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a cws");
		yggr_test_assert(data.scope().get_value<yggr::s32>("aaa") == 1);
	}

	{
		yggr::utf8_string code = "this is a cws";
		yggr::nsql_database_system::c_bson scope = "{\"aaa\" : 1}";
		bson_code_w_scope_type org_data(boost::move(code), scope);
		bson_code_w_scope_type data(org_data);

		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a cws");
		yggr_test_assert(data.scope().get_value<yggr::s32>("aaa") == 1);
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_code_w_scope<bson_type> bson_code_w_scope_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_code_w_scope_type org_data("this is a cws",
										yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

		bson_value_type org_value(org_data);
		bson_code_w_scope_type data;
		data = org_value;

		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a cws");
		yggr_test_assert(data.scope().get_value<yggr::s32>("aaa") == 1);
	}

	{
		bson_code_w_scope_type org_data("this is a cws",
										yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

		bson_code_w_scope_type data = boost::move(org_data);

		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a cws");
		yggr_test_assert(data.scope().get_value<yggr::s32>("aaa") == 1);
	}

	{
		bson_code_w_scope_type org_data("this is a cws",
										yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

		bson_code_w_scope_type data = org_data;

		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a cws");
		yggr_test_assert(data.scope().get_value<yggr::s32>("aaa") == 1);
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_code_w_scope<bson_type> bson_code_w_scope_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_code_w_scope_type org_data1("this is a cws",
										yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));
		bson_code_w_scope_type org_data2("this is a cws",
										yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

		bson_code_w_scope_type data1 = org_data1;
		bson_code_w_scope_type data2 = org_data2;

		yggr::swap(data1, boost::move(data2));

		yggr_test_assert(org_data1 == data2);
		yggr_test_assert(org_data2 == data1);
	}

	{
		bson_code_w_scope_type org_data1("this is a cws",
										yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));
		bson_code_w_scope_type org_data2("this is a cws",
										yggr::nsql_database_system::c_bson("{\"aaa\" : 1}"));

		bson_code_w_scope_type data1 = org_data1;
		bson_code_w_scope_type data2 = org_data2;

		yggr::swap(data1, data2);

		yggr_test_assert(org_data1 == data2);
		yggr_test_assert(org_data2 == data1);
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_code_w_scope<bson_type> bson_code_w_scope_type;
	typedef yggr::nsql_database_system::c_bson_value c_bson_value_type;

	// value_typeid
	{
		bson_code_w_scope_type data;

		yggr_test_assert(bson_code_w_scope_type::value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_CODEWSCOPE);
		yggr_test_assert(bson_code_w_scope_type::s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_CODEWSCOPE);
		yggr_test_assert(data.value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_CODEWSCOPE);
		yggr_test_assert(data.s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_CODEWSCOPE);
	}

	// code
	{
		bson_code_w_scope_type data("this is a cws",
									yggr::nsql_database_system::c_bson("{\"scope\" : 1}"));

		std::cout << data.code() << std::endl;
		
		yggr_test_assert(data.code() == "this is a cws");
		yggr_test_assert(data.code<yggr::utf8_string>() == "this is a cws");

		yggr_test_assert(data.code<yggr::string>() == "this is a cws");
		yggr_test_assert(data.code<boost::container::string>() == "this is a cws");
		yggr_test_assert(data.code<std::string>() == "this is a cws");

		yggr_test_assert(data.code<yggr::wstring>() == L"this is a cws");
		yggr_test_assert(data.code<boost::container::wstring>() == L"this is a cws");
		yggr_test_assert(data.code<std::wstring>() == L"this is a cws");

		data.code() = "this is a cws2";
		yggr_test_assert(data.code() == "this is a cws2");
	}

	// scope
	{
		bson_code_w_scope_type data("this is a cws",
									yggr::nsql_database_system::c_bson("{\"scope\" : 1}"));

		yggr_test_assert(data.scope() == yggr::nsql_database_system::c_bson("{\"scope\" : 1}"));

		bson_type::iterator iter = data.scope().find("scope");
		yggr_test_assert(iter != data.scope().end());

		{
			yggr::nsql_database_system::c_bson tmp_scope("{\"scope\" : 2}");
			data.scope().swap(tmp_scope);
		}

		yggr_test_assert(data.scope() == yggr::nsql_database_system::c_bson("{\"scope\" : 2}"));
	}

	//// == !=
	{
		bson_code_w_scope_type data("this is a cws",
											yggr::nsql_database_system::c_bson("{\"scope\" : 1}"));

		bson_code_w_scope_type data_same = data;

		
		bson_code_w_scope_type data_diff("this is a cws2",
											yggr::nsql_database_system::c_bson("{\"scope\" : 2}"));

		c_bson_value_type bs_val_same(data_same);
		c_bson_value_type bs_val_diff(data_diff);

		c_bson_value_type::base_type& base_bs_val_same(bs_val_same);
		c_bson_value_type::base_type& base_bs_val_diff(bs_val_diff);


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
		bson_code_w_scope_type data("this is a cws",
									yggr::nsql_database_system::c_bson("{\"scope\" : 1}"));

		std::cout << data << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data) << std::endl;
	}

	{
		bson_code_w_scope_type data("this is a cws",
									yggr::nsql_database_system::c_bson("{\"scope\" : 1}"));

		std::string chk_str = "{ \"$code\" : \"this is a cws\", \"$scope\" : { \"scope\" : 1 } }";

		{
			std::stringstream ss;
			ss << data;
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data);
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data);
			yggr_test_assert(ss.str() == chk_str);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data);
			yggr_test_assert(ss.str() == chk_str);
		}
	}

	{
		bson_type bs;
		bson_code_w_scope_type data("this is a cws",
									yggr::nsql_database_system::c_bson("{\"scope\" : 1}"));
		bs.save("c_bson_code_w_scope", data);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		bson_code_w_scope_type ld_data;
		bson_type::iterator bs_iter(bs, "c_bson_code_w_scope");
		bs_iter.load("c_bson_code_w_scope", ld_data);

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