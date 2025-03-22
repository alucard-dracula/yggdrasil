//register_user_calcuator.cpp

#include "register_user_calculator.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void register_user_calcuator(runner_shared_info_ptr_type shared_info_ptr, 
								::yggr::shared_ptr<user_calculator> p2p_cal)
{
	assert((shared_info_ptr && p2p_cal));
	shared_info_ptr->register_calculator(*p2p_cal);
}

void unregister_user_calcuator(runner_shared_info_ptr_type shared_info_ptr, 
								::yggr::shared_ptr<user_calculator> p2p_cal)
{
	assert((shared_info_ptr && p2p_cal));
	shared_info_ptr->unregister_calculator(*p2p_cal);
}
