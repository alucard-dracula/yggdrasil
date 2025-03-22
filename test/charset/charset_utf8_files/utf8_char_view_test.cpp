// utf8_char_view_test.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/charset/utf8_char.hpp>
#include <yggr/charset/utf8_char_view.hpp>
#include <yggr/charset/utf8_char_reference.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	{
		yggr::utf8_char_view chv("a");
		yggr_test_assert(chv == "a");
	}

	{
		yggr::utf8_char ch('a');
		yggr::utf8_char_view chv(ch.begin());
		yggr_test_assert(chv == ch);
	}

	{
		yggr::utf8_char ch('a');
		yggr::utf8_char_view chv(ch);
		yggr_test_assert(chv == ch);
	}

	{
		yggr::utf8_string str("abc");
		yggr::utf8_char_view chv(str, 1);
		yggr_test_assert(chv == str[1]);
	}

	{
		yggr::utf8_string str("中文");
		yggr::utf8_char_view chv(str, 1);
		std::cout << chv << std::endl;
		yggr_test_assert(chv == str[1]);
	}

	{
		yggr::utf8_string str("abc");
		yggr::utf8_char_view chv(str[1]);
		yggr_test_assert(chv == str[1]);
	}

	{
		yggr::utf8_string str("中文");
		yggr::utf8_char_view chv(str[1]);
		std::cout << chv << std::endl;
		yggr_test_assert(chv == str[1]);
	}

	{
		const yggr::utf8_string str("abc");
		yggr::utf8_char_view chv(str[1]);
		yggr_test_assert(chv == str[1]);
	}

	{
		yggr::utf8_char_view chv0("a");
		yggr::utf8_char_view chv(boost::move(chv0));
		yggr_test_assert(chv == "a");
	}

	{
		yggr::utf8_char_view chv0("a");
		yggr::utf8_char_view chv(chv0);
		yggr_test_assert(chv == "a");
	}

	std::cout << "---------test_construct end----------" << std::endl;
}

void test_operator_set(void)
{
	{
		yggr::utf8_char_view chv0("a");
		yggr::utf8_char_view chv("b");
		chv = boost::move(chv0);
		yggr_test_assert(chv == "a");
	}

	{
		yggr::utf8_char_view chv0("a");
		yggr::utf8_char_view chv("b");
		chv = chv0;
		yggr_test_assert(chv == "a");
	}

	std::cout << "---------test_operator_set end----------" << std::endl;
}

void test_swap(void)
{
	// std::swap

	{
		yggr::utf8_char_view chv_l("a");
		yggr::utf8_char_view chv_r("b");

		std::swap(boost::move(chv_l), chv_r);
		
		yggr_test_assert(chv_l == "b");
		yggr_test_assert(chv_r == "a");
	}

	{
		yggr::utf8_char_view chv_l("a");
		yggr::utf8_char_view chv_r("b");

		std::swap(chv_l, boost::move(chv_r));
		
		yggr_test_assert(chv_l == "b");
		yggr_test_assert(chv_r == "a");
	}

	{
		yggr::utf8_char_view chv_l("a");
		yggr::utf8_char_view chv_r("b");

		std::swap(chv_l, chv_r);
		
		yggr_test_assert(chv_l == "b");
		yggr_test_assert(chv_r == "a");
	}

	// boost::swap

	{
		yggr::utf8_char_view chv_l("a");
		yggr::utf8_char_view chv_r("b");

		boost::swap(boost::move(chv_l), chv_r);
		
		yggr_test_assert(chv_l == "b");
		yggr_test_assert(chv_r == "a");
	}

	{
		yggr::utf8_char_view chv_l("a");
		yggr::utf8_char_view chv_r("b");

		boost::swap(chv_l, boost::move(chv_r));
		
		yggr_test_assert(chv_l == "b");
		yggr_test_assert(chv_r == "a");
	}

	{
		yggr::utf8_char_view chv_l("a");
		yggr::utf8_char_view chv_r("b");

		boost::swap(chv_l, chv_r);
		
		yggr_test_assert(chv_l == "b");
		yggr_test_assert(chv_r == "a");
	}

	// yggr::swap

	{
		yggr::utf8_char_view chv_l("a");
		yggr::utf8_char_view chv_r("b");

		yggr::swap(boost::move(chv_l), chv_r);
		
		yggr_test_assert(chv_l == "b");
		yggr_test_assert(chv_r == "a");
	}

	{
		yggr::utf8_char_view chv_l("a");
		yggr::utf8_char_view chv_r("b");

		yggr::swap(chv_l, boost::move(chv_r));
		
		yggr_test_assert(chv_l == "b");
		yggr_test_assert(chv_r == "a");
	}

	{
		yggr::utf8_char_view chv_l("a");
		yggr::utf8_char_view chv_r("b");

		yggr::swap(chv_l, chv_r);
		
		yggr_test_assert(chv_l == "b");
		yggr_test_assert(chv_r == "a");
	}

	std::cout << "---------test_swap end----------" << std::endl;
}

void test_operator_conv(void)
{
	// operator bool
	{
		yggr::utf8_char_view chv("");
		yggr_test_assert(!chv);
	}

	{
		yggr::utf8_char_view chv("a");
		yggr_test_assert(!!chv);
	}

	// operator basic_string
	{
		// operator std::string
		{
			yggr::utf8_char_view chv("a");
			std::string str;

			str = static_cast<std::string>(chv); // gcc need using static_cast
		
			std::cout << str << std::endl;
			yggr_test_assert(str == "a");
		}

		// operator boost::container::string
		{
			yggr::utf8_char_view chv("a");
			boost::container::string str;

			str = static_cast<boost::container::string>(chv); // gcc need using static_cast
		
			std::cout << str << std::endl;
			yggr_test_assert(str == "a");
		}

		// operator yggr::string
		{
			yggr::utf8_char_view chv("a");
			yggr::string str;

			str = static_cast<yggr::string>(chv); // gcc need using static_cast
		
			std::cout << str << std::endl;
			yggr_test_assert(str == "a");
		}
	}

	// operator yggr::utf8_char
	{
		yggr::utf8_char_view chv("a");
		yggr::utf8_char ch;

		ch = static_cast<yggr::utf8_char>(chv);
		
		std::cout << ch << std::endl;
		yggr_test_assert(ch == "a");
	}

	// operator number
	{
		// operator s8
		{
			yggr::utf8_char_view chv("a");
			yggr::s8 num = 0;

			num = chv;
		
			yggr_test_assert(num == 97);
		}

		// operator u8
		{
			yggr::utf8_char_view chv("a");
			yggr::u8 num = 0;

			num = chv;
		
			yggr_test_assert(num == 97);
		}

		// operator s16
		{
			yggr::utf8_char_view chv("a");
			yggr::s16 num = 0;

			num = chv;
		
			yggr_test_assert(num == 97);
		}

		// operator u16
		{
			yggr::utf8_char_view chv("a");
			yggr::u16 num = 0;

			num = chv;
		
			yggr_test_assert(num == 97);
		}

		// operator s32
		{
			yggr::utf8_char_view chv("a");
			yggr::s32 num = 0;

			num = chv;
		
			yggr_test_assert(num == 97);
		}

		// operator u32
		{
			yggr::utf8_char_view chv("a");
			yggr::u32 num = 0;

			num = chv;
		
			yggr_test_assert(num == 97);
		}

		// operator s64
		{
			yggr::utf8_char_view chv("a");
			yggr::s64 num = 0;

			num = chv;
		
			yggr_test_assert(num == 97);
		}

		// operator u64
		{
			yggr::utf8_char_view chv("a");
			yggr::u64 num = 0;

			num = chv;
		
			yggr_test_assert(num == 97);
		}

		// operator f32
		{
			yggr::utf8_char_view chv("a");
			yggr::f32 num = 0;

			num = chv;
		
			yggr_test_assert(num == 97);
		}

		// operator f64
		{
			yggr::utf8_char_view chv("a");
			yggr::f64 num = 0;

			num = chv;
		
			yggr_test_assert(num == 97);
		}
	}

	std::cout << "---------test_operator_conv end----------" << std::endl;
}

void test_pointer(void)
{
	{
		const char* str = "a";
		yggr::utf8_char_view chv(str);
		yggr_test_assert(&chv == str);
	}

	{
		const char* str = "a";
		yggr::utf8_char_view chv(str);
		yggr_test_assert(chv.data() == str);
	}

	std::cout << "---------test_pointer end----------" << std::endl;
}

void test_other(void)
{
	{
		yggr::utf8_char_view chv("a");
		yggr_test_assert(chv.size() == 1);
		yggr_test_assert(chv.length() == 1);
		yggr_test_assert(chv.is_validate());
	}

	{
		yggr::utf8_char ch("中");
		yggr::utf8_char_view chv(ch);
		yggr_test_assert(chv.size() != 1);
		yggr_test_assert(chv.length() == 1);
		yggr_test_assert(chv.is_validate());
	}

	std::cout << "---------test_other end----------" << std::endl;
}

void test_iterators(void)
{
	{
		const char* str = "a";
		yggr::utf8_char_view chv(str);

		yggr_test_assert(str == chv.begin());
		yggr_test_assert(str + 1 == chv.end());

		yggr_test_assert(str == chv.cbegin());
		yggr_test_assert(str + 1 == chv.cend());
	}

	std::cout << "---------test_iterators end----------" << std::endl;
}

void test_is_same_address(void)
{
	{
		yggr::utf8_string str("abc");

		yggr::utf8_char ch(str[0]);

		yggr::utf8_string& str_ref = str;
		const yggr::utf8_string& str_cref = str;

		yggr::utf8_char_view chv(str[0]);

		yggr_test_assert(chv.is_same_address(str_ref[0]));
		yggr_test_assert(chv.is_same_address(str_cref[0]));
		yggr_test_assert(chv.is_same_address(chv));
		yggr_test_assert(!chv.is_same_address(ch));
	}

	std::cout << "---------test_is_same_address end----------" << std::endl;
}

void test_compare(void)
{
	{
		yggr_test_assert(0 == yggr::utf8_char_view("b").compare('b'));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare('a'));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare('c'));

		yggr_test_assert(0 == yggr::utf8_char_view("b").compare('b', YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare('a', YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare('c', YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	{
		yggr_test_assert(0 == yggr::utf8_char_view("b").compare("abc" + 1, 1));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare("abc" + 0, 1));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare("abc" + 2, 1));

		yggr_test_assert(0 == yggr::utf8_char_view("b").compare("abc" + 1, 1, YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare("abc" + 0, 1, YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare("abc" + 2, 1, YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	{
		yggr_test_assert(0 == yggr::utf8_char_view("b").compare("b"));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare("a"));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare("c"));

		yggr_test_assert(0 == yggr::utf8_char_view("b").compare("b", YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare("a", YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare("c", YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	{
		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(std::string("b")));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(std::string("a")));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(std::string("c")));

		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(std::string("b"), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(std::string("a"), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(std::string("c"), YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	{
		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(boost::container::string("b")));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(boost::container::string("a")));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(boost::container::string("c")));

		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(boost::container::string("b"), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(boost::container::string("a"), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(boost::container::string("c"), YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	{
		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(yggr::string("b")));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(yggr::string("a")));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(yggr::string("c")));

		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(yggr::string("b"), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(yggr::string("a"), YGGR_STR_INNER_STRING_CHARSET_NAME()));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(yggr::string("c"), YGGR_STR_INNER_STRING_CHARSET_NAME()));
	}

	{
		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(yggr::utf8_char("b")));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(yggr::utf8_char("a")));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(yggr::utf8_char("c")));
	}

	{
		yggr::utf8_string str("abc");
		yggr::utf8_string& str_ref = str;
		const yggr::utf8_string& str_cref = str;

		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(str_ref[1]));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(str_ref[0]));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(str_ref[2]));

		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(str_cref[1]));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(str_cref[0]));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(str_cref[2]));
	}

	{
		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(yggr::utf8_char_view("b")));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(yggr::utf8_char_view("a")));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(yggr::utf8_char_view("c")));
	}

	{
		yggr_test_assert(0 == yggr::utf8_char_view("b").compare(yggr::charset::make_string_charset_helper('b')));
		yggr_test_assert(0 < yggr::utf8_char_view("b").compare(yggr::charset::make_string_charset_helper('a')));
		yggr_test_assert(0 > yggr::utf8_char_view("b").compare(yggr::charset::make_string_charset_helper('c')));
	}
	
	// operator <=>
	{
		// char
		{
			// ==
			yggr_test_assert(yggr::utf8_char_view("b") == 'b');
			yggr_test_assert('b' == yggr::utf8_char_view("b"));

			yggr_test_assert(yggr::utf8_char_view("b") == yggr::charset::make_string_charset_helper('b'));
			yggr_test_assert(yggr::charset::make_string_charset_helper('b') == yggr::utf8_char_view("b"));

			// !=
			yggr_test_assert(yggr::utf8_char_view("b") != 'c');
			yggr_test_assert('b' != yggr::utf8_char_view("c"));

			yggr_test_assert(yggr::utf8_char_view("b") != yggr::charset::make_string_charset_helper('c'));
			yggr_test_assert(yggr::charset::make_string_charset_helper('b') != yggr::utf8_char_view("c"));

			// <
			yggr_test_assert('b' < yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") < 'c');

			yggr_test_assert(yggr::utf8_char_view("b") < yggr::charset::make_string_charset_helper('c'));
			yggr_test_assert(yggr::charset::make_string_charset_helper('b') < yggr::utf8_char_view("c"));

			// <=
			yggr_test_assert('b' <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= 'c');
			yggr_test_assert('b' <= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") <= 'b');

			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper('c'));
			yggr_test_assert(yggr::charset::make_string_charset_helper('b') <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper('b'));
			yggr_test_assert(yggr::charset::make_string_charset_helper('b') <= yggr::utf8_char_view("b"));

			// > 
			yggr_test_assert('b' > yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") > 'a');

			yggr_test_assert(yggr::utf8_char_view("b") > yggr::charset::make_string_charset_helper('a'));
			yggr_test_assert(yggr::charset::make_string_charset_helper('b') > yggr::utf8_char_view("a"));

			// >=
			yggr_test_assert('b' >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= 'a');
			yggr_test_assert('b' >= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") >= 'b');

			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper('a'));
			yggr_test_assert(yggr::charset::make_string_charset_helper('b') >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper('b'));
			yggr_test_assert(yggr::charset::make_string_charset_helper('b') >= yggr::utf8_char_view("b"));
		}

		// const char*
		{
			// ==
			yggr_test_assert(yggr::utf8_char_view("b") == "b");
			yggr_test_assert("b" == yggr::utf8_char_view("b"));

			yggr_test_assert(yggr::utf8_char_view("b") == yggr::charset::make_string_charset_helper("b"));
			yggr_test_assert(yggr::charset::make_string_charset_helper("b") == yggr::utf8_char_view("b"));

			// !=
			yggr_test_assert(yggr::utf8_char_view("b") != "c");
			yggr_test_assert("b" != yggr::utf8_char_view("c"));

			yggr_test_assert(yggr::utf8_char_view("b") != yggr::charset::make_string_charset_helper("c"));
			yggr_test_assert(yggr::charset::make_string_charset_helper("b") != yggr::utf8_char_view("c"));

			// <
			yggr_test_assert("b" < yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") < "c");

			yggr_test_assert(yggr::utf8_char_view("b") < yggr::charset::make_string_charset_helper("c"));
			yggr_test_assert(yggr::charset::make_string_charset_helper("b") < yggr::utf8_char_view("c"));

			// <=
			yggr_test_assert("b" <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= "c");
			yggr_test_assert("b" <= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") <= "b");

			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper("c"));
			yggr_test_assert(yggr::charset::make_string_charset_helper("b") <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper("b"));
			yggr_test_assert(yggr::charset::make_string_charset_helper("b") <= yggr::utf8_char_view("b"));

			// > 
			yggr_test_assert("b" > yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") > "a");

			yggr_test_assert(yggr::utf8_char_view("b") > yggr::charset::make_string_charset_helper("a"));
			yggr_test_assert(yggr::charset::make_string_charset_helper("b") > yggr::utf8_char_view("a"));

			// >=
			yggr_test_assert("b" >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= "a");
			yggr_test_assert("b" >= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") >= "b");

			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper("a"));
			yggr_test_assert(yggr::charset::make_string_charset_helper("b") >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper("b"));
			yggr_test_assert(yggr::charset::make_string_charset_helper("b") >= yggr::utf8_char_view("b"));
		}

		// const std::string
		{
			// ==
			yggr_test_assert(yggr::utf8_char_view("b") == std::string("b"));
			yggr_test_assert(std::string("b") == yggr::utf8_char_view("b"));

			yggr_test_assert(yggr::utf8_char_view("b") == yggr::charset::make_string_charset_helper(std::string("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("b")) == yggr::utf8_char_view("b"));

			// !=
			yggr_test_assert(yggr::utf8_char_view("b") != std::string("c"));
			yggr_test_assert(std::string("b") != yggr::utf8_char_view("c"));

			yggr_test_assert(yggr::utf8_char_view("b") != yggr::charset::make_string_charset_helper(std::string("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("b")) != yggr::utf8_char_view("c"));

			// <
			yggr_test_assert(std::string("b") < yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") < std::string("c"));

			yggr_test_assert(yggr::utf8_char_view("b") < yggr::charset::make_string_charset_helper(std::string("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("b")) < yggr::utf8_char_view("c"));

			// <=
			yggr_test_assert(std::string("b") <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= std::string("c"));
			yggr_test_assert(std::string("b") <= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") <= std::string("b"));

			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper(std::string("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("b")) <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper(std::string("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("b")) <= yggr::utf8_char_view("b"));

			// > 
			yggr_test_assert(std::string("b") > yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") > std::string("a"));

			yggr_test_assert(yggr::utf8_char_view("b") > yggr::charset::make_string_charset_helper(std::string("a")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("b")) > yggr::utf8_char_view("a"));

			// >=
			yggr_test_assert(std::string("b") >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= std::string("a"));
			yggr_test_assert(std::string("b") >= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") >= std::string("b"));

			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper(std::string("a")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("b")) >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper(std::string("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(std::string("b")) >= yggr::utf8_char_view("b"));
		}

		// const boost::container::string
		{
			// ==
			yggr_test_assert(yggr::utf8_char_view("b") == boost::container::string("b"));
			yggr_test_assert(boost::container::string("b") == yggr::utf8_char_view("b"));

			yggr_test_assert(yggr::utf8_char_view("b") == yggr::charset::make_string_charset_helper(boost::container::string("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("b")) == yggr::utf8_char_view("b"));

			// !=
			yggr_test_assert(yggr::utf8_char_view("b") != boost::container::string("c"));
			yggr_test_assert(boost::container::string("b") != yggr::utf8_char_view("c"));

			yggr_test_assert(yggr::utf8_char_view("b") != yggr::charset::make_string_charset_helper(boost::container::string("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("b")) != yggr::utf8_char_view("c"));

			// <
			yggr_test_assert(boost::container::string("b") < yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") < boost::container::string("c"));

			yggr_test_assert(yggr::utf8_char_view("b") < yggr::charset::make_string_charset_helper(boost::container::string("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("b")) < yggr::utf8_char_view("c"));

			// <=
			yggr_test_assert(boost::container::string("b") <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= boost::container::string("c"));
			yggr_test_assert(boost::container::string("b") <= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") <= boost::container::string("b"));

			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper(boost::container::string("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("b")) <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper(boost::container::string("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("b")) <= yggr::utf8_char_view("b"));

			// > 
			yggr_test_assert(boost::container::string("b") > yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") > boost::container::string("a"));

			yggr_test_assert(yggr::utf8_char_view("b") > yggr::charset::make_string_charset_helper(boost::container::string("a")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("b")) > yggr::utf8_char_view("a"));

			// >=
			yggr_test_assert(boost::container::string("b") >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= boost::container::string("a"));
			yggr_test_assert(boost::container::string("b") >= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") >= boost::container::string("b"));

			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper(boost::container::string("a")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("b")) >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper(boost::container::string("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(boost::container::string("b")) >= yggr::utf8_char_view("b"));
		}

		// const yggr::string
		{
			// ==
			yggr_test_assert(yggr::utf8_char_view("b") == yggr::string("b"));
			yggr_test_assert(yggr::string("b") == yggr::utf8_char_view("b"));

			yggr_test_assert(yggr::utf8_char_view("b") == yggr::charset::make_string_charset_helper(yggr::string("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("b")) == yggr::utf8_char_view("b"));

			// !=
			yggr_test_assert(yggr::utf8_char_view("b") != yggr::string("c"));
			yggr_test_assert(yggr::string("b") != yggr::utf8_char_view("c"));

			yggr_test_assert(yggr::utf8_char_view("b") != yggr::charset::make_string_charset_helper(yggr::string("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("b")) != yggr::utf8_char_view("c"));

			// <
			yggr_test_assert(yggr::string("b") < yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") < yggr::string("c"));

			yggr_test_assert(yggr::utf8_char_view("b") < yggr::charset::make_string_charset_helper(yggr::string("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("b")) < yggr::utf8_char_view("c"));

			// <=
			yggr_test_assert(yggr::string("b") <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::string("c"));
			yggr_test_assert(yggr::string("b") <= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::string("b"));

			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper(yggr::string("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("b")) <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper(yggr::string("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("b")) <= yggr::utf8_char_view("b"));

			// > 
			yggr_test_assert(yggr::string("b") > yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") > yggr::string("a"));

			yggr_test_assert(yggr::utf8_char_view("b") > yggr::charset::make_string_charset_helper(yggr::string("a")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("b")) > yggr::utf8_char_view("a"));

			// >=
			yggr_test_assert(yggr::string("b") >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::string("a"));
			yggr_test_assert(yggr::string("b") >= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::string("b"));

			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper(yggr::string("a")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("b")) >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper(yggr::string("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("b")) >= yggr::utf8_char_view("b"));
		}

		// const yggr::utf8_char
		{
			// ==
			yggr_test_assert(yggr::utf8_char_view("b") == yggr::utf8_char("b"));
			yggr_test_assert(yggr::utf8_char("b") == yggr::utf8_char_view("b"));

			yggr_test_assert(yggr::utf8_char_view("b") == yggr::charset::make_string_charset_helper(yggr::utf8_char("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char("b")) == yggr::utf8_char_view("b"));

			// !=
			yggr_test_assert(yggr::utf8_char_view("b") != yggr::utf8_char("c"));
			yggr_test_assert(yggr::utf8_char("b") != yggr::utf8_char_view("c"));

			yggr_test_assert(yggr::utf8_char_view("b") != yggr::charset::make_string_charset_helper(yggr::utf8_char("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char("b")) != yggr::utf8_char_view("c"));

			// <
			yggr_test_assert(yggr::utf8_char("b") < yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") < yggr::utf8_char("c"));

			yggr_test_assert(yggr::utf8_char_view("b") < yggr::charset::make_string_charset_helper(yggr::utf8_char("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char("b")) < yggr::utf8_char_view("c"));

			// <=
			yggr_test_assert(yggr::utf8_char("b") <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::utf8_char("c"));
			yggr_test_assert(yggr::utf8_char("b") <= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::utf8_char("b"));

			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper(yggr::utf8_char("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char("b")) <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper(yggr::utf8_char("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char("b")) <= yggr::utf8_char_view("b"));

			// > 
			yggr_test_assert(yggr::utf8_char("b") > yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") > yggr::utf8_char("a"));

			yggr_test_assert(yggr::utf8_char_view("b") > yggr::charset::make_string_charset_helper(yggr::utf8_char("a")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char("b")) > yggr::utf8_char_view("a"));

			// >=
			yggr_test_assert(yggr::utf8_char("b") >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::utf8_char("a"));
			yggr_test_assert(yggr::utf8_char("b") >= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::utf8_char("b"));

			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper(yggr::utf8_char("a")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char("b")) >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper(yggr::utf8_char("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char("b")) >= yggr::utf8_char_view("b"));
		}

		// const yggr::utf8_char_view
		{
			// ==
			yggr_test_assert(yggr::utf8_char_view("b") == yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") == yggr::utf8_char_view("b"));

			yggr_test_assert(yggr::utf8_char_view("b") == yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")) == yggr::utf8_char_view("b"));

			// !=
			yggr_test_assert(yggr::utf8_char_view("b") != yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") != yggr::utf8_char_view("c"));

			yggr_test_assert(yggr::utf8_char_view("b") != yggr::charset::make_string_charset_helper(yggr::utf8_char_view("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")) != yggr::utf8_char_view("c"));

			// <
			yggr_test_assert(yggr::utf8_char_view("b") < yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") < yggr::utf8_char_view("c"));

			yggr_test_assert(yggr::utf8_char_view("b") < yggr::charset::make_string_charset_helper(yggr::utf8_char_view("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")) < yggr::utf8_char_view("c"));

			// <=
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::utf8_char_view("b"));

			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper(yggr::utf8_char_view("c")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")) <= yggr::utf8_char_view("c"));
			yggr_test_assert(yggr::utf8_char_view("b") <= yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")) <= yggr::utf8_char_view("b"));

			// > 
			yggr_test_assert(yggr::utf8_char_view("b") > yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") > yggr::utf8_char_view("a"));

			yggr_test_assert(yggr::utf8_char_view("b") > yggr::charset::make_string_charset_helper(yggr::utf8_char_view("a")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")) > yggr::utf8_char_view("a"));

			// >=
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::utf8_char_view("b"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::utf8_char_view("b"));

			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper(yggr::utf8_char_view("a")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")) >= yggr::utf8_char_view("a"));
			yggr_test_assert(yggr::utf8_char_view("b") >= yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")));
			yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_char_view("b")) >= yggr::utf8_char_view("b"));
		}
	}

	std::cout << "---------test_compare end----------" << std::endl;
}

void test_str(void)
{
	// org_str
	{
		yggr_test_assert(yggr::utf8_char_view("b").org_str() == "b");
		yggr_test_assert(yggr::utf8_char_view("b").org_str(yggr::utf8_char_view::char_type::allocator_type()) == "b");

		yggr_test_assert(yggr::utf8_char_view("b").org_str<std::string>() == "b");
		yggr_test_assert(yggr::utf8_char_view("b").org_str<std::string>(std::string::allocator_type()) == "b");

		yggr_test_assert(yggr::utf8_char_view("b").org_str<boost::container::string>() == "b");
		yggr_test_assert(yggr::utf8_char_view("b").org_str<boost::container::string>(boost::container::string::allocator_type()) == "b");

		yggr_test_assert(yggr::utf8_char_view("b").org_str<yggr::string>() == "b");
		yggr_test_assert(yggr::utf8_char_view("b").org_str<yggr::string>(yggr::string::allocator_type()) == "b");
	}

	{
		yggr_test_assert(yggr::utf8_char_view("b").str() == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str(yggr::utf8_string::allocator_type()) == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str(YGGR_STR_INNER_STRING_CHARSET_NAME()) == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str(yggr::utf8_string::allocator_type(), YGGR_STR_INNER_STRING_CHARSET_NAME()) == "b");

		yggr_test_assert(yggr::utf8_char_view("b").str<yggr::utf8_string>() == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<yggr::utf8_string>(yggr::utf8_string::allocator_type()) == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<yggr::utf8_string>(YGGR_STR_INNER_STRING_CHARSET_NAME()) == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<yggr::utf8_string>(yggr::utf8_string::allocator_type(), YGGR_STR_INNER_STRING_CHARSET_NAME()) == "b");

		yggr_test_assert(yggr::utf8_char_view("b").str<std::string>() == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<std::string>(std::string::allocator_type()) == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<std::string>(YGGR_STR_INNER_STRING_CHARSET_NAME()) == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<std::string>(std::string::allocator_type(), YGGR_STR_INNER_STRING_CHARSET_NAME()) == "b");

		yggr_test_assert(yggr::utf8_char_view("b").str<boost::container::string>() == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<boost::container::string>(boost::container::string::allocator_type()) == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<boost::container::string>(YGGR_STR_INNER_STRING_CHARSET_NAME()) == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<boost::container::string>(boost::container::string::allocator_type(), YGGR_STR_INNER_STRING_CHARSET_NAME()) == "b");

		yggr_test_assert(yggr::utf8_char_view("b").str<yggr::string>() == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<yggr::string>(yggr::string::allocator_type()) == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<yggr::string>(YGGR_STR_INNER_STRING_CHARSET_NAME()) == "b");
		yggr_test_assert(yggr::utf8_char_view("b").str<yggr::string>(yggr::string::allocator_type(), YGGR_STR_INNER_STRING_CHARSET_NAME()) == "b");
	}

	std::cout << "---------test_str end----------" << std::endl;
}

void test_view(void)
{
	{
		yggr_test_assert(yggr::utf8_char_view("b").view() == yggr::utf8_char_view("b"));
		yggr_test_assert(yggr::utf8_char_view("b").view< yggr::utf8_char_view >() == yggr::utf8_char_view("b"));
	}

	std::cout << "---------test_view end----------" << std::endl;
}


template<typename T>
void test_arithmeric_op_set_int(void)
{
	// += -= *= /= &= |= ^=
	{
		{
			T n = 1;
			yggr::utf8_char ch(3);
			n += yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 4);
		}

		{
			T n = 5;
			yggr::utf8_char ch(3);
			n -= yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 2);
		}

		{
			T n = 1;
			yggr::utf8_char ch(3);
			n *= yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 3);
		}

		{
			T n = 1;
			yggr::utf8_char ch(3);
			n /= yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 0);
		}

		{
			T n = 1;
			yggr::utf8_char ch(3);
			n %= yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 1);
		}

		{
			T n = 1;
			yggr::utf8_char ch(3);
			n &= yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 1);
		}

		{
			T n = 1;
			yggr::utf8_char ch(3);
			n |= yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 3);
		}

		{
			T n = 1;
			yggr::utf8_char ch(3);
			n ^= yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 2);
		}
	}
}

template<typename T>
void test_arithmeric_op_set_float(void)
{
	// += -= *= /= &= |= ^=
	{
		{
			T n = 1;
			yggr::utf8_char ch(3);
			n += yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 4);
		}

		{
			T n = 1;
			yggr::utf8_char ch(3);
			n -= yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == -2);
		}

		{
			T n = 1;
			yggr::utf8_char ch(3);
			n *= yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 3);
		}

		{
			T n = 1;
			yggr::utf8_char ch(2);
			n /= yggr::utf8_char_view(ch);
			//std::cout << n << std::endl;
			yggr_test_assert(n == 0.5);
		}

	}
}

void test_arithmetic(void)
{
	{
		yggr_test_assert(yggr::utf8_char_view("a").to_number() == 97);
		yggr_test_assert(+yggr::utf8_char_view("a") == 97);
		// -utf8_char_view("a") is not -97, because utf8 range is 0x0000 - 0x10FFFF.
		// -97 out of range
		yggr_test_assert(-yggr::utf8_char_view("a") == -yggr::utf8_char("a"));
		yggr_test_assert(-yggr::utf8_char_view("a") == 0x10FFFF - 'a' + 1);

		yggr_test_assert(~yggr::utf8_char_view("a") == ~yggr::utf8_char("a"));
		yggr_test_assert(~yggr::utf8_char_view("a") == 0x10FFFF - 'a');
	}

	{
		yggr_test_assert(1 + yggr::utf8_char_view("a") == 98);
		yggr_test_assert(yggr::utf8_char_view("a") + 1 == yggr::utf8_char_view("b"));
		yggr_test_assert(yggr::utf8_char_view("a") + yggr::utf8_char_view("b") - yggr::utf8_char_view("a") == yggr::utf8_char_view("b"));
	}

	{
		yggr_test_assert(98 - yggr::utf8_char_view("a") == 1);
		yggr_test_assert(yggr::utf8_char_view("b") - 1 == yggr::utf8_char_view("a"));
		yggr_test_assert(yggr::utf8_char_view("b") - yggr::utf8_char_view("a") == yggr::utf8_char(1));
	}

	{
		yggr_test_assert(1 * yggr::utf8_char_view("a") == 97);
		yggr_test_assert(yggr::utf8_char_view("a") * 1 == yggr::utf8_char_view("a"));
		yggr_test_assert(yggr::utf8_char_view("d") * yggr::utf8_char_view("d") == 10000);
	}

	{
		yggr_test_assert(97 * 97 / yggr::utf8_char_view("a") == 97);
		yggr_test_assert(yggr::utf8_char_view("a") / 1 == yggr::utf8_char_view("a"));
		yggr_test_assert(yggr::utf8_char_view("d") / yggr::utf8_char_view("d") == 1);
	}

	{
		yggr_test_assert(100 % yggr::utf8_char_view("a") == 3);
		yggr_test_assert(yggr::utf8_char_view("d") % 97 == 3);
		yggr_test_assert(yggr::utf8_char_view("d") % yggr::utf8_char_view("a") == 3);
	}

	{
		yggr::utf8_char ch(1);
		yggr_test_assert(1 << yggr::utf8_char_view(ch) == 2);
		yggr_test_assert(yggr::utf8_char_view(ch) << 1 == 2);
		yggr_test_assert(yggr::utf8_char_view(ch) << yggr::utf8_char_view(ch) == 2);
	}

	{
		yggr::utf8_char ch(1);
		yggr_test_assert(1 >> yggr::utf8_char_view(ch) == 0);
		yggr_test_assert(yggr::utf8_char_view(ch) >> 1 == 0);
		yggr_test_assert(yggr::utf8_char_view(ch) >> yggr::utf8_char_view(ch) == 0);
	}

	// += -= *= /= &= |= ^=
	{
		test_arithmeric_op_set_int<yggr::s8>();
		test_arithmeric_op_set_int<yggr::u8>();

		test_arithmeric_op_set_int<yggr::s16>();
		test_arithmeric_op_set_int<yggr::u16>();

		test_arithmeric_op_set_int<yggr::s32>();
		test_arithmeric_op_set_int<yggr::u32>();

		test_arithmeric_op_set_int<yggr::s64>();
		test_arithmeric_op_set_int<yggr::u64>();

		test_arithmeric_op_set_float<yggr::f32>();
		test_arithmeric_op_set_float<yggr::f64>();
	}

	std::cout << "---------test_arithmetic end----------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_operator_conv();

	test_pointer();
	test_other();

	test_iterators();
	test_is_same_address();

	test_compare();

	test_str();

	test_view();

	test_arithmetic();
	
	wait_any_key(argc, argv);
	return 0;
}
