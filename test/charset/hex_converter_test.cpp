//hex_converter_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <memory>

#include <yggr/container/vector.hpp>
#include <yggr/container/array.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/set.hpp>

#include <yggr/charset/hex_converter.hpp>
#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_basic_hex_converter(void)
{
	{
		yggr::u8 hex[3] = {static_cast<yggr::u8>(-1), static_cast<yggr::u8>(-1), static_cast<yggr::u8>(-1)};
		char hex_str[7] = {0};
		yggr::u8 hex2[3] = {0};

		std::cout << yggr::charset::hex_converter::s_hex_to_string(hex_str, hex, 3) << std::endl;

		yggr::charset::hex_converter::s_string_to_hex(hex2, hex_str, 6);

		yggr_test_assert(0 == memcmp(hex, hex2, 3));
	}

	{
		char hex_str[41] = "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::u8 hex_chk[20] = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
									0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };
		yggr::u8 hex[20] = {0};
		char hex_str2[41] = {0};

		yggr::charset::hex_converter::s_string_to_hex(hex, hex_str, 40);

		yggr_test_assert(0 == memcmp(hex, hex_chk, 20));

		yggr::charset::hex_converter::s_hex_to_string(hex_str2, hex, 20);

		yggr_test_assert(0 == memcmp(hex_str, hex_str2, 40));

		std::cout << hex_str2 << std::endl;

		yggr::charset::hex_converter::s_hex_to_string(hex_str2, hex, 20, 1);

		yggr_test_assert(0 != memcmp(hex_str, hex_str2, 40));
		yggr_test_assert(0 == yggr::charset::charset_base_foo::strncmp_case(hex_str, hex_str2, 40));

		std::cout << hex_str2 << std::endl;
	}

#ifndef YGGR_NO_CWCHAR

	{
		yggr::u8 hex[3] = {static_cast<yggr::u8>(-1), static_cast<yggr::u8>(-1), static_cast<yggr::u8>(-1)};
		wchar_t hex_str[7] = {0};
		yggr::u8 hex2[3] = {0};

		std::wcout << yggr::charset::hex_converter::s_hex_to_string(hex_str, hex, 3) << std::endl;

		yggr::charset::hex_converter::s_string_to_hex(hex2, hex_str, 6);

		yggr_test_assert(0 == memcmp(hex, hex2, 3));
	}

	{
		wchar_t hex_str[41] = L"de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::u8 hex_chk[20] = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
									0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };
		yggr::u8 hex[20] = {0};
		wchar_t hex_str2[41] = {0};

		yggr::charset::hex_converter::s_string_to_hex(hex, hex_str, 40);

		yggr_test_assert(0 == memcmp(hex, hex_chk, 20));

		yggr::charset::hex_converter::s_hex_to_string(hex_str2, hex, 20);

		yggr_test_assert(0 == memcmp(hex_str, hex_str2, 40));

#if !(YGGR_CPP_VERSION < YGGR_CPP_VER_20) //std::cout << wstring is deleted
		std::wcout << hex_str2 << std::endl;
#else
		std::cout << hex_str2 << std::endl;
#endif // #if !(YGGR_CPP_VERSION < YGGR_CPP_VER_20)

		yggr::charset::hex_converter::s_hex_to_string(hex_str2, hex, 20, 1);

		yggr_test_assert(0 != memcmp(hex_str, hex_str2, 40));
		yggr_test_assert(0 == yggr::charset::charset_base_foo::strncmp_case(hex_str, hex_str2, 40));

#if !(YGGR_CPP_VERSION < YGGR_CPP_VER_20) //std::cout << wstring is deleted
		std::wcout << hex_str2 << std::endl;
#else
		std::cout << hex_str2 << std::endl;
#endif // #if !(YGGR_CPP_VERSION < YGGR_CPP_VER_20)
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "----------test_basic_hex_converter succeed-------------" << std::endl;
}

void test_hex_converter(void)
{
	// s_char_to_hex
	{
	    yggr::utf8_char ch_a('a');
	    yggr::utf8_char ch_0(0);

		yggr_test_assert(0x0a == yggr::charset::hex_converter::s_char_to_hex('a'));
		yggr_test_assert(0xff == yggr::charset::hex_converter::s_char_to_hex('k'));

		yggr_test_assert(0x0a == yggr::charset::hex_converter::s_char_to_hex(yggr::utf8_char('a')));
		yggr_test_assert(0xff == yggr::charset::hex_converter::s_char_to_hex(yggr::utf8_char(0)));

		yggr_test_assert(0x0a == yggr::charset::hex_converter::s_char_to_hex(static_cast<yggr::utf8_char::char_reference_type>(ch_a)));
		yggr_test_assert(0xff == yggr::charset::hex_converter::s_char_to_hex(static_cast<yggr::utf8_char::char_reference_type>(ch_0)));

		yggr_test_assert(0x0a == yggr::charset::hex_converter::s_char_to_hex(static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char('a'))));
		yggr_test_assert(0xff == yggr::charset::hex_converter::s_char_to_hex(static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(0))));

		yggr_test_assert(0x0a == yggr::charset::hex_converter::s_char_to_hex(yggr::utf8_char('a').view()));
		yggr_test_assert(0xff == yggr::charset::hex_converter::s_char_to_hex(yggr::utf8_char(0).view()));
	}

	// s_hex_to_char
	{
		{
			char str_chk[] = {'0', 'a'};
			char str[2] = {0};
			yggr::charset::hex_converter::s_hex_to_char(str, 0x0a);
			yggr_test_assert(0 == memcmp(str, str_chk, 2));
		}

		{
			char str_chk[] = {'f', 'f'};
			char str[2] = {0};
			yggr::charset::hex_converter::s_hex_to_char(str, -1);
			yggr_test_assert(0 == memcmp(str, str_chk, 2));
		}
	}

	// test_string
	// char, n
	{
		char hex_str[41] = "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::u8 hex_chk[20] = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
									0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };
		//char hex[20] = {0};
		std::string hex;
		std::string hex_str2;

		bool b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str);
		yggr_test_assert(b);
		yggr_test_assert(0 == memcmp(&hex[0], hex_chk, 20));

		b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str, 40);
		yggr_test_assert(b);
		yggr_test_assert(0 == memcmp(&hex[0], hex_chk, 20));

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<std::string>(hex);
		yggr_test_assert(0 == memcmp(hex_str, &hex_str2[0], 40));

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<std::string>(hex, hex_str2.get_allocator());
		yggr_test_assert(0 == memcmp(hex_str, &hex_str2[0], 40));

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<std::string>(hex, 1);
		yggr_test_assert(0 == yggr::charset::charset_base_foo::strncmp_case(hex_str, &hex_str2[0], 40));

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<std::string>(hex, hex_str2.get_allocator(), 1);
		yggr_test_assert(0 == yggr::charset::charset_base_foo::strncmp_case(hex_str, &hex_str2[0], 40));

		std::cout << hex_str2 << std::endl;
	}

	// basic_string
	{
		std::string hex_str = "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::string hex_str_upper_chk = "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
		yggr::array<yggr::u8, 20> hex_chk = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
												0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };

		yggr::array<yggr::u8, 20> hex;
		yggr::utf8_string hex_str2;

		BOOST_MPL_ASSERT((yggr::charset::is_basic_string_t<std::string>));

		bool b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str);
		yggr_test_assert(b);
		yggr_test_assert(0 == memcmp(&hex[0], &hex_chk[0], 20));

		yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex_str2, hex);
		yggr_test_assert(hex_str == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex);
		yggr_test_assert(hex_str == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex, 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex, hex_str2.get_allocator());
		yggr_test_assert(hex_str == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex, hex_str2.get_allocator(), 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::cout << hex_str2 << std::endl;

		yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex_str2, hex.begin(), hex.end());
		yggr_test_assert(hex_str == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex.begin(), hex.end());
		yggr_test_assert(hex_str == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex.begin(), hex.end(), 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex.begin(), hex.end(), hex_str2.get_allocator());
		yggr_test_assert(hex_str == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex.begin(), hex.end(), hex_str2.get_allocator(), 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::cout << hex_str2 << std::endl;
	}

	{
		yggr::utf8_string hex_str = "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::utf8_string hex_str_upper_chk = "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
		yggr::array<yggr::u8, 20> hex_chk = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
												0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };

		std::vector<yggr::u8> hex;
		yggr::utf8_string hex_str2;

		bool b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str);
		yggr_test_assert(b);
		yggr_test_assert(0 == memcmp(&hex[0], &hex_chk[0], 20));

		yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex_str2, hex);
		yggr_test_assert(hex_str == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex, 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::cout << hex_str2 << std::endl;
	}

	{
		yggr::utf8_string hex_str = "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::utf8_string hex_str_upper_chk = "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
		yggr::array<yggr::u8, 20> hex_chk = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
												0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };

		std::vector<yggr::u32> hex;
		yggr::utf8_string hex_str2;

		bool b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str);
		yggr_test_assert(b);
		yggr_test_assert(0 == memcmp(&hex[0], &hex_chk[0], 20));

		yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex_str2, hex);
		yggr_test_assert(hex_str == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex, 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::cout << hex_str2 << std::endl;
	}

	{
		yggr::utf8_string hex_str = "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::utf8_string hex_str_upper_chk = "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
		yggr::array<yggr::u8, 20> hex_chk = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
												0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };

		std::list<yggr::u8> hex;
		yggr::utf8_string hex_str2;

		bool b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str);
		std::vector<yggr::u8> tmp_hex(hex.begin(), hex.end());
		yggr_test_assert(b);
		yggr_test_assert(0 == memcmp(&tmp_hex[0], &hex_chk[0], 20));

		yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex_str2, hex);
		yggr_test_assert(hex_str == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex, 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::cout << hex_str2 << std::endl;
	}

	{
		yggr::utf8_string hex_str = "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::utf8_string hex_str_upper_chk = "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
		yggr::array<yggr::u8, 20> hex_chk = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
												0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };

		std::list<yggr::u32> hex;
		yggr::utf8_string hex_str2;

		bool b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str);
		std::vector<yggr::u32> tmp_hex(hex.begin(), hex.end());
		yggr_test_assert(b);
		yggr_test_assert(0 == memcmp(&tmp_hex[0], &hex_chk[0], 20));

		yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex_str2, hex);
		yggr_test_assert(hex_str == hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex, 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::cout << hex_str2 << std::endl;
	}

	{
		yggr::utf8_string hex_str = "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::utf8_string hex_str_upper = "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
		yggr::array<yggr::u8, 20> thex_chk = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
												0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };

		std::set<yggr::u8> hex_chk(thex_chk.begin(), thex_chk.end());

		std::set<yggr::u8> hex;
		yggr::utf8_string hex_str2;

		bool b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str);
		yggr_test_assert(b);
		yggr_test_assert(hex_chk == hex);

		yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex_str2, hex);
		yggr_test_assert(hex_str != hex_str2);

		std::cout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::utf8_string>(hex, 1);
		yggr_test_assert(hex_str_upper != hex_str2);

		std::cout << hex_str2 << std::endl;
	}

	std::cout << "---------------------" << std::endl;

#ifndef YGGR_NO_CWCHAR
	// wchar, n
	{
		wchar_t hex_str[41] = L"de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::u8 hex_chk[20] = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
									0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };
		//char hex[20] = {0};
		std::wstring hex;
		std::wstring hex_str2(40, 0);

		bool b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str);
		yggr_test_assert(b);
		yggr_test_assert(0 == memcmp(&hex[0], hex_chk, 20));

		b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str, 40);
		yggr_test_assert(b);
		yggr_test_assert(0 == memcmp(&hex[0], hex_chk, 20));

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<std::wstring>(hex);
		yggr_test_assert(0 == memcmp(hex_str, &hex_str2[0], 40));

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<std::wstring>(hex, hex_str2.get_allocator());
		yggr_test_assert(0 == memcmp(hex_str, &hex_str2[0], 40));

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<std::wstring>(hex, 1);
		yggr_test_assert(0 == yggr::charset::charset_base_foo::strncmp_case(hex_str, &hex_str2[0], 40));

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<std::wstring>(hex, hex_str2.get_allocator(), 1);
		yggr_test_assert(0 == yggr::charset::charset_base_foo::strncmp_case(hex_str, &hex_str2[0], 40));

		std::wcout << hex_str2 << std::endl;
	}

	// basic_string
	{
		yggr::wstring hex_str = L"de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9";
		yggr::wstring hex_str_upper_chk = L"DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
		yggr::array<yggr::u8, 20> hex_chk = { 0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a,
												0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9 };

		yggr::array<yggr::u8, 20> hex;
		yggr::wstring hex_str2;

		BOOST_MPL_ASSERT((yggr::charset::is_basic_string_t<yggr::wstring>));

		bool b = yggr::charset::hex_converter::s_string_to_hex(hex, hex_str);
		yggr_test_assert(b);
		yggr_test_assert(0 == memcmp(&hex[0], &hex_chk[0], 20));


		yggr::charset::hex_converter::s_hex_to_string<yggr::wstring>(hex_str2, hex);
		yggr_test_assert(hex_str == hex_str2);

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::wstring>(hex);
		yggr_test_assert(hex_str == hex_str2);

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::wstring>(hex, 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::wstring>(hex, hex_str.get_allocator());
		yggr_test_assert(hex_str == hex_str2);

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::wstring>(hex, hex_str.get_allocator(), 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::wcout << hex_str2 << std::endl;

		yggr::charset::hex_converter::s_hex_to_string<yggr::wstring>(hex_str2, hex.begin(), hex.end());
		yggr_test_assert(hex_str == hex_str2);

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::wstring>(hex.begin(), hex.end());
		yggr_test_assert(hex_str == hex_str2);

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::wstring>(hex.begin(), hex.end(), 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::wstring>(hex.begin(), hex.end(), hex_str2.get_allocator());
		yggr_test_assert(hex_str == hex_str2);

		std::wcout << hex_str2 << std::endl;

		hex_str2 = yggr::charset::hex_converter::s_hex_to_string<yggr::wstring>(hex.begin(), hex.end(), hex_str2.get_allocator(), 1);
		yggr_test_assert(hex_str_upper_chk == hex_str2);

		std::wcout << hex_str2 << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "----------test_hex_converter succeed-------------" << std::endl;
}

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_basic_hex_converter();
	test_hex_converter();

	wait_any_key(argc, argv);
	return 0;
}
