//pointer_int_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/mplex/pointer_int.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	typedef yggr::mplex::pointer_int::type int_type;

	yggr_test_assert(sizeof(int_type) == sizeof(void*));

	wait_any_key(argc, argv);
	return 0;
}