// calculator.cpp

#include "calculator.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER


proxy_msg_calculator::proxy_msg_calculator(const proxy_repeater_ptr_type& prepeater,
							const proxy_adapter_mgr_ptr_type& padp_mgr)
	: base_type(prepeater, padp_mgr)
{
}

proxy_msg_calculator::~proxy_msg_calculator(void)
{
}