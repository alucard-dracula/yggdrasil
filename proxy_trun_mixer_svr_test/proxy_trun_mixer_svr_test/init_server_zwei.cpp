//init_server.cpp

#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace detail
{

void append_server_udp(svr_ptr_type psvr, yggr::u32 port)
{
	assert(psvr);
	psvr->append<udp_svr_handler_type>(udp_svr_handler_type::link_handler_init_type(6), port);
}

} // namespace detail
