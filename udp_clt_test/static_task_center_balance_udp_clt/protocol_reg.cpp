// static_data_center_udp_svr_protocol_reg.cpp

#include "protocol_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_network_protocol(clt_ptr_type pclt)
{
	assert(pclt);
	pclt->register_network_protocol<test_pak_type>();
}