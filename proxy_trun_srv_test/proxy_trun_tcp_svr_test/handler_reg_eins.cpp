//handler_reg.cpp

#include "handler_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_calculator(proxy_runner_shared_info_ptr_type shared_info_ptr, 
						::yggr::shared_ptr<proxy_msg_calculator> pcal)
{
	assert((shared_info_ptr && pcal));
	shared_info_ptr->register_calculator(*pcal);
}
