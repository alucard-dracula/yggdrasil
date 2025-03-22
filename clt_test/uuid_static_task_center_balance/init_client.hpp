//init_server.hpp

#ifndef __CLT_TEST_UUID_STATIC_TASK_CENTER_BLANCE_INIT_CLIENT_HPP__
#define __CLT_TEST_UUID_STATIC_TASK_CENTER_BLANCE_INIT_CLIENT_HPP__

#include "config.hpp"

void init_client(yggr::u32 trd_count);
void run_client(clt_ptr_type pclt, const std::string& ip, const std::string& port);

#endif // __CLT_TEST_UUID_STATIC_TASK_CENTER_BLANCE_INIT_CLIENT_HPP__
