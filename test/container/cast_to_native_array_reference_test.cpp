//cast_to_native_array_reference_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/container/array.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	boost::array<int, 10> arr = {1, 2, 3};

	const boost::array<int, 10>& arr_cref = arr;

	yggr::container::cast_to_native_array_reference(arr)[0] = 100;

	yggr_test_assert(arr[0] == 100);

	yggr_test_assert(yggr::container::cast_to_native_array_reference(arr_cref)[0] == 100);

	wait_any_key(argc, argv);
	return 0;
}