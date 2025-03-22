//config.hpp

#ifndef __SAFE_PACKET_TOOL_TCP_CLT_CONFIG_HPP__
#define __SAFE_PACKET_TOOL_TCP_CLT_CONFIG_HPP__

#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/exception/exception.hpp>
#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/system_controller/system_code.hpp>

#include <yggr/ids/uuid.hpp>
#include <yggr/ids/base_ids_def.hpp>

#include <yggr/encryption_tool/blowfish_tool.hpp>
#include <yggr/encryption_tool/md5_tool.hpp>
#include <yggr/encryption_tool/safe_packet_tool.hpp>
#include <yggr/encryption_tool/packet_cypher_tool.hpp>

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

#include <yggr/network/start_data/encryption_pak_back_id.hpp>
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

#include <yggr/proxy/proxy_ctrl/default_proxy_msg_fixer.hpp>

#include <yggr/server/server_config/server_tcp_config.hpp>
#include <yggr/server/start_mode/server_active_start_mode.hpp>
#include <yggr/server/basic_svr_handler.hpp>
#include <yggr/server/server.hpp>

#include <yggr/log/default_log_accesser.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_set.hpp>

#include <iostream>
#include <cassert>

typedef yggr::encryption_tool::blowfish_tool<> blowfish_tool_type;
typedef yggr::encryption_tool::md5_tool md5_tool_type;
typedef yggr::encryption_tool::safe_packet_tool<blowfish_tool_type, md5_tool_type> safe_packet_tool_type;
typedef yggr::encryption_tool::packet_cypher_tool<safe_packet_tool_type> packet_cypher_tool_type;

typedef yggr::system_controller::ctrl_center ctrl_center_type;

typedef yggr::network::start_data::encryption_pak_back_id<yggr::ids::id64_type, yggr::string, 8> test_pak_type;

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

typedef 
	yggr::adapter::adapter_id_parser_def
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

typedef yggr::shared_ptr<proxy_adapter_mgr_type> proxy_adapter_mgr_ptr_type;

// proxy task_center and adapteer def end

typedef boost::unordered_set<network_info_type> owner_info_container_type;

typedef yggr::proxy::proxy_mode::proxy_mode_tactics proxy_mode_tactics_type;
typedef yggr::proxy::proxy_mode::proxy_mode_owner_id_store<owner_info_container_type> proxy_mode_data_store_type;

typedef 
	boost::mpl::vector
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
														//owner_info_container_type
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

typedef ::yggr::shared_ptr<proxy_router_type> proxy_router_ptr_type;

typedef yggr::proxy::proxy_repeater<
										proxy_runner_shared_info_ptr_type,
										proxy_task_type,
										owner_info_container_type,
										yggr::error,
										proxy_router_type
									> proxy_repeater_type;

typedef ::yggr::shared_ptr<proxy_repeater_type> proxy_repeater_ptr_type;

typedef proxy_repeater_type::args_builder_type proxy_args_builder_type;
typedef proxy_repeater_type::args_builder_ptr_type proxy_args_builder_ptr_type;

typedef ::yggr::shared_ptr<proxy_repeater_type> proxy_repeater_ptr_type;

typedef yggr::network::ex_linker<yggr::network::network_config::balance_tcpv4_config> linker_type;

typedef yggr::network::connection<linker_type, net_opak_type, net_ipak_type> conn_type;

YGGR_PP_REGISTER_SESSION_ID_CREATOR_BEGIN(test_pak_type, test_pak, const yggr::ids::id64_type& )
	test_pak.id()
YGGR_PP_REGISTER_SESSION_ID_CREATOR_END()

typedef yggr::network::packets_support::packets_checker<network_info_type, 1000> packets_checker_type;
YGGR_PP_REGISTER_SESSION_CHECKER_CREATOR_BEGIN(test_pak_type, test_pak, packets_checker_type)
	test_pak.id()
YGGR_PP_REGISTER_SESSION_CHECKER_CREATOR_END()

typedef yggr::network::packets_support::packets_crypher<packet_cypher_tool_type> packets_crypher_type;
YGGR_PP_REGISTER_SESSION_CRYPHER_CREATOR_BEGIN(test_pak_type, test_pak, packets_crypher_type)
	test_pak.key()
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


typedef yggr::proxy::proxy_ctrl::default_proxy_msg_fixer
		<
			owner_info_container_type,
			proxy_repeater_type,
			net_ipak_type,
			net_opak_type,
			proxy_adapter_mgr_type
		> base_proxy_msg_calculator_type;

typedef base_proxy_msg_calculator_type::proxy_register_msg_type proxy_register_msg_type;
typedef base_proxy_msg_calculator_type::proxy_register_back_msg_type proxy_register_back_msg_type;
typedef base_proxy_msg_calculator_type::proxy_mode_change_msg_type proxy_mode_change_msg_type;
typedef base_proxy_msg_calculator_type::proxy_mode_change_back_msg_type proxy_mode_change_back_msg_type;
typedef base_proxy_msg_calculator_type::proxy_unregister_msg_type proxy_unregister_msg_type;
typedef base_proxy_msg_calculator_type::proxy_unregister_back_msg_type proxy_unregister_back_msg_type;

typedef yggr::proxy::proxy_fix_state_def proxy_fix_state_def_type;

typedef yggr::ptr_single<proxy_task_center_type> proxy_task_center_single;
typedef yggr::ptr_single<ctrl_center_type> ctrl_center_single;
typedef yggr::ptr_single<yggr::log::default_log_accesser> log_accesser_single;

#endif // __SAFE_PACKET_TOOL_TCP_CLT_CONFIG_HPP__
