//handler_reg.hpp

#ifndef __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_HANDLER_REG_HPP__
#define __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_HANDLER_REG_HPP__

#include "config.hpp"
#include "calculator.hpp"

#include <cassert>

void reg_calculator(proxy_runner_shared_info_ptr_type shared_info_ptr,
						::yggr::shared_ptr<proxy_msg_calculator> pcal);

void reg_ctrl_center(ctrl_center_single::obj_ptr_type pctrl,
						svr_ptr_type psvr,
						boost::shared_ptr<proxy_msg_calculator> pcal);

#endif // __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_HANDLER_REG_HPP__
