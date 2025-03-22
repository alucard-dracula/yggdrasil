//register_user_calcuator.hpp

#ifndef __REGISTER_USER_CALCULATOR_HPP__
#define __REGISTER_USER_CALCULATOR_HPP__

#include "user_calculator.hpp"

void register_user_calcuator(runner_shared_info_ptr_type shared_info_ptr, 
								::yggr::shared_ptr<user_calculator> p2p_cal);

void unregister_user_calcuator(runner_shared_info_ptr_type shared_info_ptr, 
								::yggr::shared_ptr<user_calculator> p2p_cal);

#endif // __REGISTER_USER_CALCULATOR_HPP__
