// init_server.hpp

#ifndef __INIT_SERVER_HPP__
#define __INIT_SERVER_HPP__

#include "p2p_boot_server_def.hpp"

bool init_server(yggr::u32 thread_count, yggr::u16 port, yggr::u32 pool_size);

#endif // __INIT_SERVER_HPP__