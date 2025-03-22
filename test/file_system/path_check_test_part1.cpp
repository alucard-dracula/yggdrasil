//path_check_test_part1.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

//#define YGGR_NO_CWCHAR
#define BOOST_HAS_ICU

//#define YGGR_AT_LINUX

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

void test_check_file_name(void)
{
	// specifiers path
	yggr_test_assert(file_check_type::check_file_name(""));
	yggr_test_assert(file_check_type::check_file_name("."));
	yggr_test_assert(file_check_type::check_file_name(".."));
	yggr_test_assert(!file_check_type::check_file_name("/"));
	yggr_test_assert(!file_check_type::check_file_name("./"));
	yggr_test_assert(!file_check_type::check_file_name("../"));

	// invalidate path
#ifdef YGGR_AT_WINDOWS
	yggr_test_assert(!file_check_type::check_file_name("/*"));
	yggr_test_assert(!file_check_type::check_file_name("//"));
#else
	yggr_test_assert(file_check_type::check_file_name("/*"));
	yggr_test_assert(!file_check_type::check_file_name("//"));
#endif // YGGR_AT_WINDOWS

	// true
	yggr_test_assert(file_check_type::check_file_name(native_str_path));
	yggr_test_assert(file_check_type::check_file_name(&native_str_path[0]));
	yggr_test_assert(file_check_type::check_file_name(
			&native_str_path[0], 
			&native_str_path[0] + yggr::charset::charset_base_foo::strlen(native_str_path)));
	
	yggr_test_assert(file_check_type::check_file_name(stl_str_path));
	yggr_test_assert(file_check_type::check_file_name(stl_str_path.c_str()));
	yggr_test_assert(file_check_type::check_file_name(stl_str_path.begin(), stl_str_path.end()));

	yggr_test_assert(file_check_type::check_file_name(boost_str_path));
	yggr_test_assert(file_check_type::check_file_name(boost_str_path.c_str()));
	yggr_test_assert(file_check_type::check_file_name(boost_str_path.begin(), boost_str_path.end()));

	yggr_test_assert(file_check_type::check_file_name(yggr_utf8_str_path));
	yggr_test_assert(file_check_type::check_file_name(yggr_utf8_str_path.c_str()));
	yggr_test_assert(file_check_type::check_file_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end()));

	// false
	yggr_test_assert(!file_check_type::check_file_name(native_str_path_only));
	yggr_test_assert(!file_check_type::check_file_name(&native_str_path_only[0]));
	yggr_test_assert(!file_check_type::check_file_name(
				&native_str_path_only[0], 
				&native_str_path_only[0] + yggr::charset::charset_base_foo::strlen(native_str_path_only)));

	yggr_test_assert(!file_check_type::check_file_name(stl_str_path_only));
	yggr_test_assert(!file_check_type::check_file_name(stl_str_path_only.c_str()));
	yggr_test_assert(!file_check_type::check_file_name(stl_str_path_only.begin(), stl_str_path_only.end()));

	yggr_test_assert(!file_check_type::check_file_name(boost_str_path_only));
	yggr_test_assert(!file_check_type::check_file_name(boost_str_path_only.c_str()));
	yggr_test_assert(!file_check_type::check_file_name(boost_str_path_only.begin(), boost_str_path_only.end()));

	yggr_test_assert(!file_check_type::check_file_name(yggr_utf8_str_path_only));
	yggr_test_assert(!file_check_type::check_file_name(yggr_utf8_str_path_only.c_str()));
	yggr_test_assert(!file_check_type::check_file_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end()));

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_NO_WSTRING))

	// specifiers path
	yggr_test_assert(file_check_type::check_file_name(L""));
	yggr_test_assert(file_check_type::check_file_name(L"."));
	yggr_test_assert(file_check_type::check_file_name(L".."));
	yggr_test_assert(!file_check_type::check_file_name(L"/"));
	yggr_test_assert(!file_check_type::check_file_name(L"./"));
	yggr_test_assert(!file_check_type::check_file_name(L"../"));

	// invalidate path
#ifdef YGGR_AT_WINDOWS
	yggr_test_assert(!file_check_type::check_file_name(L"/*"));
	yggr_test_assert(!file_check_type::check_file_name(L"//"));
#else
	yggr_test_assert(file_check_type::check_file_name(L"/*"));
	yggr_test_assert(!file_check_type::check_file_name(L"//"));
#endif // YGGR_AT_WINDOWS

	// true
	yggr_test_assert(file_check_type::check_file_name(native_wstr_path));
	yggr_test_assert(file_check_type::check_file_name(&native_wstr_path[0]));
	yggr_test_assert(file_check_type::check_file_name(
			&native_wstr_path[0], 
			&native_wstr_path[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path)));


	yggr_test_assert(file_check_type::check_file_name(stl_wstr_path));
	yggr_test_assert(file_check_type::check_file_name(stl_wstr_path.c_str()));
	yggr_test_assert(file_check_type::check_file_name(stl_wstr_path.begin(), stl_wstr_path.end()));

	yggr_test_assert(file_check_type::check_file_name(boost_wstr_path));
	yggr_test_assert(file_check_type::check_file_name(boost_wstr_path.c_str()));
	yggr_test_assert(file_check_type::check_file_name(boost_wstr_path.begin(), boost_wstr_path.end()));

	yggr_test_assert(file_check_type::check_file_name(yggr_utf8_str_path));
	yggr_test_assert(file_check_type::check_file_name(yggr_utf8_str_path.c_str()));
	yggr_test_assert(file_check_type::check_file_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end()));

	// false
	yggr_test_assert(!file_check_type::check_file_name(native_wstr_path_only));
	yggr_test_assert(!file_check_type::check_file_name(&native_wstr_path_only[0]));
	yggr_test_assert(!file_check_type::check_file_name(
				&native_wstr_path_only[0], 
				&native_wstr_path_only[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path_only)));

	yggr_test_assert(!file_check_type::check_file_name(stl_wstr_path_only));
	yggr_test_assert(!file_check_type::check_file_name(stl_wstr_path_only.c_str()));
	yggr_test_assert(!file_check_type::check_file_name(stl_wstr_path_only.begin(), stl_wstr_path_only.end()));

	yggr_test_assert(!file_check_type::check_file_name(boost_wstr_path_only));
	yggr_test_assert(!file_check_type::check_file_name(boost_wstr_path_only.c_str()));
	yggr_test_assert(!file_check_type::check_file_name(boost_wstr_path_only.begin(), boost_wstr_path_only.end()));

	yggr_test_assert(!file_check_type::check_file_name(yggr_utf8_str_path_only));
	yggr_test_assert(!file_check_type::check_file_name(yggr_utf8_str_path_only.c_str()));
	yggr_test_assert(!file_check_type::check_file_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end()));

#endif // !(defined(BOOST_NO_CWCHAR) || defined(BOOST_NO_STD_WSTRING)) 
	std::cout << "-----------test_check_file_name successed----------------" << std::endl;
}

void test_check_path_name(void)
{
	// specifiers path
	yggr_test_assert(file_check_type::check_path_name(""));
	yggr_test_assert(file_check_type::check_path_name("."));
	yggr_test_assert(file_check_type::check_path_name(".."));
	yggr_test_assert(file_check_type::check_path_name("/"));
	yggr_test_assert(file_check_type::check_path_name("./"));
	yggr_test_assert(file_check_type::check_path_name("../"));

	// invalidate path
#ifdef YGGR_AT_WINDOWS
	yggr_test_assert(!file_check_type::check_path_name("/*"));
	yggr_test_assert(!file_check_type::check_path_name("//"));
#else
	yggr_test_assert(file_check_type::check_path_name("/*"));
	yggr_test_assert(!file_check_type::check_path_name("//"));
#endif // YGGR_AT_WINDOWS

	// true
	yggr_test_assert(file_check_type::check_path_name(native_str_path));
	yggr_test_assert(file_check_type::check_path_name(&native_str_path[0]));
	yggr_test_assert(file_check_type::check_path_name(
			&native_str_path[0], 
			&native_str_path[0] + yggr::charset::charset_base_foo::strlen(native_str_path)));
	
	yggr_test_assert(file_check_type::check_path_name(stl_str_path));
	yggr_test_assert(file_check_type::check_path_name(stl_str_path.c_str()));
	yggr_test_assert(file_check_type::check_path_name(stl_str_path.begin(), stl_str_path.end()));

	yggr_test_assert(file_check_type::check_path_name(boost_str_path));
	yggr_test_assert(file_check_type::check_path_name(boost_str_path.c_str()));
	yggr_test_assert(file_check_type::check_path_name(boost_str_path.begin(), boost_str_path.end()));

	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path));
	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path.c_str()));
	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end()));

	// false
	yggr_test_assert(file_check_type::check_path_name(native_str_path_only));
	yggr_test_assert(file_check_type::check_path_name(&native_str_path_only[0]));
	yggr_test_assert(file_check_type::check_path_name(
				&native_str_path_only[0], 
				&native_str_path_only[0] + yggr::charset::charset_base_foo::strlen(native_str_path_only)));

	yggr_test_assert(file_check_type::check_path_name(stl_str_path_only));
	yggr_test_assert(file_check_type::check_path_name(stl_str_path_only.c_str()));
	yggr_test_assert(file_check_type::check_path_name(stl_str_path_only.begin(), stl_str_path_only.end()));

	yggr_test_assert(file_check_type::check_path_name(boost_str_path_only));
	yggr_test_assert(file_check_type::check_path_name(boost_str_path_only.c_str()));
	yggr_test_assert(file_check_type::check_path_name(boost_str_path_only.begin(), boost_str_path_only.end()));

	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path_only));
	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path_only.c_str()));
	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end()));

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_NO_WSTRING))

	// specifiers path
	yggr_test_assert(file_check_type::check_path_name(L""));
	yggr_test_assert(file_check_type::check_path_name(L"."));
	yggr_test_assert(file_check_type::check_path_name(L".."));
	yggr_test_assert(file_check_type::check_path_name(L"/"));
	yggr_test_assert(file_check_type::check_path_name(L"./"));
	yggr_test_assert(file_check_type::check_path_name(L"../"));

	// invalidate path
#ifdef YGGR_AT_WINDOWS
	yggr_test_assert(!file_check_type::check_path_name(L"/*"));
	yggr_test_assert(!file_check_type::check_path_name(L"//"));
#else
	yggr_test_assert(file_check_type::check_path_name(L"/*"));
	yggr_test_assert(!file_check_type::check_path_name(L"//"));
#endif // YGGR_AT_WINDOWS

	// true
	yggr_test_assert(file_check_type::check_path_name(native_wstr_path));
	yggr_test_assert(file_check_type::check_path_name(&native_wstr_path[0]));
	yggr_test_assert(file_check_type::check_path_name(
			&native_wstr_path[0], 
			&native_wstr_path[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path)));


	yggr_test_assert(file_check_type::check_path_name(stl_wstr_path));
	yggr_test_assert(file_check_type::check_path_name(stl_wstr_path.c_str()));
	yggr_test_assert(file_check_type::check_path_name(stl_wstr_path.begin(), stl_wstr_path.end()));

	yggr_test_assert(file_check_type::check_path_name(boost_wstr_path));
	yggr_test_assert(file_check_type::check_path_name(boost_wstr_path.c_str()));
	yggr_test_assert(file_check_type::check_path_name(boost_wstr_path.begin(), boost_wstr_path.end()));

	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path));
	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path.c_str()));
	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end()));

	// false
	yggr_test_assert(file_check_type::check_path_name(native_wstr_path_only));
	yggr_test_assert(file_check_type::check_path_name(&native_wstr_path_only[0]));
	yggr_test_assert(file_check_type::check_path_name(
				&native_wstr_path_only[0], 
				&native_wstr_path_only[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path_only)));

	yggr_test_assert(file_check_type::check_path_name(stl_wstr_path_only));
	yggr_test_assert(file_check_type::check_path_name(stl_wstr_path_only.c_str()));
	yggr_test_assert(file_check_type::check_path_name(stl_wstr_path_only.begin(), stl_wstr_path_only.end()));

	yggr_test_assert(file_check_type::check_path_name(boost_wstr_path_only));
	yggr_test_assert(file_check_type::check_path_name(boost_wstr_path_only.c_str()));
	yggr_test_assert(file_check_type::check_path_name(boost_wstr_path_only.begin(), boost_wstr_path_only.end()));

	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path_only));
	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path_only.c_str()));
	yggr_test_assert(file_check_type::check_path_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end()));

#endif // !(defined(BOOST_NO_CWCHAR) || defined(BOOST_NO_STD_WSTRING)) 
	std::cout << "-----------test_check_path_name successed----------------" << std::endl;
}

void test_parsing_file_path_and_name(void)
{
	// specifiers path
	{
		std::string path;
		std::string name;
		bool bright = false;
		bright = file_check_type::parsing_file_path_and_name("", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == "");
		yggr_test_assert(name == "");
		std::cout << "path = " << path << ", name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_path_and_name(".", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == "");
		yggr_test_assert(name == ".");
		std::cout << "path = " << path << ", name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_path_and_name("..", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == "");
		yggr_test_assert(name == "..");
		std::cout << "path = " << path << ", name = " << name << std::endl;

		bright = file_check_type::parsing_file_path_and_name("c:", path, name);
		yggr_test_assert(bright);
#ifdef YGGR_AT_WINDOWS
		yggr_test_assert(path == "c:");
		yggr_test_assert(name == "");
#else
		yggr_test_assert(path == "");
		yggr_test_assert(name == "c:");
#endif //YGGR_AT_WINDOWS
		std::cout << "path = " << path << ", name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_path_and_name("/", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == "/");
		yggr_test_assert(name == "");
		std::cout << "path = " << path << ", name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_path_and_name("./", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == "./");
		yggr_test_assert(name == "");
		std::cout << "path = " << path << ", name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_path_and_name("../", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == "../");
		yggr_test_assert(name == "");
		std::cout << "path = " << path << ", name = " << name << std::endl;

		bright = file_check_type::parsing_file_path_and_name("c:/", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == "c:/");
		yggr_test_assert(name == "");
		std::cout << "path = " << path << ", name = " << name << std::endl;

#ifdef YGGR_AT_WINDOWS
		bright = file_check_type::parsing_file_path_and_name("/*", path, name);
		yggr_test_assert(!bright);
		bright = file_check_type::parsing_file_path_and_name("//", path, name);
		yggr_test_assert(!bright);
#else
		bright = file_check_type::parsing_file_path_and_name("/*", path, name);
		yggr_test_assert(bright);
		bright = file_check_type::parsing_file_path_and_name("//", path, name);
		yggr_test_assert(!bright);
#endif // YGGR_AT_WINDOWS
	}
	
	std::cout << "---------------------------" << std::endl;

	{	
		std::string path;
		std::string name;
		file_check_type::parsing_file_path_and_name(native_str_path, path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(&native_str_path[0], path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(
			&native_str_path[0], &native_str_path[0] + yggr::charset::charset_base_foo::strlen(native_str_path),
			path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;

		file_check_type::parsing_file_path_and_name(native_str_path_only, path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(&native_str_path_only[0], path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(
			&native_str_path_only[0], &native_str_path_only[0] + yggr::charset::charset_base_foo::strlen(native_str_path_only),
			path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::string path;
		yggr::string name;
		file_check_type::parsing_file_path_and_name(stl_str_path, path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(stl_str_path.c_str(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(stl_str_path.begin(), stl_str_path.end(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;

		file_check_type::parsing_file_path_and_name(stl_str_path_only, path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(stl_str_path_only.c_str(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(stl_str_path_only.begin(), stl_str_path_only.end(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::utf8_string path;
		yggr::utf8_string name;
		file_check_type::parsing_file_path_and_name(boost_str_path, path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(boost_str_path.c_str(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(boost_str_path.begin(), boost_str_path.end(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;

		file_check_type::parsing_file_path_and_name(boost_str_path_only, path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(boost_str_path_only.c_str(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(boost_str_path_only.begin(), boost_str_path_only.end(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::string path;
		yggr::utf8_string name;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path, path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path.c_str(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;

		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only, path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only.c_str(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), path, name);
		std::cout << "path = " << path << ", name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::vector<char> path;
		yggr::list<char> name;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path, path, name);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) 
					<< ", lst_name = " << std::string(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path.c_str(), path, name);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) 
					<< ", lst_name = " << std::string(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), path, name);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) 
					<< ", lst_name = " << std::string(name.begin(), name.end())  << std::endl;

		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only, path, name);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) 
					<< ", lst_name = " << std::string(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only.c_str(), path, name);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) 
					<< ", lst_name = " << std::string(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), path, name);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) 
					<< ", lst_name = " << std::string(name.begin(), name.end())  << std::endl;
	}
	
#ifndef YGGR_NO_CWCHAR

	std::cout << "---------------------------" << std::endl;

	// specifiers path
	{
		std::wstring path;
		std::wstring name;
		bool bright = false;
		bright = file_check_type::parsing_file_path_and_name("", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"");
		yggr_test_assert(name == L"");
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_path_and_name(".", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"");
		yggr_test_assert(name == L".");
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_path_and_name("..", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"");
		yggr_test_assert(name == L"..");
		std::wcout << "path = " << path << ", name = " << name << std::endl;

		bright = file_check_type::parsing_file_path_and_name("c:", path, name);
		yggr_test_assert(bright);
#ifdef YGGR_AT_WINDOWS
		yggr_test_assert(path == L"c:");
		yggr_test_assert(name == L"");
#else
		yggr_test_assert(path == L"");
		yggr_test_assert(name == L"c:");
#endif //YGGR_AT_WINDOWS
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_path_and_name("/", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"/");
		yggr_test_assert(name == L"");
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_path_and_name("./", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"./");
		yggr_test_assert(name == L"");
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_path_and_name("../", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"../");
		yggr_test_assert(name == L"");
		std::wcout << "path = " << path << ", name = " << name << std::endl;

		bright = file_check_type::parsing_file_path_and_name("c:/", path, name);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"c:/");
		yggr_test_assert(name == L"");
		std::wcout << "path = " << path << ", name = " << name << std::endl;

#ifdef YGGR_AT_WINDOWS
		bright = file_check_type::parsing_file_path_and_name(L"/*", path, name);
		yggr_test_assert(!bright);
		bright = file_check_type::parsing_file_path_and_name(L"//", path, name);
		yggr_test_assert(!bright);
#else
		bright = file_check_type::parsing_file_path_and_name(L"/*", path, name);
		yggr_test_assert(bright);
		bright = file_check_type::parsing_file_path_and_name(L"//", path, name);
		yggr_test_assert(!bright);
#endif // YGGR_AT_WINDOWS
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::wstring path;
		std::wstring name;
		file_check_type::parsing_file_path_and_name(native_wstr_path, path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(&native_wstr_path[0], path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(
			&native_wstr_path[0], &native_wstr_path[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path),
			path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;

		file_check_type::parsing_file_path_and_name(native_wstr_path_only, path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(&native_wstr_path_only[0], path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(
			&native_wstr_path_only[0], &native_wstr_path_only[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path_only),
			path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::wstring path;
		yggr::wstring name;
		file_check_type::parsing_file_path_and_name(stl_wstr_path, path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(stl_wstr_path.c_str(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(stl_wstr_path.begin(), stl_wstr_path.end(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;

		file_check_type::parsing_file_path_and_name(stl_wstr_path_only, path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(stl_wstr_path_only.c_str(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(stl_wstr_path_only.begin(), stl_wstr_path_only.end(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::utf8_string path;
		yggr::utf8_string name;
		file_check_type::parsing_file_path_and_name(boost_wstr_path, path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(boost_wstr_path.c_str(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(boost_wstr_path.begin(), boost_wstr_path.end(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;

		file_check_type::parsing_file_path_and_name(boost_wstr_path_only, path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(boost_wstr_path_only.c_str(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(boost_wstr_path_only.begin(), boost_wstr_path_only.end(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::wstring path;
		yggr::utf8_string name;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path, path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path.c_str(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;

		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only, path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only.c_str(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), path, name);
		std::wcout << "path = " << path << ", name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::vector<wchar_t> path;
		yggr::list<wchar_t> name;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path, path, name);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) 
					<< ", lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path.c_str(), path, name);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) 
					<< ", lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), path, name);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) 
					<< ", lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;

		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only, path, name);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) 
					<< ", lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only.c_str(), path, name);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) 
					<< ", lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_path_and_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), path, name);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) 
					<< ", lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;
	}

#endif // YGGR_NO_CWCHAR
	std::cout << "-----------test_parsing_file_path_and_name successed----------------" << std::endl;
}

void test_parsing_file_path(void)
{
	// specifiers path
	{
		std::string path;
		
		bool bright = false;
		bright = file_check_type::parsing_file_path("", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == "");
		std::cout << "path = " << path << std::endl;
		
		bright = file_check_type::parsing_file_path(".", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == "");
		std::cout << "path = " << path << std::endl;
		
		bright = file_check_type::parsing_file_path("..", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == "");
		std::cout << "path = " << path << std::endl;

		bright = file_check_type::parsing_file_path("c:", path);
		yggr_test_assert(bright);
#ifdef YGGR_AT_WINDOWS
		yggr_test_assert(path == "c:");
		
#else
		yggr_test_assert(path == "");
#endif //YGGR_AT_WINDOWS
		std::cout << "path = " << path << std::endl;
		
		bright = file_check_type::parsing_file_path("/", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == "/");
		std::cout << "path = " << path << std::endl;
		
		bright = file_check_type::parsing_file_path("./", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == "./");
		std::cout << "path = " << path << std::endl;
		
		bright = file_check_type::parsing_file_path("../", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == "../");
		
		std::cout << "path = " << path << std::endl;

		bright = file_check_type::parsing_file_path("c:/", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == "c:/");
		std::cout << "path = " << path << std::endl;

#ifdef YGGR_AT_WINDOWS
		bright = file_check_type::parsing_file_path("/*", path);
		yggr_test_assert(!bright);
		bright = file_check_type::parsing_file_path("//", path);
		yggr_test_assert(!bright);
#else
		bright = file_check_type::parsing_file_path("/*", path);
		yggr_test_assert(bright);
		bright = file_check_type::parsing_file_path("//", path);
		yggr_test_assert(!bright);
#endif // YGGR_AT_WINDOWS
	}
	
	std::cout << "---------------------------" << std::endl;

	{	
		std::string path;
		
		file_check_type::parsing_file_path(native_str_path, path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(&native_str_path[0], path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(
			&native_str_path[0], &native_str_path[0] + yggr::charset::charset_base_foo::strlen(native_str_path),
			path);
		std::cout << "path = " << path << std::endl;

		file_check_type::parsing_file_path(native_str_path_only, path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(&native_str_path_only[0], path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(
			&native_str_path_only[0], &native_str_path_only[0] + yggr::charset::charset_base_foo::strlen(native_str_path_only),
			path);
		std::cout << "path = " << path << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::string path;
		file_check_type::parsing_file_path(stl_str_path, path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(stl_str_path.c_str(), path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(stl_str_path.begin(), stl_str_path.end(), path);
		std::cout << "path = " << path << std::endl;

		file_check_type::parsing_file_path(stl_str_path_only, path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(stl_str_path_only.c_str(), path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(stl_str_path_only.begin(), stl_str_path_only.end(), path);
		std::cout << "path = " << path << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::utf8_string path;
		file_check_type::parsing_file_path(boost_str_path, path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(boost_str_path.c_str(), path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(boost_str_path.begin(), boost_str_path.end(), path);
		std::cout << "path = " << path << std::endl;

		file_check_type::parsing_file_path(boost_str_path_only, path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(boost_str_path_only.c_str(), path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(boost_str_path_only.begin(), boost_str_path_only.end(), path);
		std::cout << "path = " << path << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::string path;
		file_check_type::parsing_file_path(yggr_utf8_str_path, path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path.c_str(), path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), path);
		std::cout << "path = " << path << std::endl;

		file_check_type::parsing_file_path(yggr_utf8_str_path_only, path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path_only.c_str(), path);
		std::cout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), path);
		std::cout << "path = " << path << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::vector<char> path;
		file_check_type::parsing_file_path(yggr_utf8_str_path, path);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path.c_str(), path);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), path);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) << std::endl;

		file_check_type::parsing_file_path(yggr_utf8_str_path_only, path);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path_only.c_str(), path);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), path);
		std::cout << "vt_path = " << std::string(path.begin(), path.end()) << std::endl;
	}
	
#ifndef YGGR_NO_CWCHAR

	std::cout << "---------------------------" << std::endl;

	// specifiers path
	{
		std::wstring path;
		bool bright = false;
		bright = file_check_type::parsing_file_path("", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"");
		std::wcout << "path = " << path << std::endl;
		
		bright = file_check_type::parsing_file_path(".", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"");
		std::wcout << "path = " << path << std::endl;
		
		bright = file_check_type::parsing_file_path("..", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"");
		std::wcout << "path = " << path << std::endl;

		bright = file_check_type::parsing_file_path("c:", path);
		yggr_test_assert(bright);
#ifdef YGGR_AT_WINDOWS
		yggr_test_assert(path == L"c:");
#else
		yggr_test_assert(path == L"");
#endif //YGGR_AT_WINDOWS
		std::wcout << "path = " << path << std::endl;
		
		bright = file_check_type::parsing_file_path("/", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"/");
		std::wcout << "path = " << path << std::endl;
		
		bright = file_check_type::parsing_file_path("./", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"./");
		std::wcout << "path = " << path << std::endl;
		
		bright = file_check_type::parsing_file_path("../", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"../");
		std::wcout << "path = " << path << std::endl;

		bright = file_check_type::parsing_file_path("c:/", path);
		yggr_test_assert(bright);
		yggr_test_assert(path == L"c:/");
		std::wcout << "path = " << path << std::endl;

#ifdef YGGR_AT_WINDOWS
		bright = file_check_type::parsing_file_path(L"/*", path);
		yggr_test_assert(!bright);
		bright = file_check_type::parsing_file_path(L"//", path);
		yggr_test_assert(!bright);
#else
		bright = file_check_type::parsing_file_path(L"/*", path);
		yggr_test_assert(bright);
		bright = file_check_type::parsing_file_path(L"//", path);
		yggr_test_assert(!bright);
#endif // YGGR_AT_WINDOWS
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::wstring path;
		file_check_type::parsing_file_path(native_wstr_path, path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(&native_wstr_path[0], path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(
			&native_wstr_path[0], &native_wstr_path[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path),
			path);
		std::wcout << "path = " << path << std::endl;

		file_check_type::parsing_file_path(native_wstr_path_only, path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(&native_wstr_path_only[0], path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(
			&native_wstr_path_only[0], &native_wstr_path_only[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path_only),
			path);
		std::wcout << "path = " << path << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::wstring path;
		file_check_type::parsing_file_path(stl_wstr_path, path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(stl_wstr_path.c_str(), path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(stl_wstr_path.begin(), stl_wstr_path.end(), path);
		std::wcout << "path = " << path << std::endl;

		file_check_type::parsing_file_path(stl_wstr_path_only, path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(stl_wstr_path_only.c_str(), path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(stl_wstr_path_only.begin(), stl_wstr_path_only.end(), path);
		std::wcout << "path = " << path << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::utf8_string path;
		file_check_type::parsing_file_path(boost_wstr_path, path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(boost_wstr_path.c_str(), path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(boost_wstr_path.begin(), boost_wstr_path.end(), path);
		std::wcout << "path = " << path << std::endl;

		file_check_type::parsing_file_path(boost_wstr_path_only, path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(boost_wstr_path_only.c_str(), path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(boost_wstr_path_only.begin(), boost_wstr_path_only.end(), path);
		std::wcout << "path = " << path << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::wstring path;
		file_check_type::parsing_file_path(yggr_utf8_str_path, path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path.c_str(), path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), path);
		std::wcout << "path = " << path << std::endl;

		file_check_type::parsing_file_path(yggr_utf8_str_path_only, path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path_only.c_str(), path);
		std::wcout << "path = " << path << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), path);
		std::wcout << "path = " << path << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::vector<wchar_t> path;
		file_check_type::parsing_file_path(yggr_utf8_str_path, path);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path.c_str(), path);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), path);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) << std::endl;

		file_check_type::parsing_file_path(yggr_utf8_str_path_only, path);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path_only.c_str(), path);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) << std::endl;
		file_check_type::parsing_file_path(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), path);
		std::wcout << "vt_path = " << std::wstring(path.begin(), path.end()) << std::endl;
	}

#endif // YGGR_NO_CWCHAR
	std::cout << "-----------test_parsing_file_path successed----------------" << std::endl;
}

void test_parsing_file_name(void)
{
	// specifiers path
	{
		std::string name;
		bool bright = false;
		bright = file_check_type::parsing_file_name("", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == "");
		std::cout << "name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_name(".", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == ".");
		std::cout << "name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_name("..", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == "..");
		std::cout << "name = " << name << std::endl;

		bright = file_check_type::parsing_file_name("c:", name);
		yggr_test_assert(bright);
#ifdef YGGR_AT_WINDOWS
		yggr_test_assert(name == "");
#else
		yggr_test_assert(name == "c:");
#endif //YGGR_AT_WINDOWS
		std::cout << "name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_name("/", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == "");
		std::cout << "name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_name("./", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == "");
		std::cout << "name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_name("../", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == "");
		std::cout << "name = " << name << std::endl;

		bright = file_check_type::parsing_file_name("c:/", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == "");
		std::cout << "name = " << name << std::endl;

#ifdef YGGR_AT_WINDOWS
		bright = file_check_type::parsing_file_name("/*", name);
		yggr_test_assert(!bright);
		bright = file_check_type::parsing_file_name("//", name);
		yggr_test_assert(!bright);
#else
		bright = file_check_type::parsing_file_name("/*", name);
		yggr_test_assert(bright);
		bright = file_check_type::parsing_file_name("//", name);
		yggr_test_assert(!bright);
#endif // YGGR_AT_WINDOWS
	}
	
	std::cout << "---------------------------" << std::endl;

	{
		std::string name;
		file_check_type::parsing_file_name(native_str_path, name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(&native_str_path[0], name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(
			&native_str_path[0], &native_str_path[0] + yggr::charset::charset_base_foo::strlen(native_str_path),
			name);
		std::cout << "name = " << name << std::endl;

		file_check_type::parsing_file_name(native_str_path_only, name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(&native_str_path_only[0], name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(
			&native_str_path_only[0], &native_str_path_only[0] + yggr::charset::charset_base_foo::strlen(native_str_path_only),
			name);
		std::cout << "name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::string name;
		file_check_type::parsing_file_name(stl_str_path, name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(stl_str_path.c_str(), name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(stl_str_path.begin(), stl_str_path.end(), name);
		std::cout << "name = " << name << std::endl;

		file_check_type::parsing_file_name(stl_str_path_only, name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(stl_str_path_only.c_str(), name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(stl_str_path_only.begin(), stl_str_path_only.end(), name);
		std::cout << "name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::utf8_string name;
		file_check_type::parsing_file_name(boost_str_path, name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(boost_str_path.c_str(), name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(boost_str_path.begin(), boost_str_path.end(), name);
		std::cout << "name = " << name << std::endl;

		file_check_type::parsing_file_name(boost_str_path_only, name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(boost_str_path_only.c_str(), name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(boost_str_path_only.begin(), boost_str_path_only.end(), name);
		std::cout << "name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::utf8_string name;
		file_check_type::parsing_file_name(yggr_utf8_str_path, name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path.c_str(), name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), name);
		std::cout << "name = " << name << std::endl;

		file_check_type::parsing_file_name(yggr_utf8_str_path_only, name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path_only.c_str(), name);
		std::cout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), name);
		std::cout << "name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::list<char> name;
		file_check_type::parsing_file_name(yggr_utf8_str_path, name);
		std::cout << "lst_name = " << std::string(name.begin(), name.end()) << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path.c_str(), name);
		std::cout << "lst_name = " << std::string(name.begin(), name.end()) << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), name);
		std::cout << "lst_name = " << std::string(name.begin(), name.end()) << std::endl;

		file_check_type::parsing_file_name(yggr_utf8_str_path_only, name);
		std::cout << "lst_name = " << std::string(name.begin(), name.end()) << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path_only.c_str(), name);
		std::cout << "lst_name = " << std::string(name.begin(), name.end()) << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), name);
		std::cout << "lst_name = " << std::string(name.begin(), name.end()) << std::endl;
	}
	
#ifndef YGGR_NO_CWCHAR

	std::cout << "---------------------------" << std::endl;

	// specifiers path
	{
		std::wstring name;
		bool bright = false;
		bright = file_check_type::parsing_file_name("", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == L"");
		std::wcout << "name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_name(".", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == L".");
		std::wcout << "name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_name("..", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == L"..");
		std::wcout << "name = " << name << std::endl;

		bright = file_check_type::parsing_file_name("c:", name);
		yggr_test_assert(bright);
#ifdef YGGR_AT_WINDOWS
		yggr_test_assert(name == L"");
#else
		yggr_test_assert(name == L"c:");
#endif //YGGR_AT_WINDOWS
		std::wcout << "name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_name("/", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == L"");
		std::wcout << "name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_name("./", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == L"");
		std::wcout << "name = " << name << std::endl;
		
		bright = file_check_type::parsing_file_name("../", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == L"");
		std::wcout << "name = " << name << std::endl;

		bright = file_check_type::parsing_file_name("c:/", name);
		yggr_test_assert(bright);
		yggr_test_assert(name == L"");
		std::wcout << "name = " << name << std::endl;

#ifdef YGGR_AT_WINDOWS
		bright = file_check_type::parsing_file_name(L"/*", name);
		yggr_test_assert(!bright);
		bright = file_check_type::parsing_file_name(L"//", name);
		yggr_test_assert(!bright);
#else
		bright = file_check_type::parsing_file_name(L"/*", name);
		yggr_test_assert(bright);
		bright = file_check_type::parsing_file_name(L"//", name);
		yggr_test_assert(!bright);
#endif // YGGR_AT_WINDOWS
	}

	std::cout << "---------------------------" << std::endl;

	{
		std::wstring name;
		file_check_type::parsing_file_name(native_wstr_path, name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(&native_wstr_path[0], name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(
			&native_wstr_path[0], &native_wstr_path[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path),
			name);
		std::wcout << "name = " << name << std::endl;

		file_check_type::parsing_file_name(native_wstr_path_only, name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(&native_wstr_path_only[0], name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(
			&native_wstr_path_only[0], &native_wstr_path_only[0] + yggr::charset::charset_base_foo::strlen(native_wstr_path_only),
			name);
		std::wcout << "name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::wstring name;
		file_check_type::parsing_file_name(stl_wstr_path, name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(stl_wstr_path.c_str(), name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(stl_wstr_path.begin(), stl_wstr_path.end(), name);
		std::wcout << "name = " << name << std::endl;

		file_check_type::parsing_file_name(stl_wstr_path_only, name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(stl_wstr_path_only.c_str(), name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(stl_wstr_path_only.begin(), stl_wstr_path_only.end(), name);
		std::wcout << "name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::utf8_string name;
		file_check_type::parsing_file_name(boost_wstr_path, name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(boost_wstr_path.c_str(), name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(boost_wstr_path.begin(), boost_wstr_path.end(), name);
		std::wcout << "name = " << name << std::endl;

		file_check_type::parsing_file_name(boost_wstr_path_only, name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(boost_wstr_path_only.c_str(), name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(boost_wstr_path_only.begin(), boost_wstr_path_only.end(), name);
		std::wcout << "name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::utf8_string name;
		file_check_type::parsing_file_name(yggr_utf8_str_path, name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path.c_str(), name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), name);
		std::wcout << "name = " << name << std::endl;

		file_check_type::parsing_file_name(yggr_utf8_str_path_only, name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path_only.c_str(), name);
		std::wcout << "name = " << name << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), name);
		std::wcout << "name = " << name << std::endl;
	}

	std::cout << "---------------------------" << std::endl;

	{
		yggr::list<wchar_t> name;
		file_check_type::parsing_file_name(yggr_utf8_str_path, name);
		std::wcout << "lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path.c_str(), name);
		std::wcout << "lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path.begin(), yggr_utf8_str_path.end(), name);
		std::wcout << "lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;

		file_check_type::parsing_file_name(yggr_utf8_str_path_only, name);
		std::wcout << "lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path_only.c_str(), name);
		std::wcout << "lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;
		file_check_type::parsing_file_name(yggr_utf8_str_path_only.begin(), yggr_utf8_str_path_only.end(), name);
		std::wcout << "lst_name = " << std::wstring(name.begin(), name.end())  << std::endl;
	}

#endif // YGGR_NO_CWCHAR
	std::cout << "-----------test_parsing_file_name successed----------------" << std::endl;
}

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
		TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_check_file_name();
	test_check_path_name();

	test_parsing_file_path_and_name();
	test_parsing_file_path();
	test_parsing_file_name();

	wait_any_key(argc, argv);
	return 0;
}