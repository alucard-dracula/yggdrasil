// static_data_center_udp_svr_protocol_reg.cpp

#include "protocol_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_network_protocol(clt_ptr_type pclt)
{
	assert(pclt);
	
	pclt->register_network_protocol<test_pak_type>();
	pclt->register_network_protocol<proxy_register_msg_type>(yggr::network::network_direct_def_type::E_send_enable);
	pclt->register_network_protocol<proxy_register_back_msg_type>(yggr::network::network_direct_def_type::E_recv_enable);
	pclt->register_network_protocol<proxy_mode_change_msg_type>(yggr::network::network_direct_def_type::E_send_enable);
	pclt->register_network_protocol<proxy_mode_change_back_msg_type>(yggr::network::network_direct_def_type::E_recv_enable);
	pclt->register_network_protocol<proxy_unregister_msg_type>(yggr::network::network_direct_def_type::E_send_enable);
	pclt->register_network_protocol<proxy_unregister_back_msg_type>(yggr::network::network_direct_def_type::E_recv_enable);
}