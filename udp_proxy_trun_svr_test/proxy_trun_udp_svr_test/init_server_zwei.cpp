//init_server.cpp

#include <cassert>
#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

bool init_server(yggr::u32 trd_count, yggr::u32 port)
{
	try
	{
		//svr_ptr_single_type::init_ins(
		//	trd_count,
		//	udp_svr_handler_type::init_type(
		//		port,
		//		*(proxy_task_center_single::get_ins()),
		//		udp_svr_handler_type::link_handler_init_type(6) ) );

		return
			!!svr_ptr_single_type::init_ins(
				trd_count,
				create_server_handler_init(port) );
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
