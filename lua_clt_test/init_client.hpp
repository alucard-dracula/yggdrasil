//init_server.hpp

#ifndef __LUA_CLT_INIT_CLIENT_HPP__
#define __LUA_CLT_INIT_CLIENT_HPP__

#include "config.hpp"

namespace detail
{

void run_client(clt_ptr_type pclt);
void connect_client(clt_ptr_type pclt, const std::string& ip, const std::string& port);

}// namespace detail

void init_client(yggr::u32 trd_count);
void run_client(clt_ptr_type pclt, const std::string& ip, const std::string& port);

void stop_client(clt_ptr_type pclt);

void load_lua(void);
void init_log(void);

#endif // __LUA_CLT_INIT_CLIENT_HPP__
