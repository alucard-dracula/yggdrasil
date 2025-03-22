// calculator.cpp

#include "calculator.hpp"

#if defined(YGGR_USE_SEH)

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

int Calculator::test_seh_generator(int* p) const
{
	*p = 100;
	return *p;
}

//  ‰≥ˆSOCKET–≈œ¢
void Calculator::print_socket_info(const session_mgr_type::smap_type::base_type& base,
									session_mgr_type::smap_citer_type iter) const
{
	try
	{
		std::cout << iter->second->str_local_socket_info() << std::endl;
		std::cout << iter->second->str_remote_socket_info() << std::endl;
	}
	catch(const boost::system::system_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

#endif // #if defined(YGGR_USE_SEH)


