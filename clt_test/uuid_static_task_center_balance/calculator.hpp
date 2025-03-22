//calculator.hpp

#ifndef __CLT_TEST_UUID_STATIC_TASK_CENTER_BLANCE_CALCULATOR_HPP__
#define __CLT_TEST_UUID_STATIC_TASK_CENTER_BLANCE_CALCULATOR_HPP__

#include <cassert>

#include "config.hpp"

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

	template<typename Tag, typename Ctrl_Center>
	void register_dispatchers(Ctrl_Center& cc) // 注册网络异常的处理
	{

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_refused,
						yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));
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
	}

	template<typename Action_Table>
	void unregister_cal_object(Action_Table& at)
	{
		at.template unregister_calculator<test_pak_type>();
	}


	template<typename Runner, typename Handler>
	void cal_test_pak_type(const owner_info_type& owner, const test_pak_type& cdt, Runner* prunner, const Handler& handler)
	{
		//static int count = 0;
		static int i = 0;

		++i;
		std::cout << "-------------calculate " << i << "-----------------------"<< std::endl;
		std::cout << "id = " << cdt.id() << std::endl;


		if(i < send_count())
		{

			owner_info_container_type owners;
			owners.insert(owner);
			//test_back_handler
#if 0
			YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
											yggr::task_center::runtime_task_type::E_CAL_RESULT,
											owners, cdt);
#else
			//test_sender
			for(owner_info_container_type::const_iterator i = owners.begin(), isize = owners.end();
					i != isize; ++i)
			{
#	if 0
				clt_sender_type::s_send_packet(cdt.id(), *i, cdt,
													boost::bind(&this_type::handler_send,
																	this_type::shared_from_this(),
																	_1));
#	else 
				clt_sender_type()((*i).owner_id(), *i, cdt,
									boost::bind(&this_type::handler_send,
													this_type::shared_from_this(),
													_1));
#	endif // 0, 1
			}
		}
#endif // 0, 1

		return;
	}

	inline void cal_network_dispath(const yggr::ids::uuid& owner)
	{
		std::cout << "session 10061-" << owner << std::endl;
	}

	void handler_send(const boost::system::error_code& e) const;

public:
	inline static yggr::u32& send_count(void)
	{
		static yggr::u32 count = 0;
		return count;
	}

};

#endif // __CLT_TEST_UUID_STATIC_TASK_CENTER_BLANCE_CALCULATOR_HPP__