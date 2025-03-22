//calculator.cpp

#include "calculator.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void Calculator::handler_send(const boost::system::error_code& e) const
{
	if(!e)
	{
		std::cout << "send_packet_sucess" << std::endl;
	}
	else
	{
		std::cout << "send_packet_fail" << std::endl;
	}
}

void Calculator::cal_network_dispath_non_owner(void)
{
	std::cout << "cal_network_dispath_non_owner" << std::endl;
}
