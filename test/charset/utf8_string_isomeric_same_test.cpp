//utf8_string_isomeric_same_test.cpp

//#ifndef _MSC_VER
//#   define YGGR_NO_CWCHAR
//#endif // _MSC_VER

//#define YGGR_USE_STL_STRING
//#define _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef 
	yggr::charset::utf8_string_impl
	<
		yggr::utf8_string::value_type, 
		yggr::utf8_string::traits_type, 
		yggr::utf8_string::allocator_type
	> isomeric_same_utf8_string_type;

typedef isomeric_same_utf8_string_type::base_type isomeric_same_utf8_string_base_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_string, isomeric_same_utf8_string_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_string, isomeric_same_utf8_string_type>));
#endif // (BOOST_VERSION < 107000)

void test_construct(void)
{
	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(boost::move(ims_str.org_str()));
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(ims_str.org_str());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(boost::move(ims_str.org_str()), ims_str.get_allocator());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(ims_str.org_str(), ims_str.get_allocator());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(boost::move(ims_str.org_str()), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(ims_str.org_str(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(boost::move(ims_str.org_str()),
								ims_str.get_allocator(),
								YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(ims_str.org_str(),
								ims_str.get_allocator(),
								YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str1 == ims_str_chk);
	}
	
	// this_type
	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(boost::move(ims_str));
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(ims_str);
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(boost::move(ims_str), ims_str.get_allocator());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(ims_str, ims_str.get_allocator());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(boost::move(ims_str), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(ims_str, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(boost::move(ims_str),
								ims_str.get_allocator(),
								YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1(ims_str,
								ims_str.get_allocator(),
								YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(str1 == ims_str_chk);
	}

	std::cout << "----------test_construct successed-----------" << std::endl;

}

//-----------------------

void test_string_operator_equal(void)
{
	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1;
		str1 = (boost::move(ims_str.org_str()));
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1;
		str1 = (ims_str.org_str());
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1;
		str1 = (boost::move(ims_str));
		yggr_test_assert(str1 == ims_str_chk);
	}

	{
		isomeric_same_utf8_string_type ims_str("abc");
		isomeric_same_utf8_string_type ims_str_chk(ims_str);
		yggr::utf8_string str1;
		str1 = (ims_str);
		yggr_test_assert(str1 == ims_str_chk);
	}

	std::cout << "----------test_string_operator_equal successed------------" << std::endl;
}

void test_operator_compare(void)
{
	{
		isomeric_same_utf8_string_type str1("abc");
		yggr::utf8_string str2("abc");
		yggr::utf8_string str3("abcd");
		yggr_test_assert(str1.org_str() == str2);
		yggr_test_assert(str1 == str2.org_str());
		yggr_test_assert(str1 == str2);

		yggr_test_assert(str1.org_str() != str3);
		yggr_test_assert(str1 != str3.org_str());
		yggr_test_assert(str1 != str3);

		yggr_test_assert(str1.org_str() < str3);
		yggr_test_assert(str1 < str3.org_str());
		yggr_test_assert(str1 < str3);

		yggr_test_assert(str1.org_str() <= str2);
		yggr_test_assert(str1 <= str2.org_str());
		yggr_test_assert(str1 <= str2);

		yggr_test_assert(str1.org_str() <= str3);
		yggr_test_assert(str1 <= str3.org_str());
		yggr_test_assert(str1 <= str3);

		yggr_test_assert(!(str1.org_str() > str3));
		yggr_test_assert(!(str1 > str3.org_str()));
		yggr_test_assert(!(str1 > str3));

		yggr_test_assert((str1.org_str() >= str2));
		yggr_test_assert((str1 >= str2.org_str()));
		yggr_test_assert((str1 >= str2));

		yggr_test_assert(!(str1.org_str() >= str3));
		yggr_test_assert(!(str1 >= str3.org_str()));
		yggr_test_assert(!(str1 >= str3));
	}

	std::cout << "----------test_operator_compare end-----------" << std::endl;
}

void swap_test(void)
{
	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		str1.swap(boost::move(str2.org_str()));
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		str1.swap(str2.org_str());
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		str1.swap(boost::move(str2));
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		str1.swap(str2);
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		yggr::swap(boost::move(str1.org_str()), str2);
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		yggr::swap(str1.org_str(), str2);
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		yggr::swap(str1, boost::move(str2.org_str()));
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		yggr::swap(str1, str2.org_str());
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		yggr::swap(boost::move(str1), str2);
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		yggr::swap(str1, boost::move(str2));
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	{
		isomeric_same_utf8_string_type str1("abc");
		isomeric_same_utf8_string_type str1_chk("abc");
		yggr::utf8_string str2("def");
		yggr::utf8_string str2_chk("def");
		yggr::swap(str1, str2);
		yggr_test_assert(str1 == str2_chk);
		yggr_test_assert(str2 == str1_chk);
	}

	std::cout << "----------swap_test end-----------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_string_operator_equal();
	test_operator_compare();
	swap_test();

	wait_any_key(argc, argv);
	return 0;
}
