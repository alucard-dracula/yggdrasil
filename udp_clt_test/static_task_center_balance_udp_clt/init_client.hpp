// init_client.hpp

#ifndef __STATIC_DATA_CENTER_UDP_CLT_INIT_CLIENT_HPP__
#define __STATIC_DATA_CENTER_UDP_CLT_INIT_CLIENT_HPP__

#include "config.hpp"

bool init_client(yggr::u32 thread_size, task_center_type& tc, yggr::u32 service_pool_size);

void run_client(clt_ptr_type pclt, const std::string& ip, const std::string& port);

void client_connect(clt_ptr_type pclt, const std::string& ip, const std::string& port);

#endif // __STATIC_DATA_CENTER_UDP_CLT_INIT_CLIENT_HPP__
