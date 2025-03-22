//init_server.cpp

#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void run_client(clt_ptr_single_type::obj_ptr_type pclt,
				const std::string& ip, const std::string& port)
{
	assert(pclt);
	pclt->run();
	client_connect(pclt, ip, port);
}
