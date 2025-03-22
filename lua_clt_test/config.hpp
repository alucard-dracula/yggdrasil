// config.hpp

#ifndef __LUA_CLT_CONFIG_HPP__
#define __LUA_CLT_CONFIG_HPP__

#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/exception/exception.hpp>
#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/system_controller/system_code.hpp>

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

#include <yggr/adapter/adapter_mgr.hpp>
#include <yggr/adapter/adapter_parser_helper.hpp>

#include <yggr/thread/static_work_runner.hpp> // use static_work_runner
#include <yggr/thread/thread_mgr.hpp>

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
#include <yggr/network/session_helper/session_tcp_curer.hpp>
#include <yggr/network/service_handler.hpp>
#include <yggr/network/network_handler.hpp>

#include <yggr/network/session_state/session_state_fixer.hpp>
#include <yggr/network/session_state/session_state_checker.hpp>

#include <yggr/network/network_sender.hpp>

#include <yggr/client/client_config/client_tcp_config.hpp>
#include <yggr/client/start_mode/client_passive_tcp_start_mode.hpp>
#include <yggr/client/basic_clt_handler.hpp>
#include <yggr/client/client.hpp>

//#include <yggr/log/yggr_exception_log_accesser.hpp>
#include <yggr/log/default_log_accesser.hpp>

#include <yggr/script/lua_script.hpp>
#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <lua_cal_svr_procotol_impl.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_set.hpp>

#include <iostream>
#include <cassert>

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

typedef yggr::ids::id_generator<yggr::ids::uuid,
								yggr::ids::parse_uuid_genner::lagged_fibonacci607_type> task_id_gen_type;

typedef yggr::task_center::task<yggr::ids::uuid, network_info_type,
									yggr::task_center::default_task_data_info_type> task_type;

typedef yggr::task_center::support::task_data_info_parser<yggr::task_center::default_task_data_info_type> task_data_info_parser_type;

YGGR_PP_THREAD_ACTRION_HELPER_MAKE_REG_ID_PARSER(action_reg_string_parser,
													yggr::string,
													task_data_info_parser_type,
													class_name)

typedef yggr::thread::action_helper::action_reg_string_parser action_reg_id_parser_type;

typedef yggr::thread::action_helper::action_id_parser<action_reg_id_parser_type,
														task_type::class_name_getter> action_id_parser_type;

typedef yggr::thread::static_work_runner<task_type, action_id_parser_type> work_runner_type;

typedef work_runner_type::shared_info_ptr_type runner_shared_info_ptr_type;

typedef yggr::task_center::work_runner_saver<yggr::task_center::type_traits::mark_saver_condition,
												work_runner_type> cdt_saver_type;


typedef yggr::task_center::recver_handler_mgr<yggr::task_center::type_traits::mark_saver_result, task_type> rst_saver_type;

typedef yggr::task_center::task_creator<task_id_gen_type, task_type> task_creator_type;

typedef yggr::task_center::task_center<task_creator_type, cdt_saver_type, rst_saver_type> task_center_type;

//typedef yggr::adapter::adapter_id_parser_def
//		<
//			//Reg_Send_ID_Parser
//			yggr::task_center::task_data_send_typeid_getter
//			<
//				yggr::task_center::default_task_data_info_type
//			>,
//
//			//Reg_Recv_ID_Parser
//			yggr::task_center::task_data_recv_typeid_getter
//			<
//				yggr::task_center::default_task_data_info_type
//			>,
//
//			//Send_ID_Parser
//			YGGR_IDS_PARSER_CONST_MEM_FOO(net_ipak_type::base_type,
//											const yggr
//													::task_center
//														::default_task_data_info_type
//															::data_info_type&,
//											data_info),
//
//			//Task_Send_ID_Parser
//			YGGR_IDS_PARSER_CONST_MEM_FOO(task_type,
//											const yggr
//												::task_center
//													::default_task_data_info_type
//														::data_info_type&,
//											data_info),
//
//			//Task_Recv_ID_Parser
//			YGGR_IDS_PARSER_CONST_MEM_FOO(task_type,
//											yggr
//												::task_center
//													::default_task_data_info_type
//														::class_name_type,
//											class_name),
//
//			//Recv_ID_Parser
//			YGGR_IDS_PARSER_USE_TYPEID_NAME_CONV_RET(yggr::string)
//		> adapter_id_parser_def_type;

typedef 
	YGGR_ADAPTER_ID_PARSER_DEF(
		_,
		//Reg_Send_ID_Parser
		yggr::task_center::task_data_send_typeid_getter
		<
			yggr::task_center::default_task_data_info_type
		>,

		//Reg_Recv_ID_Parser
		yggr::task_center::task_data_recv_typeid_getter
		<
			yggr::task_center::default_task_data_info_type
		>,

		//Send_ID_Parser
		YGGR_IDS_PARSER_CONST_MEM_FOO(net_ipak_type::base_type,
										const yggr
												::task_center
													::default_task_data_info_type
														::data_info_type&,
										data_info),

		//Task_Send_ID_Parser
		YGGR_IDS_PARSER_CONST_MEM_FOO(task_type,
										const yggr
											::task_center
												::default_task_data_info_type
													::data_info_type&,
										data_info),

		//Task_Recv_ID_Parser
		YGGR_IDS_PARSER_CONST_MEM_FOO(task_type,
										yggr
											::task_center
												::default_task_data_info_type
													::class_name_type,
										class_name),

		//Recv_ID_Parser
		YGGR_IDS_PARSER_USE_TYPEID_NAME_CONV_RET(yggr::string)
	) adapter_id_parser_def_type;


typedef yggr::adapter::adapter_mgr<task_center_type,
									net_ipak_type,
									net_opak_type,
									adapter_id_parser_def_type,
									yggr::network::adapter_helper::io_converter> adapter_mgr_type;

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

typedef yggr::network::session<yggr::network::type_traits::tag_client,
								conn_type,
								adapter_mgr_type,
								packets_checker_type,
								packets_crypher_type,
								first_action_type,
								yggr::network::session_state::default_session_state_fixer_type
							> session_type;

typedef yggr::network::start_data::start_data_loader<test_pak_type> start_data_loader_type;

typedef yggr::network::session_helper::session_creator<test_pak_type, conn_type/*,
														yggr::network::session_helper::session_checker_creator,
														yggr::network::session_helper::session_crypher_creator*/
														> session_creator_type;

typedef yggr::client::start_mode::client_passive_tcp_start_mode<
																net_ipak_type,
																start_data_loader_type,
																session_creator_type
															> client_start_mode;

typedef yggr::network::session_helper::session_tcp_curer<session_type> session_curer_type;

typedef yggr::network::session_mgr<session_type,
									client_start_mode,
									yggr::network::session_state::default_session_state_checker_type,
									session_curer_type
									> session_mgr_type;

typedef yggr::network::service_handler<	session_mgr_type::service_type,
										boost::asio::signal_set,
										yggr::thread::boost_thread_config_type,
										std::allocator,
										std::vector,
										yggr::network::balance_io_service_selector
									> service_handler_type;


typedef yggr::client::basic_clt_handler<yggr::client::client_config::client_tcp_config_type,
										service_handler_type,
										session_mgr_type
									> tcp_clt_handler_type;

typedef yggr::network::network_handler<tcp_clt_handler_type> network_handler_type;

typedef yggr::client::client<network_handler_type> clt_type;

typedef yggr::ptr_single<clt_type> clt_ptr_single_type;
typedef clt_ptr_single_type::obj_ptr_type clt_ptr_type;

typedef yggr::network::network_sender<clt_type> clt_sender_type;

//-------------------------------------------------------------------

//static yggr::u32 send_count = 0;

// lua sprite_mgr
typedef yggr::script::script_mgr<yggr::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

typedef yggr::ptr_single<script_mgr_type> script_mgr_single_type;

typedef yggr::ptr_single<task_center_type> task_center_single_type;
typedef yggr::ptr_single<ctrl_center_type> ctrl_center_single_type;
typedef yggr::ptr_single<yggr::log::default_log_accesser> log_accesser_single_type;

typedef yggr::ptr_single<int> send_count_single_type;



#endif // __LUA_CLT_CONFIG_HPP__