//init_server.hpp

#ifndef __PORXY_SVR_TEST_INIT_SERVER_HPP__
#define __PORXY_SVR_TEST_INIT_SERVER_HPP__

#include "config.hpp"

bool init_server(yggr::u32 trd_count);
void run_server(clt_ptr_type pclt, const std::string& ip, const std::string& port);

#endif // __PORXY_SVR_TEST_INIT_SERVER_HPP__
