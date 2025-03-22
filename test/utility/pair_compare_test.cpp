// pair_compare_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/utility/pair_compare.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test1(void)
{
	{
		typedef std::pair<int, int> pair1_type;
		typedef std::pair<int, int> pair2_type;

		pair1_type pair1 = std::make_pair(1, 1);
		pair2_type pair2 = std::make_pair(1, 1);

		yggr_test_assert((pair1 == pair2));
		yggr_test_assert(!(pair1 != pair2));
		yggr_test_assert(!(pair1 < pair2));
		yggr_test_assert((pair1 <= pair2));
		yggr_test_assert(!(pair1 > pair2));
		yggr_test_assert((pair1 >= pair2));
	}

	{
		typedef std::pair<int, int> pair1_type;
		typedef std::pair<int, int> pair2_type;

		pair1_type pair1 = std::make_pair(1, 1);
		pair2_type pair2 = std::make_pair(1, 2);

		yggr_test_assert(!(pair1 == pair2));
		yggr_test_assert((pair1 != pair2));
		yggr_test_assert((pair1 < pair2));
		yggr_test_assert((pair1 <= pair2));
		yggr_test_assert(!(pair1 > pair2));
		yggr_test_assert(!(pair1 >= pair2));
	}

	{
		typedef std::pair<int, int> pair1_type;
		typedef std::pair<int, int> pair2_type;

		pair1_type pair1 = std::make_pair(2, 1);
		pair2_type pair2 = std::make_pair(1, 1);

		yggr_test_assert(!(pair1 == pair2));
		yggr_test_assert((pair1 != pair2));
		yggr_test_assert(!(pair1 < pair2));
		yggr_test_assert(!(pair1 <= pair2));
		yggr_test_assert((pair1 > pair2));
		yggr_test_assert((pair1 >= pair2));
	}
}

void test2(void)
{
	{
		typedef std::pair<const int, int> pair1_type;
		typedef std::pair<int, const int> pair2_type;

		pair1_type pair1 = std::make_pair(1, 1);
		pair2_type pair2 = std::make_pair(1, 1);

		yggr_test_assert((pair1 == pair2));
		yggr_test_assert(!(pair1 != pair2));
		yggr_test_assert(!(pair1 < pair2));
		yggr_test_assert((pair1 <= pair2));
		yggr_test_assert(!(pair1 > pair2));
		yggr_test_assert((pair1 >= pair2));
	}

	{
		typedef std::pair<const int, int> pair1_type;
		typedef std::pair<int, const int> pair2_type;

		pair1_type pair1 = std::make_pair(1, 1);
		pair2_type pair2 = std::make_pair(1, 2);

		yggr_test_assert(!(pair1 == pair2));
		yggr_test_assert((pair1 != pair2));
		yggr_test_assert((pair1 < pair2));
		yggr_test_assert((pair1 <= pair2));
		yggr_test_assert(!(pair1 > pair2));
		yggr_test_assert(!(pair1 >= pair2));
	}

	{
		typedef std::pair<const int, int> pair1_type;
		typedef std::pair<int, const int> pair2_type;

		pair1_type pair1 = std::make_pair(2, 1);
		pair2_type pair2 = std::make_pair(1, 1);

		yggr_test_assert(!(pair1 == pair2));
		yggr_test_assert((pair1 != pair2));
		yggr_test_assert(!(pair1 < pair2));
		yggr_test_assert(!(pair1 <= pair2));
		yggr_test_assert((pair1 > pair2));
		yggr_test_assert((pair1 >= pair2));
	}
}

int main(int argc, char* argv[])
{
	test1();
	wait_any_key(argc, argv);
	return 0;
}