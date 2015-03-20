// utf8_foo_test.cpp

#ifndef _MSC_VER
#	error "this file test msvc only !!!!"
#endif // _MSC_VER

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include <iostream>
#include <yggr/charset/utf8_string.hpp>
#include <cassert>

#ifdef _MSC_VER
#pragma comment(lib, "libiconv.lib")
#pragma comment(lib, "libyggr_time-vc100-d.lib")
#pragma comment(lib, "libyggr_base-vc100-d.lib")
#pragma comment(lib, "libyggr_charset-vc100-d.lib")
#endif // _MSC_VER

void test_default_charset_config(void)
{
	assert(yggr::string("gbk") == yggr::charset::default_charset_config::get_default_string_charset_name<char>());
	assert(yggr::string("ucs-2le") == yggr::charset::default_charset_config::get_default_string_charset_name<wchar_t>());
	assert(yggr::charset::default_charset_config::get_str_default_string_charset_name<char>() == yggr::string("gbk"));
	assert(yggr::charset::default_charset_config::get_str_default_string_charset_name<wchar_t>() == yggr::string("ucs-2le"));

	std::cout << "---------test_default_charset_config end------------" << std::endl;
}

void test_charset_base_foo(void)
{
	std::string std_str("abc你好");
	boost::container::string boost_str("abc你好");

#ifdef _MSC_VER
	std::wstring std_wstr(L"abc你好");
	boost::container::wstring boost_wstr(L"abc你好");
#else
    std::string std_wstr("abc你好");
	boost::container::string boost_wstr("abc你好");
#endif // _MSC_VER

	assert(yggr::charset::charset_base_foo::xchglen(std_str.c_str()) == std_str.size() * 4);
	assert(yggr::charset::charset_base_foo::xchglen(boost_str.c_str()) == boost_str.size() * 4);
	assert(yggr::charset::charset_base_foo::xchglen(std_str) == std_str.size() * 4);
	assert(yggr::charset::charset_base_foo::xchglen(boost_str) == boost_str.size() * 4);

	assert(yggr::charset::charset_base_foo::xchglen(std_wstr.c_str()) == std_wstr.size() * 4);
	assert(yggr::charset::charset_base_foo::xchglen(boost_wstr.c_str()) == boost_wstr.size() * 4);
	assert(yggr::charset::charset_base_foo::xchglen(std_wstr) == std_wstr.size() * 4);
	assert(yggr::charset::charset_base_foo::xchglen(boost_wstr) == boost_wstr.size() * 4);

	std::string std_lstr(100, 0);
	memcpy(&std_lstr[0], &std_str[0], sizeof(std::string::value_type) * std_str.size());
	assert(yggr::charset::charset_base_foo::nptr_rst_norm_lenth(std_lstr).size() == std_str.size());

	boost::container::string boost_lstr(100, 0);
	memcpy(&boost_lstr[0], &boost_str[0], sizeof(boost::container::string::value_type) * boost_str.size());
	assert(yggr::charset::charset_base_foo::nptr_rst_norm_lenth(boost_lstr).size() == boost_str.size());

#ifdef _MSC_VER
	std::wstring std_lwstr(100, 0);
	memcpy(&std_lwstr[0], &std_wstr[0], sizeof(std::wstring::value_type) * std_wstr.size());
	assert(yggr::charset::charset_base_foo::nptr_rst_norm_lenth(std_lwstr).size() == std_wstr.size());

	boost::container::wstring boost_lwstr(100, 0);
	memcpy(&boost_lwstr[0], &boost_wstr[0], sizeof(boost::container::wstring::value_type) * boost_wstr.size());
	assert(yggr::charset::charset_base_foo::nptr_rst_norm_lenth(boost_lwstr).size() == boost_wstr.size());

#endif // _MSC_VER

	std::cout << "---------test_charset_base_foo end------------" << std::endl;
}

void test_charset_converter(void)
{
	yggr::charset::charset_converter conv(yggr::charset::default_charset_config::get_default_string_charset_name<char>(),
											yggr::charset::default_charset_config::get_default_string_charset_name<wchar_t>());

	std::string std_str("abc你好");
	std::wstring std_wstr;
	conv(std_str, std_wstr);
#ifdef _MSC_VER
	std::wcout.imbue(std::locale("chs"));
	std::wcout << std_wstr << std::endl;
#endif // _MSC_VER

	boost::container::string boost_str("abc你好");
	boost::container::wstring boost_wstr;
	conv(boost_str, boost_wstr);
#ifdef _MSC_VER
	std::wcout.imbue(std::locale("chs"));
	std::wcout << boost_wstr << std::endl;
#endif // _MSC_VER

	boost::container::string boost_wstr_tmp2;
	boost::container::wstring boost_wstr2;
	conv(std_str, boost_wstr2);

#ifdef _MSC_VER
	std::wcout << boost_wstr2 << std::endl;
#endif // _MSC_VER

	yggr::charset::charset_converter conv3(yggr::charset::default_charset_config::get_default_string_charset_name<wchar_t>(),
											yggr::charset::default_charset_config::get_default_string_charset_name<char>());

#ifdef _MSC_VER
	std::wstring std_wstr3(L"abc_你好_wchar_t");
#else
    std::string std_wstr3("abc_你好_wchar_t");
#endif // _MSC_VER
	std::string std_str3;
	conv3(std_wstr3, std_str3);
	std::cout << std_str3 << std::endl;

	std::cout << "---------test_charset_converter end------------" << std::endl;
}

void test_charset_foo(void)
{
	// char to char same
	{
		std::string str("abc1你好");

		const std::string& cref_str = yggr::charset::charset_foo::xchg<std::string>(str); // return cref

		std::string& ref_str = const_cast<std::string&>(cref_str);

		ref_str[0] = 'd';
		std::cout << ref_str << std::endl;
		std::cout << str << std::endl;
		assert(&ref_str[0] == &str[0]);
	}

	// char to char diff
	{
		std::string str("abc2你好");

		//boost::container::string boost_str = yggr::charset::charset_foo::xchg<boost::container::string>(str); //return diff_type
		boost::container::string tmp(yggr::charset::charset_foo::xchg<boost::container::string>(str));
		boost::container::string boost_str(boost::move(tmp)); //return diff_type

		str[0] = 'd';
		std::cout << str << std::endl;
		std::cout << boost_str << std::endl;
		assert(&str[0] != &boost_str[0]);
	}

	//	wchar to wchar same
	{
#ifdef _MSC_VER
		std::wstring str(L"abcw1你好");
#else
        std::string str("abcw1你好");
#endif // _MSC_VER
		const std::wstring& cref_str = yggr::charset::charset_foo::xchg<std::wstring>(str); //return cref

		std::wstring& ref_str = const_cast<std::wstring&>(cref_str);

		ref_str[0] = L'd';

#ifdef _MSC_VER
		std::wcout.imbue(std::locale("chs"));
		std::wcout << ref_str << std::endl;
		std::wcout << str << std::endl;
		assert(reinterpret_cast<void*>(&ref_str[0]) == reinterpret_cast<void*>(&str[0]));
#endif // _MSC_VER

	}

	// wchar to wchar diff
	{
#ifdef _MSC_VER
		std::wstring str(L"abcw2你好");
#else
        std::string str("abcw2你好");
#endif // _MSC_VER

		//boost::container::wstring boost_str = yggr::charset::charset_foo::xchg<boost::container::wstring>(str);
		boost::container::wstring tmp(yggr::charset::charset_foo::xchg<boost::container::wstring>(str));
		boost::container::wstring boost_str(boost::move(tmp));

		str[0] = L'd';

#ifdef _MSC_VER
		std::wcout.imbue(std::locale("chs"));
		std::wcout << str << std::endl;
		std::wcout << boost_str << std::endl;
		assert(reinterpret_cast<void*>(&str[0]) != reinterpret_cast<void*>(&boost_str[0]));
#endif // _MSC_VER

	}

	// char to wchar diff
	{
		std::string str("abc3你好");

        boost::container::wstring tmp(yggr::charset::charset_foo::xchg<boost::container::wstring>(str));
		boost::container::wstring boost_str(boost::move(tmp)); //change string store type to wstring store type

		std::string rst_str((const char*)&boost_str[0], (const char*)(&boost_str[0] + boost_str.size()));

		assert(str.size() <= rst_str.size());
		assert(0 == memcmp(&boost_str[0], &str[0], str.size() * sizeof(char)));
		std::cout << str << std::endl;
		std::cout << rst_str << std::endl;
		assert(&str[0] != &rst_str[0]);
	}

	// wchar to char diff
	{
#ifdef _MSC_VER
		std::wstring str(L"abcw3你好");

        boost::container::string tmp(yggr::charset::charset_foo::xchg<boost::container::string>(str));
		boost::container::string boost_str(boost::move(tmp)); //change wstring store type to string store type

		std::wstring rst_str((const wchar_t*)&boost_str[0], (const wchar_t*)(&boost_str[0] + boost_str.size()));

		assert(str.size() == rst_str.size());
		assert(0 == memcmp(&boost_str[0], &str[0], str.size() * sizeof(wchar_t)));

		std::wcout.imbue(std::locale("chs"));
		std::wcout << str << std::endl;
		std::wcout << rst_str << std::endl;
		assert(reinterpret_cast<void*>(&str[0]) != reinterpret_cast<void*>(&rst_str[0]));
#endif // _MSC_VER

	}

	// string to wstring diff
	{
		std::string str("abc4 你好");

		boost::container::wstring tmp(yggr::charset::charset_foo::xchg<boost::container::wstring>(str, "gbk", "wchar_t"));
		boost::container::wstring boost_str(boost::move(tmp));

#ifdef _MSC_VER
		std::wcout.imbue(std::locale("chs"));
		std::wcout << boost_str << std::endl;
#endif // _MSC_VER
		std::cout << str << std::endl;
	}

	{
#ifdef _MSC_VER
		std::wstring str(L"abcw4 你好");
#else
        std::string str("abcw4 你好");
#endif // _MSC_VER

        boost::container::string tmp(yggr::charset::charset_foo::xchg<boost::container::string>(str, "wchar_t", "gbk"));
		boost::container::string boost_str(boost::move(tmp));

#ifdef _MSC_VER
		std::wcout.imbue(std::locale("chs"));
		std::wcout << str << std::endl;
#endif // _MSC_VER
		std::cout << boost_str << std::endl;
	}

	std::cout << "---------test_charset_foo end------------" << std::endl;
}

//template<typename Char>
//void test(const yggr::charset::basic_string<Char>& str,
//			const yggr::string& name = yggr::charset::charset_name_t<Char>())
//{
//}
//
////template<typename Char>
////void test(const yggr::string& name = yggr::charset::charset_name_t<Char>())
////{
////}

void test_str_wstr_converter_test(void)
{
	{
		std::string str("abc你cd好");
		std::wstring wstr(yggr::charset::str_wstr_converter::exchange(str));
		std::string str2(yggr::charset::str_wstr_converter::exchange(wstr));

		std::cout << str << std::endl;
#ifdef _MSC_VER
		std::wcout.imbue(std::locale("chs"));
		std::wcout << wstr << std::endl;
#endif // _MSC_VER
		std::cout << str2 << std::endl;
	}

	{
#ifdef _MSC_VER
		std::wstring wstr(L"abcw1你cd好");
		std::string str(yggr::charset::str_wstr_converter::exchange(wstr));
#else
        std::string wstr("abcw1你cd好");
        std::string str(wstr);
#endif // _MSC_VER
		std::wstring wstr2(yggr::charset::str_wstr_converter::exchange(str));

		std::cout << str << std::endl;
#ifdef _MSC_VER
		std::wcout.imbue(std::locale("chs"));
		std::wcout << wstr << std::endl;
		std::wcout << wstr2 << std::endl;
#endif // _MSC_VER
	}

	{
		yggr::string str("abc2你cd好");
		yggr::wstring wstr(yggr::charset::str_wstr_converter::exchange(str));
		yggr::string str2(yggr::charset::str_wstr_converter::exchange(wstr));

		std::cout << str << std::endl;
#ifdef _MSC_VER
		std::wcout.imbue(std::locale("chs"));
		std::wcout << wstr << std::endl;
#endif // _MSC_VER
		std::cout << str2 << std::endl;
	}

	{
#ifdef _MSC_VER
		yggr::wstring wstr(L"abcw2你cd好");
		yggr::string str(yggr::charset::str_wstr_converter::exchange(wstr));
#else
        yggr::string wstr("abcw2你cd好");
		yggr::string str(wstr);
#endif // _MSC_VER
		yggr::wstring wstr2(yggr::charset::str_wstr_converter::exchange(str));

		std::cout << str << std::endl;
#ifdef _MSC_VER
		std::wcout.imbue(std::locale("chs"));
		std::wcout << wstr << std::endl;
		std::wcout << wstr2 << std::endl;
#endif // _MSC_VER
	}

	std::cout << "---------test_str_wstr_converter_test end------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_default_charset_config();
	test_charset_base_foo();
	test_charset_converter();
	test_charset_foo();

	test_str_wstr_converter_test();

	yggr::utf8_char c1, c2;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
