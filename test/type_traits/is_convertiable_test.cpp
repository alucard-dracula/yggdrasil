// is_convertiable_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>

#include <yggr/type_traits/is_convertible.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

enum e_a
{
	E_1 = 0,
	E_2
};

int main(int argc, char* argv[])
{
	BOOST_MPL_ASSERT((yggr::is_convertible<char, int>));
	BOOST_MPL_ASSERT((yggr::is_convertible<int, char>));

	BOOST_MPL_ASSERT((yggr::is_convertible<int, float>));
	BOOST_MPL_ASSERT((yggr::is_convertible<float, int>));

	BOOST_MPL_ASSERT_NOT((yggr::is_convertible<int, e_a>));

	wait_any_key(argc, argv);
	return 0;
}