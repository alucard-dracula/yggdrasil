// sting_view_rangex_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <vector>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/base/yggrdef.h>

#include <yggr/charset/string_view.hpp>
#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>
#include <yggr/container_ex/is_container.hpp>
#include <yggr/iterator_ex/iterator_category_check.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void test(void)
{
	typedef yggr::string_view string_view_type;
	typedef yggr::range_ex::range_value_ex<string_view_type>::type chk_value_type;

	BOOST_MPL_ASSERT((boost::is_same<char, chk_value_type>));
	BOOST_MPL_ASSERT((yggr::is_container<yggr::string>));
	BOOST_MPL_ASSERT_NOT((yggr::is_container<string_view_type>));
	BOOST_MPL_ASSERT((yggr::is_random_access_iterator<boost::range_iterator<string_view_type>::type>));

}

int main(int argc, char* argv[])
{
	test();
	wait_any_key(argc, argv);
	return 0;
}