// init_client.cpp

#include "init_client.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void run_client(clt_ptr_type pclt, const std::string& ip, const std::string& port)
{
	assert(pclt);
	if(!pclt)
	{
		return;
	}

	pclt->run();

	client_connect(pclt, ip, port);

}
