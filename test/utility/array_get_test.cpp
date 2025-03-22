//array_get_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/mpl/int.hpp>

#include <yggr/container_ex/array.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

struct tag_2 {};
YGGR_INDEX_T_VALUE_DEF(tag_2, 2)


void array_get_test(void)
{
	typedef boost::mpl::int_<0> tag_0;
	typedef boost::mpl::int_<1> tag_1;

	{
		int arr[5] = {1, 2, 3};

		yggr_test_assert(yggr::get<0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<2>(arr) == arr[2]);

		yggr_test_assert(yggr::get<tag_0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<tag_1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<tag_2>(arr) == arr[2]);
	}

#ifndef YGGR_NO_CXX11_HDR_ARRAY
	{
		std::array<int, 5> arr = {1, 2, 3};

		yggr_test_assert(yggr::get<0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<2>(arr) == arr[2]);

		yggr_test_assert(yggr::get<tag_0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<tag_1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<tag_2>(arr) == arr[2]);
	}
#endif // YGGR_NO_CXX11_HDR_ARRAY

	{
		boost::array<int, 5> arr = {1, 2, 3};

		yggr_test_assert(yggr::get<0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<2>(arr) == arr[2]);

		yggr_test_assert(yggr::get<tag_0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<tag_1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<tag_2>(arr) == arr[2]);
	}
}

int main(int argc, char* argv[])
{
	array_get_test();

	wait_any_key(argc, argv);
	return 0;
}
