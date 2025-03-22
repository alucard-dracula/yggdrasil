// init_timer_mgr.cpp

#include "init_timer_mgr.hpp"

void init_timer_mgr(svr_ptr_type psvr, timer_mgr_type& timer_mgr)
{
	timer_mgr.add_task<heart_timer_task_pak_type>(psvr, yggr::time::time(10, 0));
}


void start_timer_mgr(timer_mgr_type& timer_mgr)
{
	timer_mgr.start();
}