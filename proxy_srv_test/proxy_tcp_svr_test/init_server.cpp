//init_server.cpp

#include <cassert>
#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

bool init_server(yggr::u32 trd_count)
{
	try
	{
		return
			!!clt_ptr_single_type::init_ins(
				trd_count,
				tcp_clt_handler_type::init_type(
					*(task_center_single::get_ins()),
					tcp_clt_handler_type::link_handler_init_type()));
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

void run_server(clt_ptr_type pclt, const std::string& ip, const std::string& port)
{
	assert(pclt);
	pclt->run();
	pclt->op_handler().connect(ip, port);
}

