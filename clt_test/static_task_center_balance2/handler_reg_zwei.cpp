//static_data_center_udp_svr_handler_reg.cpp

#include "handler_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void unreg_calculator(runner_shared_info_ptr_type shared_info_ptr, 
						boost::shared_ptr<Calculator> pcal)
{
	assert((shared_info_ptr && pcal));
	shared_info_ptr->unregister_calculator(*pcal);
}
