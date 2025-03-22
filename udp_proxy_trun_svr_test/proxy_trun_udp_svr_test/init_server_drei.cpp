//init_server.cpp

#include <cassert>
#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void run_server(svr_ptr_type psvr)
{
	assert(psvr);
	psvr->run();
}

void stop_server(svr_ptr_type psvr)
{
	assert(psvr);
	psvr->stop();
}

