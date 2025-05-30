//calculator.hpp

#ifndef __STATIC_DATA_CENTER_TCP_CLT_CALCULATOR_HPP__
#define __STATIC_DATA_CENTER_TCP_CLT_CALCULATOR_HPP__

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
	void register_dispatchers(Ctrl_Center& cc) // ע�������쳣�Ĵ���
	{

		// this is fake fixer
		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_refused,
						yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM,
						boost::bind(&this_type::cal_network_dispath<yggr::ids::id64_type>,
										shared_from_this(), _1));
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
				handler, boost::bind(&Calculator::cal_test_pak_type<owner_info_type,
																	runner_type,
																	recv_handler_type>,
										shared_from_this(), _1, _2, _3, _4));
	}

	template<typename Action_Table>
	void unregister_cal_object(Action_Table& at)
	{
		at.template unregister_calculator<test_pak_type>();
	}


	template<typename Owner, typename Runner, typename Handler>
	void cal_test_pak_type(const Owner& owner, test_pak_type& cdt, Runner* prunner, const Handler& handler)
	{
		static yggr::time::time stm;
		static int now_count = 0;

		if(now_count == 0)
		{
			stm = yggr::time::time();
		}

		++now_count;
		std::cout << "-------------calculate " << now_count << "-----------------------"<< std::endl;
		std::cout << "id = " << cdt.id() << std::endl;

		if(now_count < send_count())
		{
			owner_info_container_type owners;
			owners.insert(owner);
#if 1
			//test_back_handler
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
#endif // 0, 1
		}
		else
		{
			yggr::time::time etm;
			std::cout << (etm - stm).ex_to_second() << std::endl;
		}

		return;
	}

	void handler_send(const boost::system::error_code& e) const;


	template<typename Owner>
	void cal_network_dispath(const Owner& owner)
	{
		std::cout << "session 10061-" << owner << std::endl;
	}

public:
	inline static yggr::u32& send_count(void)
	{
		static yggr::u32 count = 0;
		return count;
	}
};

#endif // __STATIC_DATA_CENTER_TCP_CLT_CALCULATOR_HPP__
