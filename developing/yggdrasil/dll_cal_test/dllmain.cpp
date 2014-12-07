// dllmain.cpp : Defines the entry point for the DLL application.

#include "dll_cal_test.hpp"

#include <boost/atomic.hpp>

#include <yggr/base/ref_count_info.h>

#if defined(_MSC_VER) && defined(_DEBUG)
#   include <vld.h>
#endif // defined(_MSC_VER) && defined(_DEBUG)

typedef yggr::ids::inner_process_id inner_process_id_type;
typedef yggr::ids::id_generator<inner_process_id_type> inner_process_id_gen_type;
typedef yggr::ptr_single<inner_process_id_type> inner_process_id_single_type;

typedef yggr::network::start_data::pak_back_id<yggr::ids::id64_type> test_pak_type;

struct Calculator
	: public boost::enable_shared_from_this<Calculator>
{
private:
	typedef yggr::ref_count_info<> ref_count_info_type;
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

	typedef boost::unordered_set<owner_info_type> owner_info_container_type;

	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename Ctrl_Center>
	void register_dispatchers(Ctrl_Center& cc)
	{
		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::invalid_argument,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::message_size,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::name_too_long,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::network_down,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::no_buffer_space,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::no_memory,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::shut_down,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::timed_out,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::connection_reset,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::connection_aborted,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, boost::asio::error::operation_aborted,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

        CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, 2,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

         CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, 104,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		 CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, 121,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		 CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, 0x60009004,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		 CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, 0x60020001,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));

		 CTRL_HANDLER_PARAM_1(Tag, cc, yggr::u64, session_type::close_code_type::value,
								yggr::system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::cal_network_dispath<yggr::u64>,
												shared_from_this(), _1));
	}

	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
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

	void print_socket_info(session_mgr_type::smap_iter_type& iter)
	{
		try
		{
			std::cout << iter->second->str_local_socket_info() << std::endl;
			std::cout << iter->second->str_remote_socket_info() << std::endl;
		}
		catch(const compatibility::stl_exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		catch(const boost::system::error_code& e)
		{
			std::cerr << e.message() << std::endl;
		}
	}

	template<typename Owner, typename Runner, typename Handler>
	void cal_test_pak_type(const Owner& owner, const test_pak_type& cdt, Runner* prunner, const Handler& handler)
	{
		_count.grab();
		std::cout << "calculate" << std::endl;
		svr_ptr_single_type::obj_ptr_type ptr = svr_ptr_single_type::get_ins();
		if(ptr)
		{
			if(!(ptr->op_handler().get_session_mgr()->access(owner.owner_id(), boost::bind(&Calculator::print_socket_info, shared_from_this(), _1))))
			{
				std::cout << "print_error" << std::endl;
			}
		}

		owner_info_container_type owners;
		owners.insert(owner);
		YGGR_PP_TASK_CENTER_BACK_TASK(handler, prunner, error_maker_type::make_error(0),
										yggr::task_center::runtime_task_type::E_CAL_RESULT,
										owners, cdt);
		_count.drop();
	}

	template<typename Owner>
	void cal_network_dispath(const Owner& owner)
	{
		_count.grab();
		std::cout << "10054 client id = [" << owner << "] is removed" << std::endl;
		_count.drop();
	}

	void join(void)
	{
		for(;!_count.empty(););
	}

	ref_count_info_type _count;
};

typedef yggr::ptr_single<Calculator> cal_sig_type;

YGGR_DLL_API void YGGR_MODULE_CALL_T() reg_cal_enter(runner_shared_info_ptr_type ptr, single_container_type& singles)
{
	inner_process_id_gen_type gen;
	inner_process_id_single_type::init_ins(gen());
	if(!ptr || !singles.init_ins())
	{
		return;
	}

	cal_sig_type::init_ins();
	ctrl_center_single_type::get_ins()->register_dispatchers(*cal_sig_type::get_ins());
	ptr->register_calculator(*cal_sig_type::get_ins());
	svr_ptr_single_type::get_ins()->register_network_protocol<test_pak_type>();
}

YGGR_DLL_API void YGGR_MODULE_CALL_T() unreg_cal_enter(runner_shared_info_ptr_type ptr)
{
	if(!ptr)
	{
		return;
	}

	svr_ptr_single_type::get_ins()->unregister_network_protocol<test_pak_type>();
	ptr->unregister_calculator(*cal_sig_type::get_ins());
	ctrl_center_single_type::get_ins()->unregister_dispatchers(*cal_sig_type::get_ins());

	cal_sig_type::get_ins()->join();
	cal_sig_type::uninstall();

	inner_process_id_single_type::uninstall();
	single_container_type::uninstall();
}

#include <yggr/dll_make/dll_main.ipp>
