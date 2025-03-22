//init_server.hpp

#ifndef __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
#define __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__

#include <cassert>

#include "config.hpp"

bool init_server(yggr::u32 thread_size, yggr::u16 port, task_center_type& tc);

void run_server(svr_ptr_type psvr);

#endif // __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
