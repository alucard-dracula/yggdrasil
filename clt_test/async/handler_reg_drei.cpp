//static_data_center_udp_svr_handler_reg.cpp

#include "handler_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_ctrl_center_dispatchers(ctrl_center_single_type::obj_ptr_type pctrl,
									boost::shared_ptr<Calculator> pcal)
{
	assert((pctrl && pcal));
	pctrl->register_dispatchers(*pcal);
}
