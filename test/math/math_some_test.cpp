//math_some_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/math/const_var.hpp>
#include <yggr/math/gcd_lcm.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_const_var(void)
{
	std::cout << yggr::math::const_var::pi<float>() << std::endl;
	std::cout << yggr::math::const_var::pi<double>() << std::endl;
}

void test_gcd_lcm(void)
{
	yggr_test_assert(yggr::math::gcd(1, 2) == 1);
	yggr_test_assert(yggr::math::lcm(1, 2) == 2);

	yggr_test_assert(yggr::math::gcd(-1, 2) == 0);
	yggr_test_assert(yggr::math::lcm(-1, 2) == 0);

	yggr_test_assert(yggr::math::gcd(1024, 768) == 256);
	yggr_test_assert(yggr::math::lcm(1024, 768) == 3072);
}

int main(int argc, char* argv[])
{
	test_const_var();
	test_gcd_lcm();
	
	wait_any_key(argc, argv);
	return 0;
}