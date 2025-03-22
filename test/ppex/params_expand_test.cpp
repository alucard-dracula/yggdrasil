// params_expand_test.cpp

#define YGGR_TEST_PP_DEBUG

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/ppex/pp_debug.hpp>
#include <yggr/ppex/params_expand.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#define TEST_MACRO(_1_, _2_, _3_) _1_ - _2_ - _3_

int main(int argc, char* argv[])
{

#ifdef __VA_ARGS__

	std::cout << YGGR_PP_DEBUG(YGGR_PP_MACRO_PARAMS_EXPAND(TEST_MACRO, a, b, c)) << std::endl;

#else

#define __args__ a, b, c

	std::cout << YGGR_PP_DEBUG(YGGR_PP_MACRO_PARAMS_EXPAND(TEST_MACRO, __args__)) << std::endl;

#endif // __VA_ARGS__

	wait_any_key(argc, argv);
	return 0;
}