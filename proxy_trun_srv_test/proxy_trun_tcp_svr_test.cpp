// proxy_trun_tcp_svr_test.cpp

#include <iostream>
#include <cassert>

#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_set.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/system_code.hpp>

#include <yggr/exception/exception.hpp>
#include <yggr/system_controller/ctrl_center.hpp>

#include <yggr/ids/uuid.hpp>
#include <yggr/ids/base_ids_def.hpp>

#include <yggr/archive/net_oarchive.hpp>
#include <yggr/archive/net_iarchive.hpp>
#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/task_center/task.hpp>
#include <yggr/task_center/task_creator.hpp>
#include <yggr/task_center/task_center.hpp>
#include <yggr/task_center/work_runner_saver.hpp>
#include <yggr/task_center/recver_handler_mgr.hpp>

//#include <yggr/proxy/proxy_info.hpp>

//#include <yggr/safe_container/safe_hashed_map_queue.hpp>

#include <yggr/adapter/adapter_mgr.hpp>
#include <yggr/adapter/adapter_parser_helper.hpp>

#include <yggr/thread/static_work_runner.hpp> // use static_work_runner
#include <yggr/thread/thread_mgr.hpp>
#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/network/network_config/network_balance_tcp_config.hpp>
#include <yggr/network/start_data/pak_back_id.hpp>
#include <yggr/network/session_helper/session_crypher_creator.hpp>
#include <yggr/network/session_helper/session_checker_creator.hpp>
#include <yggr/network/session_helper/first_action.hpp>
#include <yggr/network/session_helper/owner_generator.hpp>
#include <yggr/network/session_helper/session_creator.hpp>

#include <yggr/network/packets_support/packets_checker.hpp>
#include <yggr/network/packets_support/packets_crypher.hpp>

#include <yggr/network/adapter_helper/io_converter.hpp>
#include <yggr/network/support/data_info_def.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/network/network_packet.hpp>
#include <yggr/network/network_config/network_tcp_config.hpp>
#include <yggr/network/connection.hpp>
#include <yggr/network/ex_linker.hpp>

#include <yggr/network/session.hpp>
#include <yggr/network/session_mgr.hpp>
#include <yggr/network/session_helper/session_killer.hpp>
#include <yggr/network/service_handler.hpp>
#include <yggr/network/network_handler.hpp>

#include <yggr/network/session_state/session_state_fixer.hpp>
#include <yggr/network/session_state/session_state_checker.hpp>

#include <yggr/proxy/proxy_task_creator.hpp>
#include <yggr/proxy/adapter_helper/proxy_io_converter.hpp>
#include <yggr/proxy/proxy_mode/proxy_mode_tactics.hpp>
#include <yggr/proxy/proxy_mode/proxy_modes.hpp>
#include <yggr/proxy/proxy_mode/proxy_mode_owner_id_store.hpp>
#include <yggr/proxy/proxy_mode/proxy_mode_creator.hpp>
#include <yggr/proxy/proxy_repeater.hpp>
#include <yggr/proxy/proxy_container.hpp>
#include <yggr/proxy/proxy_dispatcher.hpp>
#include <yggr/proxy/proxy_router.hpp>

//#include <yggr/proxy/proxy_msg/proxy_register_msg.hpp>
//#include <yggr/proxy/proxy_msg/proxy_register_back_msg.hpp>
//#include <yggr/proxy/proxy_msg/proxy_mode_change_msg.hpp>
//#include <yggr/proxy/proxy_msg/proxy_mode_change_back_msg.hpp>
//#include <yggr/proxy/proxy_msg/proxy_unregister_msg.hpp>
//#include <yggr/proxy/proxy_msg/proxy_unregister_back_msg.hpp>
#include <yggr/proxy/proxy_ctrl/basic_proxy_msg_fixer.hpp>

#include <yggr/server/server_config/server_tcp_config.hpp>
#include <yggr/server/start_mode/server_active_start_mode.hpp>
#include <yggr/server/basic_svr_handler.hpp>
#include <yggr/server/server.hpp>

//#include <yggr/log/yggr_exception_log_accesser.hpp>
#include <yggr/log/default_log_accesser.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::system_controller::ctrl_center ctrl_center_type;

typedef yggr::network::start_data::pak_back_id<yggr::ids::id64_type> test_pak_type;

typedef yggr::network::network_info<yggr::ids::id64_type> network_info_type;
typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type;

typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type,
				network_info_type
			> net_opak_type;

typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type,
				network_info_type
			> net_ipak_type;

// proxy task_center and adapteer def start
typedef net_ipak_type proxy_task_type;

typedef yggr::network::support::network_data_info_parser<data_info_def_type> proxy_task_data_info_parser_type;
typedef proxy_task_data_info_parser_type::data_info_type data_info_type;

YGGR_PP_THREAD_ACTRION_HELPER_MAKE_REG_ID_PARSER(action_reg_data_info_parser,
													data_info_type,
													proxy_task_data_info_parser_type,
													data_info)

typedef yggr::thread::action_helper::action_reg_data_info_parser action_reg_id_parser_type;

typedef yggr::thread::action_helper::action_id_parser<action_reg_id_parser_type,
														proxy_task_type::data_info_getter> proxy_action_id_parser_type;

typedef yggr::thread::static_work_runner<proxy_task_type, proxy_action_id_parser_type> proxy_work_runner_type;

typedef proxy_work_runner_type::shared_info_ptr_type proxy_runner_shared_info_ptr_type;

typedef yggr::task_center::work_runner_saver<yggr::task_center::type_traits::mark_saver_condition,
												proxy_work_runner_type> proxy_cdt_saver_type;

typedef yggr::task_center::recver_handler_mgr<yggr::task_center::type_traits::mark_saver_result, proxy_task_type> proxy_rst_saver_type;

typedef yggr::proxy::proxy_task_creator<proxy_task_type> proxy_task_creator_type;

typedef yggr::task_center::task_center<proxy_task_creator_type, proxy_cdt_saver_type, proxy_rst_saver_type> proxy_task_center_type;

typedef yggr::adapter::adapter_id_parser_def
		<
			yggr::task_center::task_data_send_typeid_getter
			<
				data_info_def_type,
				yggr::network::support::network_data_info_parser
			>,

			yggr::task_center::task_data_send_typeid_getter
			<
				data_info_def_type,
				yggr::network::support::network_data_info_parser
			>,

			YGGR_IDS_PARSER_CONST_MEM_FOO(net_ipak_type::base_type,
											const yggr
													::task_center
														::default_task_data_info_type
															::data_info_type&,
											data_info),

			YGGR_IDS_PARSER_CONST_MEM_FOO(proxy_task_type::base_type,
											const yggr
												::task_center
													::default_task_data_info_type
														::data_info_type&,
											data_info),

			YGGR_IDS_PARSER_CONST_MEM_FOO(proxy_task_type::base_type,
											const yggr
												::task_center
													::default_task_data_info_type
														::data_info_type&,
											data_info),

			YGGR_IDS_PARSER_CONST_MEM_FOO(net_opak_type::base_type,
											const yggr
													::task_center
														::default_task_data_info_type
															::data_info_type&,
											data_info)
		> proxy_adapter_id_parser_def_type;

typedef yggr::adapter::adapter_mgr<proxy_task_center_type,
									net_ipak_type,
									net_opak_type,
									proxy_adapter_id_parser_def_type,
									yggr::proxy::adapter_helper::proxy_io_converter> proxy_adapter_mgr_type;

typedef proxy_adapter_mgr_type::reg_def_type network_protocol_reg_def_type;

// proxy task_center and adapteer def end

typedef boost::unordered_set<network_info_type> owner_info_container_type;

typedef yggr::proxy::proxy_mode::proxy_mode_tactics proxy_mode_tactics_type;
typedef yggr::proxy::proxy_mode::proxy_mode_owner_id_store<owner_info_container_type> proxy_mode_data_store_type;

typedef boost::mpl::vector
					<
						yggr::proxy::proxy_mode::proxy_mode_monopolize
						<
							proxy_mode_data_store_type
						>,

						yggr::proxy::proxy_mode::proxy_mode_source_hash
						<
							proxy_mode_data_store_type
						>,

						yggr::proxy::proxy_mode::proxy_mode_blanace
						<
							proxy_mode_data_store_type
						>,

						yggr::proxy::proxy_mode::proxy_mode_all
						<
							proxy_mode_data_store_type
						>
					> mode_vector_type;

typedef yggr::proxy::proxy_mode::proxy_mode_creator<
														mode_vector_type,
														proxy_mode_data_store_type
													> proxy_mode_creator_type;

typedef yggr::proxy::proxy_container<
										proxy_action_id_parser_type::id_type,
										proxy_task_type,
										proxy_mode_tactics_type,
										proxy_mode_creator_type
									> proxy_container_type;

typedef yggr::proxy::proxy_dispatcher<proxy_action_id_parser_type,
										proxy_container_type> proxy_dispatcher_type;

typedef yggr::proxy::proxy_router<proxy_dispatcher_type> proxy_router_type;

typedef boost::shared_ptr<proxy_router_type> proxy_router_ptr_type;

typedef yggr::proxy::proxy_repeater<
										proxy_runner_shared_info_ptr_type,
										proxy_task_type,
										owner_info_container_type,
										yggr::error,
										proxy_router_type
									> proxy_repeater_type;

typedef boost::shared_ptr<proxy_repeater_type> proxy_repeater_ptr_type;

typedef yggr::network::ex_linker<yggr::network::network_config::balance_tcpv4_config> linker_type;

typedef yggr::network::connection<linker_type, net_opak_type, net_ipak_type> conn_type;

YGGR_PP_REGISTER_SESSION_ID_CREATOR_BEGIN(test_pak_type, test_pak, const yggr::ids::id64_type& )
	test_pak.id()
YGGR_PP_REGISTER_SESSION_ID_CREATOR_END()

typedef yggr::network::packets_support::packets_checker<network_info_type> packets_checker_type;
YGGR_PP_REGISTER_SESSION_CHECKER_CREATOR_BEGIN(test_pak_type, test_pak, packets_checker_type)
	test_pak.id()
YGGR_PP_REGISTER_SESSION_CHECKER_CREATOR_END()

typedef yggr::network::packets_support::packets_crypher<void> packets_crypher_type;
YGGR_PP_REGISTER_SESSION_CRYPHER_CREATOR_BEGIN(test_pak_type, test_pak, packets_crypher_type)
YGGR_PP_REGISTER_SESSION_CRYPHER_CREATOR_END()

typedef yggr::network::session_helper::first_action first_action_type;


typedef yggr::ids::pointer_id_generator<yggr::ids::id64_type> session_id_generator_type;
typedef yggr::network::start_data::start_data_generator<test_pak_type> start_data_generator_type;
typedef yggr::network::session_helper::owner_generator<network_info_type> owner_generator_type;

typedef yggr::network::session_helper::session_creator<test_pak_type, conn_type/*,
														yggr::network::session_helper::session_id_creator,
														yggr::network::session_helper::session_checker_creator,
														yggr::network::session_helper::session_crypher_creator*/
														> session_creator_type;

typedef yggr::server::start_mode::server_active_start_mode<
															session_id_generator_type,
															start_data_generator_type,
															owner_generator_type,
															session_creator_type
														> server_start_mode;



//proxy session def start

typedef yggr::network::session<yggr::network::type_traits::tag_server,
								conn_type,
								proxy_adapter_mgr_type,
								packets_checker_type,
								packets_crypher_type,
								first_action_type,
								yggr::network::session_state::default_session_state_fixer_type
							> proxy_session_type;

typedef yggr::network::session_helper::session_killer<proxy_session_type> session_killer_type;

typedef yggr::network::session_mgr<proxy_session_type,
									server_start_mode,
									yggr::network::session_state::default_session_state_checker_type,
									session_killer_type
									> proxy_session_mgr_type;


//proxy session def end


typedef yggr::network::service_handler<
											conn_type::service_type,
											boost::asio::signal_set,
											yggr::thread::boost_thread_config_type,
											std::allocator,
											std::vector,
											yggr::network::balance_io_service_selector
										> service_handler_type;

typedef yggr::server::basic_svr_handler< yggr::server::server_config::server_tcp_config_type,
										service_handler_type,
										proxy_session_mgr_type
									> tcp_svr_handler_type;


typedef yggr::network::network_handler<tcp_svr_handler_type> network_handler_type;

typedef yggr::server::server<network_handler_type> svr_type;

typedef yggr::ptr_single<svr_type> svr_ptr_single_type;
typedef svr_ptr_single_type::obj_ptr_type svr_ptr_type;


typedef yggr::proxy::proxy_ctrl::basic_proxy_msg_fixer
		<
			owner_info_container_type,
			proxy_repeater_type,
			net_ipak_type,
			net_opak_type,
			svr_ptr_single_type
		> base_proxy_msg_calculator_type;

typedef base_proxy_msg_calculator_type::proxy_register_msg_type proxy_register_msg_type;
typedef base_proxy_msg_calculator_type::proxy_register_back_msg_type proxy_register_back_msg_type;
typedef base_proxy_msg_calculator_type::proxy_mode_change_msg_type proxy_mode_change_msg_type;
typedef base_proxy_msg_calculator_type::proxy_mode_change_back_msg_type proxy_mode_change_back_msg_type;
typedef base_proxy_msg_calculator_type::proxy_unregister_msg_type proxy_unregister_msg_type;
typedef base_proxy_msg_calculator_type::proxy_unregister_back_msg_type proxy_unregister_back_msg_type;

typedef yggr::proxy::proxy_fix_state_def proxy_fix_state_def_type;

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
							const proxy_router_ptr_type& prouter)
		: base_type(prepeater, prouter)
	{
	}

	~proxy_msg_calculator(void)
	{
	}
};

int main(int argc, char* argv[])
{
	typedef yggr::ptr_single<proxy_task_center_type> proxy_task_center_single;
	typedef yggr::ptr_single<ctrl_center_type> ctrl_center_single;
	//typedef yggr::ptr_single<yggr::log::yggr_exception_log_accesser_type> log_accesser_single;
	typedef yggr::ptr_single<yggr::log::default_log_accesser> log_accesser_single;

	yggr::u32 port = 9000;
	std::cout << "please input port:" << std::endl;
	std::cin >> port;

	//yggr::log::yggr_exception_log_accesser_type::init_type log_init; //memd to BOOST_PP_LOCAL_MACRO
	//log_init.push_back("svr_nt_log");

	//log_accesser_single::init_ins(log_init);

	//yggr::log::yggr_exception_log_accesser_type::data_creator_type dc1("svr_nt_log");
	//log_accesser_single::get_ins()->register_msg(10054, dc1);

	yggr::log::default_log_accesser::init_type log_init;
	log_init.push_back("svr_nt_log");

	log_accesser_single::init_ins(log_init); 

	yggr::log::default_log_accesser::op_caller_type caller1("svr_nt_log");
	log_accesser_single::get_ins()->register_msg(10054, caller1);

	ctrl_center_single::init_ins();
	yggr::ptr_single<yggr::exception::exception>::init_ins();

	proxy_runner_shared_info_ptr_type proxy_shared_info_ptr(proxy_work_runner_type::create_work_runner_shared_info());

	if(!proxy_shared_info_ptr)
	{
		return -1;
	}


	proxy_router_ptr_type prouter(new proxy_router_type());
	proxy_repeater_ptr_type prepeater(new proxy_repeater_type(proxy_shared_info_ptr, prouter));
	//prepeater->register_repeater_data<test_pak_type>();

	boost::shared_ptr<proxy_msg_calculator> proxy_cal(new proxy_msg_calculator(prepeater, prouter));

	proxy_shared_info_ptr->register_calculator(*proxy_cal);

	proxy_task_center_single::init_ins(proxy_shared_info_ptr, proxy_task_center_type::rst_saver_init_type());

	{
	    proxy_runner_shared_info_ptr_type tmp;
		proxy_shared_info_ptr.swap(tmp);
	}

	svr_ptr_single_type::init_ins(4,
									tcp_svr_handler_type::init_type(port,
																	*(proxy_task_center_single::get_ins()),
																	tcp_svr_handler_type::link_handler_init_type()));

	svr_ptr_type psvr = svr_ptr_single_type::get_ins();
	//psvr->register_network_protocol_of_container<test_pak_type, net_ipak_type>(); // proxy turn data type registered from proxy_svr

	//register network protocol, some proxy_system's used network data
	psvr->register_network_protocol_of_container<proxy_register_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_recv_enable);

	psvr->register_network_protocol_of_container<proxy_register_back_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_send_enable);

	psvr->register_network_protocol_of_container<proxy_mode_change_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_recv_enable);

	psvr->register_network_protocol_of_container<proxy_mode_change_back_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_send_enable);

	psvr->register_network_protocol_of_container<proxy_unregister_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_recv_enable);

	psvr->register_network_protocol_of_container<proxy_unregister_back_msg_type,
													net_ipak_type>(yggr::network::network_direct_def_type::E_send_enable);


	{
		ctrl_center_single::obj_ptr_type pctrl = ctrl_center_single::get_ins();
		if(pctrl)
		{
			pctrl->register_controller(*psvr);
			pctrl->register_dispatchers(*proxy_cal);
		}
	}

	try
	{
		psvr->run();
		std::cout << "started" << std::endl;

#	ifdef _DEBUG
		char cc = 0;
		std::cin >> cc;
#	else
		psvr->join();
		//yggr::ptr_single<yggr::exception::exception>::get_ins()->join(); // linux exit hang
#	endif //_DEBUG

	}
	catch(const yggr::stl_exception& e)
	{
        std::cerr << e.what() << std::endl;
	}
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
	}

	psvr->stop();
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	{
	    svr_ptr_type tmp;
		psvr.swap(tmp);
	}
	prepeater->clear();
	ctrl_center_single::uninstall();
	svr_ptr_single_type::uninstall();
	proxy_task_center_single::uninstall();

	std::cout << "stop end" << std::endl;
	yggr::ptr_single<yggr::exception::exception>::uninstall();

	return 0;
}
