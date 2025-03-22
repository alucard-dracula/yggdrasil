//init_server.hpp

#ifndef __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
#define __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__

#include "config.hpp"

#if defined(YGGR_USE_SEH)

#include <cassert>

void run_server(svr_ptr_type psvr);

#endif // #if defined(YGGR_USE_SEH)

#endif // __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_INIT_SERVER_HPP__
