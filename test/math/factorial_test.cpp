// factorial_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <cassert>
#include <yggr/base/yggrdef.h>
#include <yggr/math/factorial.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

template<typename T>
void test()
{
   yggr_test_assert((yggr::math::factorial(0) == 1));
   yggr_test_assert((yggr::math::factorial(1) == 1));
   yggr_test_assert((yggr::math::factorial(2) == 2));
   yggr_test_assert((yggr::math::factorial(3) == 6));
   yggr_test_assert((yggr::math::factorial(4) == 24));
   yggr_test_assert((yggr::math::factorial(5) == 120));

   yggr_test_assert((yggr::math::factorial(static_cast<T>(0)) == static_cast<T>(1)));
   yggr_test_assert((yggr::math::factorial(static_cast<T>(1)) == static_cast<T>(1)));
   yggr_test_assert((yggr::math::factorial(static_cast<T>(2)) == static_cast<T>(2)));
   yggr_test_assert((yggr::math::factorial(static_cast<T>(3)) == static_cast<T>(6)));
   yggr_test_assert((yggr::math::factorial(static_cast<T>(4)) == static_cast<T>(24)));
   yggr_test_assert((yggr::math::factorial(static_cast<T>(5)) == static_cast<T>(120)));
}

void test_meta(void)
{
	yggr_test_assert(yggr::math::factorial_t<0>::value == yggr::math::factorial(0));
	yggr_test_assert(yggr::math::factorial_t<1>::value == yggr::math::factorial(1));
	yggr_test_assert(yggr::math::factorial_t<2>::value == yggr::math::factorial(2));
	yggr_test_assert(yggr::math::factorial_t<3>::value == yggr::math::factorial(3));
	yggr_test_assert(yggr::math::factorial_t<4>::value == yggr::math::factorial(4));
	yggr_test_assert(yggr::math::factorial_t<5>::value == yggr::math::factorial(5));
	yggr_test_assert(yggr::math::factorial_t<6>::value == yggr::math::factorial(6));
	yggr_test_assert(yggr::math::factorial_t<7>::value == yggr::math::factorial(7));
	yggr_test_assert(yggr::math::factorial_t<8>::value == yggr::math::factorial(8));
	yggr_test_assert(yggr::math::factorial_t<9>::value == yggr::math::factorial(9));

	yggr_test_assert(yggr::math::factorial_t<10>::value == yggr::math::factorial(10));
	yggr_test_assert(yggr::math::factorial_t<11>::value == yggr::math::factorial(11));
	yggr_test_assert(yggr::math::factorial_t<12>::value == yggr::math::factorial(12));
}

int main(int argc, char* argv[])
{
	test<yggr::s8>();
	test<yggr::u8>();
	test<yggr::s16>();
	test<yggr::u16>();
	test<yggr::s32>();
	test<yggr::u32>();
	test<yggr::s64>();
	test<yggr::u64>();

	test<yggr::f32>();
	test<yggr::f64>();

	test_meta();

	wait_any_key(argc, argv);
	return 0;
}