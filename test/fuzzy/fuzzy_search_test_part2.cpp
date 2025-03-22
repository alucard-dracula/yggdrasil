//fuzzy_search_test_part2.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

#define BOOST_HAS_ICU

#ifndef _MSC_VER
#	error "this file test only msvc"
#endif // _MSC_VER

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/container/set.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/fuzzy/fuzzy_string.hpp>

#include <yggr/regular_parse/regex_parse.hpp>
#include <yggr/regular_parse/u32regex_parse.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(regular_parse)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


void utf8_string_fuzzy_search(void) // support chs
{
#ifdef BOOST_HAS_ICU
	try
	{
		char init_str[] = "abcd efg";
		yggr::utf8_string true_str_re = " b,ef ";
		yggr::utf8_string false_str_re = " k po";

		typedef yggr::regular_parse::u32regex_parse::regex_type regex_type;

		// true
		{
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search("abcd efg", re) == 1);
			std::cout << yggr::regular_parse::u32regex_parse::search("abcd efg", re) << std::endl; // 1
		}

		{
			std::list<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 1);
			std::cout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 1
		}

		{
			std::vector<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 1);
			std::cout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 1
		}

		{
			std::set<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 1);
			std::cout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 1
		}

		{
			yggr::utf8_string str(init_str);
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 1);
			std::cout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 1
		}

		// false
		{
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search("abcd efg", re) == 0);
			std::cout << yggr::regular_parse::u32regex_parse::search("abcd efg", re) << std::endl; // 0
		}

		{
			std::list<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 0);
			std::cout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 0
		}

		{
			std::vector<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 0);
			std::cout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 0
		}

		{
			std::set<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 0);
			std::cout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 0
		}

		{
			yggr::utf8_string str(init_str);
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 0);
			std::cout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 0
		}

	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}
	catch(...)
	{
		yggr_test_assert(false);
	}

#if !defined(YGGR_NO_CWCHAR)

	try
	{
		wchar_t init_str[] = L"你好,世界";
		yggr::utf8_string true_str_re = L" 你 好 ";
		yggr::utf8_string false_str_re = L" 你世";

		typedef yggr::regular_parse::u32regex_parse::regex_type regex_type;

		// true
		{
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(L"你好,世界", re) == 1);
			std::wcout << yggr::regular_parse::u32regex_parse::search(L"你好,世界", re) << std::endl; // 1
		}

		{
			std::list<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 1);
			std::wcout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 1
		}

		{
			std::vector<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 1);
			std::wcout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 1
		}

		{
			std::set<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 1);
			std::wcout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 1
		}

		{
			yggr::utf8_string str(init_str);
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 1);
			std::wcout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 1
		}

		// false
		{
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(L"你好,世界", re) == 0);
			std::wcout << yggr::regular_parse::u32regex_parse::search(L"你好,世界", re) << std::endl; // 0
		}

		{
			std::list<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 0);
			std::wcout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 0
		}

		{
			std::vector<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 0);
			std::wcout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 0
		}

		{
			std::set<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 0);
			std::wcout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 0
		}

		{
			yggr::utf8_string str(init_str);
			regex_type re = yggr::fuzzy::fuzzy_string::make_u32regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::u32regex_parse::search(str, re) == 0);
			std::wcout << yggr::regular_parse::u32regex_parse::search(str, re) << std::endl; // 0
		}

	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::wcout << e.what() << std::endl;
		yggr_test_assert(false);
	}
	catch(...)
	{
		yggr_test_assert(false);
	}

#endif // YGGR_NO_CWCHAR

#endif // BOOST_HAS_ICU

	std::cout << "utf8_string_fuzzy_search succeed" << std::endl;
}

int main(int argc, char* argv[])
{
	utf8_string_fuzzy_search();

	wait_any_key(argc, argv);
	return 0;
}
