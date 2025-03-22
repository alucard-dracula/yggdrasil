//init_server.hpp

#ifndef __CLIENT_TEST_ASYNC_INIT_CLIENT_HPP__
#define __CLIENT_TEST_ASYNC_INIT_CLIENT_HPP__

#include <cassert>

#include "config.hpp"

bool init_client(yggr::u32 thread_size, task_center_type& tc);
void run_client(const std::string& ip, const std::string& port);

#endif // __CLIENT_TEST_ASYNC_INIT_CLIENT_HPP__
