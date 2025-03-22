//init_server.hpp

#ifndef __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
#define __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__

#include "config.hpp"
#include "calculator.hpp"

#include <cassert>

namespace detail
{

void append_server_tcp(svr_ptr_type psvr, yggr::u32 port);
void append_server_udp(svr_ptr_type psvr, yggr::u32 port);

} //namespace detail

void init_heart_timer(timer_mgr_type& timer_mgr, svr_ptr_type psvr);
void stop_heart_timer(timer_mgr_type& timer_mgr);

void append_server(svr_ptr_type psvr, yggr::u32 port);
void run_server(svr_ptr_type& psvr);
void stop_server(svr_ptr_type& psvr, ::yggr::shared_ptr<proxy_msg_calculator> pcal);
void uninstall_server(void);

#endif // __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
