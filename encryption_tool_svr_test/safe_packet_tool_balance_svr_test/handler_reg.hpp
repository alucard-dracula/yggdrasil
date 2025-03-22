//handler_reg.hpp

#ifndef __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_HANDLER_REG_HPP__
#define __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_HANDLER_REG_HPP__

#include <cassert>

#include "config.hpp"
#include "calculator.hpp"

void reg_calculator(runner_shared_info_ptr_type shared_info_ptr, 
						::yggr::shared_ptr<Calculator> pcal);

void unreg_calculator(runner_shared_info_ptr_type shared_info_ptr, 
						::yggr::shared_ptr<Calculator> pcal);

void reg_ctrl_center(ctrl_center_single_type::obj_ptr_type pctrl,
						svr_ptr_type psvr, 
						::yggr::shared_ptr<Calculator> pcal);

#endif // __SAFE_PACKET_TOOL_BALANCE_TCP_SVR_HANDLER_REG_HPP__
