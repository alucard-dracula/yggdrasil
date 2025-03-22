//register_network_protocol.hpp

#include <cassert>

#include "register_network_protocol.hpp"
#include "p2p_peer_calculator_def.hpp"
#include "user_calculator.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void register_network_protocol(clt_ptr_type pclt)
{
	typedef user_calculator::net_p2p_boot_request_type net_p2p_boot_request_type;
	typedef p2p_peer_calculator_type::net_p2p_boot_request_back_type net_p2p_boot_request_back_type;
	typedef p2p_peer_calculator_type::net_p2p_boot_start_type net_p2p_boot_start_type;
	typedef p2p_peer_calculator_type::net_p2p_nat_reduce_type net_p2p_nat_reduce_type;


	if(!pclt)
	{
		return;
	}


	pclt->register_network_protocol<net_p2p_boot_request_type>(yggr::network::network_direct_def_type::E_send_enable);
	
	pclt->register_network_protocol<test_pak_type>(yggr::network::network_direct_def_type::E_recv_enable);
	pclt->register_network_protocol<net_p2p_boot_request_back_type>(yggr::network::network_direct_def_type::E_recv_enable);

	pclt->register_network_protocol<net_p2p_boot_start_type>(/*yggr::network::network_direct_def_type::E_both_enable*/);
	pclt->register_network_protocol<net_p2p_nat_reduce_type>(/*yggr::network::network_direct_def_type::E_both_enable*/);
}
