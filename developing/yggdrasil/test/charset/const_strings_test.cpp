//const_strings_test.cpp

#ifndef _MSC_VER
#	error "this file test msvc only !!!!"
#endif // _MSC_VER

#include <iostream>
#include <yggr/charset/string.hpp>
#include <yggr/ppex/pp_debug.hpp>
#include <yggr/charset/const_strings.hpp>

//#define YGGR_USE_STL_STRING	
#define YGGR_USE_BOOST_STRING

#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER

YGGR_PP_CONST_STRING_GLOBAL_DEF(g_test_string, "ab,\\c2")

template<typename Char>
const Char* foo(const Char* ptr, const Char* ptr2 = YGGR_PP_CONST_STRING_GLOBAL_GET(g_test_string, Char))
{
	return ptr2;
}

int main(int arc, char* argv[])
{
#ifdef _DEBUG
	std::cout << YGGR_PP_DEBUG(YGGR_PP_CONST_STRING_LOCAL_DEF(test_string, "ab,\\c")) << std::endl;
	std::cout << YGGR_PP_DEBUG( YGGR_PP_CONST_CHAR_LOCAL_DEF(test_char, '/') ) << std::endl;
#endif // _DEBUG
	YGGR_PP_CONST_STRING_LOCAL_DEF(test_string, "ab,\\c");

	yggr::string str = YGGR_PP_CONST_STRING_LOCAL_GET(test_string, std::string::value_type);
	yggr::string str2 = YGGR_PP_CONST_STRING_GLOBAL_GET(g_test_string, std::string::value_type);
	std::cout << str << std::endl;
	std::cout << str2 << std::endl;

#ifndef __MINGW32__
	yggr::wstring wstr = YGGR_PP_CONST_STRING_LOCAL_GET(test_string, std::wstring::value_type);
	yggr::wstring wstr2 = YGGR_PP_CONST_STRING_GLOBAL_GET(g_test_string, std::wstring::value_type);
	std::wcout.imbue(std::locale("chs"));
	std::wcout << wstr << std::endl;
	std::wcout << wstr2 << std::endl;
#endif // 

	YGGR_PP_CONST_CHAR_LOCAL_DEF(test_char, '/')
	
	std::cout << YGGR_PP_CONST_CHAR_LOCAL_GET(test_char, char) << std::endl;

#ifndef __MINGW32__
	wchar_t wchar = YGGR_PP_CONST_CHAR_LOCAL_GET(test_char, wchar_t);
#endif // __MINGW32__
	char cc = 0;
	std::cin >> cc;
}