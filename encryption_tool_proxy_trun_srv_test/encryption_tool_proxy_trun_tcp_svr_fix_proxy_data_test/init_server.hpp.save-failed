//init_server.hpp

#ifndef __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
#define __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__

#include "config.hpp"
#include "calculator.hpp"

#include <cassert>

bool init_server(yggr::u32 thread_size, yggr::u16 port);

void run_server(svr_ptr_type psvr);
void stop_server(svr_ptr_type& psvr);
void clear_server(::yggr::shared_ptr<proxy_msg_calculator> pcal);
void uninstall_server(void);

#endif // __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
