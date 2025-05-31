//test_msvc_maroc.cpp

#define YGGR_TEST_PP_DEBUG

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/compile_link/linker.hpp>
#include <yggr/ppex/pp_debug.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

# pragma comment YGGR_COMPILE_LINK_LIB(yggr_base)
# pragma comment YGGR_COMPILE_LINK_LIB(yggr_base)

#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
	std::cout << YGGR_PP_DEBUG(YGGR_PP_LINK_LIB(yggr_base)) << std::endl;
	std::cout << YGGR_PP_DEBUG(YGGR_COMPILE_LINK_LIB(yggr_base)) << std::endl;

	wait_any_key(argc, argv);
	return 0;
}