//init_server.hpp

#ifndef __UUID_STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_INIT_SERVER_HPP__
#define __UUID_STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_INIT_SERVER_HPP__

#include "config.hpp"

bool init_server(yggr::u32 trd_count, yggr::u32 port);
void run_server(svr_ptr_type psvr);

#endif // __UUID_STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_INIT_SERVER_HPP__
