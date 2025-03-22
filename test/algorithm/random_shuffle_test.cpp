// random_shuffle_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/algorithm/shuffle.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(base)


void test(void)
{
	int arr[] = {1, 2, 3, 4, 5 };
	int arr_chk[] = {1, 2, 3, 4, 5 };

	do
	{
		yggr::shuffle(arr + 0, arr + 5);
	} while(0 == memcmp(&arr, &arr_chk, sizeof(arr)));

	yggr_test_assert(0 != memcmp(&arr, &arr_chk, sizeof(arr)));

	std::sort(arr + 0, arr + 5);
	yggr_test_assert(0 == memcmp(&arr, &arr_chk, sizeof(arr)));
}


int main(int argc, char* argv[])
{
	yggr::random::init();
	test();

	wait_any_key(argc, argv);
	return 0;
}
