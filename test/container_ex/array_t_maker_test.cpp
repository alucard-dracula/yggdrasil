//array_ref_t_maker_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container_ex/array_t_maker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	typedef yggr::array_reference_t_maker<int, 10>::type arr_ref_type;
	typedef yggr::array_const_reference_t_maker<int, 10>::type arr_cref_type;
	typedef yggr::array_t_maker<int, 10>::type array_type;

	int arr[10] = {1, 2, 3};

	arr_ref_type arr_ref = arr;
	arr_ref[0] = 100;
	yggr_test_assert(arr_ref[0] == 100);

	arr_cref_type arr_cref = arr;
	yggr_test_assert(arr_cref[0] == 100);

	array_type arr2 = {100, 2, 3};

	yggr_test_assert(0 == memcmp(arr, arr2, sizeof(arr2)));

	wait_any_key(argc, argv);
	return 0;
}