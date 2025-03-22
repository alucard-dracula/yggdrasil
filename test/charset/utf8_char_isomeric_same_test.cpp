//utf8_char_isomeric_same_test.cpp

//#ifndef _MSC_VER
//#   define YGGR_NO_CWCHAR
//#endif // _MSC_VER

//#define YGGR_USE_STL_STRING
//#define _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/charset/utf8_char.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


typedef 
	yggr::charset::utf8_char_impl
	<
		yggr::utf8_char::value_type, 
		yggr::utf8_char::traits_type, 
		yggr::utf8_char::allocator_type
	> isomeric_same_utf8_char_type;

typedef isomeric_same_utf8_char_type::base_type isomeric_same_utf8_char_base_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<yggr::utf8_char, isomeric_same_utf8_char_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_string<yggr::utf8_char, isomeric_same_utf8_char_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::utf8_char, isomeric_same_utf8_char_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_string<yggr::utf8_char, isomeric_same_utf8_char_type>));
#endif // (BOOST_VERSION < 107000)

void test_constructor(void)
{
	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(boost::move(ims_c.org_str()));
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(ims_c.org_str());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(boost::move(ims_c.org_str()), ims_c.get_allocator());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(ims_c.org_str(), ims_c.get_allocator());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(boost::move(ims_c.org_str()), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(ims_c.org_str(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(boost::move(ims_c.org_str()),
							ims_c.get_allocator(),
							YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(ims_c.org_str(), 
							ims_c.get_allocator(),
							YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(boost::move(ims_c));
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(ims_c);
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(boost::move(ims_c), ims_c.get_allocator());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(ims_c, ims_c.get_allocator());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(boost::move(ims_c), 
							ims_c.get_allocator(),
							YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1(ims_c, 
							ims_c.get_allocator(),
							YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(c1 == ims_chk);
	}

	std::cout << "----------test_constructor end-----------" << std::endl;
}

void test_operator_equal(void)
{
	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1;
		c1 = (boost::move(ims_c.org_str()));
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1;
		c1 = (ims_c.org_str());
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1;
		c1 = (boost::move(ims_c));
		yggr_test_assert(c1 == ims_chk);
	}

	{
		isomeric_same_utf8_char_type ims_c('a');
		isomeric_same_utf8_char_type ims_chk(ims_c);
		yggr::utf8_char c1;
		c1 = (ims_c);
		yggr_test_assert(c1 == ims_chk);
	}

	std::cout << "----------test_operator_equal end-----------" << std::endl;
}

void test_operator_compare(void)
{
	{
		isomeric_same_utf8_char_type c1("a");
		yggr::utf8_char c2("a");
		yggr::utf8_char c3("b");
		yggr_test_assert(c1.org_str() == c2);
		yggr_test_assert(c1 == c2.org_str());
		yggr_test_assert(c1 == c2);

		yggr_test_assert(c1.org_str() != c3);
		yggr_test_assert(c1 != c3.org_str());
		yggr_test_assert(c1 != c3);

		yggr_test_assert(c1.org_str() < c3);
		yggr_test_assert(c1 < c3.org_str());
		yggr_test_assert(c1 < c3);

		yggr_test_assert(c1.org_str() <= c2);
		yggr_test_assert(c1 <= c2.org_str());
		yggr_test_assert(c1 <= c2);

		yggr_test_assert(c1.org_str() <= c3);
		yggr_test_assert(c1 <= c3.org_str());
		yggr_test_assert(c1 <= c3);

		yggr_test_assert(!(c1.org_str() > c3));
		yggr_test_assert(!(c1 > c3.org_str()));
		yggr_test_assert(!(c1 > c3));

		yggr_test_assert((c1.org_str() >= c2));
		yggr_test_assert((c1 >= c2.org_str()));
		yggr_test_assert((c1 >= c2));

		yggr_test_assert(!(c1.org_str() >= c3));
		yggr_test_assert(!(c1 >= c3.org_str()));
		yggr_test_assert(!(c1 >= c3));

	}

	std::cout << "----------test_operator_compare end-----------" << std::endl;
}

void swap_test(void)
{
	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		c1.swap(boost::move(c2.org_str()));
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		c1.swap(c2.org_str());
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		c1.swap(boost::move(c2));
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		c1.swap(c2);
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		yggr::swap(boost::move(c1.org_str()), c2);
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		yggr::swap(c1.org_str(), c2);
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		yggr::swap(c1, boost::move(c2.org_str()));
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		yggr::swap(c1, c2.org_str());
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		yggr::swap(boost::move(c1), c2);
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		yggr::swap(c1, boost::move(c2));
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	{
		isomeric_same_utf8_char_type c1("a");
		isomeric_same_utf8_char_type c1_chk("a");
		yggr::utf8_char c2("b");
		yggr::utf8_char c2_chk("b");
		yggr::swap(c1, c2);
		yggr_test_assert(c1 == c2_chk);
		yggr_test_assert(c2 == c1_chk);
	}

	std::cout << "----------swap_test end-----------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_constructor();
	test_operator_equal();
	test_operator_compare();
	swap_test();

	wait_any_key(argc, argv);
	return 0;
}
