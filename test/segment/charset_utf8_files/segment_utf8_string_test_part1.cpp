//utf8_string_test_part1.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

//#define YGGR_NO_CWCHAR

//#define YGGR_USE_STL_STRING
//#define _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/charset/utf8_char_view.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/compile_link/linker.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(segment)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)

template<typename Seg>
void delete_segment(const yggr::string& name)
{
	typedef Seg seg_type;
	typedef yggr::segment::segment_destroyer<seg_type> destroyer_type;

	if(name.empty())
	{
		return;
	}

	{ destroyer_type del(name.c_str()); }
}

//#define TEST_IO

#define TEST_CONSTRUCT

#if defined(TEST_CONSTRUCT)

template<typename Utf8String, typename SegCont>
void test_construct(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	{
		utf8_string_type str1(alloc);
		yggr_test_assert(str1.empty());
	}

	// (n, char)
	{
		{
			// char
			utf8_string_type str2(3, 'a', alloc);
			yggr_test_assert(str2 == "aaa");

			utf8_string_type str4(3, 'a', alloc, yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "aaa");

			utf8_string_type str6(3, yggr::charset::make_string_charset_helper('a'), alloc);
			yggr_test_assert(str6 == "aaa");
		}

		{
			// const chat*
			utf8_string_type str2(3, "a", alloc);
			yggr_test_assert(str2 == "aaa");

			utf8_string_type str4(3, "a", alloc, yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "aaa");

			utf8_string_type str6(3, yggr::charset::make_string_charset_helper("a"), alloc);
			yggr_test_assert(str6 == "aaa");
		}

		{
			// utf8_char_view
			utf8_string_type str2(3, yggr::utf8_char_view("a"), alloc);
			yggr_test_assert(str2 == "aaa");

			utf8_string_type str4(3, yggr::utf8_char_view("a"), alloc, yggr::charset::charset_name_t<yggr::utf8_char_view>());
			yggr_test_assert(str4 == "aaa");

			utf8_string_type str6(3, yggr::charset::make_string_charset_helper(yggr::utf8_char_view("a")), alloc);
			yggr_test_assert(str6 == "aaa");
		}

		{
			// string
			{
				//std::string

				utf8_string_type str2(3, std::string("a"), alloc);
				yggr_test_assert(str2 == "aaa");

				utf8_string_type str4(3, std::string("a"), alloc,
										yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "aaa");

				utf8_string_type str6(3, yggr::charset::make_string_charset_helper(std::string("a")), alloc);
				yggr_test_assert(str6 == "aaa");
			}

			{
				//boost::container::string

				utf8_string_type str2(3, boost::container::string("a"), alloc);
				yggr_test_assert(str2 == "aaa");

				utf8_string_type str4(3, boost::container::string("a"), alloc,
										yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "aaa");

				utf8_string_type str6(3, yggr::charset::make_string_charset_helper(boost::container::string("a")), alloc);
				yggr_test_assert(str6 == "aaa");
			}

			{
				//yggr::string

				utf8_string_type str2(3, yggr::string("a"), alloc);
				yggr_test_assert(str2 == "aaa");

				utf8_string_type str4(3, yggr::string("a"), alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "aaa");

				utf8_string_type str6(3, yggr::charset::make_string_charset_helper(yggr::string("a")), alloc);
				yggr_test_assert(str6 == "aaa");
			}

			{
				// utf8_string_type::base_type

				utf8_string_type str2(3, typename utf8_string_type::base_type("a", alloc), alloc);
				yggr_test_assert(str2 == "aaa");

				utf8_string_type str2_2(3, typename utf8_string_type::base_type("a", alloc));
				yggr_test_assert(str2_2 == "aaa");

				utf8_string_type str4(3, typename utf8_string_type::base_type("a", alloc), alloc,
										yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "aaa");

				utf8_string_type str5(3, yggr::charset::make_string_charset_helper(typename utf8_string_type::base_type("a", alloc)) );
				yggr_test_assert(str5 == "aaa");

				utf8_string_type str6(3, yggr::charset::make_string_charset_helper(typename utf8_string_type::base_type("a", alloc)),
										alloc);
				yggr_test_assert(str6 == "aaa");

				utf8_string_type str6_2(3, yggr::charset::make_string_charset_helper(typename utf8_string_type::base_type("a", alloc)));
				yggr_test_assert(str6_2 == "aaa");
			}
		}

		{
			// utf8 char
			utf8_string_type str1(3, typename utf8_string_type::char_type('a', alloc));
			yggr_test_assert(str1 == "aaa");

			utf8_string_type str2(3, typename utf8_string_type::char_type('a', alloc), alloc);
			yggr_test_assert(str2 == "aaa");

			utf8_string_type str3(3, typename utf8_string_type::char_type('a', alloc),
									yggr::charset::charset_name_t<typename utf8_string_type::char_type>());
			yggr_test_assert(str3 == "aaa");

			utf8_string_type str4(3, typename utf8_string_type::char_type('a', alloc),
									alloc, yggr::charset::charset_name_t<typename utf8_string_type::char_type>());
			yggr_test_assert(str4 == "aaa");

			utf8_string_type str5(3, yggr::charset::make_string_charset_helper(typename utf8_string_type::char_type('a', alloc)) );
			yggr_test_assert(str5 == "aaa");

			utf8_string_type str6(3, yggr::charset::make_string_charset_helper(typename utf8_string_type::char_type('a', alloc)),
									alloc);
			yggr_test_assert(str6 == "aaa");
		}

		{
			// utf8 char_ref
			utf8_string_type org_str("abc", alloc);

			utf8_string_type str0(3, org_str[0]);
			yggr_test_assert(str0 == "aaa");

			utf8_string_type str1(3, org_str[0], alloc);
			yggr_test_assert(str1 == "aaa");

			utf8_string_type str3(3, org_str[0], yggr::charset::charset_name_t<typename utf8_string_type::char_type>());
			yggr_test_assert(str3 == "aaa");

			utf8_string_type str4(3, org_str[0], alloc, yggr::charset::charset_name_t<typename utf8_string_type::char_type>());
			yggr_test_assert(str4 == "aaa");

			utf8_string_type str5(3, yggr::charset::make_string_charset_helper(org_str[0]) );
			yggr_test_assert(str5 == "aaa");

			utf8_string_type str6(3, yggr::charset::make_string_charset_helper(org_str[0]), alloc);
			yggr_test_assert(str6 == "aaa");
		}

		{
			// utf8 char_cref
			utf8_string_type org_str("abc", alloc);
			const utf8_string_type& org_str_cref = org_str;

			utf8_string_type str0(3, org_str_cref[0]);
			yggr_test_assert(str0 == "aaa");

			utf8_string_type str1(3, org_str_cref[0], alloc);
			yggr_test_assert(str1 == "aaa");

			utf8_string_type str3(3, org_str_cref[0], yggr::charset::charset_name_t<typename utf8_string_type::char_type>());
			yggr_test_assert(str3 == "aaa");

			utf8_string_type str4(3, org_str_cref[0], alloc, yggr::charset::charset_name_t<typename utf8_string_type::char_type>());
			yggr_test_assert(str4 == "aaa");

			utf8_string_type str5(3, yggr::charset::make_string_charset_helper(org_str_cref[0]) );
			yggr_test_assert(str5 == "aaa");

			utf8_string_type str6(3, yggr::charset::make_string_charset_helper(org_str_cref[0]), alloc);
			yggr_test_assert(str6 == "aaa");
		}
	}

	// (string)
	{
		{
			//const char*

			utf8_string_type str2("abc", alloc);
			yggr_test_assert(str2 == "abc");

			utf8_string_type str5("abc", alloc, yggr::charset::charset_name_t<char>());
			yggr_test_assert(str5 == "abc");

			utf8_string_type str7(yggr::charset::make_string_charset_helper("abc"), alloc);
			yggr_test_assert(str7 == "abc");
		}

		{
			// const char*, n

			utf8_string_type str3("abcde", 3, alloc);
			yggr_test_assert(str3 == "abc");

			utf8_string_type str5("abcde", 3, alloc, yggr::charset::charset_name_t<char>());
			yggr_test_assert(str5 == "abc");

			utf8_string_type str6(yggr::charset::make_string_charset_helper("abcde"), 3, alloc);
			yggr_test_assert(str6 == "abc");
		}

		// string
		{
			{
				//const std::string&
				utf8_string_type str2(std::string("abc"), alloc);
				yggr_test_assert(str2 == "abc");

				utf8_string_type str5(std::string("abc"), alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "abc");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(std::string("abc")), alloc);
				yggr_test_assert(str7 == "abc");
			}

			{
				//const boost::container::string&
				utf8_string_type str2(boost::container::string("abc"), alloc);
				yggr_test_assert(str2 == "abc");

				utf8_string_type str5(boost::container::string("abc"), alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "abc");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(boost::container::string("abc")), alloc);
				yggr_test_assert(str7 == "abc");
			}

			{
				//const yggr::string&
				utf8_string_type str2(yggr::string("abc"), alloc);
				yggr_test_assert(str2 == "abc");

				utf8_string_type str5(yggr::string("abc"), alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "abc");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(yggr::string("abc")), alloc);
				yggr_test_assert(str7 == "abc");
			}

			{
				//utf8_string_type::base_type

				utf8_string_type str1(typename utf8_string_type::base_type("abc", alloc));
				yggr_test_assert(str1 == "abc");

				utf8_string_type str2(typename utf8_string_type::base_type("abc", alloc), alloc);
				yggr_test_assert(str2 == "abc");

				utf8_string_type str3(typename utf8_string_type::base_type("abc", alloc), yggr::charset::charset_name_t<char>());
				yggr_test_assert(str3 == "abc");

				utf8_string_type str5(typename utf8_string_type::base_type("abc", alloc), alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "abc");

				utf8_string_type str6(yggr::charset::make_string_charset_helper(typename utf8_string_type::base_type("abc", alloc)) );
				yggr_test_assert(str6 == "abc");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(typename utf8_string_type::base_type("abc", alloc)), alloc);
				yggr_test_assert(str7 == "abc");
			}

			{
				//utf8_string_type::base_type

				utf8_string_type str2(yggr::utf8_string("abc"), alloc);
				yggr_test_assert(str2 == "abc");

				utf8_string_type str5(yggr::utf8_string("abc"), alloc, yggr::charset::charset_name_t<yggr::utf8_string>());
				yggr_test_assert(str5 == "abc");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(yggr::utf8_string("abc")), alloc);
				yggr_test_assert(str7 == "abc");
			}

			{
				//utf8_string_type

				utf8_string_type str1(utf8_string_type("abc", alloc));
				yggr_test_assert(str1 == "abc");

				utf8_string_type str2(utf8_string_type("abc", alloc), alloc);
				yggr_test_assert(str2 == "abc");

				utf8_string_type str3(utf8_string_type("abc", alloc), yggr::charset::charset_name_t<utf8_string_type>());
				yggr_test_assert(str3 == "abc");

				utf8_string_type str5(utf8_string_type("abc", alloc), alloc, yggr::charset::charset_name_t<utf8_string_type>());
				yggr_test_assert(str5 == "abc");

				utf8_string_type str6(yggr::charset::make_string_charset_helper(utf8_string_type("abc", alloc)) );
				yggr_test_assert(str6 == "abc");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(utf8_string_type("abc", alloc)), alloc);
				yggr_test_assert(str7 == "abc");
			}
		}

		// string npos
		{
			{
				//const std::string&
				utf8_string_type str2(std::string("abcde"), 3, alloc);
				yggr_test_assert(str2 == "de");

				utf8_string_type str5(std::string("abcde"), 3, alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "de");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(std::string("abcde")), 3, alloc);
				yggr_test_assert(str7 == "de");
			}

			{
				//const boost::container::string&
				utf8_string_type str2(boost::container::string("abcde"), 3, alloc);
				yggr_test_assert(str2 == "de");

				utf8_string_type str5(boost::container::string("abcde"), 3, alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "de");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(boost::container::string("abcde")), 3, alloc);
				yggr_test_assert(str7 == "de");
			}

			{
				//const yggr::string&
				utf8_string_type str2(yggr::string("abcde"), 3, alloc);
				yggr_test_assert(str2 == "de");

				utf8_string_type str5(yggr::string("abcde"), 3, alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "de");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(yggr::string("abcde")), 3, alloc);
				yggr_test_assert(str7 == "de");
			}

			{
				//utf8_string_type::base_type

				utf8_string_type str1(typename utf8_string_type::base_type("abcde", alloc), 3);
				yggr_test_assert(str1 == "de");

				utf8_string_type str2(typename utf8_string_type::base_type("abcde", alloc), 3, alloc);
				yggr_test_assert(str2 == "de");

				utf8_string_type str3(typename utf8_string_type::base_type("abcde", alloc), 3, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str3 == "de");

				utf8_string_type str5(typename utf8_string_type::base_type("abcde", alloc), 3, alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "de");

				utf8_string_type str6(yggr::charset::make_string_charset_helper(typename utf8_string_type::base_type("abcde", alloc)), 3);
				yggr_test_assert(str6 == "de");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(typename utf8_string_type::base_type("abcde", alloc)), 3, alloc);
				yggr_test_assert(str7 == "de");
			}

			{
				//utf8_string_type::base_type

				utf8_string_type str2(yggr::utf8_string("abcde"), 3, alloc);
				yggr_test_assert(str2 == "de");

				utf8_string_type str5(yggr::utf8_string("abcde"), 3, alloc, yggr::charset::charset_name_t<yggr::utf8_string>());
				yggr_test_assert(str5 == "de");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(yggr::utf8_string("abcde")), 3, alloc);
				yggr_test_assert(str7 == "de");
			}

			{
				//utf8_string_type

				utf8_string_type str1(utf8_string_type("abcde", alloc), 3);
				yggr_test_assert(str1 == "de");

				utf8_string_type str2(utf8_string_type("abcde", alloc), 3, alloc);
				yggr_test_assert(str2 == "de");

				utf8_string_type str3(utf8_string_type("abcde", alloc), 3, yggr::charset::charset_name_t<utf8_string_type>());
				yggr_test_assert(str3 == "de");

				utf8_string_type str5(utf8_string_type("abcde", alloc), 3, alloc, yggr::charset::charset_name_t<utf8_string_type>());
				yggr_test_assert(str5 == "de");

				utf8_string_type str6(yggr::charset::make_string_charset_helper(utf8_string_type("abcde", alloc)), 3 );
				yggr_test_assert(str6 == "de");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(utf8_string_type("abcde", alloc)), 3, alloc);
				yggr_test_assert(str7 == "de");
			}
		}

		// string subpos sublen
		{
			{
				//const std::string&
				utf8_string_type str2(std::string("abcde"), 3, 1, alloc);
				yggr_test_assert(str2 == "d");

				utf8_string_type str5(std::string("abcde"), 3, 1, alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "d");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(std::string("abcde")), 3, 1, alloc);
				yggr_test_assert(str7 == "d");
			}

			{
				//const boost::container::string&
				utf8_string_type str2(boost::container::string("abcde"), 3, 1, alloc);
				yggr_test_assert(str2 == "d");

				utf8_string_type str5(boost::container::string("abcde"), 3, 1, alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "d");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(boost::container::string("abcde")), 3, 1, alloc);
				yggr_test_assert(str7 == "d");
			}

			{
				//const yggr::string&
				utf8_string_type str2(yggr::string("abcde"), 3, 1, alloc);
				yggr_test_assert(str2 == "d");

				utf8_string_type str5(yggr::string("abcde"), 3, 1, alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "d");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(yggr::string("abcde")), 3, 1, alloc);
				yggr_test_assert(str7 == "d");
			}

			{
				//utf8_string_type::base_type

				utf8_string_type str1(typename utf8_string_type::base_type("abcde", alloc), 3, 1);
				yggr_test_assert(str1 == "d");

				utf8_string_type str2(typename utf8_string_type::base_type("abcde", alloc), 3, 1, alloc);
				yggr_test_assert(str2 == "d");

				utf8_string_type str3(typename utf8_string_type::base_type("abcde", alloc), 3, 1, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str3 == "d");

				utf8_string_type str5(typename utf8_string_type::base_type("abcde", alloc), 3, 1, alloc, yggr::charset::charset_name_t<char>());
				yggr_test_assert(str5 == "d");

				utf8_string_type str6(yggr::charset::make_string_charset_helper(typename utf8_string_type::base_type("abcde", alloc)), 3, 1);
				yggr_test_assert(str6 == "d");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(typename utf8_string_type::base_type("abcde", alloc)), 3, 1, alloc);
				yggr_test_assert(str7 == "d");
			}

			{
				//utf8_string_type::base_type

				utf8_string_type str2(yggr::utf8_string("abcde"), 3, 1, alloc);
				yggr_test_assert(str2 == "d");

				utf8_string_type str5(yggr::utf8_string("abcde"), 3, 1, alloc, yggr::charset::charset_name_t<yggr::utf8_string>());
				yggr_test_assert(str5 == "d");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(yggr::utf8_string("abcde")), 3, 1, alloc);
				yggr_test_assert(str7 == "d");
			}

			{
				//utf8_string_type

				utf8_string_type str1(utf8_string_type("abcde", alloc), 3, 1);
				yggr_test_assert(str1 == "d");

				utf8_string_type str2(utf8_string_type("abcde", alloc), 3, 1, alloc);
				yggr_test_assert(str2 == "d");

				utf8_string_type str3(utf8_string_type("abcde", alloc), 3, 1, yggr::charset::charset_name_t<utf8_string_type>());
				yggr_test_assert(str3 == "d");

				utf8_string_type str5(utf8_string_type("abcde", alloc), 3, 1, alloc, yggr::charset::charset_name_t<utf8_string_type>());
				yggr_test_assert(str5 == "d");

				utf8_string_type str6(yggr::charset::make_string_charset_helper(utf8_string_type("abcde", alloc)), 3, 1);
				yggr_test_assert(str6 == "d");

				utf8_string_type str7(yggr::charset::make_string_charset_helper(utf8_string_type("abcde", alloc)), 3, 1, alloc);
				yggr_test_assert(str7 == "d");
			}
		}
	}

	// iterator
	{
		{
			// const char*
			char str_arr[] = {'a', 'b', 'c'};

			utf8_string_type str2(str_arr, str_arr + sizeof(str_arr), alloc);
			yggr_test_assert(str2 == "abc");

			utf8_string_type str4(str_arr, str_arr + sizeof(str_arr), alloc,
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "abc");
		}

		{
			{
				// string
				std::string str_arr = "abc";

				utf8_string_type str2(str_arr.begin(), str_arr.end(), alloc);
				yggr_test_assert(str2 == "abc");

				utf8_string_type str4(str_arr.begin(), str_arr.end(), alloc,
										yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "abc");
			}

			{
				// string
				boost::container::string str_arr = "abc";

				utf8_string_type str2(str_arr.begin(), str_arr.end(), alloc);
				yggr_test_assert(str2 == "abc");

				utf8_string_type str4(str_arr.begin(), str_arr.end(), alloc,
										yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "abc");
			}

			{
				// string
				yggr::string str_arr = "abc";

				utf8_string_type str2(str_arr.begin(), str_arr.end(), alloc);
				yggr_test_assert(str2 == "abc");

				utf8_string_type str4(str_arr.begin(), str_arr.end(), alloc,
										yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "abc");
			}

			{
				// utf8_char
				yggr::utf8_char str_arr = "a";

				utf8_string_type str2(str_arr.begin(), str_arr.end(), alloc);
				yggr_test_assert(str2 == "a");

				utf8_string_type str4(str_arr.begin(), str_arr.end(), alloc,
										yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "a");
			}

			{
				// utf8_string
				yggr::utf8_string str_arr = "abc";

				utf8_string_type str2(str_arr.begin(), str_arr.end(), alloc);
				yggr_test_assert(str2 == "abc");

				utf8_string_type str4(str_arr.begin(), str_arr.end(), alloc,
										yggr::charset::charset_name_t<yggr::utf8_string>());
				yggr_test_assert(str4 == "abc");
			}
		}

		{
			{
				// string
				std::string str_arr = "abc";

				utf8_string_type str2(str_arr.rbegin(), str_arr.rend(), alloc);
				yggr_test_assert(str2 == "cba");

				utf8_string_type str4(str_arr.rbegin(), str_arr.rend(), alloc,
										yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "cba");
			}

			{
				// string
				boost::container::string str_arr = "abc";

				utf8_string_type str2(str_arr.rbegin(), str_arr.rend(), alloc);
				yggr_test_assert(str2 == "cba");

				utf8_string_type str4(str_arr.rbegin(), str_arr.rend(), alloc,
										yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "cba");
			}

			{
				// string
				yggr::string str_arr = "abc";

				utf8_string_type str2(str_arr.rbegin(), str_arr.rend(), alloc);
				yggr_test_assert(str2 == "cba");

				utf8_string_type str4(str_arr.rbegin(), str_arr.rend(), alloc,
										yggr::charset::charset_name_t<char>());
				yggr_test_assert(str4 == "cba");
			}

			{
				// utf8_string
				yggr::utf8_string str_arr = "abc";

				utf8_string_type str2(str_arr.rbegin(), str_arr.rend(), alloc);
				yggr_test_assert(str2 == "cba");

				utf8_string_type str4(str_arr.rbegin(), str_arr.rend(), alloc,
										yggr::charset::charset_name_t<yggr::utf8_string>());
				yggr_test_assert(str4 == "cba");
			}
		}

		{
			// vector
			std::string tmp_str_arr = "abc";
			std::vector<char> str_arr(tmp_str_arr.begin(), tmp_str_arr.end());

			utf8_string_type str2(str_arr.rbegin(), str_arr.rend(), alloc);
			yggr_test_assert(str2 == "cba");

			utf8_string_type str4(str_arr.rbegin(), str_arr.rend(), alloc,
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "cba");
		}

		{
			// list
			std::string tmp_str_arr = "abc";
			std::list<char> str_arr(tmp_str_arr.begin(), tmp_str_arr.end());

			utf8_string_type str2(str_arr.rbegin(), str_arr.rend(), alloc);
			yggr_test_assert(str2 == "cba");

			utf8_string_type str4(str_arr.rbegin(), str_arr.rend(), alloc,
									yggr::charset::charset_name_t<char>());
			yggr_test_assert(str4 == "cba");
		}
	}

	std::cout << "test_construct successed" << std::endl;

}

#endif // TEST_CONSTRUCT

template<typename Utf8String, typename SegCont>
void test_operator_set(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// char
	{
		// = const char*
		{
			utf8_string_type str(alloc);
			str = "abc";
			yggr_test_assert(str == "abc");
		}

		// = BasicString
		// srd::string
		{
			utf8_string_type str(alloc);
			str = std::string("abc");
			yggr_test_assert(str == "abc");
		}

		// = boost::container::string
		{
			utf8_string_type str(alloc);
			str = boost::container::string("abc");
			yggr_test_assert(str == "abc");
		}

		// = yggr::string
		{
			utf8_string_type str(alloc);
			str = yggr::string("abc");
			yggr_test_assert(str == "abc");
		}
	}

	// = utf8_string_type
	{
		yggr::utf8_string src_str("abc");
		utf8_string_type str(alloc);
		str = boost::move(src_str);
		yggr_test_assert(str == "abc");
	}

	{
		utf8_string_type str(alloc);
		str = yggr::utf8_string("abc");
		yggr_test_assert(str == "abc");
	}

	{
		utf8_string_type str(alloc);
		str = yggr::charset::make_string_charset_helper(yggr::utf8_string("abc"));
		yggr_test_assert(str == "abc");
	}

	std::cout << "test_operator_set successed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_swap(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// char
	{
		// BasicString
		// std::string
		{
			std::string str1("abc");
			utf8_string_type str("def", alloc);

			str.swap(boost::move(str1));

			yggr_test_assert(str == "abc");
			yggr_test_assert(str1 == "def");

			std::swap(str, boost::move(str1));

			yggr_test_assert(str == "def");
			yggr_test_assert(str1 == "abc");
		}

		{
			std::string str1("abc");
			utf8_string_type str("def", alloc);

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
			utf8_string_type str("def", alloc);

			str.swap(boost::move(str1));

			yggr_test_assert(str == "abc");
			yggr_test_assert(str1 == "def");

			boost::swap(str, boost::move(str1));

			yggr_test_assert(str == "def");
			yggr_test_assert(str1 == "abc");
		}

		{
			boost::container::string str1("abc");
			utf8_string_type str("def", alloc);

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
			utf8_string_type str("def", alloc);

			str.swap(boost::move(str1));

			yggr_test_assert(str == "abc");
			yggr_test_assert(str1 == "def");

			yggr::swap(str, boost::move(str1));

			yggr_test_assert(str == "def");
			yggr_test_assert(str1 == "abc");
		}

		{
			yggr::string str1("abc");
			utf8_string_type str("def", alloc);

			str.swap(str1);

			yggr_test_assert(str == "abc");
			yggr_test_assert(str1 == "def");

			yggr::swap(str, str1);

			yggr_test_assert(str == "def");
			yggr_test_assert(str1 == "abc");
		}
	}

	{
		utf8_string_type str1("abc", alloc);
		utf8_string_type str("def", alloc);

		str.swap(boost::move(str1));

		yggr_test_assert(str == "abc");
		yggr_test_assert(str1 == "def");
	}

	{
		utf8_string_type str1("abc", alloc);
		utf8_string_type str("def", alloc);

		str.swap(str1);

		yggr_test_assert(str == "abc");
		yggr_test_assert(str1 == "def");
	}

	{
		utf8_string_type str1("abc", alloc);
		utf8_string_type str("def", alloc);

		//str.swap(yggr::charset::make_string_charset_helper(str1));
		typedef yggr::charset::string_charset_helper<utf8_string_type&> now_charset_helper_type;
		now_charset_helper_type str1_helper = yggr::charset::make_string_charset_helper(str1);
		str.swap(str1_helper);

		yggr_test_assert(str == "abc");
		yggr_test_assert(str1 == "def");
	}

	std::cout << "test_swap successed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_append(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// n, char
	{
		// n char
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(3, 'a');

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好caaa", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		// n const char*
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

#		if 0
			//assert false "aa" is a string not a char
			str.append(3, "aa");
#		else
			str.append(3, "a");
#		endif // 0, 1
			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好caaa", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		// n, string
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(3, std::string("a"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好caaa", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::string("a"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好caaa", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		// n, utf8_string
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::utf8_char("a"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好caaa", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		// string_charset_name
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper('a'));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好caaa", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper("a"));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好caaa", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper(std::string("a")));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好caaa", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(3, yggr::charset::make_string_charset_helper(typename utf8_string_type::char_type("a", alloc)));

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好caaa", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}
	}

	// string
	{
		// const char*
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append("world");

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == yggr::utf8_string("a你b好cworld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		//  const char*, n
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append("world", 3);

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好cwor", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		// string
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(std::string("world"));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == yggr::utf8_string("a你b好cworld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(std::string("world"), YGGR_STR_UTF8_STRING_CHARSET_NAME());

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == yggr::utf8_string("a你b好cworld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		// string, pos
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(yggr::string("world"), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == yggr::utf8_string("a你b好cld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		// const char*, pos, len
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append("world", 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == yggr::utf8_string("a你b好cl", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		// string, pos, len
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(std::string("world"), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == yggr::utf8_string("a你b好cl", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		// utf8_string
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append( utf8_string_type("world", alloc));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == yggr::utf8_string("a你b好cworld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append( utf8_string_type("world", alloc), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == yggr::utf8_string("a你b好cld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append( utf8_string_type("world", alloc), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == yggr::utf8_string("a你b好cl", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		// string_charset_helper
		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper("world"));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == yggr::utf8_string("a你b好cworld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper("world"), 3);

			yggr_test_assert(str.length() == 8);
			yggr_test_assert(str == yggr::utf8_string("a你b好cwor", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(std::string("world")));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == yggr::utf8_string("a你b好cworld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(yggr::string("world")), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == yggr::utf8_string("a你b好cld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(std::string("world")), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == yggr::utf8_string("a你b好cl", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(utf8_string_type("world", alloc)));

			yggr_test_assert(str.length() == 10);
			yggr_test_assert(str == yggr::utf8_string("a你b好cworld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(utf8_string_type("world", alloc)), 3);

			yggr_test_assert(str.length() == 7);
			yggr_test_assert(str == yggr::utf8_string("a你b好cld", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}

		{
			utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);

			str.append(yggr::charset::make_string_charset_helper(utf8_string_type("world", alloc)), 3, 1);

			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == yggr::utf8_string("a你b好cl", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		}
	}

	// append(iters, itere)
	{
		{
			std::string ins_str = " world";
			utf8_string_type str("hello", alloc);

			str.append(ins_str.begin(), ins_str.end());
			yggr_test_assert(str.length() == 11);
			yggr_test_assert(str == "hello world");
		}

		{
			yggr::string ins_str = " world";
			utf8_string_type str("hello", alloc);

			str.append(ins_str.rbegin(), ins_str.rend());
			yggr_test_assert(str.length() == 11);
			yggr_test_assert(str == "hellodlrow ");
		}

		{
			utf8_string_type ins_str(" world", alloc);
			utf8_string_type str("hello", alloc);

			str.append(ins_str.rbegin(), ins_str.rend());
			yggr_test_assert(str.length() == 11);
			yggr_test_assert(str == "hellodlrow ");
		}
	}

	std::cout << "test_append sucessed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_push_back(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	{
		utf8_string_type str("abc", alloc);
		str.push_back('d');
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		utf8_string_type str("abc", alloc);
		str.push_back("d");
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		utf8_string_type str("abc", alloc);
		str.push_back(std::string("d"));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		utf8_string_type str("abc", alloc);
		str.push_back(yggr::utf8_char("d"));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		utf8_string_type str("abc", alloc);
		str.push_back(yggr::charset::make_string_charset_helper('d'));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		utf8_string_type str("abc", alloc);
		str.push_back(yggr::charset::make_string_charset_helper("d"));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		utf8_string_type str("abc", alloc);
		str.push_back(yggr::charset::make_string_charset_helper(std::string("d")));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	{
		utf8_string_type str("abc", alloc);
		str.push_back(yggr::charset::make_string_charset_helper(yggr::utf8_char("d")));
		yggr_test_assert(str.length() == 4);
		yggr_test_assert(str == "abcd");
	}

	std::cout << "test_push_back" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_pop_back(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	{
		utf8_string_type str("abc", alloc);
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

	std::cout << "test_pop_back sucessed" << std::endl;

}

template<typename Utf8String, typename SegCont>
void test_insert(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// insert(pos, n, char)
	{
		{
			utf8_string_type str("abc", alloc);
			str.insert(0, 2, 'q');
			yggr_test_assert(str == "qqabc");
			str.insert(1, 2, 'k');
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper('t'));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type str("abc", alloc);
			str.insert(0, 2, "q");
			yggr_test_assert(str == "qqabc");
			str.insert(1, 2, "k");
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper("t"));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type str("abc", alloc);
			str.insert(0, 2, std::string("q"));
			yggr_test_assert(str == "qqabc");
			str.insert(1, 2, yggr::string("k"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), 3, yggr::charset::make_string_charset_helper(std::string("t")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type str("abc", alloc);
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
			utf8_string_type str("abc", alloc);
			str.insert(str.begin(), 2, 'q');
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1, 2, 'k');
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper('t'));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type str("abc", alloc);
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
			utf8_string_type str("abc", alloc);
			str.insert(str.begin(), 2, std::string("q"));
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1, 2, yggr::string("k"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), 3, yggr::charset::make_string_charset_helper(std::string("t")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type str("abc", alloc);
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
			utf8_string_type str("abc", alloc);
			str.insert(0, "qq");
			yggr_test_assert(str == "qqabc");
			str.insert(1,  "kk");
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper("ttt"));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type str("abc", alloc);
			str.insert(0, std::string("qq"));
			yggr_test_assert(str == "qqabc");
			str.insert(1, yggr::string("kk"));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(std::string("ttt")));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type str("abc", alloc);
			str.insert(0, std::string("qq"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str == "qqabc");
			str.insert(1, yggr::string("kk"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(std::string("ttt"), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type str("abc", alloc);
#		if 0
			str.insert(0, yggr::utf8_char("q")); // mpl_assert(false)
#		else
			str.insert(0, utf8_string_type("qq", alloc));
#		endif // 0, 1
			yggr_test_assert(str == "qqabc");
			str.insert(1, utf8_string_type("kk", alloc));
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(utf8_string_type("ttt", alloc)));
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}
	}

	// insert(pos, const char*, n)
	{

		{
			utf8_string_type str("abc", alloc);
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
			utf8_string_type str("abc", alloc);
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
			utf8_string_type str("abc", alloc);
			str.insert(0, stl_ins_str, 0, 2);
			yggr_test_assert(str == "qqabc");
			str.insert(1,  yggr_ins_str, 2, 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.length(), yggr::charset::make_string_charset_helper(stl_ins_str), 4);
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type ins_str("qqkkttt", alloc);
			utf8_string_type str("abc", alloc);
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
			utf8_string_type str("abc", alloc);
			str.insert(str.begin(), ins_str, ins_str + 2);
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1,  ins_str + 2, ins_str + 2 + 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), ins_str + 2 + 2, ins_str + 2 + 2 + 3);
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}

		{
			utf8_string_type ins_str("qqkkttt", alloc);
			utf8_string_type str("abc", alloc);
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
			utf8_string_type str("abc", alloc);

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
			utf8_string_type ins_str("tttkkqq", alloc);
			utf8_string_type str("abc", alloc);
			str.insert(str.begin(), ins_str.rbegin(), ins_str.rbegin() + 2);
			yggr_test_assert(str == "qqabc");
			str.insert(str.begin() + 1,  ins_str.rbegin() + 2, ins_str.rbegin() + 2 + 2);
			yggr_test_assert(str == "qkkqabc");
			str.insert(str.end(), ins_str.rbegin() + 2 + 2, ins_str.rend());
			yggr_test_assert(str == "qkkqabcttt");
			yggr_test_assert(str.length() == 10);
		}
	}

	std::cout << "test_insert sucessed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_replace(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// replace(pos, len, n, char)
	{
		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, 3, 'd');
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abcdef", alloc);
			str.replace(1, 3, 3, 'k');
			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "akkkef");
		}

		{
			utf8_string_type str("abcdef", alloc);
			str.replace(1, 3, 0, 'k');
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "aef");
		}

		{
			utf8_string_type str("abcdef", alloc);
			str.replace(1, 3, 2, 'k');
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "akkef");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, 3, "d");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, 3, std::string("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, 3, yggr::utf8_char("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper('d'));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper(yggr::string("d")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, 3, yggr::charset::make_string_charset_helper(yggr::utf8_char("d")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 0, 0, 'd');
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "abc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(0, 100, 0, 'd');
			yggr_test_assert(str.length() == 0);
			yggr_test_assert(str == "");
		}
	}

	// replace(iters, itere, n, char)
	{
		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, 3, 'd');
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, 3, "d");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, 3, std::string("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::utf8_char("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper('d'));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper("d"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper(yggr::string("d")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, 3, yggr::charset::make_string_charset_helper(yggr::utf8_char("d")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adddc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 1, 0, 'd');
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "abc");
		}

#if 0
		// this code is not accept,
		// because (stl and boost)'s result not same,
		// and !(i1 < i2) is illegal of using
		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin(), 0, 'd');
			yggr_test_assert(str.length() == 1);
			yggr_test_assert(str == "a");
		}
#endif // 0, 1

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin(), str.end(), 0, 'd');
			yggr_test_assert(str.length() == 0);
			yggr_test_assert(str == "");
		}
	}

	// replace(pos, len, const char*)
	{
		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, "def");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, std::string("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, std::string("def"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, utf8_string_type("def", alloc));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, yggr::charset::make_string_charset_helper("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, yggr::charset::make_string_charset_helper(yggr::string("def")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, yggr::charset::make_string_charset_helper(utf8_string_type("def", alloc)));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 0, "def");
			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "adefbc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(0, 100, "def");
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "def");
		}
	}

	// replace(pos, len, const char*, n)
	{

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, "def", 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, yggr::charset::make_string_charset_helper("def"), 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 0, "def", 3);
			yggr_test_assert(str.length() == 6);
			yggr_test_assert(str == "adefbc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(0, 100, "def", 3);
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "def");
		}
	}

	// replace(iters, itere, const char*)
	{
		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, "def");
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, std::string("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, utf8_string_type("def", alloc));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper("def"));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper(yggr::string("def")));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper(utf8_string_type("def", alloc)));
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

#if 0  //illegal of using
		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin(), "def");
			yggr_test_assert(str.length() == 4);
			yggr_test_assert(str == "adef");
		}
#endif // 0, 1

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin(), str.end(), "def");
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "def");
		}
	}

	// replace(iters, itere, const char*, n)
	{
		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, "def", 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, yggr::charset::make_string_charset_helper("def"), 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

#if 0 //illegal of using
		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin(), "def", 3);
			yggr_test_assert(str.length() == 4);
			yggr_test_assert(str == "adef");
		}
#endif // 0, 1

		{
			utf8_string_type str("abc", alloc);
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
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, "tagdeftag", 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, stl_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, yggr_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, utf8_rep_str, 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, yggr::charset::make_string_charset_helper(stl_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, yggr::charset::make_string_charset_helper(yggr_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, yggr::charset::make_string_charset_helper(utf8_rep_str), 3, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "adefc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(1, 1, yggr::charset::make_string_charset_helper(str), 0, 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}
	}

	//replace(i1, i2, s, e)
	{
		{
			char rep_str[] = "abc";
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, rep_str, rep_str + 3);
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}

		{
			std::string rep_str = "abc";
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.begin(), rep_str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}

		{
			yggr::string rep_str = "abc";
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.rbegin(), rep_str.rend());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "acbac");
		}

		{
			utf8_string_type rep_str("abc", alloc);
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, rep_str.begin(), rep_str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin() + 1, str.begin() + 2, str.begin(), str.end());
			yggr_test_assert(str.length() == 5);
			yggr_test_assert(str == "aabcc");
		}

		{
			utf8_string_type str("abc", alloc);
			str.replace(str.begin(), str.end(), str.rbegin(), str.rend());
			yggr_test_assert(str.length() == 3);
			yggr_test_assert(str == "cba");
		}
	}

	std::cout << "test_replace sucessed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_find(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// find(char, pos)
	{
		{
			utf8_string_type str("", alloc);
			yggr_test_assert(str.find('b') == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find("") == 0);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find('b') == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(yggr::utf8_char('b')) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper('b')) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(str[1]) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(*(str.cbegin() + 1)) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(str[1])) == 1);
		}
	}

	// find(const char*, pos)
	{
		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find("b") == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find("b", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper("b")) == 1);
		}
	}

	// find(const char*, pos, n)
	{
		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find("b", 0, 1) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find("b", 0, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}
	}

	// find(const string&, pos)
	{
		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(std::string("b")) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(std::string("b"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(utf8_string_type("b", alloc)) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(std::string("b"))) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.find(yggr::charset::make_string_charset_helper(utf8_string_type("b", alloc))) == 1);
		}
	}

	std::cout << "test_find successed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_rfind(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// rfind(char, pos)
	{
		{
			utf8_string_type str("", alloc);
			yggr_test_assert(str.rfind('b') == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind("") == str.length());
		}

		{
			utf8_string_type str ("abc", alloc);
			yggr_test_assert(str.rfind('b') == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind(yggr::utf8_char('b')) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper('b')) == 1);
		}

		{
			utf8_string_type str("abcba", alloc);
			yggr_test_assert(str.rfind(str[1]) == 3);
		}

		{
			utf8_string_type str("abcba", alloc);
			yggr_test_assert(str.rfind(*(str.cbegin() + 1)) == 3);
		}

		{
			utf8_string_type str("abcba", alloc);
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(str[1])) == 3);
		}
	}

	// rfind(const char*, pos)
	{
		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind("b") == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind("b", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper("b")) == 1);
		}
	}

	// rfind(const char*, pos, n)
	{
		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind("b", utf8_string_type::npos, 1) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind("b", utf8_string_type::npos, 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}
	}

	// rfind(const string&, pos)
	{
		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind(std::string("b")) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind(std::string("b"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind(utf8_string_type("b", alloc)) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(std::string("b"))) == 1);
		}

		{
			utf8_string_type str("abc", alloc);
			yggr_test_assert(str.rfind(yggr::charset::make_string_charset_helper(utf8_string_type("b", alloc))) == 1);
		}
	}

	std::cout << "test_rfind successed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_find_first_of(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// find_first_of(char, pos)

	{
		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of('c') == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(typename utf8_string_type::char_type('c', alloc)) == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper('c')) == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(str[2]) == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(*(str.begin() + 2)) == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(typename utf8_string_type::char_type(str[2], alloc)) == 2);
		}
	}

	// find_first_of(const char*, pos)
	{
		{
			utf8_string_type str("", alloc);
			yggr_test_assert(str.find_first_of("edc") == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of("") == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of("edc") == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper("edc")) == 2);
		}
	}

	// find_first_of(const char*, pos, n)
	{
		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of("edc", 1, 1) == 4);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper("edc"), 1, 1) == 4);
		}
	}

	// find_first_of(const string&, pos)
	{
		{
			utf8_string_type str("", alloc);
			yggr_test_assert(str.find_first_of(std::string("edc")) == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(boost::container::string("")) == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(std::string("edc")) == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(boost::container::string("edc")) == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(yggr::utf8_string("edc")) == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(std::string("edc"))) == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(boost::container::string("edc"))) == 2);
		}

		{
			utf8_string_type str("abcdefg", alloc);
			yggr_test_assert(str.find_first_of(yggr::charset::make_string_charset_helper(utf8_string_type("edc", alloc))) == 2);
		}
	}

	std::cout << "test_find_first_of successed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_find_first_not_of(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// find_first_not_of(char, pos)

	{
		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of('c') == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(typename utf8_string_type::char_type('c', alloc)) == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper('c')) == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(str[0]) == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(*(str.begin())) == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(str[0]) == 2);
		}
	}

	// find_first_not_of(const char*, pos)
	{
		{
			utf8_string_type str("", alloc);
			yggr_test_assert(str.find_first_not_of("edc") == utf8_string_type::npos);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of("") == 0);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of("edc") == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper("edc")) == 2);
		}
	}

	// find_first_not_of(const char*, pos, n)
	{
		{
			utf8_string_type str("eeeeabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of("edc", 1, 1) == 4);
		}

		{
			utf8_string_type str("eeeeabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper("edc"), 1, 1) == 4);
		}
	}

	// find_first_not_of(const string&, pos)
	{
		{
			utf8_string_type str("", alloc);
			yggr_test_assert(str.find_first_not_of(std::string("edc")) == utf8_string_type::npos);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(boost::container::string(""), 3) == 3);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(std::string("edc")) == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(boost::container::string("edc")) == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(utf8_string_type("edc", alloc)) == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(std::string("edc"))) == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(boost::container::string("edc"))) == 2);
		}

		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_first_not_of(yggr::charset::make_string_charset_helper(utf8_string_type("edc", alloc))) == 2);
		}
	}

	std::cout << "test_find_first_not_of successed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_find_last_of(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// find_last_of(char, pos)

	{
		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of('c') == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(typename utf8_string_type::char_type('c', alloc)) == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper('c')) == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(str[2]) == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(*(str.begin() + 2)) == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(typename utf8_string_type::char_type(str[2])) == 4);
		}
	}

	// find_last_of(const char*, pos)
	{
		{
			utf8_string_type str("", alloc);
			yggr_test_assert(str.find_last_of("edc") == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of("") == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of("edc") == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper("edc")) == 4);
		}
	}

	// find_last_of(const char*, pos, n)
	{
		{
			utf8_string_type str("ebcdcba", alloc);
			yggr_test_assert(str.find_last_of("edc", 1, 1) == 0);
		}

		{
			utf8_string_type str("abcdcbae", alloc);
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper("edc"), 1, 1) == utf8_string_type::npos);
		}
	}

	// find_last_of(const string&, pos)
	{
		{
			utf8_string_type str("", alloc);
			yggr_test_assert(str.find_last_of(std::string("edc")) == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(boost::container::string("")) == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(std::string("edc")) == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(boost::container::string("edc")) == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(utf8_string_type("edc", alloc)) == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(std::string("edc"))) == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(boost::container::string("edc"))) == 4);
		}

		{
			utf8_string_type str("abcdcba", alloc);
			yggr_test_assert(str.find_last_of(yggr::charset::make_string_charset_helper(utf8_string_type("edc", alloc))) == 4);
		}
	}

	std::cout << "test_find_last_of successed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_find_last_not_of(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// find_last_not_of(char, pos)

	{
		{
			utf8_string_type str("ccabcdefg", alloc);
			yggr_test_assert(str.find_last_not_of('c') == str.length() - 1);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(typename utf8_string_type::char_type('c', alloc)) == 6);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper('c')) == 6);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(str[2]) == 6);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(*(str.begin() + 2)) == 6);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(str[2]) == 6);
		}
	}

	// find_last_not_of(const char*, pos)
	{
		{
			utf8_string_type str("", alloc);
			yggr_test_assert(str.find_last_not_of("edc") == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of("") == str.length() - 1);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of("edc") == 6);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper("edc")) == 6);
		}
	}

	// find_last_not_of(const char*, pos, n)
	{
		{
			utf8_string_type str("eeeeabcdefg", alloc);
			yggr_test_assert(str.find_last_not_of("edc", 1, 1) == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdefgeeee", alloc);
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper("edc"), -1, 1) == 6);
		}
	}

	// find_last_not_of(const string&, pos)
	{
		{
			utf8_string_type str("", alloc);
			yggr_test_assert(str.find_last_not_of(std::string("edc")) == utf8_string_type::npos);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(boost::container::string(""), 3) == 3);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(std::string("edc")) == 6);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(boost::container::string("edc")) == 6);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(utf8_string_type("edc", alloc)) == 6);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(std::string("edc"))) == 6);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(boost::container::string("edc"))) == 6);
		}

		{
			utf8_string_type str("abcdefgcc", alloc);
			yggr_test_assert(str.find_last_not_of(yggr::charset::make_string_charset_helper(utf8_string_type("edc", alloc))) == 6);
		}
	}

	std::cout << "test_find_last_not_of successed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_resize_relength(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// the second parameter of the resize function can only use ASCII characters,
	// if the multibyte UTF8 character is truncated when resize,
	// is used to replace the remaining part with the incoming ASCII character

	// resize
	{
		// >
		utf8_string_type str("abc", alloc);
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

	// relength
	{
		// >
		utf8_string_type str("abc", alloc);
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

	std::cout << "test_resize_relength succeed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_copy(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	{
		utf8_string_type str("abc", alloc);
		char out_str[1024] = {0};
		std::size_t size = str.copy(out_str, 2);
		yggr_test_assert(size == 2);
		out_str[size] = 0;
		yggr_test_assert(0 == memcmp(out_str, "ab", size));
	}

	std::cout << "test_copy succeed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_iterator(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	yggr::string stlstr("abc");
	utf8_string_type str("a你b好c", alloc);

	std::cout << str.begin() - str.end() << std::endl;
	std::cout << str.end() - str.begin() << std::endl;

	typename utf8_string_type::iterator iter = str.begin();

	{
		int n = 0;
		for(typename utf8_string_type::iterator i = str.begin(), isize = str.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
			++n;
		}

		std::cout << "n = " << n << std::endl;

		n = 0;
		for(typename utf8_string_type::reverse_iterator i = str.rbegin(), isize = str.rend(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
			//char c = *i;
			++n;
		}

		std::cout << "n = " << n << std::endl;
	}

	{
		int n = 0;
		for(typename utf8_string_type::const_iterator i = str.begin(), isize = str.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
			++n;
		}

		std::cout << "n = " << n << std::endl;

		n = 0;
		for(typename utf8_string_type::const_reverse_iterator i = str.rbegin(), isize = str.rend(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
			++n;
		}

		std::cout << "n = " << n << std::endl;
	}

	utf8_string_type str2(str.rbegin(), str.rend(), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	std::cout << str2 << std::endl;
	std::cout << str.rbegin() - str.rend() << std::endl;

	// iterator
	{
		utf8_string_type str("abc", alloc);

		int idx = 0;
		for(typename utf8_string_type::iterator i = str.begin(), isize = str.end(); i != isize; ++i)
		{
			*i = 'd' + idx;
			++idx;
		}

		yggr_test_assert(str == "def");
	}

	{
		utf8_string_type str("abc", alloc);

		//int idx = 0;
		for(typename utf8_string_type::iterator i = str.begin(); i != str.cend(); ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		utf8_string_type str("abc", alloc);

		//int idx = 0;
		for(typename utf8_string_type::non_const_iterator i = str.ncbegin(); i != str.cend(); ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		utf8_string_type str("abc", alloc);

		//int idx = 0;
		for(typename utf8_string_type::non_const_iterator i = str.ncbegin(); i != str.end(); ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		utf8_string_type str("abc", alloc);

		typename utf8_string_type::non_const_iterator iter = str.begin();
		typename utf8_string_type::const_iterator citer = str.begin();
		typename utf8_string_type::const_iterator citer2 = str.ncbegin();

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
}

template<typename Utf8String, typename SegCont>
void test_element_access(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	utf8_string_type str("a你b好c", alloc);
	for(int i = 0, isize = str.length(); i != isize; ++i)
	{
		std::cout << str[i] << std::endl;
		std::cout << str.at(i) << std::endl;
	}

	std::cout << str.template str<yggr::string>() << std::endl;

}

//-----------------------

template<typename Utf8String, typename SegCont>
void test_modify(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// append_test
	utf8_string_type str("a你b好c", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	yggr_test_assert(str.length() == 5);

	str.append("d汉字f", YGGR_STR_UTF8_STRING_CHARSET_NAME());
	yggr_test_assert(str.length() == 9);
	yggr_test_assert(str == yggr::utf8_string("a你b好cd汉字f", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
	std::cout << str << std::endl;

	str.append(5, 'z');
	yggr_test_assert(str.length() == 14);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.append(5, L'多');
#else
    str.append(5, "多", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 19);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.append(5, yggr::utf8_char(L'人'));
#else
    str.append(5, yggr::utf8_char("人", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 24);
	std::cout << str << std::endl;

	str.append("a游a戏a", YGGR_STR_UTF8_STRING_CHARSET_NAME());
	yggr_test_assert(str.length() == 29);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.append(L"w游w戏w");
#else
    str.append("w游w戏w", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 34);
	std::cout << str << std::endl;

	str.append(yggr::string("boost游boost戏boost"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	yggr_test_assert(str.length() == 34 + 15 + 2);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.append(yggr::wstring(L"stdw游stdw戏stdw"));
#else
    str.append(yggr::string("stdw游stdw戏stdw"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 51 + 4 * 3 + 2);
	std::cout << str << std::endl;

	str = yggr::utf8_string("手游", YGGR_STR_UTF8_STRING_CHARSET_NAME());
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
	str = L"页游";
#else
    str = yggr::utf8_string("页游", YGGR_STR_UTF8_STRING_CHARSET_NAME());
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

	yggr::string stl_str("这是汉字");
	str.append(stl_str.begin(), stl_str.end(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	yggr_test_assert(str.length() == 20);
	std::cout << str << std::endl;

	utf8_string_type utf8_str("这是UTF8汉字", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	str.append(utf8_str.begin(), utf8_str.end());
	yggr_test_assert(str.length() == 28);
	std::cout << str << std::endl;

	str.append(utf8_str.rbegin(), utf8_str.rend());
	yggr_test_assert(str.length() == 36);
	std::cout << str << std::endl;

	std::cout << utf8_str.substr(1, 8) << std::endl;

	str = yggr::utf8_string("端游", YGGR_STR_UTF8_STRING_CHARSET_NAME());
	str.append(utf8_str, 1, 3);
	yggr_test_assert(str.length() == 5);
	std::cout << str << std::endl;
	//push_back_test

	str.push_back('d');
#ifndef YGGR_NO_CWCHAR
	str.push_back(L'然');
	str.push_back(yggr::utf8_char(L'后'));
#else
    str.push_back("然", YGGR_STR_UTF8_STRING_CHARSET_NAME());
    str.push_back(yggr::utf8_char("后", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#endif // YGGR_NO_CWCHAR

	yggr_test_assert(str.length() == 8);
	std::cout << str << std::endl;

	str.assign("assign汉字", YGGR_STR_UTF8_STRING_CHARSET_NAME());
	yggr_test_assert(str.length() == 8);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.assign(L"assign_w汉字");
#else
    str.assign("assign_w汉字", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 10);
	std::cout << str << std::endl;

	str.assign(yggr::string("stl_assign_汉字"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	yggr_test_assert(str.length() == 13);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.assign(yggr::wstring(L"stl_assign_w汉字"));
#else
    str.assign(yggr::string("stl_assign_w汉字"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 14);
	std::cout << str << std::endl;

	str.assign(boost::container::string("boost_assign_汉字"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	yggr_test_assert(str.length() == 15);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.assign(boost::container::wstring(L"boost_assign_w汉字"));
#else
    str.assign(boost::container::string("boost_assign_w汉字"), YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 16);
	std::cout << str << std::endl;

	yggr::string stl_str2("iter_stl_assign_汉字");
	str.assign(stl_str2.begin(), stl_str2.end(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	yggr_test_assert(str.length() == 18);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	yggr::wstring stl_wstr2(L"iter_stl_assign_w汉字");
	str.assign(stl_wstr2.begin(), stl_wstr2.end());
#else
    yggr::string stl_wstr2("iter_stl_assign_w汉字", YGGR_STR_UTF8_STRING_CHARSET_NAME());
    str.assign(stl_wstr2.begin(), stl_wstr2.end(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 19);
	std::cout << str << std::endl;

	boost::container::string boost_str2("iter_boost_assign_汉字");
	str.assign(boost_str2.begin(), boost_str2.end(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	yggr_test_assert(str.length() == 20);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	boost::container::wstring boost_wstr2(L"iter_boost_assign_w汉字");
	str.assign(boost_wstr2.begin(), boost_wstr2.end());
#else
    boost::container::string boost_wstr2("iter_boost_assign_w汉字");
    str.assign(boost_wstr2.begin(), boost_wstr2.end(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 21);
	std::cout << str << std::endl;

	utf8_string_type u_str(str.begin(), str.end(), alloc);
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

	//insert test

	str = "abc";

	str.insert(1, 2, 'k');
	yggr_test_assert(str.length() == 5);
	std::cout << str << std::endl;

#ifndef YGGR_NO_CWCHAR
	str.insert(2, 2, L'中');
#else
    str.insert(2, 2, "中", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
	yggr_test_assert(str.length() == 7);
	std::cout << str << std::endl;

	{
		typename utf8_string_type::iterator iter = str.insert(str.begin() + 3, 2, 't');
		yggr_test_assert(iter - str.begin() == 3);
		std::cout << yggr::utf8_char(iter, iter + 1) << std::endl;
	}

	{
#	ifndef YGGR_NO_CWCHAR
		typename utf8_string_type::iterator iter = str.insert(str.begin() + 3, 2, L'大');
#	else
		typename utf8_string_type::iterator iter = str.insert(str.begin() + 3, 2, "大", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#	endif // YGGR_NO_CWCHAR
		yggr_test_assert(iter - str.begin() == 3);
		std::cout << yggr::utf8_char(iter, iter + 1) << std::endl;
	}

	str.insert(str.begin(), "z");
#ifndef YGGR_NO_CWCHAR
	str.insert(str.begin(), L"卡");
#else
    str.insert(str.begin(), "卡", YGGR_STR_UTF8_STRING_CHARSET_NAME());
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
		yggr::wstring str_ins(L"啊不错的哦发给");
		str.insert(str.begin(), str_ins.rbegin(), str_ins.rend());
#else
        yggr::string str_ins("啊不错的哦发给");
        str.insert(str.begin(), str_ins.begin(), str_ins.end(), YGGR_STR_UTF8_STRING_CHARSET_NAME()); // can't use rbegin() rend()
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 35);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		utf8_string_type ustr("这是个", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		str.insert(str.begin(), ustr.begin(), ustr.end());
		yggr_test_assert(str.length() == 10);

		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		utf8_string_type ustr("这是个", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		str.insert(str.begin(), ustr.rbegin(), ustr.rend());
		yggr_test_assert(str.length() == 10);

		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		utf8_string_type ustr("这是个", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		str.insert(str.end(), ustr.rbegin(), ustr.rend());
		yggr_test_assert(str.length() == 10);

		std::cout << str << std::endl;
	}

	// erase test
	{
		str = yggr::charset::make_string_charset_helper("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		str.erase();
		yggr_test_assert(str.empty());
	}

	{
		str = yggr::charset::make_string_charset_helper("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		str.erase(0, 100);
		yggr_test_assert(str.empty());
	}

	{
		str = yggr::charset::make_string_charset_helper("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		str.erase(1, 100);
		yggr_test_assert(str.length() == 1);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		str.erase(6, 1);
		yggr_test_assert(str.length() == 6);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
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
		str = yggr::charset::make_string_charset_helper("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
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
	    str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str.length() == 10);
		str.replace(1, 5, 7, 'a');
		yggr_test_assert(str.length() == 12);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str.length() == 10);
#ifndef YGGR_NO_CWCHAR
		str.replace(1, 5, 7, L'啊');
#else
        str.replace(1, 5, 7, "啊", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 12);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str.length() == 10);

		std::cout << yggr::utf8_char(L'哦') << std::endl;
#ifndef YGGR_NO_CWCHAR
		str.replace(8, 2, 7, yggr::utf8_char(L'哦'));
#else
        str.replace(8, 2, 7, yggr::utf8_char("哦", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#endif //__GNUC__
		yggr_test_assert(str.length() == 15);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str.length() == 10);
#ifndef YGGR_NO_CWCHAR
		str.replace(0, 6, L"字符串");
#else
        str.replace(0, 6, "字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
		int n = str.length();
		yggr_test_assert(str.length() == 7);
		std::cout << n << ", "<< str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str.length() == 10);
		str.replace(7, 3, "string");
		yggr_test_assert(str.length() == 13);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str.length() == 10);
#ifndef YGGR_NO_CWCHAR
		str.replace(0, 6, yggr::wstring(L"字符串"), 0, 2);
#else
        str.replace(0, 6, yggr::string("字符串"), 0, 4, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 6);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr::string sstr("abc");
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr::string sstr("abc");
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str.length() == 10);
#ifndef YGGR_NO_CWCHAR
		yggr::wstring sstr(L"你好吗");
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
#else
        yggr::string sstr("你好吗");
        str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
#ifndef YGGR_NO_CWCHAR
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr::wstring sstr(L"你好吗");
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
#endif // YGGR_NO_CWCHAR
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#ifndef YGGR_NO_CWCHAR
		utf8_string_type sstr(L"你好吗", alloc);
#else
        utf8_string_type sstr("你好吗", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.begin(), sstr.end());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#ifndef YGGR_NO_CWCHAR
		utf8_string_type sstr(L"你好吗", alloc);
#else
        utf8_string_type sstr("你好吗", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr.rbegin(), sstr.rend());
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#ifndef YGGR_NO_CWCHAR
		utf8_string_type sstr(L"你好吗", alloc);
#else
        utf8_string_type sstr("你好吗", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 10);
		str.replace(str.begin(), str.begin() + 6, sstr);
		yggr_test_assert(str.length() == 7);
		std::cout << str << std::endl;
	}

	{
		str = yggr::charset::make_string_charset_helper("string 字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#ifndef YGGR_NO_CWCHAR
		utf8_string_type sstr(L"你好吗", alloc);
#else
        utf8_string_type sstr("你好吗", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.length() == 10);
		str.replace(0, 6, sstr, 0, 1);
		yggr_test_assert(str.length() == 5);
		std::cout << str << std::endl;
	}
}

template<typename Utf8String, typename SegCont>
void test_string_operations(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

#ifndef YGGR_NO_CWCHAR
	utf8_string_type str(L"utf8字符串", alloc);
#else
    utf8_string_type str("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR

	// find
	{
		yggr_test_assert(str.find('u') == 0);
		yggr_test_assert(str.find('u', 1) == utf8_string_type::npos);
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find(L'字') == 4);
		yggr_test_assert(str.find(L'字', 5) == utf8_string_type::npos);
		yggr_test_assert(str.find(yggr::utf8_char(L'字')) == 4);
		yggr_test_assert(str.find(yggr::utf8_char(L'字'), 5) == utf8_string_type::npos);
#else
        yggr_test_assert(str.find("字", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find("字", 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
		yggr_test_assert(str.find(yggr::utf8_char("字", YGGR_STR_UTF8_STRING_CHARSET_NAME())) == 4);
		yggr_test_assert(str.find(yggr::utf8_char("字", YGGR_STR_UTF8_STRING_CHARSET_NAME()), 5) == utf8_string_type::npos);
#endif // YGGR_NO_CWCHAR
	}

	{
		yggr::string sstr("utf8");
		yggr_test_assert(str.find(sstr) == 0);
		yggr_test_assert(str.find("utf8") == 0);
		yggr_test_assert(str.find(sstr, 1) == utf8_string_type::npos);
		yggr_test_assert(str.find("utf8", 1) == utf8_string_type::npos);
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr::wstring sstr(L"字符串");
		yggr_test_assert(str.find(sstr) == 4);
		yggr_test_assert(str.find(L"字符串") == 4);
		yggr_test_assert(str.find(sstr, 5) == utf8_string_type::npos);
		yggr_test_assert(str.find(L"字符串", 5) == utf8_string_type::npos);
#else
        yggr::string sstr("字符串");
        yggr_test_assert(str.find(sstr, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find("字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find(sstr, 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
		yggr_test_assert(str.find("字符串", 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
#endif // YGGR_NO_CWCHAR
	}

	{
#ifndef YGGR_NO_CWCHAR
		utf8_string_type sstr(L"tf8字符串", alloc);
#else
        utf8_string_type sstr("tf8字符串", alloc);
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.find(sstr) == 1);
		yggr_test_assert(str.find(sstr, 2) == utf8_string_type::npos);
	}

	// rfind
	{
		yggr_test_assert(str.rfind('u') == 0);
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.rfind(L'字') == 4);
		yggr_test_assert(str.rfind(L'字', 3) == utf8_string_type::npos);
		yggr_test_assert(str.rfind(yggr::utf8_char(L'字')) == 4);
		yggr_test_assert(str.rfind(yggr::utf8_char(L'字'), 3) == utf8_string_type::npos);
#else
        yggr_test_assert(str.rfind("字", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.rfind("字", 3, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
		yggr_test_assert(str.rfind(yggr::utf8_char("字", YGGR_STR_UTF8_STRING_CHARSET_NAME())) == 4);
		yggr_test_assert(str.rfind(yggr::utf8_char("字", YGGR_STR_UTF8_STRING_CHARSET_NAME()), 3) == utf8_string_type::npos);
#endif // YGGR_NO_CWCHAR
	}

	{
		yggr::string sstr("utf8");
		yggr_test_assert(str.rfind(sstr) == 0);
		yggr_test_assert(str.rfind("utf8") == 0);
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr::wstring sstr(L"字符串");
		yggr_test_assert(str.rfind(sstr) == 4);
		yggr_test_assert(str.rfind(L"字符串") == 4);
		yggr_test_assert(str.rfind(sstr, 3) == utf8_string_type::npos);
		yggr_test_assert(str.rfind(L"字符串", 3) == utf8_string_type::npos);
#else
        yggr::string sstr("字符串");
		yggr_test_assert(str.rfind(sstr, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.rfind("字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.rfind(sstr, 3, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
		yggr_test_assert(str.rfind("字符串", 3, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
#endif // YGGR_NO_CWCHAR
	}

	{
#ifndef YGGR_NO_CWCHAR
		utf8_string_type sstr(L"tf8字符串", alloc);
#else
        utf8_string_type sstr("tf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.rfind(sstr) == 1);
		yggr_test_assert(str.rfind(sstr, 0) == utf8_string_type::npos);
	}

	// find_first_of
	{
		//str == "utf8字符串"
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_first_of(L'字') == 4);
		yggr_test_assert(str.find_first_of(L'字', 1) == 4);
		yggr_test_assert(str.find_first_of(L'字', 5) == utf8_string_type::npos);
		yggr_test_assert(str.find_first_of('a') == utf8_string_type::npos);
		yggr_test_assert(str.find_first_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_first_of("abc") == utf8_string_type::npos);
		yggr_test_assert(str.find_first_of(L"字abc") == 4);
		yggr_test_assert(str.find_first_of(L"abc") == utf8_string_type::npos);
		yggr_test_assert(str.find_first_of(L"字abc", 1) == 4);
		yggr_test_assert(str.find_first_of(L"字abc",5) == utf8_string_type::npos);
#else
        yggr_test_assert(str.find_first_of("字", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_first_of("字", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_first_of("字", 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
		yggr_test_assert(str.find_first_of('a') == utf8_string_type::npos);
		yggr_test_assert(str.find_first_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_first_of("abc") == utf8_string_type::npos);
		yggr_test_assert(str.find_first_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_first_of("abc") == utf8_string_type::npos);
		yggr_test_assert(str.find_first_of("字abc", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_first_of("字abc",5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
#endif // YGGR_NO_CWCHAR
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_first_of(utf8_string_type(L"字abc", alloc)) == 4);
		yggr_test_assert(str.find_first_of(utf8_string_type(L"abc", alloc)) == utf8_string_type::npos);
		yggr_test_assert(str.find_first_of(utf8_string_type(L"字abc", alloc), 1) == 4);
		yggr_test_assert(str.find_first_of(utf8_string_type(L"字abc", alloc),5) == utf8_string_type::npos);
#else
        yggr_test_assert(str.find_first_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == 4);
		yggr_test_assert(str.find_first_of(utf8_string_type("abc", alloc)) == utf8_string_type::npos);
		yggr_test_assert(str.find_first_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1) == 4);
		yggr_test_assert(str.find_first_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()),5) == utf8_string_type::npos);
#endif // YGGR_NO_CWCHAR
	}

	// find first_not_of

	{
		//str == "utf8字符串"
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_first_not_of(L'字') == 0);
		yggr_test_assert(str.find_first_not_of(L'字', 1) == 1);
		yggr_test_assert(str.find_first_not_of(L'字', 5) == 5);
		yggr_test_assert(str.find_first_not_of('a') == 0);
		yggr_test_assert(str.find_first_not_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);
		yggr_test_assert(str.find_first_not_of("abc") == 0);
		yggr_test_assert(str.find_first_not_of(L"字abc") == 0);
		yggr_test_assert(str.find_first_not_of(L"abc") == 0);
		yggr_test_assert(str.find_first_not_of(L"字abc", 1) == 1);
		yggr_test_assert(str.find_first_not_of(L"字abc",5) == 5);
		yggr_test_assert(str.find_first_not_of(L"utf8字符串", 1) == utf8_string_type::npos);
		yggr_test_assert(str.find_first_not_of(L"", str.length()) == utf8_string_type::npos);
		yggr_test_assert(str.find_first_not_of(L"", str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_first_not_of(L"") == 0);
#else
        yggr_test_assert(str.find_first_not_of("字", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);
		yggr_test_assert(str.find_first_not_of("字", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		yggr_test_assert(str.find_first_not_of("字", 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 5);
		yggr_test_assert(str.find_first_not_of('a') == 0);
		yggr_test_assert(str.find_first_not_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);
		yggr_test_assert(str.find_first_not_of("abc") == 0);
		yggr_test_assert(str.find_first_not_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);
		yggr_test_assert(str.find_first_not_of("abc") == 0);
		yggr_test_assert(str.find_first_not_of("字abc", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		yggr_test_assert(str.find_first_not_of("字abc", 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 5);
		yggr_test_assert(str.find_first_not_of("utf8字符串", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
		yggr_test_assert(str.find_first_not_of("", str.length()) == utf8_string_type::npos);
		yggr_test_assert(str.find_first_not_of("", str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_first_not_of("") == 0);
#endif // YGGR_NO_CWCHAR
	}

	{

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_first_not_of(utf8_string_type(L"字abc", alloc)) == 0);
		yggr_test_assert(str.find_first_not_of(utf8_string_type(L"abc", alloc)) == 0);
		yggr_test_assert(str.find_first_not_of(utf8_string_type(L"字abc", alloc), 1) == 1);
		yggr_test_assert(str.find_first_not_of(utf8_string_type(L"字abc", alloc),5) == 5);
		yggr_test_assert(str.find_first_not_of(utf8_string_type(L"utf8字符串", alloc), 1) == utf8_string_type::npos);

		yggr_test_assert(str.find_first_not_of(utf8_string_type(L"", alloc), str.length()) == utf8_string_type::npos);
		yggr_test_assert(str.find_first_not_of(utf8_string_type(L"", alloc), str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_first_not_of(utf8_string_type(L"", alloc)) == 0);
#else
        yggr_test_assert(str.find_first_not_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == 0);
		yggr_test_assert(str.find_first_not_of(utf8_string_type("abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == 0);
		yggr_test_assert(str.find_first_not_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1) == 1);
		yggr_test_assert(str.find_first_not_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()),5) == 5);
		yggr_test_assert(str.find_first_not_of(utf8_string_type("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1) == utf8_string_type::npos);

		yggr_test_assert(str.find_first_not_of(utf8_string_type("", alloc), str.length()) == utf8_string_type::npos);
		yggr_test_assert(str.find_first_not_of(utf8_string_type("", alloc), str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_first_not_of(utf8_string_type("", alloc)) == 0);
#endif // YGGR_NO_CWCHAR

	}

	// find_last_of
	{
		//str == "utf8字符串"
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_of(L'字') == 4);
		yggr_test_assert(str.find_last_of(L'字', 1) == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of(L'字', 5) == 4);
		yggr_test_assert(str.find_last_of('a') == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_last_of("abc") == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of(L"字abc") == 4);
		yggr_test_assert(str.find_last_of(L"abc") == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of(L"字abc", 1) == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of(L"字abc",5) == 4);
		yggr_test_assert(str.find_last_of('u', 0) == 0);
		yggr_test_assert(str.find_last_of("u", 0) == 0);
#else
        yggr_test_assert(str.find_last_of("字", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_last_of("字", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of("字", 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_last_of('a') == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_last_of("abc") == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_last_of("abc") == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of("字abc", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of("字abc", 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 4);
		yggr_test_assert(str.find_last_of('u', 0) == 0);
		yggr_test_assert(str.find_last_of("u", 0) == 0);
#endif // YGGR_NO_CWCHAR
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_of(utf8_string_type(L"字abc", alloc)) == 4);
		yggr_test_assert(str.find_last_of(utf8_string_type(L"abc", alloc)) == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of(utf8_string_type(L"字abc", alloc), 1) == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of(utf8_string_type(L"字abc", alloc),5) == 4);
#else
        yggr_test_assert(str.find_last_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == 4);
		yggr_test_assert(str.find_last_of(utf8_string_type("abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1) == utf8_string_type::npos);
		yggr_test_assert(str.find_last_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()),5) == 4);
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_of(utf8_string_type("u", alloc), 0) == 0);
	}

	// find_last_not_of
	{
		//str == "utf8字符串"
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_not_of(L'字') == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(L'字', 1) == 1);
		yggr_test_assert(str.find_last_not_of(L'字', 5) == 5);
		yggr_test_assert(str.find_last_not_of('a') == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("字abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(L"字abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(L"abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(L"字abc", 1) == 1);
		yggr_test_assert(str.find_last_not_of(L"字abc",5) == 5);
		yggr_test_assert(str.find_last_not_of(L"utf8字符串", 1) == utf8_string_type::npos);
		yggr_test_assert(str.find_last_not_of(L"", 0) == 0);
		yggr_test_assert(str.find_last_not_of(L"", str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(L"") == str.length() - 1);
#else
        yggr_test_assert(str.find_last_not_of("字", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("字", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		yggr_test_assert(str.find_last_not_of("字", 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 5);
		yggr_test_assert(str.find_last_not_of('a') == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("字abc", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("abc") == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("字abc", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 1);
		yggr_test_assert(str.find_last_not_of("字abc", 5, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 5);
		yggr_test_assert(str.find_last_not_of("utf8字符串", 1, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == utf8_string_type::npos);
		yggr_test_assert(str.find_last_not_of("", 0) == 0);
		yggr_test_assert(str.find_last_not_of("", str.length() - 1) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of("") == str.length() - 1);
#endif // YGGR_NO_CWCHAR
	}

	{
#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.find_last_not_of(utf8_string_type(L"字abc", alloc)) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(utf8_string_type(L"abc", alloc)) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(utf8_string_type(L"字abc", alloc), 1) == 1);
		yggr_test_assert(str.find_last_not_of(utf8_string_type(L"字abc", alloc),5) == 5);
		yggr_test_assert(str.find_last_not_of(utf8_string_type(L"utf8字符串", alloc), 1) == utf8_string_type::npos);

		yggr_test_assert(str.find_last_not_of(utf8_string_type(L"", alloc), 0) == 0);
		yggr_test_assert(str.find_last_not_of(utf8_string_type(L"", alloc), str.length() - 1) == str.length() - 1);
#else
        yggr_test_assert(str.find_last_not_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(utf8_string_type("abc", alloc)) == str.length() - 1);
		yggr_test_assert(str.find_last_not_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1) == 1);
		yggr_test_assert(str.find_last_not_of(utf8_string_type("字abc", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()),5) == 5);
		yggr_test_assert(str.find_last_not_of(utf8_string_type("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()), 1) == utf8_string_type::npos);

		yggr_test_assert(str.find_last_not_of(utf8_string_type("", alloc), 0) == 0);
		yggr_test_assert(str.find_last_not_of(utf8_string_type("", alloc), str.length() - 1) == str.length() - 1);
#endif // YGGR_NO_CWCHAR
	}

	// compare_eq
	{
		//str = "utf8字符串";
		yggr_test_assert(str.compare_eq("utf8") == false);
		yggr_test_assert(str.compare_eq("utf8_string") == false);
		yggr_test_assert(str.compare_eq("utf8字符串长", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);
		yggr_test_assert(str.compare_eq("") == false);
		yggr_test_assert(str.compare_eq("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == true);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare_eq(L"utf8") == false);
		yggr_test_assert(str.compare_eq(L"utf8_string") == false);
		yggr_test_assert(str.compare_eq(L"utf8字符串长") == false);
		yggr_test_assert(str.compare_eq(L"") == false);
		yggr_test_assert(str.compare_eq(L"utf8字符串") == true);
#else
        yggr_test_assert(str.compare_eq("utf8") == false );
		yggr_test_assert(str.compare_eq("utf8_string") == false );
		yggr_test_assert(str.compare_eq("utf8字符串长", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);
		yggr_test_assert(str.compare_eq("") == false);
		yggr_test_assert(str.compare_eq("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == true);
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(str.compare_eq(0, str.length(), "utf8", 0, 1) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), "utf8_string", 2, 7) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), "utf8", 0, 0) == false);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare_eq(0, str.length(), L"utf8", 0, 1) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), L"utf8_string", 2, 7) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), L"utf8字符串长长长", 0, 8) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), L"utf8", 0, 0) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), L"长utf8字符串长长", 1, 7) == true);
#else
        yggr_test_assert(str.compare_eq(0, str.length(), "utf8", 0, 1) == false );
		yggr_test_assert(str.compare_eq(0, str.length(), "utf8_string", 2, 7) == false);
		yggr_test_assert(str.compare_eq(0, str.length(), "utf8", 0, 0) == false);
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare_eq(4, 2, "字符", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == true);
		yggr_test_assert(str.compare_eq(4, 2, L"字符") == true);
		yggr_test_assert(str.compare_eq(4, 2, yggr::string("字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == true);
		yggr_test_assert(str.compare_eq(4, 2, yggr::wstring(L"字符")) == true);

		yggr_test_assert(str.compare_eq(4, 2, "字符长", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);
		yggr_test_assert(str.compare_eq(4, 2, L"字符长") == false);
		yggr_test_assert(str.compare_eq(4, 2, yggr::string("字符长"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);
		yggr_test_assert(str.compare_eq(4, 2, yggr::wstring(L"字符长")) == false);

		yggr_test_assert(str.compare_eq(4, 3, "字符", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);
		yggr_test_assert(str.compare_eq(4, 3, L"字符") == false);
		yggr_test_assert(str.compare_eq(4, 3, yggr::string("字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);
		yggr_test_assert(str.compare_eq(4, 3, yggr::wstring(L"字符")) == false);

		yggr_test_assert(str.compare_eq(4, 2, utf8_string_type("字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == true);
		yggr_test_assert(str.compare_eq(4, 2, utf8_string_type(L"字符", alloc)) == true);

		yggr_test_assert(str.compare_eq(4, 2, utf8_string_type("字符长", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == false);
		yggr_test_assert(str.compare_eq(4, 2, yggr::wstring(L"字符长")) == false);

		yggr_test_assert(str.compare_eq(4, 3, utf8_string_type("字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == false);
		yggr_test_assert(str.compare_eq(4, 3, utf8_string_type(L"字符", alloc)) == false);
#else
        yggr_test_assert(str.compare_eq(4, 2, "字符", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == true);
		yggr_test_assert(str.compare_eq(4, 2, yggr::string("字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == true);

		yggr_test_assert(str.compare_eq(4, 2, "字符长", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);
		yggr_test_assert(str.compare_eq(4, 2, yggr::string("字符长"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);

		yggr_test_assert(str.compare_eq(4, 3, "字符", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);
		yggr_test_assert(str.compare_eq(4, 3, yggr::string("字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == false);

		yggr_test_assert(str.compare_eq(4, 2, utf8_string_type("字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == true);

		yggr_test_assert(str.compare_eq(4, 2, utf8_string_type("字符长", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == false);

		yggr_test_assert(str.compare_eq(4, 3, utf8_string_type("字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == false);
#endif // YGGR_NO_CWCHAR
	}

	// compare
	{
		//str = "utf8字符串";
		yggr_test_assert(str.compare("utf8") > 0);
		yggr_test_assert(str.compare("utf8_string") > 0);
		yggr_test_assert(str.compare("utf8字符串长", YGGR_STR_UTF8_STRING_CHARSET_NAME()) < 0);
		yggr_test_assert(str.compare("") > 0);
		yggr_test_assert(str.compare("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare(L"utf8") > 0);
		yggr_test_assert(str.compare(L"utf8_string") > 0);
		yggr_test_assert(str.compare(L"utf8字符串长") < 0);
		yggr_test_assert(str.compare(L"") > 0);
		yggr_test_assert(str.compare(L"utf8字符串") == 0);
#else
        yggr_test_assert(str.compare("utf8") > 0 );
		yggr_test_assert(str.compare("utf8_string") > 0 );
		yggr_test_assert(str.compare("utf8字符串长", YGGR_STR_UTF8_STRING_CHARSET_NAME()) < 0);
		yggr_test_assert(str.compare("") > 0);
		yggr_test_assert(str.compare("utf8字符串", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);
#endif // YGGR_NO_CWCHAR

		yggr_test_assert(str.compare(0, str.length(), "utf8", 0, 1) > 0);
		yggr_test_assert(str.compare(0, str.length(), "utf8_string", 2, 7) > 0);
		yggr_test_assert(str.compare(0, str.length(), "utf8", 0, 0) > 0);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare(0, str.length(), L"utf8", 0, 1) > 0);
		yggr_test_assert(str.compare(0, str.length(), L"utf8_string", 2, 7) > 0);
		yggr_test_assert(str.compare(0, str.length(), L"utf8字符串长长长", 0, 8) < 0);
		yggr_test_assert(str.compare(0, str.length(), L"utf8", 0, 0) > 0);
		yggr_test_assert(str.compare(0, str.length(), L"长utf8字符串长长", 1, 7) == 0);
#else
        yggr_test_assert(str.compare(0, str.length(), "utf8", 0, 1) > 0 );
		yggr_test_assert(str.compare(0, str.length(), "utf8_string", 2, 7) > 0);
		yggr_test_assert(str.compare(0, str.length(), "utf8", 0, 0) > 0);
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str.compare(4, 2, "字符", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);
		yggr_test_assert(str.compare(4, 2, L"字符") == 0);
		yggr_test_assert(str.compare(4, 2, yggr::string("字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);
		yggr_test_assert(str.compare(4, 2, yggr::wstring(L"字符")) == 0);

		yggr_test_assert(str.compare(4, 2, "字符长", YGGR_STR_UTF8_STRING_CHARSET_NAME()) < 0);
		yggr_test_assert(str.compare(4, 2, L"字符长") < 0);
		yggr_test_assert(str.compare(4, 2, yggr::string("字符长"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) < 0);
		yggr_test_assert(str.compare(4, 2, yggr::wstring(L"字符长")) < 0);

		yggr_test_assert(str.compare(4, 3, "字符", YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(str.compare(4, 3, L"字符") > 0);
		yggr_test_assert(str.compare(4, 3, yggr::string("字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(str.compare(4, 3, yggr::wstring(L"字符")) > 0);

		yggr_test_assert(str.compare(4, 2, utf8_string_type("字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == 0);
		yggr_test_assert(str.compare(4, 2, utf8_string_type(L"字符", alloc)) == 0);

		yggr_test_assert(str.compare(4, 2, utf8_string_type("字符长", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) < 0);
		yggr_test_assert(str.compare(4, 2, yggr::wstring(L"字符长")) < 0);

		yggr_test_assert(str.compare(4, 3, utf8_string_type("字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
		yggr_test_assert(str.compare(4, 3, utf8_string_type(L"字符", alloc)) > 0);
#else
        yggr_test_assert(str.compare(4, 2, "字符", YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);
		yggr_test_assert(str.compare(4, 2, yggr::string("字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == 0);

		yggr_test_assert(str.compare(4, 2, "字符长", YGGR_STR_UTF8_STRING_CHARSET_NAME()) < 0);
		yggr_test_assert(str.compare(4, 2, yggr::string("字符长"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) < 0);

		yggr_test_assert(str.compare(4, 3, "字符", YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);
		yggr_test_assert(str.compare(4, 3, yggr::string("字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > 0);

		yggr_test_assert(str.compare(4, 2, utf8_string_type("字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) == 0);

		yggr_test_assert(str.compare(4, 2, utf8_string_type("字符长", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) < 0);

		yggr_test_assert(str.compare(4, 3, utf8_string_type("字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME())) > 0);
#endif // YGGR_NO_CWCHAR
	}
}

template<typename Utf8String, typename SegCont>
void non_member_foo_test(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// operator +
	{
	    utf8_string_type str("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());

	    const utf8_string_type& cref_str = str;
		std::cout << yggr::charset::make_string_charset_helper("utf8字符", YGGR_STR_UTF8_STRING_CHARSET_NAME()) + str + "aaa" << std::endl;
		std::cout << yggr::charset::make_string_charset_helper(yggr::string("utf8字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) + str << std::endl;
		std::cout << yggr::charset::make_string_charset_helper("utf8字符", YGGR_STR_UTF8_STRING_CHARSET_NAME()) + str + yggr::string("aaa") << std::endl;

		std::cout << utf8_string_type("utf8字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()) + str << std::endl;
		std::cout << utf8_string_type("utf8字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()) + cref_str << std::endl;
		std::cout << utf8_string_type("utf8字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()) + str + yggr::string("aaa") << std::endl;

#ifndef YGGR_NO_CWCHAR
		std::cout << L"utf8字符" + str + L"bbb" << std::endl;
		std::wcout << yggr::wstring(L"utf8字符") + str << std::endl;
		std::cout << L"utf8字符" + str + yggr::wstring(L"bbb") << std::endl;
		std::cout << utf8_string_type(L"utf8字符", alloc) + str + yggr::wstring(L"bbb") << std::endl;
#endif // YGGR_NO_CWCHAR
	}

	// operator +=
	{
		utf8_string_type str("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr::string rst_str("operaotr+= ");
		rst_str += str;
		std::cout << rst_str << std::endl;

#ifndef YGGR_NO_CWCHAR
		yggr::wstring rst_wstr(L"operaotr+= ");
		rst_wstr += str;
		
		std::wcout << rst_wstr << std::endl;
#endif // YGGR_NO_CWCHAR
	}

	// operator ==
	{
	    utf8_string_type str("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str == yggr::utf8_string_view("utf8字符串"));
		yggr_test_assert(yggr::utf8_string_view("utf8字符串") == str);

		yggr_test_assert(str == yggr::charset::make_string_charset_helper(yggr::string("utf8字符串"), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(yggr::charset::make_string_charset_helper(yggr::string("utf8字符串"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) == str);

		yggr_test_assert(str == utf8_string_type("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(utf8_string_type("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()) == str);

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str == L"utf8字符串");
		yggr_test_assert(L"utf8字符串" == str);

		yggr_test_assert(str == yggr::wstring(L"utf8字符串"));
		yggr_test_assert(yggr::wstring(L"utf8字符串") == str);

		yggr_test_assert(str == utf8_string_type(L"utf8字符串", alloc));
		yggr_test_assert(utf8_string_type(L"utf8字符串", alloc) == str);
#endif // YGGR_NO_CWCHAR
	}

	// operator !=
	{

		utf8_string_type str("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str != "utf8");
		yggr_test_assert("utf8" != str);
		yggr_test_assert(str != "");
		yggr_test_assert("" != str);

		yggr_test_assert(str != yggr::string("utf8"));
		yggr_test_assert(yggr::string("utf8") != str);
		yggr_test_assert(str != yggr::string(""));
		yggr_test_assert(yggr::string("") != str);

		yggr_test_assert(str != utf8_string_type("utf8", alloc));
		yggr_test_assert(utf8_string_type("utf8", alloc) != str);
		yggr_test_assert(str != utf8_string_type("", alloc));
		yggr_test_assert(utf8_string_type("", alloc) != str);


#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str != L"utf8");
		yggr_test_assert(L"utf8" != str);
		yggr_test_assert(str != L"");
		yggr_test_assert(L"" != str);

		yggr_test_assert(str != yggr::wstring(L"utf8"));
		yggr_test_assert(yggr::wstring(L"utf8") != str);
		yggr_test_assert(str != yggr::wstring(L""));
		yggr_test_assert(yggr::wstring(L"") != str);

		yggr_test_assert(str != utf8_string_type(L"utf8", alloc));
		yggr_test_assert(utf8_string_type(L"utf8", alloc) != str);
		yggr_test_assert(str != utf8_string_type(L"", alloc));
		yggr_test_assert(utf8_string_type(L"", alloc) != str);

#endif // YGGR_NO_CWCHAR
	}

	// operator <
	{
	    utf8_string_type str("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());

		yggr_test_assert(str < yggr::utf8_string_view("utf8字符串长"));
		yggr_test_assert(str < yggr::charset::make_string_charset_helper(yggr::string("utf8字符串长"), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert("" < str );

		yggr_test_assert(!(yggr::utf8_string_view("utf8字符串长") < str));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string("utf8字符串长"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) < str));

		yggr_test_assert(str < utf8_string_type("utf8字符串长", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str < L"utf8字符串长");
		yggr_test_assert(str < yggr::wstring(L"utf8字符串长"));

		yggr_test_assert(!(L"utf8字符串长" < str));
		yggr_test_assert(!(yggr::wstring(L"utf8字符串长") < str));
		yggr_test_assert(!(str < yggr::wstring(L"")));

		yggr_test_assert(!(utf8_string_type(L"utf8字符串长", alloc) < str));
#endif // YGGR_NO_CWCHAR
	}

	// operator <=
	{
	    utf8_string_type str("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());

		yggr_test_assert(str <= yggr::utf8_string_view("utf8字符串长"));
		yggr_test_assert(str <= yggr::utf8_string_view("utf8字符串"));

		yggr_test_assert(str <= yggr::charset::make_string_charset_helper(yggr::string("utf8字符串长"), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(str <= yggr::charset::make_string_charset_helper(yggr::string("utf8字符串"), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(!(yggr::utf8_string_view("utf8字符串长") <= str));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string("utf8字符串长"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) <= str));

		yggr_test_assert(str <= utf8_string_type("utf8字符串长", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str <= L"utf8字符串长");
		yggr_test_assert(str <= L"utf8字符串");
		yggr_test_assert(str <= yggr::wstring(L"utf8字符串长"));
		yggr_test_assert(str <= yggr::wstring(L"utf8字符串"));

		yggr_test_assert(!(L"utf8字符串长" <= str));
		yggr_test_assert(!(yggr::wstring(L"utf8字符串长") <= str));

		yggr_test_assert(!(utf8_string_type(L"utf8字符串长", alloc) <= str));
#endif // YGGR_NO_CWCHAR
	}
//
	// operator >
	{
	    utf8_string_type str("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str > yggr::utf8_string_view("utf8字符"));
		yggr_test_assert(str > yggr::charset::make_string_charset_helper(yggr::string("utf8字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(str > "" );

		yggr_test_assert(!(yggr::utf8_string_view("utf8字符") > str));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string("utf8字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) > str));

		yggr_test_assert(str > utf8_string_type("utf8字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str > L"utf8字符");
		yggr_test_assert(str > yggr::wstring(L"utf8字符"));

		yggr_test_assert(!(L"utf8字符" > str));
		yggr_test_assert(!(yggr::wstring(L"utf8字符") > str));

		yggr_test_assert(!(utf8_string_type(L"utf8字符", alloc) > str));
#endif // YGGR_NO_CWCHAR
	}

	// operator >=
	{
	    utf8_string_type str("utf8字符串", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());

		yggr_test_assert(str >= yggr::utf8_string_view("utf8字符"));
		yggr_test_assert(str >= yggr::utf8_string_view("utf8字符串"));
		yggr_test_assert(str >= yggr::charset::make_string_charset_helper(yggr::string("utf8字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()));
		yggr_test_assert(str >= yggr::charset::make_string_charset_helper(yggr::string("utf8字符串"), YGGR_STR_UTF8_STRING_CHARSET_NAME()));

		yggr_test_assert(!(yggr::utf8_string_view("utf8字符") >= str));
		yggr_test_assert(!(yggr::charset::make_string_charset_helper(yggr::string("utf8字符"), YGGR_STR_UTF8_STRING_CHARSET_NAME()) >= str));

		yggr_test_assert(str >= utf8_string_type("utf8字符", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()));

#ifndef YGGR_NO_CWCHAR
		yggr_test_assert(str >= L"utf8字符");
		yggr_test_assert(str >= L"utf8字符串");

		yggr_test_assert(str >= yggr::wstring(L"utf8字符"));
		yggr_test_assert(str >= yggr::wstring(L"utf8字符串"));

		yggr_test_assert(!(L"utf8字符" >= str));
		yggr_test_assert(!(yggr::wstring(L"utf8字符") >= str));

		yggr_test_assert(!(utf8_string_type(L"utf8字符", alloc) >= str));
#endif // YGGR_NO_CWCHAR
	}

	// swap
	{
		utf8_string_type str1_chk("utf8字符串1", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		utf8_string_type str2_chk("utf8字符串2", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());

		utf8_string_type str1(str1_chk, alloc);
		utf8_string_type str2(str2_chk, alloc);

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
		utf8_string_type str1_chk("utf8字符串111", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		utf8_string_type str2_chk("utf8字符串222", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());

		utf8_string_type str1(str1_chk, alloc);
		utf8_string_type str2(str2_chk, alloc);


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

template<typename Utf8String, typename SegCont>
void test_std_replace(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	{
		utf8_string_type str("abcdefg", alloc);
		std::replace(str.begin(), str.end(), 'd', 'z');
		std::replace_if(str.begin(), str.end(),
							boost::bind(std::equal_to<typename utf8_string_type::char_type>(),
											typename utf8_string_type::char_type('a', alloc), _1), 'z');
		yggr_test_assert(str == "zbczefg");
	}

	{
		utf8_string_type str("abcdefg", alloc);
		std::replace(boost::begin(str), boost::end(str), 'd', 'z');
		std::replace_if(boost::begin(str), boost::end(str),
							boost::bind(std::equal_to<typename utf8_string_type::char_type>(),
											typename utf8_string_type::char_type('a', alloc), _1), 'z');
		yggr_test_assert(str == "zbczefg");
	}

#ifndef YGGR_NO_CWCHAR

	{
		utf8_string_type str(L"你好,世界", alloc);
		std::replace(str.begin(), str.end(), L'你', L'您');
		std::replace_if(str.begin(), str.end(),
							boost::bind(std::equal_to<typename utf8_string_type::char_type>(),
											typename utf8_string_type::char_type(',', alloc), _1), '-');
		yggr_test_assert(str == L"您好-世界");
	}

	{
		utf8_string_type str(L"你好,世界", alloc);
		std::replace(boost::begin(str), boost::end(str), L'你', L'您');
		std::replace_if(boost::begin(str), boost::end(str),
							boost::bind(std::equal_to<typename utf8_string_type::char_type>(),
											typename utf8_string_type::char_type(',', alloc), _1), '-');
		yggr_test_assert(str == L"您好-世界");
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_std_replace succeed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_io(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	std::cout << "test_io start" << std::endl;

	utf8_string_type in(alloc);
	std::cin >> in;
	std::cout << in << std::endl;

#ifndef YGGR_NO_CWCHAR
	std::wcin >> in;
	std::cout << in << std::endl;
#endif // YGGR_NO_CWCHAR

	std::cout << "test_io succeed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void test_basic_string_getline(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

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

template<typename Utf8String, typename SegCont>
void test_utf8_string_getline(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

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
		getline(std::wcin, str, L'你');

		std::cout << str << std::endl;
	}

	// const Char*
	{
		yggr::charset::utf8_string str;
		getline(std::wcin, str, "你");

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

template<typename Utf8String, typename SegCont>
void test_container_conv(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef typename utf8_string_type::base_type string_type;
	typedef typename utf8_string_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());
	{

#if 0
		utf8_string_type str("abc", alloc);
		yggr::vector<typename utf8_string_type::char_type> vt(str.begin(), str.end());
#else
		yggr::utf8_string str("abc");
		std::vector<yggr::utf8_string::char_type> vt(str.begin(), str.end());
#endif // 0, 1

		yggr_test_assert(str.length() == vt.size());

		{
			for(std::size_t i = 0, isize = str.length(); i != isize; ++i)
			{
				yggr_test_assert(str[i] == vt[i]);
			}
		}
#if 0
		// support modify_iterator only, other_iterator don't support it,
		// because destroys the semantics of the const_iterator
		vt.assign(str.cbegin(), str.cend());
#else
		vt.assign(str.begin(), str.end());
#endif // 0,1
		yggr_test_assert(str.length() == vt.size());

		{
			for(std::size_t i = 0, isize = str.length(); i != isize; ++i)
			{
				yggr_test_assert(str[i] == vt[i]);
			}
		}
	}

	{
		utf8_string_type str("abc", alloc);
		std::list<typename utf8_string_type::char_type> vt(str.begin(), str.end());

		yggr_test_assert(str.length() == vt.size());

		{
			typename std::list<typename utf8_string_type::char_type>::const_iterator iter = vt.begin();
			for(std::size_t i = 0, isize = str.length(); i != isize; ++i, ++iter)
			{
				yggr_test_assert(str[i] == *iter);
			}
		}
#if 0
		// support modify_iterator only, other_iterator don't support it,
		// because destroys the semantics of the const_iterator
		vt.assign(str.cbegin(), str.cend());
#else
		vt.assign(str.begin(), str.end());
#endif // 0,1
		yggr_test_assert(str.length() == vt.size());

		{
			typename std::list<typename utf8_string_type::char_type>::const_iterator iter = vt.begin();
			for(std::size_t i = 0, isize = str.length(); i != isize; ++i, ++iter)
			{
				yggr_test_assert(str[i] == *iter);
			}
		}
	}

	std::cout << "test_container_conv succeed" << std::endl;
}

template<typename Utf8String, typename SegCont>
void debug_test(SegCont& seg_cont)
{
	typedef Utf8String utf8_string_type;
	typedef SegCont seg_cont_type;

#if defined(TEST_CONSTRUCT)
	test_construct<utf8_string_type>(seg_cont);
#endif // TEST_CONSTRUCT

	test_operator_set<utf8_string_type>(seg_cont);
	test_swap<utf8_string_type>(seg_cont);

	test_append<utf8_string_type>(seg_cont);
	test_push_back<utf8_string_type>(seg_cont);
	test_pop_back<utf8_string_type>(seg_cont);
	test_insert<utf8_string_type>(seg_cont);
	test_replace<utf8_string_type>(seg_cont);
	test_find<utf8_string_type>(seg_cont);
	test_rfind<utf8_string_type>(seg_cont);
	test_find_first_of<utf8_string_type>(seg_cont);
	test_find_first_not_of<utf8_string_type>(seg_cont);
	test_find_last_of<utf8_string_type>(seg_cont);
	test_find_last_not_of<utf8_string_type>(seg_cont);
	test_resize_relength<utf8_string_type>(seg_cont);
	test_copy<utf8_string_type>(seg_cont);

	test_iterator<utf8_string_type>(seg_cont);
	test_element_access<utf8_string_type>(seg_cont);
	test_modify<utf8_string_type>(seg_cont);
	test_string_operations<utf8_string_type>(seg_cont);
	non_member_foo_test<utf8_string_type>(seg_cont);

	test_std_replace<utf8_string_type>(seg_cont);

	test_container_conv<utf8_string_type>(seg_cont);

#	ifdef TEST_IO
	test_basic_string_getline<utf8_string_type>(seg_cont);
	test_utf8_string_getline<utf8_string_type>(seg_cont);
	test_io<utf8_string_type>(seg_cont);
#	endif // TEST_IO
}

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // #ifndef YGGR_NO_CWCHAR

	// managed_shared_memory
	{
		typedef boost::interprocess::managed_shared_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<char, seg_type::segment_manager> alloc_type;
		typedef
			yggr::charset::utf8_string_impl
			<
				char,
				yggr::charset::basic_string<char>::traits_type,
				alloc_type
			> utf8_string_type;

		seg_cont_type seg_cont("test_managed_shared_memory_utf8_string",
								yggr::segment::segment_op::create_only(),
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"test_managed_shared_memory_utf8_string") );

		debug_test<utf8_string_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	wait_any_key(argc, argv);
	return 0;
}
