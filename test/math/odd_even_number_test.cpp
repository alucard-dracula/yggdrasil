// odd_even_number_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/mpl/assert.hpp>
#include <yggr/math/odd_even_number.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_int(void)
{
	yggr_test_assert(yggr::math::is_odd_number(-2) == false);
	yggr_test_assert(yggr::math::is_even_number(-2) == true);

	std::cout << "yggr::math::is_odd_number(-2) == " << yggr::math::is_odd_number(-2) << std::endl;
	std::cout << "yggr::math::is_even_number(-2) == " << yggr::math::is_even_number(-2) << std::endl;

	yggr_test_assert(yggr::math::is_odd_number(-1) == true);
	yggr_test_assert(yggr::math::is_even_number(-1) == false);

	std::cout << "yggr::math::is_odd_number(-1) == " << yggr::math::is_odd_number(-1) << std::endl;
	std::cout << "yggr::math::is_even_number(-1) == " << yggr::math::is_even_number(-1) << std::endl;

	yggr_test_assert(yggr::math::is_odd_number(0) == false);
	yggr_test_assert(yggr::math::is_even_number(0) == true);

	std::cout << "yggr::math::is_odd_number(0) == " << yggr::math::is_odd_number(0) << std::endl;
	std::cout << "yggr::math::is_even_number(0) == " << yggr::math::is_even_number(0) << std::endl;

	yggr_test_assert(yggr::math::is_odd_number(1) == true);
	yggr_test_assert(yggr::math::is_even_number(1) == false);

	std::cout << "yggr::math::is_odd_number(1) == " << yggr::math::is_odd_number(1) << std::endl;
	std::cout << "yggr::math::is_even_number(1) == " << yggr::math::is_even_number(1) << std::endl;

	yggr_test_assert(yggr::math::is_odd_number(2) == false);
	yggr_test_assert(yggr::math::is_even_number(2) == true);

	std::cout << "yggr::math::is_odd_number(2) == " << yggr::math::is_odd_number(2) << std::endl;
	std::cout << "yggr::math::is_even_number(2) == " << yggr::math::is_even_number(2) << std::endl;

	int test_len = 10;

	for(int k = -test_len, ksize = test_len + 1; k != ksize; ++k)
	{
		yggr_test_assert(yggr::math::is_odd_number(2 * k) == false);
		yggr_test_assert(yggr::math::is_even_number(2 * k) == true);

		std::cout << "yggr::math::is_odd_number(" << 2 * k << ") == " << yggr::math::is_odd_number(2 * k) << std::endl;
		std::cout << "yggr::math::is_even_number(" << 2 * k << ") == " << yggr::math::is_even_number(2 * k) << std::endl;

		yggr_test_assert(yggr::math::is_odd_number(2 * k + 1) == true);
		yggr_test_assert(yggr::math::is_even_number(2 * k + 1) == false);

		std::cout << "yggr::math::is_odd_number(" << 2 * k + 1 << ") == " << yggr::math::is_odd_number(2 * k + 1) << std::endl;
		std::cout << "yggr::math::is_even_number(" << 2 * k + 1 << ") == " << yggr::math::is_even_number(2 * k + 1) << std::endl;
	}

	BOOST_MPL_ASSERT_NOT((yggr::math::is_odd_number_t<int, 0>));
	BOOST_MPL_ASSERT((yggr::math::is_even_number_t<int, 0>));

	BOOST_MPL_ASSERT((yggr::math::is_odd_number_t<int, 1>));
	BOOST_MPL_ASSERT_NOT((yggr::math::is_even_number_t<int, 1>));
}

void test_float(void)
{
	yggr_test_assert(yggr::math::is_odd_number(-1.0) == true);
	yggr_test_assert(yggr::math::is_even_number(-1.0) == false);

	std::cout << "yggr::math::is_odd_number(-1.0) == " << yggr::math::is_odd_number(-1.0) << std::endl;
	std::cout << "yggr::math::is_even_number(-1.0) == " << yggr::math::is_even_number(-1.0) << std::endl;

	yggr_test_assert(yggr::math::is_odd_number(0.0) == false);
	yggr_test_assert(yggr::math::is_even_number(0.0) == true);

	std::cout << "yggr::math::is_odd_number(0.0) == " << yggr::math::is_odd_number(0.0) << std::endl;
	std::cout << "yggr::math::is_even_number(0.0) == " << yggr::math::is_even_number(0.0) << std::endl;

	yggr_test_assert(yggr::math::is_odd_number(1.0) == true);
	yggr_test_assert(yggr::math::is_even_number(1.0) == false);

	std::cout << "yggr::math::is_odd_number(1.0) == " << yggr::math::is_odd_number(1.0) << std::endl;
	std::cout << "yggr::math::is_even_number(1.0) == " << yggr::math::is_even_number(1.0) << std::endl;

	int test_len = 10;

	for(int k = -test_len, ksize = test_len + 1; k != ksize; ++k)
	{
		yggr_test_assert(yggr::math::is_odd_number(2.0 * k) == false);
		yggr_test_assert(yggr::math::is_even_number(2.0 * k) == true);

		std::cout << "yggr::math::is_odd_number(" << 2.0 * k << ") == " << yggr::math::is_odd_number(2.0 * k) << std::endl;
		std::cout << "yggr::math::is_even_number(" << 2.0 * k << ") == " << yggr::math::is_even_number(2.0 * k) << std::endl;

		yggr_test_assert(yggr::math::is_odd_number(2.0 * k + 1) == true);
		yggr_test_assert(yggr::math::is_even_number(2.0 * k + 1) == false);

		std::cout << "yggr::math::is_odd_number(" << 2.0 * k + 1 << ") == " << yggr::math::is_odd_number(2.0 * k + 1) << std::endl;
		std::cout << "yggr::math::is_even_number(" << 2.0 * k + 1 << ") == " << yggr::math::is_even_number(2.0 * k + 1) << std::endl;
	}


	yggr_test_assert(yggr::math::is_odd_number(2.2) == false);
	yggr_test_assert(yggr::math::is_even_number(2.2) == false);

	std::cout << "yggr::math::is_odd_number(2.2) == " << yggr::math::is_odd_number(2.2) << std::endl;
	std::cout << "yggr::math::is_even_number(2.2) == " << yggr::math::is_even_number(2.2) << std::endl;

	yggr_test_assert(yggr::math::is_odd_number(2.5) == false);
	yggr_test_assert(yggr::math::is_even_number(2.5) == false);

	std::cout << "yggr::math::is_odd_number(2.5) == " << yggr::math::is_odd_number(2.5) << std::endl;
	std::cout << "yggr::math::is_even_number(2.5) == " << yggr::math::is_even_number(2.5) << std::endl;
}

int main(int argc, char* argv[])
{
	test_int();
	test_float();

	wait_any_key(argc, argv);
	return 0;
}