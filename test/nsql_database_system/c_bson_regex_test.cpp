// c_bson_regex_test.cpp

#include <iostream>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson_regex.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_bson_regex bson_regex_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_regex_type bs_reg;

		yggr_test_assert(bs_reg.regex() == "");
		yggr_test_assert(bs_reg.options() == "");
		std::cout << bs_reg << std::endl;
	}

	{
		bson_regex_type bs_reg("this is a regex string", "this is a regex options");

		yggr_test_assert(bs_reg.regex() == "this is a regex string");
		yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

		std::cout << bs_reg << std::endl;
	}

	{
		bson_regex_type bs_reg_smp("this is a regex string", "this is a regex options");
		bson_value_type bs_val_reg(bs_reg_smp);
		bson_regex_type bs_reg = bs_val_reg;

		yggr_test_assert(bs_reg.regex() == "this is a regex string");
		yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

		std::cout << bs_reg << std::endl;
	}

	// std::string
	{
		{
			bson_regex_type bs_reg(std::string("this is a regex string"), "this is a regex options");

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg("this is a regex string", std::string("this is a regex options"));

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(std::string("this is a regex string"), std::string("this is a regex options"));

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}
	}

	// boost::container::string
	{
		{
			bson_regex_type bs_reg(boost::container::string("this is a regex string"), "this is a regex options");

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg("this is a regex string", boost::container::string("this is a regex options"));

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(boost::container::string("this is a regex string"), boost::container::string("this is a regex options"));

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}
	}

	// yggr::string
	{
		{
			bson_regex_type bs_reg(yggr::string("this is a regex string"), "this is a regex options");

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg("this is a regex string", yggr::string("this is a regex options"));

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(yggr::string("this is a regex string"), yggr::string("this is a regex options"));

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}
	}

	// yggr::utf8_string
	{
		{
			bson_regex_type bs_reg(yggr::utf8_string("this is a regex string"), "this is a regex options");

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg("this is a regex string", yggr::utf8_string("this is a regex options"));

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(yggr::utf8_string("this is a regex string"), yggr::utf8_string("this is a regex options"));

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}
	}

	// yggr::string_charset_helper
	{
		{
			bson_regex_type bs_reg(yggr::charset::make_string_charset_helper("this is a regex string"), "this is a regex options");

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg("this is a regex string", yggr::charset::make_string_charset_helper("this is a regex options"));

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(yggr::charset::make_string_charset_helper("this is a regex string"), 
									yggr::charset::make_string_charset_helper("this is a regex options"));

			yggr_test_assert(bs_reg.regex() == "this is a regex string");
			yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}
	}

#if !defined(YGGR_NO_CWCHAR)
	{
		bson_regex_type bs_reg(L"this is a regex string", L"this is a regex options");

		yggr_test_assert(bs_reg.regex() == L"this is a regex string");
		yggr_test_assert(bs_reg.options() == L"isx"); // optiosn: "ilmsux"

		std::cout << bs_reg << std::endl;
	}

	// std::wstring
	{
		{
			bson_regex_type bs_reg(std::wstring(L"this is a regex string"), L"this is a regex options");

			yggr_test_assert(bs_reg.regex() == L"this is a regex string");
			yggr_test_assert(bs_reg.options() == L"isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(L"this is a regex string", std::wstring(L"this is a regex options"));

			yggr_test_assert(bs_reg.regex() == L"this is a regex string");
			yggr_test_assert(bs_reg.options() == L"isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(std::wstring(L"this is a regex string"), std::wstring(L"this is a regex options"));

			yggr_test_assert(bs_reg.regex() == L"this is a regex string");
			yggr_test_assert(bs_reg.options() == L"isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}
	}

	// boost::container::wstring
	{
		{
			bson_regex_type bs_reg(boost::container::wstring(L"this is a regex string"), L"this is a regex options");

			yggr_test_assert(bs_reg.regex() == L"this is a regex string");
			yggr_test_assert(bs_reg.options() == L"isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(L"this is a regex string", boost::container::wstring(L"this is a regex options"));

			yggr_test_assert(bs_reg.regex() == L"this is a regex string");
			yggr_test_assert(bs_reg.options() == L"isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(boost::container::wstring(L"this is a regex string"), boost::container::wstring(L"this is a regex options"));

			yggr_test_assert(bs_reg.regex() == L"this is a regex string");
			yggr_test_assert(bs_reg.options() == L"isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}
	}

	// yggr::wstring
	{
		{
			bson_regex_type bs_reg(yggr::wstring(L"this is a regex string"), L"this is a regex options");

			yggr_test_assert(bs_reg.regex() == L"this is a regex string");
			yggr_test_assert(bs_reg.options() == L"isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(L"this is a regex string", yggr::wstring(L"this is a regex options"));

			yggr_test_assert(bs_reg.regex() == L"this is a regex string");
			yggr_test_assert(bs_reg.options() == L"isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}

		{
			bson_regex_type bs_reg(yggr::wstring(L"this is a regex string"), yggr::wstring(L"this is a regex options"));

			yggr_test_assert(bs_reg.regex() == L"this is a regex string");
			yggr_test_assert(bs_reg.options() == L"isx"); // optiosn: "ilmsux"

			std::cout << bs_reg << std::endl;
		}
	}

#endif // YGGR_NO_CWCHAR

	{
		bson_regex_type bs_reg_smp("this is a regex string", "this is a regex options");
		bson_regex_type bs_reg_chk = bs_reg_smp;
		bson_regex_type bs_reg = boost::move(bs_reg_smp);

		yggr_test_assert(bs_reg == bs_reg_chk);

		std::cout << bs_reg << std::endl;
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_bson_regex bson_regex_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_regex_type bs_reg_smp("this is a regex string", "this is a regex options");
		bson_value_type bs_val_reg(bs_reg_smp);
		bson_regex_type bs_reg;

		bs_reg = bs_val_reg;

		yggr_test_assert(bs_reg.regex() == "this is a regex string");
		yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"
	}

	{
		bson_regex_type bs_reg_smp("this is a regex string", "this is a regex options");
		bson_regex_type bs_reg;

		bs_reg = boost::move(bs_reg_smp);

		yggr_test_assert(bs_reg.regex() == "this is a regex string");
		yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"
	}

	{
		bson_regex_type bs_reg_smp("this is a regex string", "this is a regex options");
		bson_regex_type bs_reg;

		bs_reg = bs_reg_smp;

		yggr_test_assert(bs_reg.regex() == "this is a regex string");
		yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_bson_regex bson_regex_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	{
		bson_regex_type bs_reg_smp("this is a regex string", "this is a regex options");
		bson_regex_type bs_reg;

		bs_reg.swap(boost::move(bs_reg_smp));

		yggr_test_assert(bs_reg.regex() == "this is a regex string");
		yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"
	}

	{
		bson_regex_type bs_reg_smp("this is a regex string", "this is a regex options");
		bson_regex_type bs_reg;

		bs_reg.swap(bs_reg_smp);

		yggr_test_assert(bs_reg.regex() == "this is a regex string");
		yggr_test_assert(bs_reg.options() == "isx"); // optiosn: "ilmsux"
	}

	{
		bson_regex_type bs_reg_chk1("this is a regex string", "this is a regex options");
		bson_regex_type bs_reg_chk2("this is a regex string2", "this is a regex options");

		bson_regex_type bs_reg1("this is a regex string", "this is a regex options");
		bson_regex_type bs_reg2("this is a regex string2", "this is a regex options");

		{
			std::swap(boost::move(bs_reg1), bs_reg2);

			yggr_test_assert(bs_reg1 == bs_reg_chk2);
			yggr_test_assert(bs_reg2 == bs_reg_chk1);

			std::swap(bs_reg1, boost::move(bs_reg2));

			yggr_test_assert(bs_reg1 == bs_reg_chk1);
			yggr_test_assert(bs_reg2 == bs_reg_chk2);

			std::swap(bs_reg1, bs_reg2);

			yggr_test_assert(bs_reg1 == bs_reg_chk2);
			yggr_test_assert(bs_reg2 == bs_reg_chk1);
		}

		{
			boost::swap(boost::move(bs_reg1), bs_reg2);

			yggr_test_assert(bs_reg1 == bs_reg_chk1);
			yggr_test_assert(bs_reg2 == bs_reg_chk2);

			boost::swap(bs_reg1, boost::move(bs_reg2));

			yggr_test_assert(bs_reg1 == bs_reg_chk2);
			yggr_test_assert(bs_reg2 == bs_reg_chk1);

			boost::swap(bs_reg1, bs_reg2);

			yggr_test_assert(bs_reg1 == bs_reg_chk1);
			yggr_test_assert(bs_reg2 == bs_reg_chk2);
		}

		{
			yggr::swap(boost::move(bs_reg1), bs_reg2);

			yggr_test_assert(bs_reg1 == bs_reg_chk2);
			yggr_test_assert(bs_reg2 == bs_reg_chk1);

			yggr::swap(bs_reg1, boost::move(bs_reg2));

			yggr_test_assert(bs_reg1 == bs_reg_chk1);
			yggr_test_assert(bs_reg2 == bs_reg_chk2);

			yggr::swap(bs_reg1, bs_reg2);

			yggr_test_assert(bs_reg1 == bs_reg_chk2);
			yggr_test_assert(bs_reg2 == bs_reg_chk1);
		}
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef yggr::nsql_database_system::c_bson_regex bson_regex_type;
	typedef yggr::nsql_database_system::c_bson_value bson_value_type;

	// value_typeid
	{
		bson_regex_type bs_reg;

		yggr_test_assert(bson_regex_type::value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_REGEX);
		yggr_test_assert(bson_regex_type::s_value_typeid() == yggr::nsql_database_system::bson_typeid_def::E_BSON_TYPE_REGEX);
	}

	// compare_eq
	{
		bson_regex_type bs_reg_chk1("this is a regex string", "this is a regex options");
		bson_regex_type bs_reg_chk2("this is a regex string2", "this is a regex options");

		bson_regex_type bs_reg = bs_reg_chk1;

		bool bchk = false;

		bchk = bs_reg.compare_eq(bs_reg_chk1);
		yggr_test_assert(bchk);

		bchk = bs_reg.compare_eq(bs_reg_chk2);
		yggr_test_assert(!bchk);

		bchk = (bs_reg == bs_reg_chk1);
		yggr_test_assert(bchk);

		bchk = (bs_reg != bs_reg_chk1);
		yggr_test_assert(!bchk);

		bchk = (bs_reg == bs_reg_chk2);
		yggr_test_assert(!bchk);

		bchk = (bs_reg != bs_reg_chk2);
		yggr_test_assert(bchk);
	}

	// regex
	{
		bson_regex_type bs_reg("this is a regex string", "this is a regex options");

		bool bchk = false;

		bchk = (bs_reg.regex() == "this is a regex string");
		yggr_test_assert(bchk);

		bs_reg.regex() = "this is a regex string2";

		bchk = (bs_reg.regex() == "this is a regex string2");
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<std::string>() == std::string("this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<std::string>(std::string().get_allocator()) == std::string("this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<boost::container::string>() == boost::container::string("this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<boost::container::string>(boost::container::string().get_allocator()) == boost::container::string("this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::string>() == yggr::string("this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::string>(yggr::string().get_allocator()) == yggr::string("this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::utf8_string>() == yggr::utf8_string("this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == yggr::utf8_string("this is a regex string2"));
		yggr_test_assert(bchk);

#if !defined(YGGR_NO_CWCHAR)
		bchk = (bs_reg.regex<std::wstring>() == std::wstring(L"this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<std::wstring>(std::wstring().get_allocator()) == std::wstring(L"this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<boost::container::wstring>() == boost::container::wstring(L"this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<boost::container::wstring>(boost::container::wstring().get_allocator()) == boost::container::wstring(L"this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::wstring>() == yggr::wstring(L"this is a regex string2"));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::wstring>(yggr::wstring().get_allocator()) == yggr::wstring(L"this is a regex string2"));
		yggr_test_assert(bchk);
#endif // YGGR_NO_CWCHAR
	}

	// regex
	{
		bson_regex_type bs_reg("this is a regex string", "this is a regex options");
		yggr::utf8_string str_chk1 = "this is a regex string";
		yggr::utf8_string str_chk2 = "this is a regex string2";

		bool bchk = false;

		bchk = (bs_reg.regex() == str_chk1);
		yggr_test_assert(bchk);

		bs_reg.regex() = str_chk2;

		bchk = (bs_reg.regex() == str_chk2);
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<std::string>() == std::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<std::string>(std::string().get_allocator()) == std::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<boost::container::string>() == boost::container::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<boost::container::string>(boost::container::string().get_allocator()) == boost::container::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::string>() == yggr::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::string>(yggr::string().get_allocator()) == yggr::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::utf8_string>() == yggr::utf8_string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == yggr::utf8_string(str_chk2));
		yggr_test_assert(bchk);

#if !defined(YGGR_NO_CWCHAR)
		bchk = (bs_reg.regex<std::wstring>() == std::wstring(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<std::wstring>(std::wstring().get_allocator()) == std::wstring(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<boost::container::wstring>() == boost::container::wstring(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<boost::container::wstring>(boost::container::wstring().get_allocator()) == boost::container::wstring(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::wstring>() == yggr::wstring(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.regex<yggr::wstring>(yggr::wstring().get_allocator()) == yggr::wstring(str_chk2));
		yggr_test_assert(bchk);
#endif // YGGR_NO_CWCHAR
	}

	// options
	{
		bson_regex_type bs_reg("this is a regex string", "this is a regex options");
		yggr::utf8_string str_chk1 = "isx";
		yggr::utf8_string str_chk2 = "imsx"; // optiosn: "ilmsux"

		bool bchk = false;

		bchk = (bs_reg.options() == str_chk1);
		yggr_test_assert(bchk);

		bs_reg.options() = "this is a regex options m";

		bchk = (bs_reg.options() == str_chk2);
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<std::string>() == std::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<std::string>(std::string().get_allocator()) == std::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<boost::container::string>() == boost::container::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<boost::container::string>(boost::container::string().get_allocator()) == boost::container::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<yggr::string>() == yggr::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<yggr::string>(yggr::string().get_allocator()) == yggr::string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<yggr::utf8_string>() == yggr::utf8_string(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == yggr::utf8_string(str_chk2));
		yggr_test_assert(bchk);

#if !defined(YGGR_NO_CWCHAR)
		bchk = (bs_reg.options<std::wstring>() == std::wstring(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<std::wstring>(std::wstring().get_allocator()) == std::wstring(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<boost::container::wstring>() == boost::container::wstring(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<boost::container::wstring>(boost::container::wstring().get_allocator()) == boost::container::wstring(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<yggr::wstring>() == yggr::wstring(str_chk2));
		yggr_test_assert(bchk);

		bchk = (bs_reg.options<yggr::wstring>(yggr::wstring().get_allocator()) == yggr::wstring(str_chk2));
		yggr_test_assert(bchk);
#endif // YGGR_NO_CWCHAR
	}

	// output
	{
		bson_regex_type bs_reg("this is a regex string", "this is a regex options");
		
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs_reg) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs_reg) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs_reg) << std::endl;
	}

	{
		bson_regex_type bs_reg("this is a regex string", "this is a regex options");

		std::string chk_str1 = "{ \"$regex\" : \"this is a regex string\", \"$options\" : \"isx\" }";
		std::string chk_str2 = "{ \"$regularExpression\" : { \"pattern\" : \"this is a regex string\", \"options\" : \"isx\" } }";

		{
			std::stringstream ss;
			ss << bs_reg;
			yggr_test_assert(ss.str() == chk_str1);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs_reg);
			yggr_test_assert(ss.str() == chk_str1);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs_reg);
			yggr_test_assert(ss.str() == chk_str2);
		}

		{
			std::stringstream ss;
			ss << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs_reg);
			yggr_test_assert(ss.str() == chk_str2);
		}
	}

	//save load
	{
		bson_regex_type bs_reg("this is a regex string", "this is a regex options");

		bson_type bs;
		bs.save("c_bson_regex", bs_reg);

		std::cout << bs << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(bs) << std::endl;
		std::cout << yggr::nsql_database_system::make_bson_visit<bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(bs) << std::endl;

		bson_regex_type ld_bs_reg;
		bson_type::iterator bs_iter(bs, "c_bson_regex");
		bs_iter.load("c_bson_regex", ld_bs_reg);

		yggr_test_assert(bs_reg == ld_bs_reg);
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