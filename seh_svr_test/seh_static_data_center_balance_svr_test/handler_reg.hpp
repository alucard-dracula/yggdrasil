//handler_reg.hpp

#ifndef __UUID_STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_SVR_HANDLER_REG_HPP__
#define __UUID_STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_SVR_HANDLER_REG_HPP__

#include "config.hpp"

#if defined(YGGR_USE_SEH)

#include "calculator.hpp"
#include <cassert>

void reg_calculator(runner_shared_info_ptr_type shared_info_ptr, 
						boost::shared_ptr<Calculator> pcal);

void reg_ctrl_center(ctrl_center_single::obj_ptr_type pctrl,
						svr_ptr_type psvr, 
						boost::shared_ptr<Calculator> pcal);

#endif // #if defined(YGGR_USE_SEH)

#endif // __UUID_STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_SVR_HANDLER_REG_HPP__
