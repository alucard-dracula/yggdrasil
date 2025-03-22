//static_data_center_udp_svr_handler_reg.cpp

#include "handler_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_calculator(runner_shared_info_ptr_type shared_info_ptr, 
						::yggr::shared_ptr<Calculator> pcal)
{
	assert((shared_info_ptr && pcal));
	shared_info_ptr->register_calculator(*pcal);
}

void unreg_calculator(runner_shared_info_ptr_type shared_info_ptr, 
						::yggr::shared_ptr<Calculator> pcal)
{
	assert((shared_info_ptr && pcal));
	shared_info_ptr->unregister_calculator(*pcal);
}

void reg_ctrl_center(ctrl_center_single_type::obj_ptr_type pctrl,
						clt_ptr_type pclt, 
						::yggr::shared_ptr<Calculator> pcal)
{
	assert((pctrl && pclt && pcal));
	pctrl->register_controller(*pclt);
	pctrl->register_dispatchers(*pcal);
}
