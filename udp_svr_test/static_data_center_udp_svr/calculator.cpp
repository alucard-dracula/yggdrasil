//static_data_center_udp_svr_calculator.hpp

#include "calculator.hpp"


bool Calculator::print_socket_info(const session_mgr_type::smap_type::base_type& base,
						session_mgr_type::smap_citer_type iter) const
{
	if(iter == base.end())
	{
		return false;
	}

	try
	{
		std::cout << iter->second->str_local_address() << std::endl;
		std::cout << iter->second->str_local_port() << std::endl;

		std::cout << iter->second->str_local_socket_info() << std::endl;
		std::cout << iter->second->str_localhost_socket_info() << std::endl;
		std::cout << iter->second->str_remote_socket_info() << std::endl;
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

	return true;
}

	