// protocol_reg.cpp

#include "protocol_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_network_protocol(clt_ptr_type pclt)
{
	typedef task_type::data_info_type data_info_type;

	assert(pclt);

#if 0
	data_info_type info(test_pak_type::data_info());
#else
	data_info_type info(test_pak_type::data_id(), test_pak_type::data_ver());
#endif // 0, 1

	pclt->register_network_protocol<task_type>(info, info, yggr::network::network_direct_def_type::E_recv_enable);

}
