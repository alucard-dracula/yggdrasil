// factorial_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <cassert>
#include <yggr/base/yggrdef.h>
#include <yggr/math/sign.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void test(void)
{
	std::cout << "--------------check signed---------------" << std::endl;
	yggr_test_assert((yggr::math::sign(-10) == -1));
	yggr_test_assert((yggr::math::sign(0) == 0));
	yggr_test_assert((yggr::math::sign(10) == 1));

	std::cout << ((yggr::math::sign(-10) == -1)) << std::endl;
	std::cout << ((yggr::math::sign(0) == 0)) << std::endl;
	std::cout << ((yggr::math::sign(10) == 1)) << std::endl;


	std::cout << "--------------check float---------------" << std::endl;
	yggr_test_assert((yggr::math::sign(-10.0f) == -1));
	yggr_test_assert((yggr::math::sign(0.0f) == 0));
	yggr_test_assert((yggr::math::sign(YGGR_MATH_F32_DEFAULT_PRECISION() * 0.1f) == 0));
	yggr_test_assert((yggr::math::sign(10.0f) == 1));

	std::cout << ((yggr::math::sign(-10.0f) == -1)) << std::endl;
	std::cout << ((yggr::math::sign(0.0f) == 0)) << std::endl;
	std::cout << ((yggr::math::sign(YGGR_MATH_F32_DEFAULT_PRECISION() * 0.1f) == 0)) << std::endl;
	std::cout << ((yggr::math::sign(10.0f) == 1)) << std::endl;

	std::cout << "--------------check unsigned---------------" << std::endl;
	yggr_test_assert((yggr::math::sign(static_cast<yggr::u32>(0)) == 0));
	yggr_test_assert((yggr::math::sign(static_cast<yggr::u32>(-10)) == 1));

	std::cout << ((yggr::math::sign(static_cast<yggr::u32>(0)) == 0)) << std::endl;
	std::cout << ((yggr::math::sign(static_cast<yggr::u32>(-10)) == 1)) << std::endl;
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}