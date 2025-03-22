//init_server.cpp

#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void clear_server(::yggr::shared_ptr<proxy_msg_calculator> pcal)
{
	if(pcal)
	{
		pcal->clear();
	}
}

void uninstall_server(void)
{
	ctrl_center_single::uninstall();
	svr_ptr_single_type::uninstall();
	proxy_task_center_single::uninstall();
	yggr::ptr_single<yggr::exception::exception>::uninstall();
}