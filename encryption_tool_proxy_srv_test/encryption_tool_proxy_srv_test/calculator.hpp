//calculator.hpp

#ifndef __SAFE_PACKET_TOOL_TCP_SVR_CALCULATOR_HPP__
#define __SAFE_PACKET_TOOL_TCP_SVR_CALCULATOR_HPP__

#include <cassert>

#include "config.hpp"
#include <yggr/time/time.hpp>

struct Calculator 
	: public boost::enable_shared_from_this<Calculator>
{
private:
	typedef Calculator this_type;
public:
	ERROR_MAKER_BEGIN("Clt_Calculator")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
			ERROR_CODE_DEF(E_code_test)
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_code_test, "code test")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef network_info_type owner_info_type;

	typedef boost::unordered_multiset<owner_info_type> owner_info_container_type;

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

		at.template register_calculator<proxy_register_back_msg_type, Recv_Handler>(
				handler, boost::bind(&this_type::cal_register_back_msg_type<runner_type,
																			recv_handler_type>,
									this_type::shared_from_this(), _1, _2, _3, _4));

		at.template register_calculator<proxy_mode_change_back_msg_type, Recv_Handler>(
				handler, boost::bind(&this_type::cal_mode_change_back_msg_type<runner_type,
																				recv_handler_type>,
									this_type::shared_from_this(), _1, _2, _3, _4));

		at.template register_calculator<proxy_unregister_back_msg_type, Recv_Handler>(
				handler, boost::bind(&this_type::cal_proxy_unregister_back_msg_type<runner_type,
																					recv_handler_type>,
									this_type::shared_from_this(), _1, _2, _3, _4));
	}


	template<typename Runner, typename Handler>
	void cal_test_pak_type(const owner_info_type& owner, const test_pak_type& cdt, Runner* prunner, const Handler& handler)
	{
		//static int count = 0;
		static int i = 0;

		if(owner.size() == 1)
		{
			proxy_register_msg_type msg;
			/*msg.add_reg_data(test_pak_type::data_info(),
								proxy_mode_def_type::E_proxy_mode_monopolize,
								test_pak_type::cal_type());*/

			msg.add_reg_data(test_pak_type::data_info(),
								*now_state_single_type::get_ins(),
								test_pak_type::cal_type());

			owner_info_container_type owners;
			owners.insert(owner);
			YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
											yggr::task_center::runtime_task_type::E_CAL_RESULT,
											owners, msg);
			return;

		}

		++i;
		std::cout << "-------------calculate " << i << "-----------------------"<< std::endl;
		std::cout << "id = " << cdt.id() << std::endl;

		owner_info_container_type owners;
		owners.insert(owner);
		YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
										yggr::task_center::runtime_task_type::E_CAL_RESULT,
										owners, cdt);

		return;
	}

	template<typename Runner, typename Handler>
	void cal_register_back_msg_type(const owner_info_type& owner,
									const proxy_register_back_msg_type& cdt,
									Runner* prunner,
									const Handler& handler)
	{
		typedef proxy_register_back_msg_type::reg_back_map_type reg_back_map_type;
		typedef reg_back_map_type::const_iterator citer_type;

		const reg_back_map_type& ref_back = cdt.reg_back_map();

		//Debug Code S
		bool bend = false;
		//Debug Code E

		for(citer_type i = ref_back.begin(), isize = ref_back.end(); i != isize; ++i)
		{
			std::cout << "register result id = " << i->first
						<< ", now_mode = " << i->second.first
						<< ", state = " << i->second.second << std::endl;

			//Debug Code S
			if(i->second.first == *chg_state_single_type::get_ins() || i->second.second == 4)
			{
				bend = true;
			}
			//Debug Code E
		}

		//Debug code S
		if(*now_state_single_type::get_ins() ==  *chg_state_single_type::get_ins() || bend)
		{
			return;
		}

		proxy_mode_change_msg_type chg_msg;
		chg_msg.add_chg_data(test_pak_type::data_info(), *chg_state_single_type::get_ins());

		owner_info_container_type owners;
		owners.insert(owner);
		YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
										yggr::task_center::runtime_task_type::E_CAL_RESULT,
										owners, chg_msg);
		//Debug code E
	}

	template<typename Runner, typename Handler>
	void cal_mode_change_back_msg_type(const owner_info_type& owner,
										const proxy_mode_change_back_msg_type& cdt,
										Runner* prunner,
										const Handler& handler)
	{
		std::cout << "proxy server mode changed data_info = "
					<< cdt.now_data_info()
					<< " now mode is "
					<< cdt.now_mode() << std::endl;

		if(*now_state_single_type::get_ins() ==  *chg_state_single_type::get_ins())
		{
			return;
		}

		proxy_unregister_msg_type unreg_msg;
		unreg_msg.add_unreg_data(test_pak_type::data_info());

		owner_info_container_type owners;
		owners.insert(owner);
		YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
										yggr::task_center::runtime_task_type::E_CAL_RESULT,
										owners, unreg_msg);
	}

	template<typename Runner, typename Handler>
	void cal_proxy_unregister_back_msg_type(const owner_info_type& owner,
											const proxy_unregister_back_msg_type& cdt,
											Runner* prunner,
											const Handler& handler)
	{
		typedef proxy_unregister_back_msg_type::unreg_back_map_type unreg_back_map_type;
		typedef unreg_back_map_type::const_iterator unreg_back_map_citer_type;
		const unreg_back_map_type& unreg_back_map = cdt.unreg_back_map();

		for(unreg_back_map_citer_type i = unreg_back_map.begin(), isize = unreg_back_map.end(); i != isize; ++i)
		{
			std::cout << "unreg data = "
						<< i->first
						<< " is right "
						<< (i->second? "true" : "false") << std::endl;
		}

	}
};

#endif // __SAFE_PACKET_TOOL_TCP_SVR_CALCULATOR_HPP__
