//isomeric_same_utf8_string_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

//#  define YGGR_NO_CWCHAR
//#define YGGR_USE_STL_STRING
//#define _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/compile_link/linker.hpp>

#if defined(_MSC_VER)
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
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)
	
	{
		{
			//const string&
			utf8_string_type str2(isomeric_same_string_type("abc"));
			yggr_test_assert(str2 == "abc");

			utf8_string_type str4(isomeric_same_string_type("abc"), yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "abc");

			utf8_string_type str6(yggr::charset::make_string_charset_helper(isomeric_same_string_type("abc")));
			yggr_test_assert(str6 == "abc");
		}

		{
			//const utf8_string&
			utf8_string_type str1 = isomeric_same_utf8_string_type("abc");
			yggr_test_assert(str1 == "abc");

			utf8_string_type str3(isomeric_same_utf8_string_type("abc"), yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "abc");

			utf8_string_type str4(isomeric_same_utf8_string_type("abc"), 
									yggr::charset::charset_name_t<isomeric_same_utf8_string_type>());
			yggr_test_assert(str4 == "abc");

			utf8_string_type str5(isomeric_same_utf8_string_type("abc"),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == "abc");

			utf8_string_type str6(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("abc")));
			yggr_test_assert(str6 == "abc");

			utf8_string_type str7(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("abc")),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "abc");
		}

		{
			// const string pos
			
			utf8_string_type str3(isomeric_same_utf8_string_type("abcde"), 3, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "de");

			utf8_string_type str5(isomeric_same_utf8_string_type("abcde"), 3,
									utf8_string_type::allocator_type(),
									yggr::charset::charset_name_t<utf8_string_type>());
			yggr_test_assert(str5 == "de");

			utf8_string_type str7(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("abcde")), 3,
										yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "de");
		}

		{
			// const string pos
			utf8_string_type str1(isomeric_same_utf8_string_type("abcde"), 3);
			yggr_test_assert(str1 == "de");

			utf8_string_type str3(isomeric_same_utf8_string_type("abcde"), 3, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "de");

			utf8_string_type str4(isomeric_same_utf8_string_type("abcde"), 3, yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "de");

			utf8_string_type str5(isomeric_same_utf8_string_type("abcde"), 3,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == "de");

			utf8_string_type str6(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("abcde")), 3);
			yggr_test_assert(str6 == "de");

			utf8_string_type str7(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("abcde")), 3,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "de");

			// right usage
			utf8_string_type str8(isomeric_same_utf8_string_type("abc���"), 4);
			yggr_test_assert(str8 == "��"); // because utf8_string can correctly recognize utf8_char
		}

		{
			// const string pos, len

			utf8_string_type str3(isomeric_same_string_type("abcde"), 3, 1, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "d");

			utf8_string_type str5(isomeric_same_string_type("abcde"), 3, 1,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<isomeric_same_string_type>());
			yggr_test_assert(str5 == "d");

			utf8_string_type str6(yggr::charset::make_string_charset_helper(isomeric_same_string_type("abcde")), 3, 1);
			yggr_test_assert(str6 == "d");
		}

		{
			// const utf8_string pos, len
			utf8_string_type str1(isomeric_same_utf8_string_type("abcde"), 3, 1);
			yggr_test_assert(str1 == "d");

			utf8_string_type str3(isomeric_same_utf8_string_type("abcde"), 3, 1, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == "d");

			utf8_string_type str4(isomeric_same_utf8_string_type("abcde"), 3, 1, yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "d");

			utf8_string_type str5(isomeric_same_utf8_string_type("abcde"), 3, 1,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == "d");

			utf8_string_type str6(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("abcde")), 3, 1);
			yggr_test_assert(str6 == "d");

			utf8_string_type str7(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("abcde")), 3, 1,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == "d");

			utf8_string_type str8(isomeric_same_utf8_string_type("abc���"), 4, 1);
			yggr_test_assert(str8 == "��");
		}
	}

	// iterator
	{
		{
			// utf8_string
			isomeric_same_utf8_string_type str_arr = "abc";

			utf8_string_type str1(str_arr.begin(), str_arr.end());
			yggr_test_assert(str1 == "abc");

			utf8_string_type str2(str_arr.begin(), str_arr.end(), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "abc");

			utf8_string_type str3(str_arr.begin(), str_arr.end(), yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str3 == "abc");

			utf8_string_type str4(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "abc");
		}

		{
			// utf8_string riter
			isomeric_same_utf8_string_type str_arr = "abc";

			utf8_string_type str1(str_arr.rbegin(), str_arr.rend());
			yggr_test_assert(str1 == "cba");

			utf8_string_type str2(str_arr.rbegin(), str_arr.rend(), yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == "cba");

			utf8_string_type str3(str_arr.rbegin(), str_arr.rend(), yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str3 == "cba");

			utf8_string_type str4(str_arr.rbegin(), str_arr.rend(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "cba");
		}
	}

#ifndef YGGR_NO_CWCHAR

	// (string)
	{
		{
			//const utf8_string&
			utf8_string_type str1 = isomeric_same_utf8_string_type(L"�����");
			yggr_test_assert(str1 == L"�����");

			utf8_string_type str2(isomeric_same_utf8_string_type(L"�����"));
			yggr_test_assert(str2 == L"�����");

			utf8_string_type str3(isomeric_same_utf8_string_type(L"�����"), yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"�����");

			utf8_string_type str4(isomeric_same_utf8_string_type(L"�����"), yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == L"�����");

			utf8_string_type str5(isomeric_same_utf8_string_type(L"�����"),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == L"�����");

			utf8_string_type str6(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type(L"�����")));
			yggr_test_assert(str6 == L"�����");

			utf8_string_type str7(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type(L"�����")),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"�����");
		}

		{
			// const string pos
			utf8_string_type str1(isomeric_same_utf8_string_type(L"����𰢴�"), 3);
			yggr_test_assert(str1 == L"����");

			utf8_string_type str3(isomeric_same_utf8_string_type(L"����𰢴�"), 3, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"����");

			utf8_string_type str4(isomeric_same_utf8_string_type(L"����𰢴�"), 3, yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == L"����");

			utf8_string_type str5(isomeric_same_utf8_string_type(L"����𰢴�"), 3,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == L"����");

			utf8_string_type str6(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type(L"����𰢴�")), 3);
			yggr_test_assert(str6 == L"����");

			utf8_string_type str7(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type(L"����𰢴�")), 3,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"����");

		}

		{
			// const utf8_string pos, len
			utf8_string_type str1(isomeric_same_utf8_string_type("����𰢴�"), 3, 1);
			yggr_test_assert(str1 == L"��");

			utf8_string_type str3(isomeric_same_utf8_string_type("����𰢴�"), 3, 1, yggr::utf8_string::allocator_type());
			yggr_test_assert(str3 == L"��");

			utf8_string_type str4(isomeric_same_utf8_string_type("����𰢴�"), 3, 1, yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == L"��");

			utf8_string_type str5(isomeric_same_utf8_string_type("����𰢴�"), 3, 1,
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str5 == L"��");

			utf8_string_type str6(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("����𰢴�")), 3, 1);
			yggr_test_assert(str6 == L"��");

			utf8_string_type str7(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("����𰢴�")), 3, 1,
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str7 == L"��");
		}
	}

	// iterator
	{
		{
			// utf8_string
			isomeric_same_utf8_string_type str_arr = L"�����";

			utf8_string_type str1(str_arr.begin(), str_arr.end());
			yggr_test_assert(str1 == L"�����");

			utf8_string_type str2(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == L"�����");

			utf8_string_type str3(str_arr.begin(), str_arr.end(),
										yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str3 == "�����");

			utf8_string_type str4(str_arr.begin(), str_arr.end(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "�����");
		}

		{
			// utf8_string riter
			isomeric_same_utf8_string_type str_arr = L"��hao��";

			utf8_string_type str1(str_arr.rbegin(), str_arr.rend());
			yggr_test_assert(str1 == L"��oah��");

			utf8_string_type str2(str_arr.rbegin(), str_arr.rend(),
									yggr::utf8_string::allocator_type());
			yggr_test_assert(str2 == L"��oah��");

			utf8_string_type str3(str_arr.rbegin(), str_arr.rend(),
										yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str3 == "��oah��");

			utf8_string_type str4(str_arr.rbegin(), str_arr.rend(),
									yggr::utf8_string::allocator_type(),
									yggr::charset::charset_name_t<yggr::utf8_string>());
			yggr_test_assert(str4 == "��oah��");
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_construct successed" << std::endl;

}

#endif // TEST_CONSTRUCT

void test_operator_set(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	{
		isomeric_same_string_type src_str = "a��b��c";
		isomeric_same_utf8_string_type str_chk(src_str);
		utf8_string_type str;
		str = boost::move(src_str);
		yggr_test_assert(str == str_chk);
	}

	{

		isomeric_same_string_type src_str = "a��b��c";
		isomeric_same_utf8_string_type str_chk(src_str);
		utf8_string_type str;
		str = src_str;
		yggr_test_assert(str == str_chk);
	}

	{
		isomeric_same_utf8_string_type src_str = "a��b��c";
		isomeric_same_utf8_string_type str_chk(src_str);
		utf8_string_type str;
		str = boost::move(src_str);
		yggr_test_assert(str == str_chk);
	}

	{

		isomeric_same_utf8_string_type src_str = "a��b��c";
		isomeric_same_utf8_string_type str_chk(src_str);
		utf8_string_type str;
		str = src_str;
		yggr_test_assert(str == str_chk);
	}

	std::cout << "test_set sucessed" << std::endl;
}

void test_swap(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	{
		isomeric_same_string_type str1("abc");
		utf8_string_type str("def");

		str.swap(boost::move(str1));

		yggr_test_assert(str == "abc");
		yggr_test_assert(str1 == "def");
	}

	{
		isomeric_same_string_type str1("abc");
		utf8_string_type str("def");

		str.swap(str1);

		yggr_test_assert(str == "abc");
		yggr_test_assert(str1 == "def");
	}
	
	{
		isomeric_same_utf8_string_type str1("abc");
		utf8_string_type str("def");

		str.swap(boost::move(str1));

		yggr_test_assert(str == "abc");
		yggr_test_assert(str1 == "def");
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		utf8_string_type str("def");

		str.swap(str1);

		yggr_test_assert(str == "abc");
		yggr_test_assert(str1 == "def");
	}
}

void test_append(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// string
	{
		// string, pos
		{
			utf8_string_type str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(isomeric_same_string_type("world"), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == "a��b��cld");
		}

		// utf8_string
		{
			utf8_string_type str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(isomeric_same_utf8_string_type("world"));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == "a��b��cworld");
		}

		{
			utf8_string_type str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(isomeric_same_utf8_string_type("world"), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == "a��b��cld");
		}

		{
			utf8_string_type str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(isomeric_same_utf8_string_type("world"), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "a��b��cl");
		}

		{
			utf8_string_type str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(isomeric_same_string_type("world")), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == "a��b��cld");
		}

		{
			utf8_string_type str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("world")));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == "a��b��cworld");
		}

		{
			utf8_string_type str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("world")), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == "a��b��cld");
		}

		{
			utf8_string_type str = "a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("world")), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "a��b��cl");
		}

	}

	// append(iters, itere)
	{
		{
			isomeric_same_string_type ins_str = " world";
			utf8_string_type str = "hello";

			str.append(ins_str.rbegin(), ins_str.rend());
			yggr_test_assert(str.length() == 11);
			yggr_test_assert(str == "hellodlrow ");
		}

		{
			isomeric_same_utf8_string_type ins_str = " world";
			utf8_string_type str = "hello";

			str.append(ins_str.rbegin(), ins_str.rend());
			yggr_test_assert(str.length() == 11);
			yggr_test_assert(str == "hellodlrow ");
		}
	}

#ifndef YGGR_NO_CWCHAR
	// n, char
	{
		// n utf8_string
		{
			isomeric_same_utf8_string_type str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::utf8_char("��"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c����");
		}

		{
			utf8_string_type str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type::char_type(L"��")));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == L"a��b��c����");
		}
	}

	// string
	{
		// utf8_string
		{
			utf8_string_type str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append( isomeric_same_utf8_string_type(L"���`��m"));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == L"a��b��c���`��m");
		}

		{
			utf8_string_type str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append( isomeric_same_utf8_string_type(L"���`��m"), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"a��b��c��m");
		}

		{
			utf8_string_type str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append( isomeric_same_utf8_string_type(L"���`��m"), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == L"a��b��c��");
		}

		{
			utf8_string_type str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type(L"���`��m")));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == L"a��b��c���`��m");
		}

		{
			utf8_string_type str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type(L"���`��m")), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"a��b��c��m");
		}

		{
			utf8_string_type str = L"a��b��c";
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type(L"���`��m")), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == L"a��b��c��");
		}

	}

	// append(iters, itere)
	{
		{
			std::wstring ins_str = L" ���`��";
			isomeric_same_utf8_string_type str = L"���";

			str.append(ins_str.begin(), ins_str.end());
			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"��� ���`��");
		}

		{
			isomeric_same_utf8_string_type ins_str = L" ���`��";
			utf8_string_type str = L"���";

			str.append(ins_str.rbegin(), ins_str.rend());
			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == L"��åɩ`��� ");
		}
	}

#endif // YGGR_NO_CWCHAR
	std::cout << "test_append sucessed" << std::endl;
}


void test_insert(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// insert(pos, n, char)
	{
		{
			utf8_string_type str = "abc";
			str.insert(0, 2, isomeric_same_string_type("q"));
			yggr_test_assert(str == "qqabc");
			str.insert(1, 2, isomeric_same_string_type("k"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper(isomeric_same_string_type("t")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(iter_p, n, char)
	{
		{
			utf8_string_type str = "abc";
			str.insert(str.begin(), 2, isomeric_same_string_type("q"));
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1, 2, isomeric_same_string_type("k"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper(isomeric_same_string_type("t")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(pos, const char*)
	{
		{
			utf8_string_type str = "abc";
			str.insert(0, isomeric_same_string_type("qq"));
			yggr_test_assert(str == "qqabc");
			str.insert(1, isomeric_same_string_type("kk"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(isomeric_same_string_type("ttt")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type str = "abc";
			str.insert(0, isomeric_same_string_type("qq"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str == "qqabc");
			str.insert(1, isomeric_same_string_type("kk"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(isomeric_same_string_type("ttt"), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(pos, string, subpos, sublen)
	{
		{
			isomeric_same_string_type stl_ins_str = "qqkkttt";
			isomeric_same_string_type yggr_ins_str = "qqkkttt";
			utf8_string_type str = "abc";
			str.insert(0, stl_ins_str, 0, 2);
			yggr_test_assert(str == "qqabc");
			str.insert(1,  yggr_ins_str, 2, 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(stl_ins_str), 4);
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			isomeric_same_utf8_string_type ins_str = "qqkkttt";
			utf8_string_type str = "abc";
			str.insert(0, ins_str, 0, 2);
			yggr_test_assert(str == "qqabc");
			str.insert(1,  ins_str, 2, 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(ins_str), 4);
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);

		}
	}

	std::cout << "test_insert sucessed" << std::endl;
}

void test_replace(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// replace(pos, len, n, char)
	{
		{
			utf8_string_type str = "abc";
			str.replace(1, 1, 3, isomeric_same_string_type("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper(isomeric_same_string_type("d")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}
	}

	// replace(iters, itere, n, char)
	{

		{
			utf8_string_type str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 3, isomeric_same_string_type("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper(isomeric_same_string_type("d")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}
	}

	// replace(pos, len, const char*)
	{
		{
			utf8_string_type str = "abc";
			str.replace(1, 1, isomeric_same_string_type("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(1, 1, isomeric_same_string_type("def"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(isomeric_same_string_type("def")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("def")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

	}

	// replace(iters, itere, const char*)
	{

		{
			utf8_string_type str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, isomeric_same_string_type("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, isomeric_same_utf8_string_type("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, 
							yggr::charset::make_string_charset_helper(isomeric_same_string_type("def")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2,
						yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("def")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}
	}

	// replace(pos, len, string, subpos, sublen)
	{
		isomeric_same_string_type base_rep_str = "tagdeftag";
		isomeric_same_utf8_string_type utf8_rep_str = "tagdeftag";

		{
			utf8_string_type str = "abc";
			str.replace(1, 1, base_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(1, 1, utf8_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(base_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str = "abc";
			str.replace(1, 1, yggr::charset::make_string_charset_helper(utf8_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}
	}

	//replace(i1, i2, s, e)
	{
		{
			isomeric_same_string_type rep_str = "abc";
			utf8_string_type str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.begin(), rep_str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}

		{
			isomeric_same_utf8_string_type rep_str = "abc";
			utf8_string_type str = "abc";
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.begin(), rep_str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}
	}

	std::cout << "test_replace sucessed" << std::endl;
}

void test_find(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// find(const string&, pos)
	{
		{
			utf8_string_type str = "abc";
			yggr_test_assert(str.find(isomeric_same_string_type("b")) == 1);
		}

		{
			utf8_string_type str = "abc";
			yggr_test_assert(str.find(isomeric_same_string_type("b"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			utf8_string_type str = "abc";
			yggr_test_assert(str.find(isomeric_same_utf8_string_type("b")) == 1);
		}

		{
			utf8_string_type str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(isomeric_same_string_type("b"))) == 1);
		}

		{
			utf8_string_type str = "abc";
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("b"))) == 1);
		}
	}

	std::cout << "test_find successed" << std::endl;
}

void test_rfind(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// rfind(const string&, pos)
	{
		{
			utf8_string_type str = "abc";
			yggr_test_assert(str.rfind(isomeric_same_string_type("b")) == 1);
		}

		{
			utf8_string_type str = "abc";
			yggr_test_assert(str.rfind(isomeric_same_string_type("b"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			utf8_string_type str = "abc";
			yggr_test_assert(str.rfind(isomeric_same_utf8_string_type("b")) == 1);
		}

		{
			utf8_string_type str = "abc";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(isomeric_same_string_type("b"))) == 1);
		}

		{
			utf8_string_type str = "abc";
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("b"))) == 1);
		}
	}

	std::cout << "test_rfind successed" << std::endl;
}

void test_find_first_of(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// find_first_of(const string&, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_first_of(isomeric_same_string_type("edc")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(isomeric_same_string_type("")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(isomeric_same_string_type("edc")) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(isomeric_same_utf8_string_type("edc")) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(isomeric_same_string_type("edc"))) == 2);
		}

		{
			yggr::utf8_string str = "abcdefg";
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("edc"))) == 2);
		}
	}


	std::cout << "test_find_first_of successed" << std::endl;
}

void test_find_first_not_of(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// find_first_not_of(const string&, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_first_not_of(isomeric_same_string_type("edc")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(isomeric_same_string_type(""), 3) == 3);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(isomeric_same_string_type("edc")) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(isomeric_same_utf8_string_type("edc")) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(isomeric_same_string_type("edc"))) == 2);
		}

		{
			yggr::utf8_string str = "ccabcdefg";
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("edc"))) == 2);
		}
	}

	std::cout << "test_find_first_not_of successed" << std::endl;
}

void test_find_last_of(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// find_last_of(const string&, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_last_of(isomeric_same_string_type("edc")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(isomeric_same_string_type("edc")) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(isomeric_same_utf8_string_type("edc")) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(isomeric_same_string_type("edc"))) == 4);
		}

		{
			yggr::utf8_string str = "abcdcba";
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("edc"))) == 4);
		}
	}

	std::cout << "test_find_last_of successed" << std::endl;
}

void test_find_last_not_of(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// find_last_not_of(const string&, pos)
	{
		{
			yggr::utf8_string str = "";
			yggr_test_assert(str.find_last_not_of(isomeric_same_string_type("edc")) == yggr::utf8_string::npos);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(isomeric_same_string_type(""), 3) == 3);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(isomeric_same_string_type("edc")) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(isomeric_same_utf8_string_type("edc")) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(isomeric_same_string_type("edc"))) == 6);
		}

		{
			yggr::utf8_string str = "abcdefgcc";
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(isomeric_same_utf8_string_type("edc"))) == 6);
		}
	}

	std::cout << "test_find_last_not_of successed" << std::endl;
}

//-----------------------

void test_modify(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// append_test
	yggr::utf8_string str("a��b��c");
	yggr_test_assert(str.length() == 5);

	str.append(isomeric_same_string_type("boost��boostϷboost"));
	yggr_test_assert(str.length() == 5 + 15 + 2);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str = L"ҳ��";
#else
    str = "ҳ��";
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 2);
	std::cout << str << std::endl;

	isomeric_same_string_type isomeric_same_str("���Ǻ���");
	str.append(isomeric_same_str.begin(), isomeric_same_str.end());
	yggr_test_assert(str.length() == 2 + 4);
	std::cout << str << std::endl;

	isomeric_same_utf8_string_type isomeric_same_utf8_str("����UTF8����");
	str.append(isomeric_same_utf8_str.begin(), isomeric_same_utf8_str.end());
	yggr_test_assert(str.length() == 6 + 8);
	std::cout << str << std::endl;

	str.append(isomeric_same_utf8_str.rbegin(), isomeric_same_utf8_str.rend());
	yggr_test_assert(str.length() == 22);
	std::cout << str << std::endl;

	std::cout << str.substr(1, 8) << std::endl;

	// assign_test
	str.assign(isomeric_same_string_type("isomeric_same_str_assign_����"));
	yggr_test_assert(str.length() == 27);
	std::cout << str << std::endl;

    str.assign(isomeric_same_utf8_string_type("isomeric_same_utf8_str_assign_����"));
	yggr_test_assert(str.length() == 32);
	std::cout << str << std::endl;


	isomeric_same_string_type isomeric_same_str2("iter_isomeric_same_str_assign_����");
	str.assign(isomeric_same_str2.begin(), isomeric_same_str2.end());
	yggr_test_assert(str.length() == 32);
	std::cout << str << std::endl;

	isomeric_same_utf8_string_type isomeric_same_utf8_str2("abc����");
	str.assign(isomeric_same_utf8_str2.begin(), isomeric_same_utf8_str2.end());
	yggr_test_assert(str.length() == 5);
	std::cout << str << std::endl;


	//insert test

	{
		str = "abc";
		isomeric_same_string_type str_ins("abcde");
		str.insert(str.begin(), str_ins.begin(), str_ins.end());
		yggr_test_assert(str.length() == 8);
		std::cout << str << std::endl;
	}

	{
		str = "utf8�ַ���";
		isomeric_same_utf8_string_type ustr("���Ǹ�");
		str.insert(str.begin(), ustr.begin(), ustr.end());
		yggr_test_assert(str.length() == 10);

		std::cout << str << std::endl;
	}

	// replace test

	{
		str = "string �ַ���";
		isomeric_same_string_type sstr("abc");
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = "string �ַ���";
        isomeric_same_utf8_string_type sstr("�����");
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}
}

void test_string_operations(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	yggr::utf8_string str("utf8�ַ���");

	// find
	{
		isomeric_same_string_type sstr("utf8");
		yggr_test_assert(str.find(sstr) == 0);
		yggr_test_assert(str.find("utf8") == 0);
		yggr_test_assert(str.find(sstr, 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find("utf8", 1) == yggr::utf8_string::npos);
	}

	{

        isomeric_same_utf8_string_type sstr("tf8�ַ���");
		yggr_test_assert(str.find(sstr) == 1);
		yggr_test_assert(str.find(sstr, 2) == yggr::utf8_string::npos);
	}

	// rfind
	{
		isomeric_same_string_type sstr("utf8");
		yggr_test_assert(str.rfind(sstr) == 0);
		yggr_test_assert(str.rfind("utf8") == 0);
	}

	{
		isomeric_same_utf8_string_type sstr(L"tf8�ַ���");
		yggr_test_assert(str.rfind(sstr) == 1);
		yggr_test_assert(str.rfind(sstr, 0) == yggr::utf8_string::npos);
	}

	// find_first_of
	{
        yggr_test_assert(str.find_first_of(isomeric_same_utf8_string_type("��abc")) == 4);
		yggr_test_assert(str.find_first_of(isomeric_same_utf8_string_type("abc")) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_of(isomeric_same_utf8_string_type("��abc"), 1) == 4);
		yggr_test_assert(str.find_first_of(isomeric_same_utf8_string_type("��abc"),5) == yggr::utf8_string::npos);
	}

	// find first_not_of
	{
        yggr_test_assert(str.find_first_not_of(isomeric_same_utf8_string_type("��abc")) == 0);
		yggr_test_assert(str.find_first_not_of(isomeric_same_utf8_string_type("abc")) == 0);
		yggr_test_assert(str.find_first_not_of(isomeric_same_utf8_string_type("��abc"), 1) == 1);
		yggr_test_assert(str.find_first_not_of(isomeric_same_utf8_string_type("��abc"),5) == 5);
		yggr_test_assert(str.find_first_not_of(isomeric_same_utf8_string_type("utf8�ַ���"), 1) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_first_not_of(isomeric_same_utf8_string_type(""), str.length()) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_first_not_of(isomeric_same_utf8_string_type(""), str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_first_not_of(isomeric_same_utf8_string_type("")) == 0);
	}

	// find_last_of
	{
        yggr_test_assert(str.find_last_of(isomeric_same_utf8_string_type("��abc")) == 4);
		yggr_test_assert(str.find_last_of(isomeric_same_utf8_string_type("abc")) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(isomeric_same_utf8_string_type("��abc"), 1) == yggr::utf8_string::npos);
		yggr_test_assert(str.find_last_of(isomeric_same_utf8_string_type("��abc"),5) == 4);
		yggr_test_assert(str.find_last_of(isomeric_same_utf8_string_type("u"), 0) == 0);
	}

	// find_last_not_of
	{
        yggr_test_assert(str.find_last_not_of(isomeric_same_utf8_string_type("��abc")) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(isomeric_same_utf8_string_type("abc")) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(isomeric_same_utf8_string_type("��abc"), 1) == 1);
		yggr_test_assert(str.find_last_not_of(isomeric_same_utf8_string_type("��abc"),5) == 5);
		yggr_test_assert(str.find_last_not_of(isomeric_same_utf8_string_type("utf8�ַ���"), 1) == yggr::utf8_string::npos);

		yggr_test_assert(str.find_last_not_of(isomeric_same_utf8_string_type(""), 0) == 0);
		yggr_test_assert(str.find_last_not_of(isomeric_same_utf8_string_type(""), str.length() - 1) == str.length() - 1);
	}

	// compare_eq
	{
		//str = "utf8�ַ���";
		yggr_test_assert(str.compare_eq(4, 2, isomeric_same_string_type("�ַ�")) == true);
		yggr_test_assert(str.compare_eq(4, 2, isomeric_same_string_type("�ַ���")) == false);

		yggr_test_assert(str.compare_eq(4, 2, isomeric_same_utf8_string_type("�ַ�")) == true);
		yggr_test_assert(str.compare_eq(4, 2, isomeric_same_utf8_string_type("�ַ���")) == false);
	}

	// compare
	{
		//str = "utf8�ַ���";

		yggr_test_assert(str.compare(4, 2, isomeric_same_string_type("�ַ�")) == 0);
		yggr_test_assert(str.compare(4, 2, isomeric_same_string_type("�ַ���")) < 0);
		yggr_test_assert(str.compare(4, 3, isomeric_same_string_type("�ַ�")) > 0);

		yggr_test_assert(str.compare(4, 2, isomeric_same_utf8_string_type("�ַ�")) == 0);
		yggr_test_assert(str.compare(4, 2, isomeric_same_utf8_string_type("�ַ���")) < 0);
		yggr_test_assert(str.compare(4, 3, isomeric_same_utf8_string_type("�ַ�")) > 0);
	}
}

void non_member_foo_test(void)
{
	typedef yggr::utf8_string utf8_string_type;
	typedef utf8_string_type::base_type string_type;

	typedef 
		yggr::charset::utf8_string_impl
		<
			yggr::utf8_string::value_type,
			yggr::utf8_string::traits_type,
			yggr::utf8_string::allocator_type
		> isomeric_same_utf8_string_type;

	typedef isomeric_same_utf8_string_type::base_type isomeric_same_string_type;
	

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

	// operator +
	{
		yggr::utf8_string str("utf8�ַ���");
		std::cout << "utf8�ַ�" + str + "aaa" << std::endl;
		std::cout << L"utf8�ַ�" + str + L"bbb" << std::endl;

		std::cout << isomeric_same_string_type("utf8�ַ�") + str << std::endl;

		std::cout << "utf8�ַ�" + str + isomeric_same_string_type("aaa") << std::endl;

		std::cout << isomeric_same_utf8_string_type("utf8�ַ�") + str << std::endl;
		const yggr::utf8_string& cref_str = str;
		std::cout << isomeric_same_utf8_string_type("utf8�ַ�") + cref_str << std::endl;


		std::cout << isomeric_same_utf8_string_type("utf8�ַ�") + str + isomeric_same_string_type("aaa") << std::endl;
	}

	// operator +=
	{
		yggr::utf8_string str("utf8�ַ���");
		isomeric_same_string_type rst_str("operaotr+= ");
		rst_str += str;
		std::cout << rst_str << std::endl;
	}

	{
		yggr::utf8_string str("utf8�ַ���");
		isomeric_same_utf8_string_type rst_str("operaotr+= ");
		rst_str += str;
		std::cout << rst_str << std::endl;
	}

	// operator ==
	{
        yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str == "utf8�ַ���");
		yggr_test_assert("utf8�ַ���" == str);

		yggr_test_assert(str == isomeric_same_string_type("utf8�ַ���"));
		yggr_test_assert(isomeric_same_string_type("utf8�ַ���") == str);

		yggr_test_assert(str == isomeric_same_utf8_string_type("utf8�ַ���"));
		yggr_test_assert(isomeric_same_utf8_string_type("utf8�ַ���") == str);
	}

	// operator !=
	{

		yggr::utf8_string str("utf8�ַ���");
		
		yggr_test_assert(str != isomeric_same_string_type("utf8"));
		yggr_test_assert(isomeric_same_string_type("utf8") != str);
		yggr_test_assert(str != isomeric_same_string_type(""));
		yggr_test_assert(isomeric_same_string_type("") != str);

		yggr_test_assert(str != isomeric_same_utf8_string_type("utf8"));
		yggr_test_assert(isomeric_same_utf8_string_type("utf8") != str);
		yggr_test_assert(str != isomeric_same_utf8_string_type(""));
		yggr_test_assert(isomeric_same_utf8_string_type("") != str);

	}

	// operator <
	{
        yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str < isomeric_same_string_type("utf8�ַ�����"));

		yggr_test_assert(!(isomeric_same_string_type("utf8�ַ�����") < str));
		yggr_test_assert(!(str < isomeric_same_string_type("")));

		yggr_test_assert(str < isomeric_same_utf8_string_type("utf8�ַ�����"));
	}

	// operator <=
	{
        yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str <= isomeric_same_string_type("utf8�ַ�����"));
		yggr_test_assert(str <= isomeric_same_string_type("utf8�ַ���"));

		yggr_test_assert(!(isomeric_same_string_type("utf8�ַ�����") <= str));

		yggr_test_assert(str <= isomeric_same_utf8_string_type("utf8�ַ�����"));
	}

	// operator >
	{
        yggr::utf8_string str("utf8�ַ���");
		yggr_test_assert(str > isomeric_same_string_type("utf8�ַ�"));
		yggr_test_assert(str > isomeric_same_string_type("") );

		yggr_test_assert(!(isomeric_same_string_type("utf8�ַ�") > str));

		yggr_test_assert(str > isomeric_same_utf8_string_type("utf8�ַ�"));
	}

	// operator >=
	{
		yggr::utf8_string str("utf8�ַ���");

		yggr_test_assert(str >= isomeric_same_string_type("utf8�ַ�"));
		yggr_test_assert(str >= isomeric_same_string_type("utf8�ַ���"));

		yggr_test_assert(!(isomeric_same_string_type("utf8�ַ�") >= str));

		yggr_test_assert(str >= isomeric_same_utf8_string_type("utf8�ַ�"));
	}

	// swap
	{
		yggr::utf8_string str1_chk("utf8�ַ���1");
		yggr::utf8_string str2_chk("utf8�ַ���2");

		isomeric_same_string_type str1(str1_chk);
		isomeric_same_string_type str2(str2_chk);

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

		isomeric_same_utf8_string_type str1(str1_chk);
		isomeric_same_utf8_string_type str2(str2_chk);

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

int main(int argc, char* argv[])
{

#if defined(TEST_CONSTRUCT)
	test_construct();
#endif // TEST_CONSTRUCT
	
	test_operator_set();
	test_swap();

	test_append();
	test_insert();
	test_replace();
	test_find();
	test_rfind();
	test_find_first_of();
	test_find_first_not_of();
	test_find_last_of();
	test_find_last_not_of();

	test_modify();
	test_string_operations();
	non_member_foo_test();

	wait_any_key(argc, argv);
	return 0;
}
