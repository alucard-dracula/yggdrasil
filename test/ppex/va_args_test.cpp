//va_args_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/ppex/va_args.hpp>
//#include <yggr/ppex/pp_debug.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

int main(int argc, char* argv[])
{

#ifdef __VA_ARGS__
	yggr_test_assert((YGGR_PP_ARGS_SIZE(1, 2, 3) == 3));
	yggr_test_assert((YGGR_PP_ARGS_ELEM(0, 1, 2, 3) == 1));

#	define ARGS() 1, 2, 3, 4, 5
	yggr_test_assert((YGGR_PP_ARGS_SIZE(ARGS()) == 5));
	yggr_test_assert((YGGR_PP_ARGS_ELEM(1, ARGS()) == 2));
#	undef ARGS

#else
#	define ARGS() 1, 2, 3
	yggr_test_assert((YGGR_PP_ARGS_SIZE(ARGS()) == 3));
	yggr_test_assert((YGGR_PP_ARGS_ELEM(0, 1, 2, 3) == 1));
#	undef ARGS

#endif // __VA_ARGS__

	wait_any_key(argc, argv);
	return 0;
}
