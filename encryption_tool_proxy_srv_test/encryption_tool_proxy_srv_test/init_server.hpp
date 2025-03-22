//init_server.hpp

#ifndef __STATIC_DATA_CENTER_UDP_SVR_INIT_SERVER_HPP__
#define __STATIC_DATA_CENTER_UDP_SVR_INIT_SERVER_HPP__

#include <cassert>

#include "config.hpp"

bool init_server(yggr::u32 thread_size, task_center_type& tc);
void run_server(const std::string& ip, const std::string& port);

#endif // __STATIC_DATA_CENTER_UDP_SVR_INIT_SERVER_HPP__
