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
	pctrl->register_controller(*psvr); // ע������������Ʋ����ڷ������
	pctrl->register_dispatchers(*pcal); // ע��������������ʾ��������Ҳ�ж��쳣�Ĵ���
}
