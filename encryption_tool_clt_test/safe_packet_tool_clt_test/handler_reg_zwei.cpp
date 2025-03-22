// handler_reg.cpp

#include "handler_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_ctrl_center(ctrl_center_single_type::obj_ptr_type pctrl,
						clt_ptr_type pclt, 
						::yggr::shared_ptr<Calculator> pcal)
{
	reg_ctrl_center_controller(pctrl, pclt);
	reg_ctrl_center_dispatchers(pctrl, pcal);
}

void reg_ctrl_center_controller(ctrl_center_single_type::obj_ptr_type pctrl,
								clt_ptr_type pclt)
{
	assert((pctrl && pclt));
	pctrl->register_controller(*pclt);
}
