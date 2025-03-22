// is_same_tpl_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/type_traits/is_same_tpl.hpp>

void test(void)
{
	{
		BOOST_MPL_ASSERT((yggr::is_same_tpl<int, int>));
		BOOST_MPL_ASSERT_NOT((yggr::is_same_tpl<int, char>));
	}

	{
		BOOST_MPL_ASSERT((yggr::is_same_tpl<yggr::string, yggr::string>));
		BOOST_MPL_ASSERT((yggr::is_same_tpl<yggr::string, yggr::wstring>));
		BOOST_MPL_ASSERT_NOT((yggr::is_same_tpl<yggr::string, yggr::vector<int> >));

		BOOST_MPL_ASSERT((yggr::is_same_tpl<yggr::vector<int>, yggr::vector<char> >));
	}
}

int main(int argc, char* argv[])
{
	test();
	wait_any_key(argc, argv);
	return 0;
}