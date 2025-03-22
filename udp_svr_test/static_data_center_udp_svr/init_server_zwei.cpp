//init_server.cpp

#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void run_server(void)
{
	svr_ptr_single_type::obj_ptr_type psvr = svr_ptr_single_type::get_ins();
	if(psvr)
	{
		psvr->run();
	}
}