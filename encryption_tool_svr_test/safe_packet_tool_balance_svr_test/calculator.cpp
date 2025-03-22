// calculator.cpp

#include "calculator.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER


void Calculator::print_socket_info(session_mgr_type::smap_type::base_type& base,
									session_mgr_type::smap_citer_type iter) const
{
	try
	{
		std::cout << iter->second->str_local_socket_info() << std::endl;
		std::cout << iter->second->str_remote_socket_info() << std::endl;
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
	}
}
