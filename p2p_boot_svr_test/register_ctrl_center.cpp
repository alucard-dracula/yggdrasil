//register_ctrl_center.cpp

#include <cassert>
#include "register_ctrl_center.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void register_ctrl_center(ctrl_center_single_type::obj_ptr_type pctrl, 
							svr_ptr_type psvr,
							::yggr::shared_ptr<p2p_boot_calculator_type> pcal)
{
	assert(pctrl && psvr && pcal);
	pctrl->register_controller(*psvr);
	pctrl->register_dispatchers(*pcal);
}