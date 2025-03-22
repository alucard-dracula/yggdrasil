//register_p2p_calcuator.hpp

#ifndef __REGISTER_P2P_CALCULATOR_HPP__
#define __REGISTER_P2P_CALCULATOR_HPP__

#include "p2p_peer_calculator_def.hpp"

void register_p2p_calculator(runner_shared_info_ptr_type shared_info_ptr, 
								::yggr::shared_ptr<p2p_peer_calculator_type> p2p_cal);

void unregister_p2p_calculator(runner_shared_info_ptr_type shared_info_ptr, 
								::yggr::shared_ptr<p2p_peer_calculator_type> p2p_cal);

#endif // __REGISTER_P2P_CALCULATOR_HPP__
