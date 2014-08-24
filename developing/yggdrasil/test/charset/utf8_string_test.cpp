//utf8_string_test.cpp

#define YGGR_USE_STL_STRING

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
	yggr::utf8_string str("aÄãbºÃc");

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
	yggr::utf8_string str("aÄãbºÃc");
	for(int i = 0, isize = str.size(); i != isize; ++i)
	{
		std::cout << str[i] << std::endl;
		std::cout << str.at(i) << std::endl;
	}

	std::cout << str.str<yggr::string>() << std::endl;
#ifndef __MINGW32__
	std::wcout.imbue(std::locale("chs"));
	std::wcout << str.str<yggr::wstring>() << std::endl;
#endif // __MINGW32__
}

void test_modify(void)
{
	// append_test
	yggr::utf8_string str("aÄãbºÃc");
	assert(str.size() == 5);

	str.append("dºº×Öf");
	assert(str.size() == 9);
	std::cout << str << std::endl;

	str.append(5, 'z');
	assert(str.size() == 14);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	str.append(5, L'¶à');
#else
    str.append(5, "¶à");
#endif // __MINGW32__
	assert(str.size() == 19);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	str.append(5, yggr::utf8_char(L'ÈË'));
#else
    str.append(5, yggr::utf8_char("ÈË"));
#endif // __MINGW32__
	assert(str.size() == 24);
	std::cout << str << std::endl;

	str.append("aÓÎaÏ·a");
	assert(str.size() == 29);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	str.append(L"wÓÎwÏ·w");
#else
    str.append("wÓÎwÏ·w");
#endif // __MINGW32__
	assert(str.size() == 34);
	std::cout << str << std::endl;

	str.append(yggr::string("boostÓÎboostÏ·boost"));
	assert(str.size() == 34 + 15 + 2);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	str.append(yggr::wstring(L"stdwÓÎstdwÏ·stdw"));
#else
    str.append(yggr::string("stdwÓÎstdwÏ·stdw"));
#endif // __MINGW32__
	assert(str.size() == 51 + 4 * 3 + 2);
	std::cout << str << std::endl;

	str = "ÊÖÓÎ";
	assert(str.size() == 2);
	std::cout << str << std::endl;

	str.append("abcdefg", 3, 3);
	assert(str.size() == 5);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	str.append(L"abcdefg", 0, 3);
#else
    str.append("abcdefg", 0, 3);
#endif // __MINGW32__
	assert(str.size() == 8);
	std::cout << str << std::endl;

	str = "";
	assert(str.size() == 0);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	str = L"Ò³ÓÎ";
#else
    str = "Ò³ÓÎ";
#endif // __MINGW32__
	assert(str.size() == 2);
	std::cout << str << std::endl;

	str.append(yggr::string("abcdefg"), 3, 3);
	assert(str.size() == 5);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	str.append(yggr::wstring(L"abcdefg"), 0, 3);
#else
    str.append(yggr::string("abcdefg"), 0, 3);
#endif // __MINGW32__
	assert(str.size() == 8);
	std::cout << str << std::endl;

	char arr_str[] = "zxcvbnm";
	str.append(arr_str, arr_str + 3);
	assert(str.size() == 11);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	wchar_t arr_wstr[] = L"qwertyui";
#else
    char arr_wstr[] = "qwertyui";
#endif // __MINGW32__
	str.append(arr_wstr, arr_wstr + 5);
	assert(str.size() == 16);
	std::cout << str << std::endl;

	yggr::string stl_str("ÕâÊÇºº×Ö");
	str.append(stl_str.begin(), stl_str.end());
	assert(str.size() == 20);
	std::cout << str << std::endl;

	yggr::utf8_string utf8_str("ÕâÊÇUTF8ºº×Ö");
	str.append(utf8_str.begin(), utf8_str.end());
	assert(str.size() == 28);
	std::cout << str << std::endl;

	str.append(utf8_str.rbegin(), utf8_str.rend());
	assert(str.size() == 36);
	std::cout << str << std::endl;

	std::cout << utf8_str.substr(1, 8) << std::endl;

	str = "¶ËÓÎ";
	str.append(utf8_str, 1, 3);
	assert(str.size() == 5);
	std::cout << str << std::endl;

	//push_back_test

	str.push_back('d');
#ifndef __MINGW32__
	str.push_back(L'È»');
	str.push_back(yggr::utf8_char(L'ºó'));
#else
    str.push_back("È»");
    str.push_back(yggr::utf8_char("ºó"));
#endif // __MINGW32__


	assert(str.size() == 8);
	std::cout << str << std::endl;

	//assign_test

	str.assign("assignºº×Ö");
	assert(str.size() == 8);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	str.assign(L"assign_wºº×Ö");
#else
    str.assign("assign_wºº×Ö");
#endif // __MINGW32__
	assert(str.size() == 10);
	std::cout << str << std::endl;

	str.assign(yggr::string("stl_assign_ºº×Ö"));
	assert(str.size() == 13);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	str.assign(yggr::wstring(L"stl_assign_wºº×Ö"));
#else
    str.assign(yggr::string("stl_assign_wºº×Ö"));
#endif // __MINGW32__
	assert(str.size() == 14);
	std::cout << str << std::endl;

	str.assign(boost::container::string("boost_assign_ºº×Ö"));
	assert(str.size() == 15);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	str.assign(boost::container::wstring(L"boost_assign_wºº×Ö"));
#else
    str.assign(boost::container::string("boost_assign_wºº×Ö"));
#endif // __MINGW32__
	assert(str.size() == 16);
	std::cout << str << std::endl;

	yggr::string stl_str2("iter_stl_assign_ºº×Ö");
	str.assign(stl_str2.begin(), stl_str2.end());
	assert(str.size() == 18);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	yggr::wstring stl_wstr2(L"iter_stl_assign_wºº×Ö");
#else
    yggr::string stl_wstr2("iter_stl_assign_wºº×Ö");
#endif // __MINGW32__
	str.assign(stl_wstr2.begin(), stl_wstr2.end());
	assert(str.size() == 19);
	std::cout << str << std::endl;

	boost::container::string boost_str2("iter_boost_assign_ºº×Ö");
	str.assign(boost_str2.begin(), boost_str2.end());
	assert(str.size() == 20);
	std::cout << str << std::endl;

#ifndef __MINGW32__
	boost::container::wstring boost_wstr2(L"iter_boost_assign_wºº×Ö");
#else
    boost::container::string boost_wstr2("iter_boost_assign_wºº×Ö");
#endif // __MINGW32__
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

#ifndef __MINGW32__
	str.insert(2, 2, L'ÖÐ');
#else
    str.insert(2, 2, "ÖÐ");
#endif // __MINGW32__
	assert(str.size() == 7);
	std::cout << str << std::endl;

	{
	yggr::utf8_string::iterator iter = str.insert(str.begin() + 3, 2, 't');
	assert(iter - str.begin() == 3);
	std::cout << yggr::utf8_char(iter, iter + 1) << std::endl;
	}

	{
#ifndef __MINGW32__
	yggr::utf8_string::iterator iter = str.insert(str.begin() + 3, 2, L'´ó');
#else
    yggr::utf8_string::iterator iter = str.insert(str.begin() + 3, 2, "´ó");
#endif // __MINGW32__
	assert(iter - str.begin() == 3);
	std::cout << yggr::utf8_char(iter, iter + 1) << std::endl;
	}

	str.insert(str.begin(), 'z');
#ifndef __MINGW32__
	str.insert(str.begin(), L'¿¨');
#else
    str.insert(str.begin(), "¿¨");
#endif // __MINGW32__
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
#ifndef __MINGW32__
		yggr::wstring str_ins(L"abcdefg");
#else
        yggr::string str_ins("abcdefg");
#endif // __MINGW32__
		str.insert(str.begin(), str_ins.begin(), str_ins.end());
		assert(str.size() == 28);
		std::cout << str << std::endl;
	}

	{
#ifndef __MINGW32__
		yggr::wstring str_ins(L"°¡²»´íµÄÅ¶·¢¸ø");
		str.insert(str.begin(), str_ins.rbegin(), str_ins.rend());
#else
        yggr::string str_ins("°¡²»´íµÄÅ¶·¢¸ø");
        str.insert(str.begin(), str_ins.begin(), str_ins.end()); // can't use rbegin() rend()
#endif // __MINGW32__
		assert(str.size() == 35);
		std::cout << str << std::endl;
	}

	{
		str = "utf8×Ö·û´®";
		yggr::utf8_string ustr("ÕâÊÇ¸ö");
		str.insert(str.begin(), ustr.begin(), ustr.end());
		assert(str.size() == 10);

		std::cout << str << std::endl;
	}

	{
		str = "utf8×Ö·û´®";
		yggr::utf8_string ustr("ÕâÊÇ¸ö");
		str.insert(str.begin(), ustr.rbegin(), ustr.rend());
		assert(str.size() == 10);

		std::cout << str << std::endl;
	}

	{
		str = "utf8×Ö·û´®";
		yggr::utf8_string ustr("ÕâÊÇ¸ö");
		str.insert(str.end(), ustr.rbegin(), ustr.rend());
		assert(str.size() == 10);

		std::cout << str << std::endl;
	}

	// erase test
	{
		str = "utf8×Ö·û´®";
		str.erase();
		assert(str.empty());
	}

	{
		str = "utf8×Ö·û´®";
		str.erase(0, 100);
		assert(str.empty());
	}

	{
		str = "utf8×Ö·û´®";
		str.erase(1, 100);
		assert(str.size() == 1);
		std::cout << str << std::endl;
	}

	{
		str = "utf8×Ö·û´®";
		str.erase(6, 1);
		assert(str.size() == 6);
		std::cout << str << std::endl;
	}

	{
		str = "utf8×Ö·û´®";
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
		str = "utf8×Ö·û´®";
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
		str = "string ×Ö·û´®";
		assert(str.size() == 10);
		str.replace(1, 5, 7, 'a');
		assert(str.size() == 12);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
		assert(str.size() == 10);
#ifndef __MINGW32__
		str.replace(1, 5, 7, L'°¡');
#else
        str.replace(1, 5, 7, "°¡");
#endif // __MINGW32__
		assert(str.size() == 12);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
		assert(str.size() == 10);
#ifndef __MINGW32__
		str.replace(8, 2, 7, yggr::utf8_char(L'Å¶'));
#else
        str.replace(8, 2, 7, yggr::utf8_char("Å¶"));
#endif //__MINGW32__
		assert(str.size() == 15);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
		assert(str.size() == 10);
#ifndef __MINGW32__
		str.replace(0, 6, L"×Ö·û´®");
#else
        str.replace(0, 6, "×Ö·û´®");
#endif // __MINGW32__
		int n = str.size();
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
		assert(str.size() == 10);
		str.replace(7, 3, "string");
		assert(str.size() == 13);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
		assert(str.size() == 10);
#ifndef __MINGW32__
		str.replace(0, 6, yggr::wstring(L"×Ö·û´®"), 0, 2);
#else
        str.replace(0, 6, yggr::string("×Ö·û´®"), 0, 2);
#endif // __MINGW32__
		assert(str.size() == 6);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
		yggr::string sstr("abc");
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
		yggr::string sstr("abc");
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
#ifndef __MINGW32__
		yggr::wstring sstr(L"ÄãºÃÂð");
#else
        yggr::string sstr("ÄãºÃÂð");
#endif // __MINGW32__
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
#ifndef __MINGW32__
		yggr::wstring sstr(L"ÄãºÃÂð");
#else
        yggr::string sstr("ÄãºÃÂð");
#endif // __MINGW32__
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
#ifndef __MINGW32__
		yggr::utf8_string sstr(L"ÄãºÃÂð");
#else
        yggr::utf8_string sstr("ÄãºÃÂð");
#endif // __MINGW32__
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
#ifndef __MINGW32__
		yggr::utf8_string sstr(L"ÄãºÃÂð");
#else
        yggr::utf8_string sstr("ÄãºÃÂð");
#endif // __MINGW32__
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
#ifndef __MINGW32__
		yggr::utf8_string sstr(L"ÄãºÃÂð");
#else
        yggr::utf8_string sstr("ÄãºÃÂð");
#endif // __MINGW32__
		assert(str.size() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr);
		assert(str.size() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string ×Ö·û´®";
#ifndef __MINGW32__
		yggr::utf8_string sstr(L"ÄãºÃÂð");
#else
        yggr::utf8_string sstr("ÄãºÃÂð");
#endif // __MINGW32__
		assert(str.size() == 10);
		str.replace(0, 6, sstr, 0, 1);
		assert(str.size() == 5);
		std::cout << str << std::endl;
	}
}

void test_string_operations(void)
{

#ifndef __MINGW32__
	yggr::utf8_string str(L"utf8×Ö·û´®");
#else
    yggr::utf8_string str("utf8×Ö·û´®");
#endif // __MINGW32__

	// find
	{
		assert(str.find('u') == 0);
		assert(str.find('u', 1) == yggr::utf8_string::npos);
#ifndef __MINGW32__
		assert(str.find(L'×Ö') == 4);
		assert(str.find(L'×Ö', 5) == yggr::utf8_string::npos);
		assert(str.find(yggr::utf8_char(L'×Ö')) == 4);
		assert(str.find(yggr::utf8_char(L'×Ö'), 5) == yggr::utf8_string::npos);
#else
        assert(str.find("×Ö") == 4);
		assert(str.find("×Ö", 5) == yggr::utf8_string::npos);
		assert(str.find(yggr::utf8_char("×Ö")) == 4);
		assert(str.find(yggr::utf8_char("×Ö"), 5) == yggr::utf8_string::npos);
#endif // __MINGW32__
	}

	{
		yggr::string sstr("utf8");
		assert(str.find(sstr) == 0);
		assert(str.find("utf8") == 0);
		assert(str.find(sstr, 1) == yggr::utf8_string::npos);
		assert(str.find("utf8", 1) == yggr::utf8_string::npos);
	}

	{
#ifndef __MINGW32__
		yggr::wstring sstr(L"×Ö·û´®");
		assert(str.find(sstr) == 4);
		assert(str.find(L"×Ö·û´®") == 4);
		assert(str.find(sstr, 5) == yggr::utf8_string::npos);
		assert(str.find(L"×Ö·û´®", 5) == yggr::utf8_string::npos);
#else
        yggr::string sstr("×Ö·û´®");
        assert(str.find(sstr) == 4);
		assert(str.find("×Ö·û´®") == 4);
		assert(str.find(sstr, 5) == yggr::utf8_string::npos);
		assert(str.find("×Ö·û´®", 5) == yggr::utf8_string::npos);
#endif // __MINGW32__
	}

	{
#ifndef __MINGW32__
		yggr::utf8_string sstr(L"tf8×Ö·û´®");
#else
        yggr::utf8_string sstr("tf8×Ö·û´®");
#endif // __MINGW32__
		assert(str.find(sstr) == 1);
		assert(str.find(sstr, 2) == yggr::utf8_string::npos);
	}

	// rfind
	{
		assert(str.rfind('u') == 0);
#ifndef __MINGW32__
		assert(str.rfind(L'×Ö') == 4);
		assert(str.rfind(L'×Ö', 3) == yggr::utf8_string::npos);
		assert(str.rfind(yggr::utf8_char(L'×Ö')) == 4);
		assert(str.rfind(yggr::utf8_char(L'×Ö'), 3) == yggr::utf8_string::npos);
#else
        assert(str.rfind("×Ö") == 4);
		assert(str.rfind("×Ö", 3) == yggr::utf8_string::npos);
		assert(str.rfind(yggr::utf8_char("×Ö")) == 4);
		assert(str.rfind(yggr::utf8_char("×Ö"), 3) == yggr::utf8_string::npos);
#endif // __MINGW32__
	}

	{
		yggr::string sstr("utf8");
		assert(str.rfind(sstr) == 0);
		assert(str.rfind("utf8") == 0);
	}

	{
#ifndef __MINGW32__
		yggr::wstring sstr(L"×Ö·û´®");
		assert(str.rfind(sstr) == 4);
		assert(str.rfind(L"×Ö·û´®") == 4);
		assert(str.rfind(sstr, 3) == yggr::utf8_string::npos);
		assert(str.rfind(L"×Ö·û´®", 3) == yggr::utf8_string::npos);
#else
        yggr::string sstr("×Ö·û´®");
		assert(str.rfind(sstr) == 4);
		assert(str.rfind("×Ö·û´®") == 4);
		assert(str.rfind(sstr, 3) == yggr::utf8_string::npos);
		assert(str.rfind("×Ö·û´®", 3) == yggr::utf8_string::npos);
#endif // __MINGW32__
	}

	{
#ifndef __MINGW32__
		yggr::utf8_string sstr(L"tf8×Ö·û´®");
#else
        yggr::utf8_string sstr("tf8×Ö·û´®");
#endif // __MINGW32__
		assert(str.rfind(sstr) == 1);
		assert(str.rfind(sstr, 0) == yggr::utf8_string::npos);
	}

	// find_first_of
	{
		//str == "utf8×Ö·û´®"
#ifndef __MINGW32__
		assert(str.find_first_of(L'×Ö') == 4);
		assert(str.find_first_of(L'×Ö', 1) == 4);
		assert(str.find_first_of(L'×Ö', 5) == yggr::utf8_string::npos);
		assert(str.find_first_of('a') == yggr::utf8_string::npos);
		assert(str.find_first_of("×Öabc") == 4);
		assert(str.find_first_of("abc") == yggr::utf8_string::npos);
		assert(str.find_first_of(L"×Öabc") == 4);
		assert(str.find_first_of(L"abc") == yggr::utf8_string::npos);
		assert(str.find_first_of(L"×Öabc", 1) == 4);
		assert(str.find_first_of(L"×Öabc",5) == yggr::utf8_string::npos);
#else
        assert(str.find_first_of("×Ö") == 4);
		assert(str.find_first_of("×Ö", 1) == 4);
		assert(str.find_first_of("×Ö", 5) == yggr::utf8_string::npos);
		assert(str.find_first_of('a') == yggr::utf8_string::npos);
		assert(str.find_first_of("×Öabc") == 4);
		assert(str.find_first_of("abc") == yggr::utf8_string::npos);
		assert(str.find_first_of("×Öabc") == 4);
		assert(str.find_first_of("abc") == yggr::utf8_string::npos);
		assert(str.find_first_of("×Öabc", 1) == 4);
		assert(str.find_first_of("×Öabc",5) == yggr::utf8_string::npos);
#endif // __MINGW32__
	}

	{
#ifndef __MINGW32__
		assert(str.find_first_of(yggr::utf8_string(L"×Öabc")) == 4);
		assert(str.find_first_of(yggr::utf8_string(L"abc")) == yggr::utf8_string::npos);
		assert(str.find_first_of(yggr::utf8_string(L"×Öabc"), 1) == 4);
		assert(str.find_first_of(yggr::utf8_string(L"×Öabc"),5) == yggr::utf8_string::npos);
#else
        assert(str.find_first_of(yggr::utf8_string("×Öabc")) == 4);
		assert(str.find_first_of(yggr::utf8_string("abc")) == yggr::utf8_string::npos);
		assert(str.find_first_of(yggr::utf8_string("×Öabc"), 1) == 4);
		assert(str.find_first_of(yggr::utf8_string("×Öabc"),5) == yggr::utf8_string::npos);
#endif // __MINGW32__
	}

	// find first_not_of

	{
		//str == "utf8×Ö·û´®"
#ifndef __MINGW32__
		assert(str.find_first_not_of(L'×Ö') == 0);
		assert(str.find_first_not_of(L'×Ö', 1) == 1);
		assert(str.find_first_not_of(L'×Ö', 5) == 5);
		assert(str.find_first_not_of('a') == 0);
		assert(str.find_first_not_of("×Öabc") == 0);
		assert(str.find_first_not_of("abc") == 0);
		assert(str.find_first_not_of(L"×Öabc") == 0);
		assert(str.find_first_not_of(L"abc") == 0);
		assert(str.find_first_not_of(L"×Öabc", 1) == 1);
		assert(str.find_first_not_of(L"×Öabc",5) == 5);
		assert(str.find_first_not_of(L"utf8×Ö·û´®", 1) == yggr::utf8_string::npos);
		assert(str.find_first_not_of(L"", str.size()) == yggr::utf8_string::npos);
		assert(str.find_first_not_of(L"", str.size() - 1) == str.size() - 1);
		assert(str.find_first_not_of(L"") == 0);
#else
        assert(str.find_first_not_of("×Ö") == 0);
		assert(str.find_first_not_of("×Ö", 1) == 1);
		assert(str.find_first_not_of("×Ö", 5) == 5);
		assert(str.find_first_not_of('a') == 0);
		assert(str.find_first_not_of("×Öabc") == 0);
		assert(str.find_first_not_of("abc") == 0);
		assert(str.find_first_not_of("×Öabc") == 0);
		assert(str.find_first_not_of("abc") == 0);
		assert(str.find_first_not_of("×Öabc", 1) == 1);
		assert(str.find_first_not_of("×Öabc",5) == 5);
		assert(str.find_first_not_of("utf8×Ö·û´®", 1) == yggr::utf8_string::npos);
		assert(str.find_first_not_of("", str.size()) == yggr::utf8_string::npos);
		assert(str.find_first_not_of("", str.size() - 1) == str.size() - 1);
		assert(str.find_first_not_of("") == 0);
#endif // __MINGW32__
	}

	{

#ifndef __MINGW32__
		assert(str.find_first_not_of(yggr::utf8_string(L"×Öabc")) == 0);
		assert(str.find_first_not_of(yggr::utf8_string(L"abc")) == 0);
		assert(str.find_first_not_of(yggr::utf8_string(L"×Öabc"), 1) == 1);
		assert(str.find_first_not_of(yggr::utf8_string(L"×Öabc"),5) == 5);
		assert(str.find_first_not_of(yggr::utf8_string(L"utf8×Ö·û´®"), 1) == yggr::utf8_string::npos);

		assert(str.find_first_not_of(yggr::utf8_string(L""), str.size()) == yggr::utf8_string::npos);
		assert(str.find_first_not_of(yggr::utf8_string(L""), str.size() - 1) == str.size() - 1);
		assert(str.find_first_not_of(yggr::utf8_string(L"")) == 0);
#else
        assert(str.find_first_not_of(yggr::utf8_string("×Öabc")) == 0);
		assert(str.find_first_not_of(yggr::utf8_string("abc")) == 0);
		assert(str.find_first_not_of(yggr::utf8_string("×Öabc"), 1) == 1);
		assert(str.find_first_not_of(yggr::utf8_string("×Öabc"),5) == 5);
		assert(str.find_first_not_of(yggr::utf8_string("utf8×Ö·û´®"), 1) == yggr::utf8_string::npos);

		assert(str.find_first_not_of(yggr::utf8_string(""), str.size()) == yggr::utf8_string::npos);
		assert(str.find_first_not_of(yggr::utf8_string(""), str.size() - 1) == str.size() - 1);
		assert(str.find_first_not_of(yggr::utf8_string("")) == 0);
#endif // __MINGW32__

	}

	// find_last_of
	{
		//str == "utf8×Ö·û´®"
#ifndef __MINGW32__
		assert(str.find_last_of(L'×Ö') == 4);
		assert(str.find_last_of(L'×Ö', 1) == yggr::utf8_string::npos);
		assert(str.find_last_of(L'×Ö', 5) == 4);
		assert(str.find_last_of('a') == yggr::utf8_string::npos);
		assert(str.find_last_of("×Öabc") == 4);
		assert(str.find_last_of("abc") == yggr::utf8_string::npos);
		assert(str.find_last_of(L"×Öabc") == 4);
		assert(str.find_last_of(L"abc") == yggr::utf8_string::npos);
		assert(str.find_last_of(L"×Öabc", 1) == yggr::utf8_string::npos);
		assert(str.find_last_of(L"×Öabc",5) == 4);
		assert(str.find_last_of('u', 0) == 0);
		assert(str.find_last_of("u", 0) == 0);
#else
        assert(str.find_last_of("×Ö") == 4);
		assert(str.find_last_of("×Ö", 1) == yggr::utf8_string::npos);
		assert(str.find_last_of("×Ö", 5) == 4);
		assert(str.find_last_of('a') == yggr::utf8_string::npos);
		assert(str.find_last_of("×Öabc") == 4);
		assert(str.find_last_of("abc") == yggr::utf8_string::npos);
		assert(str.find_last_of("×Öabc") == 4);
		assert(str.find_last_of("abc") == yggr::utf8_string::npos);
		assert(str.find_last_of("×Öabc", 1) == yggr::utf8_string::npos);
		assert(str.find_last_of("×Öabc",5) == 4);
		assert(str.find_last_of('u', 0) == 0);
		assert(str.find_last_of("u", 0) == 0);
#endif // __MINGW32__
	}

	{
#ifndef __MINGW32__
		assert(str.find_last_of(yggr::utf8_string(L"×Öabc")) == 4);
		assert(str.find_last_of(yggr::utf8_string(L"abc")) == yggr::utf8_string::npos);
		assert(str.find_last_of(yggr::utf8_string(L"×Öabc"), 1) == yggr::utf8_string::npos);
		assert(str.find_last_of(yggr::utf8_string(L"×Öabc"),5) == 4);
#else
        assert(str.find_last_of(yggr::utf8_string("×Öabc")) == 4);
		assert(str.find_last_of(yggr::utf8_string("abc")) == yggr::utf8_string::npos);
		assert(str.find_last_of(yggr::utf8_string("×Öabc"), 1) == yggr::utf8_string::npos);
		assert(str.find_last_of(yggr::utf8_string("×Öabc"),5) == 4);
#endif // __MINGW32__
		assert(str.find_last_of(yggr::utf8_string("u"), 0) == 0);
	}

	// find_last_not_of
	{
		//str == "utf8×Ö·û´®"
#ifndef __MINGW32__
		assert(str.find_last_not_of(L'×Ö') == str.size() - 1);
		assert(str.find_last_not_of(L'×Ö', 1) == 1);
		assert(str.find_last_not_of(L'×Ö', 5) == 5);
		assert(str.find_last_not_of('a') == str.size() - 1);
		assert(str.find_last_not_of("×Öabc") == str.size() - 1);
		assert(str.find_last_not_of("abc") == str.size() - 1);
		assert(str.find_last_not_of(L"×Öabc") == str.size() - 1);
		assert(str.find_last_not_of(L"abc") == str.size() - 1);
		assert(str.find_last_not_of(L"×Öabc", 1) == 1);
		assert(str.find_last_not_of(L"×Öabc",5) == 5);
		assert(str.find_last_not_of(L"utf8×Ö·û´®", 1) == yggr::utf8_string::npos);
		assert(str.find_last_not_of(L"", 0) == 0);
		assert(str.find_last_not_of(L"", str.size() - 1) == str.size() - 1);
		assert(str.find_last_not_of(L"") == str.size() - 1);
#else
        assert(str.find_last_not_of("×Ö") == str.size() - 1);
		assert(str.find_last_not_of("×Ö", 1) == 1);
		assert(str.find_last_not_of("×Ö", 5) == 5);
		assert(str.find_last_not_of('a') == str.size() - 1);
		assert(str.find_last_not_of("×Öabc") == str.size() - 1);
		assert(str.find_last_not_of("abc") == str.size() - 1);
		assert(str.find_last_not_of("×Öabc") == str.size() - 1);
		assert(str.find_last_not_of("abc") == str.size() - 1);
		assert(str.find_last_not_of("×Öabc", 1) == 1);
		assert(str.find_last_not_of("×Öabc",5) == 5);
		assert(str.find_last_not_of("utf8×Ö·û´®", 1) == yggr::utf8_string::npos);
		assert(str.find_last_not_of("", 0) == 0);
		assert(str.find_last_not_of("", str.size() - 1) == str.size() - 1);
		assert(str.find_last_not_of("") == str.size() - 1);
#endif // __MINGW32__
	}

	{
#ifndef __MINGW32__
		assert(str.find_last_not_of(yggr::utf8_string(L"×Öabc")) == str.size() - 1);
		assert(str.find_last_not_of(yggr::utf8_string(L"abc")) == str.size() - 1);
		assert(str.find_last_not_of(yggr::utf8_string(L"×Öabc"), 1) == 1);
		assert(str.find_last_not_of(yggr::utf8_string(L"×Öabc"),5) == 5);
		assert(str.find_last_not_of(yggr::utf8_string(L"utf8×Ö·û´®"), 1) == yggr::utf8_string::npos);

		assert(str.find_last_not_of(yggr::utf8_string(L""), 0) == 0);
		assert(str.find_last_not_of(yggr::utf8_string(L""), str.size() - 1) == str.size() - 1);
#else
        assert(str.find_last_not_of(yggr::utf8_string("×Öabc")) == str.size() - 1);
		assert(str.find_last_not_of(yggr::utf8_string("abc")) == str.size() - 1);
		assert(str.find_last_not_of(yggr::utf8_string("×Öabc"), 1) == 1);
		assert(str.find_last_not_of(yggr::utf8_string("×Öabc"),5) == 5);
		assert(str.find_last_not_of(yggr::utf8_string("utf8×Ö·û´®"), 1) == yggr::utf8_string::npos);

		assert(str.find_last_not_of(yggr::utf8_string(""), 0) == 0);
		assert(str.find_last_not_of(yggr::utf8_string(""), str.size() - 1) == str.size() - 1);
#endif // __MINGW32__
	}

	// compare

	{
		//str = "utf8×Ö·û´®";
		assert(str.compare("utf8") > 0);
		assert(str.compare("utf8_string") > 0);
		assert(str.compare("utf8×Ö·û´®³¤") < 0);
		assert(str.compare("") > 0);
		assert(str.compare("utf8×Ö·û´®") == 0);

#ifndef __MINGW32__
		assert(str.compare(L"utf8") > 0);
		assert(str.compare(L"utf8_string") > 0);
		assert(str.compare(L"utf8×Ö·û´®³¤") < 0);
		assert(str.compare(L"") > 0);
		assert(str.compare(L"utf8×Ö·û´®") == 0);
#else
        assert(str.compare("utf8") > 0 );
		assert(str.compare("utf8_string") > 0 );
		assert(str.compare("utf8×Ö·û´®³¤") < 0);
		assert(str.compare("") > 0);
		assert(str.compare("utf8×Ö·û´®") == 0);
#endif // __MINGW32__

		assert(str.compare("utf8", 0, 1) > 0);
		assert(str.compare("utf8_string", 2, 7) > 0);
		assert(str.compare("utf8", 0, 0) > 0);

#ifndef __MINGW32__
		assert(str.compare(L"utf8", 0, 1) > 0);
		assert(str.compare(L"utf8_string", 2, 7) > 0);
		assert(str.compare(L"utf8×Ö·û´®³¤³¤³¤", 0, 8) < 0);
		assert(str.compare(L"utf8", 0, 0) > 0);
		assert(str.compare(L"³¤utf8×Ö·û´®³¤³¤", 1, 7) == 0);
#else
        assert(str.compare("utf8", 0, 1) > 0 );
		assert(str.compare("utf8_string", 2, 7) > 0);
		assert(str.compare("utf8", 0, 0) > 0);
#endif // __MINGW32__

#ifndef __MINGW32__
		assert(str.compare(4, 2, "×Ö·û") == 0);
		assert(str.compare(4, 2, L"×Ö·û") == 0);
		assert(str.compare(4, 2, yggr::string("×Ö·û")) == 0);
		assert(str.compare(4, 2, yggr::wstring(L"×Ö·û")) == 0);

		assert(str.compare(4, 2, "×Ö·û³¤") < 0);
		assert(str.compare(4, 2, L"×Ö·û³¤") < 0);
		assert(str.compare(4, 2, yggr::string("×Ö·û³¤"))< 0);
		assert(str.compare(4, 2, yggr::wstring(L"×Ö·û³¤")) < 0);

		assert(str.compare(4, 3, "×Ö·û") > 0);
		assert(str.compare(4, 3, L"×Ö·û") > 0);
		assert(str.compare(4, 3, yggr::string("×Ö·û")) > 0);
		assert(str.compare(4, 3, yggr::wstring(L"×Ö·û")) > 0);

		assert(str.compare(4, 2, yggr::utf8_string("×Ö·û")) == 0);
		assert(str.compare(4, 2, yggr::utf8_string(L"×Ö·û")) == 0);

		assert(str.compare(4, 2, yggr::utf8_string("×Ö·û³¤")) < 0);
		assert(str.compare(4, 2, yggr::wstring(L"×Ö·û³¤")) < 0);

		assert(str.compare(4, 3, yggr::utf8_string("×Ö·û")) > 0);
		assert(str.compare(4, 3, yggr::utf8_string(L"×Ö·û")) > 0);
#else
        assert(str.compare(4, 2, "×Ö·û") == 0);
		assert(str.compare(4, 2, yggr::string("×Ö·û")) == 0);

		assert(str.compare(4, 2, "×Ö·û³¤") < 0);
		assert(str.compare(4, 2, yggr::string("×Ö·û³¤")) < 0);

		assert(str.compare(4, 3, "×Ö·û") > 0);
		assert(str.compare(4, 3, yggr::string("×Ö·û")) > 0);

		assert(str.compare(4, 2, yggr::utf8_string("×Ö·û")) == 0);

		assert(str.compare(4, 2, yggr::utf8_string("×Ö·û³¤")) < 0);

		assert(str.compare(4, 3, yggr::utf8_string("×Ö·û")) > 0);
#endif //
	}
}

void non_member_foo_test(void)
{
	// operator +
	{
#ifndef __MINGW32__
		yggr::utf8_string str("utf8×Ö·û´®");
		std::cout << "utf8×Ö·û" + str + "aaa" << std::endl;
		std::cout << L"utf8×Ö·û" + str + L"bbb" << std::endl;

		std::cout << yggr::string("utf8×Ö·û") + str << std::endl;
		std::wcout.imbue(std::locale("chs"));
		std::wcout << yggr::wstring(L"utf8×Ö·û") + str << std::endl;

		std::cout << "utf8×Ö·û" + str + yggr::string("aaa") << std::endl;
		std::cout << L"utf8×Ö·û" + str + yggr::wstring(L"bbb") << std::endl;

		std::cout << yggr::utf8_string("utf8×Ö·û") + str + yggr::string("aaa") << std::endl;
		std::cout << yggr::utf8_string(L"utf8×Ö·û") + str + yggr::wstring(L"bbb") << std::endl;
#else
        yggr::utf8_string str("utf8×Ö·û´®");
		std::cout << "utf8×Ö·û" + str + "aaa" << std::endl;

		std::cout << yggr::string("utf8×Ö·û") + str << std::endl;

		std::cout << "utf8×Ö·û" + str + yggr::string("aaa") << std::endl;

		std::cout << yggr::utf8_string("utf8×Ö·û") + str + yggr::string("aaa") << std::endl;
#endif // __MINGW32__
	}

	// operator ==
	{
#ifndef __MINGW32__
		yggr::utf8_string str("utf8×Ö·û´®");
		assert(str == "utf8×Ö·û´®");
		assert("utf8×Ö·û´®" == str);
		assert(str == L"utf8×Ö·û´®");
		assert(L"utf8×Ö·û´®" == str);

		assert(str == yggr::string("utf8×Ö·û´®"));
		assert(yggr::string("utf8×Ö·û´®") == str);
		assert(str == yggr::wstring(L"utf8×Ö·û´®"));
		assert(yggr::wstring(L"utf8×Ö·û´®") == str);

		assert(str == yggr::utf8_string("utf8×Ö·û´®"));
		assert(yggr::utf8_string("utf8×Ö·û´®") == str);
		assert(str == yggr::utf8_string(L"utf8×Ö·û´®"));
		assert(yggr::utf8_string(L"utf8×Ö·û´®") == str);
#else
        yggr::utf8_string str("utf8×Ö·û´®");
		assert(str == "utf8×Ö·û´®");
		assert("utf8×Ö·û´®" == str);

		assert(str == yggr::string("utf8×Ö·û´®"));
		assert(yggr::string("utf8×Ö·û´®") == str);

		assert(str == yggr::utf8_string("utf8×Ö·û´®"));
		assert(yggr::utf8_string("utf8×Ö·û´®") == str);
#endif // __MINGW32__
	}

	// operator !=
	{
		yggr::utf8_string str("utf8×Ö·û´®");
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
#ifndef __MINGW32__
		yggr::utf8_string str("utf8×Ö·û´®");
		assert(str < "utf8×Ö·û´®³¤");
		assert(str < L"utf8×Ö·û´®³¤");
		assert(str < yggr::string("utf8×Ö·û´®³¤"));
		assert(str < yggr::wstring(L"utf8×Ö·û´®³¤"));
		assert("" < str );

		assert(!("utf8×Ö·û´®³¤" < str));
		assert(!(L"utf8×Ö·û´®³¤" < str));
		assert(!(yggr::string("utf8×Ö·û´®³¤") < str));
		assert(!(yggr::wstring(L"utf8×Ö·û´®³¤") < str));
		assert(!(str < yggr::wstring(L"")));

		assert(str < yggr::utf8_string("utf8×Ö·û´®³¤"));
		assert(!(yggr::utf8_string(L"utf8×Ö·û´®³¤") < str));
#else
        yggr::utf8_string str("utf8×Ö·û´®");
		assert(str < "utf8×Ö·û´®³¤");
		assert(str < yggr::string("utf8×Ö·û´®³¤"));
		assert("" < str );

		assert(!("utf8×Ö·û´®³¤" < str));
		assert(!(yggr::string("utf8×Ö·û´®³¤") < str));
		assert(!(str < yggr::wstring(L"")));

		assert(str < yggr::utf8_string("utf8×Ö·û´®³¤"));
#endif // __MINGW32__
	}

	// operator <=
	{
#ifndef __MINGW32__
		yggr::utf8_string str("utf8×Ö·û´®");
		assert(str <= "utf8×Ö·û´®³¤");
		assert(str <= "utf8×Ö·û´®");
		assert(str <= L"utf8×Ö·û´®³¤");
		assert(str <= L"utf8×Ö·û´®");
		assert(str <= yggr::string("utf8×Ö·û´®³¤"));
		assert(str <= yggr::string("utf8×Ö·û´®"));
		assert(str <= yggr::wstring(L"utf8×Ö·û´®³¤"));
		assert(str <= yggr::wstring(L"utf8×Ö·û´®"));

		assert(!("utf8×Ö·û´®³¤" <= str));
		assert(!(L"utf8×Ö·û´®³¤" <= str));
		assert(!(yggr::string("utf8×Ö·û´®³¤") <= str));
		assert(!(yggr::wstring(L"utf8×Ö·û´®³¤") <= str));

		assert(str <= yggr::utf8_string("utf8×Ö·û´®³¤"));
		assert(!(yggr::utf8_string(L"utf8×Ö·û´®³¤") <= str));
#else
        yggr::utf8_string str("utf8×Ö·û´®");
		assert(str <= "utf8×Ö·û´®³¤");
		assert(str <= "utf8×Ö·û´®");
		assert(str <= yggr::string("utf8×Ö·û´®³¤"));
		assert(str <= yggr::string("utf8×Ö·û´®"));

		assert(!("utf8×Ö·û´®³¤" <= str));
		assert(!(yggr::string("utf8×Ö·û´®³¤") <= str));

		assert(str <= yggr::utf8_string("utf8×Ö·û´®³¤"));
#endif // __MINGW32__
	}

	// operator >
	{
#ifndef __MINGW32__
		yggr::utf8_string str("utf8×Ö·û´®");
		assert(str > "utf8×Ö·û");
		assert(str > L"utf8×Ö·û");
		assert(str > yggr::string("utf8×Ö·û"));
		assert(str > yggr::wstring(L"utf8×Ö·û"));
		assert(str > "" );

		assert(!("utf8×Ö·û" > str));
		assert(!(L"utf8×Ö·û" > str));
		assert(!(yggr::string("utf8×Ö·û") > str));
		assert(!(yggr::wstring(L"utf8×Ö·û") > str));

		assert(str > yggr::utf8_string("utf8×Ö·û"));
		assert(!(yggr::utf8_string(L"utf8×Ö·û") > str));
#else
        yggr::utf8_string str("utf8×Ö·û´®");
		assert(str > "utf8×Ö·û");
		assert(str > yggr::string("utf8×Ö·û"));
		assert(str > "" );

		assert(!("utf8×Ö·û" > str));
		assert(!(yggr::string("utf8×Ö·û") > str));

		assert(str > yggr::utf8_string("utf8×Ö·û"));
#endif // __MINGW32__
	}

	// operator >=
	{
#ifndef __MINGW32__
		yggr::utf8_string str("utf8×Ö·û´®");
		assert(str >= "utf8×Ö·û");
		assert(str >= "utf8×Ö·û´®");
		assert(str >= L"utf8×Ö·û");
		assert(str >= L"utf8×Ö·û´®");
		assert(str >= yggr::string("utf8×Ö·û"));
		assert(str >= yggr::string("utf8×Ö·û´®"));
		assert(str >= yggr::wstring(L"utf8×Ö·û"));
		assert(str >= yggr::wstring(L"utf8×Ö·û´®"));

		assert(!("utf8×Ö·û" >= str));
		assert(!(L"utf8×Ö·û" >= str));
		assert(!(yggr::string("utf8×Ö·û") >= str));
		assert(!(yggr::wstring(L"utf8×Ö·û") >= str));

		assert(str >= yggr::utf8_string("utf8×Ö·û"));
		assert(!(yggr::utf8_string(L"utf8×Ö·û") >= str));
#else
        yggr::utf8_string str("utf8×Ö·û´®");
		assert(str >= "utf8×Ö·û");
		assert(str >= "utf8×Ö·û´®");
		assert(str >= yggr::string("utf8×Ö·û"));
		assert(str >= yggr::string("utf8×Ö·û´®"));

		assert(!("utf8×Ö·û" >= str));
		assert(!(yggr::string("utf8×Ö·û") >= str));

		assert(str >= yggr::utf8_string("utf8×Ö·û"));
#endif // __MINGW32__
	}

	// swap
	{
		yggr::utf8_string str1_chk("utf8×Ö·û´®1");
		yggr::utf8_string str2_chk("utf8×Ö·û´®2");

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

#ifndef __MINGW32__
	std::wcin.imbue(std::locale("chs"));
	std::wcin >> in;
	std::cout << in << std::endl;
#endif // __MINGW32__
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
