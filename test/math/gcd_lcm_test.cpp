// gcd_lcm_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <yggr/math/gcd_lcm.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

int main(int argc, char* argv[])
{
	yggr_test_assert(yggr::math::gcd(6, 4) == 2);
	yggr_test_assert(yggr::math::lcm(6, 4) == 12);

	yggr_test_assert(yggr::math::gcd(4, 6) == 2);
	yggr_test_assert(yggr::math::lcm(4, 6) == 12);

	BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::math::gcd_t<6, 4>::value == 2)>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::math::lcm_t<6, 4>::value == 12)>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::math::gcd_t<4, 6>::value == 2)>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::math::lcm_t<4, 6>::value == 12)>));


	std::cout << "all_test_end" << std::endl;

	wait_any_key(argc, argv);

	return 0;
}
