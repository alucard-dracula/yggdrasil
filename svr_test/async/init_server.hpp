//init_server.hpp

#ifndef __ASYNC_TCP_SVR_INIT_SERVER_HPP__
#define __ASYNC_TCP_SVR_INIT_SERVER_HPP__

#include "config.hpp"

bool init_server(yggr::u32 trd_count, yggr::u32 port);
void run_server(svr_ptr_type psvr);

#endif // __ASYNC_TCP_SVR_INIT_SERVER_HPP__
