//cpp_version_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

//#if !defined(YGGR_CPP_VERSION)
//#	error "YGGR_CPP_VERSION not defined"
//#endif // YGGR_CPP_VERSION

int main(int argc, char* argv[])
{
	std::cout << YGGR__CPLUSPLUS << std::endl;
	std::cout << YGGR_CPP_VERSION << std::endl;
	std::cout << YGGR_CPP_VERSION << std::endl;

	wait_any_key(argc, argv);
	return 0;
}