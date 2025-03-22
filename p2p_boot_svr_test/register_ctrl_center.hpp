//register_ctrl_center.hpp

#ifndef __REGISTER_CTRL_CENTER_HPP__
#define __REGISTER_CTRL_CENTER_HPP__

#include "p2p_boot_server_def.hpp"
#include "p2p_boot_calculator_def.hpp"

void register_ctrl_center(ctrl_center_single_type::obj_ptr_type pctrl, 
							svr_ptr_type psvr,
							::yggr::shared_ptr<p2p_boot_calculator_type> pcal);

#endif // __REGISTER_CTRL_CENTER_HPP__