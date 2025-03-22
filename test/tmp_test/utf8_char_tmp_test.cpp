//utf8_char_test.cpp

#ifndef _MSC_VER
#	error "this file test msvc only !!!!"
#endif // _MSC_VER

//#define YGGR_USE_STL_STRING
//#define _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

#include <iostream>
#include <cassert>

#include <yggr/charset/utf8_char.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

//void test_constructor(void)
//{
//	yggr::utf8_char c1('a');
//	yggr::utf8_char c1_1 = 'a';
//	std::cout << c1 << std::endl;
//	assert(c1 == c1_1);
//	assert(c1 == 'a');
//	assert(c1 == "a");
//
//	yggr::utf8_char c2("b");
//
//#if 0
//	yggr::utf8_char c2("abc"); // assert fail
//#endif // 0, 1
//
//	yggr::utf8_char c2_2("b", 1);
//	std::cout << c2 << std::endl;
//
//	assert(c2 == c2_2);
//
//	yggr::utf8_char c3("你");
//	yggr::utf8_char c3_3(std::string("你").c_str(), std::string("你").size());
//	std::cout << c3 << std::endl;
//	assert(c3 == c3_3);
//	assert(c3 == "你");
//
//	yggr::utf8_char c4(std::string("好"));
//
//#if 0
//	yggr::utf8_char c4(yggr::string("好a")); // assert fail
//#endif // 0, 1
//	
//	std::cout << c4<< std::endl;
//
//#ifndef YGGR_NO_CWCHAR
//	yggr::utf8_char c5(L"宽");
//#else
//    yggr::utf8_char c5("宽");
//#endif //YGGR_NO_CWCHAR
//	std::cout << c5 << std::endl;
//
//#ifndef YGGR_NO_CWCHAR
//	yggr::utf8_char c6(yggr::wstring(L"字", 1));
//#else
//    yggr::utf8_char c6(yggr::string("字").c_str(), yggr::string("字").size());
//#endif // YGGR_NO_CWCHAR
//
//#if 0
//	yggr::utf8_char c4(yggr::string("好a")); // assert fail
//#endif // 0, 1
//	std::cout << c6<< std::endl;
//
//	yggr::utf8_char c7(c5.begin(), c5.end(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
//
//#if 0
//	yggr::utf8_char c4(yggr::string("好a")); // assert fail
//#endif // 0, 1
//
//	std::cout << c7 << std::endl;
//
//#ifndef YGGR_NO_CWCHAR
//	std::wstring c6_2(L"字");
//#else
//    std::string c6_2("字");
//#endif //YGGR_NO_CWCHAR
//	yggr::utf8_char c8(c6_2.begin(), c6_2.end());
//	std::cout << c8 << std::endl;
//
//	yggr::utf8_char c9 = c8;
//	std::cout << c9 << std::endl;
//
//	yggr::utf8_char c10(boost::move(c8));
//	std::cout << c10 <<std::endl;
//	assert(!c8.empty());
//
//	std::cout << "----------test_constructor end-----------" << std::endl;
//}
//
//void test_operator_equal(void)
//{
//	yggr::utf8_char c1 = "你";
//	std::cout << c1 << std::endl;
//
//	assert(c1 == "你");
//
//	c1 = 'a';
//	std::cout << c1 << std::endl;
//
//	assert(c1 == "a");
//
//#if 0
//	c1 = "abc"; // assert fail
//#endif // 0, 1
//
//#ifndef YGGR_NO_CWCHAR
//	c1 = L"好";
//#else
//    c1 = "好";
//#endif //YGGR_NO_CWCHAR
//	std::cout << c1 << std::endl;
//
//	yggr::utf8_char c2 = 'b';
//
//	std::cout << c2 << std::endl;
//
//	c2 = c1;
//	std::cout << c2 << std::endl;
//
//#ifndef YGGR_NO_CWCHAR
//	std::wcout.imbue(std::locale("chs"));
//	std::wcout << c2 << std::endl;
//#endif // YGGR_NO_CWCHAR
//
//	//std::cin >> c2; //ok right
//	//std::cout << c2 << std::endl;
//
//	std::cout << "----------test_operator_equal end-----------" << std::endl;
//}
//
//void test_operator_compare(void)
//{
//	{
//		yggr::utf8_char uchar = "好";
//		std::string str = "好";
//		assert(uchar == str);
//		assert(uchar == str.c_str());
//		assert(!(uchar != str));
//		assert(!(uchar != str.c_str()));
//	}
//
//	{
//		yggr::utf8_char uchar = "好";
//#ifndef YGGR_NO_CWCHAR
//		std::wstring str = L"好";
//#else
//        std::string str = "好";
//#endif // YGGR_NO_CWCHAR
//		assert(uchar == str);
//		assert(uchar == str.c_str());
//		assert(!(uchar != str));
//		assert(!(uchar != str.c_str()));
//	}
//
//	{
//		yggr::utf8_char uchar = "好";
//		boost::container::string str = "不";
//		assert(uchar != str);
//		assert(uchar != str.c_str());
//		assert(!(uchar == str));
//		assert(!(uchar == str.c_str()));
//	}
//
//	{
//		yggr::utf8_char uchar = "好";
//#ifndef YGGR_NO_CWCHAR
//		boost::container::wstring str = L"不";
//#else
//        boost::container::string str = "不";
//#endif // YGGR_NO_CWCHAR
//		assert(uchar != str);
//		assert(uchar != str.c_str());
//		assert(!(uchar == str));
//		assert(!(uchar == str.c_str()));
//	}
//
//	{
//		yggr::utf8_char uchar = "好";
//#ifndef YGGR_NO_CWCHAR
//		yggr::utf8_char str = L"不";
//#else
//        yggr::utf8_char str = "不";
//#endif // YGGR_NO_CWCHAR
//		assert(uchar != str);
//		assert(!(uchar == str));
//#if 0
//		assert(!(uchar == str.c_str())); //conv fail don't like to use
//#else
//		//ok
//		assert(!(uchar == yggr::charset::make_string_charset_helper(
//							str.c_str(), yggr::charset::charset_name_t<yggr::utf8_char>())));
//#endif // 0, 1
//	}
//
//	std::cout << "----------test_operator_compare end-----------" << std::endl;
//}
//
//void swap_test(void)
//{
//	yggr::utf8_char c1 = "你";
//	yggr::utf8_char c2 = "好";
//
//	yggr::utf8_char c3 = c1;
//	yggr::utf8_char c4 = c2;
//
//	c3.swap(c4);
//	assert(c1 == c4);
//	assert(c2 == c3);
//
//	boost::swap(c1, c2);
//	assert(c1 == c3);
//	assert(c2 == c4);
//
//	std::swap(c1, c2);
//	assert(c1 == c4);
//	assert(c2 == c3);
//
//	yggr::utf8_char c;
//	assert(!c);
//	c = 'a';
//	assert(c);
//
//	std::cout << "----------swap_test end-----------" << std::endl;
//}
//
//#include <yggr/charset/utf8_string.hpp>
//
//void utf8_string_to_char_test(void)
//{
//	yggr::utf8_string str("你好bcdef吗");
//	yggr::utf8_char uc1(str.begin(), str.begin() + 1);
//	std::cout << uc1 << std::endl;
//
//	yggr::utf8_char uc2(str.rbegin(), str.rbegin() + 1);
//	std::cout << uc2 << std::endl;
//
//	std::cout << "----------utf8_string_to_char_test end-----------" << std::endl;
//}

int main(int argc, char* argv[])
{
	//test_constructor();
	//test_operator_equal();
	//test_operator_compare();
	//swap_test();
	//utf8_string_to_char_test();

	std::cout << "all test successed" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
