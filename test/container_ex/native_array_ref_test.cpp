// native_array_ref_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <cassert>
#include <yggr/container_ex/array.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test(void)
{
	typedef int arr_type[3];

	typedef yggr::container_ex::native_array_ref<arr_type> arr_ref_type;
	typedef yggr::container_ex::native_array_ref<arr_type> arr_cref_type;

	arr_type arr1 = {1, 2, 3};
	arr_type arr2 = {1, 2, 3};
	arr_type arr3 = {3, 4, 5};

	arr_ref_type arr1_ref(arr1);
	arr_ref_type arr2_ref(arr2);
	arr_ref_type arr3_ref(arr3);

	arr_cref_type arr1_cref(arr1);
	arr_cref_type arr2_cref(arr2);
	arr_cref_type arr3_cref(arr3);

	yggr_test_assert(arr1 == arr1_ref);
	yggr_test_assert(arr2 == arr2_ref);
	yggr_test_assert(arr3 == arr3_ref);

	yggr_test_assert(arr1 == arr1_cref);
	yggr_test_assert(arr2 == arr2_cref);
	yggr_test_assert(arr3 == arr3_cref);

	yggr_test_assert(!(arr1 != arr1_ref));
	yggr_test_assert(!(arr2 != arr2_ref));
	yggr_test_assert(!(arr3 != arr3_ref));

	yggr_test_assert(!(arr1 != arr1_cref));
	yggr_test_assert(!(arr2 != arr2_cref));
	yggr_test_assert(!(arr3 != arr3_cref));

	yggr_test_assert(arr1_ref == arr1_ref);
	yggr_test_assert(arr1_ref == arr2_ref);
	yggr_test_assert(arr1_ref != arr3_ref);

	yggr_test_assert(arr1_cref == arr1_cref);
	yggr_test_assert(arr1_cref == arr2_cref);
	yggr_test_assert(arr1_cref != arr3_cref);

	yggr_test_assert(arr1_ref == arr1_cref);
	yggr_test_assert(arr1_ref == arr2_cref);
	yggr_test_assert(arr1_ref != arr3_cref);

	yggr_test_assert(arr1_cref == arr1_ref);
	yggr_test_assert(arr1_cref == arr2_ref);
	yggr_test_assert(arr1_cref != arr3_ref);

	yggr_test_assert(arr1 < arr3_ref);
	yggr_test_assert(arr3 > arr1_ref);

	yggr_test_assert(arr1_ref < arr3_ref);
	yggr_test_assert(arr3_ref > arr1_ref);

	char *p = arr3_ref;
	yggr_test_assert(p);


	memcpy(arr3_ref, arr1_ref, arr1_ref.size() * sizeof(int));

	yggr_test_assert(arr3_ref == arr1_ref);
	yggr_test_assert(arr3_ref == arr1_cref);

	yggr_test_assert(0 == memcmp(arr1, arr3, sizeof(arr1)));


	std::cout << "test success" << std::endl;
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}