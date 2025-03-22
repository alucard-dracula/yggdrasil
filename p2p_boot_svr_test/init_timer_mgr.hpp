// init_timer_mgr.hpp

#ifndef __INIT_TIMER_MGR_HPP__
#define __INIT_TIMER_MGR_HPP__

#include "p2p_boot_server_def.hpp"

void init_timer_mgr(svr_ptr_type psvr, timer_mgr_type& timer_mgr);
void start_timer_mgr(timer_mgr_type& timer_mgr);

#endif // __INIT_TIMER_MGR_HPP__