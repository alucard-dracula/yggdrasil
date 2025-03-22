//init_server.hpp

#ifndef __PROXY_TURN_SVR_INIT_SERVER_HPP__
#define __PROXY_TURN_SVR_INIT_SERVER_HPP__

#include <cassert>

#include "config.hpp"

bool init_server(yggr::u32 trd_count, yggr::u32 port);
void run_server(svr_ptr_type psvr);

#endif // __PROXY_TURN_SVR_INIT_SERVER_HPP__
