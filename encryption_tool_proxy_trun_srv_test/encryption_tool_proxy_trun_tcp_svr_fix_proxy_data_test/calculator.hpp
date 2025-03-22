//calculator.hpp

#ifndef __SAFE_PACKET_TOOL_TCP_SVR_CALCULATOR_HPP__
#define __SAFE_PACKET_TOOL_TCP_SVR_CALCULATOR_HPP__

#include "config.hpp"
//#include <yggr/time/time.hpp>

#include <cassert>

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

	template<typename Tag, typename CtrlCenter>
	void register_dispatchers(CtrlCenter& cc)
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
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

		base_type::register_cal_object<Runner>(at, handler);


		base_type::template register_repeater_special_data<test_pak_type>(
			boost::bind(&this_type::cal_test_pak_type<owner_info_type,
														runner_type,
														recv_handler_type>,
						boost::shared_from_raw(this), _1, _2, _3, _4));
	}

public:
	//proxy_msg_calculator(const proxy_repeater_ptr_type& prepeater,
	//						const proxy_adapter_mgr_ptr_type& padp_mgr)
	//	: base_type(prepeater, padp_mgr)
	//{
	//}

	//~proxy_msg_calculator(void)
	//{
	//}

	proxy_msg_calculator(const proxy_repeater_ptr_type& prepeater,
							const proxy_adapter_mgr_ptr_type& padp_mgr);

	~proxy_msg_calculator(void);

	// proxy turn server fix proxy data
	template<typename Owner, typename Runner, typename Handler>
	void cal_test_pak_type(const Owner& owner,
										const net_ipak_type& cdt,
										Runner* prunner,
										const Handler& handler)
	{
		std::cout << "calculate at proxy" << std::endl;

		test_pak_type data;

		bool rst = cdt.load(data);
		if(!rst)
		{
			std::cout << "calculate at proxy load failed" << std::endl;
			return;
		}

		base_type::route(owner, cdt, prunner, handler, test_pak_type::cal_type());
	}
};

#endif // __SAFE_PACKET_TOOL_TCP_SVR_CALCULATOR_HPP__
