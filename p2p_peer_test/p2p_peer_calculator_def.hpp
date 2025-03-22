//p2p_peer_calculator_def.hpp

#ifndef __P2P_PEER_CALCULATOR_DEF_HPP__
#define __P2P_PEER_CALCULATOR_DEF_HPP__

#include "p2p_peer_def.hpp"

#include <yggr/p2p/protocol/p2p_peer_boot_info.hpp>
#include <yggr/p2p/protocol/p2p_boot_request.hpp>
#include <yggr/p2p/protocol/p2p_boot_request_back.hpp>
#include <yggr/p2p/protocol/p2p_peer_start.hpp>

#include <yggr/p2p/p2p_peer_runtime_info_mgr.hpp>
#include <yggr/p2p/calculator/p2p_peer_calculator.hpp>

#include <yggr/serialization/list.hpp>
#include <yggr/serialization/string.hpp>

typedef yggr::p2p::p2p_peer_boot_info<owner_type::owner_id_type, socket_info_type> p2p_peer_boot_info_type;
typedef yggr::list<p2p_peer_boot_info_type> p2p_peer_info_container_type;

typedef yggr::p2p::p2p_boot_request<socket_info_type> p2p_boot_request_type;
typedef yggr::p2p::p2p_boot_request_back<p2p_peer_info_container_type> p2p_boot_request_back_type;
typedef yggr::p2p::p2p_peer_start<yggr::u32> p2p_peer_start_type;


typedef 
	yggr::p2p::p2p_peer_runtime_info_mgr
	<
		owner_type,
		conn_type,
		p2p_boot_request_back_type,
		p2p_peer_start_type,
		packets_checker_type,
		packets_crypher_type
	> p2p_peer_runtime_info_mgr_type;

typedef 
	yggr::p2p::p2p_peer_calculator
	<
		clt_ptr_single_type,
		owner_info_container_type,
		p2p_boot_request_back_type,
		p2p_peer_start_type,
		p2p_peer_runtime_info_mgr_type
	> p2p_peer_calculator_type;


#endif // __P2P_PEER_CALCULATOR_DEF_HPP__
