//init_server.cpp

#include <cassert>
#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void init_proxy(proxy_runner_shared_info_ptr_type pinfo)
{
	proxy_task_center_single::init_ins(pinfo, proxy_task_center_type::rst_saver_init_type());
}

udp_svr_handler_type::init_type create_server_handler_init(yggr::u32 port)
{
	return
		udp_svr_handler_type::init_type(
				port,
				*(proxy_task_center_single::get_ins()),
				udp_svr_handler_type::link_handler_init_type(6) );
}
