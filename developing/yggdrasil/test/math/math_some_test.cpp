//math_some_test.cpp

#include <cassert>
#include <iostream>
#include <yggr/math/const_var.hpp>
#include <yggr/math/gcd_lcm.hpp>


void test_const_var(void)
{
	std::cout << yggr::math::const_var::pi<float>() << std::endl;
	std::cout << yggr::math::const_var::pi<double>() << std::endl;
}

void test_gcd_lcm(void)
{
	assert(yggr::math::gcd(1, 2) == 1);
	assert(yggr::math::lcm(1, 2) == 2);

	assert(yggr::math::gcd(-1, 2) == 0);
	assert(yggr::math::lcm(-1, 2) == 0);

	assert(yggr::math::gcd(1024, 768) == 256);
	assert(yggr::math::lcm(1024, 768) == 3072);
}

int main(int argc, char* argv[])
{
	test_const_var();
	test_gcd_lcm();
	char cc = 0;
	std::cin >> cc;
	return 0;
}