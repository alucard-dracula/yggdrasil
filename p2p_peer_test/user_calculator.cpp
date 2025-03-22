//user_calculator.cpp

#include "user_calculator.hpp"

void user_calculator::prv_handler_get_localhost_socket_info(const session_mgr_type::smap_type::base_type& base,
															session_mgr_type::smap_citer_type iter,
															socket_info_type& socket_info) const
{
	try
	{
		socket_info_type tmp(iter->second->localhost_socket_info<socket_info_type>());
		yggr::swap(socket_info, tmp);
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

void user_calculator::prv_handler_get_remote_socket_info(const session_mgr_type::smap_type::base_type& base,
															session_mgr_type::smap_citer_type iter,
															socket_info_type& socket_info) const
{
	try
	{
		socket_info_type tmp(iter->second->remote_socket_info<socket_info_type>());
		yggr::swap(socket_info, tmp);
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

void user_calculator::print_socket_info(const session_mgr_type::smap_type::base_type& base,
											session_mgr_type::smap_citer_type iter) const
{
	try
	{
		std::cout << "local_socket " << iter->second->str_local_socket_info() << std::endl;
		std::cout << "localhost_socket " << iter->second->str_localhost_socket_info() << std::endl;
		std::cout << "remote_socket " << iter->second->str_remote_socket_info() << std::endl;
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

