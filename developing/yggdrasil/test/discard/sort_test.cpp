//sort_test.cpp

#include <vector>
#include <yggr/base/yggrdef.h>
#include <yggr/base/random.h>
#include <yggr/time/time.hpp>

int main(int argc, char* argv[])
{
	yggr::random::init();

	int arr[200000] = {0};

	for(int i = 0; i != 200000; ++i)
	{
		arr[i] = yggr::random::gen_random_integer<int>();
	}

	yggr::time::time tms;
	std::sort(arr, arr + 200000);
	yggr::time::time tme;

	std::cout << tme - tms << std::endl;

	char cc = 0;
	std::cin >> cc;

	return 0;

}