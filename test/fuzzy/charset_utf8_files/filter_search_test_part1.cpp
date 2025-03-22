//filter_search_test_part1.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

//#define BOOST_HAS_ICU

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/container/set.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/fuzzy/filter_string.hpp>

#include <yggr/regular_parse/regex_parse.hpp>
#include <yggr/regular_parse/u32regex_parse.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(regular_parse)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void string_filter_search(void) // not support chs
{
	try
	{
		char init_str[] = "abcd efg";
		yggr::string true_str_re = "e f b,";
		yggr::string false_str_re = " k po";

		// true
		{
			boost::basic_regex<char> re = yggr::fuzzy::filter_string::make_regex(true_str_re);
			//std::cout << re.str() << std::endl;
			yggr_test_assert(yggr::regular_parse::regex_parse::match("abcd efg", re) == 1);
			std::cout << yggr::regular_parse::regex_parse::match("abcd efg", re) << std::endl; // 1
		}

		{
			std::list<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<char> re = yggr::fuzzy::filter_string::make_regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 1);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 1
		}

		{
			std::vector<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<char> re = yggr::fuzzy::filter_string::make_regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 1);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 1
		}

		{
			std::set<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<char> re = yggr::fuzzy::filter_string::make_regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 1);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 1
		}

		// false
		{
			boost::basic_regex<char> re = yggr::fuzzy::filter_string::make_regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match("abcd efg", re) == 0);
			std::cout << yggr::regular_parse::regex_parse::match("abcd efg", re) << std::endl; // 0
		}

		{
			std::list<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<char> re = yggr::fuzzy::filter_string::make_regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 0);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 0
		}

		{
			std::vector<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<char> re = yggr::fuzzy::filter_string::make_regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 0);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 0
		}

		{
			std::set<char> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<char> re = yggr::fuzzy::filter_string::make_regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 0);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 0
		}

	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}
	catch(...)
	{
		yggr_test_assert(false);
	}

#ifndef YGGR_NO_CWCHAR

	try
	{
	    
		wchar_t init_str[] = L"你好,世界";
		yggr::wstring true_str_re = L" 好 你 ";
		yggr::wstring false_str_re = L" 你世";

		// true
		{
			boost::basic_regex<wchar_t> re = yggr::fuzzy::filter_string::make_regex(true_str_re);
			//std::wcout << re.str() << std::endl;
			yggr_test_assert(yggr::regular_parse::regex_parse::match(L"你好,世界", re) == 1);
			std::cout << yggr::regular_parse::regex_parse::match(L"你好,世界", re) << std::endl; // 1
		}

		{
			std::list<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<wchar_t> re = yggr::fuzzy::filter_string::make_regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 1);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 1
		}

		{
			std::vector<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<wchar_t> re = yggr::fuzzy::filter_string::make_regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 1);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 1
		}

		{
			std::set<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<wchar_t> re = yggr::fuzzy::filter_string::make_regex(true_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 1);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 1
		}

		// false
		{
			boost::basic_regex<wchar_t> re = yggr::fuzzy::filter_string::make_regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(L"你好,世界", re) == 0);
			std::cout << yggr::regular_parse::regex_parse::match(L"你好,世界", re) << std::endl; // 0
		}

		{
			std::list<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<wchar_t> re = yggr::fuzzy::filter_string::make_regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 0);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 0
		}

		{
			std::vector<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<wchar_t> re = yggr::fuzzy::filter_string::make_regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 0);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 0
		}

		{
			std::set<wchar_t> str(init_str, init_str + yggr::charset::charset_base_foo::strlen(init_str));
			boost::basic_regex<wchar_t> re = yggr::fuzzy::filter_string::make_regex(false_str_re);
			yggr_test_assert(yggr::regular_parse::regex_parse::match(str, re) == 0);
			std::cout << yggr::regular_parse::regex_parse::match(str, re) << std::endl; // 0
		}
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}
	catch(...)
	{
		yggr_test_assert(false);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "string_filter_match succeed" << std::endl;

}

int main(int argc, char* argv[])
{
    
//#ifndef YGGR_NO_CWCHAR
//	 TEST_PP_WIO_IMBUE("chs"); 
//#endif // YGGR_NO_CWCHAR

	string_filter_search();

	wait_any_key(argc, argv);
	return 0;
}
