//utf8_char_reference_test.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

//#define YGGR_USE_STL_STRING
//#define _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/charset/utf8_char.hpp>
#include <yggr/charset/utf8_char_reference.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_utf8_char_reference(void)
{
	typedef yggr::charset::utf8_char char_type;
	typedef yggr::charset::basic_utf8_char_reference<char_type> char_ref_type;
	typedef yggr::charset::basic_utf8_char_const_reference<char_type> char_cref_type;

	// hello world
	{
		char_type ch1 = "a";
		char_type ch2 = "b";

		char_ref_type ch1_ref = ch1;
		char_ref_type ch2_ref = ch2;
		yggr_test_assert(ch1_ref == "a");
		yggr_test_assert(ch2_ref == "b");
	}

	// construct
	{
		{
			char_type ch = "a";
			char_ref_type ch_ref(ch);
			yggr_test_assert(ch_ref == "a");
		}

		{
			char_type ch = "a";
			char_ref_type ch_ref(ch, ch.begin());
			yggr_test_assert(ch_ref == "a");
		}

		{
			char_type ch = "a";
			char_ref_type ch_ref(ch, 0);
			yggr_test_assert(ch_ref == "a");
		}

#if 0
		{
			char_type ch = "a";
			char_ref_type ch_ref(ch, 1); // yggr_test_assert(false)
			yggr_test_assert(ch_ref == "a");
		}
#endif // 0, 1

	}

	// operator=
	{
		char_type ch = "a";
		char_ref_type ch_ref(ch, 0);
		ch_ref = 'b';
		yggr_test_assert(ch_ref == "b");
		yggr_test_assert(ch == "b");

		{
			char_type ch = "a";
			char_cref_type ch_cref = ch;

			ch_ref = ch_cref;
			yggr_test_assert(ch_ref == "a");

			yggr_test_assert(!(ch_ref != ch_cref));
		}


		ch_ref = yggr::charset::make_string_charset_helper('c');
		yggr_test_assert(ch_ref == "c");
		yggr_test_assert(ch == "c");


		{
			char_type ch = "a";
			char_cref_type ch_cref = ch;

			ch_ref = yggr::charset::make_string_charset_helper(ch_cref);
			yggr_test_assert(ch_ref == "a");
			yggr_test_assert(ch == "a");

			yggr_test_assert(ch_ref == ch_cref);
		}

	}

	{
		char_type ch = "a";
		char_ref_type ch_ref(ch, 0);
		ch_ref = "b";
		yggr_test_assert(ch_ref == "b");
		yggr_test_assert(ch == "b");

		ch_ref = yggr::charset::make_string_charset_helper("c");
		yggr_test_assert(ch_ref == "c");
		yggr_test_assert(ch == "c");
	}

	{
		char_type ch = "a";
		char_ref_type ch_ref(ch, 0);
		ch_ref = std::string("b");
		yggr_test_assert(ch_ref == "b");
		yggr_test_assert(ch == "b");

		ch_ref = yggr::string("c");
		yggr_test_assert(ch_ref == "c");
		yggr_test_assert(ch == "c");

		ch_ref = char_type("d");
		yggr_test_assert(ch_ref == "d");
		yggr_test_assert(ch == "d");

		ch_ref = yggr::charset::make_string_charset_helper('e');
		yggr_test_assert(ch_ref == "e");
		yggr_test_assert(ch == "e");

		ch_ref = yggr::charset::make_string_charset_helper("e");
		yggr_test_assert(ch_ref == "e");
		yggr_test_assert(ch == "e");

		ch_ref = yggr::charset::make_string_charset_helper(std::string("f"));
		yggr_test_assert(ch_ref == "f");
		yggr_test_assert(ch == "f");

		ch_ref = yggr::charset::make_string_charset_helper(yggr::string("g"));
		yggr_test_assert(ch_ref == "g");
		yggr_test_assert(ch == "g");

		ch_ref = yggr::charset::make_string_charset_helper(char_type("h"));
		yggr_test_assert(ch_ref == "h");
		yggr_test_assert(ch == "h");
	}

	{
		char_type ch1 = "a";
		char_type ch2 = "b";
		char_type ch3 = "c";

		char_ref_type ch1_ref = ch1;
		char_ref_type ch2_ref = ch2;
		char_ref_type ch3_ref = ch3;

		ch1 = ch1;
		yggr_test_assert(ch1 == "a");

		ch1 = ch2;
		yggr_test_assert(ch2 == "b");

		ch1 = yggr::charset::make_string_charset_helper(ch3_ref);
		yggr_test_assert(ch1 == "c");
	}

	// swap
	{
		char_type ch1 = "a";
		char_type ch2 = "b";
		char_type ch3 = "c";

		char_ref_type ch1_ref = ch1;
		char_ref_type ch2_ref = ch2;
		char_ref_type ch3_ref = ch3;

		yggr::swap(ch1_ref, ch1);
		yggr_test_assert(ch1_ref == "a");
		yggr_test_assert(ch1 == "a");

		yggr::swap(ch1_ref, ch2_ref);
		yggr_test_assert(ch1_ref == "b");
		yggr_test_assert(ch1 == "b");

		yggr_test_assert(ch2_ref == "a");
		yggr_test_assert(ch2 == "a");

		yggr::swap(ch1_ref, ch2_ref);

		yggr::swap(ch1, ch3_ref);
		yggr_test_assert(ch1 == "c");
		yggr_test_assert(ch1_ref == "c");

		yggr_test_assert(ch3 == "a");
		yggr_test_assert(ch3_ref == "a");
	}

	// operator&
	{
		char_type ch = "a";
		char_ref_type ch_ref = ch;

		yggr_test_assert(&ch == ch.c_str());
		yggr_test_assert(&ch_ref == ch.c_str());
		yggr_test_assert(&ch == &ch_ref);
	}

	// iterator
	{
		char_type ch = "a";
		char_ref_type ch_ref = ch;

		yggr_test_assert(ch.c_str() == ch_ref.c_str());
		yggr_test_assert(ch.begin() == ch_ref.begin());
		yggr_test_assert(ch.cbegin() == ch_ref.begin());
		yggr_test_assert(ch.begin() == ch_ref.cbegin());

		yggr_test_assert(ch.end() == ch_ref.end());
		yggr_test_assert(ch.cend() == ch_ref.end());
		yggr_test_assert(ch.end() == ch_ref.cend());
	}

	// operator type
	{
		char_type ch = "a";
		char_ref_type ch_ref = ch;

		{
			std::string str = ch;
			yggr_test_assert(str == "a");
			str.clear();
			str = static_cast<std::string>(ch_ref); // gcc need using static_cast
			yggr_test_assert(str == "a");
			yggr_test_assert(str == ch_ref);
			yggr_test_assert(ch_ref == str);
		}

		{
			boost::container::string str = ch;
			yggr_test_assert(str == "a");
			str.clear();
			str = static_cast<boost::container::string>(ch_ref); // gcc need using static_cast
			yggr_test_assert(str == "a");
			yggr_test_assert(str == ch_ref);
			yggr_test_assert(ch_ref == str);
		}
	}

	// size
	{
		char_type ch = "a";
		char_ref_type ch_ref = ch;

		yggr_test_assert(ch_ref.size() == 1);
		yggr_test_assert(ch.size() == ch_ref.size());
		yggr_test_assert(ch.length() == ch_ref.length());
		yggr_test_assert(ch_ref.index() == 0);
	}

	// assign
	{
		// assign(char)
		{
			char_type ch = "a";
			char_ref_type ch_ref = ch;

			ch_ref.assign('b');

			yggr_test_assert(ch == "b");
			yggr_test_assert(ch_ref == "b");

			ch_ref.assign(yggr::charset::make_string_charset_helper('c'));

			yggr_test_assert(ch == "c");
			yggr_test_assert(ch_ref == "c");
		}

		{
			char_type ch = "a";
			char_ref_type ch_ref = ch;

			ch_ref.assign("b");

			yggr_test_assert(ch == "b");
			yggr_test_assert(ch_ref == "b");

			ch_ref.assign(yggr::charset::make_string_charset_helper("c"));

			yggr_test_assert(ch == "c");
			yggr_test_assert(ch_ref == "c");
		}

		{
			char_type ch = "a";
			char_ref_type ch_ref = ch;

			ch_ref.assign("b", 1);

			yggr_test_assert(ch == "b");
			yggr_test_assert(ch_ref == "b");

			ch_ref.assign(yggr::charset::make_string_charset_helper("c"), 1);

			yggr_test_assert(ch == "c");
			yggr_test_assert(ch_ref == "c");
		}

		{
			char_type ch = "a";
			char_ref_type ch_ref = ch;

			ch_ref.assign(std::string("b"));

			yggr_test_assert(ch == "b");
			yggr_test_assert(ch_ref == "b");

			ch_ref.assign(yggr::charset::make_string_charset_helper(std::string("c")));

			yggr_test_assert(ch == "c");
			yggr_test_assert(ch_ref == "c");
		}

		{
			char_type ch = "a";
			char_ref_type ch_ref = ch;

			ch_ref.assign(boost::container::string("b"));

			yggr_test_assert(ch == "b");
			yggr_test_assert(ch_ref == "b");

			ch_ref.assign(yggr::charset::make_string_charset_helper(boost::container::string("c")));

			yggr_test_assert(ch == "c");
			yggr_test_assert(ch_ref == "c");
		}

		{
			char_type ch = "a";
			char_ref_type ch_ref = ch;

			ch_ref.assign(char_type("b"));

			yggr_test_assert(ch == "b");
			yggr_test_assert(ch_ref == "b");

			ch_ref.assign(yggr::charset::make_string_charset_helper(char_type("c")));

			yggr_test_assert(ch == "c");
			yggr_test_assert(ch_ref == "c");
		}

		{
			char_type ch = "a";
			char_type ch2 = "b";

			char_ref_type ch_ref = ch;

			ch_ref.assign(ch.begin(), ch.end(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char_type));

			yggr_test_assert(ch == "a");
			yggr_test_assert(ch_ref == "a");

			ch_ref.assign(ch_ref.begin(), ch_ref.end(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char_type));

			yggr_test_assert(ch == "a");
			yggr_test_assert(ch_ref == "a");

			ch_ref.assign(ch2.begin(), ch2.end(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char_type));

			yggr_test_assert(ch == "b");
			yggr_test_assert(ch_ref == "b");
		}
	}

	// compare
	{
		{
			char_type ch1 = "a";
			char_type ch2 = "b";
			char_type ch3 = "a";

			char_ref_type ch1_ref = ch1;
			char_ref_type ch2_ref = ch2;
			char_ref_type ch3_ref = ch3;

			// ==
			yggr_test_assert(ch1_ref == ch1);
			yggr_test_assert(ch1 == ch1_ref);
			yggr_test_assert(ch1_ref == ch1_ref);

			yggr_test_assert(ch1_ref == ch3);
			yggr_test_assert(ch3 == ch1_ref);
			yggr_test_assert(ch1_ref == ch3_ref);

			// !=
			yggr_test_assert(ch1_ref != ch2);
			yggr_test_assert(ch2 != ch1_ref);
			yggr_test_assert(ch1_ref != ch2_ref);

			// <
			yggr_test_assert(ch1_ref < ch2);
			yggr_test_assert(!(ch2 < ch1_ref));
			yggr_test_assert(ch1_ref < ch2_ref);

			// <=
			yggr_test_assert(ch1_ref <= ch1);
			yggr_test_assert(ch1 <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch1_ref);

			yggr_test_assert(ch1_ref <= ch2);
			yggr_test_assert(!(ch2 <= ch1_ref));
			yggr_test_assert(ch1_ref <= ch2_ref);

			yggr_test_assert(ch1_ref <= ch3);
			yggr_test_assert(ch3 <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch3_ref);

			// >
			yggr_test_assert(ch2_ref > ch1);
			yggr_test_assert(!(ch1 > ch2_ref));
			yggr_test_assert(ch2_ref > ch1_ref);

			// >=
			yggr_test_assert(ch1_ref >= ch1);
			yggr_test_assert(ch1 >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch1_ref);

			yggr_test_assert(ch2_ref >= ch1);
			yggr_test_assert(!(ch1 >= ch2_ref));
			yggr_test_assert(ch2_ref >= ch1_ref);

			yggr_test_assert(ch1_ref >= ch3);
			yggr_test_assert(ch3 >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch3_ref);

		}
	}

#ifndef YGGR_NO_CWCHAR

	// hello world
	{
		char_type ch1 = L"你";
		char_type ch2 = L"好";

		char_ref_type ch1_ref = ch1;
		char_ref_type ch2_ref = ch2;
		yggr_test_assert(ch1_ref == L"你");
		yggr_test_assert(ch2_ref == L"好");

	}

	// construct
	{
		{
			char_type ch = L"你";
			char_ref_type ch_ref(ch);
			yggr_test_assert(ch_ref == L"你");
		}

		{
			char_type ch = L"你";
			char_ref_type ch_ref(ch, ch.begin());
			yggr_test_assert(ch_ref == L"你");
		}

		{
			char_type ch = L"你";
			char_ref_type ch_ref(ch, 0);
			yggr_test_assert(ch_ref == L"你");
		}

#if 0
		{
			char_type ch = L"你";
			char_ref_type ch_ref(ch, 1); // yggr_test_assert(false)
			yggr_test_assert(ch_ref == L"你");
		}
#endif // 0, 1

	}

	// operator=
	{
		char_type ch = L"你";
		char_ref_type ch_ref(ch, 0);
		ch_ref = L'好';
		yggr_test_assert(ch_ref == L"好");
		yggr_test_assert(ch == L"好");

		ch_ref = yggr::charset::make_string_charset_helper(L'吗');
		yggr_test_assert(ch_ref == L"吗");
		yggr_test_assert(ch == L"吗");
	}

	{
		char_type ch = L"你";
		char_ref_type ch_ref(ch, 0);
		ch_ref = L"好";
		yggr_test_assert(ch_ref == L"好");
		yggr_test_assert(ch == L"好");

		ch_ref = yggr::charset::make_string_charset_helper(L"吗");
		yggr_test_assert(ch_ref == L"吗");
		yggr_test_assert(ch == L"吗");
	}

	{
		char_type ch = L"你";
		char_ref_type ch_ref(ch, 0);
		ch_ref = std::wstring(L"好");
		yggr_test_assert(ch_ref == L"好");
		yggr_test_assert(ch == L"好");

		ch_ref = yggr::wstring(L"吗");
		yggr_test_assert(ch_ref == L"吗");
		yggr_test_assert(ch == L"吗");

		ch_ref = char_type(L"d");
		yggr_test_assert(ch_ref == L"d");
		yggr_test_assert(ch == L"d");

		ch_ref = yggr::charset::make_string_charset_helper('e');
		yggr_test_assert(ch_ref == L"e");
		yggr_test_assert(ch == L"e");

		ch_ref = yggr::charset::make_string_charset_helper(L"e");
		yggr_test_assert(ch_ref == L"e");
		yggr_test_assert(ch == L"e");

		ch_ref = yggr::charset::make_string_charset_helper(std::wstring(L"f"));
		yggr_test_assert(ch_ref == L"f");
		yggr_test_assert(ch == L"f");

		ch_ref = yggr::charset::make_string_charset_helper(yggr::wstring(L"g"));
		yggr_test_assert(ch_ref == L"g");
		yggr_test_assert(ch == L"g");

		ch_ref = yggr::charset::make_string_charset_helper(char_type(L"h"));
		yggr_test_assert(ch_ref == L"h");
		yggr_test_assert(ch == L"h");
	}

	{
		char_type ch1 = L"你";
		char_type ch2 = L"好";
		char_type ch3 = L"吗";

		char_ref_type ch1_ref = ch1;
		char_ref_type ch2_ref = ch2;
		char_ref_type ch3_ref = ch3;

		ch1 = ch1;
		yggr_test_assert(ch1 == L"你");

		ch1 = ch2;
		yggr_test_assert(ch2 == L"好");

		ch1 = yggr::charset::make_string_charset_helper(ch3_ref);
		yggr_test_assert(ch1 == L"吗");
	}

	// swap
	{
		char_type ch1 = L"你";
		char_type ch2 = L"好";
		char_type ch3 = L"吗";

		char_ref_type ch1_ref = ch1;
		char_ref_type ch2_ref = ch2;
		char_ref_type ch3_ref = ch3;

		yggr::swap(ch1_ref, ch1);
		yggr_test_assert(ch1_ref == L"你");
		yggr_test_assert(ch1 == L"你");

		yggr::swap(ch1_ref, ch2_ref);
		yggr_test_assert(ch1_ref == L"好");
		yggr_test_assert(ch1 == L"好");

		yggr_test_assert(ch2_ref == L"你");
		yggr_test_assert(ch2 == L"你");

		yggr::swap(ch1_ref, ch2_ref);

		yggr::swap(ch1, ch3_ref);
		yggr_test_assert(ch1 == L"吗");
		yggr_test_assert(ch1_ref == L"吗");

		yggr_test_assert(ch3 == L"你");
		yggr_test_assert(ch3_ref == L"你");
	}

	// operator&
	{
		char_type ch = L"你";
		char_ref_type ch_ref = ch;

		yggr_test_assert(&ch == ch.c_str());
		yggr_test_assert(&ch_ref == ch.c_str());
		yggr_test_assert(&ch == &ch_ref);
	}

	// iterator
	{
		char_type ch = L"你";
		char_ref_type ch_ref = ch;

		yggr_test_assert(ch.c_str() == ch_ref.c_str());
		yggr_test_assert(ch.begin() == ch_ref.begin());
		yggr_test_assert(ch.cbegin() == ch_ref.begin());
		yggr_test_assert(ch.begin() == ch_ref.cbegin());

		yggr_test_assert(ch.end() == ch_ref.end());
		yggr_test_assert(ch.cend() == ch_ref.end());
		yggr_test_assert(ch.end() == ch_ref.cend());
	}

	// operator type
	{
		char_type ch = L"你";
		char_ref_type ch_ref = ch;

		{
			std::wstring str = ch;
			yggr_test_assert(str == L"你");
			str.clear();
			str = static_cast<std::wstring>(ch_ref); // gcc need using static_cast
			yggr_test_assert(str == L"你");
			yggr_test_assert(str == ch_ref);
			yggr_test_assert(ch_ref == str);
		}

		{
			boost::container::wstring str = ch;
			yggr_test_assert(str == L"你");
			str.clear();
			str = static_cast<boost::container::wstring>(ch_ref); // gcc need using static_cast
			yggr_test_assert(str == L"你");
			yggr_test_assert(str == ch_ref);
			yggr_test_assert(ch_ref == str);
		}
	}

	// size
	{
		char_type ch = L"你";
		char_ref_type ch_ref = ch;

		yggr_test_assert(ch_ref.size() == 3);
		yggr_test_assert(ch.size() == ch_ref.size());
		yggr_test_assert(ch.length() == ch_ref.length());
		yggr_test_assert(ch_ref.index() == 0);
	}

	// assign
	{
		// assign(char)
		{
			//int m = '好';
			char_type ch = L"你";
			char_ref_type ch_ref = ch;

#if 0
			// !!!must assert false!!!
			// because  'x'
			//	if the 'x' is multibyte char
			//		the 'x' may be the current file encoding bigendian or littleendian
			//			specific according to cpu platform decision
			// also in GCC wchar_t may be 4 bytes, so there is no way to judge this situation

			ch_ref.assign('好');
			ch_ref.assign('好', "gbk");
#endif // 0, 1

			ch_ref.assign(L'好');

			yggr_test_assert(ch == L"好");
			yggr_test_assert(ch_ref == L"好");

			ch_ref.assign(yggr::charset::make_string_charset_helper(L'吗'));

			yggr_test_assert(ch == L"吗");
			yggr_test_assert(ch_ref == L"吗");
		}

		{
			char_type ch = L"你";
			char_ref_type ch_ref = ch;

			ch_ref.assign(L"好");

			yggr_test_assert(ch == L"好");
			yggr_test_assert(ch_ref == L"好");

			ch_ref.assign(yggr::charset::make_string_charset_helper(L"吗"));

			yggr_test_assert(ch == L"吗");
			yggr_test_assert(ch_ref == L"吗");
		}

		{
			char_type ch = L"你";
			char_ref_type ch_ref = ch;

			ch_ref.assign(L"好", 1);

			yggr_test_assert(ch == L"好");
			yggr_test_assert(ch_ref == L"好");

			ch_ref.assign(yggr::charset::make_string_charset_helper(L"吗"), 1);

			yggr_test_assert(ch == L"吗");
			yggr_test_assert(ch_ref == L"吗");
		}

		{
			char_type ch = L"你";
			char_ref_type ch_ref = ch;

			ch_ref.assign(std::wstring(L"好"));

			yggr_test_assert(ch == L"好");
			yggr_test_assert(ch_ref == L"好");

			ch_ref.assign(yggr::charset::make_string_charset_helper(std::wstring(L"吗")));

			yggr_test_assert(ch == L"吗");
			yggr_test_assert(ch_ref == L"吗");
		}

		{
			char_type ch = L"你";
			char_ref_type ch_ref = ch;

			ch_ref.assign(boost::container::wstring(L"好"));

			yggr_test_assert(ch == L"好");
			yggr_test_assert(ch_ref == L"好");

			ch_ref.assign(yggr::charset::make_string_charset_helper(boost::container::wstring(L"吗")));

			yggr_test_assert(ch == L"吗");
			yggr_test_assert(ch_ref == L"吗");
		}

		{
			char_type ch = L"你";
			char_ref_type ch_ref = ch;

			ch_ref.assign(char_type(L"好"));

			yggr_test_assert(ch == L"好");
			yggr_test_assert(ch_ref == L"好");

			ch_ref.assign(yggr::charset::make_string_charset_helper(char_type(L"吗")));

			yggr_test_assert(ch == L"吗");
			yggr_test_assert(ch_ref == L"吗");
		}

		{
			char_type ch = L"你";
			char_type ch2 = L"好";

			char_ref_type ch_ref = ch;

			ch_ref.assign(ch.begin(), ch.end(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char_type));

			yggr_test_assert(ch == L"你");
			yggr_test_assert(ch_ref == L"你");

			ch_ref.assign(ch_ref.begin(), ch_ref.end(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char_type));

			yggr_test_assert(ch == L"你");
			yggr_test_assert(ch_ref == L"你");

			ch_ref.assign(ch2.begin(), ch2.end(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char_type));

			yggr_test_assert(ch == L"好");
			yggr_test_assert(ch_ref == L"好");
		}
	}

	// compare
	{
		// test stand
		yggr_test_assert(L'你' < L'好');
		yggr_test_assert(char_type(L'你') < char_type(L'好'));

		{
			char_type ch1 = L"你";
			char_type ch2 = L"好";
			char_type ch3 = L"你";

			char_ref_type ch1_ref = ch1;
			char_ref_type ch2_ref = ch2;
			char_ref_type ch3_ref = ch3;

			// ==
			yggr_test_assert(ch1_ref == ch1);
			yggr_test_assert(ch1 == ch1_ref);
			yggr_test_assert(ch1_ref == ch1_ref);

			yggr_test_assert(ch1_ref == ch3);
			yggr_test_assert(ch3 == ch1_ref);
			yggr_test_assert(ch1_ref == ch3_ref);

			// !=
			yggr_test_assert(ch1_ref != ch2);
			yggr_test_assert(ch2 != ch1_ref);
			yggr_test_assert(ch1_ref != ch2_ref);

			// <
			yggr_test_assert(ch1_ref < ch2);
			yggr_test_assert(!(ch2 < ch1_ref));
			yggr_test_assert(ch1_ref < ch2_ref);

			// <=
			yggr_test_assert(ch1_ref <= ch1);
			yggr_test_assert(ch1 <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch1_ref);

			yggr_test_assert(ch1_ref <= ch2);
			yggr_test_assert(!(ch2 <= ch1_ref));
			yggr_test_assert(ch1_ref <= ch2_ref);

			yggr_test_assert(ch1_ref <= ch3);
			yggr_test_assert(ch3 <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch3_ref);

			// >
			yggr_test_assert(ch2_ref > ch1);
			yggr_test_assert(!(ch1 > ch2_ref));
			yggr_test_assert(ch2_ref > ch1_ref);

			// >=
			yggr_test_assert(ch1_ref >= ch1);
			yggr_test_assert(ch1 >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch1_ref);

			yggr_test_assert(ch2_ref >= ch1);
			yggr_test_assert(!(ch1 >= ch2_ref));
			yggr_test_assert(ch2_ref >= ch1_ref);

			yggr_test_assert(ch1_ref >= ch3);
			yggr_test_assert(ch3 >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch3_ref);

		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_utf8_char_reference success" << std::endl;
}

void test_utf8_char_const_reference(void)
{
	typedef yggr::charset::utf8_char char_type;
	typedef yggr::charset::basic_utf8_char_reference<char_type> char_ref_type;
	typedef yggr::charset::basic_utf8_char_const_reference<char_type> char_cref_type;
	typedef char_type::char_view_type char_view_type;

	// hello world
	{
		char_type ch1 = "a";
		char_type ch2 = "b";

		char_cref_type ch1_ref = ch1;
		char_cref_type ch2_ref = ch2;
		yggr_test_assert(ch1_ref == "a");
		yggr_test_assert(ch2_ref == "b");

	}

	// construct
	{
		{
			char_type ch = "a";
			char_cref_type ch_ref(ch);
			yggr_test_assert(ch_ref == "a");
		}

		{
			char_type ch = "a";
			char_cref_type ch_ref(ch, ch.begin());
			yggr_test_assert(ch_ref == "a");
		}

		{
			char_type ch = "a";
			char_cref_type ch_ref(ch, 0);
			yggr_test_assert(ch_ref == "a");
		}

#if 0
		{
			char_type ch = "a";
			char_cref_type ch_ref(ch, 1); // yggr_test_assert(false)
			yggr_test_assert(ch_ref == "a");
		}
#endif // 0, 1

	}


	// operator&
	{
		char_type ch = "a";
		char_cref_type ch_ref = ch;

		yggr_test_assert(&ch == ch.c_str());
		yggr_test_assert(&ch_ref == ch.c_str());
		yggr_test_assert(&ch == &ch_ref);
	}

	// iterator
	{
		char_type ch = "a";
		char_cref_type ch_ref = ch;

		yggr_test_assert(ch.c_str() == ch_ref.c_str());
		yggr_test_assert(ch.begin() == ch_ref.begin());
		yggr_test_assert(ch.cbegin() == ch_ref.begin());
		yggr_test_assert(ch.begin() == ch_ref.cbegin());

		yggr_test_assert(ch.end() == ch_ref.end());
		yggr_test_assert(ch.cend() == ch_ref.end());
		yggr_test_assert(ch.end() == ch_ref.cend());
	}

	// operator type
	{
		char_type ch = "a";
		char_cref_type ch_ref = ch;

		{
			std::string str = ch;
			yggr_test_assert(str == "a");
			str.clear();
			str = static_cast<std::string>(ch_ref); // gcc need using static_cast
			yggr_test_assert(str == "a");
			yggr_test_assert(str == ch_ref);
			yggr_test_assert(ch_ref == str);
		}

		{
			boost::container::string str = ch;
			yggr_test_assert(str == "a");
			str.clear();
			str = static_cast<boost::container::string>(ch_ref); // gcc need using static_cast
			yggr_test_assert(str == "a");
			yggr_test_assert(str == ch_ref);
			yggr_test_assert(ch_ref == str);
		}
	}

	// size
	{
		char_type ch = "a";
		char_cref_type ch_ref = ch;

		yggr_test_assert(ch_ref.size() == 1);
		yggr_test_assert(ch.size() == ch_ref.size());
		yggr_test_assert(ch.length() == ch_ref.length());
		yggr_test_assert(ch_ref.index() == 0);
	}

	// compare
	{
		{
			char_type ch1 = "a";
			char_type ch2 = "b";
			char_type ch3 = "a";

			char_cref_type ch1_ref = ch1;
			char_cref_type ch2_ref = ch2;
			char_cref_type ch3_ref = ch3;

			// ==
			yggr_test_assert(ch1_ref == ch1);
			yggr_test_assert(ch1 == ch1_ref);
			yggr_test_assert(ch1_ref == ch1_ref);

			yggr_test_assert(ch1_ref == ch3);
			yggr_test_assert(ch3 == ch1_ref);
			yggr_test_assert(ch1_ref == ch3_ref);

			// !=
			yggr_test_assert(ch1_ref != ch2);
			yggr_test_assert(ch2 != ch1_ref);
			yggr_test_assert(ch1_ref != ch2_ref);

			// <
			yggr_test_assert(ch1_ref < ch2);
			yggr_test_assert(!(ch2 < ch1_ref));
			yggr_test_assert(ch1_ref < ch2_ref);

			// <=
			yggr_test_assert(ch1_ref <= ch1);
			yggr_test_assert(ch1 <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch1_ref);

			yggr_test_assert(ch1_ref <= ch2);
			yggr_test_assert(!(ch2 <= ch1_ref));
			yggr_test_assert(ch1_ref <= ch2_ref);

			yggr_test_assert(ch1_ref <= ch3);
			yggr_test_assert(ch3 <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch3_ref);

			// >
			yggr_test_assert(ch2_ref > ch1);
			yggr_test_assert(!(ch1 > ch2_ref));
			yggr_test_assert(ch2_ref > ch1_ref);

			// >=
			yggr_test_assert(ch1_ref >= ch1);
			yggr_test_assert(ch1 >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch1_ref);

			yggr_test_assert(ch2_ref >= ch1);
			yggr_test_assert(!(ch1 >= ch2_ref));
			yggr_test_assert(ch2_ref >= ch1_ref);

			yggr_test_assert(ch1_ref >= ch3);
			yggr_test_assert(ch3 >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch3_ref);

		}
	}

#ifndef YGGR_NO_CWCHAR

	// hello world
	{
		char_type ch1 = L"你";
		char_type ch2 = L"好";

		char_cref_type ch1_ref = ch1;
		char_cref_type ch2_ref = ch2;
		yggr_test_assert(ch1_ref == L"你");
		yggr_test_assert(ch2_ref == L"好");

	}

	// construct
	{
		{
			char_type ch = L"你";
			char_cref_type ch_ref(ch);
			yggr_test_assert(ch_ref == L"你");
		}

		{
			char_type ch = L"你";
			char_cref_type ch_ref(ch, ch.begin());
			yggr_test_assert(ch_ref == L"你");
		}

		{
			char_type ch = L"你";
			char_cref_type ch_ref(ch, 0);
			yggr_test_assert(ch_ref == L"你");
		}

#if 0
		{
			char_type ch = L"你";
			char_cref_type ch_ref(ch, 1); // yggr_test_assert(false)
			yggr_test_assert(ch_ref == L"你");
		}
#endif // 0, 1

	}

	// operator&
	{
		char_type ch = L"你";
		char_cref_type ch_ref = ch;

		yggr_test_assert(&ch == ch.c_str());
		yggr_test_assert(&ch_ref == ch.c_str());
		yggr_test_assert(&ch == &ch_ref);
	}

	// iterator
	{
		char_type ch = L"你";
		char_cref_type ch_ref = ch;

		yggr_test_assert(ch.c_str() == ch_ref.c_str());
		yggr_test_assert(ch.begin() == ch_ref.begin());
		yggr_test_assert(ch.cbegin() == ch_ref.begin());
		yggr_test_assert(ch.begin() == ch_ref.cbegin());

		yggr_test_assert(ch.end() == ch_ref.end());
		yggr_test_assert(ch.cend() == ch_ref.end());
		yggr_test_assert(ch.end() == ch_ref.cend());
	}

	// operator type
	{
		char_type ch = L"你";
		char_cref_type ch_ref = ch;

		{
			std::wstring str = ch;
			yggr_test_assert(str == L"你");
			str.clear();
			str = static_cast<std::wstring>(ch_ref); // gcc need using static_cast
			yggr_test_assert(str == L"你");
			yggr_test_assert(str == ch_ref);
			yggr_test_assert(ch_ref == str);
		}

		{
			boost::container::wstring str = ch;
			yggr_test_assert(str == L"你");
			str.clear();
			str = static_cast<boost::container::wstring>(ch_ref); // gcc need using static_cast
			yggr_test_assert(str == L"你");
			yggr_test_assert(str == ch_ref);
			yggr_test_assert(ch_ref == str);
		}
	}

	// size
	{
		char_type ch = L"你";
		char_cref_type ch_ref = ch;

		yggr_test_assert(ch_ref.size() == 3);
		yggr_test_assert(ch.size() == ch_ref.size());
		yggr_test_assert(ch.length() == ch_ref.length());
		yggr_test_assert(ch_ref.index() == 0);
	}

	// compare
	{
		// test stand
		yggr_test_assert(L'你' < L'好');
		yggr_test_assert(char_type(L'你') < char_type(L'好'));

		{
			char_type ch1 = L"你";
			char_type ch2 = L"好";
			char_type ch3 = L"你";

			char_cref_type ch1_ref = ch1;
			char_cref_type ch2_ref = ch2;
			char_cref_type ch3_ref = ch3;

			// ==
			yggr_test_assert(ch1_ref == ch1);
			yggr_test_assert(ch1 == ch1_ref);
			yggr_test_assert(ch1_ref == ch1_ref);

			yggr_test_assert(ch1_ref == ch3);
			yggr_test_assert(ch3 == ch1_ref);
			yggr_test_assert(ch1_ref == ch3_ref);

			// !=
			yggr_test_assert(ch1_ref != ch2);
			yggr_test_assert(ch2 != ch1_ref);
			yggr_test_assert(ch1_ref != ch2_ref);

			// <
			yggr_test_assert(ch1_ref < ch2);
			yggr_test_assert(!(ch2 < ch1_ref));
			yggr_test_assert(ch1_ref < ch2_ref);

			// <=
			yggr_test_assert(ch1_ref <= ch1);
			yggr_test_assert(ch1 <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch1_ref);

			yggr_test_assert(ch1_ref <= ch2);
			yggr_test_assert(!(ch2 <= ch1_ref));
			yggr_test_assert(ch1_ref <= ch2_ref);

			yggr_test_assert(ch1_ref <= ch3);
			yggr_test_assert(ch3 <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch3_ref);

			// >
			yggr_test_assert(ch2_ref > ch1);
			yggr_test_assert(!(ch1 > ch2_ref));
			yggr_test_assert(ch2_ref > ch1_ref);

			// >=
			yggr_test_assert(ch1_ref >= ch1);
			yggr_test_assert(ch1 >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch1_ref);

			yggr_test_assert(ch2_ref >= ch1);
			yggr_test_assert(!(ch1 >= ch2_ref));
			yggr_test_assert(ch2_ref >= ch1_ref);

			yggr_test_assert(ch1_ref >= ch3);
			yggr_test_assert(ch3 >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch3_ref);

		}
	}

	// const_cast
	{
		char_type ch = L"你";
		char_cref_type ch_cref = ch;

		char_ref_type ch_ref = ch_cref.const_cast_<char_ref_type>();
		char_view_type ch_view = ch_cref.const_cast_<char_view_type>();

		yggr_test_assert(ch_ref == ch);
		yggr_test_assert(ch_view == ch);

	}

	{
		char_type ch = L"你";
		char_ref_type ch_ref = ch;

		char_cref_type ch_cref = ch_ref.const_cast_<char_cref_type>();
		char_view_type ch_view = ch_ref.const_cast_<char_view_type>();

		yggr_test_assert(ch_cref == ch);
		yggr_test_assert(ch_view == ch);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_utf8_char_const_reference success" << std::endl;
}

void test_utf8_char_view(void)
{
	typedef yggr::charset::utf8_char char_type;
	typedef yggr::charset::basic_utf8_char_reference<char_type> char_ref_type;
	typedef yggr::charset::basic_utf8_char_const_reference<char_type> char_cref_type;
	typedef yggr::charset::basic_utf8_char_view<char> char_view_type;

	// hello world
	{
		char_type ch1 = "a";
		char_type ch2 = "b";

		char_cref_type ch1_ref = ch1;
		char_cref_type ch2_ref = ch2;

		char_view_type ch1_view = ch1;
		char_view_type ch2_view = ch2;

		yggr_test_assert(ch1_ref == "a");
		yggr_test_assert(ch2_ref == "b");

		yggr_test_assert(ch1_view == "a");
		yggr_test_assert(ch2_view == "b");

	}

	// construct
	{
		{
			char_type ch = "a";
			char_view_type ch_view(ch);
			yggr_test_assert(ch_view == "a");
		}

		{
			char_type ch = "a";
			char_view_type ch_vew(ch.begin());
			yggr_test_assert(ch_vew == "a");
		}

		{
			char_type ch = "a";
			char_view_type ch_view(ch.c_str());
			yggr_test_assert(ch_view == "a");
		}

		{
			char_type ch = "a";
			char_ref_type ch_ref = ch;
			char_cref_type ch_cref = ch_ref;

			char_view_type ch_view1 = ch_ref;
			char_view_type ch_view2 = ch_cref;
			yggr_test_assert(ch_view1 == "a");
			yggr_test_assert(ch_view2 == "a");
		}

	}


	// operator&
	{
		char_type ch = "a";
		char_view_type ch_view = ch;

		yggr_test_assert(&ch == ch.c_str());
		yggr_test_assert(&ch_view == ch.c_str());
		yggr_test_assert(&ch == &ch_view);
	}

	// iterator
	{
		char_type ch = "a";
		char_view_type ch_view = ch;

		yggr_test_assert(ch.c_str() == ch_view.data());
		yggr_test_assert(ch.c_str() == ch_view.begin());
		yggr_test_assert(ch.c_str() == ch_view.cbegin());

		yggr_test_assert(ch.c_str() + ch.size() == ch_view.end());
		yggr_test_assert(ch.c_str() + ch.size() == ch_view.cend());
	}

	// operator type
	{
		char_type ch = "a";
		char_view_type ch_view = ch;

		{
			std::string str = ch;
			yggr_test_assert(str == "a");
			str.clear();
			str = static_cast<std::string>(ch_view); // gcc need using static_cast
			yggr_test_assert(str == "a");
			yggr_test_assert(str == ch_view);
			yggr_test_assert(ch_view == str);
		}

		{
			boost::container::string str = ch;
			yggr_test_assert(str == "a");
			str.clear();
			str = ch_view;
			yggr_test_assert(str == "a");
			yggr_test_assert(str == ch_view);
			yggr_test_assert(ch_view == str);
		}
	}

	// size
	{
		char_type ch = "a";
		char_view_type ch_view = ch;

		yggr_test_assert(ch_view.size() == 1);
		yggr_test_assert(ch.size() == ch_view.size());
		yggr_test_assert(ch.length() == ch_view.length());
	}

	// compare
	{
		{
			char_type ch1 = "a";
			char_type ch2 = "b";
			char_type ch3 = "a";

			char_cref_type ch1_ref = ch1;
			char_cref_type ch2_ref = ch2;
			char_cref_type ch3_ref = ch3;

			char_view_type ch1_view = ch1_ref;
			char_view_type ch2_view = ch2_ref;
			char_view_type ch3_view = ch3_ref;

			// ==
			yggr_test_assert(ch1_ref == ch1_view);
			yggr_test_assert(ch1_view == ch1_ref);
			yggr_test_assert(ch1_ref == ch1_view);

			yggr_test_assert(ch1_ref == ch3_view);
			yggr_test_assert(ch3_view == ch1_ref);
			yggr_test_assert(ch1_ref == ch3_view);

			yggr_test_assert(ch1_view == ch1);
			yggr_test_assert(ch1 == ch1_view);
			yggr_test_assert(ch1_view == ch1_view);

			yggr_test_assert(ch1_view == ch3);
			yggr_test_assert(ch3 == ch1_view);
			yggr_test_assert(ch1_view == ch3_view);

			yggr_test_assert(ch1_view == ch3_view);
			yggr_test_assert(ch3_view == ch1_view);

			// !=
			yggr_test_assert(ch1_ref != ch2_view);
			yggr_test_assert(ch2_view != ch1_ref);
			yggr_test_assert(ch1_ref != ch2_view);

			yggr_test_assert(ch1 != ch2_view);
			yggr_test_assert(ch2_view != ch1);
			yggr_test_assert(ch1 != ch2_view);

			yggr_test_assert(ch1_view != ch2_view);
			yggr_test_assert(ch2_view != ch1_view);
			yggr_test_assert(ch1_view != ch2_view);

			// <
			yggr_test_assert(ch1_view < ch2);
			yggr_test_assert(!(ch2 < ch1_view));
			yggr_test_assert(ch1_view < ch2_ref);

			yggr_test_assert(ch1_ref < ch2_view);
			yggr_test_assert(!(ch2_view < ch1_ref));
			yggr_test_assert(ch1_ref < ch2_view);

			yggr_test_assert(ch1_view < ch2_view);
			yggr_test_assert(!(ch1_view < ch1_view));

			// <=
			yggr_test_assert(ch1_view <= ch1);
			yggr_test_assert(ch1 <= ch1_view);
			yggr_test_assert(ch1_view <= ch1_ref);

			yggr_test_assert(ch1_view <= ch2);
			yggr_test_assert(!(ch2 <= ch1_view));
			yggr_test_assert(ch1_view <= ch2_ref);

			yggr_test_assert(ch1_view <= ch3);
			yggr_test_assert(ch3 <= ch1_view);
			yggr_test_assert(ch1_view <= ch3_ref);

			yggr_test_assert(ch1_ref <= ch1_view);
			yggr_test_assert(ch1_view <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch1_view);

			yggr_test_assert(ch1_ref <= ch2_view);
			yggr_test_assert(!(ch2_view <= ch1_ref));
			yggr_test_assert(ch1_ref <= ch2_view);

			yggr_test_assert(ch1_ref <= ch3_view);
			yggr_test_assert(ch3_view <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch3_view);

			yggr_test_assert(ch1_view <= ch1_view);

			yggr_test_assert(ch1_view <= ch2_view);
			yggr_test_assert(!(ch2_view <= ch1_view));

			yggr_test_assert(ch1_view <= ch3_view);
			yggr_test_assert(ch3_view <= ch1_view);

			// >
			yggr_test_assert(ch2_view > ch1);
			yggr_test_assert(!(ch1 > ch2_view));
			yggr_test_assert(ch2_view > ch1_ref);

			yggr_test_assert(ch2_ref > ch1_view);
			yggr_test_assert(!(ch1_view > ch2_ref));
			yggr_test_assert(ch2_ref > ch1_view);

			yggr_test_assert(ch2_view > ch1_view);
			yggr_test_assert(!(ch1_view > ch2_view));

			// >=
			yggr_test_assert(ch1_view >= ch1);
			yggr_test_assert(ch1 >= ch1_view);
			yggr_test_assert(ch1_view >= ch1_ref);

			yggr_test_assert(ch2_view >= ch1);
			yggr_test_assert(!(ch1 >= ch2_view));
			yggr_test_assert(ch2_view >= ch1_ref);

			yggr_test_assert(ch1_view >= ch3);
			yggr_test_assert(ch3 >= ch1_view);
			yggr_test_assert(ch1_view >= ch3_ref);

			yggr_test_assert(ch1_ref >= ch1_view);
			yggr_test_assert(ch1_view >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch1_view);

			yggr_test_assert(ch2_ref >= ch1_view);
			yggr_test_assert(!(ch1_view >= ch2_ref));
			yggr_test_assert(ch2_ref >= ch1_view);

			yggr_test_assert(ch1_ref >= ch3_view);
			yggr_test_assert(ch3_view >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch3_view);

			yggr_test_assert(ch1_view >= ch1_view);

			yggr_test_assert(ch2_view >= ch1_view);
			yggr_test_assert(!(ch1_view >= ch2_view));

			yggr_test_assert(ch1_view >= ch3_view);
			yggr_test_assert(ch3_view >= ch1_view);
		}
	}



#ifndef YGGR_NO_CWCHAR

	// hello world
	{
		char_type ch1 = L"你";
		char_type ch2 = L"好";

		char_cref_type ch1_ref = ch1;
		char_cref_type ch2_ref = ch2;

		char_view_type ch1_view = ch1_ref;
		char_view_type ch2_view = ch2_ref;

		yggr_test_assert(ch1_ref == L"你");
		yggr_test_assert(ch2_ref == L"好");

		yggr_test_assert(ch1_view == L"你");
		yggr_test_assert(ch2_view == L"好");

	}

	// construct
	{
		{
			char_type ch = L"你";
			char_view_type ch_view(ch);
			yggr_test_assert(ch_view == L"你");
		}

		{
			char_type ch = L"你";
			char_cref_type ch_ref(ch, ch.begin());
			char_view_type ch_view(ch_ref);
			yggr_test_assert(ch_ref == L"你");
			yggr_test_assert(ch_view == L"你");
		}

		{
			char_type ch = L"你";
			char_ref_type ch_ref(ch, 0);
			char_view_type ch_view(ch_ref);
			yggr_test_assert(ch_ref == L"你");
			yggr_test_assert(ch_view == L"你");

			char_view_type ch_view2 = ch_view;

			yggr_test_assert(ch_view2 == L"你");
		}

	}

	// operator&
	{
		char_type ch = L"你";
		char_view_type ch_view = ch;

		yggr_test_assert(&ch == ch_view.data());
		yggr_test_assert(&ch_view == ch.c_str());
		yggr_test_assert(&ch == &ch_view);
	}

	// iterator
	{
		char_type ch = L"你";
		char_cref_type ch_ref = ch;
		char_view_type ch_view = ch_ref;

		yggr_test_assert(ch.c_str() == ch_view.data());
		yggr_test_assert(ch.c_str() == ch_view.begin());


		yggr_test_assert(ch.c_str() + ch.size() == ch_view.end());
		yggr_test_assert(ch.c_str() + ch.size() == ch_view.cend());
	}

	// operator type
	{
		char_type ch = L"你";
		char_view_type ch_view = ch;

		{
			std::wstring str = ch;
			yggr_test_assert(str == L"你");
			str.clear();
			str = static_cast<std::wstring>(ch_view); // gcc need using static_cast
			yggr_test_assert(str == L"你");
			yggr_test_assert(str == ch_view);
			yggr_test_assert(ch_view == str);
		}

		{
			boost::container::wstring str = ch;
			yggr_test_assert(str == L"你");
			str.clear();
			str = static_cast<boost::container::wstring>(ch_view); // gcc need using static_cast
			yggr_test_assert(str == L"你");
			yggr_test_assert(str == ch_view);
			yggr_test_assert(ch_view == str);
		}
	}

	// size
	{
		char_type ch = L"你";
		char_view_type ch_view = ch;

		yggr_test_assert(ch_view.size() == 3);
		yggr_test_assert(ch.size() == ch_view.size());
		yggr_test_assert(ch.length() == ch_view.length());
	}

	// compare
	{
		// test stand
		yggr_test_assert(L'你' < L'好');
		yggr_test_assert(char_type(L'你') < char_type(L'好'));

		{
			char_type ch1 = L"你";
			char_type ch2 = L"好";
			char_type ch3 = L"你";

			char_cref_type ch1_ref = ch1;
			char_cref_type ch2_ref = ch2;
			char_cref_type ch3_ref = ch3;

			char_view_type ch1_view = ch1_ref;
			char_view_type ch2_view = ch2_ref;
			char_view_type ch3_view = ch3_ref;

			// ==
			yggr_test_assert(ch1_view == ch1);
			yggr_test_assert(ch1 == ch1_view);
			yggr_test_assert(ch1_ref == ch1_view);

			yggr_test_assert(ch1_view == ch3);
			yggr_test_assert(ch3 == ch1_view);
			yggr_test_assert(ch1_view == ch3_ref);

			yggr_test_assert(ch1_ref == ch1_view);
			yggr_test_assert(ch1_view == ch1_ref);
			yggr_test_assert(ch1_ref == ch1_view);

			yggr_test_assert(ch1_ref == ch3_view);
			yggr_test_assert(ch3_view == ch1_ref);
			yggr_test_assert(ch1_ref == ch3_view);

			yggr_test_assert(ch1_view == ch1_view);

			yggr_test_assert(ch1_view == ch3_view);
			yggr_test_assert(ch3_view == ch1_view);

			// !=
			yggr_test_assert(ch1_view != ch2);
			yggr_test_assert(ch2 != ch1_view);
			yggr_test_assert(ch1_view != ch2_ref);

			yggr_test_assert(ch1_ref != ch2_view);
			yggr_test_assert(ch2_view != ch1_ref);
			yggr_test_assert(ch1_ref != ch2_view);

			yggr_test_assert(ch1_view != ch2_view);
			yggr_test_assert(ch2_view != ch1_view);

			// <
			yggr_test_assert(ch1_view < ch2);
			yggr_test_assert(!(ch2 < ch1_view));
			yggr_test_assert(ch1_view < ch2_ref);

			yggr_test_assert(ch1_ref < ch2_view);
			yggr_test_assert(!(ch2_view < ch1_ref));
			yggr_test_assert(ch1_ref < ch2_view);

			yggr_test_assert(ch1_view < ch2_view);
			yggr_test_assert(!(ch2_view < ch1_view));

			// <=
			yggr_test_assert(ch1_ref <= ch1);
			yggr_test_assert(ch1 <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch1_ref);

			yggr_test_assert(ch1_ref <= ch2);
			yggr_test_assert(!(ch2 <= ch1_ref));
			yggr_test_assert(ch1_ref <= ch2_ref);

			yggr_test_assert(ch1_ref <= ch3);
			yggr_test_assert(ch3 <= ch1_ref);
			yggr_test_assert(ch1_ref <= ch3_ref);

			yggr_test_assert(ch1_view <= ch1);
			yggr_test_assert(ch1 <= ch1_view);
			yggr_test_assert(ch1_view <= ch1_ref);

			yggr_test_assert(ch1_ref <= ch2_view);
			yggr_test_assert(!(ch2_view <= ch1_ref));
			yggr_test_assert(ch1_ref <= ch2_view);

			yggr_test_assert(ch1_view <= ch3);
			yggr_test_assert(ch3 <= ch1_view);
			yggr_test_assert(ch1_view <= ch3_ref);

			yggr_test_assert(ch1_view <= ch1_view);

			yggr_test_assert(ch1_view <= ch2_view);
			yggr_test_assert(!(ch2_view <= ch1_view));

			yggr_test_assert(ch1_view <= ch3_view);
			yggr_test_assert(ch3_view <= ch1_view);

			// >
			yggr_test_assert(ch2_view > ch1);
			yggr_test_assert(!(ch1 > ch2_view));
			yggr_test_assert(ch2_view > ch1_ref);

			yggr_test_assert(ch2_ref > ch1_view);
			yggr_test_assert(!(ch1_view > ch2_ref));
			yggr_test_assert(ch2_ref > ch1_view);

			yggr_test_assert(ch2_view > ch1_view);
			yggr_test_assert(!(ch1_view > ch2_view));

			// >=
			yggr_test_assert(ch1_view >= ch1);
			yggr_test_assert(ch1 >= ch1_view);
			yggr_test_assert(ch1_view >= ch1_ref);

			yggr_test_assert(ch2_view >= ch1);
			yggr_test_assert(!(ch1 >= ch2_view));
			yggr_test_assert(ch2_view >= ch1_ref);

			yggr_test_assert(ch1_view >= ch3);
			yggr_test_assert(ch3 >= ch1_view);
			yggr_test_assert(ch1_view >= ch3_ref);

			yggr_test_assert(ch1_ref >= ch1_view);
			yggr_test_assert(ch1_view >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch1_view);

			yggr_test_assert(ch2_ref >= ch1_view);
			yggr_test_assert(!(ch1_view >= ch2_ref));
			yggr_test_assert(ch2_ref >= ch1_view);

			yggr_test_assert(ch1_ref >= ch3_view);
			yggr_test_assert(ch3_view >= ch1_ref);
			yggr_test_assert(ch1_ref >= ch3_view);

			yggr_test_assert(ch1_view >= ch1_view);

			yggr_test_assert(ch2_view >= ch1_view);
			yggr_test_assert(!(ch1_view >= ch2_view));

			yggr_test_assert(ch1_view >= ch3_view);
			yggr_test_assert(ch3_view >= ch1_view);
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_utf8_char_view success" << std::endl;
}

int main(int argc, char* argv[])
{
	test_utf8_char_reference();
	test_utf8_char_const_reference();
	test_utf8_char_view();

	wait_any_key(argc, argv);
	return 0;
}
