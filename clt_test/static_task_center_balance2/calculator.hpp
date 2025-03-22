//calculator.hpp

#ifndef __CLT_TEST_STATIC_TASK_CENTER_BLANCE2_CALCULATOR_HPP__
#define __CLT_TEST_STATIC_TASK_CENTER_BLANCE2_CALCULATOR_HPP__

#include "config.hpp"
#include <yggr/time/time.hpp>

#include <cassert>

struct Calculator 
	: public boost::enable_shared_from_this<Calculator>
{
private:
	typedef Calculator this_type;
public:
	ERROR_MAKER_BEGIN("Calculator")
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
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_refused,
						yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this()));

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

		 CTRL_HANDLER(Tag, cc, boost::asio::error::connection_refused,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath,
										this_type::shared_from_this(), _1));

		 // non owner
		 CTRL_HANDLER(Tag, cc, boost::asio::error::invalid_argument,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		CTRL_HANDLER(Tag, cc, boost::asio::error::message_size,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		CTRL_HANDLER(Tag, cc, boost::asio::error::name_too_long,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		CTRL_HANDLER(Tag, cc, boost::asio::error::network_down,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		CTRL_HANDLER(Tag, cc, boost::asio::error::no_buffer_space,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		CTRL_HANDLER(Tag, cc, boost::asio::error::no_memory,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		CTRL_HANDLER(Tag, cc, boost::asio::error::shut_down,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		CTRL_HANDLER(Tag, cc, boost::asio::error::timed_out,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_reset,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_aborted,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		CTRL_HANDLER(Tag, cc, boost::asio::error::operation_aborted,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

        CTRL_HANDLER(Tag, cc, 2,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		if(boost::asio::error::connection_reset != 104) // windows not equal see boost asio/erron.h
		{
			CTRL_HANDLER(Tag, cc, 104,
							yggr::system_controller::system_code::E_SESSION_SYSTEM,
							boost::bind(&this_type::cal_network_dispath_non_owner,
											this_type::shared_from_this() ));
		}

		 CTRL_HANDLER(Tag, cc, 121,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		 CTRL_HANDLER(Tag, cc, 0x60009004,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		 CTRL_HANDLER(Tag, cc, 0x60020001,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		 CTRL_HANDLER(Tag, cc, boost::asio::error::connection_refused,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

		 CTRL_HANDLER(Tag, cc, boost::asio::error::not_connected,
						yggr::system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::cal_network_dispath_non_owner,
										this_type::shared_from_this() ));

	}

	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;

		typedef typename action_table_type::id_parser_type id_parser_type;
		typedef typename id_parser_type::id_type reg_id_type;


		reg_id_type reg_id = id_parser_type::template get_reg_id<test_pak_type>();
		std::cout << reg_id << std::endl;

		at.template register_calculator<net_ipak_type, Recv_Handler>(
				reg_id,
				handler, boost::bind(&this_type::cal_test_pak_type<runner_type,
																	recv_handler_type>,
										this_type::shared_from_this(), _1, _2, _3, _4));
	}

	template<typename Action_Table>
	void unregister_cal_object(Action_Table& at)
	{
		typedef Action_Table action_table_type;

		typedef typename action_table_type::id_parser_type id_parser_type;
		typedef typename id_parser_type::id_type reg_id_type;

		reg_id_type reg_id = id_parser_type::template get_reg_id<test_pak_type>();

		at.unregister_calculator(reg_id);
	}

	template<typename Runner, typename Handler>
	void cal_test_pak_type(const owner_info_type& owner, net_ipak_type& ipak, 
								Runner* prunner, const Handler& handler)
	{
		static yggr::time::time stm;
		static int now_count = 0;

		if(now_count == 0)
		{
			stm = yggr::time::time();
		}

		++now_count;
		std::cout << "-------------calculate " << now_count << "-----------------------"<< std::endl;
		
		test_pak_type cdt;
		ipak.load(cdt);
		
		std::cout << "id = " << cdt.id() << std::endl;


		if(now_count < send_count())
		{

			owner_info_container_type owners;
			owners.insert(owner);
			//test_sender
			for(owner_info_container_type::const_iterator i = owners.begin(), isize = owners.end();
					i != isize; ++i)
			{
#if 0
				clt_sender_type::s_send_packet(cdt.id(), *i, cdt,
													boost::bind(&this_type::handler_send,
																	this_type::shared_from_this(),
																	_1));
#else
				clt_sender_type()((*i).owner_id(), *i, cdt,
													boost::bind(&this_type::handler_send,
																	this_type::shared_from_this(),
																	_1));
#endif // 0, 1
			}

		}
		else
		{
			yggr::time::time etm;
			std::cout << (etm - stm).ex_to_second() << std::endl;
		}

		return;
	}

	void handler_send(const boost::system::error_code& e) const;

	inline void cal_network_dispath(const yggr::ids::id64_type& owner)
	{
		std::cout << "session 10061-" << owner << std::endl;
	}


	void cal_network_dispath_non_owner(void);

public:
	inline static yggr::u32& send_count(void)
	{
		static yggr::u32 count = 0;
		return count;
	}
};

#endif // __CLT_TEST_STATIC_TASK_CENTER_BLANCE2_CALCULATOR_HPP__