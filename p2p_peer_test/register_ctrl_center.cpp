//register_ctrl_center.cpp

#include <cassert>

#include "register_ctrl_center.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void register_ctrl_center(ctrl_center_single_type::obj_ptr_type pctrl, 
							clt_ptr_type pclt)
{
	assert(pctrl && pclt);
	pctrl->register_controller(*pclt);
}
