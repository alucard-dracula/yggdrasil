// init_client.cpp

#include "init_client.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void client_connect(clt_ptr_type pclt, const std::string& ip, const std::string& port)
{
	assert(pclt);
	if(!pclt)
	{
		return;
	}

#if YGGR_TEST_PP_USING_TIMEOUT()
	pclt->op_handler().connect(ip, port, boost::posix_time::seconds(1));
#else
	pclt->op_handler().connect(ip, port);
#endif // YGGR_TEST_PP_USING_TIMEOUT
}
