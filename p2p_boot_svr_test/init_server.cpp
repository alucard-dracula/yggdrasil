// init_server.cpp

#include "init_server.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

bool init_server(yggr::u32 thread_count, yggr::u16 port, yggr::u32 pool_size)
{
	try
	{
		return
			!!svr_ptr_single_type::init_ins(
				thread_count,
				udp_svr_handler_type::init_type(
					port,
					*(task_center_single_type::get_ins()),
					udp_svr_handler_type::link_handler_init_type(pool_size)));
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
