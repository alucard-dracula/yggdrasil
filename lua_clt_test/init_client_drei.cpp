//init_server.cpp

#include <cassert>
#include "init_client.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER


namespace detail
{

void connect_client(clt_ptr_type pclt, const std::string& ip, const std::string& port)
{
    assert(pclt);
	pclt->op_handler().connect(ip, port);
}

}// namespace detail
