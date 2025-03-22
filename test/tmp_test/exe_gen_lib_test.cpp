//exe_gen_lib_test.cpp

#define YGGR_ALGORITHM_DEBUG
#define YGGR_ALGORITHM_KNUTH_MORRIS_PRATT_DEBUG

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/yggrdef.h>

//#if defined(YGGR_DLL_DEF)
//#	error "hhhh"
//#endif // YGGR_DLL_DEF
//
//
//#define BOOST_SYMBOL_EXPORT
//
//#if !defined(BOOST_SYMBOL_EXPORT)
//#	error "bbbb"
//#endif // bbb

#include <boost/serialization/singleton.hpp>

//#include <boost/config/platform/win32.hpp>
//#include <boost/serialization/force_include.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER


#include <iostream>

#include <yggr/ppex/pp_debug.hpp>

int main(int argc, char* argv[])
{
	std::cout << YGGR_PP_DEBUG((BOOST_DLLEXPORT)) << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
