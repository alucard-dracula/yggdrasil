// is_convertiable_size_t_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>

#include <yggr/type_traits/is_convertible_size_t.hpp>

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
	BOOST_MPL_ASSERT((yggr::is_convertible_size_t<int>::type));
	BOOST_MPL_ASSERT((yggr::is_convertible_size_t<char>::type));

	BOOST_MPL_ASSERT((yggr::is_convertible_size_t<e_a>::type));

	wait_any_key(argc, argv);
	return 0;
}