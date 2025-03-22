// bit_counter_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/utility/bit_counter.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_count_1(void)
{
	{
		yggr::u32 val = 0xffff0000;
		yggr_test_assert(yggr::bit_counter<1>::bit_count(val) == 16);
		std::cout << yggr::bit_counter<1>::bit_count(val) << " == " << 16 << std::endl;
	}

	{
		yggr::u16 val = 0xf001;
		yggr_test_assert(yggr::bit_counter<1>::bit_count(val) == 5);
		std::cout << yggr::bit_counter<1>::bit_count(val) << " == " << 5 << std::endl;
	}
};

void test_count_0(void)
{
	{
		yggr::u32 val = 0xffff0000;
		yggr_test_assert(yggr::bit_counter<0>::bit_count(val) == 32 - 16);
		std::cout << yggr::bit_counter<0>::bit_count(val) << " == " << 32 - 16 << std::endl;
	}

	{
		yggr::u16 val = 0xf001;
		yggr_test_assert(yggr::bit_counter<0>::bit_count(val) == 16 - 5);
		std::cout << yggr::bit_counter<0>::bit_count(val) << " == " << 16 - 5 << std::endl;
	}
};

int main(int argc, char* argv[])
{
	test_count_1();
	test_count_0();

	wait_any_key(argc, argv);
	return 0;
}