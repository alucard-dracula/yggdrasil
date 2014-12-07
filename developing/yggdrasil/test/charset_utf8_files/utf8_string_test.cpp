//utf8_string_test.cpp

#ifndef __GNUC__
#	error "this file test of gnu only !!!"
#endif // __GNUC__

//#define YGGR_USE_STL_STRING

#define YGGR_SYS_DEFAULT_STRING_CHARSET "utf-8"

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include <iostream>
#include <yggr/charset/utf8_string.hpp>

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

//#define TEST_IO

#include <yggr/ppex/pp_debug.hpp>
void test_iterator(void)
{
	yggr::string stlstr("abc");
	yggr::utf8_string str("a你b好c");

	std::cout << str.begin() - str.end() << std::endl;
	std::cout << str.end() - str.begin() << std::endl;

	yggr::utf8_string::iterator iter = str.begin();

	{
		int n = 0;
		for(yggr::utf8_string::iterator i = str.begin(), isize = str.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
			++n;
		}

		std::cout << "n = " << n << std::endl;

		n = 0;
		for(yggr::utf8_string::reverse_iterator i = str.rbegin(), isize = str.rend(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
			char c = *i;
			++n;
		}

		std::cout << "n = " << n << std::endl;
	}

	{
		int n = 0;
		for(yggr::utf8_string::const_iterator i = str.begin(), isize = str.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
			++n;
		}

		std::cout << "n = " << n << std::endl;

		n = 0;
		for(yggr::utf8_string::const_reverse_iterator i = str.rbegin(), isize = str.rend(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
			++n;
		}

		std::cout << "n = " << n << std::endl;
	}

	yggr::utf8_string str2(str.rbegin(), str.rend(), UTF8_CHARSET_NAME);
	std::cout << str2 << std::endl;
	std::cout << str.rbegin() - str.rend() << std::endl;
}

void test_element_access(void)
{
	yggr::utf8_string str("a你b好c");
	for(int i = 0, isize = str.size(); i != isize; ++i)
	{
		std::cout << str[i] << std::endl;
		std::cout << str.at(i) << std::endl;
	}

	std::cout << str.str<yggr::string>() << std::endl;
#ifdef _MSC_VER
	std::wcout.imbue(std::locale("chs"));
	std::wcout << str.str<yggr::wstring>() << std::endl;
#endif // _MSC_VER
}

void test_modify(void)
{
	// append_test
	yggr::utf8_string str("a你b好c");
	assert(str.size() == 5);

	str.append("d汉字f");
	assert(str.size() == 9);
	std::cout << str << std::endl;

	str.append(5, 'z');
	assert(str.size() == 14);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str.append(5, L'多');
#else
    str.append(5, "多");
#endif // _MSC_VER
	assert(str.size() == 19);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str.append(5, yggr::utf8_char(L'人'));
#else
    str.append(5, yggr::utf8_char("人"));
#endif // _MSC_VER
	assert(str.size() == 24);
	std::cout << str << std::endl;

	str.append("a游a戏a");
	assert(str.size() == 29);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str.append(L"w游w戏w");
#else
    str.append("w游w戏w");
#endif // _MSC_VER
	assert(str.size() == 34);
	std::cout << str << std::endl;

	str.append(yggr::string("boost游boost戏boost"));
	assert(str.size() == 34 + 15 + 2);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str.append(yggr::wstring(L"stdw游stdw戏stdw"));
#else
    str.append(yggr::string("stdw游stdw戏stdw"));
#endif // _MSC_VER
	assert(str.size() == 51 + 4 * 3 + 2);
	std::cout << str << std::endl;

	str = "手游";
	assert(str.size() == 2);
	std::cout << str << std::endl;

	str.append("abcdefg", 3, 3);
	assert(str.size() == 5);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str.append(L"abcdefg", 0, 3);
#else
    str.append("abcdefg", 0, 3);
#endif // _MSC_VER
	assert(str.size() == 8);
	std::cout << str << std::endl;

	str = "";
	assert(str.size() == 0);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str = L"页游";
#else
    str = "页游";
#endif // _MSC_VER
	assert(str.size() == 2);
	std::cout << str << std::endl;

	str.append(yggr::string("abcdefg"), 3, 3);
	assert(str.size() == 5);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str.append(yggr::wstring(L"abcdefg"), 0, 3);
#else
    str.append(yggr::string("abcdefg"), 0, 3);
#endif // _MSC_VER
	assert(str.size() == 8);
	std::cout << str << std::endl;

	char arr_str[] = "zxcvbnm";
	str.append(arr_str, arr_str + 3);
	assert(str.size() == 11);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	wchar_t arr_wstr[] = L"qwertyui";
#else
    char arr_wstr[] = "qwertyui";
#endif // _MSC_VER
	str.append(arr_wstr, arr_wstr + 5);
	assert(str.size() == 16);
	std::cout << str << std::endl;

	yggr::string stl_str("这是汉字");
	str.append(stl_str.begin(), stl_str.end());
	assert(str.size() == 20);
	std::cout << str << std::endl;

	yggr::utf8_string utf8_str("这是UTF8汉字");
	str.append(utf8_str.begin(), utf8_str.end());
	assert(str.size() == 28);
	std::cout << str << std::endl;

	str.append(utf8_str.rbegin(), utf8_str.rend());
	assert(str.size() == 36);
	std::cout << str << std::endl;

	std::cout << utf8_str.substr(1, 8) << std::endl;

	str = "端游";
	str.append(utf8_str, 1, 3);
	assert(str.size() == 5);
	std::cout << str << std::endl;

	//push_back_test

	str.push_back('d');
#ifdef _MSC_VER
	str.push_back(L'然');
	str.push_back(yggr::utf8_char(L'后'));
#else
    str.push_back("然");
    str.push_back(yggr::utf8_char("后"));
#endif // _MSC_VER


	assert(str.size() == 8);
	std::cout << str << std::endl;

	//assign_test

	str.assign("assign汉字");
	assert(str.size() == 8);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str.assign(L"assign_w汉字");
#else
    str.assign("assign_w汉字");
#endif // _MSC_VER
	assert(str.size() == 10);
	std::cout << str << std::endl;

	str.assign(yggr::string("stl_assign_汉字"));
	assert(str.size() == 13);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str.assign(yggr::wstring(L"stl_assign_w汉字"));
#else
    str.assign(yggr::string("stl_assign_w汉字"));
#endif // _MSC_VER
	assert(str.size() == 14);
	std::cout << str << std::endl;

	str.assign(boost::container::string("boost_assign_汉字"));
	assert(str.size() == 15);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str.assign(boost::container::wstring(L"boost_assign_w汉字"));
#else
    str.assign(boost::container::string("boost_assign_w汉字"));
#endif // _MSC_VER
	assert(str.size() == 16);
	std::cout << str << std::endl;

	yggr::string stl_str2("iter_stl_assign_汉字");
	str.assign(stl_str2.begin(), stl_str2.end());
	assert(str.size() == 18);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	yggr::wstring stl_wstr2(L"iter_stl_assign_w汉字");
#else
    yggr::string stl_wstr2("iter_stl_assign_w汉字");
#endif // _MSC_VER
	str.assign(stl_wstr2.begin(), stl_wstr2.end());
	assert(str.size() == 19);
	std::cout << str << std::endl;

	boost::container::string boost_str2("iter_boost_assign_汉字");
	str.assign(boost_str2.begin(), boost_str2.end());
	assert(str.size() == 20);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	boost::container::wstring boost_wstr2(L"iter_boost_assign_w汉字");
#else
    boost::container::string boost_wstr2("iter_boost_assign_w汉字");
#endif // _MSC_VER
	str.assign(boost_wstr2.begin(), boost_wstr2.end());
	assert(str.size() == 21);
	std::cout << str << std::endl;

	yggr::utf8_string u_str(str.begin(), str.end());
	assert(u_str.size() == 21);
	std::cout << u_str << std::endl;

	str = "";
	assert(str.empty());
	str.assign(boost::move(u_str));
	assert(str.size() == 21);
	//assert(u_str.empty());
	std::cout << str << std::endl;

	str.assign(u_str);
	//assert(str.empty());

	//insert test

	str = "abc";

	str.insert(1, 2, 'k');
	assert(str.size() == 5);
	std::cout << str << std::endl;

#ifdef _MSC_VER
	str.insert(2, 2, L'中');
#else
    str.insert(2, 2, "中");
#endif // _MSC_VER
	assert(str.size() == 7);
	std::cout << str << std::endl;

	{
	yggr::utf8_string::iterator iter = str.insert(str.begin() + 3, 2, 't');
	assert(iter - str.begin() == 3);
	std::cout << yggr::utf8_char(iter, iter + 1) << std::endl;
	}

	{
#ifdef _MSC_VER
	yggr::utf8_string::iterator iter = str.insert(str.begin() + 3, 2, L'大');
#else
    yggr::utf8_string::iterator iter = str.insert(str.begin() + 3, 2, "大");
#endif // _MSC_VER
	assert(iter - str.begin() == 3);
	std::cout << yggr::utf8_char(iter, iter + 1) << std::endl;
	}

	str.insert(str.begin(), 'z');
#ifdef _MSC_VER
	str.insert(str.begin(), L'卡');
#else
    str.insert(str.begin(), "卡");
#endif // _MSC_VER
	str.insert(str.begin(), yggr::utf8_char('x'));

	assert(str.size() == 14);
	std::cout << str << std::endl;

	{
		yggr::string str_ins("abcdefg");
		str.insert(str.begin(), str_ins.begin(), str_ins.end());
		assert(str.size() == 21);
		std::cout << str << std::endl;
	}

	{
#ifdef _MSC_VER
		yggr::wstring str_ins(L"abcdefg");
#else
        yggr::string str_ins("abcdefg");
#endif // _MSC_VER
		str.insert(str.begin(), str_ins.begin(), str_ins.end());
		assert(str.size() == 28);
		std::cout << str << std::endl;
	}

	{
#ifdef _MSC_VER
		yggr::wstring str_ins(L"啊不错的哦发给");
		str.insert(str.begin(), str_ins.rbegin(), str_ins.rend());
#else
        yggr::string str_ins("啊不错的哦发给");
        str.insert(str.begin(), str_ins.begin(), str_ins.end()); // can't use rbegin() rend()
#endif // _MSC_VER
		assert(str.size() == 35);
		std::cout << str << std::endl;
	}

	{
		str = "utf8字符串";
		yggr::utf8_string ustr("这是个");
		str.insert(str.begin(), ustr.begin(), ustr.end());
		assert(str.size() == 10);

		std::cout << str << std::endl;
	}

	{
		str = "utf8字符串";
		yggr::utf8_string ustr("这是个");
		str.insert(str.begin(), ustr.rbegin(), ustr.rend());
		assert(str.size() == 10);

		std::cout << str << std::endl;
	}

	{
		str = "utf8字符串";
		yggr::utf8_string ustr("这是个");
		str.insert(str.end(), ustr.rbegin(), ustr.rend());
		assert(str.size() == 10);

		std::cout << str << std::endl;
	}

	// erase test
	{
		str = "utf8字符串";
		str.erase();
		assert(str.empty());
	}

	{
		str = "utf8字符串";
		str.erase(0, 100);
		assert(str.empty());
	}

	{
		str = "utf8字符串";
		str.erase(1, 100);
		assert(str.size() == 1);
		std::cout << str << std::endl;
	}

	{
		str = "utf8字符串";
		str.erase(6, 1);
		assert(str.size() == 6);
		std::cout << str << std::endl;
	}

	{
		str = "utf8字符串";
		str.erase(str.begin());
		assert(str.size() == 6);
		str.erase(str.begin() + 1);
		assert(str.size() == 5);
		str.erase(str.end());
		assert(str.size() == 5);
		str.erase(str.end() - 1);
		assert(str.size() == 4);
		std::cout << str << std::endl;
	}

	{
		str = "utf8字符串";
		str.erase(str.begin(), str.begin() + 1);
		assert(str.size() == 6);
		str.erase(str.begin() + 1, str.begin() + 2);
		assert(str.size() == 5);
		str.erase(str.end() - 1, str.end());
		assert(str.size() == 4);
		str.erase(str.begin() + 1, str.end() - 1);
		assert(str.size() == 2);
		std::cout << str << std::endl;
	}

	// replace test
	{
		str = "string 字符串";
		assert(str.size() == 10);
		str.replace(1, 5, 7, 'a');
		assert(str.size() == 12);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
		assert(str.size() == 10);
#ifdef _MSC_VER
		str.replace(1, 5, 7, L'啊');
#else
        str.replace(1, 5, 7, "啊");
#endif // _MSC_VER
		assert(str.size() == 12);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
		assert(str.size() == 10);
#ifdef _MSC_VER
		str.replace(8, 2, 7, yggr::utf8_char(L'哦'));
#else
        str.replace(8, 2, 7, yggr::utf8_char("哦"));
#endif //__GNUC__
		assert(str.size() == 15);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
		assert(str.size() == 10);
#ifdef _MSC_VER
		str.replace(0, 6, L"字符串");
#else
        str.replace(0, 6, "字符串");
#endif // _MSC_VER
		int n = str.size();
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
		assert(str.size() == 10);
		str.replace(7, 3, "string");
		assert(str.size() == 13);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
		assert(str.size() == 10);
#ifdef _MSC_VER
		str.replace(0, 6, yggr::wstring(L"字符串"), 0, 2);
#else
        str.replace(0, 6, yggr::string("字符串"), 0, 2);
#endif // _MSC_VER
		assert(str.size() == 6);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
		yggr::string sstr("abc");
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
		yggr::string sstr("abc");
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
#ifdef _MSC_VER
		yggr::wstring sstr(L"你好吗");
#else
        yggr::string sstr("你好吗");
#endif // _MSC_VER
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
#ifdef _MSC_VER
		str = "string 字符串";
		yggr::wstring sstr(L"你好吗");
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		assert(str.size() == 7);
		std::cout << str << std::endl;
#endif // _MSC_VER
	}

	{
		str = "string 字符串";
#ifdef _MSC_VER
		yggr::utf8_string sstr(L"你好吗");
#else
        yggr::utf8_string sstr("你好吗");
#endif // _MSC_VER
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
#ifdef _MSC_VER
		yggr::utf8_string sstr(L"你好吗");
#else
        yggr::utf8_string sstr("你好吗");
#endif // _MSC_VER
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
#ifdef _MSC_VER
		yggr::utf8_string sstr(L"你好吗");
#else
        yggr::utf8_string sstr("你好吗");
#endif // _MSC_VER
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr);
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string 字符串";
#ifdef _MSC_VER
		yggr::utf8_string sstr(L"你好吗");
#else
        yggr::utf8_string sstr("你好吗");
#endif // _MSC_VER
		assert(str.size() == 10);
		str.replace(0, 6, sstr, 0, 1);
		assert(str.size() == 5);
		std::cout << str << std::endl;
	}
}

void test_string_operations(void)
{

#ifdef _MSC_VER
	yggr::utf8_string str(L"utf8字符串");
#else
    yggr::utf8_string str("utf8字符串");
#endif // _MSC_VER

	// find
	{
		assert(str.find('u') == 0);
		assert(str.find('u', 1) == yggr::utf8_string::npos);
#ifdef _MSC_VER
		assert(str.find(L'字') == 4);
		assert(str.find(L'字', 5) == yggr::utf8_string::npos);
		assert(str.find(yggr::utf8_char(L'字')) == 4);
		assert(str.find(yggr::utf8_char(L'字'), 5) == yggr::utf8_string::npos);
#else
        assert(str.find("字") == 4);
		assert(str.find("字", 5) == yggr::utf8_string::npos);
		assert(str.find(yggr::utf8_char("字")) == 4);
		assert(str.find(yggr::utf8_char("字"), 5) == yggr::utf8_string::npos);
#endif // _MSC_VER
	}

	{
		yggr::string sstr("utf8");
		assert(str.find(sstr) == 0);
		assert(str.find("utf8") == 0);
		assert(str.find(sstr, 1) == yggr::utf8_string::npos);
		assert(str.find("utf8", 1) == yggr::utf8_string::npos);
	}

	{
#ifdef _MSC_VER
		yggr::wstring sstr(L"字符串");
		assert(str.find(sstr) == 4);
		assert(str.find(L"字符串") == 4);
		assert(str.find(sstr, 5) == yggr::utf8_string::npos);
		assert(str.find(L"字符串", 5) == yggr::utf8_string::npos);
#else
        yggr::string sstr("字符串");
        assert(str.find(sstr) == 4);
		assert(str.find("字符串") == 4);
		assert(str.find(sstr, 5) == yggr::utf8_string::npos);
		assert(str.find("字符串", 5) == yggr::utf8_string::npos);
#endif // _MSC_VER
	}

	{
#ifdef _MSC_VER
		yggr::utf8_string sstr(L"tf8字符串");
#else
        yggr::utf8_string sstr("tf8字符串");
#endif // _MSC_VER
		assert(str.find(sstr) == 1);
		assert(str.find(sstr, 2) == yggr::utf8_string::npos);
	}

	// rfind
	{
		assert(str.rfind('u') == 0);
#ifdef _MSC_VER
		assert(str.rfind(L'字') == 4);
		assert(str.rfind(L'字', 3) == yggr::utf8_string::npos);
		assert(str.rfind(yggr::utf8_char(L'字')) == 4);
		assert(str.rfind(yggr::utf8_char(L'字'), 3) == yggr::utf8_string::npos);
#else
        assert(str.rfind("字") == 4);
		assert(str.rfind("字", 3) == yggr::utf8_string::npos);
		assert(str.rfind(yggr::utf8_char("字")) == 4);
		assert(str.rfind(yggr::utf8_char("字"), 3) == yggr::utf8_string::npos);
#endif // _MSC_VER
	}

	{
		yggr::string sstr("utf8");
		assert(str.rfind(sstr) == 0);
		assert(str.rfind("utf8") == 0);
	}

	{
#ifdef _MSC_VER
		yggr::wstring sstr(L"字符串");
		assert(str.rfind(sstr) == 4);
		assert(str.rfind(L"字符串") == 4);
		assert(str.rfind(sstr, 3) == yggr::utf8_string::npos);
		assert(str.rfind(L"字符串", 3) == yggr::utf8_string::npos);
#else
        yggr::string sstr("字符串");
		assert(str.rfind(sstr) == 4);
		assert(str.rfind("字符串") == 4);
		assert(str.rfind(sstr, 3) == yggr::utf8_string::npos);
		assert(str.rfind("字符串", 3) == yggr::utf8_string::npos);
#endif // _MSC_VER
	}

	{
#ifdef _MSC_VER
		yggr::utf8_string sstr(L"tf8字符串");
#else
        yggr::utf8_string sstr("tf8字符串");
#endif // _MSC_VER
		assert(str.rfind(sstr) == 1);
		assert(str.rfind(sstr, 0) == yggr::utf8_string::npos);
	}

	// find_first_of
	{
		//str == "utf8字符串"
#ifdef _MSC_VER
		assert(str.find_first_of(L'字') == 4);
		assert(str.find_first_of(L'字', 1) == 4);
		assert(str.find_first_of(L'字', 5) == yggr::utf8_string::npos);
		assert(str.find_first_of('a') == yggr::utf8_string::npos);
		assert(str.find_first_of("字abc") == 4);
		assert(str.find_first_of("abc") == yggr::utf8_string::npos);
		assert(str.find_first_of(L"字abc") == 4);
		assert(str.find_first_of(L"abc") == yggr::utf8_string::npos);
		assert(str.find_first_of(L"字abc", 1) == 4);
		assert(str.find_first_of(L"字abc",5) == yggr::utf8_string::npos);
#else
        assert(str.find_first_of("字") == 4);
		assert(str.find_first_of("字", 1) == 4);
		assert(str.find_first_of("字", 5) == yggr::utf8_string::npos);
		assert(str.find_first_of('a') == yggr::utf8_string::npos);
		assert(str.find_first_of("字abc") == 4);
		assert(str.find_first_of("abc") == yggr::utf8_string::npos);
		assert(str.find_first_of("字abc") == 4);
		assert(str.find_first_of("abc") == yggr::utf8_string::npos);
		assert(str.find_first_of("字abc", 1) == 4);
		assert(str.find_first_of("字abc",5) == yggr::utf8_string::npos);
#endif // _MSC_VER
	}

	{
#ifdef _MSC_VER
		assert(str.find_first_of(yggr::utf8_string(L"字abc")) == 4);
		assert(str.find_first_of(yggr::utf8_string(L"abc")) == yggr::utf8_string::npos);
		assert(str.find_first_of(yggr::utf8_string(L"字abc"), 1) == 4);
		assert(str.find_first_of(yggr::utf8_string(L"字abc"),5) == yggr::utf8_string::npos);
#else
        assert(str.find_first_of(yggr::utf8_string("字abc")) == 4);
		assert(str.find_first_of(yggr::utf8_string("abc")) == yggr::utf8_string::npos);
		assert(str.find_first_of(yggr::utf8_string("字abc"), 1) == 4);
		assert(str.find_first_of(yggr::utf8_string("字abc"),5) == yggr::utf8_string::npos);
#endif // _MSC_VER
	}

	// find first_not_of

	{
		//str == "utf8字符串"
#ifdef _MSC_VER
		assert(str.find_first_not_of(L'字') == 0);
		assert(str.find_first_not_of(L'字', 1) == 1);
		assert(str.find_first_not_of(L'字', 5) == 5);
		assert(str.find_first_not_of('a') == 0);
		assert(str.find_first_not_of("字abc") == 0);
		assert(str.find_first_not_of("abc") == 0);
		assert(str.find_first_not_of(L"字abc") == 0);
		assert(str.find_first_not_of(L"abc") == 0);
		assert(str.find_first_not_of(L"字abc", 1) == 1);
		assert(str.find_first_not_of(L"字abc",5) == 5);
		assert(str.find_first_not_of(L"utf8字符串", 1) == yggr::utf8_string::npos);
		assert(str.find_first_not_of(L"", str.size()) == yggr::utf8_string::npos);
		assert(str.find_first_not_of(L"", str.size() - 1) == str.size() - 1);
		assert(str.find_first_not_of(L"") == 0);
#else
        assert(str.find_first_not_of("字") == 0);
		assert(str.find_first_not_of("字", 1) == 1);
		assert(str.find_first_not_of("字", 5) == 5);
		assert(str.find_first_not_of('a') == 0);
		assert(str.find_first_not_of("字abc") == 0);
		assert(str.find_first_not_of("abc") == 0);
		assert(str.find_first_not_of("字abc") == 0);
		assert(str.find_first_not_of("abc") == 0);
		assert(str.find_first_not_of("字abc", 1) == 1);
		assert(str.find_first_not_of("字abc",5) == 5);
		assert(str.find_first_not_of("utf8字符串", 1) == yggr::utf8_string::npos);
		assert(str.find_first_not_of("", str.size()) == yggr::utf8_string::npos);
		assert(str.find_first_not_of("", str.size() - 1) == str.size() - 1);
		assert(str.find_first_not_of("") == 0);
#endif // _MSC_VER
	}

	{

#ifdef _MSC_VER
		assert(str.find_first_not_of(yggr::utf8_string(L"字abc")) == 0);
		assert(str.find_first_not_of(yggr::utf8_string(L"abc")) == 0);
		assert(str.find_first_not_of(yggr::utf8_string(L"字abc"), 1) == 1);
		assert(str.find_first_not_of(yggr::utf8_string(L"字abc"),5) == 5);
		assert(str.find_first_not_of(yggr::utf8_string(L"utf8字符串"), 1) == yggr::utf8_string::npos);

		assert(str.find_first_not_of(yggr::utf8_string(L""), str.size()) == yggr::utf8_string::npos);
		assert(str.find_first_not_of(yggr::utf8_string(L""), str.size() - 1) == str.size() - 1);
		assert(str.find_first_not_of(yggr::utf8_string(L"")) == 0);
#else
        assert(str.find_first_not_of(yggr::utf8_string("字abc")) == 0);
		assert(str.find_first_not_of(yggr::utf8_string("abc")) == 0);
		assert(str.find_first_not_of(yggr::utf8_string("字abc"), 1) == 1);
		assert(str.find_first_not_of(yggr::utf8_string("字abc"),5) == 5);
		assert(str.find_first_not_of(yggr::utf8_string("utf8字符串"), 1) == yggr::utf8_string::npos);

		assert(str.find_first_not_of(yggr::utf8_string(""), str.size()) == yggr::utf8_string::npos);
		assert(str.find_first_not_of(yggr::utf8_string(""), str.size() - 1) == str.size() - 1);
		assert(str.find_first_not_of(yggr::utf8_string("")) == 0);
#endif // _MSC_VER

	}

	// find_last_of
	{
		//str == "utf8字符串"
#ifdef _MSC_VER
		assert(str.find_last_of(L'字') == 4);
		assert(str.find_last_of(L'字', 1) == yggr::utf8_string::npos);
		assert(str.find_last_of(L'字', 5) == 4);
		assert(str.find_last_of('a') == yggr::utf8_string::npos);
		assert(str.find_last_of("字abc") == 4);
		assert(str.find_last_of("abc") == yggr::utf8_string::npos);
		assert(str.find_last_of(L"字abc") == 4);
		assert(str.find_last_of(L"abc") == yggr::utf8_string::npos);
		assert(str.find_last_of(L"字abc", 1) == yggr::utf8_string::npos);
		assert(str.find_last_of(L"字abc",5) == 4);
		assert(str.find_last_of('u', 0) == 0);
		assert(str.find_last_of("u", 0) == 0);
#else
        assert(str.find_last_of("字") == 4);
		assert(str.find_last_of("字", 1) == yggr::utf8_string::npos);
		assert(str.find_last_of("字", 5) == 4);
		assert(str.find_last_of('a') == yggr::utf8_string::npos);
		assert(str.find_last_of("字abc") == 4);
		assert(str.find_last_of("abc") == yggr::utf8_string::npos);
		assert(str.find_last_of("字abc") == 4);
		assert(str.find_last_of("abc") == yggr::utf8_string::npos);
		assert(str.find_last_of("字abc", 1) == yggr::utf8_string::npos);
		assert(str.find_last_of("字abc",5) == 4);
		assert(str.find_last_of('u', 0) == 0);
		assert(str.find_last_of("u", 0) == 0);
#endif // _MSC_VER
	}

	{
#ifdef _MSC_VER
		assert(str.find_last_of(yggr::utf8_string(L"字abc")) == 4);
		assert(str.find_last_of(yggr::utf8_string(L"abc")) == yggr::utf8_string::npos);
		assert(str.find_last_of(yggr::utf8_string(L"字abc"), 1) == yggr::utf8_string::npos);
		assert(str.find_last_of(yggr::utf8_string(L"字abc"),5) == 4);
#else
        assert(str.find_last_of(yggr::utf8_string("字abc")) == 4);
		assert(str.find_last_of(yggr::utf8_string("abc")) == yggr::utf8_string::npos);
		assert(str.find_last_of(yggr::utf8_string("字abc"), 1) == yggr::utf8_string::npos);
		assert(str.find_last_of(yggr::utf8_string("字abc"),5) == 4);
#endif // _MSC_VER
		assert(str.find_last_of(yggr::utf8_string("u"), 0) == 0);
	}

	// find_last_not_of
	{
		//str == "utf8字符串"
#ifdef _MSC_VER
		assert(str.find_last_not_of(L'字') == str.size() - 1);
		assert(str.find_last_not_of(L'字', 1) == 1);
		assert(str.find_last_not_of(L'字', 5) == 5);
		assert(str.find_last_not_of('a') == str.size() - 1);
		assert(str.find_last_not_of("字abc") == str.size() - 1);
		assert(str.find_last_not_of("abc") == str.size() - 1);
		assert(str.find_last_not_of(L"字abc") == str.size() - 1);
		assert(str.find_last_not_of(L"abc") == str.size() - 1);
		assert(str.find_last_not_of(L"字abc", 1) == 1);
		assert(str.find_last_not_of(L"字abc",5) == 5);
		assert(str.find_last_not_of(L"utf8字符串", 1) == yggr::utf8_string::npos);
		assert(str.find_last_not_of(L"", 0) == 0);
		assert(str.find_last_not_of(L"", str.size() - 1) == str.size() - 1);
		assert(str.find_last_not_of(L"") == str.size() - 1);
#else
        assert(str.find_last_not_of("字") == str.size() - 1);
		assert(str.find_last_not_of("字", 1) == 1);
		assert(str.find_last_not_of("字", 5) == 5);
		assert(str.find_last_not_of('a') == str.size() - 1);
		assert(str.find_last_not_of("字abc") == str.size() - 1);
		assert(str.find_last_not_of("abc") == str.size() - 1);
		assert(str.find_last_not_of("字abc") == str.size() - 1);
		assert(str.find_last_not_of("abc") == str.size() - 1);
		assert(str.find_last_not_of("字abc", 1) == 1);
		assert(str.find_last_not_of("字abc",5) == 5);
		assert(str.find_last_not_of("utf8字符串", 1) == yggr::utf8_string::npos);
		assert(str.find_last_not_of("", 0) == 0);
		assert(str.find_last_not_of("", str.size() - 1) == str.size() - 1);
		assert(str.find_last_not_of("") == str.size() - 1);
#endif // _MSC_VER
	}

	{
#ifdef _MSC_VER
		assert(str.find_last_not_of(yggr::utf8_string(L"字abc")) == str.size() - 1);
		assert(str.find_last_not_of(yggr::utf8_string(L"abc")) == str.size() - 1);
		assert(str.find_last_not_of(yggr::utf8_string(L"字abc"), 1) == 1);
		assert(str.find_last_not_of(yggr::utf8_string(L"字abc"),5) == 5);
		assert(str.find_last_not_of(yggr::utf8_string(L"utf8字符串"), 1) == yggr::utf8_string::npos);

		assert(str.find_last_not_of(yggr::utf8_string(L""), 0) == 0);
		assert(str.find_last_not_of(yggr::utf8_string(L""), str.size() - 1) == str.size() - 1);
#else
        assert(str.find_last_not_of(yggr::utf8_string("字abc")) == str.size() - 1);
		assert(str.find_last_not_of(yggr::utf8_string("abc")) == str.size() - 1);
		assert(str.find_last_not_of(yggr::utf8_string("字abc"), 1) == 1);
		assert(str.find_last_not_of(yggr::utf8_string("字abc"),5) == 5);
		assert(str.find_last_not_of(yggr::utf8_string("utf8字符串"), 1) == yggr::utf8_string::npos);

		assert(str.find_last_not_of(yggr::utf8_string(""), 0) == 0);
		assert(str.find_last_not_of(yggr::utf8_string(""), str.size() - 1) == str.size() - 1);
#endif // _MSC_VER
	}

	// compare

	{
		//str = "utf8字符串";
		assert(str.compare("utf8") > 0);
		assert(str.compare("utf8_string") > 0);
		assert(str.compare("utf8字符串长") < 0);
		assert(str.compare("") > 0);
		assert(str.compare("utf8字符串") == 0);

#ifdef _MSC_VER
		assert(str.compare(L"utf8") > 0);
		assert(str.compare(L"utf8_string") > 0);
		assert(str.compare(L"utf8字符串长") < 0);
		assert(str.compare(L"") > 0);
		assert(str.compare(L"utf8字符串") == 0);
#else
        assert(str.compare("utf8") > 0 );
		assert(str.compare("utf8_string") > 0 );
		assert(str.compare("utf8字符串长") < 0);
		assert(str.compare("") > 0);
		assert(str.compare("utf8字符串") == 0);
#endif // _MSC_VER

		assert(str.compare("utf8", 0, 1) > 0);
		assert(str.compare("utf8_string", 2, 7) > 0);
		assert(str.compare("utf8", 0, 0) > 0);

#ifdef _MSC_VER
		assert(str.compare(L"utf8", 0, 1) > 0);
		assert(str.compare(L"utf8_string", 2, 7) > 0);
		assert(str.compare(L"utf8字符串长长长", 0, 8) < 0);
		assert(str.compare(L"utf8", 0, 0) > 0);
		assert(str.compare(L"长utf8字符串长长", 1, 7) == 0);
#else
        assert(str.compare("utf8", 0, 1) > 0 );
		assert(str.compare("utf8_string", 2, 7) > 0);
		assert(str.compare("utf8", 0, 0) > 0);
#endif // _MSC_VER

#ifdef _MSC_VER
		assert(str.compare(4, 2, "字符") == 0);
		assert(str.compare(4, 2, L"字符") == 0);
		assert(str.compare(4, 2, yggr::string("字符")) == 0);
		assert(str.compare(4, 2, yggr::wstring(L"字符")) == 0);

		assert(str.compare(4, 2, "字符长") < 0);
		assert(str.compare(4, 2, L"字符长") < 0);
		assert(str.compare(4, 2, yggr::string("字符长"))< 0);
		assert(str.compare(4, 2, yggr::wstring(L"字符长")) < 0);

		assert(str.compare(4, 3, "字符") > 0);
		assert(str.compare(4, 3, L"字符") > 0);
		assert(str.compare(4, 3, yggr::string("字符")) > 0);
		assert(str.compare(4, 3, yggr::wstring(L"字符")) > 0);

		assert(str.compare(4, 2, yggr::utf8_string("字符")) == 0);
		assert(str.compare(4, 2, yggr::utf8_string(L"字符")) == 0);

		assert(str.compare(4, 2, yggr::utf8_string("字符长")) < 0);
		assert(str.compare(4, 2, yggr::wstring(L"字符长")) < 0);

		assert(str.compare(4, 3, yggr::utf8_string("字符")) > 0);
		assert(str.compare(4, 3, yggr::utf8_string(L"字符")) > 0);
#else
        assert(str.compare(4, 2, "字符") == 0);
		assert(str.compare(4, 2, yggr::string("字符")) == 0);

		assert(str.compare(4, 2, "字符长") < 0);
		assert(str.compare(4, 2, yggr::string("字符长")) < 0);

		assert(str.compare(4, 3, "字符") > 0);
		assert(str.compare(4, 3, yggr::string("字符")) > 0);

		assert(str.compare(4, 2, yggr::utf8_string("字符")) == 0);

		assert(str.compare(4, 2, yggr::utf8_string("字符长")) < 0);

		assert(str.compare(4, 3, yggr::utf8_string("字符")) > 0);
#endif //
	}
}

void non_member_foo_test(void)
{
	// operator +
	{
#ifdef _MSC_VER
		yggr::utf8_string str("utf8字符串");
		std::cout << "utf8字符" + str + "aaa" << std::endl;
		std::cout << L"utf8字符" + str + L"bbb" << std::endl;

		std::cout << yggr::string("utf8字符") + str << std::endl;
		std::wcout.imbue(std::locale("chs"));
		std::wcout << yggr::wstring(L"utf8字符") + str << std::endl;

		std::cout << "utf8字符" + str + yggr::string("aaa") << std::endl;
		std::cout << L"utf8字符" + str + yggr::wstring(L"bbb") << std::endl;

		std::cout << yggr::utf8_string("utf8字符") + str << std::endl;
		const yggr::utf8_string& cref_str = str;
		std::cout << yggr::utf8_string("utf8字符") + cref_str << std::endl;


		std::cout << yggr::utf8_string("utf8字符") + str + yggr::string("aaa") << std::endl;
		std::cout << yggr::utf8_string(L"utf8字符") + str + yggr::wstring(L"bbb") << std::endl;
#else
        yggr::utf8_string str("utf8字符串");
		std::cout << "utf8字符" + str + "aaa" << std::endl;

		std::cout << yggr::string("utf8字符") + str << std::endl;

		std::cout << "utf8字符" + str + yggr::string("aaa") << std::endl;

		std::cout << yggr::utf8_string("utf8字符") + str + yggr::string("aaa") << std::endl;
#endif // _MSC_VER
	}

	// operator ==
	{
#ifdef _MSC_VER
		yggr::utf8_string str("utf8字符串");
		assert(str == "utf8字符串");
		assert("utf8字符串" == str);
		assert(str == L"utf8字符串");
		assert(L"utf8字符串" == str);

		assert(str == yggr::string("utf8字符串"));
		assert(yggr::string("utf8字符串") == str);
		assert(str == yggr::wstring(L"utf8字符串"));
		assert(yggr::wstring(L"utf8字符串") == str);

		assert(str == yggr::utf8_string("utf8字符串"));
		assert(yggr::utf8_string("utf8字符串") == str);
		assert(str == yggr::utf8_string(L"utf8字符串"));
		assert(yggr::utf8_string(L"utf8字符串") == str);
#else
        yggr::utf8_string str("utf8字符串");
		assert(str == "utf8字符串");
		assert("utf8字符串" == str);

		assert(str == yggr::string("utf8字符串"));
		assert(yggr::string("utf8字符串") == str);

		assert(str == yggr::utf8_string("utf8字符串"));
		assert(yggr::utf8_string("utf8字符串") == str);
#endif // _MSC_VER
	}

	// operator !=
	{
		yggr::utf8_string str("utf8字符串");
		assert(str != "utf8");
		assert("utf8" != str);
		assert(str != L"utf8");
		assert(L"utf8" != str);
		assert(str != "");
		assert("" != str);
		assert(str != L"");
		assert(L"" != str);

		assert(str != yggr::string("utf8"));
		assert(yggr::string("utf8") != str);
		assert(str != yggr::wstring(L"utf8"));
		assert(yggr::wstring(L"utf8") != str);
		assert(str != yggr::string(""));
		assert(yggr::string("") != str);
		assert(str != yggr::wstring(L""));
		assert(yggr::wstring(L"") != str);

		assert(str != yggr::utf8_string("utf8"));
		assert(yggr::utf8_string("utf8") != str);
		assert(str != yggr::utf8_string(L"utf8"));
		assert(yggr::utf8_string(L"utf8") != str);
		assert(str != yggr::utf8_string(""));
		assert(yggr::utf8_string("") != str);
		assert(str != yggr::utf8_string(L""));
		assert(yggr::utf8_string(L"") != str);
	}

	// operator <
	{
#ifdef _MSC_VER
		yggr::utf8_string str("utf8字符串");
		assert(str < "utf8字符串长");
		assert(str < L"utf8字符串长");
		assert(str < yggr::string("utf8字符串长"));
		assert(str < yggr::wstring(L"utf8字符串长"));
		assert("" < str );

		assert(!("utf8字符串长" < str));
		assert(!(L"utf8字符串长" < str));
		assert(!(yggr::string("utf8字符串长") < str));
		assert(!(yggr::wstring(L"utf8字符串长") < str));
		assert(!(str < yggr::wstring(L"")));

		assert(str < yggr::utf8_string("utf8字符串长"));
		assert(!(yggr::utf8_string(L"utf8字符串长") < str));
#else
        yggr::utf8_string str("utf8字符串");
		assert(str < "utf8字符串长");
		assert(str < yggr::string("utf8字符串长"));
		assert("" < str );

		assert(!("utf8字符串长" < str));
		assert(!(yggr::string("utf8字符串长") < str));
		assert(!(str < yggr::wstring(L"")));

		assert(str < yggr::utf8_string("utf8字符串长"));
#endif // _MSC_VER
	}

	// operator <=
	{
#ifdef _MSC_VER
		yggr::utf8_string str("utf8字符串");
		assert(str <= "utf8字符串长");
		assert(str <= "utf8字符串");
		assert(str <= L"utf8字符串长");
		assert(str <= L"utf8字符串");
		assert(str <= yggr::string("utf8字符串长"));
		assert(str <= yggr::string("utf8字符串"));
		assert(str <= yggr::wstring(L"utf8字符串长"));
		assert(str <= yggr::wstring(L"utf8字符串"));

		assert(!("utf8字符串长" <= str));
		assert(!(L"utf8字符串长" <= str));
		assert(!(yggr::string("utf8字符串长") <= str));
		assert(!(yggr::wstring(L"utf8字符串长") <= str));

		assert(str <= yggr::utf8_string("utf8字符串长"));
		assert(!(yggr::utf8_string(L"utf8字符串长") <= str));
#else
        yggr::utf8_string str("utf8字符串");
		assert(str <= "utf8字符串长");
		assert(str <= "utf8字符串");
		assert(str <= yggr::string("utf8字符串长"));
		assert(str <= yggr::string("utf8字符串"));

		assert(!("utf8字符串长" <= str));
		assert(!(yggr::string("utf8字符串长") <= str));

		assert(str <= yggr::utf8_string("utf8字符串长"));
#endif // _MSC_VER
	}

	// operator >
	{
#ifdef _MSC_VER
		yggr::utf8_string str("utf8字符串");
		assert(str > "utf8字符");
		assert(str > L"utf8字符");
		assert(str > yggr::string("utf8字符"));
		assert(str > yggr::wstring(L"utf8字符"));
		assert(str > "" );

		assert(!("utf8字符" > str));
		assert(!(L"utf8字符" > str));
		assert(!(yggr::string("utf8字符") > str));
		assert(!(yggr::wstring(L"utf8字符") > str));

		assert(str > yggr::utf8_string("utf8字符"));
		assert(!(yggr::utf8_string(L"utf8字符") > str));
#else
        yggr::utf8_string str("utf8字符串");
		assert(str > "utf8字符");
		assert(str > yggr::string("utf8字符"));
		assert(str > "" );

		assert(!("utf8字符" > str));
		assert(!(yggr::string("utf8字符") > str));

		assert(str > yggr::utf8_string("utf8字符"));
#endif // _MSC_VER
	}

	// operator >=
	{
#ifdef _MSC_VER
		yggr::utf8_string str("utf8字符串");
		assert(str >= "utf8字符");
		assert(str >= "utf8字符串");
		assert(str >= L"utf8字符");
		assert(str >= L"utf8字符串");
		assert(str >= yggr::string("utf8字符"));
		assert(str >= yggr::string("utf8字符串"));
		assert(str >= yggr::wstring(L"utf8字符"));
		assert(str >= yggr::wstring(L"utf8字符串"));

		assert(!("utf8字符" >= str));
		assert(!(L"utf8字符" >= str));
		assert(!(yggr::string("utf8字符") >= str));
		assert(!(yggr::wstring(L"utf8字符") >= str));

		assert(str >= yggr::utf8_string("utf8字符"));
		assert(!(yggr::utf8_string(L"utf8字符") >= str));
#else
        yggr::utf8_string str("utf8字符串");
		assert(str >= "utf8字符");
		assert(str >= "utf8字符串");
		assert(str >= yggr::string("utf8字符"));
		assert(str >= yggr::string("utf8字符串"));

		assert(!("utf8字符" >= str));
		assert(!(yggr::string("utf8字符") >= str));

		assert(str >= yggr::utf8_string("utf8字符"));
#endif // _MSC_VER
	}

	// swap
	{
		yggr::utf8_string str1_chk("utf8字符串1");
		yggr::utf8_string str2_chk("utf8字符串2");

		yggr::utf8_string str1(str1_chk);
		yggr::utf8_string str2(str2_chk);

		assert(str1 == str1_chk);
		assert(str2 == str2_chk);

		std::swap(str1, str2);
		assert(str1 == str2_chk);
		assert(str2 == str1_chk);

		boost::swap(str1, str2);
		assert(str1 == str1_chk);
		assert(str2 == str2_chk);

		std::swap(boost::move(str1), str2);
		assert(str1 == str2_chk);
		assert(str2 == str1_chk);

		boost::swap(boost::move(str1), str2);
		assert(str1 == str1_chk);
		assert(str2 == str2_chk);

		std::swap(str1, boost::move(str2));
		assert(str1 == str2_chk);
		assert(str2 == str1_chk);

		boost::swap(str1, boost::move(str2));
		assert(str1 == str1_chk);
		assert(str2 == str2_chk);
	}
}

void test_io(void)
{
	yggr::utf8_string in;
	std::cin >> in;
	std::cout << in << std::endl;

#ifdef _MSC_VER
	std::wcin.imbue(std::locale("chs"));
	std::wcin >> in;
	std::cout << in << std::endl;
#endif // _MSC_VER
}

int main(int argc, char* argv[])
{
	test_iterator();
	test_element_access();
	test_modify();
	test_string_operations();
	non_member_foo_test();

#	ifdef TEST_IO
	test_io();
#	endif // TEST_IO

	char cc = 0;
	std::cin >> cc;
	return 0;
}
