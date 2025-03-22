// init_client.cpp

#include "init_client.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

bool init_client(yggr::u32 thread_size, task_center_type& tc, yggr::u32 service_pool_size)
{
	try
	{
		return
			!!clt_ptr_single_type::init_ins(
				thread_size,
				udp_clt_handler_type::init_type(
					tc,
					udp_clt_handler_type::link_handler_init_type(service_pool_size)));
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

