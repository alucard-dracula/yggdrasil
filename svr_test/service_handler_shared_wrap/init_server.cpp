//init_server.cpp

#include <cassert>
#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

bool init_server(service_handler_ptr_type psh, yggr::u32 port)
{
	try
	{
		return
			!!svr_ptr_single_type::init_ins(
				psh,
				tcp_svr_handler_type::init_type(
					port,
					*(task_center_single_type::get_ins()),
					tcp_svr_handler_type::link_handler_init_type() ) );
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
