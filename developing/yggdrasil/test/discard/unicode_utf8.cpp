//unicode_utf8.cpp

#include <iostream>
#include <string>

#include <yggr/charset/charset_converter.hpp>
#include <yggr/charset/utf8_string.hpp>

#ifdef _DEBUG
#   include <vld.h>
#endif //_DEBUG

void test1(void)
{
	std::string input = "abc你好123";
	std::cout << input << std::endl;
	std::string output;

	yggr::charset::charset_converter conv("GBK", "UTF-8");
	if(conv(input, output, yggr::charset::charset_helper::xchglen(input)))
	{
		yggr::charset::charset_helper::nptr_rst_norm_lenth(output);
		//std::cout << output << std::endl;
		std::cout << yggr::charset::charset_helper::utf8_strlen(output) << std::endl;

#   ifdef WIN32
		yggr::charset::charset_converter convr("UTF-8", "GBK");
		std::string rst;

		if(convr(output, rst, yggr::charset::charset_helper::xchglen(output)))
		{
			std::cout << yggr::charset::charset_helper::nptr_rst_norm_lenth(rst) << std::endl;
		}
#   endif // WIN32
	}
	else
	{
		std::cout << "change err" << std::endl;
	}

}

void test2(void)
{
#ifdef _MSC_VER
	std::wstring input = L"abc飞机答复艰苦机!,！！，，。。》？";
	std::wcout.imbue(std::locale("chs"));
	std::wcout << input << std::endl;

	std::wstring output;

	yggr::charset::charset_converter conv("WCHAR_T", "UTF-8");
	if(conv(input, output, yggr::charset::charset_helper::xchglen(input)))
	{
		yggr::charset::charset_helper::nptr_rst_norm_lenth(output);
		//std::wcout << output << std::endl;
		std::cout << yggr::charset::charset_helper::utf8_strlen(output) << std::endl;

		yggr::charset::charset_converter convr("UTF-8", "WCHAR_T");
		std::wstring rst;

		if(convr(output, rst, yggr::charset::charset_helper::xchglen(output)))
		{
			std::wcout << yggr::charset::charset_helper::nptr_rst_norm_lenth(rst) << std::endl;
		}
	}
	else
	{
		std::cout << "change err" << std::endl;
	}
#else
    std::cout << "not complete support wstring" << std::endl;
#endif //_MSC_VER
}


void test3(void)
{
	yggr::charset::utf8_string str("abc你好，世界def测试。", "GBK");

#ifdef WIN32
	std::cout << str.str() << std::endl;
#else
    std::cout << str.str("UTF-8") << std::endl;
#endif //WIN32

#ifdef _MSC_VER
	std::wcout.imbue(std::locale("chs"));
	std::wcout << str.wstr() << std::endl;
#endif //_MSC_VER
}


void test4(void)
{
#ifdef _MSC_VER
	yggr::charset::utf8_string str(L"abc你好，世界def测试。", "WCHAR_T");

	std::cout << str.str() << std::endl;

	std::wcout.imbue(std::locale("chs"));
	std::wcout << str.wstr() << std::endl;
#endif //_MSC_VER
}

void test5(void)
{
	yggr::charset::utf8_string istr("abc你好，世界def测试。", "GBK");
	//yggr::charset::utf8_string istr("abcef,", "GBK");

#ifdef WIN32
	std::string str(istr.str("GBK"));
#else
    std::string str(istr.str("UTF-8"));
#endif // WIN32
	yggr::charset::charset_helper::nptr_rst_norm_lenth(str);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	std::wcout.imbue(std::locale("chs"));
	std::wstring wstr(istr.wstr("WCHAR_T"));
	yggr::charset::charset_helper::nptr_rst_norm_lenth(wstr);
	std::wcout << wstr << std::endl;
#endif //_MSC_VER
}


void test6(void)
{
#ifdef _MSC_VER
	yggr::charset::utf8_string istr(L"abc你好，世界def测试。", "GBK"); //empty

	std::string str(istr.str("GBK")); //error
	yggr::charset::charset_helper::nptr_rst_norm_lenth(str); 
	std::cout << str << std::endl;

	std::wcout.imbue(std::locale("chs"));
	std::wstring wstr(istr.wstr("WCHAR_T")); //error
	yggr::charset::charset_helper::nptr_rst_norm_lenth(wstr);
	std::wcout << wstr << std::endl;
#endif //_MSC_VER
}



int main(int argc, char* argv[])
{
	test1();
	test2();

	test3();
	test4();

	test5();
	test6();

	//std::wstring wstr(L"abc飞机答复艰苦机");
	/*std::string wstr("abc飞机答复艰苦机");
	wstr[0] = 0;
	std::cout << wstr.c_str() << std::endl;

	std::cout << wstr.length() << std::endl;
	std::cout << wstr.size() << std::endl;
	std::cout << wstr.capacity() << std::endl;*/

	char cc = 0;
	std::cin >> cc;
	return 0;
}
