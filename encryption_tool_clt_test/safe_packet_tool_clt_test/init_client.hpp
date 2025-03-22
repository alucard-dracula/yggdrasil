//init_server.hpp

#ifndef __SAFE_PACKET_TOOL_TCP_INIT_CLIENT_HPP__
#define __SAFE_PACKET_TOOL_TCP_INIT_CLIENT_HPP__

#include <cassert>

#include "config.hpp"

bool init_client(yggr::u32 thread_size, task_center_type& tc);
void run_client(const std::string& ip, const std::string& port);

#endif // __SAFE_PACKET_TOOL_TCP_INIT_CLIENT_HPP__
