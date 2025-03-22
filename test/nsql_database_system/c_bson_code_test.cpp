// c_bson_code_test.cpp

#include <iostream>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson_code.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	{
		yggr::nsql_database_system::c_bson_code data;
		std::cout << data << std::endl;
		
		yggr_test_assert(data.code() == "");
	}

	{
		yggr::nsql_database_system::c_bson_value val(yggr::nsql_database_system::c_bson_code("this is a bson value"));
		yggr::nsql_database_system::c_bson_code data = val;

		yggr_test_assert(data.code() == "this is a bson value");
	}

	{
		yggr::nsql_database_system::c_bson_code data("this is a const char* code");
		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a const char* code");
	}

	{
		yggr::nsql_database_system::c_bson_code data("this is a const char* code");
		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a const char* code");
	}

	{
		yggr::nsql_database_system::c_bson_code data(yggr::string("this is a yggr::string code"));
		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a yggr::string code");
	}

	{
		yggr::nsql_database_system::c_bson_code data(yggr::charset::make_string_charset_helper(
														"this is a string_charset_helper code"));
		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a string_charset_helper code");
	}

	{
		yggr::utf8_string str("this is a yggr::utf8_string&& code");
		yggr::nsql_database_system::c_bson_code data(boost::move(str));
		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a yggr::utf8_string&& code");
	}

	{
		yggr::nsql_database_system::c_bson_code data(yggr::utf8_string("this is a yggr::utf8_string code"));
		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a yggr::utf8_string code");
	}

	{
		yggr::nsql_database_system::c_bson_code org_data("this is a c_bson_code&& code");
		yggr::nsql_database_system::c_bson_code data(boost::move(org_data));
		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a c_bson_code&& code");
	}

	{
		yggr::nsql_database_system::c_bson_code org_data("this is a c_bson_code code");
		yggr::nsql_database_system::c_bson_code data(org_data);
		std::cout << data << std::endl;

		yggr_test_assert(data.code() == "this is a c_bson_code code");
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	{
		yggr::nsql_database_system::c_bson_value val(yggr::nsql_database_system::c_bson_code("this is a bson value"));
		yggr::nsql_database_system::c_bson_code data;
		data = val;

		yggr_test_assert(data.code() == "this is a bson value");
	}

	{
		yggr::nsql_database_system::c_bson_code data;
		data = "this is a const char* value";

		yggr_test_assert(data.code() == "this is a const char* value");
	}

	{
		yggr::nsql_database_system::c_bson_code data;
		data = "this is a bson_code value";

		yggr_test_assert(data.code() == "this is a bson_code value");
	}

	{
		yggr::nsql_database_system::c_bson_code data;
		data = yggr::string("this is a bson_code value");

		yggr_test_assert(data.code() == "this is a bson_code value");
	}

	{
		yggr::nsql_database_system::c_bson_code data;
		data = yggr::charset::make_string_charset_helper("this is a bson_code value");

		yggr_test_assert(data.code() == "this is a bson_code value");
	}

	{
		yggr::nsql_database_system::c_bson_code data;
		data = yggr::utf8_string("this is a bson_code value");

		yggr_test_assert(data.code() == "this is a bson_code value");
	}

	{
		yggr::nsql_database_system::c_bson_code org_data("this is a bson_code value");
		yggr::nsql_database_system::c_bson_code data;
		data = boost::move(org_data);

		yggr_test_assert(data.code() == "this is a bson_code value");
	}

	{
		yggr::nsql_database_system::c_bson_code org_data("this is a bson_code value");
		yggr::nsql_database_system::c_bson_code data;
		data = org_data;

		yggr_test_assert(data.code() == "this is a bson_code value");
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	{
		yggr::nsql_database_system::c_bson_code org_data1("this is a bson_code 1");
		yggr::nsql_database_system::c_bson_code org_data2("this is a bson_code 2");

		yggr::nsql_database_system::c_bson_code data1 = org_data1;
		yggr::nsql_database_system::c_bson_code data2 = org_data2;
		
		data1.swap(boost::move(data2));

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		yggr::swap(data1, boost::move(data2));

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);
	}

	{
		yggr::nsql_database_system::c_bson_code org_data1("this is a bson_code 1");
		yggr::nsql_database_system::c_bson_code org_data2("this is a bson_code 2");

		yggr::nsql_database_system::c_bson_code data1 = org_data1;
		yggr::nsql_database_system::c_bson_code data2 = org_data2;
		
		data1.swap(data2);

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
	typedef yggr::nsql_database_system::c_bson_code c_bson_code_type;
	typedef yggr::nsql_database_system::c_bson_value c_bson_value_type;

	// value_typeid
	{
		c_bson_code_type data;

		yggr_test_assert(c_bson_code_type::value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_CODE);
		yggr_test_assert(c_bson_code_type::s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_CODE);
		yggr_test_assert(data.value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_CODE);
		yggr_test_assert(data.s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_CODE);
	}

	// code
	{
		c_bson_code_type data("this is a bson_code");

		yggr_test_assert(data.code() == "this is a bson_code");
		yggr_test_assert(data.code<yggr::utf8_string>() == "this is a bson_code");

		yggr_test_assert(data.code<yggr::string>() == "this is a bson_code");
		yggr_test_assert(data.code<boost::container::string>() == "this is a bson_code");
		yggr_test_assert(data.code<std::string>() == "this is a bson_code");

		yggr_test_assert(data.code<yggr::wstring>() == L"this is a bson_code");
		yggr_test_assert(data.code<boost::container::wstring>() == L"this is a bson_code");
		yggr_test_assert(data.code<std::wstring>() == L"this is a bson_code");

		data.code() = "this is a bson_code2";

		yggr_test_assert(data.code() == "this is a bson_code2");
	}

	// == !=
	{
		c_bson_code_type data("this is a bson_code");
		c_bson_code_type data_same(data);
		c_bson_code_type data_diff("this is a bson_code_2");

		c_bson_value_type bs_val_same(data_same);
		c_bson_value_type bs_val_diff(data_diff);

		c_bson_value_type::base_type& base_bs_val_same(bs_val_same);
		c_bson_value_type::base_type& base_bs_val_diff(bs_val_diff);

		std::cout << base_bs_val_same << std::endl;

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

	// out_put
	{
		c_bson_code_type data("this is a bson_code");
		std::cout << data << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data) << std::endl;
	}

	{
		c_bson_code_type data("this is a bson_code");

		std::string chk_str = "{ \"$code\" : \"this is a bson_code\" }";

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
		c_bson_code_type data("this is a bson_code");
		bs.save("c_bson_code", data);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		c_bson_code_type ld_data;
		bson_type::iterator bs_iter(bs, "c_bson_code");
		bs_iter.load("c_bson_code", ld_data);

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