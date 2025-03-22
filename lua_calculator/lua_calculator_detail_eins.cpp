// lua_calculator_detail.cpp : Defines the entry point for the DLL application.

#include "lua_calculator_detail.hpp"

#if defined(_MSC_VER)
#   include <vld.h>
#endif // defined(_MSC_VER) 

//void Calculator::print_socket_info(const session_mgr_type::smap_type::base_type& base,
//									session_mgr_type::smap_citer_type iter) const
//{
//	try
//	{
//		std::cout << iter->second->str_local_socket_info() << std::endl;
//		std::cout << iter->second->str_remote_socket_info() << std::endl;
//	}
//	catch(const compatibility::stl_exception& e)
//	{
//		std::cerr << e.what() << std::endl;
//	}
//	catch(const boost::system::error_code& e)
//	{
//		std::cerr << e.message() << std::endl;
//	}
//}

void Calculator::cal_network_dispath(const yggr::u64& owner)
{
	_count.grab();
	std::cout << "10054 client id = [" << owner << "] is removed" << std::endl;
	_count.drop();
}

void Calculator::join(void)
{
	for(;!_count.empty(););
}

void register_calculator(runner_shared_info_ptr_type ptr)
{
	if(!ptr)
	{
		return;
	}

	ptr->register_calculator(*cal_sig_type::get_ins());
}
