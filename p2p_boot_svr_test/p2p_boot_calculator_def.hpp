//p2p_boot_calcuator_def,hpp

#ifndef __P2P_BOOT_CALCUATOR_DEF_HPP__
#define __P2P_BOOT_CALCUATOR_DEF_HPP__

#include "p2p_boot_server_def.hpp"

#include <yggr/p2p/protocol/p2p_peer_boot_info.hpp>
#include <yggr/p2p/protocol/p2p_boot_request.hpp>
#include <yggr/p2p/protocol/p2p_boot_request_back.hpp>

#include <yggr/p2p/calculator/p2p_boot_calculator.hpp>

#include <yggr/p2p/p2p_boot_runtime_info_mgr.hpp>
#include <yggr/p2p/p2p_peer_session_id_generator.hpp>

#include <yggr/serialization/list.hpp>
#include <yggr/serialization/string.hpp>

typedef yggr::p2p::p2p_peer_boot_info<owner_type::owner_id_type, socket_info_type> p2p_peer_boot_info_type;
typedef yggr::list<p2p_peer_boot_info_type> p2p_peer_info_container_type;

typedef yggr::p2p::p2p_boot_request<socket_info_type> p2p_boot_request_type;
typedef yggr::p2p::p2p_boot_request_back<p2p_peer_info_container_type> p2p_boot_request_back_type;

typedef yggr::p2p::p2p_boot_runtime_info_mgr
		<
			owner_info_container_type, 
			p2p_boot_request_type, 
			p2p_boot_request_back_type,
			yggr::p2p::p2p_peer_session_id_generator
			<
				owner_type::owner_id_type,
				yggr::ids::id_generator<owner_type::owner_id_type>
			>
		> p2p_boot_runtime_info_mgr_type;

typedef yggr::p2p::p2p_boot_calcuator
		<
			svr_ptr_single_type,
			session_mgr_type,
			owner_info_container_type,
			p2p_boot_request_type,
			p2p_boot_request_back_type,
			p2p_boot_runtime_info_mgr_type
		> p2p_boot_calculator_type;

#endif // __P2P_BOOT_CALCUATOR_HPP__
