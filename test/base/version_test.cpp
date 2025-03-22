//version_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

int main(int argc, char* argv[])
{
	std::cout << YGGR_VERSION_S << std::endl;
	std::cout << std::hex << YGGR_VERSION_HEX << std::endl;

	wait_any_key(argc, argv);
	return 0;
}