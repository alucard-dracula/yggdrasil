//handler_reg.hpp

#ifndef __SVR_TEST_SERVICE_HANDLER_SARED_WRAP_TEST_HANDLER_REG_HPP__
#define __SVR_TEST_SERVICE_HANDLER_SARED_WRAP_TEST_HANDLER_REG_HPP__

#include <cassert>

#include "config.hpp"
#include "calculator.hpp"

void reg_calculator(runner_shared_info_ptr_type shared_info_ptr, 
						boost::shared_ptr<Calculator> pcal);

void unreg_calculator(runner_shared_info_ptr_type shared_info_ptr, 
						boost::shared_ptr<Calculator> pcal);

void reg_ctrl_center(ctrl_center_single_type::obj_ptr_type pctrl,
						svr_ptr_type psvr, 
						boost::shared_ptr<Calculator> pcal);

#endif // __SVR_TEST_SERVICE_HANDLER_SARED_WRAP_TEST_HANDLER_REG_HPP__
