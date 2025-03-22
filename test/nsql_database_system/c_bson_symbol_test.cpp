// c_bson_symbol_test.cpp

#include <iostream>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson_symbol.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson_symbol bson_symbol_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_symbol_type bs_symbol;

		yggr_test_assert(bs_symbol.symbol() == "");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol_smp("this is a bson symbol");
		bson_value_type bs_symbol_val(bs_symbol_smp);
		bson_symbol_type bs_symbol(bs_symbol_val);

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol("this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(std::string("this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(boost::container::string("this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(yggr::string("this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(yggr::utf8_string("this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	// make_string_charset_helper
	{
		bson_symbol_type bs_symbol(yggr::charset::make_string_charset_helper("this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(yggr::charset::make_string_charset_helper(std::string("this is a bson symbol")));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(yggr::charset::make_string_charset_helper(boost::container::string("this is a bson symbol")));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(yggr::charset::make_string_charset_helper(yggr::string("this is a bson symbol")));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(yggr::charset::make_string_charset_helper(yggr::utf8_string("this is a bson symbol")));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

#if !defined(YGGR_NO_CWCHAR)
	{
		bson_symbol_type bs_symbol(L"this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(std::wstring(L"this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(boost::container::wstring(L"this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(yggr::wstring(L"this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	// make_string_charset_helper
	{
		bson_symbol_type bs_symbol(yggr::charset::make_string_charset_helper(L"this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(yggr::charset::make_string_charset_helper(std::wstring(L"this is a bson symbol")));

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(yggr::charset::make_string_charset_helper(boost::container::wstring(L"this is a bson symbol")));

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol(yggr::charset::make_string_charset_helper(yggr::wstring(L"this is a bson symbol")));

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	{
		bson_symbol_type bs_symbol_smp("this is a bson symbol");
		bson_symbol_type bs_symbol(boost::move(bs_symbol_smp));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	{
		bson_symbol_type bs_symbol_smp("this is a bson symbol");
		bson_symbol_type bs_symbol(bs_symbol_smp);

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
		std::cout << bs_symbol << std::endl;
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson_symbol bson_symbol_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_symbol_type bs_symbol_smp("this is a bson symbol");
		bson_value_type bs_symbol_val(bs_symbol_smp);
		bson_symbol_type bs_symbol;
		bs_symbol = bs_symbol_val;

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = "this is a bson symbol";

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = std::string("this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = boost::container::string("this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::string("this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::utf8_string("this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	// make_string_charset_helper
	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::charset::make_string_charset_helper("this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::charset::make_string_charset_helper(std::string("this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::charset::make_string_charset_helper(boost::container::string("this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::charset::make_string_charset_helper(yggr::string("this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::charset::make_string_charset_helper(yggr::utf8_string("this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

#if !defined(YGGR_NO_CWCHAR)
	{
		bson_symbol_type bs_symbol;
		bs_symbol = L"this is a bson symbol";

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = std::wstring(L"this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = boost::container::wstring(L"this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::wstring(L"this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
	}

	// make_string_charset_helper
	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::charset::make_string_charset_helper(L"this is a bson symbol");

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::charset::make_string_charset_helper(std::wstring(L"this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::charset::make_string_charset_helper(boost::container::wstring(L"this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol;
		bs_symbol = yggr::charset::make_string_charset_helper(yggr::wstring(L"this is a bson symbol"));

		yggr_test_assert(bs_symbol.symbol() == L"this is a bson symbol");
	}

#endif // YGGR_NO_CWCHAR

	{
		bson_symbol_type bs_symbol_smp("this is a bson symbol");
		bson_symbol_type bs_symbol;
		bs_symbol = boost::move(bs_symbol_smp);

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol_smp("this is a bson symbol");
		bson_symbol_type bs_symbol;
		bs_symbol = bs_symbol_smp;

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson_symbol bson_symbol_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_symbol_type bs_symbol_smp("this is a bson symbol");
		bson_symbol_type bs_symbol;

		bs_symbol.swap(boost::move(bs_symbol_smp));

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol_smp("this is a bson symbol");
		bson_symbol_type bs_symbol;

		bs_symbol.swap(bs_symbol_smp);

		yggr_test_assert(bs_symbol.symbol() == "this is a bson symbol");
	}

	{
		bson_symbol_type bs_symbol_chk1("this is a bson symbol");
		bson_symbol_type bs_symbol_chk2("this is a bson symbol2");

		bson_symbol_type bs_symbol1 = bs_symbol_chk1;
		bson_symbol_type bs_symbol2 = bs_symbol_chk2;

		{
			std::swap(boost::move(bs_symbol1), bs_symbol2);

			yggr_test_assert(bs_symbol1 == bs_symbol_chk2);
			yggr_test_assert(bs_symbol2 == bs_symbol_chk1);

			std::swap(bs_symbol1, boost::move(bs_symbol2));

			yggr_test_assert(bs_symbol1 == bs_symbol_chk1);
			yggr_test_assert(bs_symbol2 == bs_symbol_chk2);

			std::swap(bs_symbol1, bs_symbol2);

			yggr_test_assert(bs_symbol1 == bs_symbol_chk2);
			yggr_test_assert(bs_symbol2 == bs_symbol_chk1);
		}

		{
			boost::swap(boost::move(bs_symbol1), bs_symbol2);

			yggr_test_assert(bs_symbol1 == bs_symbol_chk1);
			yggr_test_assert(bs_symbol2 == bs_symbol_chk2);

			boost::swap(bs_symbol1, boost::move(bs_symbol2));

			yggr_test_assert(bs_symbol1 == bs_symbol_chk2);
			yggr_test_assert(bs_symbol2 == bs_symbol_chk1);

			boost::swap(bs_symbol1, bs_symbol2);

			yggr_test_assert(bs_symbol1 == bs_symbol_chk1);
			yggr_test_assert(bs_symbol2 == bs_symbol_chk2);
		}

		{
			yggr::swap(boost::move(bs_symbol1), bs_symbol2);

			yggr_test_assert(bs_symbol1 == bs_symbol_chk2);
			yggr_test_assert(bs_symbol2 == bs_symbol_chk1);

			yggr::swap(bs_symbol1, boost::move(bs_symbol2));

			yggr_test_assert(bs_symbol1 == bs_symbol_chk1);
			yggr_test_assert(bs_symbol2 == bs_symbol_chk2);

			yggr::swap(bs_symbol1, bs_symbol2);

			yggr_test_assert(bs_symbol1 == bs_symbol_chk2);
			yggr_test_assert(bs_symbol2 == bs_symbol_chk1);
		}
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_symbol bson_symbol_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	// value_typeid
	{
		bson_symbol_type bs_symbol;

		yggr_test_assert(bson_symbol_type::value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_SYMBOL);
		yggr_test_assert(bson_symbol_type::s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_SYMBOL);
		yggr_test_assert(bs_symbol.value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_SYMBOL);
		yggr_test_assert(bs_symbol.s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_SYMBOL);
	}

	// compare_eq
	{
		bson_symbol_type bs_symbol_chk1("this is a bson symbol");
		bson_symbol_type bs_symbol_chk2("this is a bson symbo2");

		bson_symbol_type bs_symbol = bs_symbol_chk1;

		bool bchk = false;

		bchk = bs_symbol.compare_eq(bs_symbol_chk1);
		yggr_test_assert(bchk);

		bchk = bs_symbol.compare_eq(bs_symbol_chk2);
		yggr_test_assert(!bchk);

		bchk = (bs_symbol == bs_symbol_chk1);
		yggr_test_assert(bchk);

		bchk = (bs_symbol != bs_symbol_chk1);
		yggr_test_assert(!bchk);

		bchk = (bs_symbol == bs_symbol_chk2);
		yggr_test_assert(!bchk);

		bchk = (bs_symbol != bs_symbol_chk2);
		yggr_test_assert(bchk);
	}

	// symbol
	{
		bson_symbol_type bs_symbol("this is a bson symbol");

		bool bchk = false;

		bchk = (bs_symbol.symbol() == "this is a bson symbol");
		yggr_test_assert(bchk);

		bs_symbol.symbol() = "this is a bson symbol2";

		bchk = (bs_symbol.symbol() == "this is a bson symbol2");
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<std::string>() == std::string("this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<std::string>(std::string().get_allocator()) == std::string("this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<boost::container::string>() == boost::container::string("this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<boost::container::string>(boost::container::string().get_allocator()) == boost::container::string("this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<yggr::string>() == yggr::string("this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<yggr::string>(yggr::string().get_allocator()) == yggr::string("this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<yggr::utf8_string>() == yggr::utf8_string("this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == yggr::utf8_string("this is a bson symbol2"));
		yggr_test_assert(bchk);

#if !defined(YGGR_NO_CWCHAR)
		bchk = (bs_symbol.symbol<std::wstring>() == std::wstring(L"this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<std::wstring>(std::wstring().get_allocator()) == std::wstring(L"this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<boost::container::wstring>() == boost::container::wstring(L"this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<boost::container::wstring>(boost::container::wstring().get_allocator()) == boost::container::wstring(L"this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<yggr::wstring>() == yggr::wstring(L"this is a bson symbol2"));
		yggr_test_assert(bchk);

		bchk = (bs_symbol.symbol<yggr::wstring>(yggr::wstring().get_allocator()) == yggr::wstring(L"this is a bson symbol2"));
		yggr_test_assert(bchk);
#endif // YGGR_NO_CWCHAR
	}

	// output
	{
		bson_symbol_type bs_symbol("this is a bson symbol");
		
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs_symbol) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs_symbol) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs_symbol) << std::endl;
	}

	{
		bson_symbol_type bs_symbol("this is a bson symbol");

		std::string chk_str1 = "\"this is a bson symbol\"";
		std::string chk_str2 = "{ \"$symbol\" : \"this is a bson symbol\" }";

		{
			std::stringstream ss;
			ss << bs_symbol;
			yggr_test_assert(ss.str() == chk_str1);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs_symbol);
			yggr_test_assert(ss.str() == chk_str1);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs_symbol);
			yggr_test_assert(ss.str() == chk_str2);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs_symbol);
			yggr_test_assert(ss.str() == chk_str2);
		}
	}

	//save load
	{
		bson_symbol_type bs_symbol("this is a bson symbol");

		bson_type bs;
		bs.save("c_bson_symbol", bs_symbol);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		bson_symbol_type ld_bs_symbol;
		bson_type::iterator bs_iter(bs, "c_bson_symbol");
		bs_iter.load("c_bson_symbol", ld_bs_symbol);

		yggr_test_assert(bs_symbol == ld_bs_symbol);
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