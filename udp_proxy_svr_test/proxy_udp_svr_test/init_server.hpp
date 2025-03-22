//init_server.hpp

#ifndef __STATIC_DATA_CENTER_UDP_SVR_INIT_SERVER_HPP__
#define __STATIC_DATA_CENTER_UDP_SVR_INIT_SERVER_HPP__

#include <cassert>

#include "config.hpp"

void run_client(clt_ptr_single_type::obj_ptr_type pclt,
				const std::string& ip, const std::string& port);

void client_connect(clt_ptr_single_type::obj_ptr_type pclt,
						const std::string& ip, const std::string& port);

#endif // __STATIC_DATA_CENTER_UDP_SVR_INIT_SERVER_HPP__
