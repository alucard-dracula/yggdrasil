//handler_reg.cpp

#include <cassert>
#include "handler_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_ctrl_center(ctrl_center_single_type::obj_ptr_type pctrl,
						svr_ptr_type psvr)
{
	assert((pctrl && psvr));
	pctrl->register_controller(*psvr); 
}
