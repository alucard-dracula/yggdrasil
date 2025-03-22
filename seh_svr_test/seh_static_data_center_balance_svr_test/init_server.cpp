//init_server.cpp

#include "init_server.hpp"

#if defined(YGGR_USE_SEH)

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void run_server(svr_ptr_type psvr)
{
	assert(psvr);
	psvr->run();
}

#endif // #if defined(YGGR_USE_SEH)

