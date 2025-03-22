//init_server.cpp

#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void run_server(svr_ptr_type psvr)
{
	assert(psvr);
	psvr->run();
}

void stop_server(svr_ptr_type& psvr)
{
	assert(psvr);
	psvr->stop();
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	{
	    svr_ptr_type tmp;
		psvr.swap(tmp);
	}
}