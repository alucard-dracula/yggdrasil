//init_server.cpp

#include "init_client.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

bool init_client(yggr::u32 thread_size, task_center_type& tc)
{
	try
	{
		return
			!!clt_ptr_single_type::init_ins(
				thread_size,
				tcp_clt_handler_type::init_type(tc, tcp_clt_handler_type::link_handler_init_type()));
	}
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
		return false;
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
}

void run_client(const std::string& ip, const std::string& port)
{
	clt_ptr_type pclt = clt_ptr_single_type::get_ins();
	if(pclt)
	{
		pclt->run();
		pclt->op_handler().connect(ip, port);
	}
}
