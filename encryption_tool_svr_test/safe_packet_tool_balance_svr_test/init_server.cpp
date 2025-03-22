//init_server.cpp

#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

bool init_server(yggr::u32 thread_size, yggr::u16 port, task_center_type& tc)
{
	try
	{
		return 
			!!svr_ptr_single_type::init_ins(
				thread_size,
				tcp_svr_handler_type::init_type(port, tc, tcp_svr_handler_type::link_handler_init_type()));
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
		return false;
	}
}

void run_server(svr_ptr_type psvr)
{
	assert(psvr);
	psvr->run();
}
