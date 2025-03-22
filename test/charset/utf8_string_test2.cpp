//utf8_string_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

//
//#ifndef _MSC_VER
//#   define YGGR_NO_CWCHAR
//#endif // _MSC_VER

//#define YGGR_USE_STL_STRING
//#define _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

//#define TEST_IO

#define TEST_CONSTRUCT

void test_operations_starts_ends_with(void)
{
	std::cout << "--------------test_operations_starts_ends_with begin-----------------" << std::endl;
	{
		yggr::utf8_string org("abcde");
		yggr::utf8_string_view nsv(org);
		std::cout << org << std::endl;

		yggr_test_assert(org.starts_with(nsv.substr()));
		yggr_test_assert(!org.starts_with(nsv.substr(1)));

		yggr_test_assert(org.starts_with(nsv.front()));
		yggr_test_assert(!org.starts_with(nsv.back()));

		yggr_test_assert(org.starts_with(nsv.substr().data()));
		yggr_test_assert(!org.starts_with(nsv.substr(1).data()));
	}

	{
		yggr::utf8_string org("ÄãabcdeºÃ");
		yggr::utf8_string_view nsv(org);
		std::cout << org << std::endl;

		yggr_test_assert(org.starts_with(nsv.substr()));
		yggr_test_assert(!org.starts_with(nsv.substr(1)));

		yggr_test_assert(org.starts_with(nsv.front()));
		yggr_test_assert(!org.starts_with(nsv.back()));

		yggr_test_assert(org.starts_with(nsv.substr().data(), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(!org.starts_with(nsv.substr(1).data(), YGGR_STR_INNER_STRING_CHARSET_NAME()));

		yggr_test_assert(org.starts_with(
				yggr::charset::make_string_charset_helper(
					nsv.substr().data(), YGGR_STR_INNER_STRING_CHARSET_NAME())));
		yggr_test_assert(!org.starts_with(
				yggr::charset::make_string_charset_helper(
					nsv.substr(1).data(), YGGR_STR_INNER_STRING_CHARSET_NAME())));
	}

	{
		yggr::utf8_string org("abcde");
		yggr::utf8_string_view nsv(org);
		std::cout << org << std::endl;

		yggr_test_assert(org.ends_with(nsv.substr()));
		yggr_test_assert(!org.ends_with(nsv.substr(1, 3)));

		yggr_test_assert(org.ends_with(nsv.back()));
		yggr_test_assert(!org.ends_with(nsv.front()));

		yggr_test_assert(org.ends_with(nsv.substr().data()));
		yggr_test_assert(org.ends_with(nsv.substr(1).data()));
		yggr_test_assert(!org.ends_with("edcba"));
	}

	{
		yggr::utf8_string org("ÄãabcdeºÃ");
		yggr::utf8_string_view nsv(org);
		std::cout << org << std::endl;

		yggr_test_assert(org.ends_with(nsv.substr()));
		yggr_test_assert(!org.ends_with(nsv.substr(1, 3)));

		yggr_test_assert(org.ends_with(nsv.back()));
		yggr_test_assert(!org.ends_with(nsv.front()));

		yggr_test_assert(org.ends_with(nsv.substr().data(), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(org.ends_with(nsv.substr(1).data(), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(!org.ends_with("edcba"));

		yggr_test_assert(org.ends_with(
				yggr::charset::make_string_charset_helper(
					nsv.substr().data(), YGGR_STR_INNER_STRING_CHARSET_NAME())));
		yggr_test_assert(org.ends_with(
				yggr::charset::make_string_charset_helper(
					nsv.substr(1).data(), YGGR_STR_INNER_STRING_CHARSET_NAME())));
	}

	std::cout << "--------------test_operations_compare end-----------------" << std::endl;
}

void test_operations_contains(void)
{
	std::cout << "--------------test_operations_contains begin-----------------" << std::endl;
	{
		yggr::utf8_string org_t("abcde");
		yggr::utf8_string_view nsv_t(org_t);

		yggr::utf8_string org_f("fgh");
		yggr::utf8_string_view nsv_f(org_f);

		std::cout << org_t << std::endl;
		std::cout << org_f << std::endl;

		yggr_test_assert(org_t.contains(nsv_t));
		yggr_test_assert(!org_t.contains(nsv_f));
		
		yggr_test_assert(org_t.contains(nsv_t.substr(1)));
		yggr_test_assert(!org_t.contains(nsv_f.substr(1)));

		yggr_test_assert(org_t.contains(nsv_t.front()));
		yggr_test_assert(!org_t.contains(nsv_f.front()));


		yggr_test_assert(org_t.contains(nsv_t.back()));
		yggr_test_assert(!org_t.contains(nsv_f.back()));

		yggr_test_assert(org_t.contains(nsv_t.data()));
		yggr_test_assert(!org_t.contains(nsv_f.data()));

		yggr_test_assert(org_t.contains(nsv_t.data(), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(!org_t.contains(nsv_f.data(), YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	{
		yggr::utf8_string org_t("ÄãabcdeºÃ");
		yggr::utf8_string_view nsv_t(org_t);

		yggr::utf8_string org_f("×Öfgh·û");
		yggr::utf8_string_view nsv_f(org_f);

		std::cout << org_t << std::endl;
		std::cout << org_f << std::endl;

		yggr_test_assert(org_t.contains(nsv_t));
		yggr_test_assert(!org_t.contains(nsv_f));
		
		yggr_test_assert(org_t.contains(nsv_t.substr(1)));
		yggr_test_assert(!org_t.contains(nsv_f.substr(1)));

		yggr_test_assert(org_t.contains(nsv_t.front()));
		yggr_test_assert(!org_t.contains(nsv_f.front()));


		yggr_test_assert(org_t.contains(nsv_t.back()));
		yggr_test_assert(!org_t.contains(nsv_f.back()));

		if(yggr::charset::charset_name_t<char>() != yggr::charset::charset_name_t<yggr::utf8_string>())
		{
			try
			{
				org_t.contains(nsv_t.data());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}

			try
			{
				org_t.contains(nsv_f.data());
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			yggr_test_assert(org_t.contains(nsv_t.data()));
			yggr_test_assert(!org_t.contains(nsv_f.data()));
		}

		yggr_test_assert(org_t.contains(nsv_t.data(), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(!org_t.contains(nsv_f.data(), YGGR_STR_INNER_STRING_CHARSET_NAME()));

		yggr_test_assert(org_t.contains(
				yggr::charset::make_string_charset_helper(
					nsv_t.data(), YGGR_STR_INNER_STRING_CHARSET_NAME())));
		yggr_test_assert(!org_t.contains(
				yggr::charset::make_string_charset_helper(
					nsv_f.data(), YGGR_STR_INNER_STRING_CHARSET_NAME())));
	}

	std::cout << "--------------test_operations_contains end-----------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_operations_starts_ends_with();
	test_operations_contains();

	wait_any_key(argc, argv);
	return 0;
}
