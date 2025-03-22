// division_ex_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/base/yggrdef.h>
#include <yggr/math/division_ex.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_div_floor(void)
{
	std::cout << "----------------test_div_floor-------------" << std::endl;

	yggr_test_assert(yggr::math::division_floor(11, 2) == 5);
	std::cout << "division_floor(11, 2) = "<< yggr::math::division_floor(11, 2) << std::endl;

	yggr_test_assert(yggr::math::division_floor(11.0, 2) == 5);
	std::cout << "division_floor(11.0, 2) = " << yggr::math::division_floor(11.0, 2) << std::endl;

	yggr_test_assert(yggr::math::division_floor(11, 3) == 3);
	std::cout << "division_floor(11, 3) = " << yggr::math::division_floor(11, 3) << std::endl;

	yggr_test_assert(yggr::math::division_floor(11.0, 3) == 3);
	std::cout << "division_floor(11.0, 3) = " << yggr::math::division_floor(11.0, 3) << std::endl;

	yggr_test_assert(yggr::math::division_floor(11, 5) == 2);
	std::cout << "division_floor(11, 5) = " << yggr::math::division_floor(11, 5) << std::endl;

	yggr_test_assert(yggr::math::division_floor(11.0, 5) == 2);
	std::cout << "division_floor(11.0, 5) = " << yggr::math::division_floor(11.0, 5) << std::endl;

}

void test_div_round(void)
{
	std::cout << "----------------test_div_round-------------" << std::endl;

	yggr_test_assert(yggr::math::division_round(11, 2) == 6);
	std::cout << "division_round(11, 2) = " << yggr::math::division_round(11, 2) << std::endl;

	yggr_test_assert(yggr::math::division_round(11.0, 2) == 6);
	std::cout << "division_round(11.0, 2) = " << yggr::math::division_round(11.0, 2) << std::endl;

	yggr_test_assert(yggr::math::division_round(11, 3) == 4);
	std::cout << "division_round(11, 3) = " << yggr::math::division_round(11, 3) << std::endl;

	yggr_test_assert(yggr::math::division_round(11.0, 3) == 4);
	std::cout << "division_round(11.0, 3) = " << yggr::math::division_round(11.0, 3) << std::endl;

	yggr_test_assert(yggr::math::division_round(11, 5) == 2);
	std::cout << "division_round(11, 5) = " << yggr::math::division_round(11, 5) << std::endl;

	yggr_test_assert(yggr::math::division_round(11.0, 5) == 2);
	std::cout << "division_round(11.0, 5) = " << yggr::math::division_round(11.0, 5) << std::endl;
}

void test_div_ceil(void)
{
	std::cout << "----------------test_div_ceil-------------" << std::endl;
	yggr_test_assert(yggr::math::division_ceil(11, 2) == 6);
	std::cout << "division_ceil(11, 2) = " << yggr::math::division_ceil(11, 2) << std::endl;

	yggr_test_assert(yggr::math::division_ceil(11.0, 2) == 6);
	std::cout << "division_ceil(11.0, 2) = " << yggr::math::division_ceil(11.0, 2) << std::endl;

	yggr_test_assert(yggr::math::division_ceil(11, 3) == 4);
	std::cout << "division_ceil(11, 3) = " << yggr::math::division_ceil(11, 3) << std::endl;

	yggr_test_assert(yggr::math::division_ceil(11.0, 3) == 4);
	std::cout << "division_ceil(11.0, 3) = " << yggr::math::division_ceil(11.0, 3) << std::endl;

	yggr_test_assert(yggr::math::division_ceil(11, 5) == 3);
	std::cout << "division_ceil(11, 5) = " << yggr::math::division_ceil(11, 5) << std::endl;

	yggr_test_assert(yggr::math::division_ceil(11.0, 5) == 3);
	std::cout << "division_ceil(11.0, 5) = "  << yggr::math::division_ceil(11.0, 4) << std::endl;
}

int main(int argc, char* argv[])
{
	test_div_floor();
	test_div_round();
	test_div_ceil();

	wait_any_key(argc, argv);
	return 0;
}