// run_server.cpp

#include "run_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void run_server(void)
{
	assert(svr_ptr_single_type::get_ins());
	svr_ptr_single_type::get_ins()->run();
}
