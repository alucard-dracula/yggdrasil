//utf8_char_test.cpp

#ifndef _MSC_VER
#	error "this file test msvc only !!!!"
#endif // _MSC_VER

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include <iostream>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/mplex/iterator_to_value_t.hpp>

#ifdef _MSC_VER
#pragma comment(lib, "libiconv.lib")
#ifdef _DEBUG

#	pragma comment(lib, "libyggr_time-vc100-d.lib")
#	pragma comment(lib, "libyggr_base-vc100-d.lib")
#	pragma comment(lib, "libyggr_charset-vc100-d.lib")
#else
#	pragma comment(lib, "libyggr_time-vc100.lib")
#	pragma comment(lib, "libyggr_base-vc100.lib")
#	pragma comment(lib, "libyggr_charset-vc100.lib")
#endif // _DEBUG
#endif // _MSC_VER

void test_constructor(void)
{
	yggr::utf8_char c1('a');
	yggr::utf8_char c1_1 = 'a';
	std::cout << c1 << std::endl;

	yggr::utf8_char c2("b");
	//yggr::utf8_char c2("abc"); // assert fail
	std::cout << c2 << std::endl;

	yggr::utf8_char c3("你");
	std::cout << c3 << std::endl;

	yggr::utf8_char c4(std::string("好"));
	//yggr::utf8_char c4(yggr::string("好a")); // assert fail
	std::cout << c4<< std::endl;

#ifdef _MSC_VER
	yggr::utf8_char c5(L"宽");
#else
    yggr::utf8_char c5("宽");
#endif //__MINGE32__
	std::cout << c5 << std::endl;

#ifdef _MSC_VER
	yggr::utf8_char c6(yggr::wstring(L"字"));
#else
    yggr::utf8_char c6(yggr::string("字"));
#endif // __MINGE32__
	//yggr::utf8_char c4(yggr::string("好a")); // assert fail
	std::cout << c6<< std::endl;

	yggr::utf8_char c7(c5.begin(), c5.end(), UTF8_CHARSET_NAME);
	//yggr::utf8_char c4(yggr::string("好a")); // assert fail
	std::cout << c7 << std::endl;

#ifdef _MSC_VER
	std::wstring c6_2(L"字");
#else
    std::string c6_2("字");
#endif //__GNUC__
	yggr::utf8_char c8(c6_2.begin(), c6_2.end());
	std::cout << c8 << std::endl;

	yggr::utf8_char c9 = c8;
	std::cout << c9 << std::endl;

	yggr::utf8_char c10(boost::move(c8));
	std::cout << c10 <<std::endl;
	assert(c8.empty());

	std::cout << "----------test_constructor end-----------" << std::endl;
}

void test_operator_equal(void)
{
	yggr::utf8_char c1 = "你";
	std::cout << c1 << std::endl;

	c1 = 'a';
	std::cout << c1 << std::endl;

	//c1 = "abc"; // assert fail

#ifdef _MSC_VER
	c1 = L"好";
#else
    c1 = "好";
#endif //__GNUC__
	std::cout << c1 << std::endl;

	yggr::utf8_char c2 = 'b';

	std::cout << c2 << std::endl;

	c2 = c1;
	std::cout << c2 << std::endl;

#ifdef _MSC_VER
	std::wcout.imbue(std::locale("chs"));
	std::wcout << c2 << std::endl;
#endif // _MSC_VER

	//std::cin >> c2; //ok right
	//std::cout << c2 << std::endl;

	std::cout << "----------test_operator_equal end-----------" << std::endl;
}

void test_operator_compare(void)
{
	{
		yggr::utf8_char uchar = "好";
		std::string str = "好";
		assert(uchar == str);
		assert(uchar == str.c_str());
		assert(!(uchar != str));
		assert(!(uchar != str.c_str()));
	}

	{
		yggr::utf8_char uchar = "好";
#ifdef _MSC_VER
		std::wstring str = L"好";
#else
        std::string str = "好";
#endif // _MSC_VER
		assert(uchar == str);
		assert(uchar == str.c_str());
		assert(!(uchar != str));
		assert(!(uchar != str.c_str()));
	}

	{
		yggr::utf8_char uchar = "好";
		boost::container::string str = "不";
		assert(uchar != str);
		assert(uchar != str.c_str());
		assert(!(uchar == str));
		assert(!(uchar == str.c_str()));
	}

	{
		yggr::utf8_char uchar = "好";
#ifdef _MSC_VER
		boost::container::wstring str = L"不";
#else
        boost::container::string str = "不";
#endif // _MSC_VER
		assert(uchar != str);
		assert(uchar != str.c_str());
		assert(!(uchar == str));
		assert(!(uchar == str.c_str()));
	}

	{
		yggr::utf8_char uchar = "好";
#ifdef _MSC_VER
		yggr::utf8_char str = L"不";
#else
        yggr::utf8_char str = "不";
#endif // _MSC_VER
		assert(uchar != str);
		assert(!(uchar == str));
		//assert(!(uchar == str.c_str())); //conv fail don't like to use
	}

	std::cout << "----------test_operator_compare end-----------" << std::endl;
}

void swap_test(void)
{
	yggr::utf8_char c1 = "你";
	yggr::utf8_char c2 = "好";

	yggr::utf8_char c3 = c1;
	yggr::utf8_char c4 = c2;

	c3.swap(c4);
	assert(c1 == c4);
	assert(c2 == c3);

	boost::swap(c1, c2);
	assert(c1 == c3);
	assert(c2 == c4);

	std::swap(c1, c2);
	assert(c1 == c4);
	assert(c2 == c3);

	yggr::utf8_char c;
	assert(!c);
	c = 'a';
	assert(c);
}

void utf8_string_to_char_test(void)
{
	yggr::utf8_string str("你好bcdef吗");
	yggr::utf8_char uc1(str.begin(), str.begin() + 1);
	std::cout << uc1 << std::endl;

	yggr::utf8_char uc2(str.rbegin(), str.rbegin() + 1);
	std::cout << uc2 << std::endl;
}

int main(int argc, char* argv[])
{
	test_constructor();
	test_operator_equal();
	test_operator_compare();
	swap_test();
	utf8_string_to_char_test();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
