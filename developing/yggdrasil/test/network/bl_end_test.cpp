//bl_end_test.cpp

#include <iostream>
#include <yggr/network/hn_conv.hpp>

int main(int argc, char* argv[])
{
	std::cout << yggr::network::is_little_end() << std::endl;
	std::cout << yggr::network::is_big_end() << std::endl;

	std::cout << sizeof(std::time(0)) << std::endl;

	//std::cout << std::hex << (int)'l' << std::endl;
	//std::cout << std::hex << (int)'?' << std::endl;
	//std::cout << std::hex << (int)'b' << std::endl;
	std::cout << (yggr::s64)0xaad9cf7e552630b5 << std::endl;
	std::cout << (yggr::s64)0xffffffff81302655 << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}