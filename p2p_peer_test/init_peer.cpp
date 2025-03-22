// init_peer.cpp

#include "init_peer.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void init_peer(yggr::u32 trd_count)
{
	clt_ptr_single_type::init_ins(
		trd_count,
		p2p_peer_handler_type::init_type(
			*(task_center_single_type::get_ins()),
			p2p_peer_handler_type::link_handler_init_type(trd_count)));
}

void run_peer(clt_ptr_type pclt, const std::string& ip, const std::string& port)
{
	assert(pclt);
	if(!pclt)
	{
		return;
	}

	pclt->run();
	pclt->op_handler().connect(ip, port);
}
