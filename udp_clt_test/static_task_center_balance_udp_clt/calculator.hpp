//calculator.hpp

#ifndef __STATIC_DATA_CENTER_UDP_CLT_CALCULATOR_HPP__
#define __STATIC_DATA_CENTER_UDP_CLT_CALCULATOR_HPP__

#include <cassert>

#include "config.hpp"
#include <yggr/time/time.hpp>


struct Calculator : public boost::enable_shared_from_this<Calculator>
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

	template<typename Tag, typename CtrlCenter>
	void register_dispatchers(CtrlCenter& cc)
	{
		CTRL_HANDLER(Tag, cc, boost::asio::error::invalid_argument,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::message_size,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::name_too_long,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::network_down,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::no_buffer_space,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::no_memory,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::shut_down,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::timed_out,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_reset,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_aborted,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::operation_aborted,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, 2,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		if(boost::asio::error::connection_reset != 104) // windows not equal see boost asio/erron.h
		{
			CTRL_HANDLER(Tag, cc, 104,
							yggr::system_controller::system_code::E_SESSION_SYSTEM,
							boost::bind(&this_type::cal_network_dispath,
											this_type::shared_from_this(), _1));
		}

		CTRL_HANDLER(Tag, cc, 121,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, 0x60009004,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, 0x60020001,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, session_type::close_code_type::value,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
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

		static yggr::time::time stm;

		static int i = 0;

		if(!i)
		{
			stm = yggr::time::time();
		}

		++i;
		std::cout << "-------------calculate " << i << "-----------------------"<< std::endl;
		std::cout << "id = " << cdt.id() << std::endl;


		if(i < send_count())
		{

			owner_info_container_type owners;
			owners.insert(owner);
			YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
											yggr::task_center::runtime_task_type::E_CAL_RESULT,
											owners, cdt);
		}
		else
		{
			yggr::time::time etm;
			std::cout << (etm - stm).ex_to_second() << std::endl;
		}

		return;
	}

	inline void cal_network_dispath(const yggr::u64& owner)
	{
		std::cout << "10054 client id = [" << owner << "] is removed" << std::endl;
	}

public:
	inline static yggr::u32& send_count(void)
	{
		static yggr::u32 count = 0;
		return count;
	}
};

#endif // __STATIC_DATA_CENTER_UDP_SVR_CALCULATOR_HPP__