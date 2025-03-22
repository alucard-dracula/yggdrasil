//handler_reg.cpp

#include "handler_reg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void reg_ctrl_center(ctrl_center_single::obj_ptr_type pctrl,
						svr_ptr_type psvr, 
						boost::shared_ptr<proxy_msg_calculator> pcal)
{
	assert((pctrl && psvr && pcal));
	pctrl->register_controller(*psvr); // 注册控制器（控制部分在服务器里）
	pctrl->register_dispatchers(*pcal); // 注册分派器（这里表示运算器里也有对异常的处理）
}
