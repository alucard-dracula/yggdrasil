//init_server.cpp

#include <cassert>
#include "init_client.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void init_client(yggr::u32 trd_count)
{
	clt_ptr_single_type::init_ins(
		trd_count,
		tcp_clt_handler_type::init_type(
			*(task_center_single_type::get_ins()),
			tcp_clt_handler_type::link_handler_init_type()));
}

void run_client(clt_ptr_type pclt, const std::string& ip, const std::string& port)
{
	assert(pclt);
	pclt->run();
	pclt->op_handler().connect(ip, port);
}

