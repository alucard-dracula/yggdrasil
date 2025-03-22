// client_connect.cpp

#include "handler_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void client_connect(clt_ptr_type pclt, const std::string& ip, const std::string& port)
{
	assert(pclt);
	if(pclt)
	{
		pclt->op_handler().connect(ip, port);
	}
}