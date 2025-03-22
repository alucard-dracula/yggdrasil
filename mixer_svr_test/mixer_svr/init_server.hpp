//init_server.hpp

#ifndef __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
#define __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__

#include <cassert>

#include "config.hpp"

namespace detail
{

void append_server_tcp(svr_ptr_type psvr, yggr::u32 port);
void append_server_udp(svr_ptr_type psvr, yggr::u32 port);

} //namespace detail

void append_server(svr_ptr_type psvr, yggr::u32 port);
void run_server(svr_ptr_type psvr); // bigobj only

#endif // __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
