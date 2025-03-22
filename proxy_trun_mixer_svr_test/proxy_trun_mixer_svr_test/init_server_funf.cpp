//init_server.cpp

#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void init_heart_timer(timer_mgr_type& timer_mgr, svr_ptr_type psvr)
{
    timer_mgr.add_task<heart_timer_task_pak_type>(psvr, yggr::time::time(10, 0));
}

void stop_heart_timer(timer_mgr_type& timer_mgr)
{
    timer_mgr.stop();
    timer_mgr.clear();
}

void stop_server(svr_ptr_type& psvr, ::yggr::shared_ptr<proxy_msg_calculator> pcal)
{
    psvr->stop();
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	{
	    svr_ptr_type().swap(psvr);

	    //svr_ptr_type tmp;
		//psvr.swap(tmp);
	}

	pcal->clear();
}

void uninstall_server(void)
{
    ctrl_center_single::uninstall();
	svr_ptr_single_type::uninstall();
	proxy_task_center_single::uninstall();
	yggr::ptr_single<yggr::exception::exception>::uninstall();
}
