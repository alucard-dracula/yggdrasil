//register_network_protocol.cpp

#include <cassert>
#include "register_p2p_calculator.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void register_p2p_calculator(runner_shared_info_ptr_type shared_info_ptr, 
								::yggr::shared_ptr<p2p_boot_calculator_type> p2p_cal)
{
	assert((shared_info_ptr && p2p_cal));
	shared_info_ptr->register_calculator(*p2p_cal);
}

void unregister_p2p_calculator(runner_shared_info_ptr_type shared_info_ptr, 
								::yggr::shared_ptr<p2p_boot_calculator_type> p2p_cal)
{
	assert((shared_info_ptr && p2p_cal));
	shared_info_ptr->unregister_calculator(*p2p_cal);
}
