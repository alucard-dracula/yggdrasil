//va_args_test.cpp

#include <iostream>

#include <yggr/ppex/pp_debug.hpp>

#define TEST(...) (__VA_ARGS__)

int main(int argc, char* argv[])
{

#ifdef _DEBUG
	std::cout << (YGGR_PP_DEBUG(TEST())) << std::endl;
#endif // _DEBUG

	std::cout << TEST(1, 2, 3) << std::endl;
	char cc = 0;
	std::cin >> cc;
}
