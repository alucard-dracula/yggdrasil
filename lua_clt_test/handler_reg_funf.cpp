// handler_reg.cpp

#include "handler_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace detail
{

void reg_ctrl_center_ctrl(ctrl_center_single_type::obj_ptr_type pctrl, clt_ptr_type pclt)
{
	assert((pctrl && pclt));
	pctrl->register_controller(*pclt);
}

} // namespace detail
