//path_check_test_part3.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

#define BOOST_HAS_ICU

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/file_system/path_check.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(regular_parse)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

//#define YGGR_TEST_USING_LINUX_PATH

#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)

	char native_str_path[] = "c:/aaa/bbb/native_string.txt";
	std::string stl_str_path("c:/aaa/bbb/stl_string.txt");
	boost::container::string boost_str_path("c:/aaa/bbb/boost_string.txt");
	yggr::utf8_string yggr_utf8_str_path("c:/aaa/bbb/yggr_utf8_string.txt");

#if !defined(YGGR_NO_CWCHAR)
	wchar_t native_wstr_path[] = L"c:/一级目录/二级目录/原始的_宽字符串.txt";
	std::wstring stl_wstr_path(L"c:/一级目录/二级目录/stl的_宽字符串.txt");
	boost::container::wstring boost_wstr_path(L"c:/一级目录/二级目录/boost的_宽字符串.txt");
#endif // YGGR_NO_CWCHAR

	char native_str_path_only[] = "c:/aaa/bbb/";
	std::string stl_str_path_only("c:/aaa/bbb/");
	boost::container::string boost_str_path_only("c:/aaa/bbb/");
	yggr::utf8_string yggr_utf8_str_path_only("c:/aaa/bbb/");

#if !defined(YGGR_NO_CWCHAR)
	wchar_t native_wstr_path_only[] = L"c:/一级目录/二级目录/";
	std::wstring stl_wstr_path_only(L"c:/一级目录/二级目录/");
	boost::container::wstring boost_wstr_path_only(L"c:/一级目录/二级目录/");
#endif // YGGR_NO_CWCHAR

#else

	char native_str_path[] = "/aaa/bbb/native_string.txt";
	std::string stl_str_path("/aaa/bbb/stl_string.txt");
	boost::container::string boost_str_path("/aaa/bbb/boost_string.txt");
	yggr::utf8_string yggr_utf8_str_path("/aaa/bbb/yggr_utf8_string.txt");

#if !defined(YGGR_NO_CWCHAR)
	wchar_t native_wstr_path[] = L"/一级目录/二级目录/原始的_宽字符串.txt";
	std::wstring stl_wstr_path(L"/一级目录/二级目录/stl的_宽字符串.txt");
	boost::container::wstring boost_wstr_path(L"/一级目录/二级目录/boost的_宽字符串.txt");
#endif // YGGR_NO_CWCHAR

	char native_str_path_only[] = "/aaa/bbb/";
	std::string stl_str_path_only("/aaa/bbb/");
	boost::container::string boost_str_path_only("/aaa/bbb/");
	yggr::utf8_string yggr_utf8_str_path_only("/aaa/bbb/");

#if !defined(YGGR_NO_CWCHAR)
	wchar_t native_wstr_path_only[] = L"/一级目录/二级目录/";
	std::wstring stl_wstr_path_only(L"/一级目录/二级目录/");
	boost::container::wstring boost_wstr_path_only(L"/一级目录/二级目录/");
#endif // YGGR_NO_CWCHAR

#endif //YGGR_AT_WINDOWS

typedef yggr::file_system::path_check file_check_type;

void test_split_path(void)
{
	{
		std::vector<std::string> cont;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, "");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "");

		size = file_check_type::split_path(cont, ".");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == ".");

		size = file_check_type::split_path(cont, "..");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "..");

		size = file_check_type::split_path(cont, "c:");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "c:");

		size = file_check_type::split_path(cont, "/");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "/");

		size = file_check_type::split_path(cont, "./");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == ".");

		size = file_check_type::split_path(cont, "../");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "..");

		size = file_check_type::split_path(cont, "c:/");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "c:");

		size = file_check_type::split_path(cont, "/aaa");
		yggr_test_assert(cont.size() == 2);
		yggr_test_assert(size == 2);
		yggr_test_assert(cont[0] == "/");
		yggr_test_assert(cont[1] == "aaa");

		size = file_check_type::split_path(cont, "/aaa/bbb");
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
		yggr_test_assert(cont[0] == "/");
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");

		size = file_check_type::split_path(cont, "/aaa/*");
#ifdef YGGR_AT_WINDOWS
		yggr_test_assert(size == 0);
#else
		yggr_test_assert(size == 3);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(cont[0] == "/");
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "*");
#endif // YGGR_AT_WINDOWS

		size = file_check_type::split_path(cont, "/aaa//");
		yggr_test_assert(size == 0);

		size = file_check_type::split_path(cont, "//aaa");
		yggr_test_assert(size == 0);

		size = file_check_type::split_path(cont, "aaa//bbb");
		yggr_test_assert(size == 0);
	}

	std::cout << "---------------------------" << std::endl;

	// c:/aaa/bbb/native_string.txt
	// /aaa/bbb/native_string.txt
	{
		std::vector< std::pair<const char*, const char*> > cont;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, native_str_path);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
		yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
		yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
		yggr_test_assert(std::string(cont[3].first, cont[3].second) == "native_string.txt");

		file_check_type::split_path(cont, &native_str_path[0]);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
		yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
		yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
		yggr_test_assert(std::string(cont[3].first, cont[3].second) == "native_string.txt");

		file_check_type::split_path(
			cont, &native_str_path[0], &native_str_path[0] + yggr::charset::charset_base_foo::strlen(native_str_path));
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
		yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
		yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
		yggr_test_assert(std::string(cont[3].first, cont[3].second) == "native_string.txt");

		// path_only
		size = file_check_type::split_path(cont, native_str_path_only);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
		yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
		yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
	}

	{
		std::vector<std::string> cont;
		std::string alloc_gen;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, native_str_path);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "native_string.txt");

		file_check_type::split_path(cont, &native_str_path[0]);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "native_string.txt");

		file_check_type::split_path(
			cont, &native_str_path[0], &native_str_path[0] + yggr::charset::charset_base_foo::strlen(native_str_path));
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "native_string.txt");

		// ------------allocator version-------------------
		size = file_check_type::split_path(cont, native_str_path, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "native_string.txt");

		file_check_type::split_path(cont, &native_str_path[0], alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "native_string.txt");

		file_check_type::split_path(
			cont, &native_str_path[0],
			&native_str_path[0] + yggr::charset::charset_base_foo::strlen(native_str_path),
			alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "native_string.txt");

		// path_only
		size = file_check_type::split_path(cont, native_str_path_only);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");

		// ------------allocator version-------------------
		size = file_check_type::split_path(cont, native_str_path_only, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
	}

	std::cout << "---------------------------" << std::endl;

	// c:/aaa/bbb/stl_string.txt
	//	/aaa/bbb/stl_string.txt
	{
		std::size_t size = 0;

		{
			std::vector< std::pair<std::string::const_iterator, std::string::const_iterator> > cont;
			size = file_check_type::split_path(cont, stl_str_path);
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
			yggr_test_assert(std::string(cont[3].first, cont[3].second) == "stl_string.txt");
		}

		{
			std::vector< std::pair<const char*, const char*> > cont;

			file_check_type::split_path(cont, stl_str_path.c_str());
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
			yggr_test_assert(std::string(cont[3].first, cont[3].second) == "stl_string.txt");
		}

		{
			std::vector< std::pair<std::string::const_iterator, std::string::const_iterator> > cont;
			file_check_type::split_path(cont, stl_str_path.begin(), stl_str_path.end());
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
			yggr_test_assert(std::string(cont[3].first, cont[3].second) == "stl_string.txt");
		}

		// path_only

		{
			std::vector< std::pair<std::string::const_iterator, std::string::const_iterator> > cont;
			size = file_check_type::split_path(cont, stl_str_path_only);
			yggr_test_assert(cont.size() == 3);
			yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
		}
	}

	{
		std::vector<std::string> cont;
		std::string alloc_gen;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, stl_str_path);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "stl_string.txt");

		file_check_type::split_path(cont, stl_str_path.c_str());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "stl_string.txt");

		file_check_type::split_path(
			cont, stl_str_path.begin(), stl_str_path.end());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "stl_string.txt");

		// ------------allocator version-------------------
		size = file_check_type::split_path(cont, stl_str_path, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "stl_string.txt");

		file_check_type::split_path(cont, stl_str_path.c_str(), alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "stl_string.txt");

		file_check_type::split_path(cont, stl_str_path.begin(), stl_str_path.end(), alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "stl_string.txt");

		//path_only
		size = file_check_type::split_path(cont, stl_str_path_only);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");

		size = file_check_type::split_path(cont, stl_str_path_only, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
	}

	std::cout << "---------------------------" << std::endl;

	// c:/aaa/bbb/boost_string.txt
	//	/aaa/bbb/boost_string.txt
	{
		std::size_t size = 0;

		{
			std::vector< std::pair<boost::container::string::const_iterator, boost::container::string::const_iterator> > cont;
			size = file_check_type::split_path(cont, boost_str_path);
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
			yggr_test_assert(std::string(cont[3].first, cont[3].second) == "boost_string.txt");
		}

		{
			std::vector< std::pair<const char*, const char*> > cont;

			file_check_type::split_path(cont, boost_str_path.c_str());
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
			yggr_test_assert(std::string(cont[3].first, cont[3].second) == "boost_string.txt");
		}

		{
			std::vector< std::pair<boost::container::string::const_iterator, boost::container::string::const_iterator> > cont;
			file_check_type::split_path(cont, boost_str_path.begin(), boost_str_path.end());
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
			yggr_test_assert(std::string(cont[3].first, cont[3].second) == "boost_string.txt");
		}

		// path_only
		{
			std::vector< std::pair<boost::container::string::const_iterator, boost::container::string::const_iterator> > cont;
			size = file_check_type::split_path(cont, boost_str_path_only);
			yggr_test_assert(cont.size() == 3);
			yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(std::string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(std::string(cont[2].first, cont[2].second) == "bbb");
		}
	}

	{
		std::vector<std::string> cont;
		std::string alloc_gen;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, boost_str_path);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "boost_string.txt");

		file_check_type::split_path(cont, boost_str_path.c_str());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "boost_string.txt");

		file_check_type::split_path(
			cont, boost_str_path.begin(), boost_str_path.end());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "boost_string.txt");

		// ------------allocator version-------------------
		size = file_check_type::split_path(cont, boost_str_path, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "boost_string.txt");

		file_check_type::split_path(cont, boost_str_path.c_str(), alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "boost_string.txt");

		file_check_type::split_path(cont, boost_str_path.begin(), boost_str_path.end(), alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "boost_string.txt");

		// path only
		size = file_check_type::split_path(cont, boost_str_path_only);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");

		size = file_check_type::split_path(cont, boost_str_path_only, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
	}

	std::cout << "---------------------------" << std::endl;

	// c:/aaa/bbb/yggr_utf8_string.txt
	//	/aaa/bbb/yggr_utf8_string.txt
	{

		std::size_t size = 0;

		{
			std::vector< std::pair<yggr::utf8_string::const_iterator, yggr::utf8_string::const_iterator> > cont;
			size = file_check_type::split_path(cont, yggr_utf8_str_path);
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(yggr::utf8_string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(yggr::utf8_string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(yggr::utf8_string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(yggr::utf8_string(cont[2].first, cont[2].second) == "bbb");
			yggr_test_assert(yggr::utf8_string(cont[3].first, cont[3].second) == "yggr_utf8_string.txt");
		}

		{
			std::vector< std::pair<const char*, const char*> > cont;

			file_check_type::split_path(cont, yggr_utf8_str_path.c_str());
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(yggr::utf8_string(cont[0].first, cont[0].second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "c:");
#else
			yggr_test_assert(yggr::utf8_string(cont[0].first, cont[0].second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(yggr::utf8_string(cont[1].first, cont[1].second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "aaa");
			yggr_test_assert(yggr::utf8_string(cont[2].first, cont[2].second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "bbb");
			yggr_test_assert(yggr::utf8_string(cont[3].first, cont[3].second, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == "yggr_utf8_string.txt");
		}

		{
			std::vector< std::pair<yggr::utf8_string::iterator, yggr::utf8_string::iterator> > cont;
			file_check_type::split_path(cont, yggr_utf8_str_path.begin(), yggr_utf8_str_path.end());
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(yggr::utf8_string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(yggr::utf8_string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(yggr::utf8_string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(yggr::utf8_string(cont[2].first, cont[2].second) == "bbb");
			yggr_test_assert(yggr::utf8_string(cont[3].first, cont[3].second) == "yggr_utf8_string.txt");
		}

		// path_only
		{
			std::vector< std::pair<yggr::utf8_string::const_iterator, yggr::utf8_string::const_iterator> > cont;
			size = file_check_type::split_path(cont, yggr_utf8_str_path_only);
			yggr_test_assert(cont.size() == 3);
			yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(yggr::utf8_string(cont[0].first, cont[0].second) == "c:");
#else
			yggr_test_assert(yggr::utf8_string(cont[0].first, cont[0].second) == "/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(yggr::utf8_string(cont[1].first, cont[1].second) == "aaa");
			yggr_test_assert(yggr::utf8_string(cont[2].first, cont[2].second) == "bbb");
		}
	}

	{
		std::vector<yggr::utf8_string> cont;
		yggr::utf8_string alloc_gen;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, yggr_utf8_str_path);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "yggr_utf8_string.txt");

		file_check_type::split_path(cont, yggr_utf8_str_path.c_str());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "yggr_utf8_string.txt");

		file_check_type::split_path(
			cont, yggr_utf8_str_path.begin(), yggr_utf8_str_path.end());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "yggr_utf8_string.txt");

		// ------------allocator version-------------------
		size = file_check_type::split_path(cont, yggr_utf8_str_path, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "yggr_utf8_string.txt");

		file_check_type::split_path(cont, yggr_utf8_str_path.c_str(), alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "yggr_utf8_string.txt");

		file_check_type::split_path(cont, yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
		yggr_test_assert(cont[3] == "yggr_utf8_string.txt");

		// path_only
		size = file_check_type::split_path(cont, yggr_utf8_str_path_only);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");

		size = file_check_type::split_path(cont, yggr_utf8_str_path_only, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == "c:");
#else
		yggr_test_assert(cont[0] == "/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");
	}

#ifndef YGGR_NO_CWCHAR

	std::cout << "---------------------------" << std::endl;

	{
		std::vector<std::wstring> cont;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, "");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == L"");

		size = file_check_type::split_path(cont, ".");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == L".");

		size = file_check_type::split_path(cont, "..");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == L"..");

		size = file_check_type::split_path(cont, "c:");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == L"c:");

		size = file_check_type::split_path(cont, "/");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == L"/");

		size = file_check_type::split_path(cont, "./");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == L".");

		size = file_check_type::split_path(cont, "../");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == L"..");

		size = file_check_type::split_path(cont, "c:/");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == L"c:");

		size = file_check_type::split_path(cont, "/aaa");
		yggr_test_assert(cont.size() == 2);
		yggr_test_assert(size == 2);
		yggr_test_assert(cont[0] == L"/");
		yggr_test_assert(cont[1] == L"aaa");

		size = file_check_type::split_path(cont, "/aaa/bbb");
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
		yggr_test_assert(cont[0] == L"/");
		yggr_test_assert(cont[1] == L"aaa");
		yggr_test_assert(cont[2] == L"bbb");

		size = file_check_type::split_path(cont, "/aaa/*");
#ifdef YGGR_AT_WINDOWS
		yggr_test_assert(size == 0);
#else
		yggr_test_assert(size == 3);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(cont[0] == L"/");
		yggr_test_assert(cont[1] == L"aaa");
		yggr_test_assert(cont[2] == L"*");
#endif // YGGR_AT_WINDOWS

		size = file_check_type::split_path(cont, "/aaa//");
		yggr_test_assert(size == 0);

		size = file_check_type::split_path(cont, "//aaa");
		yggr_test_assert(size == 0);

		size = file_check_type::split_path(cont, "aaa//bbb");
		yggr_test_assert(size == 0);
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::vector<std::string> cont;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, L"");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "");

		size = file_check_type::split_path(cont, L".");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == ".");

		size = file_check_type::split_path(cont, L"..");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "..");

		size = file_check_type::split_path(cont, L"c:");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "c:");

		size = file_check_type::split_path(cont, L"/");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "/");

		size = file_check_type::split_path(cont, L"./");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == ".");

		size = file_check_type::split_path(cont, L"../");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "..");

		size = file_check_type::split_path(cont, L"c:/");
		yggr_test_assert(cont.size() == 1);
		yggr_test_assert(size == 1);
		yggr_test_assert(cont[0] == "c:");

		size = file_check_type::split_path(cont, L"/aaa");
		yggr_test_assert(cont.size() == 2);
		yggr_test_assert(size == 2);
		yggr_test_assert(cont[0] == "/");
		yggr_test_assert(cont[1] == "aaa");

		size = file_check_type::split_path(cont, L"/aaa/bbb");
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
		yggr_test_assert(cont[0] == "/");
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "bbb");

		size = file_check_type::split_path(cont, L"/aaa/*");
#ifdef YGGR_AT_WINDOWS
		yggr_test_assert(size == 0);
#else
		yggr_test_assert(size == 3);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(cont[0] == "/");
		yggr_test_assert(cont[1] == "aaa");
		yggr_test_assert(cont[2] == "*");
#endif // YGGR_AT_WINDOWS

		size = file_check_type::split_path(cont, L"/aaa//");
		yggr_test_assert(size == 0);

		size = file_check_type::split_path(cont, L"//aaa");
		yggr_test_assert(size == 0);

		size = file_check_type::split_path(cont, L"aaa//bbb");
		yggr_test_assert(size == 0);
	}

	std::cout << "---------------------------" << std::endl;

	// c:/一级目录/二级目录/原始的_宽字符串.txt
	// /一级目录/二级目录/原始的_宽字符串.txt
	{
		std::vector< std::pair<const wchar_t*, const wchar_t*> > cont;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, native_wstr_path);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
		yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
		yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
		yggr_test_assert(std::wstring(cont[3].first, cont[3].second) == L"原始的_宽字符串.txt");

		file_check_type::split_path(cont, &native_wstr_path[0]);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
		yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
		yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
		yggr_test_assert(std::wstring(cont[3].first, cont[3].second) == L"原始的_宽字符串.txt");

		file_check_type::split_path(
			cont, &native_wstr_path[0],
			&native_wstr_path[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path));
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
		yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
		yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
		yggr_test_assert(std::wstring(cont[3].first, cont[3].second) == L"原始的_宽字符串.txt");

		// path_only
		size = file_check_type::split_path(cont, native_wstr_path_only);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
		yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
		yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
	}

	{
		std::vector<std::wstring> cont;
		std::wstring alloc_gen;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, native_wstr_path);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"原始的_宽字符串.txt");

		file_check_type::split_path(cont, &native_wstr_path[0]);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"原始的_宽字符串.txt");

		file_check_type::split_path(
			cont, &native_wstr_path[0], &native_wstr_path[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path));
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"原始的_宽字符串.txt");

		// ------------allocator version-------------------
		size = file_check_type::split_path(cont, native_wstr_path, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"原始的_宽字符串.txt");

		file_check_type::split_path(cont, &native_wstr_path[0], alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"原始的_宽字符串.txt");

		file_check_type::split_path(
			cont, &native_wstr_path[0],
			&native_wstr_path[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path),
			alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"原始的_宽字符串.txt");

		// path_only
		size = file_check_type::split_path(cont, native_wstr_path_only);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");

		// ------------allocator version-------------------
		size = file_check_type::split_path(cont, native_wstr_path_only, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "---------------------------" << std::endl;

	// c:/一级目录/二级目录/stl的_宽字符串.txt
	//	/一级目录/二级目录/stl的_宽字符串.txt
	{
		std::size_t size = 0;

		{
			std::vector< std::pair<std::wstring::const_iterator, std::wstring::const_iterator> > cont;
			size = file_check_type::split_path(cont, stl_wstr_path);
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
			yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
			yggr_test_assert(std::wstring(cont[3].first, cont[3].second) == L"stl的_宽字符串.txt");
		}

		{
			std::vector< std::pair<const wchar_t*, const wchar_t*> > cont;

			file_check_type::split_path(cont, stl_wstr_path.c_str());
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
			yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
			yggr_test_assert(std::wstring(cont[3].first, cont[3].second) == L"stl的_宽字符串.txt");
		}

		{
			std::vector< std::pair<std::wstring::const_iterator, std::wstring::const_iterator> > cont;
			file_check_type::split_path(cont, stl_wstr_path.begin(), stl_wstr_path.end());
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
			yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
			yggr_test_assert(std::wstring(cont[3].first, cont[3].second) == L"stl的_宽字符串.txt");
		}

		// path_only

		{
			std::vector< std::pair<std::wstring::const_iterator, std::wstring::const_iterator> > cont;
			size = file_check_type::split_path(cont, stl_wstr_path_only);
			yggr_test_assert(cont.size() == 3);
			yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
			yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
		}
	}

	{
		std::vector<std::wstring> cont;
		std::wstring alloc_gen;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, stl_wstr_path);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"stl的_宽字符串.txt");

		file_check_type::split_path(cont, stl_wstr_path.c_str());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"stl的_宽字符串.txt");

		file_check_type::split_path(
			cont, stl_wstr_path.begin(), stl_wstr_path.end());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"stl的_宽字符串.txt");

		// ------------allocator version-------------------
		size = file_check_type::split_path(cont, stl_wstr_path, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"stl的_宽字符串.txt");

		file_check_type::split_path(cont, stl_wstr_path.c_str(), alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"stl的_宽字符串.txt");

		file_check_type::split_path(cont, stl_wstr_path.begin(), stl_wstr_path.end(), alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"stl的_宽字符串.txt");

		//path_only
		size = file_check_type::split_path(cont, stl_wstr_path_only);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");

		size = file_check_type::split_path(cont, stl_wstr_path_only, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
	}

	// c:/一级目录/二级目录/boost的_宽字符串.txt
	//	/一级目录/二级目录/boost的_宽字符串.txt
	{
		std::size_t size = 0;

		{
			std::vector< std::pair<boost::container::wstring::const_iterator, boost::container::wstring::const_iterator> > cont;
			size = file_check_type::split_path(cont, boost_wstr_path);
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
			yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
			yggr_test_assert(std::wstring(cont[3].first, cont[3].second) == L"boost的_宽字符串.txt");
		}

		{
			std::vector< std::pair<const wchar_t*, const wchar_t*> > cont;

			file_check_type::split_path(cont, boost_wstr_path.c_str());
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
			yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
			yggr_test_assert(std::wstring(cont[3].first, cont[3].second) == L"boost的_宽字符串.txt");
		}

		{
			std::vector< std::pair<boost::container::wstring::const_iterator, boost::container::wstring::const_iterator> > cont;
			file_check_type::split_path(cont, boost_wstr_path.begin(), boost_wstr_path.end());
			yggr_test_assert(cont.size() == 4);
			yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
			yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
			yggr_test_assert(std::wstring(cont[3].first, cont[3].second) == L"boost的_宽字符串.txt");
		}

		// path_only

		{
			std::vector< std::pair<boost::container::wstring::const_iterator, boost::container::wstring::const_iterator> > cont;
			size = file_check_type::split_path(cont, boost_wstr_path_only);
			yggr_test_assert(cont.size() == 3);
			yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"c:");
#else
			yggr_test_assert(std::wstring(cont[0].first, cont[0].second) == L"/");
#endif // YGGR_AT_WINDOWS
			yggr_test_assert(std::wstring(cont[1].first, cont[1].second) == L"一级目录");
			yggr_test_assert(std::wstring(cont[2].first, cont[2].second) == L"二级目录");
		}
	}

	{
		std::vector<std::wstring> cont;
		std::wstring alloc_gen;
		std::size_t size = 0;

		size = file_check_type::split_path(cont, boost_wstr_path);
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"boost的_宽字符串.txt");

		file_check_type::split_path(cont, boost_wstr_path.c_str());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"boost的_宽字符串.txt");

		file_check_type::split_path(
			cont, boost_wstr_path.begin(), boost_wstr_path.end());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"boost的_宽字符串.txt");

		// ------------allocator version-------------------
		size = file_check_type::split_path(cont, boost_wstr_path, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"boost的_宽字符串.txt");

		file_check_type::split_path(cont, boost_wstr_path.c_str(), alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"boost的_宽字符串.txt");

		file_check_type::split_path(cont, boost_wstr_path.begin(), boost_wstr_path.end(), alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 4);
		yggr_test_assert(size == 4);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
		yggr_test_assert(cont[3] == L"boost的_宽字符串.txt");

		//path_only
		size = file_check_type::split_path(cont, boost_wstr_path_only);
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");

		size = file_check_type::split_path(cont, boost_wstr_path_only, alloc_gen.get_allocator());
		yggr_test_assert(cont.size() == 3);
		yggr_test_assert(size == 3);
#if defined(YGGR_AT_WINDOWS) && !defined(YGGR_TEST_USING_LINUX_PATH)
		yggr_test_assert(cont[0] == L"c:");
#else
		yggr_test_assert(cont[0] == L"/");
#endif // YGGR_AT_WINDOWS
		yggr_test_assert(cont[1] == L"一级目录");
		yggr_test_assert(cont[2] == L"二级目录");
	}

	std::cout << "-----------test_split_path successed----------------" << std::endl;
}

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_split_path();

	wait_any_key(argc, argv);
	return 0;
}
