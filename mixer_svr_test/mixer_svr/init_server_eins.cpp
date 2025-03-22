//init_server.cpp

#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace detail
{

void append_server_tcp(svr_ptr_type psvr, yggr::u32 port)
{
	assert(psvr);
	psvr->append<tcp_svr_handler_type>(tcp_svr_handler_type::link_handler_init_type(), port);
}

} //namespace detail


void append_server(svr_ptr_type psvr, yggr::u32 port)
{
	assert(psvr);
	detail::append_server_tcp(psvr, port);
	detail::append_server_udp(psvr, port);
}
