//utf8_string_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

//#define YGGR_NO_CWCHAR

//#define YGGR_USE_STL_STRING
//#define _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <cassert>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

//#define TEST_IO

#define TEST_CONSTRUCT

#if defined(TEST_CONSTRUCT)

void test_construct(void)
{
	// (n, char)
	{
		{
			// char
			yggr::utf8_string str1(3, 'a');
			yggr_test_assert(str1 == "aaa");

			yggr::utf8_string str2(3, 'a', str1.get_allocator());
			yggr_test_assert(str2 == "aaa");

			yggr::utf8_string str3(3, 'a', yggr::charset::charset_name_t<char>());
			yggr_test_assert(str3 == "aaa");

			yggr::utf8_string str4(3, 'a',
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "aaa");

			yggr::utf8_string str5(3, yggr::charset::make_string_charset_helper('a'));
			yggr_test_assert(str5 == "aaa");

			yggr::utf8_string str6(3, yggr::charset::make_string_charset_helper('a'),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str6 == "aaa");
		}

		{
			// utf8 char
			yggr::utf8_string str1(3, yggr::utf8_string::char_type('a'));
			yggr_test_assert(str1 == "aaa");

			yggr::utf8_string str2(3, yggr::utf8_string::char_type('a'), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "aaa");

			yggr::utf8_string str3(3, yggr::utf8_string::char_type('a'), yggr::charset::charset_name_t<yggr::utf8_string::char_type>());
			yggr_test_assert(str3 == "aaa");

			yggr::utf8_string str4(3, yggr::utf8_string::char_type('a'),
										yggr::utf8_string::allocator_type(),
										yggr::charset::charset_name_t<yggr::utf8_string::char_type>());
			yggr_test_assert(str4 == "aaa");

			yggr::utf8_string str5(3, yggr::charset::make_string_charset_helper(yggr::utf8_string::char_type('a')));
			yggr_test_assert(str5 == "aaa");

			yggr::utf8_string str6(3, yggr::charset::make_string_charset_helper(yggr::utf8_string::char_type('a')),
										yggr::utf8_string::allocator_type());
			yggr_test_assert(str6 == "aaa");
		}

		{
			// const chat*
			yggr::utf8_string str1(3, "a");
			yggr_test_assert(str1 == "aaa");

			yggr::utf8_string str2(3, "a", yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "aaa");

			yggr::utf8_string str3(3, "a", yggr::charset::charset_name_t<char>());
			yggr_test_assert(str3 == "aaa");

			yggr::utf8_string str4(3, "a",
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "aaa");

			yggr::utf8_string str5(3, yggr::charset::make_string_charset_helper("a"));
			yggr_test_assert(str5 == "aaa");

			yggr::utf8_string str6(3, yggr::charset::make_string_charset_helper("a"),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str6 == "aaa");

#		if 0
			// assert false, because "abc" is a string not a char
			yggr::utf8_string str(3, yggr::charset::make_string_charset_helper(std::string("abc")));
#		else
			yggr::utf8_string str(3, yggr::charset::make_string_charset_helper(std::string("a")));
#		endif // 0, 1
			yggr_test_assert(str == "aaa");
		}

		{
			// string
			// const chat*
			yggr::utf8_string str1(3, std::string("a"));
			yggr_test_assert(str1 == "aaa");

			yggr::utf8_string str2(3, std::string("a"), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "aaa");

			yggr::utf8_string str3(3, std::string("a"), yggr::charset::charset_name_t<char>());
			yggr_test_assert(str3 == "aaa");

			yggr::utf8_string str4(3, yggr::string("a"),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "aaa");

			yggr::utf8_string str5(3, yggr::charset::make_string_charset_helper(yggr::string("a")));
			yggr_test_assert(str5 == "aaa");

			yggr::utf8_string str6(3, yggr::charset::make_string_charset_helper(yggr::string("a")),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str6 == "aaa");
		}
	}

	// (string)
	{
		{
			//const char*
			yggr::utf8_string str1 = "abc";
			yggr_test_assert(str1 == "abc");

			yggr::utf8_string str2("abc");
			yggr_test_assert(str2 == "abc");

			yggr::utf8_string str3("abc", yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "abc");

			yggr::utf8_string str4("abc", yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "abc");

			yggr::utf8_string str5("abc",
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str5 == "abc");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper("abc"));
			yggr_test_assert(str6 == "abc");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper("abc"),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "abc");
		}

		{
			//const string&
			yggr::utf8_string str1 = std::string("abc");
			yggr_test_assert(str1 == "abc");

			yggr::utf8_string str2(yggr::string("abc"));
			yggr_test_assert(str2 == "abc");

			yggr::utf8_string str3(std::string("abc"), yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "abc");

			yggr::utf8_string str4(yggr::string("abc"), yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "abc");

			yggr::utf8_string str5(std::string("abc"),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str5 == "abc");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(yggr::string("abc")));
			yggr_test_assert(str6 == "abc");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(std::string("abc")),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "abc");

			yggr::utf8_string str8(yggr::charset::make_string_charset_helper(std::string("abc"), YGGR_STR_UTF8_STRING_CHARSET_NAME()),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str8 == "abc");
		}

		{
			//const utf8_string&
			yggr::utf8_string str1 = yggr::utf8_string("abc");
			yggr_test_assert(str1 == "abc");

			yggr::utf8_string str2(yggr::utf8_string("abc"));
			yggr_test_assert(str2 == "abc");

			yggr::utf8_string str3(yggr::utf8_string("abc"), yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "abc");

			yggr::utf8_string str4(yggr::utf8_string("abc"), yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "abc");

			yggr::utf8_string str5(yggr::utf8_string("abc"),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == "abc");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(yggr::utf8_string("abc")));
			yggr_test_assert(str6 == "abc");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(yggr::utf8_string("abc")),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "abc");
		}

		{
			// const char*, n
			yggr::utf8_string str1("abcde", 3);
			yggr_test_assert(str1 == "abc");

			yggr::utf8_string str3("abcde", 3, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "abc");

			yggr::utf8_string str4("abcde", 3, yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "abc");

			yggr::utf8_string str5("abcde", 3,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str5 == "abc");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper("abcde"), 3);
			yggr_test_assert(str6 == "abc");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper("abcde"), 3, yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "abc");
		}

		{
			// const string pos
			yggr::utf8_string str1(std::string("abcde"), 3);
			yggr_test_assert(str1 == "de");

			yggr::utf8_string str3(yggr::string("abcde"), 3, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "de");

			yggr::utf8_string str4(std::string("abcde"), 3, yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "de");

			yggr::utf8_string str5(yggr::string("abcde"), 3,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str5 == "de");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(std::string("abcde")), 3);
			yggr_test_assert(str6 == "de");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(yggr::string("abcde")), 3,
										yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "de");

			// wrong usage
			try
			{
				yggr::utf8_string str8(std::string("abc���"), 4);
				yggr_test_assert(false);
			}
			catch(const yggr::charset::charset_error_def::error_type&)
			{
				yggr_test_assert(true);
			}
			catch(...)
			{
			    yggr_test_assert(true);
			}
		}

		{
			// const string pos
			yggr::utf8_string str1(yggr::utf8_string("abcde"), 3);
			yggr_test_assert(str1 == "de");

			yggr::utf8_string str3(yggr::utf8_string("abcde"), 3, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "de");

			yggr::utf8_string str4(yggr::utf8_string("abcde"), 3, yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "de");

			yggr::utf8_string str5(yggr::utf8_string("abcde"), 3,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == "de");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(yggr::utf8_string("abcde")), 3);
			yggr_test_assert(str6 == "de");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(yggr::utf8_string("abcde")), 3,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "de");

			// right usage
			yggr::utf8_string str8(yggr::utf8_string("abc���"), 4);
			yggr_test_assert(str8 == "��"); // because utf8_string can correctly recognize utf8_char
		}

		{
			// const string pos, len, using const char*
			yggr::utf8_string str1("abcde", 3, 1);
			yggr_test_assert(str1 == "d");

			yggr::utf8_string str3("abcde", 3, 1, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "d");

			yggr::utf8_string str4("abcde", 3, 1, yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "d");

			yggr::utf8_string str5("abcde", 3, 1,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str5 == "d");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper("abcde"), 3, 1);
			yggr_test_assert(str6 == "d");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper("abcde"), 3, 1,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "d");
		}

		{
			// const string pos, len
			yggr::utf8_string str1(std::string("abcde"), 3, 1);
			yggr_test_assert(str1 == "d");

			yggr::utf8_string str3(yggr::string("abcde"), 3, 1, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "d");

			yggr::utf8_string str4(std::string("abcde"), 3, 1, yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "d");

			yggr::utf8_string str5(yggr::string("abcde"), 3, 1,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str5 == "d");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(yggr::string("abcde")), 3, 1);
			yggr_test_assert(str6 == "d");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(std::string("abcde")), 3, 1,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "d");
		}

		{
			// const utf8_string pos, len
			yggr::utf8_string str1(yggr::utf8_string("abcde"), 3, 1);
			yggr_test_assert(str1 == "d");

			yggr::utf8_string str3(yggr::utf8_string("abcde"), 3, 1, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "d");

			yggr::utf8_string str4(yggr::utf8_string("abcde"), 3, 1, yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "d");

			yggr::utf8_string str5(yggr::utf8_string("abcde"), 3, 1,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == "d");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(yggr::utf8_string("abcde")), 3, 1);
			yggr_test_assert(str6 == "d");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(yggr::utf8_string("abcde")), 3, 1,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "d");

			yggr::utf8_string str8(yggr::utf8_string("abc���"), 4, 1);
			yggr_test_assert(str8 == "��");
		}
	}

	// iterator
	{
		{
			// const char*
			char str_arr[] = {'a', 'b', 'c'};
			yggr::utf8_string str1(str_arr, str_arr + sizeof(str_arr));
			yggr_test_assert(str1 == "abc");

			yggr::utf8_string str2(str_arr, str_arr + sizeof(str_arr), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "abc");

			yggr::utf8_string str3(str_arr, str_arr + sizeof(str_arr), yggr::charset::charset_name_t<char>());
			yggr_test_assert(str3 == "abc");

			yggr::utf8_string str4(str_arr, str_arr + sizeof(str_arr),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "abc");
		}

		{
			// string
			std::string str_arr = "abc";

			yggr::utf8_string str1(str_arr.begin(), str_arr.end());
			yggr_test_assert(str1 == "abc");

			yggr::utf8_string str2(str_arr.begin(), str_arr.end(), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "abc");

			yggr::utf8_string str3(str_arr.begin(), str_arr.end(), yggr::charset::charset_name_t<char>());
			yggr_test_assert(str3 == "abc");

			yggr::utf8_string str4(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "abc");
		}

		{
			// vector
			std::string tmp_str_arr = "abc";
			std::vector<char> str_arr(tmp_str_arr.begin(), tmp_str_arr.end());

			yggr::utf8_string str1(str_arr.begin(), str_arr.end());
			yggr_test_assert(str1 == "abc");

			yggr::utf8_string str2(str_arr.begin(), str_arr.end(), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "abc");

			yggr::utf8_string str3(str_arr.begin(), str_arr.end(), yggr::charset::charset_name_t<char>());
			yggr_test_assert(str3 == "abc");

			yggr::utf8_string str4(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "abc");
		}

		{
			// list
			std::string tmp_str_arr = "abc";
			std::list<char> str_arr(tmp_str_arr.begin(), tmp_str_arr.end());

			yggr::utf8_string str1(str_arr.begin(), str_arr.end());
			yggr_test_assert(str1 == "abc");

			yggr::utf8_string str2(str_arr.begin(), str_arr.end(), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "abc");

			yggr::utf8_string str3(str_arr.begin(), str_arr.end(), yggr::charset::charset_name_t<char>());
			yggr_test_assert(str3 == "abc");

			yggr::utf8_string str4(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "abc");
		}

		{
			// utf8_string
			yggr::utf8_string str_arr = "abc";

			yggr::utf8_string str1(str_arr.begin(), str_arr.end());
			yggr_test_assert(str1 == "abc");

			yggr::utf8_string str2(str_arr.begin(), str_arr.end(), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "abc");

			yggr::utf8_string str3(str_arr.begin(), str_arr.end(), yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str3 == "abc");

			yggr::utf8_string str4(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "abc");
		}

		{
			// utf8_string riter
			yggr::utf8_string str_arr = "abc";

			yggr::utf8_string str1(str_arr.rbegin(), str_arr.rend());
			yggr_test_assert(str1 == "cba");

			yggr::utf8_string str2(str_arr.rbegin(), str_arr.rend(), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "cba");

			yggr::utf8_string str3(str_arr.rbegin(), str_arr.rend(), yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str3 == "cba");

			yggr::utf8_string str4(str_arr.rbegin(), str_arr.rend(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "cba");
		}
	}

	// string_view
	{
		yggr::utf8_string str1(yggr::string_view("abc"));
		yggr_test_assert(str1 == "abc");

		yggr::utf8_string str2(yggr::string_view("abc"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
		yggr_test_assert(str2 == "abc");

		yggr::utf8_string str3(yggr::string_view("abc"), yggr::utf8_string::allocator_type());
		yggr_test_assert(str3 == "abc");

		yggr::utf8_string str4(yggr::string_view("abc"), 
								yggr::utf8_string::allocator_type(), 
								YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
		yggr_test_assert(str3 == "abc");

		yggr::utf8_string str5(yggr::string_view("abc"), 1, 3);
		yggr_test_assert(str5 == "bc");

		yggr::utf8_string str6(yggr::string_view("abc"), 1, 3, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
		yggr_test_assert(str6 == "bc");

		yggr::utf8_string str7(yggr::string_view("abc"), 1, 3,
									yggr::utf8_string::allocator_type(),
									YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
		yggr_test_assert(str7 == "bc");

		yggr::utf8_string str8(yggr::charset::make_string_charset_helper(yggr::string_view("abc")));
		yggr_test_assert(str8 == "abc");

		yggr::utf8_string str9(yggr::charset::make_string_charset_helper(yggr::string_view("abc")),
								yggr::utf8_string::allocator_type());
		yggr_test_assert(str9 == "abc");

		yggr::utf8_string str10(yggr::charset::make_string_charset_helper(yggr::string_view("abc")),
								1, 3);
		yggr_test_assert(str10 == "bc");

		yggr::utf8_string str11(yggr::charset::make_string_charset_helper(yggr::string_view("abc")),
								1, 3, yggr::utf8_string::allocator_type());
		yggr_test_assert(str11 == "bc");
	}

	// utf8_string_view
	{
		yggr::utf8_string str1(yggr::utf8_string_view("abc"));
		yggr_test_assert(str1 == "abc");

		yggr::utf8_string str2(yggr::utf8_string_view("abc"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str2 == "abc");

		yggr::utf8_string str3(yggr::utf8_string_view("abc"), yggr::utf8_string::allocator_type());
		yggr_test_assert(str3 == "abc");

		yggr::utf8_string str4(yggr::utf8_string_view("abc"), 
								yggr::utf8_string::allocator_type(), 
								YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str3 == "abc");

		yggr::utf8_string str5(yggr::utf8_string_view("abc"), 1, 3);
		yggr_test_assert(str5 == "bc");

		yggr::utf8_string str6(yggr::utf8_string_view("abc"), 1, 3, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str6 == "bc");

		yggr::utf8_string str7(yggr::utf8_string_view("abc"), 1, 3,
									yggr::utf8_string::allocator_type(),
									YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str7 == "bc");

		yggr::utf8_string str8(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("abc")));
		yggr_test_assert(str8 == "abc");

		yggr::utf8_string str9(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("abc")),
								yggr::utf8_string::allocator_type());
		yggr_test_assert(str9 == "abc");

		yggr::utf8_string str10(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("abc")),
								1, 3);
		yggr_test_assert(str10 == "bc");

		yggr::utf8_string str11(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("abc")),
								1, 3, yggr::utf8_string::allocator_type());
		yggr_test_assert(str11 == "bc");
	}

#ifndef YGGR_NO_CWCHAR

	// (n, char)
	{
#if defined(_MSC_VER)
		{
			// char
			yggr::utf8_string str1(3, L'��');
			yggr_test_assert(str1 == L"������");

			yggr::utf8_string str2(3, L'��', yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == L"������");

			yggr::utf8_string str3(3, L'��', yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str3 == L"������");

			yggr::utf8_string str4(3, L'��', yggr::utf8_string::allocator_type(), yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == L"������");

			yggr::utf8_string str5(3, yggr::charset::make_string_charset_helper(L'��'));
			yggr_test_assert(str5 == L"������");

			yggr::utf8_string str6(3, yggr::charset::make_string_charset_helper(L'��'),  yggr::utf8_string::allocator_type());
			yggr_test_assert(str6 == L"������");
		}

		{
			// utf8 char
			yggr::utf8_string str1(3, yggr::utf8_string::char_type(L'��'));
			yggr_test_assert(str1 == L"������");

			yggr::utf8_string str2(3, yggr::utf8_string::char_type(L'��'), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == L"������");

			yggr::utf8_string str3(3, yggr::utf8_string::char_type(L'��'),
									yggr::charset::charset_name_t<yggr::utf8_string::char_type>());
			yggr_test_assert(str3 == L"������");

			yggr::utf8_string str4(3, yggr::utf8_string::char_type(L'��'),
										yggr::utf8_string::allocator_type(),
										yggr::charset::charset_name_t<yggr::utf8_string::char_type>());
			yggr_test_assert(str4 == L"������");

			yggr::utf8_string str5(3, yggr::charset::make_string_charset_helper(yggr::utf8_string::char_type(L'��')));
			yggr_test_assert(str5 == L"������");

			yggr::utf8_string str6(3, yggr::charset::make_string_charset_helper(yggr::utf8_string::char_type(L'��')),
										yggr::utf8_string::allocator_type());
			yggr_test_assert(str6 == L"������");
		}

#endif // _MSC_VER

		{
			// const wchat_t*
			yggr::utf8_string str1(3, L"��");
			yggr_test_assert(str1 == L"������");

			yggr::utf8_string str2(3, L"��", yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == L"������");

			yggr::utf8_string str3(3, L"��", yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str3 == L"������");

			yggr::utf8_string str4(3, L"��", yggr::utf8_string::allocator_type(), yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == L"������");

			yggr::utf8_string str5(3, yggr::charset::make_string_charset_helper(L"��"));
			yggr_test_assert(str5 == L"������");

			yggr::utf8_string str6(3, yggr::charset::make_string_charset_helper(L"��"),  yggr::utf8_string::allocator_type());
			yggr_test_assert(str6 == L"������");
		}

		{
			// const wchat_t*
			yggr::utf8_string str1(3, std::wstring(L"��"));
			yggr_test_assert(str1 == L"������");

			yggr::utf8_string str2(3, std::wstring(L"��"), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == L"������");

			yggr::utf8_string str3(3, yggr::wstring(L"��"), yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str3 == L"������");

			yggr::utf8_string str4(3, yggr::wstring(L"��"),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == L"������");

			yggr::utf8_string str5(3, yggr::charset::make_string_charset_helper(std::wstring(L"��")));
			yggr_test_assert(str5 == L"������");

			yggr::utf8_string str6(3, yggr::charset::make_string_charset_helper(yggr::wstring(L"��")),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str6 == L"������");
		}
	}

	// (string)
	{
		{
			//const wchar_t*
			yggr::utf8_string str1 = L"�����";
			yggr_test_assert(str1 == L"�����");

			yggr::utf8_string str2(L"�����");
			yggr_test_assert(str2 == L"�����");

			yggr::utf8_string str3(L"�����", yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"�����");

			if(sizeof(wchar_t) == 2)
			{
				yggr::utf8_string str4(L"�����", yggr::charset::charset_name_t<yggr::s16>());
				yggr_test_assert(str4 == L"�����");
			}
			else
			{
				yggr_test_assert(sizeof(wchar_t) == 4);
				yggr::utf8_string str4(L"�����", yggr::charset::charset_name_t<yggr::s32>());
				yggr_test_assert(str4 == L"�����");
			}

			yggr::utf8_string str5(L"�����",
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str5 == L"�����");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(L"�����"));
			yggr_test_assert(str6 == L"�����");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(L"�����"),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"�����");
		}

		{
			//const wstring&
			yggr::utf8_string str1 = std::wstring(L"�����");
			yggr_test_assert(str1 == L"�����");

			yggr::utf8_string str2(yggr::wstring(L"�����"));
			yggr_test_assert(str2 == L"�����");

			yggr::utf8_string str3(std::wstring(L"�����"), yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"�����");

			yggr::utf8_string str4(yggr::wstring(L"�����"), yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == L"�����");

			yggr::utf8_string str5(std::wstring(L"�����"),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str5 == L"�����");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(yggr::wstring(L"�����")));
			yggr_test_assert(str6 == L"�����");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(std::wstring(L"�����")),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"�����");
		}

		{
			//const utf8_string&
			yggr::utf8_string str1 = yggr::utf8_string(L"�����");
			yggr_test_assert(str1 == L"�����");

			yggr::utf8_string str2(yggr::utf8_string(L"�����"));
			yggr_test_assert(str2 == L"�����");

			yggr::utf8_string str3(yggr::utf8_string(L"�����"), yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"�����");

			yggr::utf8_string str4(yggr::utf8_string(L"�����"), yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == L"�����");

			yggr::utf8_string str5(yggr::utf8_string(L"�����"),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == L"�����");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"�����")));
			yggr_test_assert(str6 == L"�����");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"�����")),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"�����");
		}

		{
			// const char*, n
			yggr::utf8_string str1(L"����𰢴�", 3);
			yggr_test_assert(str1 == L"�����");

			yggr::utf8_string str3(L"����𰢴�", 3, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"�����");

			yggr::utf8_string str4(L"����𰢴�", 3, yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == L"�����");

			yggr::utf8_string str5(L"����𰢴�", 3,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str5 == L"�����");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(L"����𰢴�"), 3);
			yggr_test_assert(str6 == L"�����");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(L"����𰢴�"), 3,
										yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"�����");
		}

		{
			// const string pos
			yggr::utf8_string str1(std::wstring(L"����𰢴�"), 3);
			yggr_test_assert(str1 == L"����");

			yggr::utf8_string str3(yggr::wstring(L"����𰢴�"), 3, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"����");

			yggr::utf8_string str4(std::wstring(L"����𰢴�"), 3, yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == L"����");

			yggr::utf8_string str5(yggr::wstring(L"����𰢴�"), 3,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str5 == L"����");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(std::wstring(L"����𰢴�")), 3);
			yggr_test_assert(str6 == L"����");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(yggr::wstring(L"����𰢴�")), 3,
										yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"����");

		}

		{
			// const string pos
			yggr::utf8_string str1(yggr::utf8_string(L"����𰢴�"), 3);
			yggr_test_assert(str1 == L"����");

			yggr::utf8_string str3(yggr::utf8_string(L"����𰢴�"), 3, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"����");

			yggr::utf8_string str4(yggr::utf8_string(L"����𰢴�"), 3, yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == L"����");

			yggr::utf8_string str5(yggr::utf8_string(L"����𰢴�"), 3,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == L"����");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"����𰢴�")), 3);
			yggr_test_assert(str6 == L"����");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"����𰢴�")), 3,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"����");

		}

		{
			// const string pos, len
			yggr::utf8_string str1(std::wstring(L"����𰢴�"), 3, 1);
			yggr_test_assert(str1 == L"��");

			yggr::utf8_string str3(yggr::wstring(L"����𰢴�"), 3, 1, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"��");

			yggr::utf8_string str4(std::wstring(L"����𰢴�"), 3, 1, yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == L"��");

			yggr::utf8_string str5(yggr::wstring(L"����𰢴�"), 3, 1,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str5 == L"��");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(std::wstring(L"����𰢴�")), 3, 1);
			yggr_test_assert(str6 == L"��");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(yggr::wstring(L"����𰢴�")), 3, 1,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"��");
		}

		{
			// const utf8_string pos, len
			yggr::utf8_string str1(yggr::utf8_string("����𰢴�"), 3, 1);
			yggr_test_assert(str1 == L"��");

			yggr::utf8_string str3(yggr::utf8_string("����𰢴�"), 3, 1, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"��");

			yggr::utf8_string str4(yggr::utf8_string("����𰢴�"), 3, 1, yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == L"��");

			yggr::utf8_string str5(yggr::utf8_string("����𰢴�"), 3, 1,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == L"��");

			yggr::utf8_string str6(yggr::charset::make_string_charset_helper(yggr::utf8_string("����𰢴�")), 3, 1);
			yggr_test_assert(str6 == L"��");

			yggr::utf8_string str7(yggr::charset::make_string_charset_helper(yggr::utf8_string("����𰢴�")), 3, 1,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"��");
		}
	}

	// iterator
	{
		{
			// const wchar*
			wchar_t str_arr[] = {L'��', L'��', L'��'};
			yggr::utf8_string str1(str_arr, str_arr + sizeof(str_arr) / sizeof(wchar_t));
			yggr_test_assert(str1 == "�����");

			yggr::utf8_string str2(str_arr, str_arr + sizeof(str_arr) / sizeof(wchar_t), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "�����");

			yggr::utf8_string str3(str_arr, str_arr + sizeof(str_arr) / sizeof(wchar_t), yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str3 == "�����");

			yggr::utf8_string str4(str_arr, str_arr + sizeof(str_arr) / sizeof(wchar_t),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == "�����");
		}

		{
			// vector
			std::wstring tmp_str_arr = L"�����";
			std::vector<wchar_t> str_arr(tmp_str_arr.begin(), tmp_str_arr.end());

			yggr::utf8_string str1(str_arr.begin(), str_arr.end());
			yggr_test_assert(str1 == "�����");

			yggr::utf8_string str2(str_arr.begin(), str_arr.end(), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "�����");

			yggr::utf8_string str3(str_arr.begin(), str_arr.end(), yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str3 == "�����");

			yggr::utf8_string str4(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == "�����");
		}

		{
			// list
			std::wstring tmp_str_arr = L"�����";
			std::list<wchar_t> str_arr(tmp_str_arr.begin(), tmp_str_arr.end());

			yggr::utf8_string str1(str_arr.begin(), str_arr.end());
			yggr_test_assert(str1 == "�����");

			yggr::utf8_string str2(str_arr.begin(), str_arr.end(), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "�����");

			yggr::utf8_string str3(str_arr.begin(), str_arr.end(), yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str3 == "�����");

			yggr::utf8_string str4(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == "�����");
		}

		{
			// string
			std::wstring str_arr = L"�����";

			yggr::utf8_string str1(str_arr.begin(), str_arr.end());
			yggr_test_assert(str1 == "�����");

			yggr::utf8_string str2(str_arr.begin(), str_arr.end(), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "�����");

			yggr::utf8_string str3(str_arr.begin(), str_arr.end(), yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str3 == "�����");

			yggr::utf8_string str4(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<wchar_t>());
			yggr_test_assert(str4 == "�����");
		}

		{
			// utf8_string
			yggr::utf8_string str_arr = L"�����";

			yggr::utf8_string str1(str_arr.begin(), str_arr.end());
			yggr_test_assert(str1 == L"�����");

			yggr::utf8_string str2(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == L"�����");

			yggr::utf8_string str3(str_arr.begin(), str_arr.end(),
										yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str3 == "�����");

			yggr::utf8_string str4(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "�����");
		}

		{
			// utf8_string riter
			yggr::utf8_string str_arr = L"��hao��";

			yggr::utf8_string str1(str_arr.rbegin(), str_arr.rend());
			yggr_test_assert(str1 == L"��oah��");

			yggr::utf8_string str2(str_arr.rbegin(), str_arr.rend(),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == L"��oah��");

			yggr::utf8_string str3(str_arr.rbegin(), str_arr.rend(),
										yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str3 == "��oah��");

			yggr::utf8_string str4(str_arr.rbegin(), str_arr.rend(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "��oah��");
		}
	}

	// string_view
	{
		yggr::utf8_string str1(yggr::wstring_view(L"�����"));
		yggr_test_assert(str1 == L"�����");

		yggr::utf8_string str2(yggr::wstring_view(L"�����"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
		yggr_test_assert(str2 == L"�����");

		yggr::utf8_string str3(yggr::wstring_view(L"�����"), yggr::utf8_string::allocator_type());
		yggr_test_assert(str3 == L"�����");

		yggr::utf8_string str4(yggr::wstring_view(L"�����"), 
								yggr::utf8_string::allocator_type(), 
								YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
		yggr_test_assert(str3 == L"�����");

		yggr::utf8_string str5(yggr::wstring_view(L"�����"), 1, 3);
		yggr_test_assert(str5 == L"����");

		yggr::utf8_string str6(yggr::wstring_view(L"�����"), 1, 3, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
		yggr_test_assert(str6 == L"����");

		yggr::utf8_string str7(yggr::wstring_view(L"�����"), 1, 3,
									yggr::utf8_string::allocator_type(),
									YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
		yggr_test_assert(str7 == L"����");

		yggr::utf8_string str8(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"�����")));
		yggr_test_assert(str8 == L"�����");

		yggr::utf8_string str9(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"�����")),
								yggr::utf8_string::allocator_type());
		yggr_test_assert(str9 == L"�����");

		yggr::utf8_string str10(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"�����")),
								1, 3);
		yggr_test_assert(str10 == L"����");

		yggr::utf8_string str11(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"�����")),
								1, 3, yggr::utf8_string::allocator_type());
		yggr_test_assert(str11 == L"����");
	}

	// utf8_string_view
	{
		yggr::utf8_string org_str = L"�����";
		yggr::utf8_string_view org_str_view = org_str;

		yggr::utf8_string str1(org_str_view);
		yggr_test_assert(str1 == L"�����");

		yggr::utf8_string str2(org_str_view, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str2 == L"�����");

		yggr::utf8_string str3(org_str_view, yggr::utf8_string::allocator_type());
		yggr_test_assert(str3 == L"�����");

		yggr::utf8_string str4(org_str_view, 
								yggr::utf8_string::allocator_type(), 
								YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str3 == L"�����");

		yggr::utf8_string str5(org_str_view, 1, 3);
		yggr_test_assert(str5 == L"����");

		yggr::utf8_string str6(org_str_view, 1, 3, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str6 == L"����");

		yggr::utf8_string str7(org_str_view, 1, 3,
									yggr::utf8_string::allocator_type(),
									YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str7 == L"����");

		yggr::utf8_string str8(yggr::charset::make_string_charset_helper(org_str_view));
		yggr_test_assert(str8 == L"�����");

		yggr::utf8_string str9(yggr::charset::make_string_charset_helper(org_str_view),
								yggr::utf8_string::allocator_type());
		yggr_test_assert(str9 == L"�����");

		yggr::utf8_string str10(yggr::charset::make_string_charset_helper(org_str_view),
								1, 3);
		yggr_test_assert(str10 == L"����");

		yggr::utf8_string str11(yggr::charset::make_string_charset_helper(org_str_view),
								1, 3, yggr::utf8_string::allocator_type());
		yggr_test_assert(str11 == L"����");
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_construct successed" << std::endl;
}

#endif // TEST_CONSTRUCT

void test_operator_set(void)
{
	// char
	{
		// = const char*
		{
			yggr::utf8_string str;
			str = "abc";
			yggr_test_assert(str == "abc");
		}

		// = BasicString
		// srd::string
		{
			yggr::utf8_string str;
			str = std::string("abc");
			yggr_test_assert(str == "abc");
		}

		// = boost::container::string
		{
			yggr::utf8_string str;
			str = boost::container::string("abc");
			yggr_test_assert(str == "abc");
		}

		// = yggr::string
		{
			yggr::utf8_string str;
			str = yggr::string("abc");
			yggr_test_assert(str == "abc");
		}
	}

	//wchar_t
#ifndef YGGR_NO_CWCHAR
	{
		// = const wchar_t*
		{
			yggr::utf8_string str;
			str = L"abc";
			yggr_test_assert(str == L"abc");
		}

		// = BasicString
		// = std::wstring
		{
			yggr::utf8_string str;
			str = std::wstring(L"abc");
			yggr_test_assert(str == L"abc");
		}

		// = boost::container::wstring
		{
			yggr::utf8_string str;
			str = boost::container::wstring(L"abc");
			yggr_test_assert(str == L"abc");
		}

		// = yggr::wstring
		{
			yggr::utf8_string str;
			str = yggr::wstring(L"abc");
			yggr_test_assert(str == L"abc");
		}
	}
#endif // YGGR_NO_CWCHAR

	// = yggr::utf8_string
	{
		yggr::utf8_string src_str("abc");
		yggr::utf8_string str;
		str = boost::move(src_str);
		yggr_test_assert(str == "abc");
	}

	{
		yggr::utf8_string str;
		str = yggr::utf8_string("abc");
		yggr_test_assert(str == "abc");
	}

	{
		yggr::utf8_string str;
		str = yggr::charset::make_string_charset_helper(yggr::utf8_string("abc"));
		yggr_test_assert(str == "abc");
	}

	std::cout << "test_operator_set successed" << std::endl;
}

void test_swap(void)
{
	// char
	
	{
		// BasicString
		// std::string
		{
			std::string str1("abc");
			yggr::utf8_string str("def");

			str.swap(boost::move(str1));

			yggr_test_assert(str == "abc");
			yggr_test_assert(str1 == "def");

			std::swap(str, boost::move(str1));

			yggr_test_assert(str == "def");
			yggr_test_assert(str1 == "abc");
		}

		{
			std::string str1("abc");
			yggr::utf8_string str("def");

			str.swap(str1);

			yggr_test_assert(str == "abc");
			yggr_test_assert(str1 == "def");

			std::swap(str, str1);

			yggr_test_assert(str == "def");
			yggr_test_assert(str1 == "abc");
		}

		// boost::container::string
		{
			boost::container::string str1("abc");
			yggr::utf8_string str("def");

			str.swap(boost::move(str1));

			yggr_test_assert(str == "abc");
			yggr_test_assert(str1 == "def");

			boost::swap(str, boost::move(str1));

			yggr_test_assert(str == "def");
			yggr_test_assert(str1 == "abc");
		}

		{
			boost::container::string str1("abc");
			yggr::utf8_string str("def");

			str.swap(str1);

			yggr_test_assert(str == "abc");
			yggr_test_assert(str1 == "def");

			boost::swap(str, str1);

			yggr_test_assert(str == "def");
			yggr_test_assert(str1 == "abc");
		}

		// yggr::string
		{
			yggr::string str1("abc");
			yggr::utf8_string str("def");

			str.swap(boost::move(str1));

			yggr_test_assert(str == "abc");
			yggr_test_assert(str1 == "def");

			yggr::swap(str, boost::move(str1));

			yggr_test_assert(str == "def");
			yggr_test_assert(str1 == "abc");
		}

		{
			yggr::string str1("abc");
			yggr::utf8_string str("def");

			str.swap(str1);

			yggr_test_assert(str == "abc");
			yggr_test_assert(str1 == "def");

			yggr::swap(str, str1);

			yggr_test_assert(str == "def");
			yggr_test_assert(str1 == "abc");
		}
	}

	//wchar_t
#ifndef YGGR_NO_CWCHAR
	{
		// BasicString
		// std::wstring
		{
			std::wstring str1(L"abc");
			yggr::utf8_string str(L"def");

			str.swap(boost::move(str1));

			yggr_test_assert(str == L"abc");
			yggr_test_assert(str1 == L"def");
		}

		{
			std::wstring str1(L"abc");
			yggr::utf8_string str(L"def");

			str.swap(str1);

			yggr_test_assert(str == L"abc");
			yggr_test_assert(str1 == L"def");
		}

		// boost::container::wstring
		{
			boost::container::wstring str1(L"abc");
			yggr::utf8_string str(L"def");

			str.swap(boost::move(str1));

			yggr_test_assert(str == L"abc");
			yggr_test_assert(str1 == L"def");
		}

		{
			boost::container::wstring str1(L"abc");
			yggr::utf8_string str(L"def");

			str.swap(str1);

			yggr_test_assert(str == L"abc");
			yggr_test_assert(str1 == L"def");
		}

		// yggr::wstring
		{
			yggr::wstring str1(L"abc");
			yggr::utf8_string str(L"def");

			str.swap(boost::move(str1));

			yggr_test_assert(str == L"abc");
			yggr_test_assert(str1 == L"def");
		}

		{
			yggr::wstring str1(L"abc");
			yggr::utf8_string str(L"def");

			str.swap(str1);

			yggr_test_assert(str == L"abc");
			yggr_test_assert(str1 == L"def");
		}
	}
#endif // YGGR_NO_CWCHAR

	{
		yggr::utf8_string str1("abc");
		yggr::utf8_string str("def");

		str.swap(boost::move(str1));

		yggr_test_assert(str == "abc");
		yggr_test_assert(str1 == "def");
	}

	{
		yggr::utf8_string str1("abc");
		yggr::utf8_string str("def");

		str.swap(str1);

		yggr_test_assert(str == "abc");
		yggr_test_assert(str1 == "def");
	}

	{
	    typedef yggr::charset::string_charset_helper<yggr::utf8_string&, const yggr::string&> string_charset_helper_type;
		yggr::utf8_string str1("abc");
		yggr::utf8_string str("def");
		string_charset_helper_type h_str1 = yggr::charset::make_string_charset_helper(str1);

		str.swap(h_str1);

		yggr_test_assert(str == "abc");
		yggr_test_assert(str1 == "def");
	}

	std::cout << "test_swap successed" << std::endl;
}

void test_append(void)
{
	// n, char
	{
		// n char
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, 'a');

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��caaa");
		}

		// n const char*
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

#		if 0
			 assert false "aa" is a string not a char
			str.append(3, "aa");
#		else
			str.append(3, "a");
#		endif // 0, 1
			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��caaa");
		}

		// n, string
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, std::string("a"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��caaa");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::string("a"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��caaa");
		}

		// n, utf8_string
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::utf8_char("a"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��caaa");
		}

		// string_charset_name
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper('a'));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��caaa");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper("a"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��caaa");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper(std::string("a")));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��caaa");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper(yggr::utf8_string::char_type("a")));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��caaa");
		}
	}

	// string
	{
		// const char*
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append("world");

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == "a��b��cworld");
		}

		//  const char*, n
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append("world", 3);

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��cwor");
		}

		// string
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(std::string("world"));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == "a��b��cworld");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(std::string("world"), YGGR_STR_UTF8_STRING_CHARSET_NAME());

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == "a��b��cworld");
		}

		// string, pos
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::string("world"), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == "a��b��cld");
		}

		// const char*, pos, len
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append("world", 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "a��b��cl");
		}

		// string, pos, len
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(std::string("world"), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "a��b��cl");
		}

		// utf8_string
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append( yggr::utf8_string("world"));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == "a��b��cworld");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append( yggr::utf8_string("world"), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == "a��b��cld");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append( yggr::utf8_string("world"), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "a��b��cl");
		}

		// string_view
		{
			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::string_view("world"));
				yggr_test_assert(str == "a��b��cworld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::string_view("world"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
				yggr_test_assert(str == "a��b��cworld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::charset::make_string_charset_helper(yggr::string_view("world")));
				yggr_test_assert(str == "a��b��cworld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::string_view("world"), 1);
				yggr_test_assert(str == "a��b��corld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::string_view("world"), 1, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
				yggr_test_assert(str == "a��b��corld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::charset::make_string_charset_helper(yggr::string_view("world")), 1);
				yggr_test_assert(str == "a��b��corld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::string_view("world"), 1, 2);
				yggr_test_assert(str == "a��b��cor");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::string_view("world"), 1, 2, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
				yggr_test_assert(str == "a��b��cor");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::charset::make_string_charset_helper(yggr::string_view("world")), 1, 2);
				yggr_test_assert(str == "a��b��cor");
			}
		}

		// utf8_string_view
		{
			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::utf8_string_view("world"));
				yggr_test_assert(str == "a��b��cworld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::utf8_string_view("world"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
				yggr_test_assert(str == "a��b��cworld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("world")));
				yggr_test_assert(str == "a��b��cworld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::utf8_string_view("world"), 1);
				yggr_test_assert(str == "a��b��corld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::utf8_string_view("world"), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME());
				yggr_test_assert(str == "a��b��corld");
			}

			{
				yggr::utf8_string str = "a��b��c";
				str.append(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("world")), 1);
				yggr_test_assert(str == "a��b��corld");
			}
		}

		// string_charset_helper
		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper("world"));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == "a��b��cworld");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper("world"), 3);

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == "a��b��cwor");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(std::string("world")));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == "a��b��cworld");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(yggr::string("world")), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == "a��b��cld");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(std::string("world")), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "a��b��cl");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(yggr::utf8_string("world")));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == "a��b��cworld");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(yggr::utf8_string("world")), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == "a��b��cld");
		}

		{
			yggr::utf8_string str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(yggr::utf8_string("world")), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "a��b��cl");
		}

	}

	// append(iters, itere)
	{
		{
			std::string ins_str = " world";
			yggr::utf8_string str = "hello";

			str.append(ins_str.begin(), ins_str.end());
			yggr_test_assert(str.length() == 11);
			yggr_test_assert(str == "hello world");
		}

		{
			yggr::string ins_str = " world";
			yggr::utf8_string str = "hello";

			str.append(ins_str.rbegin(), ins_str.rend());
			yggr_test_assert(str.length() == 11);
			yggr_test_assert(str == "hellodlrow ");
		}

		{
			yggr::utf8_string ins_str = " world";
			yggr::utf8_string str = "hello";

			str.append(ins_str.rbegin(), ins_str.rend());
			yggr_test_assert(str.length() == 11);
			yggr_test_assert(str == "hellodlrow ");
		}
	}

#ifndef YGGR_NO_CWCHAR
	// n, char
	{
		// n char
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, L'��');

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c����");
		}

		// n const char*
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

#		if 0
			// assert false "aa" is a string not a char
			str.append(3, "aa");
#		else
			str.append(3, L"��");
#		endif // 0, 1
			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c����");
		}

		// n string
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, std::wstring(L"��"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c����");
		}

		// n utf8_string
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::utf8_char("��"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c����");
		}

		// string_charset_name
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper(L'��'));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c����");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper(L"��"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c����");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper(std::wstring(L"��")));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c����");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper(yggr::utf8_string::char_type(L"��")));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c����");
		}
	}

	// string
	{
		// const char*
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(L"���`��m");

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == L"a��b��c���`��m");
		}

		 // const char*, n
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(L"���`��m", 3);

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c���`");
		}

		// string
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(std::wstring(L"���`��m"));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == L"a��b��c���`��m");
		}

		// string, pos
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::wstring(L"���`��m"), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"a��b��c��m");
		}

		// const Char*, pos, len
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(L"���`��m", 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == L"a��b��c��");
		}

		// string, pos, len
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(std::wstring(L"���`��m"), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == L"a��b��c��");
		}

		// utf8_string
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append( yggr::utf8_string(L"���`��m"));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == L"a��b��c���`��m");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append( yggr::utf8_string(L"���`��m"), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"a��b��c��m");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append( yggr::utf8_string(L"���`��m"), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == L"a��b��c��");
		}

		// string_charset_helper
		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(L"���`��m"));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == L"a��b��c���`��m");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(L"���`��m"), 3);

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c���`");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(std::wstring(L"���`��m")));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == L"a��b��c���`��m");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(yggr::wstring(L"���`��m")), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"a��b��c��m");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(std::wstring(L"���`��m")), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == L"a��b��c��");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"���`��m")));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == L"a��b��c���`��m");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"���`��m")), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"a��b��c��m");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"���`��m")), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == L"a��b��c��");
		}

	}

	// append(iters, itere)
	{
		{
			std::wstring ins_str = L" ���`��";
			yggr::utf8_string str = L"���";

			str.append(ins_str.begin(), ins_str.end());
			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"��� ���`��");
		}

		{
			yggr::wstring ins_str = L" ���`��";
			yggr::utf8_string str = L"���";

			str.append(ins_str.rbegin(), ins_str.rend());
			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"��åɩ`��� ");
		}

		{
			yggr::utf8_string ins_str = L" ���`��";
			yggr::utf8_string str = L"���";

			str.append(ins_str.rbegin(), ins_str.rend());
			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"��åɩ`��� ");
		}
	}

	// string_view
	{
		{
			yggr::utf8_string str = L"a��b��c";
			str.append(yggr::wstring_view(L"world"));
			yggr_test_assert(str == L"a��b��cworld");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			str.append(yggr::wstring_view(L"world"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
			yggr_test_assert(str == L"a��b��cworld");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			str.append(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"world")));
			yggr_test_assert(str == L"a��b��cworld");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			str.append(yggr::wstring_view(L"world"), 1);
			yggr_test_assert(str == L"a��b��corld");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			str.append(yggr::wstring_view(L"world"), 1, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
			std::cout << str << std::endl;
			yggr_test_assert(str == L"a��b��corld");
		}

		{
			yggr::utf8_string str = L"a��b��c";
			str.append(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"world")), 1);
			yggr_test_assert(str == L"a��b��corld");
		}
	}

#endif // YGGR_NO_CWCHAR
	std::cout << "test_append sucessed" << std::endl;
}

void test_push_back(void)
{
	{
		yggr::utf8_string str("abc");
		str.push_back('d');
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		yggr::utf8_string str("abc");
		str.push_back("d");
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		yggr::utf8_string str("abc");
		str.push_back(std::string("d"));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		yggr::utf8_string str("abc");
		str.push_back(yggr::utf8_char("d"));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		yggr::utf8_string str("abc");
		str.push_back(yggr::charset::make_string_charset_helper('d'));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		yggr::utf8_string str("abc");
		str.push_back(yggr::charset::make_string_charset_helper("d"));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		yggr::utf8_string str("abc");
		str.push_back(yggr::charset::make_string_charset_helper(std::string("d")));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		yggr::utf8_string str("abc");
		str.push_back(yggr::charset::make_string_charset_helper(yggr::utf8_char("d")));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

#ifndef YGGR_NO_CWCHAR
	{
		// hello world ja
		yggr::utf8_string str(L"�ϥ�`���`");
		str.push_back(L'��');
		yggr_test_assert(str.length() == 7);
		yggr_test_assert(str == L"�ϥ�`���`��");
	}

	{
		yggr::utf8_string str(L"�ϥ�`���`");
		str.push_back(L"��");
		yggr_test_assert(str.length() == 7);
		yggr_test_assert(str == L"�ϥ�`���`��");
	}

	{
		yggr::utf8_string str(L"�ϥ�`���`");
		str.push_back(std::wstring(L"��"));
		yggr_test_assert(str.length() == 7);
		yggr_test_assert(str == L"�ϥ�`���`��");
	}

	{
		yggr::utf8_string str(L"�ϥ�`���`");
		str.push_back(yggr::utf8_char(L"��"));
		yggr_test_assert(str.length() == 7);
		yggr_test_assert(str == L"�ϥ�`���`��");
	}

	{
		yggr::utf8_string str(L"�ϥ�`���`");
		str.push_back(yggr::charset::make_string_charset_helper(L'��'));
		yggr_test_assert(str.length() == 7);
		yggr_test_assert(str == L"�ϥ�`���`��");
	}

	{
		yggr::utf8_string str(L"�ϥ�`���`");
		str.push_back(yggr::charset::make_string_charset_helper(L"��"));
		yggr_test_assert(str.length() == 7);
		yggr_test_assert(str == L"�ϥ�`���`��");
	}

	{
		yggr::utf8_string str(L"�ϥ�`���`");
		str.push_back(yggr::charset::make_string_charset_helper(std::wstring(L"��")));
		yggr_test_assert(str.length() == 7);
		yggr_test_assert(str == L"�ϥ�`���`��");
	}

	{
		yggr::utf8_string str("�ϥ�`���`");
		str.push_back(yggr::charset::make_string_charset_helper(yggr::utf8_char("��")));
		yggr_test_assert(str.length() == 7);
		yggr_test_assert(str == "�ϥ�`���`��");
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_push_back" << std::endl;
}

void test_pop_back(void)
{
	{
		yggr::utf8_string str("abc");
		str.pop_back();
		yggr_test_assert(str.length() == 2);
		yggr_test_assert(str == "ab");

		str.pop_back();
		yggr_test_assert(str.length() == 1);
		yggr_test_assert(str == "a");

		str.pop_back();
		yggr_test_assert(str.length() == 0);
		yggr_test_assert(str == "");

		str.pop_back();
		yggr_test_assert(str.length() == 0);
		yggr_test_assert(str == "");
	}

#ifndef YGGR_NO_CWCHAR

	{
		yggr::utf8_string str(L"�����");
		str.pop_back();
		yggr_test_assert(str.length() == 2);
		yggr_test_assert(str == L"���");

		str.pop_back();
		yggr_test_assert(str.length() == 1);
		yggr_test_assert(str == L"��");

		str.pop_back();
		yggr_test_assert(str.length() == 0);
		yggr_test_assert(str == L"");

		str.pop_back();
		yggr_test_assert(str.length() == 0);
		yggr_test_assert(str == L"");
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_pop_back sucessed" << std::endl;

}

void test_insert(void)
{
	// insert(pos, n, char)
	{
		{
			yggr::utf8_string str = "abc";
			str.insert(0, 2, 'q');
			yggr_test_assert(str == "qqabc");
			str.insert(1, 2, 'k');
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper('t'));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(0, 2, "q");
			yggr_test_assert(str == "qqabc");
			str.insert(1, 2, "k");
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper("t"));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(0, 2, std::string("q"));
			yggr_test_assert(str == "qqabc");
			str.insert(1, 2, yggr::string("k"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper(std::string("t")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(0, 2, yggr::utf8_char("q"));
			yggr_test_assert(str == "qqabc");
			str.insert(1, 2, yggr::utf8_char("k"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper(yggr::utf8_char("t")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(iter_p, n, char)
	{
		{
			yggr::utf8_string str = "abc";
			str.insert(str.begin(), 2, 'q');
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1, 2, 'k');
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper('t'));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = "abc";
#		if 0
			str.insert(str.begin(), 1, "qq"); // assert false "qq" is a string not a char
#		else
			str.insert(str.begin(), 2, "q");
#		endif // 0, 1
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1, 2, "k");
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper("t"));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(str.begin(), 2, std::string("q"));
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1, 2, yggr::string("k"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper(std::string("t")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(str.begin(), 2, yggr::utf8_char("q"));
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1, 2, yggr::utf8_char("k"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper(yggr::utf8_char("t")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(pos, const char*)
	{

		{
			yggr::utf8_string str = "abc";
			str.insert(0, "qq");
			yggr_test_assert(str == "qqabc");
			str.insert(1,  "kk");
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper("ttt"));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(0, std::string("qq"));
			yggr_test_assert(str == "qqabc");
			str.insert(1, yggr::string("kk"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(std::string("ttt")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(0, std::string("qq"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str == "qqabc");
			str.insert(1, yggr::string("kk"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(std::string("ttt"), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = "abc";
#		if 0
			str.insert(0, yggr::utf8_char("q")); // mpl_assert(false)
#		else
			str.insert(0, yggr::utf8_string("qq"));
#		endif // 0, 1
			yggr_test_assert(str == "qqabc");
			str.insert(1, yggr::utf8_string("kk"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(yggr::utf8_string("ttt")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(pos, const char*, n)
	{

		{
			yggr::utf8_string str = "abc";
			str.insert(0, "qqnil", 2);
			yggr_test_assert(str == "qqabc");
			str.insert(1,  "kknil", 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper("tttnil"), 3);
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);

			str.insert(str.length(), str);
			yggr_test_assert(str == "qkkqabctttqkkqabcttt");
			yggr_test_assert(str.length() == 20);
		}
	}

	// insert(pos, string, subpos, sublen)
	{

		{
			yggr::utf8_string str = "abc";
			str.insert(0, "qqkkttt", 0, 2);
			yggr_test_assert(str == "qqabc");
			str.insert(1,  "qqkkttt", 2, 2);
			yggr_test_assert(str == "qkkqabc");
			// this insert is not substring insert
			str.insert(str.length(), yggr::charset::make_string_charset_helper("qqkkttt"), 4);
			yggr_test_assert(str == "qkkqabcqqkk");
			yggr_test_assert(str.length() == 11);
		}

		{
			std::string stl_ins_str = "qqkkttt";
			yggr::string yggr_ins_str = "qqkkttt";
			yggr::utf8_string str = "abc";
			str.insert(0, stl_ins_str, 0, 2);
			yggr_test_assert(str == "qqabc");
			str.insert(1,  yggr_ins_str, 2, 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(stl_ins_str), 4);
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string ins_str = "qqkkttt";
			yggr::utf8_string str = "abc";
			str.insert(0, ins_str, 0, 2);
			yggr_test_assert(str == "qqabc");
			str.insert(1,  ins_str, 2, 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(ins_str), 4);
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);

		}
	}

	// insert(iter_p, iters, itere)
	{

		{
			char ins_str[] = "qqkkttt";
			yggr::utf8_string str = "abc";
			str.insert(str.begin(), ins_str, ins_str + 2);
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1,  ins_str + 2, ins_str + 2 + 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), ins_str + 2 + 2, ins_str + 2 + 2 + 3);
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string ins_str = "qqkkttt";
			yggr::utf8_string str = "abc";
			str.insert(str.begin(), ins_str.begin(), ins_str.begin() + 2);
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1,  ins_str.begin() + 2, ins_str.begin() + 2 + 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), ins_str.begin() + 2 + 2, ins_str.end());
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			char ins_str[] = "tttkkqq";
			yggr::utf8_string str = "abc";

			std::reverse_iterator<char*> start(ins_str + sizeof(ins_str) - 1);
			std::reverse_iterator<char*> last(ins_str);

			str.insert(str.begin(), start, start + 2);
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1,  start + 2, start + 2 + 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), start + 2 + 2, last);
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string ins_str = "tttkkqq";
			yggr::utf8_string str = "abc";
			str.insert(str.begin(), ins_str.rbegin(), ins_str.rbegin() + 2);
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1,  ins_str.rbegin() + 2, ins_str.rbegin() + 2 + 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), ins_str.rbegin() + 2 + 2, ins_str.rend());
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(string_view ...)
	{
		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::string_view("xyz"));
			yggr_test_assert(str == "axyzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::string_view("xyz"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
			yggr_test_assert(str == "axyzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::charset::make_string_charset_helper(yggr::string_view("xyz")));
			yggr_test_assert(str == "axyzbc");
		}


		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::string_view("xyz"), 1);
			yggr_test_assert(str == "ayzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::string_view("xyz"), 1, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
			yggr_test_assert(str == "ayzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::charset::make_string_charset_helper(yggr::string_view("xyz")), 1);
			yggr_test_assert(str == "ayzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::string_view("xyz"), 1, yggr::string_view::npos);
			yggr_test_assert(str == "ayzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::string_view("xyz"), 1, yggr::string_view::npos, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
			yggr_test_assert(str == "ayzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::charset::make_string_charset_helper(yggr::string_view("xyz")), 1, yggr::string_view::npos);
			yggr_test_assert(str == "ayzbc");
		}
	}

	// utf8_string_view
	{
		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::utf8_string_view("xyz"));
			yggr_test_assert(str == "axyzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::utf8_string_view("xyz"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str == "axyzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::charset::make_string_charset_helper(yggr::utf8_string_view("xyz")));
			yggr_test_assert(str == "axyzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::utf8_string_view("xyz"), 1);
			yggr_test_assert(str == "ayzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::utf8_string_view("xyz"), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str == "ayzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::charset::make_string_charset_helper(yggr::utf8_string_view("xyz")), 1);
			yggr_test_assert(str == "ayzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::utf8_string_view("xyz"), 1, yggr::utf8_string_view::npos);
			yggr_test_assert(str == "ayzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::utf8_string_view("xyz"), 1, yggr::utf8_string_view::npos, 
						YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str == "ayzbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.insert(1, yggr::charset::make_string_charset_helper(yggr::utf8_string_view("xyz")), 1, 
						yggr::utf8_string_view::npos);
			yggr_test_assert(str == "ayzbc");
		}
	}

#ifndef YGGR_NO_CWCHAR

	// insert(pos, n, char)
	{
		{
			yggr::utf8_string str = L"�����";
			str.insert(0, 2, L'��');
			yggr_test_assert(str == L"��������");
			str.insert(1, 2, L't');
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper(L'?'));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = L"�����";
			str.insert(0, 2, L"��");
			yggr_test_assert(str == L"��������");
			str.insert(1, 2, L"t");
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper(L"?"));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = L"�����";
			str.insert(0, 2, std::wstring(L"��"));
			yggr_test_assert(str == L"��������");
			str.insert(1, 2, yggr::wstring(L"t"));
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper(std::wstring(L"?")));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = L"�����";
			str.insert(0, 2, yggr::utf8_char(L"��"));
			yggr_test_assert(str == L"��������");
			str.insert(1, 2, yggr::utf8_char(L"t"));
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper(yggr::utf8_char(L"?")));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(iter_p, n, char)

	{
		{
			yggr::utf8_string str = L"�����";
			str.insert(str.begin(), 2, L'��');
			yggr_test_assert(str == L"��������");
			str.insert(str.begin() + 1, 2, L't');
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper(L'?'));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = L"�����";
			str.insert(str.begin(), 2, L"��");
			yggr_test_assert(str == L"��������");
			str.insert(str.begin() + 1, 2, L"t");
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper(L"?"));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = L"�����";
			str.insert(str.begin(), 2, std::wstring(L"��"));
			yggr_test_assert(str == L"��������");
			str.insert(str.begin() + 1, 2, yggr::wstring(L"t"));
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper(std::wstring(L"?")));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = L"�����";
			str.insert(str.begin(), 2, yggr::utf8_char(L"��"));
			yggr_test_assert(str == L"��������");
			str.insert(str.begin() + 1, 2, yggr::utf8_char(L"t"));
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper(yggr::utf8_char(L"?")));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(pos, const char*)
	{
		{
			yggr::utf8_string str = L"�����";
			str.insert(0, L"���");
			yggr_test_assert(str == L"��������");
			str.insert(1, L"tt");
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(L"???"));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = L"�����";
			str.insert(0, std::wstring(L"���"));
			yggr_test_assert(str == L"��������");
			str.insert(1, yggr::wstring(L"tt"));
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(std::wstring(L"???")));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string str = L"�����";
			str.insert(0, yggr::utf8_string(L"���"));
			yggr_test_assert(str == L"��������");
			str.insert(1, yggr::utf8_string(L"tt"));
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(yggr::utf8_string(L"???")));
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(pos, const char*, n)
	{

		{
			yggr::utf8_string str = L"�����";
			str.insert(0, L"���nil", 2);
			yggr_test_assert(str == L"��������");
			str.insert(1,  L"ttnil", 2);
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(L"???nil"), 3);
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);

			str.insert(0, str);
			yggr_test_assert(str == L"��tt�������???��tt�������???");
			yggr_test_assert(str.length() == 20);
		}
	}

	// insert(pos, string, subpos, sublen)
	{
		{
			yggr::utf8_string str = L"�����";
			str.insert(0, L"���tt???", 0, 2);
			yggr_test_assert(str == L"��������");
			str.insert(1,  L"���tt???", 2, 2);
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(L"���tt???"), 4);
			yggr_test_assert(str == L"��tt���������tt");
			yggr_test_assert(str.length() == 11);
		}

		{
			std::wstring stl_ins_str = L"���tt???";
			yggr::wstring yggr_ins_str = L"���tt???";
			yggr::utf8_string str = L"�����";
			str.insert(0, stl_ins_str, 0, 2);
			yggr_test_assert(str == L"��������");
			str.insert(1,  yggr_ins_str, 2, 2);
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(stl_ins_str), 4);
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string ins_str = L"���tt???";
			yggr::utf8_string str = L"�����";
			str.insert(0, ins_str, 0, 2);
			yggr_test_assert(str == L"��������");
			str.insert(1,  ins_str, 2, 2);
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(ins_str), 4);
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);

			str.insert(0, str);
			yggr_test_assert(str == L"��tt�������???��tt�������???");
			yggr_test_assert(str.length() == 20);
		}

	}

	// insert(iter_p, iters, itere)
	{

		{
			wchar_t ins_str[] = L"���tt???";
			yggr::utf8_string str = L"�����";
			str.insert(str.begin(), ins_str, ins_str + 2);
			yggr_test_assert(str == L"��������");
			str.insert(str.begin() + 1,  ins_str + 2, ins_str + 2 + 2);
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.end(), ins_str + 2 + 2, ins_str + 2 + 2 + 3);
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string ins_str = L"���tt???";
			yggr::utf8_string str = L"�����";
			str.insert(str.begin(), ins_str.begin(), ins_str.begin() + 2);
			yggr_test_assert(str == L"��������");
			str.insert(str.begin() + 1,  ins_str.begin() + 2, ins_str.begin() + 2 + 2);
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.end(), ins_str.begin() + 2 + 2, ins_str.end());
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			wchar_t ins_str[] = L"???tt���";
			std::reverse_iterator<wchar_t*> start(ins_str + (sizeof(ins_str) / sizeof(wchar_t) - 1));
			std::reverse_iterator<wchar_t*> last(ins_str);

			yggr::utf8_string str = L"�����";
			str.insert(str.begin(), start, start + 2);
			yggr_test_assert(str == L"��������");
			str.insert(str.begin() + 1,  start + 2, start + 2 + 2);
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.end(), start + 2 + 2, last);
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}

		{
			yggr::utf8_string ins_str = L"???tt���";
			yggr::utf8_string str = L"�����";
			str.insert(str.begin(), ins_str.rbegin(), ins_str.rbegin() + 2);
			yggr_test_assert(str == L"��������");
			str.insert(str.begin() + 1,  ins_str.rbegin() + 2, ins_str.rbegin() + 2 + 2);
			yggr_test_assert(str == L"��tt�������");
			str.insert(str.end(), ins_str.rbegin() + 2 + 2, ins_str.rend());
			yggr_test_assert(str == L"��tt�������???");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(string_view ...)
	{
		{
			yggr::utf8_string str = L"���Ǵ�";
			str.insert(1, yggr::wstring_view(L"��Դ��"));
			yggr_test_assert(str == L"����Դ��Ǵ�");
		}

		{
			yggr::utf8_string str = L"���Ǵ�";
			str.insert(1, yggr::wstring_view(L"��Դ��"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
			yggr_test_assert(str == L"����Դ��Ǵ�");
		}

		{
			yggr::utf8_string str = L"���Ǵ�";
			str.insert(1, yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��Դ��")));
			yggr_test_assert(str == L"����Դ��Ǵ�");
		}


		{
			yggr::utf8_string str = L"���Ǵ�";
			str.insert(1, yggr::wstring_view(L"��Դ��"), 1);
			yggr_test_assert(str == L"��Դ��Ǵ�");
		}

		{
			yggr::utf8_string str = L"���Ǵ�";
			str.insert(1, yggr::wstring_view(L"��Դ��"), 1, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
			yggr_test_assert(str == L"��Դ��Ǵ�");
		}

		{
			yggr::utf8_string str = L"���Ǵ�";
			str.insert(1, yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��Դ��")), 1);
			yggr_test_assert(str == L"��Դ��Ǵ�");
		}

		{
			yggr::utf8_string str = L"���Ǵ�";
			str.insert(1, yggr::wstring_view(L"��Դ��"), 1, yggr::wstring_view::npos);
			yggr_test_assert(str == L"��Դ��Ǵ�");
		}

		{
			yggr::utf8_string str = L"���Ǵ�";
			str.insert(1, yggr::wstring_view(L"��Դ��"), 1, yggr::wstring_view::npos, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
			yggr_test_assert(str == L"��Դ��Ǵ�");
		}

		{
			yggr::utf8_string str = L"���Ǵ�";
			str.insert(1, yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��Դ��")), 1, yggr::wstring_view::npos);
			yggr_test_assert(str == L"��Դ��Ǵ�");
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_insert sucessed" << std::endl;
}

void test_replace(void)
{
	// replace(pos, len, n, char)
	{
		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, 3, 'd');
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abcdef";
			str.replace(1, 3, 3, 'k');
			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "akkkef");
		}

		{
			yggr::utf8_string str = "abcdef";
			str.replace(1, 3, 0, 'k');
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "aef");
		}

		{
			yggr::utf8_string str = "abcdef";
			str.replace(1, 3, 2, 'k');
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "akkef");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, 3, "d");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, 3, std::string("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, 3, yggr::utf8_char("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper('d'));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper(yggr::string("d")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper(yggr::utf8_char("d")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 0, 0, 'd');
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "abc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(0, 100, 0, 'd');
			yggr_test_assert(str.length() == 0);
			yggr_test_assert(str == "");
		}
	}

	// replace(iters, itere, n, char)
	{
		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 3, 'd');
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 3, "d");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 3, std::string("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::utf8_char("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper('d'));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper(yggr::string("d")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper(yggr::utf8_char("d")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 1, 0, 'd');
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "abc");
		}

#if 0
		// this code is not accept,
		// because (stl and boost)'s result not same,
		// and !(i1 < i2) is illegal of using
		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin(), 0, 'd');
			yggr_test_assert(str.length() == 1);
			yggr_test_assert(str == "a");
		}
#endif // 0, 1

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin(), str.end(), 0, 'd');
			yggr_test_assert(str.length() == 0);
			yggr_test_assert(str == "");
		}
	}

	// replace(pos, len, const char*)
	{

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, "def");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, std::string("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, std::string("def"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, yggr::utf8_string("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(yggr::string("def")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(yggr::utf8_string("def")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 0, "def");
			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "adefbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(0, 100, "def");
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "def");
		}
	}

	// replace(pos, len, const char*, n)
	{

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, "def", 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper("def"), 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}


		{
			yggr::utf8_string str = "abc";
			str.replace(1, 0, "def", 3);
			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "adefbc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(0, 100, "def", 3);
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "def");
		}
	}

	// replace(iters, itere, const char*)
	{

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, "def");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, std::string("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, yggr::utf8_string("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper(yggr::string("def")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper(yggr::utf8_string("def")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

#if 0  //illegal of using
		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin(), "def");
			yggr_test_assert(str.length() == 4);
			yggr_test_assert(str == "adef");
		}
#endif // 0, 1

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin(), str.end(), "def");
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "def");
		}
	}

	// replace(iters, itere, const char*, n)
	{

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, "def", 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper("def"), 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

#if 0 //illegal of using
		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin(), "def", 3);
			yggr_test_assert(str.length() == 4);
			yggr_test_assert(str == "adef");
		}

#endif // 0, 1

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin(), str.end(), "def", 3);
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "def");
		}
	}

	// replace(pos, len, string, subpos, sublen)
	{
		std::string stl_rep_str = "tagdeftag";
		yggr::string yggr_rep_str = "tagdeftag";
		yggr::utf8_string utf8_rep_str = "tagdeftag";

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, "tagdeftag", 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, stl_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, yggr_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}


		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, utf8_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(stl_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(yggr_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(utf8_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(str), 0, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}
	}

	//replace(i1, i2, s, e)
	{
		{
			char rep_str[] = "abc";
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, rep_str, rep_str + 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}

		{
			std::string rep_str = "abc";
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.begin(), rep_str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}

		{
			yggr::string rep_str = "abc";
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.rbegin(), rep_str.rend());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "acbac");
		}

		{
			yggr::utf8_string rep_str = "abc";
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.begin(), rep_str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, str.begin(), str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}

		{
			yggr::utf8_string str = "abc";
			str.replace(str.begin(), str.end(), str.rbegin(), str.rend());
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "cba");
		}
	}


#ifndef YGGR_NO_CWCHAR
	// replace(pos, len, n, char)
	{
		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, 3, L'��');
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, 3, L"��");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, 3, std::wstring(L"��"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, 3, yggr::utf8_char(L"��"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper(L'��'));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper(L"��"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper(yggr::wstring(L"��")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper(yggr::utf8_char(L"��")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 0, 0, L'��');
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == L"�����");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(0, 100, 0, L'��');
			yggr_test_assert(str.length() == 0);
			yggr_test_assert(str == L"");
		}
	}

	// replace(iters, itere, n, char)
	{
		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, 3, L'��');
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, 3, L"��");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, 3, std::wstring(L"��"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::utf8_char(L"��"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper(L'��'));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper(L"��"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper(yggr::wstring(L"��")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper(yggr::utf8_char(L"��")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 1, 0, L'��');
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == L"�����");
		}

#if 0 //illegal of using
		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin(), 0, L'��');
			yggr_test_assert(str.length() == 1);
			yggr_test_assert(str == L"��");
		}
#endif // 0, 1

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin(), str.end(), 0, L'��');
			yggr_test_assert(str.length() == 0);
			yggr_test_assert(str == L"");
		}
	}

	// replace(pos, len, const char*)
	{

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, L"������");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, std::wstring(L"������"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, yggr::utf8_string(L"������"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(L"������"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(yggr::wstring(L"������")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(yggr::utf8_string(L"������")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 0, L"������");
			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == L"�㻵��������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(0, 100, L"������");
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == L"������");
		}
	}

	// replace(pos, len, const char*, n)
	{

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, L"������", 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(L"������"), 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}


		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 0, L"������", 3);
			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == L"�㻵��������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(0, 100, L"������", 3);
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == L"������");
		}
	}

	// replace(iters, itere, const char*)
	{

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, L"������");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, std::wstring(L"������"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, yggr::utf8_string(L"������"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper(L"������"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper(yggr::wstring(L"������")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper(yggr::utf8_string(L"������")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

#if 0 //illegal of using
		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin(), L"������");
			yggr_test_assert(str.length() == 4);
			yggr_test_assert(str == L"�㻵����");
		}
#endif // 0, 1

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin(), str.end(), L"������");
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == L"������");
		}
	}

	// replace(iters, itere, const char*, n)
	{

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, L"������", 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper(L"������"), 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

#if 0 //illegal of using
		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin(), L"������", 3);
			yggr_test_assert(str.length() == 4);
			yggr_test_assert(str == L"�㻵����");
		}
#endif // 0, 1

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin(), str.end(), L"������", 3);
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == L"������");
		}
	}

	// replace(pos, len, string, subpos, sublen)
	{
		std::wstring stl_rep_str = L"tag������tag";
		yggr::wstring yggr_rep_str = L"tag������tag";
		yggr::utf8_string utf8_rep_str = L"tag������tag";

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, L"tag������tag", 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, stl_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, yggr_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}


		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, utf8_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(stl_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(yggr_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(utf8_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(str), 0, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"���������");
		}
	}

	//replace(i1, i2, s, e)
	{
		{
			wchar_t rep_str[] = L"������";
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, rep_str, rep_str + 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			std::wstring rep_str = L"������";
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.begin(), rep_str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::wstring rep_str = L"������";
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.rbegin(), rep_str.rend());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string rep_str = L"������";
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.begin(), rep_str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"�㻵������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin() + 1, str.begin() + 2, str.begin(), str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == L"���������");
		}

		{
			yggr::utf8_string str = L"�����";
			str.replace(str.begin(), str.end(), str.rbegin(), str.rend());
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == L"�����");
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_replace sucessed" << std::endl;
}

void test_find(void)
{
	// find(char, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find('b') == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find("") == 0);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find('b') == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::utf8_char('b')) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper('b')) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(str[1]) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(*(str.cbegin() + 1)) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(str[1])) == 1);
		}
	}

	// find(const char*, pos)
	{
		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find("b") == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find("b", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper("b")) == 1);
		}
	}

	// find(const char*, pos, n)
	{
		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find("b", 0, 1) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find("b", 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}
	}

	// find(const string&, pos)
	{
		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(std::string("b")) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(std::string("b"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::utf8_string("b")) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(std::string("b"))) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::utf8_string("b"))) == 1);
		}
	}

	// find(const string_view, pos)
	{
		// string_view
		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::string_view("b")) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::string_view("b"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::string_view("b"))) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::string_view("b"), 1) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::string_view("b"), 1, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::string_view("b")), 1) == 1);
		}

		// utf8_string_view
		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::utf8_string_view("b")) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::utf8_string_view("b"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("b"))) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::utf8_string_view("b"), 1) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::utf8_string_view("b"), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("b")), 1) == 1);
		}
	}

#ifndef YGGR_NO_CWCHAR

	// find(char, pos)
	{
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(L'��') == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::utf8_char(L'��')) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(L'��')) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(str[2]) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(*(str.cbegin() + 2)) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(str[2])) == 2);
		}
	}

	// find(const char*, pos)
	{
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(L"��") == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(L"��")) == 2);
		}
	}

	// find(const char*, pos, n)
	{
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(L"��", 0, 1) == 2);
		}
	}

	// find(const string&, pos)
	{
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(std::wstring(L"��")) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::wstring(L"��"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::wstring(L"��"))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"��"))) == 2);
		}
	}

	// find(const string_view, pos)
	{
		// string_view
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::wstring_view(L"��")) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::wstring_view(L"��"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��"))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::wstring_view(L"��"), 1) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::wstring_view(L"��"), 1, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��")), 1) == 2);
		}

		// utf8_string_view
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::utf8_string_view(yggr::utf8_string(L"��"))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::utf8_string_view(yggr::utf8_string(L"��")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��")))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::utf8_string_view(yggr::utf8_string(L"��")), 1) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::utf8_string_view(yggr::utf8_string(L"��")), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��"))), 1) == 2);
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_find successed" << std::endl;
}

void test_rfind(void)
{
	// rfind(char, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.rfind('b') == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind("") == str.length());
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind('b') == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::utf8_char('b')) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper('b')) == 1);
		}

		{
			yggr::utf8_string str = "abcba";
			yggr_test_assert(str.rfind(str[1]) == 3);
		}

		{
			yggr::utf8_string str = "abcba";
			yggr_test_assert(str.rfind(*(str.cbegin() + 1)) == 3);
		}

		{
			yggr::utf8_string str = "abcba";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(str[1])) == 3);
		}
	}

	// rfind(const char*, pos)
	{
		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind("b") == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind("b", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper("b")) == 1);
		}
	}

	// rfind(const char*, pos, n)
	{
		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind("b", yggr::utf8_string::npos, 1) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind("b", yggr::utf8_string::npos, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}
	}

	// rfind(const string&, pos)
	{
		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(std::string("b")) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(std::string("b"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::utf8_string("b")) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(std::string("b"))) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(yggr::utf8_string("b"))) == 1);
		}
	}

	// rfind(const StringView*, pos)
	{
		//string_view
		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::string_view("b")) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::string_view("b"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(yggr::string_view("b"))) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::string_view("b"), yggr::utf8_string::npos) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::string_view("b"), yggr::utf8_string::npos, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(yggr::string_view("b")), yggr::utf8_string::npos) == 1);
		}

		//utf8_string_view
		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::utf8_string_view("b")) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::utf8_string_view("b"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("b"))) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::utf8_string_view("b"), yggr::utf8_string::npos) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::utf8_string_view("b"), yggr::utf8_string::npos, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			yggr::utf8_string str = "abc";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("b")), yggr::utf8_string::npos) == 1);
		}
	}

#ifndef YGGR_NO_CWCHAR

	// rfind(char, pos)
	{
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(L'��') == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::utf8_char(L'��')) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(L'��')) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b�����";
			yggr_test_assert(str.rfind(str[2]) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b�����";
			yggr_test_assert(str.rfind(*(str.begin() + 2)) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b�����";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(L'��')) == 5);
		}
	}

	// rfind(const char*, pos)
	{
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(L"��") == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(L"��")) == 2);
		}
	}

	// rfind(const char*, pos, n)
	{
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(L"��", yggr::utf8_string::npos, 1) == 2);
		}
	}

	// rfind(const string&, pos)
	{
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(std::wstring(L"��")) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::utf8_string(L"��")) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(yggr::wstring(L"��"))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"��"))) == 2);
		}

		{
			yggr::utf8_string str = "abcde";
			yggr_test_assert(str.rfind(str.data(), 1) == 0);
		}
	}

	// rfind(const StringView*, pos)
	{
		//string_view
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::wstring_view(L"��")) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::wstring_view(L"��"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��"))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::wstring_view(L"��"), yggr::utf8_string::npos) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::wstring_view(L"��"), yggr::utf8_string::npos, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��")), yggr::utf8_string::npos) == 2);
		}

		//utf8_string_view
		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::utf8_string_view(yggr::utf8_string(L"��"))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::utf8_string_view(yggr::utf8_string(L"��")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(
					yggr::charset::make_string_charset_helper(
						yggr::utf8_string_view(yggr::utf8_string(L"��")))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(yggr::utf8_string_view(yggr::utf8_string(L"��")), yggr::utf8_string::npos) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(
				str.rfind(
					yggr::utf8_string_view(yggr::utf8_string(L"��")),
					yggr::utf8_string::npos,
					YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��";
			yggr_test_assert(str.rfind(
					yggr::charset::make_string_charset_helper(
						yggr::utf8_string_view(yggr::utf8_string(L"��"))),
					yggr::utf8_string::npos) == 2);
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_rfind successed" << std::endl;
}

void test_find_first_of(void)
{
	// find_first_of(char, pos)

	{
		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of('c') == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::utf8_string::char_type('c')) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper('c')) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(str[2]) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(*(str.begin() + 2)) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::utf8_string::char_type(str[2])) == 2);
		}
	}

	// find_first_of(const char*, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_first_of("edc") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of("") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of("edc") == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper("edc")) == 2);
		}
	}

	// find_first_of(const char*, pos, n)
	{
		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of("edc", 1, 1) == 4);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper("edc"), 1, 1) == 4);
		}
	}

	// find_first_of(const string&, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_first_of(std::string("edc")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(boost::container::string("")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(std::string("edc")) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(boost::container::string("edc")) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::utf8_string("edc")) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(std::string("edc"))) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(boost::container::string("edc"))) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(yggr::utf8_string("edc"))) == 2);
		}
	}

	// find_first_of(const string_view&, pos)
	{
		// string_view
		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::string_view("edc")) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::string_view("edc"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(yggr::string_view("edc"))) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::string_view("edc"), 1) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::string_view("edc"), 1, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(yggr::string_view("edc")), 1) == 2);
		}

		// utf8_string_view
		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::utf8_string_view("edc")) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::utf8_string_view("edc"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("edc"))) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::utf8_string_view("edc"), 1) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::utf8_string_view("edc"), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("edc")), 1) == 2);
		}
	}

#ifndef YGGR_NO_CWCHAR
	// find_first_of(char, pos)

	{
		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(L'��') == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::utf8_string::char_type(L'��')) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(L'��')) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(str[2]) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(*(str.begin() + 2)) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::utf8_string::char_type(str[2])) == 2);
		}
	}

	// find_first_of(const char*, pos)
	{
		{
			yggr::utf8_string str = L"";
			yggr_test_assert(str.find_first_of(L"��b��") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of("") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(L"��b��") == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(L"��b��")) == 2);
		}
	}

	// find_first_of(const char*, pos, n)
	{
		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(L"��b��", 1, 1) == 4);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(L"��b��"), 1, 1) == 4);
		}
	}

	// find_first_of(const string&, pos)
	{
		{
			yggr::utf8_string str = L"";
			yggr_test_assert(str.find_first_of(std::wstring(L"��b��")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(boost::container::wstring(L"")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(std::wstring(L"��b��")) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(boost::container::wstring(L"��b��")) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::utf8_string(L"��b��")) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(std::wstring(L"��b��"))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(boost::container::wstring(L"��b��"))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"��b��"))) == 2);
		}
	}

	// find_first_of(const string_view&, pos)
	{
		// string_view
		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::wstring_view(L"��b��")) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::wstring_view(L"��b��"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��b��"))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::wstring_view(L"��b��"), 1) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::wstring_view(L"��b��"), 1, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��b��")), 1) == 2);
		}

		
		// utf8_string_view
		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��"))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��b��")))) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��")), 1) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��")), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��b��"))), 1) == 2);
		}
	}
#endif //YGGR_NO_CWCHAR

	std::cout << "test_find_first_of successed" << std::endl;
}

void test_find_first_not_of(void)
{
	// find_first_not_of(char, pos)

	{
		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of('c') == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string::char_type('c')) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper('c')) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(str[0]) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(*(str.begin())) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(str[0]) == 2);
		}
	}

	// find_first_not_of(const char*, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_first_not_of("edc") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of("") == 0);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of("edc") == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper("edc")) == 2);
		}
	}

	// find_first_not_of(const char*, pos, n)
	{
		{
			yggr::utf8_string str = "eeeeabcdefg";
			yggr_test_assert(str.find_first_not_of("edc", 1, 1) == 4);
		}

		{
			yggr::utf8_string str = "eeeeabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper("edc"), 1, 1) == 4);
		}
	}

	// find_first_not_of(const string&, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_first_not_of(std::string("edc")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(boost::container::string(""), 3) == 3);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(std::string("edc")) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(boost::container::string("edc")) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string("edc")) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(std::string("edc"))) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(boost::container::string("edc"))) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string("edc"))) == 2);
		}
	}

	// find_first_not_of(const string_view&, pos)
	{
		// string_view
		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::string_view("edc")) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::string_view("edc"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(yggr::string_view("edc"))) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::string_view("edc"), 1) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::string_view("edc"), 1, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(yggr::string_view("edc")), 1) == 2);
		}

		// utf8_string_view
		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string_view("edc")) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string_view("edc"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("edc"))) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string_view("edc"), 1) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string_view("edc"), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("edc")), 1) == 2);
		}
	}

#ifndef YGGR_NO_CWCHAR

	// find_first_not_of(char, pos)
	{
		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(L'��') == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string::char_type(L'��')) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(L'��')) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(str[0]) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(*(str.begin())) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(str[0]) == 2);
		}
	}

	// find_first_not_of(const char*, pos)
	{
		{
			yggr::utf8_string str = L"";
			yggr_test_assert(str.find_first_not_of(L"��b��") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of("") == 0);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(L"��b��") == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(L"��b��")) == 2);
		}
	}

	// find_first_not_of(const char*, pos, n)
	{
		{
			yggr::utf8_string str = L"����������a��b��c";
			yggr_test_assert(str.find_first_not_of(L"��b��", 1, 1) == 4);
		}

		{
			yggr::utf8_string str = L"����������a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(L"��b��"), 1, 1) == 4);
		}
	}

	// find_first_not_of(const string&, pos)
	{
		{
			yggr::utf8_string str = L"";
			yggr_test_assert(str.find_first_not_of(std::wstring(L"��b��")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(boost::container::wstring(L"")) == 0);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(std::wstring(L"��b��")) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(boost::container::wstring(L"��b��")) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string(L"��b��")) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(std::wstring(L"��b��"))) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(boost::container::wstring(L"��b��"))) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"��b��"))) == 2);
		}
	}

	// find_first_not_of(const string_view&, pos)
	{
		// string_view
		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::wstring_view(L"��b��")) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::wstring_view(L"��b��"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��b��"))) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::wstring_view(L"��b��"), 1) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::wstring_view(L"��b��"), 1, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��b��")), 1) == 2);
		}

		// utf8_string_view
		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��"))) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��b��")))) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��")), 1) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��")), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 2);
		}

		{
			yggr::utf8_string str = L"�ú���a��b��c";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��b��"))), 1) == 2);
		}
	}

#endif //YGGR_NO_CWCHAR

	std::cout << "test_find_first_not_of successed" << std::endl;
}

void test_find_last_of(void)
{
	// find_last_of(char, pos)

	{
		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of('c') == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(yggr::utf8_string::char_type('c')) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper('c')) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(str[2]) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(*(str.begin() + 2)) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(yggr::utf8_string::char_type(str[2])) == 4);
		}
	}

	// find_last_of(const char*, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_last_of("edc") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of("") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of("edc") == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper("edc")) == 4);
		}
	}

	// find_last_of(const char*, pos, n)
	{
		{
			yggr::utf8_string str = "ebcdcba";
			yggr_test_assert(str.find_last_of("edc", 1, 1) == 0);
		}

		{
			yggr::utf8_string str = "abcdcbae";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper("edc"), 1, 1) == yggr::utf8_string::npos);
		}
	}

	// find_last_of(const string&, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_last_of(std::string("edc")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(boost::container::string("")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(std::string("edc")) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(boost::container::string("edc")) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(yggr::utf8_string("edc")) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(std::string("edc"))) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(boost::container::string("edc"))) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::utf8_string("edc"))) == 4);
		}
	}


#ifndef YGGR_NO_CWCHAR
	// find_last_of(char, pos)

	{
		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_last_of(L'��') == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_last_of(yggr::utf8_string::char_type(L'��')) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(L'��')) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_last_of(str[2]) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_last_of(*(str.begin() + 2)) == 2);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_last_of(yggr::utf8_string::char_type(str[2])) == 2);
		}
	}

	// find_last_of(const char*, pos)
	{
		{
			yggr::utf8_string str = L"";
			yggr_test_assert(str.find_last_of(L"��b��") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of("") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of(L"��b��") == 4);
		}

		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(L"��b��")) == 4);
		}
	}

	// find_last_of(const char*, pos, n)
	{
		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of(L"��b��", str.length(), 1) == 3);
		}

		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(L"��b��"), str.length(), 1) == 3);
		}
	}

	// find_last_of(const string&, pos)
	{
		{
			yggr::utf8_string str = L"";
			yggr_test_assert(str.find_last_of(std::wstring(L"��b��")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"��a��b��c";
			yggr_test_assert(str.find_last_of(boost::container::wstring(L"")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of(std::wstring(L"��b��")) == 4);
		}

		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of(boost::container::wstring(L"��b��")) == 4);
		}

		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of(yggr::utf8_string(L"��b��")) == 4);
		}

		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(std::wstring(L"��b��"))) == 4);
		}

		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(boost::container::wstring(L"��b��"))) == 4);
		}

		{
			yggr::utf8_string str = L"��a�����a��";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"��b��"))) == 4);
		}
	}
#endif //YGGR_NO_CWCHAR

	std::cout << "test_find_last_of successed" << std::endl;
}

void test_find_last_not_of(void)
{
	// find_last_not_of(char, pos)

	{
		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_last_not_of('c') == str.length() - 1);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string::char_type('c')) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper('c')) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(str[2]) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(*(str.begin() + 2)) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(str[2]) == 6);
		}
	}

	// find_last_not_of(const char*, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_last_not_of("edc") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of("") == str.length() - 1);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of("edc") == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper("edc")) == 6);
		}
	}

	// find_last_not_of(const char*, pos, n)
	{
		{
			yggr::utf8_string str = "eeeeabcdefg";
			yggr_test_assert(str.find_last_not_of("edc", 1, 1) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdefgeeee";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper("edc"), -1, 1) == 6);
		}
	}

	// find_last_not_of(const string&, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_last_not_of(std::string("edc")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(boost::container::string(""), 3) == 3);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(std::string("edc")) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(boost::container::string("edc")) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string("edc")) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(std::string("edc"))) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(boost::container::string("edc"))) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string("edc"))) == 6);
		}
	}

	// find_last_not_of(const string_view, pos)
	{
		// string_view
		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::string_view("edc")) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::string_view("edc"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(yggr::string_view("edc"))) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgeeee";
			yggr_test_assert(str.find_last_not_of(yggr::string_view("edc"), -1) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgeeee";
			yggr_test_assert(str.find_last_not_of(yggr::string_view("edc"), -1, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgeeee";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(yggr::string_view("edc")), -1) == 6);
		}

		// utf8_string_view
		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string_view("edc")) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string_view("edc"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("edc"))) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgeeee";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string_view("edc"), -1) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgeeee";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string_view("edc"), -1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgeeee";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("edc")), -1) == 6);
		}
	}

#ifndef YGGR_NO_CWCHAR
	// find_first_not_of(char, pos)

	{
		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(L'��') == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string::char_type(L'��')) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(L'��')) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(str[2]) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(*(str.begin() + 2)) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(str[2]) == 5);
		}
	}

	// find_last_not_of(const char*, pos)
	{
		{
			yggr::utf8_string str = L"";
			yggr_test_assert(str.find_last_not_of(L"��b��") == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of("") == str.length() - 1);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(L"��b��") == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(L"��b��")) == 5);
		}
	}

	// find_last_not_of(const char*, pos, n)
	{
		{
			yggr::utf8_string str = L"��a��b��c��������";
			yggr_test_assert(str.find_last_not_of(L"��b��", 1, 1) == 1);
		}

		{
			yggr::utf8_string str = L"����������a��b��";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(L"��b��"), -1, 1) == 7);
		}
	}

	// find_first_not_of(const string&, pos)
	{
		{
			yggr::utf8_string str = L"";
			yggr_test_assert(str.find_last_not_of(std::wstring(L"��b��")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(boost::container::wstring(L"")) == str.length() - 1);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(std::wstring(L"��b��")) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(boost::container::wstring(L"��b��")) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string(L"��b��")) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(std::wstring(L"��b��"))) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(boost::container::wstring(L"��b��"))) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string(L"��b��"))) == 5);
		}
	}

	// find_last_not_of(const string_view, pos)
	{
		// string_view
		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::wstring_view(L"��b��")) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::wstring_view(L"��b��"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��b��"))) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::wstring_view(L"��b��"), -1) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::wstring_view(L"��b��"), -1, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��b��")), -1) == 5);
		}

		// utf8_string_view
		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��"))) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��b��")))) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��")), -1) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::utf8_string_view(yggr::utf8_string(L"��b��")), -1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 5);
		}

		{
			yggr::utf8_string str = L"��a��b��c�ú�";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��b��"))), -1) == 5);
		}
	}

#endif //YGGR_NO_CWCHAR

	std::cout << "test_find_last_not_of successed" << std::endl;
}

std::size_t op_resize(std::size_t old_size, 
						char* buf, std::size_t new_size,
						std::size_t char_size)
{
	if(new_size == old_size)
	{
		return new_size;
	}
	else if(new_size < old_size)
	{
		if(new_size < char_size * 2)
		{
			return 0;
		}
		else
		{
			return old_size - char_size * 2;
		}
	}
	else // new_size > old_size
	{
		yggr::utf8_char ch = 'z';
		std::memcpy(buf + (old_size), ch.data(), ch.size());
		return old_size + ch.size();
	}
}

std::size_t op_relength(std::size_t old_size, std::size_t old_len, 
							char* buf, std::size_t new_size, std::size_t new_len,
							std::size_t char_size)
{
	if(new_len == old_len)
	{
		return new_size;
	}
	else if(new_len < old_len)
	{
		if(new_len < 2)
		{
			return 0;
		}
		else
		{
			return old_size - char_size * 2;
		}
	}
	else // new_len > old_len
	{
		yggr_test_assert((new_size - old_size) / yggr::charset::utf8_foo::E_char_max_size == new_len - old_len);
		yggr::utf8_char ch = 'z';
		std::memcpy(buf + (old_size), ch.data(), ch.size());
		return old_size + ch.size();
	}
}

void test_resize_relength(void)
{
	// the second parameter of the resize function can only use ASCII characters,
	// if the multibyte UTF8 character is truncated when resize,
	// is used to replace the remaining part with the incoming ASCII character

	// resize
	{
		// >
		yggr::utf8_string str = "abc";
		str.resize(10, 'd');
		yggr_test_assert(str.length() == 10);
		yggr_test_assert(str.size() == 10);
		yggr_test_assert(str == "abcddddddd");

		// <
		str.resize(1, 'd');
		yggr_test_assert(str.length() == 1);
		yggr_test_assert(str.size() == 1);
		yggr_test_assert(str == "a");
	}

	// resize_and_overwrite
	{
		yggr::utf8_string str = "abc";

		str.resize_and_overwrite(str.size(), boost::bind(&op_resize, str.size(), _1, _2, 1));
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
		yggr_test_assert(str.length() == 3);

		str.resize_and_overwrite(str.size() - 1, boost::bind(&op_resize, str.size(), _1, _2, 1));
		yggr_test_assert(str == "a");
		yggr_test_assert(str.size() == 1);
		yggr_test_assert(str.length() == 1);

		str.resize_and_overwrite(str.size() + 5, boost::bind(&op_resize, str.size(), _1, _2, 1));
		yggr_test_assert(str == "az");
		yggr_test_assert(str.size() == 2);
		yggr_test_assert(str.length() == 2);
	}

	// relength
	{
		// >
		yggr::utf8_string str = "abc";
		str.relength(10, 'd');
		yggr_test_assert(str.length() == 10);
		yggr_test_assert(str.size() == 10);
		yggr_test_assert(str == "abcddddddd");

		// <
		str.relength(1, 'd');
		yggr_test_assert(str.length() == 1);
		yggr_test_assert(str.size() == 1);
		yggr_test_assert(str == "a");
	}

	// relength_and_overwrite
	{
		yggr::utf8_string str = "abc";

		str.relength_and_overwrite(str.length(), boost::bind(&op_relength, str.size(), str.length(), _1, _2, _3, 1));
		yggr_test_assert(str == "abc");
		yggr_test_assert(str.size() == 3);
		yggr_test_assert(str.length() == 3);

		str.relength_and_overwrite(str.length() - 1, boost::bind(&op_relength, str.size(), str.length(), _1, _2, _3, 1) );
		yggr_test_assert(str == "a");
		yggr_test_assert(str.size() == 1);
		yggr_test_assert(str.length() == 1);

		str.relength_and_overwrite(str.length() + 5, boost::bind(&op_relength, str.size(), str.length(), _1, _2, _3, 1) );
		yggr_test_assert(str == "az");
		yggr_test_assert(str.size() == 2);
		yggr_test_assert(str.length() == 2);
	}

#ifndef YGGR_NO_CWCHAR

	// resize
	{
		// >
		yggr::utf8_string str = L"�����";
		str.resize(10, 'd');
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str.size() == 10);
		yggr_test_assert(str == L"�����d");

		// <
		str.resize(1, 'd');
		yggr_test_assert(str.length() == 1);
		yggr_test_assert(str.size() == 1);
		yggr_test_assert(str == L"d"); // multibyte UTF8 character str[0] is truncated
	}

	// resize_and_overwrite
	{
		yggr::utf8_string str = L"�����";

		std::size_t old_size = str.size();
		std::size_t old_len = str.length();

		str.resize_and_overwrite(str.size(), 
									boost::bind(
										&op_resize, str.size(), _1, _2, yggr::utf8_char(L'��').size()));
		yggr_test_assert(str == L"�����");
		yggr_test_assert(str.size() == old_size);
		yggr_test_assert(str.length() == old_len);

		old_size = str.size();
		old_len = str.length();

		str.resize_and_overwrite(str.size() - yggr::utf8_char(L'��').size(), 
									boost::bind(
										&op_resize, str.size(), _1, _2, yggr::utf8_char(L'��').size()));
		yggr_test_assert(str == L"��");
		yggr_test_assert(str.size() == yggr::utf8_char(L'��').size());
		yggr_test_assert(str.length() == 1);

		old_size = str.size();
		old_len = str.length();

		str.resize_and_overwrite(str.size() + yggr::utf8_char(L'��').size() * 5, 
									boost::bind(
										&op_resize, str.size(), _1, _2, yggr::utf8_char(L'��').size()));
		yggr_test_assert(str == L"��z");
		yggr_test_assert(str.size() == yggr::utf8_char(L'��').size() + 1);
		yggr_test_assert(str.length() == 2);
	}

	// relength
	{
		// >
		yggr::utf8_string str = L"�����";
		str.relength(10, L'd');
		yggr_test_assert(str.length() == 10);
		yggr_test_assert(str.size() == 16);
		yggr_test_assert(str == L"�����ddddddd");

		// <
		str.relength(1, L'd');
		yggr_test_assert(str.length() == 1);
		yggr_test_assert(str.size() == 3);
		yggr_test_assert(str == L"��");
	}

	// relength_and_overwrite
	{
		yggr::utf8_string str = L"�����";

		std::size_t old_size = str.size();
		std::size_t old_len = str.length();

		str.relength_and_overwrite(str.length(), 
									boost::bind(
										&op_relength,
										str.size(), str.length(),
										_1, _2, _3,
										yggr::utf8_char(L'��').size()));
		yggr_test_assert(str == L"�����");
		yggr_test_assert(str.size() == old_size);
		yggr_test_assert(str.length() == old_len);

		old_size = str.size();
		old_len = str.length();

		str.relength_and_overwrite(str.length() - 1, 
									boost::bind(
										&op_relength, 
										str.size(), str.length(),
										_1, _2, _3,
										yggr::utf8_char(L'��').size()));
		yggr_test_assert(str == L"��");
		yggr_test_assert(str.size() == yggr::utf8_char(L'��').size());
		yggr_test_assert(str.length() == 1);

		old_size = str.size();
		old_len = str.length();

		str.relength_and_overwrite(str.length() + 5, 
									boost::bind(
										&op_relength, 
										str.size(), str.length(),
										_1, _2, _3, 
										yggr::utf8_char(L'��').size()));
		yggr_test_assert(str == L"��z");
		yggr_test_assert(str.size() == yggr::utf8_char(L'��').size() + 1);
		yggr_test_assert(str.length() == 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_resize_relength succeed" << std::endl;
}

void test_copy(void)
{
	{
		yggr::utf8_string str = "abc";
		char out_str[1024] = {0};
		std::size_t size = str.copy(out_str, 2);
		yggr_test_assert(size == 2);
		out_str[size] = 0;
		yggr_test_assert(0 == memcmp(out_str, "ab", size));
	}

#ifndef YGGR_NO_CWCHAR

	{
		yggr::utf8_string str = L"�����";
		wchar_t out_str[1024] = {0};
		std::size_t size = str.copy(out_str, 2);
		yggr_test_assert(size == 2);
		out_str[size] = 0;
		yggr_test_assert(0 == memcmp(out_str, L"���", size * sizeof(wchar_t)));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_copy succeed" << std::endl;
}

void test_iterator(void)
{
	yggr::string stlstr("abc");
	yggr::utf8_string str("a��b��c");

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
			//char c = *i;
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

	yggr::utf8_string str2(str.rbegin(), str.rend(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	std::cout << str2 << std::endl;
	std::cout << str.rbegin() - str.rend() << std::endl;

	// iterator
	{
		yggr::utf8_string str = "abc";

		int idx = 0;
		for(yggr::utf8_string::iterator i = str.begin(), isize = str.end(); i != isize; ++i)
		{
			*i = 'd' + idx;
			++idx;
		}

		yggr_test_assert(str == "def");
	}

	{
		yggr::utf8_string str = "abc";

		int idx = 0;
		for(yggr::utf8_string::iterator i = str.begin(); i != str.cend(); ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		yggr::utf8_string str = "abc";

		int idx = 0;
		for(yggr::utf8_string::non_const_iterator i = str.ncbegin(); i != str.cend(); ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		yggr::utf8_string str = "abc";

		int idx = 0;
		for(yggr::utf8_string::non_const_iterator i = str.ncbegin(); i != str.end(); ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		yggr::utf8_string str = "abc";

		yggr::utf8_string::non_const_iterator iter = str.begin();
		yggr::utf8_string::const_iterator citer = str.begin();
		yggr::utf8_string::const_iterator citer2 = str.ncbegin();

		yggr_test_assert(*iter == 'a');
		yggr_test_assert(*citer == 'a');
		yggr_test_assert(*citer2 == 'a');

		iter = str.begin() + 1;
		citer = str.begin() + 1;
		citer2 = str.ncbegin() + 1;

		yggr_test_assert(*iter == 'b');
		yggr_test_assert(*citer == 'b');
		yggr_test_assert(*citer2 == 'b');
	}

#ifndef YGGR_NO_CWCHAR
	// iterator
	{
		yggr::utf8_string str = L"�����";

		std::wstring rep = L"�Է���";

		int idx = 0;
		for(yggr::utf8_string::iterator i = str.begin(), isize = str.end(); i != isize; ++i)
		{
			*i = rep[idx];
			++idx;
		}

		yggr_test_assert(str == L"�Է���");
	}

	// iterator
	{
		yggr::utf8_string str = L"��a��";

		yggr::utf8_string rep = L"�Է���";

		int idx = 0;
		for(yggr::utf8_string::iterator i = str.begin(), isize = str.end(); i != isize; ++i)
		{
			*i = rep[idx];
			++idx;
		}

		yggr_test_assert(str == L"�Է���");
	}

	{
		yggr::utf8_string str = L"�Է���";

		int idx = 0;
		for(yggr::utf8_string::iterator i = str.begin(); i != str.cend(); ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		yggr::utf8_string str = L"�Է���";

		int idx = 0;
		for(yggr::utf8_string::non_const_iterator i = str.ncbegin(); i != str.cend(); ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		yggr::utf8_string str = L"�Է���";

		int idx = 0;
		for(yggr::utf8_string::non_const_iterator i = str.ncbegin(); i != str.end(); ++i)
		{
			std::cout << *i << std::endl;
		}
	}
#endif // YGGR_NO_CWCHAR
}

void test_element_access(void)
{
	yggr::utf8_string str("a��b��c");
	for(int i = 0, isize = str.length(); i != isize; ++i)
	{
		std::cout << str[i] << std::endl;
		std::cout << str.at(i) << std::endl;
	}

	std::cout << str.str<yggr::string>() << std::endl;
#ifndef YGGR_NO_CWCHAR
	std::wcout << str.str<yggr::wstring>() << std::endl;
#endif // YGGR_NO_CWCHAR

}


//-----------------------

void test_modify(void)
{
	// append_test
	yggr::utf8_string str("a��b��c");
	yggr_test_assert(str.length() == 5);

	str.append("d����f");
	yggr_test_assert(str.length() == 9);
	yggr_test_assert(str == "a��b��cd����f");
	std::cout << str << std::endl;

	str.append(5, 'z');
	yggr_test_assert(str.length() == 14);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.append(5, L'��');
#else
    str.append(5, "��");
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 19);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.append(5, yggr::utf8_char(L'��'));
#else
    str.append(5, yggr::utf8_char("��"));
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 24);
	std::cout << str << std::endl;

	str.append("a��aϷa");
	yggr_test_assert(str.length() == 29);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.append(L"w��wϷw");
#else
    str.append("w��wϷw");
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 34);
	std::cout << str << std::endl;

	str.append(yggr::string("boost��boostϷboost"));
	yggr_test_assert(str.length() == 34 + 15 + 2);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.append(yggr::wstring(L"stdw��stdwϷstdw"));
#else
    str.append(yggr::string("stdw��stdwϷstdw"));
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 51 + 4 * 3 + 2);
	std::cout << str << std::endl;

	str = "����";
	yggr_test_assert(str.length() == 2);
	std::cout << str << std::endl;

	str.append("abcdefg", 3);
	yggr_test_assert(str.length() == 5);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.append(L"abcdefg", 3);
#else
    str.append("abcdefg", 3);
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 8);
	std::cout << str << std::endl;

	str = "";
	yggr_test_assert(str.length() == 0);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str = L"ҳ��";
#else
    str = "ҳ��";
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 2);
	std::cout << str << std::endl;

	str.append(yggr::string("abcdefg"), 3, 3);
	yggr_test_assert(str.length() == 5);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.append(yggr::wstring(L"abcdefg"), 0, 3);
#else
    str.append(yggr::string("abcdefg"), 0, 3);
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 8);
	std::cout << str << std::endl;

	char arr_str[] = "zxcvbnm";
	str.append(arr_str, arr_str + 3);
	yggr_test_assert(str.length() == 11);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	wchar_t arr_wstr[] = L"qwertyui";
#else
    char arr_wstr[] = "qwertyui";
#endif // YGGR_NO_CWCHAR
	str.append(arr_wstr, arr_wstr + 5);
	yggr_test_assert(str.length() == 16);
	std::cout << str << std::endl;

	yggr::string stl_str("���Ǻ���");
	str.append(stl_str.begin(), stl_str.end());
	yggr_test_assert(str.length() == 20);
	std::cout << str << std::endl;

	yggr::utf8_string utf8_str("����UTF8����");
	str.append(utf8_str.begin(), utf8_str.end());
	yggr_test_assert(str.length() == 28);
	std::cout << str << std::endl;

	str.append(utf8_str.rbegin(), utf8_str.rend());
	yggr_test_assert(str.length() == 36);
	std::cout << str << std::endl;

	std::cout << utf8_str.substr(1, 8) << std::endl;

	str = "����";
	str.append(utf8_str, 1, 3);
	yggr_test_assert(str.length() == 5);
	std::cout << str << std::endl;
	//push_back_test

	str.push_back('d');
#ifndef YGGR_NO_CWCHAR
	str.push_back(L'Ȼ');
	str.push_back(yggr::utf8_char(L'��'));
#else
    str.push_back("Ȼ");
    str.push_back(yggr::utf8_char("��"));
#endif // YGGR_NO_CWCHAR


	yggr_test_assert(str.length() == 8);
	std::cout << str << std::endl;

	str.assign("assign����");
	yggr_test_assert(str.length() == 8);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.assign(L"assign_w����");
#else
    str.assign("assign_w����");
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 10);
	std::cout << str << std::endl;

	str.assign(yggr::string("stl_assign_����"));
	yggr_test_assert(str.length() == 13);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.assign(yggr::wstring(L"stl_assign_w����"));
#else
    str.assign(yggr::string("stl_assign_w����"));
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 14);
	std::cout << str << std::endl;

	str.assign(boost::container::string("boost_assign_����"));
	yggr_test_assert(str.length() == 15);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.assign(boost::container::wstring(L"boost_assign_w����"));
#else
    str.assign(boost::container::string("boost_assign_w����"));
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 16);
	std::cout << str << std::endl;

	yggr::string stl_str2("iter_stl_assign_����");
	str.assign(stl_str2.begin(), stl_str2.end());
	yggr_test_assert(str.length() == 18);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	yggr::wstring stl_wstr2(L"iter_stl_assign_w����");
#else
    yggr::string stl_wstr2("iter_stl_assign_w����");
#endif // YGGR_NO_CWCHAR
	str.assign(stl_wstr2.begin(), stl_wstr2.end());
	yggr_test_assert(str.length() == 19);
	std::cout << str << std::endl;

	boost::container::string boost_str2("iter_boost_assign_����");
	str.assign(boost_str2.begin(), boost_str2.end());
	yggr_test_assert(str.length() == 20);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	boost::container::wstring boost_wstr2(L"iter_boost_assign_w����");
#else
    boost::container::string boost_wstr2("iter_boost_assign_w����");
#endif // YGGR_NO_CWCHAR
	str.assign(boost_wstr2.begin(), boost_wstr2.end());
	yggr_test_assert(str.length() == 21);
	std::cout << str << std::endl;

	yggr::utf8_string u_str(str.begin(), str.end());
	yggr_test_assert(u_str.length() == 21);
	std::cout << u_str << std::endl;

	str = "";
	yggr_test_assert(str.empty());
	str.assign(boost::move(u_str));
	yggr_test_assert(str.length() == 21);
	//yggr_test_assert(u_str.empty());
	std::cout << str << std::endl;

	str.assign(u_str);
	//yggr_test_assert(str.empty());

	// assign string view
	{
		str.assign(yggr::string_view("abc"));
		yggr_test_assert(str == "abc");

		str.assign(yggr::utf8_string_view("abc"));
		yggr_test_assert(str == "abc");

		str.assign(yggr::string_view("abc"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
		yggr_test_assert(str == "abc");

		str.assign(yggr::utf8_string_view("abc"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str == "abc");

		str.assign(yggr::string_view("abc"), 1, 3);
		yggr_test_assert(str == "bc");

		str.assign(yggr::utf8_string_view("abc"), 1, 3);
		yggr_test_assert(str == "bc");

		str.assign(yggr::string_view("abc"), 1, 3, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME());
		yggr_test_assert(str == "bc");

		str.assign(yggr::utf8_string_view("abc"), 1, 3, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str == "bc");

		str.assign(yggr::charset::make_string_charset_helper(yggr::string_view("abc")));
		yggr_test_assert(str == "abc");

		str.assign(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("abc")));
		yggr_test_assert(str == "abc");

		str.assign(yggr::charset::make_string_charset_helper(yggr::string_view("abc")), 1, 3);
		yggr_test_assert(str == "bc");

		str.assign(yggr::charset::make_string_charset_helper(yggr::utf8_string_view("abc")), 1, 3);
		yggr_test_assert(str == "bc");
	}

#ifndef YGGR_NO_CWCHAR
	{
		str.assign(yggr::wstring_view(L"�����"));
		yggr_test_assert(str == L"�����");

		str.assign(yggr::wstring_view(L"�����"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
		yggr_test_assert(str == L"�����");

		str.assign(yggr::wstring_view(L"�����"), 1, 3);
		yggr_test_assert(str == L"����");

		str.assign(yggr::wstring_view(L"�����"), 1, 3, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME());
		yggr_test_assert(str == L"����");

		str.assign(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"�����")));
		yggr_test_assert(str == L"�����");

		str.assign(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"�����")), 1, 3);
		yggr_test_assert(str == L"����");
	}
#endif // YGGR_NO_CWCHAR

	//insert test
	str = "abc";

	str.insert(1, 2, 'k');
	yggr_test_assert(str.length() == 5);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.insert(2, 2, L'��');
#else
    str.insert(2, 2, "��");
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 7);
	std::cout << str << std::endl;

	{
		yggr::utf8_string::iterator iter = str.insert(str.begin() + 3, 2, 't');
		yggr_test_assert(iter - str.begin() == 3);
		std::cout << yggr::utf8_char(iter, iter + 1) << std::endl;
	}

	{
#	ifndef YGGR_NO_CWCHAR
		yggr::utf8_string::iterator iter = str.insert(str.begin() + 3, 2, L'��');
#	else
		yggr::utf8_string::iterator iter = str.insert(str.begin() + 3, 2, "��");
#	endif // YGGR_NO_CWCHAR
		yggr_test_assert(iter - str.begin() == 3);
		std::cout << yggr::utf8_char(iter, iter + 1) << std::endl;
	}

	str.insert(str.begin(), "z");
#ifndef YGGR_NO_CWCHAR
	str.insert(str.begin(), L"��");
#else
    str.insert(str.begin(), "��");
#endif // YGGR_NO_CWCHAR
	str.insert(str.begin(), 1, yggr::utf8_char('x'));

	yggr_test_assert(str.length() == 14);
	std::cout << str << std::endl;

	{
		yggr::string str_ins("abcdefg");
		str.insert(str.begin(), str_ins.begin(), str_ins.end());
		yggr_test_assert(str.length() == 21);
		std::cout << str << std::endl;
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr::wstring str_ins(L"abcdefg");
#else
        yggr::string str_ins("abcdefg");
#endif // YGGR_NO_CWCHAR
		str.insert(str.begin(), str_ins.begin(), str_ins.end());
		yggr_test_assert(str.length() == 28);
		std::cout << str << std::endl;
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr::wstring str_ins(L"�������Ŷ����");
		str.insert(str.begin(), str_ins.rbegin(), str_ins.rend());
#else
        yggr::string str_ins("�������Ŷ����");
        str.insert(str.begin(), str_ins.begin(), str_ins.end()); // can't use rbegin() rend()
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 35);
		std::cout << str << std::endl;
	}

	{
		str = "utf8�ַ���";
		yggr::utf8_string ustr("���Ǹ�");
		str.insert(str.begin(), ustr.begin(), ustr.end());
		yggr_test_assert(str.length() == 10);

		std::cout << str << std::endl;
	}

	{
		str = "utf8�ַ���";
		yggr::utf8_string ustr("���Ǹ�");
		str.insert(str.begin(), ustr.rbegin(), ustr.rend());
		yggr_test_assert(str.length() == 10);

		std::cout << str << std::endl;
	}

	{
		str = "utf8�ַ���";
		yggr::utf8_string ustr("���Ǹ�");
		str.insert(str.end(), ustr.rbegin(), ustr.rend());
		yggr_test_assert(str.length() == 10);

		std::cout << str << std::endl;
	}

	// erase test
	{
		str = "utf8�ַ���";
		str.erase();
		yggr_test_assert(str.empty());
	}

	{
		str = "utf8�ַ���";
		str.erase(0, 100);
		yggr_test_assert(str.empty());
	}

	{
		str = "utf8�ַ���";
		str.erase(1, 100);
		yggr_test_assert(str.length() == 1);
		std::cout << str << std::endl;
	}

	{
		str = "utf8�ַ���";
		str.erase(6, 1);
		yggr_test_assert(str.length() == 6);
		std::cout << str << std::endl;
	}

	{
		str = "utf8�ַ���";
		str.erase(str.begin());
		yggr_test_assert(str.length() == 6);
		str.erase(str.begin() + 1);
		yggr_test_assert(str.length() == 5);
		str.erase(str.end());
		yggr_test_assert(str.length() == 5);
		str.erase(str.end() - 1);
		yggr_test_assert(str.length() == 4);
		std::cout << str << std::endl;
	}

	{
		str = "utf8�ַ���";
		str.erase(str.begin(), str.begin() + 1);
		yggr_test_assert(str.length() == 6);
		str.erase(str.begin() + 1, str.begin() + 2);
		yggr_test_assert(str.length() == 5);
		str.erase(str.end() - 1, str.end());
		yggr_test_assert(str.length() == 4);
		str.erase(str.begin() + 1, str.end() - 1);
		yggr_test_assert(str.length() == 2);
		std::cout << str << std::endl;
	}

	// replace test
	{
		str = "string �ַ���";
		yggr_test_assert(str.length() == 10);
		str.replace(1, 5, 7, 'a');
		yggr_test_assert(str.length() == 12);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
		yggr_test_assert(str.length() == 10);
#ifndef YGGR_NO_CWCHAR
		str.replace(1, 5, 7, L'��');
#else
        str.replace(1, 5, 7, "��");
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 12);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
		yggr_test_assert(str.length() == 10);

		std::cout << yggr::utf8_char(L'Ŷ') << std::endl;
#ifndef YGGR_NO_CWCHAR
		str.replace(8, 2, 7, yggr::utf8_char(L'Ŷ'));
#else
        str.replace(8, 2, 7, yggr::utf8_char("Ŷ"));
#endif //__GNUC__
		yggr_test_assert(str.length() == 15);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
		yggr_test_assert(str.length() == 10);
#ifndef YGGR_NO_CWCHAR
		str.replace(0, 6, L"�ַ���");
#else
        str.replace(0, 6, "�ַ���");
#endif // YGGR_NO_CWCHAR
		int n = str.length();
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
		yggr_test_assert(str.length() == 10);
		str.replace(7, 3, "string");
		yggr_test_assert(str.length() == 13);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
		yggr_test_assert(str.length() == 10);
#ifndef YGGR_NO_CWCHAR
		str.replace(0, 6, yggr::wstring(L"�ַ���"), 0, 2);
#else
        str.replace(0, 6, yggr::string("�ַ���"), 0, 4);
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 6);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
		yggr::string sstr("abc");
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
		yggr::string sstr("abc");
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
#ifndef YGGR_NO_CWCHAR
		yggr::wstring sstr(L"�����");
#else
        yggr::string sstr("�����");
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
#ifndef YGGR_NO_CWCHAR
		str = "string �ַ���";
		yggr::wstring sstr(L"�����");
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
#endif // YGGR_NO_CWCHAR
	}

	{
		str = "string �ַ���";
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string sstr(L"�����");
#else
        yggr::utf8_string sstr("�����");
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string sstr(L"�����");
#else
        yggr::utf8_string sstr("�����");
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string sstr(L"�����");
#else
        yggr::utf8_string sstr("�����");
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr);
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string sstr(L"�����");
#else
        yggr::utf8_string sstr("�����");
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 10);
		str.replace(0, 6, sstr, 0, 1);
		yggr_test_assert(str.length() == 5);
		std::cout << str << std::endl;
	}
}

void test_string_operations(void)
{

#ifndef YGGR_NO_CWCHAR
	yggr::utf8_string str(L"utf8�ַ���");
#else
    yggr::utf8_string str("utf8�ַ���");
#endif // YGGR_NO_CWCHAR

	// find
	{
		yggr_test_assert(str.find('u') == 0);
		yggr_test_assert(str.find('u', 1) == yggr::utf8_string::npos);
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find(L'��') == 4);
		yggr_test_assert(str.find(L'��', 5) == yggr::utf8_string::npos);
		yggr_test_assert(str.find(yggr::utf8_char(L'��')) == 4);
		yggr_test_assert(str.find(yggr::utf8_char(L'��'), 5) == yggr::utf8_string::npos);
#else
        yggr_test_assert(str.find("��") == 4);
		yggr_test_assert(str.find("��", 5) == yggr::utf8_string::npos);
		yggr_test_assert(str.find(yggr::utf8_char("��")) == 4);
		yggr_test_assert(str.find(yggr::utf8_char("��"), 5) == yggr::utf8_string::npos);
#endif // YGGR_NO_CWCHAR
	}

	{
		yggr::string sstr("utf8");
		yggr_test_assert(str.find(sstr) == 0);
		yggr_test_assert(str.find("utf8") == 0);
		yggr_test_assert(str.find(sstr, 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find("utf8", 1) == yggr::utf8_string::npos);
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr::wstring sstr(L"�ַ���");
		yggr_test_assert(str.find(sstr) == 4);
		yggr_test_assert(str.find(L"�ַ���") == 4);
		yggr_test_assert(str.find(sstr, 5) == yggr::utf8_string::npos);
		yggr_test_assert(str.find(L"�ַ���", 5) == yggr::utf8_string::npos);
#else
        yggr::string sstr("�ַ���");
        yggr_test_assert(str.find(sstr) == 4);
		yggr_test_assert(str.find("�ַ���") == 4);
		yggr_test_assert(str.find(sstr, 5) == yggr::utf8_string::npos);
		yggr_test_assert(str.find("�ַ���", 5) == yggr::utf8_string::npos);
#endif // YGGR_NO_CWCHAR
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string sstr(L"tf8�ַ���");
#else
        yggr::utf8_string sstr("tf8�ַ���");
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.find(sstr) == 1);
		yggr_test_assert(str.find(sstr, 2) == yggr::utf8_string::npos);
	}

	// rfind
	{
		yggr_test_assert(str.rfind('u') == 0);
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.rfind(L'��') == 4);
		yggr_test_assert(str.rfind(L'��', 3) == yggr::utf8_string::npos);
		yggr_test_assert(str.rfind(yggr::utf8_char(L'��')) == 4);
		yggr_test_assert(str.rfind(yggr::utf8_char(L'��'), 3) == yggr::utf8_string::npos);
#else
        yggr_test_assert(str.rfind("��") == 4);
		yggr_test_assert(str.rfind("��", 3) == yggr::utf8_string::npos);
		yggr_test_assert(str.rfind(yggr::utf8_char("��")) == 4);
		yggr_test_assert(str.rfind(yggr::utf8_char("��"), 3) == yggr::utf8_string::npos);
#endif // YGGR_NO_CWCHAR
	}

	{
		yggr::string sstr("utf8");
		yggr_test_assert(str.rfind(sstr) == 0);
		yggr_test_assert(str.rfind("utf8") == 0);
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr::wstring sstr(L"�ַ���");
		yggr_test_assert(str.rfind(sstr) == 4);
		yggr_test_assert(str.rfind(L"�ַ���") == 4);
		yggr_test_assert(str.rfind(sstr, 3) == yggr::utf8_string::npos);
		yggr_test_assert(str.rfind(L"�ַ���", 3) == yggr::utf8_string::npos);
#else
        yggr::string sstr("�ַ���");
		yggr_test_assert(str.rfind(sstr) == 4);
		yggr_test_assert(str.rfind("�ַ���") == 4);
		yggr_test_assert(str.rfind(sstr, 3) == yggr::utf8_string::npos);
		yggr_test_assert(str.rfind("�ַ���", 3) == yggr::utf8_string::npos);
#endif // YGGR_NO_CWCHAR
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string sstr(L"tf8�ַ���");
#else
        yggr::utf8_string sstr("tf8�ַ���");
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.rfind(sstr) == 1);
		yggr_test_assert(str.rfind(sstr, 0) == yggr::utf8_string::npos);
	}

	// find_first_of
	{
		//str == "utf8�ַ���"
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_first_of(L'��') == 4);
		yggr_test_assert(str.find_first_of(L'��', 1) == 4);
		yggr_test_assert(str.find_first_of(L'��', 5) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of('a') == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of("��abc") == 4);
		yggr_test_assert(str.find_first_of("abc") == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of(L"��abc") == 4);
		yggr_test_assert(str.find_first_of(L"abc") == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of(L"��abc", 1) == 4);
		yggr_test_assert(str.find_first_of(L"��abc",5) == yggr::utf8_string::npos);
#else
        yggr_test_assert(str.find_first_of("��") == 4);
		yggr_test_assert(str.find_first_of("��", 1) == 4);
		yggr_test_assert(str.find_first_of("��", 5) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of('a') == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of("��abc") == 4);
		yggr_test_assert(str.find_first_of("abc") == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of("��abc") == 4);
		yggr_test_assert(str.find_first_of("abc") == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of("��abc", 1) == 4);
		yggr_test_assert(str.find_first_of("��abc",5) == yggr::utf8_string::npos);
#endif // YGGR_NO_CWCHAR
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_first_of(yggr::utf8_string(L"��abc")) == 4);
		yggr_test_assert(str.find_first_of(yggr::utf8_string(L"abc")) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of(yggr::utf8_string(L"��abc"), 1) == 4);
		yggr_test_assert(str.find_first_of(yggr::utf8_string(L"��abc"),5) == yggr::utf8_string::npos);
#else
        yggr_test_assert(str.find_first_of(yggr::utf8_string("��abc")) == 4);
		yggr_test_assert(str.find_first_of(yggr::utf8_string("abc")) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of(yggr::utf8_string("��abc"), 1) == 4);
		yggr_test_assert(str.find_first_of(yggr::utf8_string("��abc"),5) == yggr::utf8_string::npos);
#endif // YGGR_NO_CWCHAR
	}

	// find first_not_of

	{
		//str == "utf8�ַ���"
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_first_not_of(L'��') == 0);
		yggr_test_assert(str.find_first_not_of(L'��', 1) == 1);
		yggr_test_assert(str.find_first_not_of(L'��', 5) == 5);
		yggr_test_assert(str.find_first_not_of('a') == 0);
		yggr_test_assert(str.find_first_not_of("��abc") == 0);
		yggr_test_assert(str.find_first_not_of("abc") == 0);
		yggr_test_assert(str.find_first_not_of(L"��abc") == 0);
		yggr_test_assert(str.find_first_not_of(L"abc") == 0);
		yggr_test_assert(str.find_first_not_of(L"��abc", 1) == 1);
		yggr_test_assert(str.find_first_not_of(L"��abc",5) == 5);
		yggr_test_assert(str.find_first_not_of(L"utf8�ַ���", 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_not_of(L"", str.length()) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_not_of(L"", str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_first_not_of(L"") == 0);
#else
        yggr_test_assert(str.find_first_not_of("��") == 0);
		yggr_test_assert(str.find_first_not_of("��", 1) == 1);
		yggr_test_assert(str.find_first_not_of("��", 5) == 5);
		yggr_test_assert(str.find_first_not_of('a') == 0);
		yggr_test_assert(str.find_first_not_of("��abc") == 0);
		yggr_test_assert(str.find_first_not_of("abc") == 0);
		yggr_test_assert(str.find_first_not_of("��abc") == 0);
		yggr_test_assert(str.find_first_not_of("abc") == 0);
		yggr_test_assert(str.find_first_not_of("��abc", 1) == 1);
		yggr_test_assert(str.find_first_not_of("��abc",5) == 5);
		yggr_test_assert(str.find_first_not_of("utf8�ַ���", 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_not_of("", str.length()) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_not_of("", str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_first_not_of("") == 0);
#endif // YGGR_NO_CWCHAR
	}

	{

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string(L"��abc")) == 0);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string(L"abc")) == 0);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string(L"��abc"), 1) == 1);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string(L"��abc"),5) == 5);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string(L"utf8�ַ���"), 1) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_first_not_of(yggr::utf8_string(L""), str.length()) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string(L""), str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string(L"")) == 0);
#else
        yggr_test_assert(str.find_first_not_of(yggr::utf8_string("��abc")) == 0);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string("abc")) == 0);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string("��abc"), 1) == 1);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string("��abc"),5) == 5);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string("utf8�ַ���"), 1) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_first_not_of(yggr::utf8_string(""), str.length()) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string(""), str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_first_not_of(yggr::utf8_string("")) == 0);
#endif // YGGR_NO_CWCHAR

	}

	// find_last_of
	{
		//str == "utf8�ַ���"
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_of(L'��') == 4);
		yggr_test_assert(str.find_last_of(L'��', 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(L'��', 5) == 4);
		yggr_test_assert(str.find_last_of('a') == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of("��abc") == 4);
		yggr_test_assert(str.find_last_of("abc") == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(L"��abc") == 4);
		yggr_test_assert(str.find_last_of(L"abc") == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(L"��abc", 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(L"��abc",5) == 4);
		yggr_test_assert(str.find_last_of('u', 0) == 0);
		yggr_test_assert(str.find_last_of("u", 0) == 0);
#else
        yggr_test_assert(str.find_last_of("��") == 4);
		yggr_test_assert(str.find_last_of("��", 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of("��", 5) == 4);
		yggr_test_assert(str.find_last_of('a') == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of("��abc") == 4);
		yggr_test_assert(str.find_last_of("abc") == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of("��abc") == 4);
		yggr_test_assert(str.find_last_of("abc") == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of("��abc", 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of("��abc",5) == 4);
		yggr_test_assert(str.find_last_of('u', 0) == 0);
		yggr_test_assert(str.find_last_of("u", 0) == 0);
#endif // YGGR_NO_CWCHAR
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_of(yggr::utf8_string(L"��abc")) == 4);
		yggr_test_assert(str.find_last_of(yggr::utf8_string(L"abc")) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::utf8_string(L"��abc"), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::utf8_string(L"��abc"),5) == 4);
#else
        yggr_test_assert(str.find_last_of(yggr::utf8_string("��abc")) == 4);
		yggr_test_assert(str.find_last_of(yggr::utf8_string("abc")) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::utf8_string("��abc"), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::utf8_string("��abc"),5) == 4);
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_of(yggr::utf8_string("u"), 0) == 0);
	}

	// find_last_of
	{
//#ifndef YGGR_NO_CWCHAR
#ifndef _MSC_VER
		//string_view
		yggr_test_assert(str.find_last_of(yggr::wstring_view(L"��abc")) == 4);
		yggr_test_assert(str.find_last_of(yggr::wstring_view(L"abc")) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::wstring_view(L"��abc"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_last_of(yggr::wstring_view(L"abc"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��abc"))) == 4);
		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"abc"))) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::wstring_view(L"��abc"), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::wstring_view(L"��abc"), 5) == 4);

		yggr_test_assert(str.find_last_of(yggr::wstring_view(L"��abc"), 1, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::wstring_view(L"��abc"), 5, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 4);

		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��abc")), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"��abc")), 5) == 4);

		// utf8_string_view
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string(L"��abc"))) == 4);
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string(L"abc"))) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string(L"��abc")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string(L"abc")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��abc")))) == 4);
		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"abc")))) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string(L"��abc")), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string(L"��abc")), 5) == 4);

		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string(L"��abc")), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string(L"��abc")), 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);

		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��abc"))), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"��abc"))), 5) == 4);

#else
		// string_view
		yggr_test_assert(str.find_last_of(yggr::string_view("��abc")) == 4);
		yggr_test_assert(str.find_last_of(yggr::string_view("abc")) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::string_view("��abc"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_last_of(yggr::string_view("abc"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::string_view("��abc"))) == 4);
		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::string_view("abc"))) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::string_view("��abc"), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::string_view("��abc"), 5) == 4);

		yggr_test_assert(str.find_last_of(yggr::string_view("��abc"), 1, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::string_view("��abc"), 5, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 4);

		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::string_view("��abc")), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::string_view("��abc")), 5) == 4);

		// utf8_string_view
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string("��abc"))) == 4);
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string("abc"))) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string("��abc")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string("abc")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("��abc")))) == 4);
		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("abc")))) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string("��abc")), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string("��abc")), 5) == 4);

		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string("��abc")), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view(yggr::utf8_string("��abc")), 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);

		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("��abc"))), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("��abc"))), 5) == 4);

#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_of(yggr::utf8_string_view("u"), 0) == 0);
	}

	// find_last_not_of
	{
		//str == "utf8�ַ���"
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_not_of(L'��') == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(L'��', 1) == 1);
		yggr_test_assert(str.find_last_not_of(L'��', 5) == 5);
		yggr_test_assert(str.find_last_not_of('a') == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("��abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(L"��abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(L"abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(L"��abc", 1) == 1);
		yggr_test_assert(str.find_last_not_of(L"��abc",5) == 5);
		yggr_test_assert(str.find_last_not_of(L"utf8�ַ���", 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_not_of(L"", 0) == 0);
		yggr_test_assert(str.find_last_not_of(L"", str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(L"") == str.length() - 1);
#else
        yggr_test_assert(str.find_last_not_of("��") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("��", 1) == 1);
		yggr_test_assert(str.find_last_not_of("��", 5) == 5);
		yggr_test_assert(str.find_last_not_of('a') == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("��abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("��abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("��abc", 1) == 1);
		yggr_test_assert(str.find_last_not_of("��abc",5) == 5);
		yggr_test_assert(str.find_last_not_of("utf8�ַ���", 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_not_of("", 0) == 0);
		yggr_test_assert(str.find_last_not_of("", str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("") == str.length() - 1);
#endif // YGGR_NO_CWCHAR
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string(L"��abc")) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string(L"abc")) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string(L"��abc"), 1) == 1);
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string(L"��abc"),5) == 5);
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string(L"utf8�ַ���"), 1) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_not_of(yggr::utf8_string(L""), 0) == 0);
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string(L""), str.length() - 1) == str.length() - 1);
#else
        yggr_test_assert(str.find_last_not_of(yggr::utf8_string("��abc")) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string("abc")) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string("��abc"), 1) == 1);
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string("��abc"),5) == 5);
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string("utf8�ַ���"), 1) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_not_of(yggr::utf8_string(""), 0) == 0);
		yggr_test_assert(str.find_last_not_of(yggr::utf8_string(""), str.length() - 1) == str.length() - 1);
#endif // YGGR_NO_CWCHAR
	}

	// compare_eq
	{
		//str = "utf8�ַ���";
		//const char*
		yggr_test_assert(str.compare_eq("utf8") == false);
		yggr_test_assert(str.compare_eq("utf8_string") == false);
		yggr_test_assert(str.compare_eq("utf8�ַ�����") == false);
		yggr_test_assert(str.compare_eq("") == false);
		yggr_test_assert(str.compare_eq("utf8�ַ���") == true);

		// std::string
		yggr_test_assert(str.compare_eq(std::string("utf8")) == false);
		yggr_test_assert(str.compare_eq(std::string("utf8_string")) == false);
		yggr_test_assert(str.compare_eq(std::string("utf8�ַ�����")) == false);
		yggr_test_assert(str.compare_eq(std::string("")) == false);
		yggr_test_assert(str.compare_eq(std::string("utf8�ַ���")) == true);

		// boost::container::string
		yggr_test_assert(str.compare_eq(boost::container::string("utf8")) == false);
		yggr_test_assert(str.compare_eq(boost::container::string("utf8_string")) == false);
		yggr_test_assert(str.compare_eq(boost::container::string("utf8�ַ�����")) == false);
		yggr_test_assert(str.compare_eq(boost::container::string("")) == false);
		yggr_test_assert(str.compare_eq(boost::container::string("utf8�ַ���")) == true);

		// yggr::string
		yggr_test_assert(str.compare_eq(yggr::string("utf8")) == false);
		yggr_test_assert(str.compare_eq(yggr::string("utf8_string")) == false);
		yggr_test_assert(str.compare_eq(yggr::string("utf8�ַ�����")) == false);
		yggr_test_assert(str.compare_eq(yggr::string("")) == false);
		yggr_test_assert(str.compare_eq(yggr::string("utf8�ַ���")) == true);

		// yggr::utf8_string
		yggr_test_assert(str.compare_eq(yggr::utf8_string("utf8")) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string("utf8_string")) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string("utf8�ַ�����")) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string("")) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string("utf8�ַ���")) == true);

		// yggr::string_view
		yggr_test_assert(str.compare_eq(yggr::string_view("utf8")) == false);
		yggr_test_assert(str.compare_eq(yggr::string_view("utf8_string")) == false);
		yggr_test_assert(str.compare_eq(yggr::string_view("utf8�ַ�����")) == false);
		yggr_test_assert(str.compare_eq(yggr::string_view("")) == false);
		yggr_test_assert(str.compare_eq(yggr::string_view("utf8�ַ���")) == true);

		// yggr::utf8_string_view
		yggr_test_assert(str.compare_eq(yggr::utf8_string_view(yggr::utf8_string("utf8"))) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string_view(yggr::utf8_string("utf8_string"))) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����"))) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string_view(yggr::utf8_string(""))) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ���"))) == true);

#ifndef YGGR_NO_CWCHAR
		// const char*
		yggr_test_assert(str.compare_eq(L"utf8") == false);
		yggr_test_assert(str.compare_eq(L"utf8_string") == false);
		yggr_test_assert(str.compare_eq(L"utf8�ַ�����") == false);
		yggr_test_assert(str.compare_eq(L"") == false);
		yggr_test_assert(str.compare_eq(L"utf8�ַ���") == true);

		// std::string
		yggr_test_assert(str.compare_eq(std::wstring(L"utf8")) == false);
		yggr_test_assert(str.compare_eq(std::wstring(L"utf8_string")) == false);
		yggr_test_assert(str.compare_eq(std::wstring(L"utf8�ַ�����")) == false);
		yggr_test_assert(str.compare_eq(std::wstring(L"")) == false);
		yggr_test_assert(str.compare_eq(std::wstring(L"utf8�ַ���")) == true);

		// boost::container::string
		yggr_test_assert(str.compare_eq(boost::container::wstring(L"utf8")) == false);
		yggr_test_assert(str.compare_eq(boost::container::wstring(L"utf8_string")) == false);
		yggr_test_assert(str.compare_eq(boost::container::wstring(L"utf8�ַ�����")) == false);
		yggr_test_assert(str.compare_eq(boost::container::wstring(L"")) == false);
		yggr_test_assert(str.compare_eq(boost::container::wstring(L"utf8�ַ���")) == true);

		// yggr::string
		yggr_test_assert(str.compare_eq(yggr::wstring(L"utf8")) == false);
		yggr_test_assert(str.compare_eq(yggr::wstring(L"utf8_string")) == false);
		yggr_test_assert(str.compare_eq(yggr::wstring(L"utf8�ַ�����")) == false);
		yggr_test_assert(str.compare_eq(yggr::wstring(L"")) == false);
		yggr_test_assert(str.compare_eq(yggr::wstring(L"utf8�ַ���")) == true);

		// yggr::string_view
		yggr_test_assert(str.compare_eq(yggr::wstring_view(L"utf8")) == false);
		yggr_test_assert(str.compare_eq(yggr::wstring_view(L"utf8_string")) == false);
		yggr_test_assert(str.compare_eq(yggr::wstring_view(L"utf8�ַ�����")) == false);
		yggr_test_assert(str.compare_eq(yggr::wstring_view(L"")) == false);
		yggr_test_assert(str.compare_eq(yggr::wstring_view(L"utf8�ַ���")) == true);

		// yggr::utf8_string_view
		yggr_test_assert(str.compare_eq(yggr::utf8_string_view(yggr::utf8_string(L"utf8"))) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string_view(yggr::utf8_string(L"utf8_string"))) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ�����"))) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string_view(yggr::utf8_string(L""))) == false);
		yggr_test_assert(str.compare_eq(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ���"))) == true);

#else
        yggr_test_assert(str.compare_eq("utf8") == false );
		yggr_test_assert(str.compare_eq("utf8_string") == false );
		yggr_test_assert(str.compare_eq("utf8�ַ�����") == false);
		yggr_test_assert(str.compare_eq("") == false);
		yggr_test_assert(str.compare_eq("utf8�ַ���") == true);
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(str.compare_eq(0, str.length(), "utf8", 0, 1) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), "utf8_string", 2, 7) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), "utf8", 0, 0) == false);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare_eq(0, str.length(), L"utf8", 0, 1) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), L"utf8_string", 2, 7) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), L"utf8�ַ���������", 0, 8) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), L"utf8", 0, 0) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), L"��utf8�ַ�������", 1, 7) == true);
#else
        yggr_test_assert(str.compare_eq(0, str.length(), "utf8", 0, 1) == false );
		yggr_test_assert(str.compare_eq(0, str.length(), "utf8_string", 2, 7) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), "utf8", 0, 0) == false);
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare_eq(4, 2, "�ַ�") == true);
		yggr_test_assert(str.compare_eq(4, 2, L"�ַ�") == true);
		yggr_test_assert(str.compare_eq(4, 2, yggr::string("�ַ�")) == true);
		yggr_test_assert(str.compare_eq(4, 2, yggr::wstring(L"�ַ�")) == true);

		yggr_test_assert(str.compare_eq(4, 2, "�ַ���") == false);
		yggr_test_assert(str.compare_eq(4, 2, L"�ַ���") == false);
		yggr_test_assert(str.compare_eq(4, 2, yggr::string("�ַ���")) == false);
		yggr_test_assert(str.compare_eq(4, 2, yggr::wstring(L"�ַ���")) == false);

		yggr_test_assert(str.compare_eq(4, 3, "�ַ�") == false);
		yggr_test_assert(str.compare_eq(4, 3, L"�ַ�") == false);
		yggr_test_assert(str.compare_eq(4, 3, yggr::string("�ַ�")) == false);
		yggr_test_assert(str.compare_eq(4, 3, yggr::wstring(L"�ַ�")) == false);

		yggr_test_assert(str.compare_eq(4, 2, yggr::utf8_string("�ַ�")) == true);
		yggr_test_assert(str.compare_eq(4, 2, yggr::utf8_string(L"�ַ�")) == true);

		yggr_test_assert(str.compare_eq(4, 2, yggr::utf8_string("�ַ���")) == false);
		yggr_test_assert(str.compare_eq(4, 2, yggr::wstring(L"�ַ���")) == false);

		yggr_test_assert(str.compare_eq(4, 3, yggr::utf8_string("�ַ�")) == false);
		yggr_test_assert(str.compare_eq(4, 3, yggr::utf8_string(L"�ַ�")) == false);
#else
        yggr_test_assert(str.compare_eq(4, 2, "�ַ�") == true);
		yggr_test_assert(str.compare_eq(4, 2, yggr::string("�ַ�")) == true);

		yggr_test_assert(str.compare_eq(4, 2, "�ַ���") == false);
		yggr_test_assert(str.compare_eq(4, 2, yggr::string("�ַ���")) == false);

		yggr_test_assert(str.compare_eq(4, 3, "�ַ�") == false);
		yggr_test_assert(str.compare_eq(4, 3, yggr::string("�ַ�")) == false);

		yggr_test_assert(str.compare_eq(4, 2, yggr::utf8_string("�ַ�")) == true);

		yggr_test_assert(str.compare_eq(4, 2, yggr::utf8_string("�ַ���")) == false);

		yggr_test_assert(str.compare_eq(4, 3, yggr::utf8_string("�ַ�")) == false);
#endif // YGGR_NO_CWCHAR
	}

	// compare
	{
		//str = "utf8�ַ���";
		yggr_test_assert(str.compare("utf8") > 0);
		yggr_test_assert(str.compare("utf8_string") > 0);
		yggr_test_assert(str.compare("utf8�ַ�����") < 0);
		yggr_test_assert(str.compare("") > 0);
		yggr_test_assert(str.compare("utf8�ַ���") == 0);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare(L"utf8") > 0);
		yggr_test_assert(str.compare(L"utf8_string") > 0);
		yggr_test_assert(str.compare(L"utf8�ַ�����") < 0);
		yggr_test_assert(str.compare(L"") > 0);
		yggr_test_assert(str.compare(L"utf8�ַ���") == 0);
#else
        yggr_test_assert(str.compare("utf8") > 0 );
		yggr_test_assert(str.compare("utf8_string") > 0 );
		yggr_test_assert(str.compare("utf8�ַ�����") < 0);
		yggr_test_assert(str.compare("") > 0);
		yggr_test_assert(str.compare("utf8�ַ���") == 0);
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(str.compare(0, str.length(), "utf8", 0, 1) > 0);
		yggr_test_assert(str.compare(0, str.length(), "utf8_string", 2, 7) > 0);
		yggr_test_assert(str.compare(0, str.length(), "utf8", 0, 0) > 0);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare(0, str.length(), L"utf8", 0, 1) > 0);
		yggr_test_assert(str.compare(0, str.length(), L"utf8_string", 2, 7) > 0);
		yggr_test_assert(str.compare(0, str.length(), L"utf8�ַ���������", 0, 8) < 0);
		yggr_test_assert(str.compare(0, str.length(), L"utf8", 0, 0) > 0);
		yggr_test_assert(str.compare(0, str.length(), L"��utf8�ַ�������", 1, 7) == 0);
#else
        yggr_test_assert(str.compare(0, str.length(), "utf8", 0, 1) > 0 );
		yggr_test_assert(str.compare(0, str.length(), "utf8_string", 2, 7) > 0);
		yggr_test_assert(str.compare(0, str.length(), "utf8", 0, 0) > 0);
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare(4, 2, "�ַ�") == 0);
		yggr_test_assert(str.compare(4, 2, L"�ַ�") == 0);
		yggr_test_assert(str.compare(4, 2, yggr::string("�ַ�")) == 0);
		yggr_test_assert(str.compare(4, 2, yggr::wstring(L"�ַ�")) == 0);

		yggr_test_assert(str.compare(4, 2, "�ַ���") < 0);
		yggr_test_assert(str.compare(4, 2, L"�ַ���") < 0);
		yggr_test_assert(str.compare(4, 2, yggr::string("�ַ���")) < 0);
		yggr_test_assert(str.compare(4, 2, yggr::wstring(L"�ַ���")) < 0);

		yggr_test_assert(str.compare(4, 3, "�ַ�") > 0);
		yggr_test_assert(str.compare(4, 3, L"�ַ�") > 0);
		yggr_test_assert(str.compare(4, 3, yggr::string("�ַ�")) > 0);
		yggr_test_assert(str.compare(4, 3, yggr::wstring(L"�ַ�")) > 0);

		yggr_test_assert(str.compare(4, 2, yggr::utf8_string("�ַ�")) == 0);
		yggr_test_assert(str.compare(4, 2, yggr::utf8_string(L"�ַ�")) == 0);

		yggr_test_assert(str.compare(4, 2, yggr::utf8_string("�ַ���")) < 0);
		yggr_test_assert(str.compare(4, 2, yggr::wstring(L"�ַ���")) < 0);

		yggr_test_assert(str.compare(4, 3, yggr::utf8_string("�ַ�")) > 0);
		yggr_test_assert(str.compare(4, 3, yggr::utf8_string(L"�ַ�")) > 0);
#else
        yggr_test_assert(str.compare(4, 2, "�ַ�") == 0);
		yggr_test_assert(str.compare(4, 2, yggr::string("�ַ�")) == 0);

		yggr_test_assert(str.compare(4, 2, "�ַ���") < 0);
		yggr_test_assert(str.compare(4, 2, yggr::string("�ַ���")) < 0);

		yggr_test_assert(str.compare(4, 3, "�ַ�") > 0);
		yggr_test_assert(str.compare(4, 3, yggr::string("�ַ�")) > 0);

		yggr_test_assert(str.compare(4, 2, yggr::utf8_string("�ַ�")) == 0);

		yggr_test_assert(str.compare(4, 2, yggr::utf8_string("�ַ���")) < 0);

		yggr_test_assert(str.compare(4, 3, yggr::utf8_string("�ַ�")) > 0);
#endif // YGGR_NO_CWCHAR

		//string_view
		{
#ifndef YGGR_NO_CWCHAR
			yggr_test_assert(str.compare(yggr::wstring_view(L"utf8")) > 0);
			yggr_test_assert(str.compare(yggr::wstring_view(L"utf8_string")) > 0);
			yggr_test_assert(str.compare(yggr::wstring_view(L"utf8�ַ�����")) < 0);
			yggr_test_assert(str.compare(yggr::wstring_view(L"")) > 0);
			yggr_test_assert(str.compare(yggr::wstring_view(L"utf8�ַ���")) == 0);

			yggr_test_assert(str.compare(yggr::wstring_view(L"utf8"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::wstring_view(L"utf8_string"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::wstring_view(L"utf8�ַ�����"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) < 0);
			yggr_test_assert(str.compare(yggr::wstring_view(L""), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::wstring_view(L"utf8�ַ���"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 0);

			yggr_test_assert(str.compare(4, 2, yggr::wstring_view(L"�ַ�")) == 0);
			yggr_test_assert(str.compare(4, 2, yggr::wstring_view(L"�ַ���")) < 0);
			yggr_test_assert(str.compare(4, 3, yggr::wstring_view(L"�ַ�")) > 0);

			yggr_test_assert(str.compare(4, 2, yggr::wstring_view(L"�ַ�"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) == 0);
			yggr_test_assert(str.compare(4, 2, yggr::wstring_view(L"�ַ���"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) < 0);
			yggr_test_assert(str.compare(4, 3, yggr::wstring_view(L"�ַ�"), YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8"), 0) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8_string"), 2) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8"), 1) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8"), 0, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8_string"), 2, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8"), 1, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8"), 0, 1) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8_string"), 2, 7) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8"), 0, 0) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8"), 0, 1, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8_string"), 2, 7, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::wstring_view(L"utf8"), 0, 0, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME()) > 0);

#else
			yggr_test_assert(str.compare(yggr::string_view("utf8")) > 0);
			yggr_test_assert(str.compare(yggr::string_view("utf8_string")) > 0);
			yggr_test_assert(str.compare(yggr::string_view("utf8�ַ�����")) < 0);
			yggr_test_assert(str.compare(yggr::string_view("")) > 0);
			yggr_test_assert(str.compare(yggr::string_view("utf8�ַ���")) == 0);

			yggr_test_assert(str.compare(yggr::string_view("utf8"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::string_view("utf8_string"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::string_view("utf8�ַ�����"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) < 0);
			yggr_test_assert(str.compare(yggr::string_view(""), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::string_view("utf8�ַ���"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 0);

			yggr_test_assert(str.compare(4, 2, yggr::string_view("�ַ�")) == 0);
			yggr_test_assert(str.compare(4, 2, yggr::string_view("�ַ���")) < 0);
			yggr_test_assert(str.compare(4, 3, yggr::string_view("�ַ�")) > 0);

			yggr_test_assert(str.compare(4, 2, yggr::string_view("�ַ�"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) == 0);
			yggr_test_assert(str.compare(4, 2, yggr::string_view("�ַ���"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) < 0);
			yggr_test_assert(str.compare(4, 3, yggr::string_view("�ַ�"), YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8"), 0) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8_string"), 2) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8"), 1) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8"), 0, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8_string"), 2, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8"), 1, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8"), 0, 1) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8_string"), 2, 7) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8"), 0, 0) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8"), 0, 1, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8_string"), 2, 7, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::string_view("utf8"), 0, 0, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME()) > 0);

#endif // YGGR_NO_CWCHAR
		}

		//utf8_string
		{
#ifndef YGGR_NO_CWCHAR
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(L"utf8"))) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(L"utf8_string"))) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ�����"))) < 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(L""))) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ���"))) == 0);

			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(L"utf8")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(L"utf8_string")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ�����")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) < 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(L"")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ���")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);

			yggr_test_assert(str.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string(L"�ַ�"))) == 0);
			yggr_test_assert(str.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string(L"�ַ���"))) < 0);
			yggr_test_assert(str.compare(4, 3, yggr::utf8_string_view(yggr::utf8_string(L"�ַ�"))) > 0);

			yggr_test_assert(str.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string(L"�ַ�")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);
			yggr_test_assert(str.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string(L"�ַ���")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) < 0);
			yggr_test_assert(str.compare(4, 3, yggr::utf8_string_view(yggr::utf8_string(L"�ַ�")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8")), 0) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8_string")), 2) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8")), 1) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8")), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8_string")), 2, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8")), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8")), 0, 1) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8_string")), 2, 7) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8")), 0, 0) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8")), 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8_string")), 2, 7, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string(L"utf8")), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

#else
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string("utf8"))) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string("utf8_string"))) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����"))) < 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string(""))) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ���"))) == 0);

			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string("utf8")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string("utf8_string")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) < 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string("")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ���")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);

			yggr_test_assert(str.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string("�ַ�"))) == 0);
			yggr_test_assert(str.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string("�ַ���"))) < 0);
			yggr_test_assert(str.compare(4, 3, yggr::utf8_string_view(yggr::utf8_string("�ַ�"))) > 0);

			yggr_test_assert(str.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string("�ַ�")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);
			yggr_test_assert(str.compare(4, 2, yggr::utf8_string_view(yggr::utf8_string("�ַ���")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) < 0);
			yggr_test_assert(str.compare(4, 3, yggr::utf8_string_view(yggr::utf8_string("�ַ�")), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8")), 0) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8_string")), 2) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8")), 1) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8")), 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8_string")), 2, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8")), 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8")), 0, 1) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8_string")), 2, 7) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8")), 0, 0) > 0);

			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8")), 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0 );
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8_string")), 2, 7, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
			yggr_test_assert(str.compare(0, str.length(), yggr::utf8_string_view(yggr::utf8_string("utf8")), 0, 0, YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

#endif // YGGR_NO_CWCHAR
		}
	}
}

void non_member_foo_test(void)
{
	// operator +
	{
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string str("utf8�ַ���");
		std::cout << "utf8�ַ�" + str + "aaa" << std::endl;
		std::cout << L"utf8�ַ�" + str + L"bbb" << std::endl;

		std::cout << yggr::string("utf8�ַ�") + str << std::endl;
		std::wcout << yggr::wstring(L"utf8�ַ�") + str << std::endl;

		std::cout << "utf8�ַ�" + str + yggr::string("aaa") << std::endl;
		std::cout << L"utf8�ַ�" + str + yggr::wstring(L"bbb") << std::endl;

		std::cout << yggr::utf8_string("utf8�ַ�") + str << std::endl;
		const yggr::utf8_string& cref_str = str;
		std::cout << yggr::utf8_string("utf8�ַ�") + cref_str << std::endl;

		std::cout << yggr::utf8_string("utf8�ַ�") + str + yggr::string("aaa") << std::endl;
		std::cout << yggr::utf8_string(L"utf8�ַ�") + str + yggr::wstring(L"bbb") << std::endl;

		std::cout << yggr::utf8_string("utf8�ַ�") + yggr::string_view("aaa") << std::endl;
		std::cout << yggr::string_view("utf8�ַ�") + yggr::utf8_string("aaa") << std::endl;

		std::cout << yggr::utf8_string("utf8�ַ�") + yggr::charset::make_string_charset_helper(yggr::string_view("aaa")) << std::endl;
		std::cout << yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�")) + yggr::utf8_string("aaa") << std::endl;

		std::cout << yggr::utf8_string("utf8�ַ�") + yggr::wstring_view(L"aaa") << std::endl;
		std::cout << yggr::wstring_view(L"utf8�ַ�") + yggr::utf8_string("aaa") << std::endl;

		std::cout << yggr::utf8_string("utf8�ַ�") + yggr::charset::make_string_charset_helper(yggr::wstring_view(L"aaa")) << std::endl;
		std::cout << yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8�ַ�")) + yggr::utf8_string("aaa") << std::endl;
#else
        yggr::utf8_string str("utf8�ַ���");
		std::cout << "utf8�ַ�" + str + "aaa" << std::endl;

		std::cout << yggr::string("utf8�ַ�") + str << std::endl;

		std::cout << "utf8�ַ�" + str + yggr::string("aaa") << std::endl;

		std::cout << yggr::utf8_string("utf8�ַ�") + str + yggr::string("aaa") << std::endl;

		std::cout << yggr::utf8_string("utf8�ַ�") + yggr::string_view("aaa") << std::endl;
		std::cout << yggr::string_view("utf8�ַ�") + yggr::utf8_string("aaa") << std::endl;

		std::cout << yggr::utf8_string("utf8�ַ�") + yggr::charset::make_string_charset_helper(yggr::string_view("aaa")) << std::endl;
		std::cout << yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�")) + yggr::utf8_string("aaa") << std::endl;

#endif // YGGR_NO_CWCHAR

		std::cout << yggr::utf8_string("utf8�ַ�") + yggr::utf8_string_view("aaa") << std::endl;
		std::cout << yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�")) + yggr::utf8_string("aaa") << std::endl;
	}

	// operator +=
	{
		yggr::utf8_string str("utf8�ַ���");

		{
			std::string rst_str("operaotr+= ");
			rst_str += str;
			yggr_test_assert(rst_str == "operaotr+= utf8�ַ���");
			std::cout << rst_str << std::endl;
		}

		{
			boost::container::string rst_str("operaotr+= ");
			rst_str += str;
			yggr_test_assert(rst_str == "operaotr+= utf8�ַ���");
			std::cout << rst_str << std::endl;
		}

		{
			yggr::string rst_str("operaotr+= ");
			rst_str += str;
			yggr_test_assert(rst_str == "operaotr+= utf8�ַ���");
			std::cout << rst_str << std::endl;
		}

		// string_view
		{
			yggr::utf8_string rst_str = str;
			rst_str += yggr::string_view(" operaotr+=");
			yggr_test_assert(rst_str == "utf8�ַ��� operaotr+=");
			std::cout << rst_str << std::endl;
		}

		{
			yggr::utf8_string rst_str = str;
			rst_str += yggr::charset::make_string_charset_helper(yggr::string_view(" operaotr+="));
			yggr_test_assert(rst_str == "utf8�ַ��� operaotr+=");
			std::cout << rst_str << std::endl;
		}

		{
			yggr::utf8_string rst_str = str;
			rst_str += yggr::utf8_string_view(" operaotr+=");
			yggr_test_assert(rst_str == "utf8�ַ��� operaotr+=");
			std::cout << rst_str << std::endl;
		}

		{
			yggr::utf8_string rst_str = str;
			rst_str += yggr::charset::make_string_charset_helper(yggr::utf8_string_view(" operaotr+="));
			yggr_test_assert(rst_str == "utf8�ַ��� operaotr+=");
			std::cout << rst_str << std::endl;
		}

#ifndef YGGR_NO_CWCHAR
		{
			std::wstring rst_wstr(L"operaotr+= ");
			rst_wstr += str;
			yggr_test_assert(rst_wstr == L"operaotr+= utf8�ַ���");

			std::wcout << rst_wstr << std::endl;
		}

		{
			boost::container::wstring rst_wstr(L"operaotr+= ");
			rst_wstr += str;
			yggr_test_assert(rst_wstr == L"operaotr+= utf8�ַ���");

			std::wcout << rst_wstr << std::endl;
		}

		{
			yggr::wstring rst_wstr(L"operaotr+= ");
			rst_wstr += str;
			yggr_test_assert(rst_wstr == L"operaotr+= utf8�ַ���");
			
			std::wcout << rst_wstr << std::endl;
		}

		// string_view
		{
			yggr::utf8_string rst_str = str;
			rst_str += yggr::wstring_view(L" operaotr+=");
			yggr_test_assert(rst_str == L"utf8�ַ��� operaotr+=");
			std::cout << rst_str << std::endl;
		}

		{
			yggr::utf8_string rst_str = str;
			rst_str += yggr::charset::make_string_charset_helper(yggr::wstring_view(L" operaotr+="));
			yggr_test_assert(rst_str == L"utf8�ַ��� operaotr+=");
			std::cout << rst_str << std::endl;
		}

		{
			yggr::utf8_string rst_str = str;
			rst_str += yggr::utf8_string_view(yggr::utf8_string(L" operaotr+="));
			yggr_test_assert(rst_str == L"utf8�ַ��� operaotr+=");
			std::cout << rst_str << std::endl;
		}

		{
			yggr::utf8_string rst_str = str;
			rst_str += yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L" operaotr+=")));
			yggr_test_assert(rst_str == L"utf8�ַ��� operaotr+=");
			std::cout << rst_str << std::endl;
		}

#endif // YGGR_NO_CWCHAR
	}

	// operator ==
	{
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string str("utf8�ַ���");

		yggr_test_assert(str == "utf8�ַ���");
		yggr_test_assert("utf8�ַ���" == str);
		yggr_test_assert(str == L"utf8�ַ���");
		yggr_test_assert(L"utf8�ַ���" == str);

		yggr_test_assert(str == yggr::string("utf8�ַ���"));
		yggr_test_assert(yggr::string("utf8�ַ���") == str);
		yggr_test_assert(str == yggr::wstring(L"utf8�ַ���"));
		yggr_test_assert(yggr::wstring(L"utf8�ַ���") == str);

		yggr_test_assert(str == yggr::utf8_string("utf8�ַ���"));
		yggr_test_assert(yggr::utf8_string("utf8�ַ���") == str);
		yggr_test_assert(str == yggr::utf8_string(L"utf8�ַ���"));
		yggr_test_assert(yggr::utf8_string(L"utf8�ַ���") == str);

		yggr_test_assert(str == yggr::wstring_view(L"utf8�ַ���"));
		yggr_test_assert(yggr::wstring_view(L"utf8�ַ���") == str);

		yggr_test_assert(str == yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8�ַ���")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8�ַ���")) == str);

		yggr_test_assert(str == yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ���")));
		yggr_test_assert(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ���")) == str);

		yggr_test_assert(str == yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ���"))));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ���"))) == str);

#else
        yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str == "utf8�ַ���");
		yggr_test_assert("utf8�ַ���" == str);

		yggr_test_assert(str == yggr::string("utf8�ַ���"));
		yggr_test_assert(yggr::string("utf8�ַ���") == str);

		yggr_test_assert(str == yggr::utf8_string("utf8�ַ���"));
		yggr_test_assert(yggr::utf8_string("utf8�ַ���") == str);

		yggr_test_assert(str == yggr::string_view("utf8�ַ���"));
		yggr_test_assert(yggr::string_view("utf8�ַ���") == str);

		yggr_test_assert(str == yggr::utf8_string_view(yggr::utf8_string("utf8�ַ���")));
		yggr_test_assert(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ���")) == str);

		yggr_test_assert(str == yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ���"))));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ���"))) == str);

#endif // YGGR_NO_CWCHAR
	}

	// operator !=
	{
		yggr::utf8_string str("utf8�ַ���");

		yggr_test_assert(str != "utf8");
		yggr_test_assert("utf8" != str);
		yggr_test_assert(str != "");
		yggr_test_assert("" != str);

		yggr_test_assert(str != yggr::string("utf8"));
		yggr_test_assert(yggr::string("utf8") != str);
		yggr_test_assert(str != yggr::string(""));
		yggr_test_assert(yggr::string("") != str);

		yggr_test_assert(str != yggr::utf8_string("utf8"));
		yggr_test_assert(yggr::utf8_string("utf8") != str);
		yggr_test_assert(str != yggr::utf8_string(""));
		yggr_test_assert(yggr::utf8_string("") != str);

		yggr_test_assert(str != yggr::string_view("utf8"));
		yggr_test_assert(yggr::string_view("utf8") != str);

		yggr_test_assert(str != yggr::charset::make_string_charset_helper(yggr::string_view("utf8")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string_view("utf8")) != str);

		yggr_test_assert(str != yggr::utf8_string_view(yggr::utf8_string("utf8")));
		yggr_test_assert(yggr::utf8_string_view(yggr::utf8_string("utf8")) != str);

		yggr_test_assert(str != yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8"))));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8"))) != str);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str != L"utf8");
		yggr_test_assert(L"utf8" != str);
		yggr_test_assert(str != L"");
		yggr_test_assert(L"" != str);

		yggr_test_assert(str != yggr::wstring(L"utf8"));
		yggr_test_assert(yggr::wstring(L"utf8") != str);
		yggr_test_assert(str != yggr::wstring(L""));
		yggr_test_assert(yggr::wstring(L"") != str);

		yggr_test_assert(str != yggr::utf8_string(L"utf8"));
		yggr_test_assert(yggr::utf8_string(L"utf8") != str);
		yggr_test_assert(str != yggr::utf8_string(L""));
		yggr_test_assert(yggr::utf8_string(L"") != str);

		yggr_test_assert(str != yggr::wstring_view(L"utf8"));
		yggr_test_assert(yggr::wstring_view(L"utf8") != str);

		yggr_test_assert(str != yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8")));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8")) != str);

		yggr_test_assert(str != yggr::utf8_string_view(yggr::utf8_string(L"utf8")));
		yggr_test_assert(yggr::utf8_string_view(yggr::utf8_string(L"utf8")) != str);

		yggr_test_assert(str != yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"utf8"))));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"utf8"))) != str);

#endif // YGGR_NO_CWCHAR
	}

	// operator <
	{
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string str("utf8�ַ���");

		yggr_test_assert(str < "utf8�ַ�����");
		yggr_test_assert(str < L"utf8�ַ�����");
		yggr_test_assert(str < yggr::string("utf8�ַ�����"));
		yggr_test_assert(str < yggr::wstring(L"utf8�ַ�����"));
		yggr_test_assert("" < str );

		yggr_test_assert(!("utf8�ַ�����" < str));
		yggr_test_assert(!(L"utf8�ַ�����" < str));
		yggr_test_assert(!(yggr::string("utf8�ַ�����") < str));
		yggr_test_assert(!(yggr::wstring(L"utf8�ַ�����") < str));
		yggr_test_assert(!(str < yggr::wstring(L"")));

		yggr_test_assert(str < yggr::utf8_string("utf8�ַ�����"));
		yggr_test_assert(!(yggr::utf8_string(L"utf8�ַ�����") < str));

		yggr_test_assert(str < yggr::wstring_view(L"utf8�ַ�����"));
		yggr_test_assert(!(yggr::wstring_view(L"utf8�ַ�����") < str));

		yggr_test_assert(str < yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8�ַ�����")));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::wstring_view(L"utf8�ַ�����")) < str));

		yggr_test_assert(str < yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ�����")));
		yggr_test_assert(!(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ�����")) < str));

		yggr_test_assert(str < yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ�����"))));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string(L"utf8�ַ�����"))) < str));

#else
        yggr::utf8_string str("utf8�ַ���");

		yggr_test_assert(str < "utf8�ַ�����");
		yggr_test_assert(str < yggr::string("utf8�ַ�����"));
		yggr_test_assert("" < str );

		yggr_test_assert(!("utf8�ַ�����" < str));
		yggr_test_assert(!(yggr::string("utf8�ַ�����") < str));
		yggr_test_assert(!(str < yggr::string("")));

		yggr_test_assert(str < yggr::utf8_string("utf8�ַ�����"));

		yggr_test_assert(str < yggr::string_view("utf8�ַ�����"));
		yggr_test_assert(!(yggr::string_view("utf8�ַ�����") < str));

		yggr_test_assert(str < yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�����")));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�����")) < str));

		yggr_test_assert(str < yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����")));
		yggr_test_assert(!(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����")) < str));

		yggr_test_assert(str < yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����"))));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����"))) < str));

#endif // YGGR_NO_CWCHAR
	}

	// operator <=
	{
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str <= "utf8�ַ�����");
		yggr_test_assert(str <= "utf8�ַ���");
		yggr_test_assert(str <= L"utf8�ַ�����");
		yggr_test_assert(str <= L"utf8�ַ���");
		yggr_test_assert(str <= yggr::string("utf8�ַ�����"));
		yggr_test_assert(str <= yggr::string("utf8�ַ���"));
		yggr_test_assert(str <= yggr::wstring(L"utf8�ַ�����"));
		yggr_test_assert(str <= yggr::wstring(L"utf8�ַ���"));

		yggr_test_assert(!("utf8�ַ�����" <= str));
		yggr_test_assert(!(L"utf8�ַ�����" <= str));
		yggr_test_assert(!(yggr::string("utf8�ַ�����") <= str));
		yggr_test_assert(!(yggr::wstring(L"utf8�ַ�����") <= str));

		yggr_test_assert(str <= yggr::utf8_string("utf8�ַ�����"));
		yggr_test_assert(!(yggr::utf8_string(L"utf8�ַ�����") <= str));

		yggr_test_assert(str <= yggr::string_view("utf8�ַ�����"));
		yggr_test_assert(!(yggr::string_view("utf8�ַ�����") <= str));

		yggr_test_assert(str <= yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�����")));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�����")) <= str));

		yggr_test_assert(str <= yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����")));
		yggr_test_assert(!(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����")) <= str));

		yggr_test_assert(str <= yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����"))));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����"))) <= str));

#else
        yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str <= "utf8�ַ�����");
		yggr_test_assert(str <= "utf8�ַ���");
		yggr_test_assert(str <= yggr::string("utf8�ַ�����"));
		yggr_test_assert(str <= yggr::string("utf8�ַ���"));

		yggr_test_assert(!("utf8�ַ�����" <= str));
		yggr_test_assert(!(yggr::string("utf8�ַ�����") <= str));

		yggr_test_assert(str <= yggr::utf8_string("utf8�ַ�����"));

		yggr_test_assert(str <= yggr::string_view("utf8�ַ�����"));
		yggr_test_assert(!(yggr::string_view("utf8�ַ�����") <= str));

		yggr_test_assert(str <= yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�����")));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�����")) <= str));

		yggr_test_assert(str <= yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����")));
		yggr_test_assert(!(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����")) <= str));

		yggr_test_assert(str <= yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����"))));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�����"))) <= str));

#endif // YGGR_NO_CWCHAR
	}

	// operator >
	{
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str > "utf8�ַ�");
		yggr_test_assert(str > L"utf8�ַ�");
		yggr_test_assert(str > yggr::string("utf8�ַ�"));
		yggr_test_assert(str > yggr::wstring(L"utf8�ַ�"));
		yggr_test_assert(str > "" );

		yggr_test_assert(!("utf8�ַ�" > str));
		yggr_test_assert(!(L"utf8�ַ�" > str));
		yggr_test_assert(!(yggr::string("utf8�ַ�") > str));
		yggr_test_assert(!(yggr::wstring(L"utf8�ַ�") > str));

		yggr_test_assert(str > yggr::utf8_string("utf8�ַ�"));
		yggr_test_assert(!(yggr::utf8_string(L"utf8�ַ�") > str));

		yggr_test_assert(str > yggr::string_view("utf8�ַ�"));
		yggr_test_assert(!(yggr::string_view("utf8�ַ�") > str));

		yggr_test_assert(str > yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�")));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�")) > str));

		yggr_test_assert(str > yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�")));
		yggr_test_assert(!(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�")) > str));

		yggr_test_assert(str > yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�"))));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�"))) > str));
#else
        yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str > "utf8�ַ�");
		yggr_test_assert(str > yggr::string("utf8�ַ�"));
		yggr_test_assert(str > "" );

		yggr_test_assert(!("utf8�ַ�" > str));
		yggr_test_assert(!(yggr::string("utf8�ַ�") > str));

		yggr_test_assert(str > yggr::utf8_string("utf8�ַ�"));

		yggr_test_assert(str > yggr::string_view("utf8�ַ�"));
		yggr_test_assert(!(yggr::string_view("utf8�ַ�") > str));

		yggr_test_assert(str > yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�")));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�")) > str));

		yggr_test_assert(str > yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�")));
		yggr_test_assert(!(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�")) > str));

		yggr_test_assert(str > yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�"))));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�"))) > str));

#endif // YGGR_NO_CWCHAR
	}

	// operator >=
	{
#ifndef YGGR_NO_CWCHAR
		yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str >= "utf8�ַ�");
		yggr_test_assert(str >= "utf8�ַ���");
		yggr_test_assert(str >= L"utf8�ַ�");
		yggr_test_assert(str >= L"utf8�ַ���");
		yggr_test_assert(str >= yggr::string("utf8�ַ�"));
		yggr_test_assert(str >= yggr::string("utf8�ַ���"));
		yggr_test_assert(str >= yggr::wstring(L"utf8�ַ�"));
		yggr_test_assert(str >= yggr::wstring(L"utf8�ַ���"));

		yggr_test_assert(!("utf8�ַ�" >= str));
		yggr_test_assert(!(L"utf8�ַ�" >= str));
		yggr_test_assert(!(yggr::string("utf8�ַ�") >= str));
		yggr_test_assert(!(yggr::wstring(L"utf8�ַ�") >= str));

		yggr_test_assert(str >= yggr::utf8_string("utf8�ַ�"));
		yggr_test_assert(!(yggr::utf8_string(L"utf8�ַ�") >= str));

		yggr_test_assert(str >= yggr::string_view("utf8�ַ�"));
		yggr_test_assert(!(yggr::string_view("utf8�ַ�") >= str));

		yggr_test_assert(str >= yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�")));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�")) >= str));

		yggr_test_assert(str >= yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�")));
		yggr_test_assert(!(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�")) >= str));

		yggr_test_assert(str >= yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�"))));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�"))) >= str));

#else
        yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str >= "utf8�ַ�");
		yggr_test_assert(str >= "utf8�ַ���");
		yggr_test_assert(str >= yggr::string("utf8�ַ�"));
		yggr_test_assert(str >= yggr::string("utf8�ַ���"));

		yggr_test_assert(!("utf8�ַ�" >= str));
		yggr_test_assert(!(yggr::string("utf8�ַ�") >= str));

		yggr_test_assert(str >= yggr::utf8_string("utf8�ַ�"));

		yggr_test_assert(str >= yggr::string_view("utf8�ַ�"));
		yggr_test_assert(!(yggr::string_view("utf8�ַ�") >= str));

		yggr_test_assert(str >= yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�")));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string_view("utf8�ַ�")) >= str));

		yggr_test_assert(str >= yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�"))));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::utf8_string_view(yggr::utf8_string("utf8�ַ�"))) >= str));

#endif // YGGR_NO_CWCHAR
	}

	// swap
	{
		yggr::utf8_string str1_chk("utf8�ַ���1");
		yggr::utf8_string str2_chk("utf8�ַ���2");

		yggr::utf8_string str1(str1_chk);
		yggr::utf8_string str2(str2_chk);

		yggr_test_assert(str1 == str1_chk);
		yggr_test_assert(str2 == str2_chk);

		std::swap(str1, str2);
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);

		boost::swap(str1, str2);
		yggr_test_assert(str1 == str1_chk);
		yggr_test_assert(str2 == str2_chk);

		std::swap(boost::move(str1), str2);
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);

		boost::swap(boost::move(str1), str2);
		yggr_test_assert(str1 == str1_chk);
		yggr_test_assert(str2 == str2_chk);

		std::swap(str1, boost::move(str2));
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);

		boost::swap(str1, boost::move(str2));
		yggr_test_assert(str1 == str1_chk);
		yggr_test_assert(str2 == str2_chk);
	}

	{
		yggr::utf8_string str1_chk("utf8�ַ���111");
		yggr::utf8_string str2_chk("utf8�ַ���222");

		yggr::utf8_string str1(str1_chk);
		yggr::utf8_string str2(str2_chk);

		//std::string stl_str1 = str1.str<std::string>();
		//std::wstring stl_str2 = str2.str<std::wstring>();

		std::string stl_str1 = str1;
#ifndef YGGR_NO_CWCHAR
		std::wstring stl_str2 = str2;
#else
		std::string stl_str2 = str2;
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(stl_str1 == str1_chk);
		yggr_test_assert(str2_chk == stl_str2);

		std::swap(stl_str1, str2);
		yggr_test_assert(str2_chk == stl_str1);

		std::swap(str1, stl_str2);
		yggr_test_assert(stl_str2 == str1_chk);

		boost::swap(stl_str1, str2);
		boost::swap(str1, stl_str2);

		yggr_test_assert(stl_str1 == str1_chk);
		yggr_test_assert(str2_chk == stl_str2);

		// yggr_swap

		yggr::swap(stl_str1, str2);
		yggr_test_assert(str2_chk == stl_str1);

		yggr::swap(str1, stl_str2);
		yggr_test_assert(stl_str2 == str1_chk);

		std::swap(stl_str1, str2);
		std::swap(str1, stl_str2);

		yggr_test_assert(stl_str1 == str1_chk);
		yggr_test_assert(str2_chk == stl_str2);
	}

}

void test_std_replace(void)
{
	{
		yggr::utf8_string str = "abcdefg";
		std::replace(str.begin(), str.end(), 'd', 'z');
		std::replace_if(str.begin(), str.end(),
							boost::bind(std::equal_to<yggr::utf8_string::char_type>(),
											yggr::utf8_string::char_type('a'), _1), 'z');
		yggr_test_assert(str == "zbczefg");
	}

	{
		yggr::utf8_string str = "abcdefg";
		std::replace(boost::begin(str), boost::end(str), 'd', 'z');
		std::replace_if(boost::begin(str), boost::end(str),
							boost::bind(std::equal_to<yggr::utf8_string::char_type>(),
											yggr::utf8_string::char_type('a'), _1), 'z');
		yggr_test_assert(str == "zbczefg");
	}

#ifndef YGGR_NO_CWCHAR

	{
		yggr::utf8_string str = L"���,����";
		std::replace(str.begin(), str.end(), L'��', L'��');
		std::replace_if(str.begin(), str.end(),
							boost::bind(std::equal_to<yggr::utf8_string::char_type>(),
											yggr::utf8_string::char_type(','), _1), '-');
		yggr_test_assert(str == L"����-����");
	}

	{
		yggr::utf8_string str = L"���,����";
		std::replace(boost::begin(str), boost::end(str), L'��', L'��');
		std::replace_if(boost::begin(str), boost::end(str),
							boost::bind(std::equal_to<yggr::utf8_string::char_type>(),
											yggr::utf8_string::char_type(','), _1), '-');
		yggr_test_assert(str == L"����-����");
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_std_replace succeed" << std::endl;
}

void test_io(void)
{
	std::cout << "test_io start" << std::endl;

	yggr::utf8_string in;
	std::cin >> in;
	std::cout << in << std::endl;

#ifndef YGGR_NO_CWCHAR
	std::wcin >> in;
	std::cout << in << std::endl;
#endif // YGGR_NO_CWCHAR

	std::cout << "test_io succeed" << std::endl;
}

void test_basic_string_getline(void)
{
	std::cout << "test_basic_string_getline start" << std::endl;

#ifndef YGGR_NO_CWCHAR

	{
		std::wstring str;
		yggr::charset::getline(std::wcin, str);

		std::wcout << str << std::endl;
	}

	{
		boost::container::wstring str;
		yggr::charset::getline(std::wcin, str, L'\n');

		std::wcout << str << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_basic_string_getline success" << std::endl;
}

void test_utf8_string_getline(void)
{
	std::cout << "test_utf8_string_getline start" << std::endl;

	// char
	{
		yggr::charset::utf8_string str;
		getline(std::cin, str);

		std::cout << str << std::endl;
	}

	{
		yggr::charset::utf8_string str;
		getline(std::cin, str, ' ');

		std::cout << str << std::endl;
	}

	//Char
	{
		yggr::charset::utf8_string str;
		getline(std::cin, str, '\n');

		std::cout << str << std::endl;
	}

	// const Char*
	{
		yggr::charset::utf8_string str;
		getline(std::cin, str, "\n");

		std::cout << str << std::endl;
	}

#if	0
	{
		yggr::charset::utf8_string str;
		getline(std::cin, str, "ab"); // yggr_test_assert(false), because dim's length > 1

		std::cout << str << std::endl;
	}
#endif // 0, 1

	// const basic_string
	{
		yggr::charset::utf8_string str;
		getline(std::cin, str, std::string("\n"));

		std::cout << str << std::endl;
	}

	{
		yggr::charset::utf8_string str;
		getline(std::cin, str, boost::container::string("\n"));

		std::cout << str << std::endl;
	}

	// utf8_char
	{
		yggr::charset::utf8_string str;
		getline(std::cin, str, yggr::charset::utf8_string::char_type("\n"));

		std::cout << str << std::endl;
	}

#ifndef YGGR_NO_CWCHAR
	// char
	{
		yggr::charset::utf8_string str;
		getline(std::wcin, str);

		std::cout << str << std::endl;
	}

	{
		yggr::charset::utf8_string str;
		getline(std::wcin, str, ' ');

		std::cout << str << std::endl;
	}

	{
		yggr::charset::utf8_string str;
		getline(std::cin, str, L' ');

		std::wcout << str << std::endl;
	}

	//Char
	{
		yggr::charset::utf8_string str;
		getline(std::wcin, str, L'��');

		std::cout << str << std::endl;
	}

	// const Char*
	{
		yggr::charset::utf8_string str;
		getline(std::wcin, str, "��");

		std::wcout << str << std::endl;
	}

	// const basic_string
	{
		yggr::charset::utf8_string str;
		getline(std::wcin, str, std::wstring(L"\n"));

		std::wcout << str << std::endl;
	}

	{
		yggr::charset::utf8_string str;
		getline(std::wcin, str, boost::container::wstring(L"\n"));

		std::wcout << str << std::endl;
	}

	// utf8_char
	{
		yggr::charset::utf8_string str;
		getline(std::wcin, str, yggr::charset::utf8_string::char_type("\n"));

		std::cout << str << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_utf8_string_getline success" << std::endl;
}

void test_container_conv(void)
{

	{
		yggr::utf8_string str = "abc";
		std::vector<yggr::utf8_string::char_type> vt(str.begin(), str.end());

		yggr_test_assert(str.length() == vt.size());

		{
			for(std::size_t i = 0, isize = str.length(); i != isize; ++i)
			{
				yggr_test_assert(str[i] == vt[i]);
			}
		}

		vt.assign(str.cbegin(), str.cend());
		yggr_test_assert(str.length() == vt.size());

		{
			for(std::size_t i = 0, isize = str.length(); i != isize; ++i)
			{
				yggr_test_assert(str[i] == vt[i]);
			}
		}
	}

	{
		yggr::utf8_string str = "abc";
		std::list<yggr::utf8_string::char_type> vt(str.begin(), str.end());

		yggr_test_assert(str.length() == vt.size());

		{
			std::list<yggr::utf8_string::char_type>::const_iterator iter = vt.begin();
			for(std::size_t i = 0, isize = str.length(); i != isize; ++i, ++iter)
			{
				yggr_test_assert(str[i] == *iter);
			}
		}

		vt.assign(str.cbegin(), str.cend());
		yggr_test_assert(str.length() == vt.size());

		{
			std::list<yggr::utf8_string::char_type>::const_iterator iter = vt.begin();
			for(std::size_t i = 0, isize = str.length(); i != isize; ++i, ++iter)
			{
				yggr_test_assert(str[i] == *iter);
			}
		}
	}

	std::cout << "test_container_conv succeed" << std::endl;
}


int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

#if defined(TEST_CONSTRUCT)
	test_construct();
#endif // TEST_CONSTRUCT

	test_operator_set();
	test_swap();

	test_append();
	test_push_back();
	test_pop_back();
	test_insert();
	test_replace();
	test_find();
	test_rfind();
	test_find_first_of();
	test_find_first_not_of();
	test_find_last_of();
	test_find_last_not_of();
	test_resize_relength();
	test_copy();

	test_iterator();
	test_element_access();
	test_modify();
	test_string_operations();
	non_member_foo_test();

	test_std_replace();

	test_container_conv();

#	ifdef TEST_IO
	test_basic_string_getline();
	test_utf8_string_getline();
	test_io();
#	endif // TEST_IO

	wait_any_key(argc, argv);
	return 0;
}
