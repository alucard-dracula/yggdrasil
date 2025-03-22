// utf8_foo_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

// #define YGGR_NO_CWCHAR

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_default_charset_config(void)
{
	yggr_test_assert(yggr::string("char") == yggr::charset::default_charset_config::get_default_string_charset_name<char>());
//#if _LIBICONV_VERSION < 0x010D
//	yggr_test_assert(yggr::string("ucs-2le") == yggr::charset::default_charset_config::get_default_string_charset_name<wchar_t>());
//#else
//	yggr_test_assert(yggr::string("wchar_t") == yggr::charset::default_charset_config::get_default_string_charset_name<wchar_t>());
//#endif // _LIBICONV_VERSION
#if !(defined(YGGR_NO_U16STRING_T) && defined(YGGR_NO_U32STRING_T))
	if(sizeof(wchar_t) == sizeof(yggr::c16))
	{
		yggr_test_assert(yggr::charset::default_charset_config::get_default_string_charset_name<yggr::c16>()
			   == yggr::charset::default_charset_config::get_default_string_charset_name<wchar_t>());
	}
	else if(sizeof(wchar_t) == sizeof(yggr::c32))
	{
		yggr_test_assert(yggr::charset::default_charset_config::get_default_string_charset_name<yggr::c32>()
			   == yggr::charset::default_charset_config::get_default_string_charset_name<wchar_t>());
	}
	else
	{
		yggr_test_assert(false);
	}
#else
	yggr_test_assert(yggr::string("wchar_t") == yggr::charset::default_charset_config::get_default_string_charset_name<wchar_t>());
	yggr_test_assert(yggr::charset::default_charset_config::get_str_default_string_charset_name<wchar_t>() == yggr::string("wchar_t"));
#endif // YGGR_NO_U16STRING_T


	yggr_test_assert(yggr::charset::default_charset_config::get_str_default_string_charset_name<char>() == yggr::string("char"));

//#if _LIBICONV_VERSION < 0x010D
//	yggr_test_assert(yggr::charset::default_charset_config::get_str_default_string_charset_name<wchar_t>() == yggr::string("ucs-2le"));
//#else
//	yggr_test_assert(yggr::charset::default_charset_config::get_str_default_string_charset_name<wchar_t>() == yggr::string("wchar_t"));
//#endif // _LIBICONV_VERSION

	std::cout << "---------test_default_charset_config end------------" << std::endl;
}

void test_charset_base_foo(void)
{
	std::string std_str("abc你好");
	boost::container::string boost_str("abc你好");

	std::wstring std_wstr(L"abc你好");
	boost::container::wstring boost_wstr(L"abc你好");

	yggr_test_assert(yggr::charset::charset_base_foo::xchglen(std_str.c_str()) == (std_str.size() + 1) * 4);
	yggr_test_assert(yggr::charset::charset_base_foo::xchglen(boost_str.c_str()) == (boost_str.size() + 1) * 4);
	yggr_test_assert(yggr::charset::charset_base_foo::xchglen(std_str) == (std_str.size() + 1) * 4);
	yggr_test_assert(yggr::charset::charset_base_foo::xchglen(boost_str) == (boost_str.size() + 1) * 4);

	yggr_test_assert(yggr::charset::charset_base_foo::xchglen(std_wstr.c_str()) == (std_wstr.size() + 1) * 4);
	yggr_test_assert(yggr::charset::charset_base_foo::xchglen(boost_wstr.c_str()) == (boost_wstr.size() + 1) * 4);
	yggr_test_assert(yggr::charset::charset_base_foo::xchglen(std_wstr) == (std_wstr.size() + 1) * 4);
	yggr_test_assert(yggr::charset::charset_base_foo::xchglen(boost_wstr) == (boost_wstr.size() + 1) * 4);

	std::string std_lstr(100, 0);
	memcpy(&std_lstr[0], &std_str[0], sizeof(std::string::value_type) * std_str.size());
	yggr_test_assert(yggr::charset::charset_base_foo::normal_length(std_lstr).size() == std_str.size());

	boost::container::string boost_lstr(100, 0);
	memcpy(&boost_lstr[0], &boost_str[0], sizeof(boost::container::string::value_type) * boost_str.size());
	yggr_test_assert(yggr::charset::charset_base_foo::normal_length(boost_lstr).size() == boost_str.size());

	std::wstring std_lwstr(100, 0);
	memcpy(&std_lwstr[0], &std_wstr[0], sizeof(std::wstring::value_type) * std_wstr.size());
	yggr_test_assert(yggr::charset::charset_base_foo::normal_length(std_lwstr).size() == std_wstr.size());

	boost::container::wstring boost_lwstr(100, 0);
	memcpy(&boost_lwstr[0], &boost_wstr[0], sizeof(boost::container::wstring::value_type) * boost_wstr.size());
	yggr_test_assert(yggr::charset::charset_base_foo::normal_length(boost_lwstr).size() == boost_wstr.size());

	std::cout << "---------test_charset_base_foo end------------" << std::endl;
}

void test_charset_converter(void)
{
	yggr::charset::charset_converter conv(yggr::charset::default_charset_config::get_default_string_charset_name<char>(),
											yggr::charset::default_charset_config::get_default_string_charset_name<wchar_t>());

	std::string std_str("abc你好");
	std::wstring std_wstr;
	conv(std_str, std_wstr);
	std::wcout << std_wstr << std::endl;

	boost::container::string boost_str("abc你好");
	boost::container::wstring boost_wstr;
	conv(boost_str, boost_wstr);
	std::wcout << boost_wstr << std::endl;

	boost::container::string boost_wstr_tmp2;
	boost::container::wstring boost_wstr2;
	conv(std_str, boost_wstr2);
	std::wcout << boost_wstr2 << std::endl;

	yggr::charset::charset_converter conv3(yggr::charset::default_charset_config::get_default_string_charset_name<wchar_t>(),
											yggr::charset::default_charset_config::get_default_string_charset_name<char>());

	std::wstring std_wstr3(L"abc_你好_wchar_t");
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

		const std::string& cref_str = yggr::charset::charset_foo::s_xchg<std::string>(str); // return cref

		std::string& ref_str = const_cast<std::string&>(cref_str);

		ref_str[0] = 'd';
		std::cout << ref_str << std::endl;
		std::cout << str << std::endl;
		yggr_test_assert(&ref_str[0] == &str[0]);
	}

	// char to char diff
	{
		std::string str("abc2你好");

		//boost::container::string boost_str = yggr::charset::charset_foo::s_xchg<boost::container::string>(str); //return diff_type
		boost::container::string tmp(yggr::charset::charset_foo::s_xchg<boost::container::string>(str));
		boost::container::string boost_str(boost::move(tmp)); //return diff_type

		str[0] = 'd';
		std::cout << str << std::endl;
		std::cout << boost_str << std::endl;
		yggr_test_assert(&str[0] != &boost_str[0]);
	}

	//	wchar to wchar same
	{
		std::wstring str(L"abcw1你好");
		const std::wstring& cref_str = yggr::charset::charset_foo::s_xchg<std::wstring>(str); //return cref

		std::wstring& ref_str = const_cast<std::wstring&>(cref_str);

		ref_str[0] = L'd';

		std::wcout << ref_str << std::endl;
		std::wcout << str << std::endl;
		yggr_test_assert(reinterpret_cast<void*>(&ref_str[0]) == reinterpret_cast<void*>(&str[0]));

	}

	// wchar to wchar diff
	{
		std::wstring str(L"abcw2你好");

		//boost::container::wstring boost_str = yggr::charset::charset_foo::s_xchg<boost::container::wstring>(str);
		boost::container::wstring tmp(yggr::charset::charset_foo::s_xchg<boost::container::wstring>(str));
		boost::container::wstring boost_str(boost::move(tmp));

		str[0] = L'd';

		std::wcout << str << std::endl;
		std::wcout << boost_str << std::endl;
		yggr_test_assert(reinterpret_cast<void*>(&str[0]) != reinterpret_cast<void*>(&boost_str[0]));

	}

	// char to wchar diff
	{
		std::string str("abc3你好");

        boost::container::wstring tmp(yggr::charset::charset_foo::s_xchg<boost::container::wstring>(str));
		boost::container::wstring boost_str(boost::move(tmp)); //change string store type to wstring store type

		std::string rst_str(reinterpret_cast<const char*>(&boost_str[0]), 
								reinterpret_cast<const char*>(&boost_str[0] + boost_str.size()));

		yggr_test_assert(str.size() <= rst_str.size());
		yggr_test_assert(0 == memcmp(&boost_str[0], &str[0], str.size() * sizeof(char)));
		std::cout << str << std::endl;
		std::cout << rst_str << std::endl;
		yggr_test_assert(&str[0] != &rst_str[0]);
	}

	// wchar to char diff
	{
		std::wstring str(L"abcw3你好");

        boost::container::string tmp(yggr::charset::charset_foo::s_xchg<boost::container::string>(str));
		boost::container::string boost_str(boost::move(tmp)); //change wstring store type to string store type

		std::wstring rst_str((const wchar_t*)&boost_str[0], (const wchar_t*)(&boost_str[0] + boost_str.size()));

		yggr_test_assert(str.size() == rst_str.size());
		yggr_test_assert(0 == memcmp(&boost_str[0], &str[0], str.size() * sizeof(wchar_t)));

		std::wcout << str << std::endl;
		std::wcout << rst_str << std::endl;
		yggr_test_assert(reinterpret_cast<void*>(&str[0]) != reinterpret_cast<void*>(&rst_str[0]));

	}

	// string to wstring diff
	{
		std::string str("abc4 你好");

		boost::container::wstring tmp(yggr::charset::charset_foo::s_xchg<boost::container::wstring>(str, "gbk", "wchar_t"));
		boost::container::wstring boost_str(boost::move(tmp));

		std::wcout << boost_str << std::endl;
		std::cout << str << std::endl;
	}

	{
		std::wstring str(L"abcw4 你好");

        boost::container::string tmp(yggr::charset::charset_foo::s_xchg<boost::container::string>(str, "wchar_t", "gbk"));
		boost::container::string boost_str(boost::move(tmp));

		std::wcout << str << std::endl;
		std::cout << boost_str << std::endl;
	}

	std::cout << "---------test_charset_foo end------------" << std::endl;
}

void test_str_wstr_converter_test(void)
{
	{
		std::string str("abc��cd��");
		std::wstring wstr(yggr::charset::str_wstr_converter::s_conv(str));
		std::string str2(yggr::charset::str_wstr_converter::s_conv(wstr));

		std::cout << str << std::endl;
		std::wcout << wstr << std::endl;
		std::cout << str2 << std::endl;
	}

	{
		std::wstring wstr(L"abcw1��cd��");
		std::string str(yggr::charset::str_wstr_converter::s_conv(wstr));
		std::wstring wstr2(yggr::charset::str_wstr_converter::s_conv(str));

		std::cout << str << std::endl;
		std::wcout << wstr << std::endl;
		std::wcout << wstr2 << std::endl;
	}

	{
		yggr::string str("abc2��cd��");
		yggr::wstring wstr(yggr::charset::str_wstr_converter::s_conv(str));
		yggr::string str2(yggr::charset::str_wstr_converter::s_conv(wstr));

		std::cout << str << std::endl;
		std::wcout << wstr << std::endl;
		std::cout << str2 << std::endl;
	}

	{
		yggr::wstring wstr(L"abcw2��cd��");
		yggr::string str(yggr::charset::str_wstr_converter::s_conv(wstr));
		yggr::wstring wstr2(yggr::charset::str_wstr_converter::s_conv(str));

		std::cout << str << std::endl;
		std::wcout << wstr << std::endl;
		std::wcout << wstr2 << std::endl;
	}

	std::cout << "---------test_str_wstr_converter_test end------------" << std::endl;
}

void test_utf8_foo(void)
{
	// '��' unicode = 0x4e25 : 2, utf8 = 0xe4b8a5 : 3
	// charlen
	{
		yggr_test_assert(yggr::charset::utf8_foo::charlen('a') == 1);
		yggr_test_assert(yggr::charset::utf8_foo::charlen("a") == 1);
#ifndef _DEBUG
		yggr_test_assert(yggr::charset::utf8_foo::charlen("��") == 0); // debug assert(false), release == 0
#endif // _DEBUG
		
		yggr_test_assert(yggr::charset::utf8_foo::charlen(std::string("a")) == 1);
#ifndef _DEBUG
		yggr_test_assert(yggr::charset::utf8_foo::charlen(std::string("��")) == 0); // debug assert(false), release == 0
#endif // _DEBUG
		
		yggr_test_assert(yggr::charset::utf8_foo::charlen(std::string("a").begin()) == 1);
#ifndef _DEBUG
		yggr_test_assert(yggr::charset::utf8_foo::charlen(std::string("��").begin()) == 0);
#endif // _DEBUG

		{
			char str[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5)};
			yggr_test_assert(yggr::charset::utf8_foo::charlen(str) == 3);
			yggr_test_assert(yggr::charset::utf8_foo::charlen(str, 4) == 3);
		}

		{
			char str[] = { static_cast<char>(0x4e), static_cast<char>(0x25) };
			yggr_test_assert(yggr::charset::utf8_foo::charlen(str) != 0); // !!!!invalid use !!!!
#ifndef _DEBUG
			yggr_test_assert(yggr::charset::utf8_foo::charlen(str, 2) == 0); // !!!!right use !!!! // debug assert(false), release == 0
#endif // _DEBUG
		}

#ifndef YGGR_NO_CWCHAR
		//yggr_test_assert(yggr::charset::utf8_foo::charlen(L"a") == 1);  // !!!!invalid use !!!!
#	ifndef _DEBUG
		yggr_test_assert(yggr::charset::utf8_foo::charlen(L"a", 2) == 0); // !!!!right use !!!! // debug assert(false), release == 0
#	endif // _DEBUG

#	ifndef _DEBUG
		yggr_test_assert(yggr::charset::utf8_foo::charlen(L'��', 2) == 0); // debug assert(false), release == 0
		yggr_test_assert(yggr::charset::utf8_foo::charlen(L"��", 2) == 0); // debug assert(false), release == 0


		yggr_test_assert(yggr::charset::utf8_foo::charlen(std::wstring(L"a", 2)) == 0); // debug assert(false), release == 0
		yggr_test_assert(yggr::charset::utf8_foo::charlen(std::wstring(L"��", 2)) == 0); // debug assert(false), release == 0
		
		yggr_test_assert(yggr::charset::utf8_foo::charlen(std::wstring(L"a").begin(), 2) == 0); // debug assert(false), release == 0
		yggr_test_assert(yggr::charset::utf8_foo::charlen(std::wstring(L"��").begin(), 2) == 0); // debug assert(false), release == 0

#	endif // _DEBUG

#endif // YGGR_NO_CWCHAR
	}

	// charcmp
	{
		char str[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5)};
		char str2[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5)};
		char str3[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa4)};

		yggr_test_assert(-1 == yggr::charset::utf8_foo::charcmp('a', 'b'));
		yggr_test_assert(1 == yggr::charset::utf8_foo::charcmp('b', 'a'));

		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp('a', "a"));
		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp("a", 'a'));

		yggr_test_assert(-1 == yggr::charset::utf8_foo::charcmp('a', str));
		yggr_test_assert(1 == yggr::charset::utf8_foo::charcmp(str, 'a'));

		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp('a', std::string("a")));
		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp(std::string("a"), 'a'));

		yggr_test_assert(-1 == yggr::charset::utf8_foo::charcmp('a', std::string("b")));
		yggr_test_assert(1 == yggr::charset::utf8_foo::charcmp(std::string("b"), 'a'));

		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp('a', std::string("a").begin()));
		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp(std::string("a").begin(), 'a'));

		yggr_test_assert(-1 == yggr::charset::utf8_foo::charcmp('a', std::string("b").begin()));
		yggr_test_assert(1 == yggr::charset::utf8_foo::charcmp(std::string("b").begin(), 'a'));

		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp("a", std::string("a").begin()));
		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp(std::string("a").begin(), "a"));

		yggr_test_assert(-1 == yggr::charset::utf8_foo::charcmp("a", std::string("b").begin()));
		yggr_test_assert(1 == yggr::charset::utf8_foo::charcmp(std::string("b").begin(), "a"));

		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp(str, str));
		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp(str, str2));

		yggr_test_assert(-1 == yggr::charset::utf8_foo::charcmp(str3, str));
		yggr_test_assert(1 == yggr::charset::utf8_foo::charcmp(str, str3));

		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp(std::string(str, str + 3), str));
		yggr_test_assert(0 == yggr::charset::utf8_foo::charcmp(str, std::string(str2, str2 + 3)));

		yggr_test_assert(-1 == yggr::charset::utf8_foo::charcmp(std::string(str3, str3 + 3), std::string(str, str + 3)));
		yggr_test_assert(1 == yggr::charset::utf8_foo::charcmp(boost::container::string(str, str + 3), std::string(str3, str3 + 3)));

#ifndef YGGR_NO_CWCHAR
		// test_style
		yggr::charset::utf8_foo::charcmp(L'a', L'a');

		yggr::charset::utf8_foo::charcmp(L'a', L"a");
		yggr::charset::utf8_foo::charcmp(L"a", L'a');
		yggr::charset::utf8_foo::charcmp(L"a", L"a");

		yggr::charset::utf8_foo::charcmp(std::wstring(L"a"), L"a");
		yggr::charset::utf8_foo::charcmp(L"a", yggr::wstring(L"a"));
		yggr::charset::utf8_foo::charcmp(std::wstring(L"a"), yggr::wstring(L"a"));

		yggr::charset::utf8_foo::charcmp(std::wstring(L"a").begin(), L"a");
		yggr::charset::utf8_foo::charcmp(L"a", yggr::wstring(L"a").begin());
		yggr::charset::utf8_foo::charcmp(std::wstring(L"a").begin(), yggr::wstring(L"a").begin());


#endif // YGGR_NO_CWCHAR
	}

	// strlen
	{
		char str[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), 0};
		
		yggr_test_assert(yggr::charset::utf8_foo::strlen(str) == 2);
		yggr_test_assert(yggr::charset::utf8_foo::strlen("abc") == 3);
		yggr_test_assert(yggr::charset::utf8_foo::strlen(str, sizeof(str)) == 3);

		yggr_test_assert(yggr::charset::utf8_foo::strlen(str, str + sizeof(str) - 1) == 2);
		yggr_test_assert(yggr::charset::utf8_foo::strlen("abc", 4) == 4);
		yggr_test_assert(yggr::charset::utf8_foo::strlen(str, str + sizeof(str)) == 3);

		yggr_test_assert(yggr::charset::utf8_foo::strlen(std::string(str, str + sizeof(str) - 1)) == 2);
		yggr_test_assert(yggr::charset::utf8_foo::strlen(std::string("abc")) == 3);
		yggr_test_assert(yggr::charset::utf8_foo::strlen(boost::container::string(str, str + sizeof(str))) == 3);

		{
			std::string t_str(str, sizeof(str));
			yggr_test_assert(yggr::charset::utf8_foo::strlen(t_str.begin(), t_str.end()) == 3);
		}

		{
			boost::container::string t_str(str, sizeof(str));
			yggr_test_assert(yggr::charset::utf8_foo::strlen(t_str.begin(), t_str.end()) == 3);
		}

		yggr_test_assert(yggr::charset::utf8_foo::strlen(str, str + sizeof(str) - 2) == 0);

#ifndef YGGR_NO_CWCHAR
		// test_style
		wchar_t wstr[] = {L'a', 0}; 
		yggr::charset::utf8_foo::strlen(L"a");

		yggr::charset::utf8_foo::strlen(wstr, wstr + 1);
		yggr::charset::utf8_foo::strlen(wstr, 1);

		yggr::charset::utf8_foo::strlen(std::wstring(L"a"));

		std::wstring stl_wstr = L"ab";
		yggr::charset::utf8_foo::strlen(stl_wstr.begin(), stl_wstr.end());

#endif // YGGR_NO_CWCHAR

	}

	// strnlen
	{
		char str[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), 0};
		
		yggr_test_assert(yggr::charset::utf8_foo::strnlen(str) == 2);
		yggr_test_assert(yggr::charset::utf8_foo::strnlen("abc") == 3);
		yggr_test_assert(yggr::charset::utf8_foo::strnlen(str, sizeof(str)) == 3);

		yggr_test_assert(yggr::charset::utf8_foo::strnlen(str, str + sizeof(str) - 1) == 2);
		yggr_test_assert(yggr::charset::utf8_foo::strnlen("abc", 4) == 4);
		yggr_test_assert(yggr::charset::utf8_foo::strnlen(str, str + sizeof(str)) == 3);

		yggr_test_assert(yggr::charset::utf8_foo::strnlen(std::string(str, str + sizeof(str) - 1)) == 2);
		yggr_test_assert(yggr::charset::utf8_foo::strnlen(std::string("abc")) == 3);
		yggr_test_assert(yggr::charset::utf8_foo::strnlen(boost::container::string(str, str + sizeof(str))) == 3);

		{
			std::string t_str(str, sizeof(str));
			yggr_test_assert(yggr::charset::utf8_foo::strnlen(t_str.begin(), t_str.end()) == 3);
		}

		{
			boost::container::string t_str(str, sizeof(str));
			yggr_test_assert(yggr::charset::utf8_foo::strnlen(t_str.begin(), t_str.end()) == 3);
		}

		yggr_test_assert(yggr::charset::utf8_foo::strnlen(str, str + sizeof(str) - 2) == 1);
	}

	// is_utf8_string
	{
		char str[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), 0};
		
		
		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string("") == true);
		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(std::string()) == true);
		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(boost::container::string()) == true);
		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(str, str) == false);


		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(str) == true);
		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string("abc") == true);
		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(str, sizeof(str)) == true);

		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(str, str + sizeof(str) - 1) == true);
		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string("abc", 4) == true);
		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(str, str + sizeof(str)) == true);

		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(std::string(str, str + sizeof(str) - 1)) == true);
		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(std::string("abc")) == true);
		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(boost::container::string(str, str + sizeof(str))) == true);

		{
			std::string t_str(str, sizeof(str));
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(t_str.begin(), t_str.end()) == true);
		}

		{
			boost::container::string t_str(str, sizeof(str));
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(t_str.begin(), t_str.end()) == true);
		}

		yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(str, str + sizeof(str) - 2) == false);

		{
			char str[] = "你好";
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string("你好") == false); 
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(std::string("你好")) == false); 
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(boost::container::string("你好")) == false); 
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(str, str + 2) == false); 
		}

#ifndef YGGR_NO_CWCHAR

		{
			wchar_t wstr[] = {L'a', L'b', 0};
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(L"") == true);
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(std::wstring()) == true);
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(boost::container::wstring()) == true);
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(wstr, wstr) == false);

			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(L"ab") == true); // result true but string is a\0b\0
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(std::wstring(L"ab")) == true); // result true but string is a\0b\0
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(boost::container::wstring(L"ab")) == true); // result true but string is a\0b\0
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(wstr, wstr + 2) == true); // result true but string is a\0b\0
		}

		{
			// the example is can passed is_utf8_string,
			// because "你好" == 0x4f60 0x597d
			wchar_t wstr[] = {L'��', L'��', 0};

			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(L"你好") == true); 
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(std::wstring(L"你好")) == true); 
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(boost::container::wstring(L"你好")) == true); 
			yggr_test_assert(yggr::charset::utf8_foo::is_utf8_string(wstr, wstr + 2) == true); 
		}


#endif // YGGR_NO_CWCHAR
	}

	// strcmp
	{
		char str[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), 0};
		char str2[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), 0};
		char str3[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa6), 0};

		yggr_test_assert(-1 == yggr::charset::utf8_foo::strcmp("aa", str));
		yggr_test_assert(1 == yggr::charset::utf8_foo::strcmp(str, "aa"));

		yggr_test_assert(0 == yggr::charset::utf8_foo::strcmp("aa", std::string("aa")));
		yggr_test_assert(0 == yggr::charset::utf8_foo::strcmp(std::string("aa"), "aa"));
		yggr_test_assert(0 == yggr::charset::utf8_foo::strcmp(std::string("aa"), std::string("aa")));

		yggr_test_assert(0 == yggr::charset::utf8_foo::strcmp(str, str));
		yggr_test_assert(0 == yggr::charset::utf8_foo::strcmp(str, str2));

		yggr_test_assert(-1 == yggr::charset::utf8_foo::strcmp(str, str3));
		yggr_test_assert(1 == yggr::charset::utf8_foo::strcmp(str3, str));

		yggr_test_assert(0 == yggr::charset::utf8_foo::strcmp(std::string(str), str));
		yggr_test_assert(0 == yggr::charset::utf8_foo::strcmp(str, std::string(str2)));

		yggr_test_assert(-1 == yggr::charset::utf8_foo::strcmp(boost::container::string(str), str3));
		yggr_test_assert(1 == yggr::charset::utf8_foo::strcmp(str3, boost::container::string(str)));

		yggr_test_assert(0 == yggr::charset::utf8_foo::strcmp(std::string(str), std::string(str)));
		yggr_test_assert(0 == yggr::charset::utf8_foo::strcmp(boost::container::string(str), std::string(str2)));

		yggr_test_assert(-1 == yggr::charset::utf8_foo::strcmp(boost::container::string(str), std::string(str3)));
		yggr_test_assert(1 == yggr::charset::utf8_foo::strcmp(boost::container::string(str3), boost::container::string(str)));
	}

	// strcmp_eq
	{
		char str[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), 0};
		char str2[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), 0};
		char str3[] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5), static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa6), 0};

		yggr_test_assert(false == yggr::charset::utf8_foo::strcmp_eq("aa", str));
		yggr_test_assert(false == yggr::charset::utf8_foo::strcmp_eq(str, "aa"));

		yggr_test_assert(true == yggr::charset::utf8_foo::strcmp_eq("aa", std::string("aa")));
		yggr_test_assert(true == yggr::charset::utf8_foo::strcmp_eq(std::string("aa"), "aa"));
		yggr_test_assert(true == yggr::charset::utf8_foo::strcmp_eq(std::string("aa"), std::string("aa")));

		yggr_test_assert(true == yggr::charset::utf8_foo::strcmp_eq(str, str));
		yggr_test_assert(true == yggr::charset::utf8_foo::strcmp_eq(str, str2));

		yggr_test_assert(false == yggr::charset::utf8_foo::strcmp_eq(str, str3));
		yggr_test_assert(false == yggr::charset::utf8_foo::strcmp_eq(str3, str));

		yggr_test_assert(true == yggr::charset::utf8_foo::strcmp_eq(std::string(str), str));
		yggr_test_assert(true == yggr::charset::utf8_foo::strcmp_eq(str, std::string(str2)));

		yggr_test_assert(false == yggr::charset::utf8_foo::strcmp_eq(boost::container::string(str), str3));
		yggr_test_assert(false == yggr::charset::utf8_foo::strcmp_eq(str3, boost::container::string(str)));

		yggr_test_assert(true == yggr::charset::utf8_foo::strcmp_eq(std::string(str), std::string(str)));
		yggr_test_assert(true == yggr::charset::utf8_foo::strcmp_eq(boost::container::string(str), std::string(str2)));

		yggr_test_assert(false == yggr::charset::utf8_foo::strcmp_eq(boost::container::string(str), std::string(str3)));
		yggr_test_assert(false == yggr::charset::utf8_foo::strcmp_eq(boost::container::string(str3), boost::container::string(str)));
	}

	// seach
	{
		std::string str = "";
		std::string cmp = "aba";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search(str, cmp);
		yggr_test_assert(nkmp_idx == -1);
	}

	{
		std::string str = "";
		std::string cmp = "";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search(str, cmp);
		yggr_test_assert(nkmp_idx == 0);
	}

	{
		std::string str = "kabaabak";
		std::string cmp = "aba";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search(str, cmp);
		yggr_test_assert(nkmp_idx == 1);
	}

	{
		std::string str = "";
		std::string cmp = "aba";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search(str.c_str(), -1, cmp.c_str(), -1);
		yggr_test_assert(nkmp_idx == -1);
	}

	{
		std::string str = "";
		std::string cmp = "";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search(str.c_str(), -1, cmp.c_str(), -1);
		yggr_test_assert(nkmp_idx == 0);
	}

	{
		std::string str = "kabaabak";
		std::string cmp = "aba";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search(str.c_str(), -1, cmp.c_str(), -1);
		yggr_test_assert(nkmp_idx == 1);
	}

	//search_n
	{
		std::string str = "";
		std::string cmp = "a";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search_n(str, 2, cmp);

		yggr_test_assert(nkmp_idx == -1);
	}

	{
		std::string str = "";
		std::string cmp = "";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search_n(str, 0, cmp);

		yggr_test_assert(nkmp_idx == 0);
	}

	{
		std::string str = "kabaabak";
		std::string cmp = "a";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search_n(str, 2, cmp);

		yggr_test_assert(nkmp_idx == 3);
	}

	{
		std::string str = "";
		std::string cmp = "a";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search_n(str.c_str(), -1, 2, cmp);

		yggr_test_assert(nkmp_idx == -1);
	}

	{
		std::string str = "";
		std::string cmp = "";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search_n(str.c_str(), -1, 0, cmp);

		yggr_test_assert(nkmp_idx == 0);
	}

	{
		std::string str = "kabaabak";
		std::string cmp = "a";

		yggr::size_type nkmp_idx = yggr::charset::utf8_foo::search_n(str.c_str(), -1, 2, cmp);

		yggr_test_assert(nkmp_idx == 3);
	}

	std::cout << "---------test_utf8_foo end------------" << std::endl;
}


void test_char_to_number(void)
{
	{
		char arr[4] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa5)};
		char arr2[4] = {0};
		char arr3[4] = {static_cast<char>(0xe4), static_cast<char>(0xb8), static_cast<char>(0xa6)};
		yggr::s32 num = yggr::charset::utf8_foo::char_to_number(arr);
		yggr_test_assert(num == 0x00004e25);

		std::size_t n = 0;
		char* ptr = yggr::charset::utf8_foo::number_to_char(arr2, num, 3, n);
		yggr_test_assert(n == 3);
		yggr_test_assert(ptr == arr2);
		
		ptr = yggr::charset::utf8_foo::number_to_char(arr2, num, 3);
		yggr_test_assert(ptr == arr2);
		yggr_test_assert(0 == memcmp(arr, ptr, 3));

		ptr = yggr::charset::utf8_foo::number_to_char(arr2, num + 1, 3);
		yggr_test_assert(ptr == arr2);
		yggr_test_assert(0 == memcmp(arr3, ptr, 3));

		int nnn = 0;
	}

	std::cout << "---------test_char_to_number end------------" << std::endl;
}

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_default_charset_config();
	test_charset_base_foo();
	test_charset_converter();
	test_charset_foo();

	test_str_wstr_converter_test();

	test_utf8_foo();

	test_char_to_number();

	wait_any_key(argc, argv);
	return 0;
}
