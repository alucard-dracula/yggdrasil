// params_expand_test.cpp

#define YGGR_TEST_PP_DEBUG

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/ppex/pp_debug.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/tuple_ex/tuple.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	std::cout << YGGR_PP_DEBUG(YGGR_PP_FOO_TYPES_DEF(BOOST_TUPLE_LIMIT_LENGTH()) ) << std::endl;
	std::cout << YGGR_PP_DEBUG(YGGR_PP_FOO_TYPES_DEF(BOOST_TUPLE_LIMIT_LENGTH()) ) << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
