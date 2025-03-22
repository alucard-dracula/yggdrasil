//calculator.hpp

#ifndef __PROXY_TURN_SVR_CALCULATOR_HPP__
#define __PROXY_TURN_SVR_CALCULATOR_HPP__

#include <cassert>

#include "config.hpp"

struct proxy_msg_calculator
	: public base_proxy_msg_calculator_type
{
public:
	ERROR_MAKER_BEGIN("Turn_Svr_Calculator")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
			ERROR_CODE_DEF(E_code_test)
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_code_test, "code test")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef base_proxy_msg_calculator_type base_type;

	typedef base_type::proxy_repeater_ptr_type proxy_repeater_ptr_type;
	typedef base_type::proxy_router_ptr_type proxy_router_ptr_type;

private:

	typedef proxy_msg_calculator this_type;

private:
	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename Ctrl_Center>
	void register_dispatchers(Ctrl_Center& cc)
	{
		base_type::register_dispatchers<Tag>(cc);

		CTRL_HANDLER(Tag, cc, proxy_session_type::close_code_type::value,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_fix_network_exception,
												boost::shared_from_raw(this), _1));
	}

public:

	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		base_type::register_cal_object<Runner>(at, handler);
	}

public:
	proxy_msg_calculator(const proxy_repeater_ptr_type& prepeater, 
							const proxy_adapter_mgr_ptr_type& padp_mgr);

	~proxy_msg_calculator(void);

};

#endif // __PROXY_TURN_SVR_CALCULATOR_HPP__
