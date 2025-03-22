// c_bson_decimal128_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>

#include <yggr/nsql_database_system/c_bson_decimal128.hpp>

#include <iostream>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include <yggr/compile_link/linker.hpp>

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_bson_decimal128 bson_decimal128_type;

	{
		bson_decimal128_type data;

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 0);
		yggr_test_assert(data.var_low() == 0);
	}

	{
		bson_decimal128_type data = 1000;

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 0);
		yggr_test_assert(data.var_low() == 1000);
	}

	{
		bson_decimal128_type data(1000);

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 0);
		yggr_test_assert(data.var_low() == 1000);
	}

	{
		bson_decimal128_type data(0, 1000);

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 0);
		yggr_test_assert(data.var_low() == 1000);
	}

	{
		bson_decimal128_type org_data(1, 0);
		bson_value_type org_val(org_data);
		bson_decimal128_type data(org_val);

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
	}

	{
		bson_decimal128_type org_data(1, 0);
		bson_decimal128_type data(boost::move(static_cast<bson_decimal128_type::org_type&>(org_data)));

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
	}

	{
		bson_decimal128_type org_data(1, 0);
		bson_decimal128_type data(static_cast<const bson_decimal128_type::org_type&>(org_data));

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
	}

	{
		bson_decimal128_type org_data(1, 0);
		bson_decimal128_type data(boost::move(org_data));

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
	}

	{
		bson_decimal128_type org_data(1, 0);
		bson_decimal128_type data(org_data);

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_bson_decimal128 bson_decimal128_type;


	{
		bson_decimal128_type data;
		data = (1000);

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 0);
		yggr_test_assert(data.var_low() == 1000);
	}

	{
		bson_decimal128_type org_data(1, 0);
		bson_value_type org_val(org_data);
		bson_decimal128_type data;
		data = (org_val);

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
	}

	{
		bson_decimal128_type::org_type org_data = {1, 0};
		bson_decimal128_type data;
		data = (boost::move(static_cast<bson_decimal128_type::org_type&>(org_data)));

		std::cout << data << std::endl;
#if BSON_BYTE_ORDER == BSON_LITTLE_ENDIAN
		yggr_test_assert(data.var_high() == 0);
		yggr_test_assert(data.var_low() == 1);
#elif BSON_BYTE_ORDER == BSON_BIG_ENDIAN
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
#endif // BSON_BYTE_ORDER == BSON_BIG_ENDIAN
	}

	{
		bson_decimal128_type::org_type org_data = {1, 0};
		bson_decimal128_type data;
		data = org_data;

		std::cout << data << std::endl;
#if BSON_BYTE_ORDER == BSON_LITTLE_ENDIAN
		yggr_test_assert(data.var_high() == 0);
		yggr_test_assert(data.var_low() == 1);
#elif BSON_BYTE_ORDER == BSON_BIG_ENDIAN
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
#endif // BSON_BYTE_ORDER == BSON_BIG_ENDIAN
	}

	{
		bson_decimal128_type org_data(1, 0);
		bson_decimal128_type data;
		data = (boost::move(static_cast<bson_decimal128_type::org_type&>(org_data)));

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
	}

	{
		bson_decimal128_type org_data(1, 0);
		bson_decimal128_type data;
		data = (static_cast<const bson_decimal128_type::org_type&>(org_data));

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
	}

	{
		bson_decimal128_type org_data(1, 0);
		bson_decimal128_type data;
		data = (boost::move(org_data));

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
	}

	{
		bson_decimal128_type org_data(1, 0);
		bson_decimal128_type data;
		data = (org_data);

		std::cout << data << std::endl;
		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 0);
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_bson_decimal128 bson_decimal128_type;

	{
		bson_decimal128_type org_data1(1000);
		bson_decimal128_type org_data2(2000);

		bson_decimal128_type data1(org_data1);
		bson_decimal128_type data2(org_data2);

		data1.swap(boost::move(data2));

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		data1.swap(data2);

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		data1.swap(boost::move(static_cast<bson_decimal128_type::org_type&>(data2)));

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		data1.swap(static_cast<bson_decimal128_type::org_type&>(data2));

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);
	}

	{
		bson_decimal128_type org_data1(1000);
		bson_decimal128_type org_data2(2000);

		bson_decimal128_type data1(org_data1);
		bson_decimal128_type data2(org_data2);

		std::swap(data1, boost::move(data2));

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		std::swap(boost::move(data1), data2);

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		std::swap(data1, data2);

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		std::swap(data1, boost::move(static_cast<bson_decimal128_type::org_type&>(data2)));

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		std::swap(boost::move(static_cast<bson_decimal128_type::org_type&>(data1)), data2);

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		std::swap(
			static_cast<bson_decimal128_type::org_type&>(data1),
			static_cast<bson_decimal128_type::org_type&>(data2));

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);
	}

	{
		bson_decimal128_type org_data1(1000);
		bson_decimal128_type org_data2(2000);

		bson_decimal128_type data1(org_data1);
		bson_decimal128_type data2(org_data2);

		boost::swap(data1, boost::move(data2));

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		boost::swap(boost::move(data1), data2);

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		boost::swap(data1, data2);

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		boost::swap(data1, boost::move(static_cast<bson_decimal128_type::org_type&>(data2)));

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		boost::swap(boost::move(static_cast<bson_decimal128_type::org_type&>(data1)), data2);

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		boost::swap(
			static_cast<bson_decimal128_type::org_type&>(data1),
			static_cast<bson_decimal128_type::org_type&>(data2));

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);
	}

	{
		bson_decimal128_type org_data1(1000);
		bson_decimal128_type org_data2(2000);

		bson_decimal128_type data1(org_data1);
		bson_decimal128_type data2(org_data2);

		yggr::swap(data1, boost::move(data2));

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		yggr::swap(boost::move(data1), data2);

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		yggr::swap(data1, data2);

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		yggr::swap(data1, boost::move(static_cast<bson_decimal128_type::org_type&>(data2)));

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);

		yggr::swap(boost::move(static_cast<bson_decimal128_type::org_type&>(data1)), data2);

		yggr_test_assert(data1 == org_data2);
		yggr_test_assert(data2 == org_data1);

		yggr::swap(
			static_cast<bson_decimal128_type::org_type&>(data1),
			static_cast<bson_decimal128_type::org_type&>(data2));

		yggr_test_assert(data1 == org_data1);
		yggr_test_assert(data2 == org_data2);
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

//#include <yggr/time/time_basic_foo.hpp>

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	typedef yggr::nsql_database_system::c_bson_decimal128 bson_decimal128_type;

	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;
	typedef yggr::nsql_database_system::bson_typeid_def bson_typeid_def_type;

	{
		yggr_test_assert(bson_decimal128_type::s_value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DECIMAL128);
		yggr_test_assert(bson_decimal128_type().value_typeid() == bson_typeid_def_type::E_BSON_TYPE_DECIMAL128);
	}

	{
		bson_decimal128_type data(1, 1000);

		yggr_test_assert(data.var_high() == 1);
		yggr_test_assert(data.var_low() == 1000);
	}

	{
		bson_decimal128_type data(1000);

		std::cout << data.str() << std::endl;

		yggr_test_assert(data.str() == "1.000E-6173");
		yggr_test_assert(data.str(yggr::utf8_string().get_allocator()) == "1.000E-6173");

		yggr_test_assert(data.str<std::string>() == "1.000E-6173");
		yggr_test_assert(data.str<std::string>(std::string().get_allocator()) == "1.000E-6173");

		yggr_test_assert(data.str<boost::container::string>() == "1.000E-6173");
		yggr_test_assert(data.str<boost::container::string>(boost::container::string().get_allocator()) == "1.000E-6173");

		yggr_test_assert(data.str<yggr::string>() == "1.000E-6173");
		yggr_test_assert(data.str<yggr::string>(yggr::string().get_allocator()) == "1.000E-6173");

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(data.str<std::wstring>() == L"1.000E-6173");
		yggr_test_assert(data.str<std::wstring>(std::wstring().get_allocator()) == L"1.000E-6173");

		yggr_test_assert(data.str<boost::container::wstring>() == L"1.000E-6173");
		yggr_test_assert(data.str<boost::container::wstring>(boost::container::wstring().get_allocator()) == L"1.000E-6173");

		yggr_test_assert(data.str<yggr::wstring>() == L"1.000E-6173");
		yggr_test_assert(data.str<yggr::wstring>(yggr::wstring().get_allocator()) == L"1.000E-6173");
#endif // YGGR_NO_CWCHAR
	}

	// cmper
	{
		bson_decimal128_type dec128(1000);

		bson_decimal128_type dec128_same(dec128);
		bson_decimal128_type dec128_diff(2000);

		bson_decimal128_type::org_type dec128_org_same = dec128_same;
		bson_decimal128_type::org_type dec128_org_diff = dec128_diff;

		bson_value_type bs_val_same(dec128_same);
		bson_value_type bs_val_diff(dec128_diff);

		bson_value_type::org_type bs_org_val_same = bs_val_same;
		bson_value_type::org_type bs_org_val_diff = bs_val_diff;

		// dec128 <=> int
		yggr_test_assert(dec128 == 1000);
		yggr_test_assert(dec128 == 1000);
		yggr_test_assert(dec128 != 2000);
		yggr_test_assert(dec128 < 2000);
		yggr_test_assert(dec128 <= 2000);
		yggr_test_assert(!(dec128 > 2000));
		yggr_test_assert(!(dec128 >= 2000));

		// int <=> dec128
		yggr_test_assert(1000 == dec128);
		yggr_test_assert(1000 == dec128_same);
		yggr_test_assert(1000 != dec128_diff);
		yggr_test_assert(1000 < dec128_diff);
		yggr_test_assert(1000 <= dec128_diff);
		yggr_test_assert(!(1000 > dec128_diff));
		yggr_test_assert(!(1000 >= dec128_diff));

		// dec128 <=> dec128
		yggr_test_assert(dec128 == dec128);
		yggr_test_assert(dec128 == dec128_same);
		yggr_test_assert(dec128 != dec128_diff);
		yggr_test_assert(dec128 < dec128_diff);
		yggr_test_assert(dec128 <= dec128_diff);
		yggr_test_assert(!(dec128 > dec128_diff));
		yggr_test_assert(!(dec128 >= dec128_diff));

		// dec128 <=> dec128_org
		yggr_test_assert(dec128 == dec128_org_same);
		yggr_test_assert(dec128 != dec128_org_diff);
		yggr_test_assert(dec128 < dec128_org_diff);
		yggr_test_assert(dec128 <= dec128_org_diff);
		yggr_test_assert(!(dec128 > dec128_org_diff));
		yggr_test_assert(!(dec128 >= dec128_org_diff));

		// dec128_org <=> dec128
		yggr_test_assert(dec128_org_same == dec128);
		yggr_test_assert(dec128_org_same == dec128_same);
		yggr_test_assert(dec128_org_same != dec128_diff);
		yggr_test_assert(dec128_org_same < dec128_diff);
		yggr_test_assert(dec128_org_same <= dec128_diff);
		yggr_test_assert(!(dec128_org_same > dec128_diff));
		yggr_test_assert(!(dec128_org_same >= dec128_diff));

		// dec128_org <=> dec128_org
		yggr_test_assert(dec128_org_same == dec128_org_same);
		yggr_test_assert(dec128_org_same != dec128_org_diff);
		yggr_test_assert(dec128_org_same < dec128_org_diff);
		yggr_test_assert(dec128_org_same <= dec128_org_diff);
		yggr_test_assert(!(dec128_org_same > dec128_org_diff));
		yggr_test_assert(!(dec128_org_same >= dec128_org_diff));

		// dec128 <=> bson_value
		yggr_test_assert(dec128 == bs_val_same);
		yggr_test_assert(dec128 != bs_val_diff);
		yggr_test_assert(dec128 < bs_val_diff);
		yggr_test_assert(dec128 <= bs_val_diff);
		yggr_test_assert(!(dec128 > bs_val_diff));
		yggr_test_assert(!(dec128 >= bs_val_diff));

		// bson_value <=> dec128
		yggr_test_assert(bs_val_same == dec128_same);
		yggr_test_assert(bs_val_same != dec128_diff);
		yggr_test_assert(bs_val_same < dec128_diff);
		yggr_test_assert(bs_val_same <= dec128_diff);
		yggr_test_assert(!(bs_val_same > dec128_diff));
		yggr_test_assert(!(bs_val_same >= dec128_diff));

		// bson_value <=> bson_value
		yggr_test_assert(bs_val_same == bs_val_same);
		yggr_test_assert(bs_val_same != bs_val_diff);
		yggr_test_assert(bs_val_same < bs_val_diff);
		yggr_test_assert(bs_val_same <= bs_val_diff);
		yggr_test_assert(!(bs_val_same > bs_val_diff));
		yggr_test_assert(!(bs_val_same >= bs_val_diff));
	}

	// output
	{
		bson_decimal128_type data(1000);

		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(data) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(data) << std::endl;
	}

	{
		bson_decimal128_type data(1000);

		std::string chk_str = "{ \"$numberDecimal\" : \"1.000E-6173\" }";

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
		bson_decimal128_type data(1000);
		bs.save("c_bson_date", data);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		bson_decimal128_type ld_data;
		bson_type::iterator bs_iter(bs, "c_bson_date");
		bs_iter.load("c_bson_date", ld_data);

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
