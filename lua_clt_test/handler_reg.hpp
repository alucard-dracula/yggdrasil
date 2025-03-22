//handler_reg.hpp

#ifndef __LUA_CLT_HANDLER_REG_HPP__
#define __LUA_CLT_HANDLER_REG_HPP__

#include <cassert>

#include "config.hpp"
#include "calculator.hpp"

void reg_calculator(runner_shared_info_ptr_type shared_info_ptr, 
						::yggr::shared_ptr<Calculator> pcal);

void unreg_calculator(runner_shared_info_ptr_type shared_info_ptr, 
						::yggr::shared_ptr<Calculator> pcal);

void reg_ctrl_center(ctrl_center_single_type::obj_ptr_type pctrl,
						clt_ptr_type pclt, 
						::yggr::shared_ptr<Calculator> pcal);

namespace detail
{

void reg_ctrl_center_ctrl(ctrl_center_single_type::obj_ptr_type pctrl,
							clt_ptr_type pclt);
void reg_ctrl_center_dis(ctrl_center_single_type::obj_ptr_type pctrl,
							::yggr::shared_ptr<Calculator> pcal);
} // namespace detail

#endif // __LUA_CLT_HANDLER_REG_HPP__