// calculator.hpp

#ifndef __LUA_CLT_CALCULATOR_HPP__
#define __LUA_CLT_CALCULATOR_HPP__

#include "config.hpp"

struct Calculator 
	: public boost::enable_shared_from_this<Calculator>
{
private:
	typedef Calculator this_type;
public:
	ERROR_MAKER_BEGIN("LUA_Clt_Calculator")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
			ERROR_CODE_DEF(E_code_test)
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_code_test, "code test")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef network_info_type owner_info_type;

	typedef boost::unordered_multiset<owner_info_type> owner_info_container_type;

	template<typename Tag, typename CtrlCenter>
	void register_dispatchers(CtrlCenter& cc) // 注册网络异常的处理
	{

		// this is fake fixer
		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_refused,
								yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM,
								boost::bind(&this_type::cal_network_dispath,
												this_type::shared_from_this()));
	}

	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

		at.template register_calculator<test_pak_type, Recv_Handler>(
				handler, boost::bind(&this_type::cal_test_pak_type<runner_type,
																	recv_handler_type>,
										this_type::shared_from_this(), _1, _2, _3, _4));

		at.template register_calculator<rst_procotol_type, Recv_Handler>(
				handler, boost::bind(&this_type::cal_rst_procotol_type<runner_type,
																		recv_handler_type>,
										this_type::shared_from_this(), _1, _2, _3, _4));

	}

	template<typename Action_Table>
	void unregister_cal_object(Action_Table& at)
	{
		at.template unregister_calculator<test_pak_type>();
		at.template unregister_calculator<rst_procotol_type>();
	}


	template<typename Runner, typename Handler>
	void cal_test_pak_type(const owner_info_type& owner, const test_pak_type& cdt, Runner* prunner, const Handler& handler)
	{

		std::cout << "cdt = " << cdt.id() << std::endl;
		cdt_procotol_type rst;
		script_mgr_single_type::get_ins()->execute_rule(rst, yggr::const_args_anchor("create_num"));

		owner_info_container_type owners;
		owners.insert(owner);
		//test_back_handler
		YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
										yggr::task_center::runtime_task_type::E_CAL_RESULT,
										owners, rst);
	}

	template<typename Runner, typename Handler>
	void cal_rst_procotol_type(const owner_info_type& owner, const rst_procotol_type& cdt, Runner* prunner, const Handler& handler)
	{
		//static int count = 0;
		static int i = 0;
		//if(i != send_count)
		if(i != (*send_count_single_type::get_ins()))
		{
			++i;
			std::cout << "-------------calculate " << i << "-----------------------"<< std::endl;
			std::cout << "rst = " << cdt.rst << std::endl;

			cdt_procotol_type rst;
			script_mgr_single_type::get_ins()->execute_rule(rst, yggr::const_args_anchor("create_num"));

			owner_info_container_type owners;
			owners.insert(owner);
			//test_back_handler
			YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
											yggr::task_center::runtime_task_type::E_CAL_RESULT,
											owners, rst);
		}
	}

	void handler_send(const boost::system::error_code& e) const;
	//{
	//	if(!e)
	//	{
	//		std::cout << "send_packet_sucess" << std::endl;
	//	}
	//	else
	//	{
	//		std::cout << "send_packet_fail" << std::endl;
	//	}
	//}

	void cal_network_dispath(void);
	//{
	//	std::cout << "tcp 10061-" << std::endl;
	//}

	//template<typename Owner>
	//void cal_network_dispath2(const Owner& owner)
	//{
	//	std::cout << "session 10061-" << owner << std::endl;
	//}
};

#endif // __LUA_CLT_CALCULATOR_HPP__
