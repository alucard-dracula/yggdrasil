// init_peer.hpp

#ifndef __INIT_PEER_HPP__
#define __INIT_PEER_HPP__

#include "p2p_peer_def.hpp"

void init_peer(yggr::u32 trd_count);

void run_peer(clt_ptr_type pclt, const std::string& ip, const std::string& port);


#endif // __INIT_PEER_HPP__