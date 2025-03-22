//local_file_operator_test_part1.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>

#include <cassert>

#include <boost/bind.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/set.hpp>

#include <yggr/time/time.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(base)

#if defined(YGGR_AT_WINDOWS)
#	define TEST_OF_ADMIN 0 // if you not want to test of admin set the macro to 0
#else
#	define TEST_OF_ADMIN 1
#endif // YGGR_AT_WINDOWS

#define TEST_STL_VERSION 0

#if TEST_STL_VERSION
	typedef yggr::file_system::local_fsys_basic_operators<
				yggr::file_system::stl_fstream_config_type> local_fsys_operators;
#else
	typedef yggr::file_system::local_fsys_operators local_fsys_operators;
#endif // TEST_STL_VERSION

void test_make_path_object(void)
{
	// iter, iter
	{
		char str_path[] = "const char pointer";
		local_fsys_operators::path_type path = 
			local_fsys_operators::make_path_object(str_path, str_path + sizeof(str_path) - 1);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	#if !defined(YGGR_NO_CWCHAR)
	{
		wchar_t str_path[] = L"const char pointer";
		local_fsys_operators::path_type path = 
			local_fsys_operators::make_path_object(str_path, str_path + sizeof(str_path) / sizeof(wchar_t) - 1);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

#endif // !defined(YGGR_NO_CWCHAR)

	// target
	{
		char str_path[] = "const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		std::string str_path = "const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		boost::container::string str_path = "const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		yggr::string str_path = "const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		yggr::string_view str_path = "const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		yggr::utf8_string str_path = "const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		yggr::utf8_string_view str_path = "const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

#if !defined(YGGR_NO_CWCHAR)
	{
		wchar_t str_path[] = L"const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		std::wstring str_path = L"const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		boost::container::wstring str_path = L"const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		yggr::wstring str_path = L"const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		yggr::wstring_view str_path = L"const char pointer";
		local_fsys_operators::path_type path = local_fsys_operators::make_path_object(str_path);
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

#endif // !defined(YGGR_NO_CWCHAR)

	{
		yggr::string str_path = "const char pointer";
		local_fsys_operators::path_type path = 
			local_fsys_operators::make_path_object(yggr::charset::make_string_charset_helper(str_path));
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		yggr::string_view str_path = "const char pointer";
		local_fsys_operators::path_type path = 
			local_fsys_operators::make_path_object(yggr::charset::make_string_charset_helper(str_path));
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	{
		yggr::utf8_string_view str_path = "const char pointer";
		local_fsys_operators::path_type path = 
			local_fsys_operators::make_path_object(yggr::charset::make_string_charset_helper(str_path));
		std::cout << path << std::endl;
		yggr_test_assert(path == local_fsys_operators::make_path_object(str_path));
	}

	std::cout << "--------------test_make_path_object succeed-------------" << std::endl;
}

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_make_path_object();

	wait_any_key(argc, argv);
	return 0;
}
