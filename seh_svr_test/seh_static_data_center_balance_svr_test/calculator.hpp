//calculator.hpp

#ifndef __UUID_STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_SVR_CALCULATOR_HPP__
#define __UUID_STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_SVR_CALCULATOR_HPP__

#include "config.hpp"

#if defined(YGGR_USE_SEH)

// declar Calcuator
struct Calculator
	: public boost::enable_shared_from_this<Calculator> // logic calculator, extends boost::enable_shared_from_this, fix release time
{
private:
	typedef Calculator this_type;
public:
	// declar yggr::exception::throw_error
	ERROR_MAKER_BEGIN("Clt_Calculator")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
			ERROR_CODE_DEF(E_code_test) // declar code
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_code_test, "code test") // declar code message
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef network_info_type owner_info_type; 

	typedef boost::unordered_set<owner_info_type> owner_info_container_type; //owner infos

	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename CtrlCenter>
	void register_dispatchers(CtrlCenter& cc) // handler register dispatchers
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

		at.template register_calculator<test_pak_type, Recv_Handler>(
				handler, boost::bind(&this_type::cal_test_pak_type<runner_type, 
																	recv_handler_type>,
										this_type::shared_from_this(), _1, _2, _3, _4),
										boost::bind(&this_type::handler_seh<test_pak_type>, 
														this_type::shared_from_this(), _1, _2)); // use custom seh fixer
	}

	template<typename Action_Table>
	void unregister_cal_object(Action_Table& at) // handler unregister calculator
	{
		at.template unregister_calculator<test_pak_type>();
	}

	void print_socket_info(const session_mgr_type::smap_type::base_type& base,
							session_mgr_type::smap_citer_type iter) const;

	// android can optimization failed this foo, so need write to cpp

	//int test_seh_generator(int* p) const
	//{
	//	*p = 100;
	//	return *p;
	//}

	int test_seh_generator(int* p) const;

	//handler cal test_pak_type
	template<typename Runner, typename Handler>
	void cal_test_pak_type(const owner_info_type& owner, const test_pak_type& cdt, Runner* prunner, const Handler& handler) 
	{
		//owner: uuid, cdt: network_packet, prunner: the cdt's runner, handler: handler of back packet

		static yggr::size_type online_size = 0;

		// test seh s
		if(online_size == 0)
		{
			int *p = 0;
			std::cout << "test seh code run " << p << std::endl;
			++online_size;
			std::cout << test_seh_generator(p) << *p << std::endl;
		}
		// test seh e;

		std::cout << "calculate" << std::endl;
		svr_ptr_single_type::obj_ptr_type ptr = svr_ptr_single_type::get_ins(); // get server ptr
		if(ptr)
		{
			// access server and print socket info
			if(!(ptr->op_handler().
					get_session_mgr()->
						access(owner.owner_id(), 
								boost::bind(&Calculator::print_socket_info, this_type::shared_from_this(), _1, _2))))
			{
				std::cout << "print_error" << std::endl;
			}
		}

		owner_info_container_type owners; // back to some owners
		owners.insert(owner); // the current packet is sent only to the sender
		YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0), 
										yggr::task_center::runtime_task_type::E_CAL_RESULT,
										owners, cdt); // echo "cdt" to sender
	}

	inline void cal_network_dispath(const yggr::u64& owner)
	{
		std::cout << "10054 client id = [" << owner << "] is removed" << std::endl;
	}

	template<typename Data>
	void handler_seh(const owner_info_type& owner, const Data* pdata) const
	{
		std::cout << "seh occur owner id = " << owner.owner_id() << std::endl;
	}
};

#endif // #if defined(YGGR_USE_SEH)

#endif // __UUID_STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_SVR_CALCULATOR_HPP__