//init_server.hpp

#ifndef __STATIC_DATA_CENTER_UDP_SVR_INIT_SERVER_HPP__
#define __STATIC_DATA_CENTER_UDP_SVR_INIT_SERVER_HPP__

#include <cassert>

#include "config.hpp"

bool init_server(yggr::u32 thread_size, yggr::u16 port, task_center_type& tc, yggr::u32 service_pool_size);
void run_server(void);

#endif // __STATIC_DATA_CENTER_UDP_SVR_INIT_SERVER_HPP__
