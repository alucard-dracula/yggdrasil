//fuzzy_string_test.cpp

#ifndef __GNUC__
#	error "this file test only gcc !!!"
#endif // __GNUC__

#define BOOST_HAS_ICU

#define  YGGR_SYS_DEFAULT_STRING_CHARSET "utf-8"

#include <iostream>
#include <yggr/fuzzy/fuzzy_string.hpp>

void test1(void)
{
	try
	{
		std::cout << yggr::fuzzy::fuzzy_string::make_string("  ab  ,fc,./iyu  ") << std::endl;
	}
	catch(const yggr::fuzzy::fuzzy_string::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

#ifndef __MINGW32__
void test2(void)
{
	try
	{
		yggr::charset::utf8_string str(yggr::fuzzy::fuzzy_string::make_u32_string("  犯得上  达到但是  "));
		std::cout << str << std::endl;
	}
	catch(const yggr::fuzzy::fuzzy_string::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		yggr::charset::utf8_string str2(yggr::fuzzy::fuzzy_string::make_u32_string("  你好  ,fc,./iyu  "));
		std::cout << str2 << std::endl;
	}
	catch(const yggr::fuzzy::fuzzy_string::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}
#endif //__MINGW32__

int main(int argc, char* argv[])
{
	test1();

#ifndef __MINGW32__
	test2();
#endif // __MINGW32__

	char cc = 0;
	std::cin >> cc;
	return 0;
}
