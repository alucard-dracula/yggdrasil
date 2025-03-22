// escape_codes_converter_test.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/network/hn_conv.hpp>
#include <yggr/charset/escape_codes_converter.hpp>

#include <yggr/bytes/byte_size.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_escape_codes_basic_converter(void)
{
	typedef yggr::charset::escape_codes_basic_converter conv_type;

	{
		char escape_str[3 + 1] = {0};
		//yggr::u8 ch = ' '; // %20 $20
		yggr::u8 ch = '<'; // %3C $3C

		//yggr_test_assert(ch == 0x20);
		yggr_test_assert(ch == 0x3C);

		std::cout << conv_type::s_escape_byte(escape_str, ch, conv_type::SEPUrl<char>()) << std::endl;
		std::cout << conv_type::s_escape_byte(escape_str, ch, conv_type::SEPStringLiteral<char>()) << std::endl;

		yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str);
		yggr_test_assert(ch == ch2);
	}

	{
		char escape_str[6 + 1] = {0};
		char data[2] = { ' ', '<' };

		std::cout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), 2, conv_type::SEPUrl<char>()) << std::endl;
		yggr_test_assert(yggr::string(escape_str) == "%20%3c");

		std::cout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), 2, conv_type::SEPStringLiteral<char>()) << std::endl;
		yggr_test_assert(yggr::string(escape_str) == "$20$3c");


		char data2[2] = {0};
		std::size_t unescape_size = conv_type::s_unescape_bytes(reinterpret_cast<yggr::u8*>(data2), escape_str, 6, conv_type::SEPStringLiteral<char>());
		yggr_test_assert(unescape_size == 2);

		yggr_test_assert(0 == memcmp(data, data2, 2));
	}

#if !defined(YGGR_NO_CWCHAR)
	{
		wchar_t escape_wstr[3 + 1] = {0};
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		

		std::wcout << conv_type::s_escape_byte(escape_wstr, ch, conv_type::SEPUrl<wchar_t>()) << std::endl;
		std::wcout << conv_type::s_escape_byte(escape_wstr, ch, conv_type::SEPStringLiteral<wchar_t>()) << std::endl;

		yggr::u8 ch2 = conv_type::s_unescape_byte(escape_wstr);
		yggr_test_assert(ch == ch2);
	}

	{
		wchar_t escape_str[6 + 1] = {0};
		char data[2] = { ' ', '<' };

		

		std::wcout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), 2, conv_type::SEPUrl<wchar_t>()) << std::endl;
		yggr_test_assert(yggr::wstring(escape_str) == L"%20%3c");

		std::wcout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), 2, conv_type::SEPStringLiteral<wchar_t>()) << std::endl;
		yggr_test_assert(yggr::wstring(escape_str) == L"$20$3c");

	}

	{
		wchar_t escape_str[6 + 1] = {0};
		char data[2] = { ' ', '<' };

		

		std::wcout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), 2, conv_type::SEPUrl<wchar_t>()) << std::endl;
		yggr_test_assert(yggr::wstring(escape_str) == L"%20%3c");

		std::wcout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), 2, conv_type::SEPStringLiteral<wchar_t>()) << std::endl;
		yggr_test_assert(yggr::wstring(escape_str) == L"$20$3c");


		char data2[2] = {0};
		std::size_t unescape_size = conv_type::s_unescape_bytes(reinterpret_cast<yggr::u8*>(data2), escape_str, 6, conv_type::SEPStringLiteral<wchar_t>());
		yggr_test_assert(unescape_size == 2);

		yggr_test_assert(0 == memcmp(data, data2, 2));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "--------------test_escape_codes_basic_converter end-----------------" << std::endl;
}

void test_escape_codes_converter(void)
{
	typedef yggr::charset::escape_codes_converter conv_type;

	{
		char escape_str[3 + 1] = {0};
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		std::cout << conv_type::s_escape_byte(escape_str, ch, conv_type::SEPUrl<char>()) << std::endl;
		std::cout << conv_type::s_escape_byte(escape_str, ch, conv_type::SEPStringLiteral<char>()) << std::endl;

		yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str);
		yggr_test_assert(ch == ch2);
	}

	{
		yggr::string escape_str;
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		std::cout << conv_type::s_escape_byte(escape_str, ch, conv_type::SEPUrl<char>()) << std::endl;
		std::cout << conv_type::s_escape_byte(escape_str, ch, conv_type::SEPStringLiteral<char>()) << std::endl;

		yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str.data());
		yggr_test_assert(ch == ch2);
	}

	{
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		{
			std::string escape_str = conv_type::s_escape_byte<std::string>(ch, conv_type::SEPUrl<char>());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str);
			yggr_test_assert(ch == ch2);
		}

		{
			std::string escape_str = conv_type::s_escape_byte<std::string>(ch, conv_type::SEPUrl<char>(), std::string().get_allocator());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str);
			yggr_test_assert(ch == ch2);
		}
	}

	{
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		{
			boost::container::string escape_str = conv_type::s_escape_byte<boost::container::string>(ch, conv_type::SEPUrl<char>());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str);
			yggr_test_assert(ch == ch2);
		}

		{
			boost::container::string escape_str = conv_type::s_escape_byte<boost::container::string>(ch, conv_type::SEPUrl<char>(), boost::container::string().get_allocator());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str);
			yggr_test_assert(ch == ch2);
		}
	}

	{
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		{
			yggr::string escape_str = conv_type::s_escape_byte<yggr::string>(ch, conv_type::SEPUrl<char>());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str);
			yggr_test_assert(ch == ch2);
		}

		{
			yggr::string escape_str = conv_type::s_escape_byte<yggr::string>(ch, conv_type::SEPUrl<char>(), yggr::string().get_allocator());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str);
			yggr_test_assert(ch == ch2);
		}
	}

#if !(BOOST_VERSION < 106400)
	{
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);


		{
			yggr::string escape_str = conv_type::s_escape_byte<yggr::string>(ch, conv_type::SEPUrl<char>());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(yggr::string_view(escape_str));
			yggr_test_assert(ch == ch2);
		}


		{
			yggr::string escape_str = conv_type::s_escape_byte<yggr::string>(ch, conv_type::SEPUrl<char>(), yggr::string().get_allocator());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(yggr::string_view(escape_str));
			yggr_test_assert(ch == ch2);
		}
	}

#endif // #if !(BOOST_VERSION < 106400)

	{
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		{
			yggr::utf8_string escape_str = conv_type::s_escape_byte<yggr::utf8_string>(ch, conv_type::SEPUrl<char>());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str.data());
			yggr_test_assert(ch == ch2);
		}

		{
			yggr::utf8_string escape_str = conv_type::s_escape_byte<yggr::utf8_string>(ch, conv_type::SEPUrl<char>(), yggr::utf8_string().get_allocator());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str.data());
			yggr_test_assert(ch == ch2);
		}
	}

	{
		char escape_str[6 + 1] = {0};
		char data[2] = { ' ', '<' };

		std::cout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), 2, conv_type::SEPUrl<char>()) << std::endl;
		yggr_test_assert(yggr::string(escape_str) == "%20%3c");

		std::cout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), 2, conv_type::SEPStringLiteral<char>()) << std::endl;
		yggr_test_assert(yggr::string(escape_str) == "$20$3c");

		{
			char data2[2] = {0};
			std::size_t unescape_size = conv_type::s_unescape_bytes(reinterpret_cast<yggr::u8*>(data2), escape_str, 6, conv_type::SEPStringLiteral<char>());
			yggr_test_assert(unescape_size == 2);
			yggr_test_assert(0 == memcmp(data, data2, 2));
		}

		{
			yggr::string data2;
			std::size_t unescape_size = conv_type::s_unescape_bytes(data2, escape_str, 6, conv_type::SEPStringLiteral<char>());
			yggr_test_assert(unescape_size == 2);
			yggr_test_assert(data2 == " <");
		}

		{
			yggr::string data2;
			std::size_t unescape_size = conv_type::s_unescape_bytes(data2, escape_str, conv_type::SEPStringLiteral<char>());
			yggr_test_assert(unescape_size == 2);
			yggr_test_assert(data2 == " <");
		}
	}

	{
		char data[2] = { ' ', '<' };
		yggr::string hex_buf(data, data + 2);

		{
			yggr::string escape_str;
			std::cout << conv_type::s_escape_bytes(escape_str, hex_buf, conv_type::SEPUrl<char>()) << std::endl;
			yggr_test_assert(escape_str == "%20%3c");
		}

		{
			yggr::string escape_str;
			std::cout << conv_type::s_escape_bytes(escape_str, hex_buf, conv_type::SEPStringLiteral<char>()) << std::endl;
			yggr_test_assert(escape_str == "$20$3c");
		}

		{
			yggr::string escape_str = conv_type::s_escape_bytes<yggr::string>(hex_buf, conv_type::SEPUrl<char>());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%20%3c");
		}

		{
			yggr::string escape_str = conv_type::s_escape_bytes<yggr::string>(hex_buf, conv_type::SEPStringLiteral<char>());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "$20$3c");
		}

		{
			yggr::string escape_str =
				conv_type::s_escape_bytes<yggr::string>(hex_buf, conv_type::SEPUrl<char>(), yggr::string().get_allocator());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%20%3c");
		}

		{
			yggr::string escape_str =
				conv_type::s_escape_bytes<yggr::string>(hex_buf, conv_type::SEPStringLiteral<char>(), yggr::string().get_allocator());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "$20$3c");
		}
	}

	{
		char data[2] = { ' ', '<' };
		yggr::string hex_buf(data, data + 2);

		{
			yggr::string escape_str = conv_type::s_escape_bytes<yggr::string>(hex_buf, conv_type::SEPUrl<char>());
			std::cout << escape_str << std::endl;
			yggr_test_assert(escape_str == "%20%3c");

			escape_str = "aaa" + escape_str + "ccc";
			yggr_test_assert(escape_str == "aaa%20%3cccc");

			yggr::string hex_buf2;
			std::size_t conv_size = conv_type::s_unescape_bytes(hex_buf2, escape_str, conv_type::SEPUrl<char>());
			yggr_test_assert(conv_size == 8);
			yggr_test_assert(hex_buf2 == "aaa <ccc");

		}
	}

#if !defined(YGGR_NO_CWCHAR)
	{
		wchar_t escape_wstr[3 + 1] = {0};
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		

		std::wcout << conv_type::s_escape_byte(escape_wstr, ch, conv_type::SEPUrl<wchar_t>()) << std::endl;
		std::wcout << conv_type::s_escape_byte(escape_wstr, ch, conv_type::SEPStringLiteral<wchar_t>()) << std::endl;

		yggr::u8 ch2 = conv_type::s_unescape_byte(escape_wstr);
		yggr_test_assert(ch == ch2);
	}

	{
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		{
			std::wstring escape_str = conv_type::s_escape_byte<std::wstring>(ch, conv_type::SEPUrl<wchar_t>());
			
			std::wcout << escape_str << std::endl;
			yggr_test_assert(escape_str == L"%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str.data());
			yggr_test_assert(ch == ch2);
		}

		{
			std::wstring escape_str = conv_type::s_escape_byte<std::wstring>(ch, conv_type::SEPUrl<wchar_t>(), std::wstring().get_allocator());
			
			std::wcout << escape_str << std::endl;
			yggr_test_assert(escape_str == L"%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str.data());
			yggr_test_assert(ch == ch2);
		}
	}

	{
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		{
			boost::container::wstring escape_str = conv_type::s_escape_byte<boost::container::wstring>(ch, conv_type::SEPUrl<wchar_t>());
			
			std::wcout << escape_str << std::endl;
			yggr_test_assert(escape_str == L"%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str.data());
			yggr_test_assert(ch == ch2);
		}

		{
			boost::container::wstring escape_str = conv_type::s_escape_byte<boost::container::wstring>(ch, conv_type::SEPUrl<wchar_t>(), boost::container::wstring().get_allocator());
			
			std::wcout << escape_str << std::endl;
			yggr_test_assert(escape_str == L"%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str.data());
			yggr_test_assert(ch == ch2);
		}
	}

	{
		yggr::u8 ch = '<'; // %3C $3C

		yggr_test_assert(ch == 0x3C);

		{
			yggr::wstring escape_str = conv_type::s_escape_byte<yggr::wstring>(ch, conv_type::SEPUrl<wchar_t>());
			
			std::wcout << escape_str << std::endl;
			yggr_test_assert(escape_str == L"%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str.data());
			yggr_test_assert(ch == ch2);
		}

		{
			yggr::wstring escape_str = conv_type::s_escape_byte<yggr::wstring>(ch, conv_type::SEPUrl<wchar_t>(), yggr::wstring().get_allocator());
			
			std::wcout << escape_str << std::endl;
			yggr_test_assert(escape_str == L"%3c");
			yggr::u8 ch2 = conv_type::s_unescape_byte(escape_str.data());
			yggr_test_assert(ch == ch2);
		}
	}

	{
		wchar_t escape_str[6 + 1] = {0};
		char data[2] = { ' ', '<' };

		

		std::wcout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), 2, conv_type::SEPUrl<wchar_t>()) << std::endl;
		yggr_test_assert(yggr::wstring(escape_str) == L"%20%3c");

		std::wcout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), 2, conv_type::SEPStringLiteral<wchar_t>()) << std::endl;
		yggr_test_assert(yggr::wstring(escape_str) == L"$20$3c");

		{
			yggr::string data2;
			std::size_t unescape_size = conv_type::s_unescape_bytes(data2, escape_str, 6, conv_type::SEPStringLiteral<wchar_t>());
			yggr_test_assert(unescape_size == 2);
			yggr_test_assert(data2 == " <");
		}

		{
			yggr::string data2;
			std::size_t unescape_size = conv_type::s_unescape_bytes(data2, escape_str, conv_type::SEPStringLiteral<wchar_t>());
			yggr_test_assert(unescape_size == 2);
			yggr_test_assert(data2 == " <");
		}
	}

	// don't using this style, because wchat_t unknow using big_endian or little_eendian
	{
		wchar_t escape_str[sizeof(wchar_t) * 3 * 2  + 1] = {0};
		wchar_t data[2] = { L' ', L'<' };

		std::wcout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), sizeof(wchar_t) * 2, conv_type::SEPUrl<wchar_t>()) << std::endl;

        if(sizeof(wchar_t) == 2)
         {
            if(yggr::network::is_little_endian())
            {
                yggr_test_assert(yggr::wstring(escape_str) == L"%20%00%3c%00");
            }
            else
            {
                yggr_test_assert(yggr::wstring(escape_str) == L"%00%20%00%3c");
            }
         }
         else
         {
            yggr_test_assert(sizeof(wchar_t) == 4);
            if(yggr::network::is_little_endian())
            {
                yggr_test_assert(yggr::wstring(escape_str) == L"%20%00%00%00%3c%00%00%00");
            }
            else
            {
                yggr_test_assert(yggr::wstring(escape_str) == L"%00%00%00%20%00%00%00%3c");
            }
         }

		std::wcout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), sizeof(wchar_t) * 2, conv_type::SEPStringLiteral<wchar_t>()) << std::endl;

		if(sizeof(wchar_t) == 2)
         {
            if(yggr::network::is_little_endian())
            {
                yggr_test_assert(yggr::wstring(escape_str) == L"$20$00$3c$00");
            }
            else
            {
                yggr_test_assert(yggr::wstring(escape_str) == L"$00$20$00$3c");
            }
         }
         else
         {
            yggr_test_assert(sizeof(wchar_t) == 4);
            if(yggr::network::is_little_endian())
            {
                yggr_test_assert(yggr::wstring(escape_str) == L"$20$00$00$00$3c$00$00$00");
            }
            else
            {
                yggr_test_assert(yggr::wstring(escape_str) == L"$00$00$00$20$00$00$00$3c");
            }
         }

		{
			yggr::wstring data2;
			std::size_t unescape_size = conv_type::s_unescape_bytes(data2, escape_str, sizeof(wchar_t) * 3 * 2, conv_type::SEPStringLiteral<wchar_t>());
			yggr_test_assert(unescape_size == sizeof(wchar_t) * 2);
			yggr_test_assert(data2 == L" <");
		}

		{
			yggr::wstring data2;
			std::size_t unescape_size = conv_type::s_unescape_bytes(data2, escape_str, conv_type::SEPStringLiteral<wchar_t>());
			yggr_test_assert(unescape_size == sizeof(wchar_t) * 2);
			yggr_test_assert(data2 == L" <");
		}
	}

	{
		wchar_t escape_str[sizeof(char) * 3 * 2 + 1] = {0};
		char data[2] = { ' ', '<' };

		std::wcout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), sizeof(char) * 2, conv_type::SEPUrl<wchar_t>()) << std::endl;
		yggr_test_assert(yggr::wstring(escape_str) == L"%20%3c");

		std::wcout << conv_type::s_escape_bytes(escape_str, reinterpret_cast<const yggr::u8*>(data), sizeof(char) * 2, conv_type::SEPStringLiteral<wchar_t>()) << std::endl;
		yggr_test_assert(yggr::wstring(escape_str) == L"$20$3c");


		char data2[2] = {0};
		std::size_t unescape_size = conv_type::s_unescape_bytes(reinterpret_cast<yggr::u8*>(data2), escape_str, sizeof(char) * 3 * 2, conv_type::SEPStringLiteral<wchar_t>());
		yggr_test_assert(unescape_size == 2);

		yggr_test_assert(0 == memcmp(data, data2, 2));
	}

	{
		char data[2] = { ' ', '<' };
		yggr::string hex_buf(data, data + 2);

		{
			std::size_t n = yggr::byte_size(boost::begin(hex_buf), boost::end(hex_buf));
			assert(n == 2);
			yggr::wstring escape_str;
			std::wcout << conv_type::s_escape_bytes(escape_str, hex_buf, conv_type::SEPUrl<wchar_t>()) << std::endl;
			yggr_test_assert(escape_str == L"%20%3c");
		}

		{
			yggr::wstring escape_str;
			std::wcout << conv_type::s_escape_bytes(escape_str, hex_buf, conv_type::SEPStringLiteral<wchar_t>()) << std::endl;
			yggr_test_assert(escape_str == L"$20$3c");
		}

		{
			yggr::wstring escape_str = conv_type::s_escape_bytes<yggr::wstring>(hex_buf, conv_type::SEPUrl<wchar_t>());
			std::wcout << escape_str << std::endl;
			yggr_test_assert(escape_str == L"%20%3c");
		}

		{
			yggr::wstring escape_str = conv_type::s_escape_bytes<yggr::wstring>(hex_buf, conv_type::SEPStringLiteral<wchar_t>());
			std::wcout << escape_str << std::endl;
			yggr_test_assert(escape_str == L"$20$3c");
		}

		{
			yggr::wstring escape_str =
				conv_type::s_escape_bytes<yggr::wstring>(hex_buf, conv_type::SEPUrl<wchar_t>(), yggr::wstring().get_allocator());
			std::wcout << escape_str << std::endl;
			yggr_test_assert(escape_str == L"%20%3c");
		}

		{
			yggr::wstring escape_str =
				conv_type::s_escape_bytes<yggr::wstring>(hex_buf, conv_type::SEPStringLiteral<wchar_t>(), yggr::wstring().get_allocator());
			std::wcout << escape_str << std::endl;
			yggr_test_assert(escape_str == L"$20$3c");
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "--------------test_escape_codes_converter end-----------------" << std::endl;
}

void test_unescape_codes_converter(void)
{
	typedef yggr::charset::escape_codes_converter conv_type;

	{
		yggr::string escape_str = "aaa%20%3cccc";
		yggr::string hex_buf;

		std::size_t conv_size = conv_type::s_unescape_bytes(hex_buf, escape_str, conv_type::SEPUrl<char>());
		yggr_test_assert(conv_size == 8);
		yggr_test_assert(hex_buf == "aaa <ccc");
	}

	{
		yggr::utf8_string escape_str = "aaa%20%3cccc";
		yggr::string hex_buf;

		std::size_t conv_size = conv_type::s_unescape_bytes(hex_buf, escape_str, conv_type::SEPUrl<char>());
		yggr_test_assert(conv_size == 8);
		yggr_test_assert(hex_buf == "aaa <ccc");
	}

	{
		yggr::utf8_string escape_str("aaa%20%3cccc你好", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr::utf8_string hex_buf;

		std::size_t conv_size =
			conv_type::s_unescape_bytes(
				hex_buf.org_str(), // !!! don't using yggr::utf8_string, because unescape_bytes destroys the utf8_string length !!!
				escape_str, conv_type::SEPUrl<char>());
		hex_buf.recount_length();
		yggr_test_assert(conv_size == 14);
		yggr_test_assert(hex_buf.size() == conv_size);
		yggr_test_assert(hex_buf.length() == 10);
		yggr_test_assert(hex_buf == yggr::utf8_string("aaa <ccc你好", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
	}

	{
		yggr::string_view escape_str = "aaa%20%3cccc";
		yggr::string hex_buf;

		std::size_t conv_size = conv_type::s_unescape_bytes(hex_buf, escape_str, conv_type::SEPUrl<char>());
		yggr_test_assert(conv_size == 8);
		yggr_test_assert(hex_buf.size() == 8);
		yggr_test_assert(hex_buf.length() == 8);
		yggr_test_assert(hex_buf == "aaa <ccc");
	}

	{
		yggr::utf8_string_view escape_str = "aaa%20%3cccc";
		yggr::string hex_buf;

		std::size_t conv_size = conv_type::s_unescape_bytes(hex_buf, escape_str, conv_type::SEPUrl<char>());
		yggr_test_assert(conv_size == 8);
		yggr_test_assert(hex_buf.size() == 8);
		yggr_test_assert(hex_buf.length() == 8);
		yggr_test_assert(hex_buf == "aaa <ccc");
	}

#if !defined(YGGR_NO_CWCHAR)
	// don't using this style, because wchat_t unknow using big_endian or little_eendian
	{
		yggr::wstring escape_str;

        if(sizeof(wchar_t) == 2)
        {
            if(yggr::network::is_little_endian())
            {
                escape_str = L"aaa%20%00%3c%00ccc";
            }
            else
            {
                escape_str = L"aaa%00%20%00%3cccc";
            }
        }
        else
        {
            yggr_test_assert(sizeof(wchar_t) == 4);
            if(yggr::network::is_little_endian())
            {
                escape_str = L"aaa%20%00%00%00%3c%00%00%00ccc";
            }
            else
            {
                escape_str = L"aaa%00%00%00%20%00%00%00%3cccc";
            }
        }

		yggr::wstring hex_buf;

		std::size_t conv_size = conv_type::s_unescape_bytes(hex_buf, escape_str, conv_type::SEPUrl<wchar_t>());
		yggr_test_assert(conv_size == sizeof(wchar_t) * 8);
		yggr_test_assert(hex_buf == L"aaa <ccc");
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "--------------test_unescape_codes_converter end-----------------" << std::endl;
}

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	test_escape_codes_basic_converter();
	test_escape_codes_converter();
	test_unescape_codes_converter();

	std::cout << "--------------all end-----------------" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
