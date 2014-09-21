//runtime_speed_test_test.cpp

#include <iostream>
#include <yggr/time/time.hpp>
#include <yggr/time/runtime_speed_test.hpp>

typedef yggr::time::time time_type;
typedef yggr::time::runtime_speed_test<time_type, yggr::f64, std::stringstream> speed_tester_type;

void test(void)
{
	std::stringstream ss;
	std::cout << "input any test waitting time" << std::endl;
	{
		speed_tester_type tester(1.0f, ss);
		char cc = 0;
		std::cin >> cc;
	}

	std::cout << ss.str() << std::endl;
}


int main(int argc, char* argv[])
{
	test();
	char cc = 0;
	std::cin >> cc;
	return 0;
}