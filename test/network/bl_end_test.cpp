//bl_end_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/network/hn_conv.hpp>

int main(int argc, char* argv[])
{
	std::cout << yggr::network::is_little_endian() << std::endl;
	std::cout << yggr::network::is_big_endian() << std::endl;

	std::cout << sizeof(std::time(0)) << std::endl;

	//std::cout << std::hex << (int)'l' << std::endl;
	//std::cout << std::hex << (int)'?' << std::endl;
	//std::cout << std::hex << (int)'b' << std::endl;
	std::cout << static_cast<yggr::s64>(0xaad9cf7e552630b5) << std::endl;
	std::cout << static_cast<yggr::s64>(0xffffffff81302655) << std::endl;

	wait_any_key(argc, argv);
	return 0;
}