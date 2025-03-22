// protocol_reg.cpp

#include "protocol_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_network_protocol(svr_ptr_type psvr)
{
	assert(psvr);
	bool b = true;

	//register network protocol, some proxy_system's used network data

	b = b && psvr->register_network_protocol_of_container<proxy_register_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_recv_enable);

	b = b && psvr->register_network_protocol_of_container<proxy_register_back_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_send_enable);

	b = b && psvr->register_network_protocol_of_container<proxy_mode_change_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_recv_enable);

	b = b && psvr->register_network_protocol_of_container<proxy_mode_change_back_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_send_enable);

	b = b && psvr->register_network_protocol_of_container<proxy_unregister_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_recv_enable);

	b = b && psvr->register_network_protocol_of_container<proxy_unregister_back_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_send_enable);

	assert(b);
}