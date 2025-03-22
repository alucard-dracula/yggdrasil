//modulus_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/math/math.hpp>

#include <typeinfo>
#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	int a = 10;
	int b = 20;
	float c = 20.5;

	std::cout << yggr::math::modulus(a, b) << std::endl;
	yggr_test_assert(typeid(yggr::math::modulus(a, b)) == typeid(a / b));

	std::cout << yggr::math::modulus(a, c) << std::endl;
	yggr_test_assert(typeid(yggr::math::modulus(a, c)) == typeid(a / c));

	wait_any_key(argc, argv);
	return 0;
}
