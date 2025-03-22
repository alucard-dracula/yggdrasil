//filter_string_test_part1.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

//#define BOOST_HAS_ICU

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/fuzzy/filter_string.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(regular_parse)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


void test1(void)
{
	try
	{
		{
			yggr::string text = "  ab  ,fc,./iyu  ";
			char result_cmp[] = ".*ab.*|.*fc.*|.*iyu.*";

			std::cout << yggr::fuzzy::filter_string::make_string(text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text) == yggr::string(result_cmp));

			std::cout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text) == yggr::string(result_cmp));

			std::cout << yggr::fuzzy::filter_string::make_string(text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.begin(), text.end()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.begin(), text.end()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string(text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.c_str()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.c_str()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string("  ab  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string("  ab  ,fc,./iyu  ") == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), "  ab  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), "  ab  ,fc,./iyu  ") == result_cmp);
		}

		{
			std::string tmp;
			yggr::string text = "  ab  ,fc,./iyu  ";
			char result_cmp[] = ".*ab.*|.*fc.*|.*iyu.*";

			std::cout << yggr::fuzzy::filter_string::make_string<std::string>(text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::string>(text) == std::string(result_cmp));

			std::cout << yggr::fuzzy::filter_string::make_string<std::string>(tmp.get_allocator(), text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::string>(tmp.get_allocator(), text) == std::string(result_cmp));

			std::cout << yggr::fuzzy::filter_string::make_string<std::string>(text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::string>(text.begin(), text.end()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string<std::string>(tmp.get_allocator(), text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::string>(tmp.get_allocator(), text.begin(), text.end()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string<std::string>(text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::string>(text.c_str()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string<std::string>(tmp.get_allocator(), text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::string>(tmp.get_allocator(), text.c_str()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string<std::string>("  ab  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::string>("  ab  ,fc,./iyu  ") == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string<std::string>(tmp.get_allocator(), "  ab  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::string>(tmp.get_allocator(), "  ab  ,fc,./iyu  ") == result_cmp);
		}

		{
			std::string text = "  ab  ,fc,./iyu  ";
			char result_cmp[] = ".*ab.*|.*fc.*|.*iyu.*";

			std::cout << yggr::fuzzy::filter_string::make_string(text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text) == yggr::string(result_cmp));

			std::cout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string(text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.begin(), text.end()) == yggr::string(result_cmp));

			std::cout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.begin(), text.end()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string(text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.c_str()) == yggr::string(result_cmp));

			std::cout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.c_str()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string("  ab  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string("  ab  ,fc,./iyu  ") == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), "  ab  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), "  ab  ,fc,./iyu  ") == result_cmp);
		}

		{
			yggr::string tmp;
			std::string text = "  ab  ,fc,./iyu  ";
			char result_cmp[] = ".*ab.*|.*fc.*|.*iyu.*";

			std::cout << yggr::fuzzy::filter_string::make_string<yggr::string>(text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::string>(text) == yggr::string(result_cmp));

			std::cout << yggr::fuzzy::filter_string::make_string<yggr::string>(tmp.get_allocator(), text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::string>(tmp.get_allocator(), text) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string<yggr::string>(text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::string>(text.begin(), text.end()) == yggr::string(result_cmp));

			std::cout << yggr::fuzzy::filter_string::make_string<yggr::string>(tmp.get_allocator(), text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::string>(tmp.get_allocator(), text.begin(), text.end()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string<yggr::string>(text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::string>(text.c_str()) == yggr::string(result_cmp));

			std::cout << yggr::fuzzy::filter_string::make_string<yggr::string>(tmp.get_allocator(), text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::string>(tmp.get_allocator(), text.c_str()) == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string<yggr::string>("  ab  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::string>("  ab  ,fc,./iyu  ") == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string<yggr::string>(tmp.get_allocator(), "  ab  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::string>(tmp.get_allocator(), "  ab  ,fc,./iyu  ") == result_cmp);
		}
		
		{
			char result_cmp[] = ".*aaa.*|.*bbb.*|.*ccc.*|.*ddd.*";
			
			std::cout << yggr::fuzzy::filter_string::make_string("  aaa ,  bbb,   ccc   ,  ddd  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string("  aaa ,  bbb,   ccc   ,  ddd  ") == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string("aaa ,  bbb,   ccc   ,  ddd  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string("aaa ,  bbb,   ccc   ,  ddd  ") == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string("  aaa ,  bbb,   ccc   ,  ddd") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string("  aaa ,  bbb,   ccc   ,  ddd") == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string("aaa ,  bbb,   ccc   ,  ddd") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string("aaa ,  bbb,   ccc   ,  ddd") == result_cmp);
		}

		{
			char result_cmp[] = ".*ab.*|.*fc.*|.*iyu.*";

			std::cout << yggr::fuzzy::filter_string::make_string("  ab  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string("  ab  ,fc,./iyu  ") == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string("ab  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string("ab  ,fc,./iyu  ") == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string("  ab  ,fc,./iyu") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string("  ab  ,fc,./iyu") == result_cmp);

			std::cout << yggr::fuzzy::filter_string::make_string("ab  ,fc,./iyu") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string("ab  ,fc,./iyu") == result_cmp);
		}
		
	}
	catch(const yggr::fuzzy::filter_string::error_type& e)
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
		{
			yggr::wstring text = L"  你好  ,fc,./iyu  ";
			wchar_t result_cmp[] = L".*你好.*|.*fc.*|.*iyu.*";

			std::wcout << yggr::fuzzy::filter_string::make_string(text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text) == yggr::wstring(result_cmp));

			std::wcout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text) == yggr::wstring(result_cmp));

			std::wcout << yggr::fuzzy::filter_string::make_string(text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.begin(), text.end()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.begin(), text.end()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.c_str()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.c_str()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(L"  你好  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(L"  你好  ,fc,./iyu  ") == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), L"  你好  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), L"  你好  ,fc,./iyu  ") == result_cmp);
		}

		{
			std::wstring tmp;
			yggr::wstring text = L"  你好  ,fc,./iyu  ";
			wchar_t result_cmp[] = L".*你好.*|.*fc.*|.*iyu.*";

			std::wcout << yggr::fuzzy::filter_string::make_string<std::wstring>(text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::wstring>(text) == std::wstring(result_cmp));

			std::wcout << yggr::fuzzy::filter_string::make_string<std::wstring>(tmp.get_allocator(), text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::wstring>(tmp.get_allocator(), text) == std::wstring(result_cmp));

			std::wcout << yggr::fuzzy::filter_string::make_string<std::wstring>(text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::wstring>(text.begin(), text.end()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string<std::wstring>(tmp.get_allocator(), text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::wstring>(tmp.get_allocator(), text.begin(), text.end()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string<std::wstring>(text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::wstring>(text.c_str()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string<std::wstring>(tmp.get_allocator(), text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::wstring>(tmp.get_allocator(), text.c_str()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string<std::wstring>(L"  你好  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::wstring>(L"  你好  ,fc,./iyu  ") == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string<std::wstring>(tmp.get_allocator(), L"  你好  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<std::wstring>(tmp.get_allocator(), L"  你好  ,fc,./iyu  ") == result_cmp);
		}

		{
			std::wstring text = L"  你好  ,fc,./iyu  ";
			wchar_t result_cmp[] = L".*你好.*|.*fc.*|.*iyu.*";

			std::wcout << yggr::fuzzy::filter_string::make_string(text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text) == yggr::wstring(result_cmp));

			std::wcout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.begin(), text.end()) == yggr::wstring(result_cmp));

			std::wcout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.begin(), text.end()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.c_str()) == yggr::wstring(result_cmp));

			std::wcout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), text.c_str()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(L"  你好  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(L"  你好  ,fc,./iyu  ") == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(text.get_allocator(), L"  你好  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(text.get_allocator(), L"  你好  ,fc,./iyu  ") == result_cmp);
		}

		{
			yggr::wstring tmp;
			std::wstring text = L"  你好  ,fc,./iyu  ";
			wchar_t result_cmp[] = L".*你好.*|.*fc.*|.*iyu.*";

			std::wcout << yggr::fuzzy::filter_string::make_string<yggr::wstring>(text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::wstring>(text) == yggr::wstring(result_cmp));

			std::wcout << yggr::fuzzy::filter_string::make_string<yggr::wstring>(tmp.get_allocator(), text) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::wstring>(tmp.get_allocator(), text) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string<yggr::wstring>(text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::wstring>(text.begin(), text.end()) == yggr::wstring(result_cmp));

			std::wcout << yggr::fuzzy::filter_string::make_string<yggr::wstring>(tmp.get_allocator(), text.begin(), text.end()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::wstring>(tmp.get_allocator(), text.begin(), text.end()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string<yggr::wstring>(text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::wstring>(text.c_str()) == yggr::wstring(result_cmp));

			std::wcout << yggr::fuzzy::filter_string::make_string<yggr::wstring>(tmp.get_allocator(), text.c_str()) << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::wstring>(tmp.get_allocator(), text.c_str()) == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string<yggr::wstring>(L"  你好  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::wstring>(L"  你好  ,fc,./iyu  ") == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string<yggr::wstring>(tmp.get_allocator(), L"  你好  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string<yggr::wstring>(tmp.get_allocator(), L"  你好  ,fc,./iyu  ") == result_cmp);
		}
		
		{
			wchar_t result_cmp[] = L".*犯得上.*|.*达到但是.*";
			
			std::wcout << yggr::fuzzy::filter_string::make_string(L"    犯得上  达到但是    ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(L"    犯得上  达到但是    ") == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(L"犯得上  达到但是    ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(L"犯得上  达到但是    ") == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(L"    犯得上  达到但是") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(L"    犯得上  达到但是") == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(L"犯得上  达到但是") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(L"犯得上  达到但是") == result_cmp);
		}

		{
			wchar_t result_cmp[] = L".*你好.*|.*fc.*|.*iyu.*";

			std::wcout << yggr::fuzzy::filter_string::make_string(L"  你好  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(L"  你好  ,fc,./iyu  ") == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(L"你好  ,fc,./iyu  ") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(L"你好  ,fc,./iyu  ") == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(L"  你好  ,fc,./iyu") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(L"  你好  ,fc,./iyu") == result_cmp);

			std::wcout << yggr::fuzzy::filter_string::make_string(L"你好  ,fc,./iyu") << std::endl;
			yggr_test_assert(yggr::fuzzy::filter_string::make_string(L"你好  ,fc,./iyu") == result_cmp);
		}
		
	}
	catch(const yggr::fuzzy::filter_string::error_type& e)
	{
		std::wcout << e.what() << std::endl;
		yggr_test_assert(false);
	}
	catch(...)
	{
		yggr_test_assert(false);
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test1 succeed" << std::endl;
}

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test1();

	wait_any_key(argc, argv);
	return 0;
}
