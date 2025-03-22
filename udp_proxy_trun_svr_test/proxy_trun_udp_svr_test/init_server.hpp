//init_server.hpp

#ifndef __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
#define __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__

#include <cassert>

#include "config.hpp"

void init_proxy(proxy_runner_shared_info_ptr_type pinfo);
bool init_server(yggr::u32 trd_count, yggr::u32 port);
void run_server(svr_ptr_type psvr);
void stop_server(svr_ptr_type psvr);

udp_svr_handler_type::init_type create_server_handler_init(yggr::u32 port);

#endif // __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
