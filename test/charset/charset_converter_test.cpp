// charset_converter_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>

#include <yggr/charset/default_charset_config.hpp>
#include <yggr/charset/charset_converter.hpp>
#include <yggr/network/hn_conv.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)

void test_utf8_ascii(void)
{
	typedef yggr::charset::charset_converter conv_type;

	conv_type utf8_to_ascii_conv(YGGR_STR_UTF8_STRING_CHARSET_NAME(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char));
	conv_type ascii_to_utf8_conv(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char), YGGR_STR_UTF8_STRING_CHARSET_NAME());

	{
		char str_arr[] = {'a', 'b', 'c'};
		std::string str(str_arr, sizeof(str_arr));
		std::string out_str;
		utf8_to_ascii_conv(str, out_str);

		yggr_test_assert(out_str.size() == 3);
		yggr_test_assert(out_str == "abc");

		std::string utf8_str;
		ascii_to_utf8_conv(out_str, utf8_str);

		yggr_test_assert(utf8_str.size() == 3);
		yggr_test_assert(0 == memcmp(&utf8_str[0], str_arr, utf8_str.size()));
	}

	{
		char str_arr[] = {'a', 'b', 'c', 0};
		std::string str(str_arr, sizeof(str_arr));
		std::string out_str;
		utf8_to_ascii_conv(str, out_str);

		yggr_test_assert(out_str.size() == 4);
		out_str.erase(out_str.end() - 1);
		yggr_test_assert(out_str == "abc");
		out_str.push_back(0);

		std::string utf8_str;
		ascii_to_utf8_conv(out_str, utf8_str);

		yggr_test_assert(utf8_str.size() == 4);
		yggr_test_assert(0 == memcmp(&utf8_str[0], str_arr, utf8_str.size()));
	}
}

void test_utf8_ascii_of_char_arr(void)
{
	typedef yggr::charset::charset_converter conv_type;

	conv_type utf8_to_ascii_conv(YGGR_STR_UTF8_STRING_CHARSET_NAME(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char));
	conv_type ascii_to_utf8_conv(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char), YGGR_STR_UTF8_STRING_CHARSET_NAME());

	{
		char str_arr[] = {'a', 'b', 'c'};
		std::string str(str_arr, sizeof(str_arr));
		std::string out_str;
		utf8_to_ascii_conv(str.c_str(), 3, out_str);

		yggr_test_assert(out_str.size() == 3);
		yggr_test_assert(out_str == "abc");

		std::string utf8_str;
		ascii_to_utf8_conv(out_str.c_str(), 3, utf8_str);

		yggr_test_assert(utf8_str.size() == 3);
		yggr_test_assert(0 == memcmp(&utf8_str[0], str_arr, utf8_str.size()));
	}

	{
		char str_arr[] = {'a', 'b', 'c', 0};
		std::string str(str_arr, sizeof(str_arr));
		std::string out_str;
		utf8_to_ascii_conv(str.c_str(), out_str);

		yggr_test_assert(out_str.size() == 3);
		yggr_test_assert(out_str == "abc");

		std::string utf8_str;
		ascii_to_utf8_conv(out_str.c_str(), utf8_str);

		yggr_test_assert(utf8_str.size() == 3);
		yggr_test_assert(0 == memcmp(&utf8_str[0], str_arr, utf8_str.size()));
	}
}


void test_utf8_ucs(void)
{
#ifndef YGGR_NO_WSTRING
	typedef yggr::charset::charset_converter conv_type;

	conv_type utf8_to_ucs_conv(YGGR_STR_UTF8_STRING_CHARSET_NAME(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t));
	conv_type ucs_to_utf8_conv(YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t), YGGR_STR_UTF8_STRING_CHARSET_NAME());

	{
		char str_arr[] = {'a', 'b', 'c'};
		std::string str(str_arr, sizeof(str_arr));
		std::wstring out_str;
		utf8_to_ucs_conv(str, out_str);

		yggr_test_assert(out_str.size() == 3);
		yggr_test_assert(out_str == L"abc");

		std::string utf8_str;
		ucs_to_utf8_conv(out_str, utf8_str);

		yggr_test_assert(utf8_str.size() == 3);
		yggr_test_assert(0 == memcmp(&utf8_str[0], str_arr, utf8_str.size()));
	}

	{
		char str_arr[] = {'a', 'b', 'c', 0};
		std::string str(str_arr, sizeof(str_arr));
		std::wstring out_str;
		utf8_to_ucs_conv(str, out_str);

		yggr_test_assert(out_str.size() == 4);
		out_str.erase(out_str.end() - 1);
		yggr_test_assert(out_str == L"abc");
		out_str.push_back(0);

		std::string utf8_str;
		ucs_to_utf8_conv(out_str, utf8_str);

		yggr_test_assert(utf8_str.size() == 4);
		yggr_test_assert(0 == memcmp(&utf8_str[0], str_arr, utf8_str.size()));
	}
#endif //YGGR_NO_WSTRING
}


void test_utf8_utf32(void)
{
	typedef std::vector<yggr::u32> utf32_string_type;
	typedef yggr::charset::charset_converter conv_type;

	conv_type utf8_to_utf32_conv(YGGR_STR_UTF8_STRING_CHARSET_NAME(), "utf-32");
	conv_type utf32_to_utf8_conv("utf-32", YGGR_STR_UTF8_STRING_CHARSET_NAME());

	{
		char str_arr[] = {'a', 'b', 'c'};
		std::string str(str_arr, sizeof(str_arr));
		utf32_string_type out_str;
		utf8_to_utf32_conv(str, out_str);

		// first 32bit U+0000FEFF or U+0000FFFE 
		if(out_str[0] == htonl(0x0000fffe) || out_str[0] == htonl(0x0000feff))
		{
			yggr_test_assert(out_str.size() == sizeof(str_arr) + 1); 
		}
		else
		{
			yggr_test_assert(out_str.size() == sizeof(str_arr)); 
		}

		std::string utf8_str;
		utf32_to_utf8_conv(out_str, utf8_str);

		yggr_test_assert(utf8_str.size() == 3);
		yggr_test_assert(0 == memcmp(&utf8_str[0], str_arr, utf8_str.size()));
	}

	{
		char str_arr[] = {'a', 'b', 'c', 0};
		std::string str(str_arr, sizeof(str_arr));
		utf32_string_type out_str;
		utf8_to_utf32_conv(str, out_str);

		// first 32bit U+0000FEFF or U+0000FFFE 
		if(out_str[0] == htonl(0x0000fffe) || out_str[0] == htonl(0x0000feff))
		{
			yggr_test_assert(out_str.size() == sizeof(str_arr) + 1); 
		}
		else
		{
			yggr_test_assert(out_str.size() == sizeof(str_arr)); 
		}

		std::string utf8_str;
		utf32_to_utf8_conv(out_str, utf8_str);

		yggr_test_assert(utf8_str.size() == 4);
		yggr_test_assert(0 == memcmp(&utf8_str[0], str_arr, utf8_str.size()));
	}
	
}

void test_warning(void)
{
#ifndef YGGR_NO_WSTRING
	typedef yggr::charset::charset_converter conv_type;

	conv_type conv(YGGR_STR_UTF8_STRING_CHARSET_NAME(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t));

	char str_arr1[] = { 'a', 'b', 'c' };
	char str_arr2[] = { 'a', 'b', 'c', 0 };

	std::wstring wstr1(sizeof(str_arr2) / sizeof(wchar_t), 0);
	memcpy(&wstr1[0], str_arr1, sizeof(str_arr1));
	std::wstring wstr2(sizeof(str_arr2) / sizeof(wchar_t), 0);
	memcpy(&wstr2[0], str_arr2, sizeof(str_arr2));

	std::wstring out_wstr1;
	std::wstring out_wstr2;

	conv(wstr1, out_wstr1);
	conv(wstr2, out_wstr2);

	yggr_test_assert(out_wstr1 == out_wstr2);
	yggr_test_assert(out_wstr1.size() == out_wstr2.size());

	yggr_test_assert(out_wstr1[out_wstr1.size() - 1] == 0);
	yggr_test_assert(out_wstr2[out_wstr2.size() - 1] == 0);
#endif // YGGR_NO_WSTRING
}


int main(int argc, char* argv[])
{
	test_utf8_ascii();
	test_utf8_ascii_of_char_arr();
	test_utf8_ucs();
	test_utf8_utf32();

	test_warning();

	wait_any_key(argc, argv);
	return 0;
}