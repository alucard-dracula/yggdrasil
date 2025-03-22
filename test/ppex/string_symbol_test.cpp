//string_symbol_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/ppex/string_symbol.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

int main(int argc, char* argv[])
{
	std::cout << YGGR_PP_STRING_SYMBOL( YGGR_PP_SYMBOL_EQUAL_TO ) << std::endl;

	wait_any_key(argc, argv);
	return 0;
}