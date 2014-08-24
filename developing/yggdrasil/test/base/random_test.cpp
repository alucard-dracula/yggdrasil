//random_test.cpp

#include <iostream>
#include <vector>
#include <yggr/base/random.h>
#include <yggr/base/yggrdef.h>
#include <algorithm>

#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER

#define LEN 10

typedef std::vector<yggr::u32> u32vt;

void test1(void)
{
	u32vt vt;

	for(int i = 0; i < LEN; ++i)
	{
		vt.push_back(yggr::random::gen_random<yggr::u32>(0, 2));

		std::cout << vt.back() << std::endl;
	}

	std::sort(vt.begin(), vt.end());

	if(vt.front() < 0 || vt.back() > 1)
	{
		std::cout << "error" << std::endl;
	}

	//for(int i = 0; i < LEN; ++i)
	//{
	//	int k = std::count(vt.begin(), vt.end(), vt[i]);

	//	if(k > 1)
	//	{
	//		std::cout << "idx = " << i << " val = " << vt[i] << " Same = " << k << std::endl;
	//	}
	//}

	std::cout << "-----------------" << std::endl;
	{
	for(int j = 0; j < LEN; ++j)
	{
		std::cout << yggr::random::gen_random<yggr::u32>(10, 20) << std::endl;
	}
	}

	std::cout << "-----------------" << std::endl;
	{
	for(int j = 0; j < LEN; ++j)
	{
		std::cout << yggr::random::gen_random<yggr::u32>(20, 30) << std::endl;
	}
	}

	std::cout << "-----------------" << std::endl;
	{
	for(int j = 0; j < LEN; ++j)
	{
		std::cout << yggr::random::gen_random<yggr::f32>(10.0, 11.0) << std::endl;
	}
	}

	std::cout << "-----------------" << std::endl;
	{
	for(int j = 0; j < LEN; ++j)
	{
		std::cout << yggr::random::gen_random<yggr::f32>(11.0, 12.0) << std::endl;
	}
	}

	std::cout << "-----------------" << std::endl;

	//std::cout << yggr::random::gen_random<yggr::f32>(11.0, 10.0) << std::endl; //assert error
}

void test2(void)
{
	for(int i = 0; i != LEN; ++i)
	{
		std::cout << yggr::random::gen_random<yggr::f32>(10.0, 15.0) << std::endl;
	}
}

void test3(void)
{
	int arr[] = {1, 2, 3, 4, 5 };

	std::random_shuffle(arr, arr+ 5);
}

void test4(void)
{
	for(int j = 0; j < 1000000; ++j)
	{
		assert(yggr::random::gen_random<yggr::u32>(0, 2) < 2);
	}
}

int main(int argc, char* argv[])
{
	yggr::random::init();
	//test1();
	//test2();
	//test3();

	test4();

	std::cout << "end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
