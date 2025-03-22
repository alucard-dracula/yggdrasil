//register_network_protocol.cpp


#include "register_network_protocol.hpp"
#include "p2p_boot_calculator_def.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void register_network_protocol(svr_ptr_type psvr)
{
	typedef p2p_boot_calculator_type::net_p2p_boot_request_type net_p2p_boot_request_type;
	typedef p2p_boot_calculator_type::net_p2p_boot_request_back_type net_p2p_boot_request_back_type;
	
	if(!psvr)
	{
		return;
	}

	psvr->register_network_protocol<test_pak_type>(yggr::network::network_direct_def_type::E_send_enable);
	psvr->register_network_protocol<net_p2p_boot_request_back_type>(yggr::network::network_direct_def_type::E_send_enable);

	psvr->register_network_protocol<net_p2p_boot_request_type>(yggr::network::network_direct_def_type::E_recv_enable);
}