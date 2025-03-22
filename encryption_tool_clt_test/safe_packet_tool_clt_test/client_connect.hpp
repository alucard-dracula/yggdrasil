//handler_reg.hpp

#ifndef __SAFE_PACKET_TOOL_TCP_CLT_CLIENT_CONNECT_HPP__
#define __SAFE_PACKET_TOOL_TCP_CLT_CLIENT_CONNECT_HPP__

#include <cassert>

#include "config.hpp"

void client_connect(clt_ptr_type pclt, const std::string& ip, const std::string& port);

#endif // __SAFE_PACKET_TOOL_TCP_CLT_CLIENT_CONNECT_HPP__
