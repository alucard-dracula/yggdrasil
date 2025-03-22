//config.hpp

#ifndef __ASYNC_TCP_SVR_CONFIG_HPP__
#define __ASYNC_TCP_SVR_CONFIG_HPP__

#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/exception/exception.hpp>
#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/system_controller/system_code.hpp>

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
#include <yggr/network/network_config/network_balance_udp_config.hpp> //use udp config
#include <yggr/network/start_data/pak_back_id.hpp>

#include <yggr/network/session_helper/first_action.hpp>
#include <yggr/network/session_helper/owner_generator.hpp>
#include <yggr/network/session_helper/session_creator.hpp>

#include <yggr/network/packets_support/packets_checker.hpp>
#include <yggr/network/packets_support/packets_crypher.hpp>

#include <yggr/network/adapter_helper/io_converter.hpp>
#include <yggr/network/support/data_info_def.hpp>
#include <yggr/network/network_info.hpp>
#include <yggr/network/network_packet.hpp>
#include <yggr/network/connection.hpp>
#include <yggr/network/ex_linker.hpp>

#include <yggr/network/session.hpp>
#include <yggr/network/session_mgr.hpp>
#include <yggr/network/session_helper/session_killer.hpp>
#include <yggr/network/service_handler.hpp>
#include <yggr/network/network_mixer_handler.hpp>

#include <yggr/network/session_state/session_state_fixer.hpp>
#include <yggr/network/session_state/session_state_checker.hpp>

#include <yggr/network/heart/heart_data.hpp>
#include <yggr/network/heart/heart_src_fixer.hpp>
#include <yggr/network/heart/heart_checker.hpp>
#include <yggr/network/heart/heart_timer_task_mixer_pak.hpp> // use heart_timer_task_mixer_pak

#include <yggr/time/timer_mgr.hpp>

#include <yggr/server/server_config/server_tcp_config.hpp>
#include <yggr/server/server_config/server_udp_config.hpp>
#include <yggr/server/start_mode/server_active_start_mode.hpp>
#include <yggr/server/basic_svr_handler.hpp>
#include <yggr/server/server.hpp>

#include <yggr/log/default_log_accesser.hpp>

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

#if 0
typedef yggr::adapter::adapter_id_parser_def
		<
			yggr::task_center::task_data_send_typeid_getter
			<
				yggr::task_center::default_task_data_info_type
			>,

			yggr::task_center::task_data_recv_typeid_getter
			<
				yggr::task_center::default_task_data_info_type
			>,

			YGGR_IDS_PARSER_CONST_MEM_FOO(net_ipak_type::base_type,
											const yggr
													::task_center
														::default_task_data_info_type
															::data_info_type&,
											data_info),

			YGGR_IDS_PARSER_CONST_MEM_FOO(task_type,
											const yggr
												::task_center
													::default_task_data_info_type
														::data_info_type&,
											data_info),

			YGGR_IDS_PARSER_CONST_MEM_FOO(task_type,
											yggr
												::task_center
													::default_task_data_info_type
														::class_name_type,
											class_name),

			YGGR_IDS_PARSER_USE_TYPEID_NAME_CONV_RET(yggr::string)
		> adapter_id_parser_def_type;

#else

namespace detail
{
typedef 
	yggr::task_center::task_data_send_typeid_getter
	<
		yggr::task_center::default_task_data_info_type
	> adapter_reg_send_id_parser_type;

typedef 
	yggr::task_center::task_data_recv_typeid_getter
	<
		yggr::task_center::default_task_data_info_type
	> adapter_reg_recv_id_parser_type;

typedef 
	YGGR_IDS_PARSER_CONST_MEM_FOO(
		net_ipak_type::base_type,
		const yggr
				::task_center
					::default_task_data_info_type
						::data_info_type&,
		data_info) adapter_send_id_parser_type;

typedef 
	YGGR_IDS_PARSER_CONST_MEM_FOO(
		task_type,
		const yggr
			::task_center
				::default_task_data_info_type
					::data_info_type&,
		data_info) adapter_task_send_id_parser_type;

typedef 
	YGGR_IDS_PARSER_CONST_MEM_FOO(
		task_type,
		yggr
			::task_center
				::default_task_data_info_type
					::class_name_type,
		class_name) adapter_task_recv_id_parser_type;

typedef YGGR_IDS_PARSER_USE_TYPEID_NAME_CONV_RET(yggr::string) adapter_recv_id_parser_type;
} // namespace detail

// using YGGR_ADAPTER_ID_PARSER_DEF can relieve bigobj
typedef
	YGGR_ADAPTER_ID_PARSER_DEF(
		0,
		detail::adapter_reg_send_id_parser_type,
		detail::adapter_reg_recv_id_parser_type,
		detail::adapter_send_id_parser_type,
		detail::adapter_task_send_id_parser_type,
		detail::adapter_task_recv_id_parser_type,
		detail::adapter_recv_id_parser_type
	) adapter_id_parser_def_type;

#endif // 0, 1


typedef yggr::adapter::adapter_mgr<task_center_type,
									net_ipak_type,
									net_opak_type,
									adapter_id_parser_def_type,
									yggr::network::adapter_helper::io_converter> adapter_mgr_type;

typedef yggr::network::ex_linker<
									yggr::network::network_config::balance_tcpv4_config
								> tcp_linker_type; // tcp linker

typedef yggr::network::ex_linker<
									yggr::network::network_config::balance_udpv4_config,
									yggr::network::udp_linker
								> udp_linker_type; // udp linker

typedef yggr::network::connection<
									tcp_linker_type,
									net_opak_type,
									net_ipak_type
								> tcp_conn_type; // tcp connection

typedef yggr::network::connection<udp_linker_type,
									net_opak_type,
									net_ipak_type,
									yggr::network::io::udp_async_strand_io
								> udp_conn_type; // udp connection


YGGR_PP_REGISTER_SESSION_ID_CREATOR_BEGIN(test_pak_type, test_pak, const yggr::ids::id64_type& ) // 定义会话ID的创建器类型
	test_pak.id()
YGGR_PP_REGISTER_SESSION_ID_CREATOR_END()

typedef yggr::network::packets_support::packets_checker<network_info_type> packets_checker_type; // 定义数据包检测类型
YGGR_PP_REGISTER_SESSION_CHECKER_CREATOR_BEGIN(test_pak_type, test_pak, packets_checker_type) // 定义数据包检测器的创建器类型
	test_pak.id()
YGGR_PP_REGISTER_SESSION_CHECKER_CREATOR_END()

typedef yggr::network::packets_support::packets_crypher<void> packets_crypher_type; // 定义数据包加密器类型（该例题使用的是 不加密的加密器类型）
YGGR_PP_REGISTER_SESSION_CRYPHER_CREATOR_BEGIN(test_pak_type, test_pak, packets_crypher_type) // 定义数据包加密器的创建器类型
YGGR_PP_REGISTER_SESSION_CRYPHER_CREATOR_END()

typedef yggr::network::session_helper::first_action first_action_type;

typedef yggr::network::heart::heart_data<> heart_data_type;

typedef yggr::network::session_state::session_state_fixer
		<
			yggr::network::session_state::session_state_def
			<
				yggr::network::session_state::tag_def::E_session_state_online
			>,
			yggr::network::heart::heart_src_fixer<heart_data_type>
		> udp_session_state_fixer_type;

typedef yggr::network::session<yggr::network::type_traits::tag_server,
								tcp_conn_type,
								adapter_mgr_type,
								packets_checker_type,
								packets_crypher_type,
								first_action_type,
								yggr::network::session_state::default_session_state_fixer_type
							> tcp_session_type; // tcp session

typedef yggr::network::session<yggr::network::type_traits::tag_server,
								udp_conn_type,
								adapter_mgr_type,
								packets_checker_type,
								packets_crypher_type,
								first_action_type,
								udp_session_state_fixer_type
							> udp_session_type; // udp session

typedef yggr::ids::pointer_id_generator<yggr::ids::id64_type> session_id_generator_type;
typedef yggr::network::start_data::start_data_generator<test_pak_type> start_data_generator_type;
typedef yggr::network::session_helper::owner_generator<network_info_type> owner_generator_type;

typedef yggr::network::session_helper::session_creator<test_pak_type, tcp_conn_type/*,
														yggr::network::session_helper::session_id_creator,
														yggr::network::session_helper::session_checker_creator,
														yggr::network::session_helper::session_crypher_creator*/
														> tcp_session_creator_type; // tcp session creator

typedef yggr::network::session_helper::session_creator<test_pak_type, udp_conn_type/*,
														yggr::network::session_helper::session_id_creator,
														yggr::network::session_helper::session_checker_creator,
														yggr::network::session_helper::session_crypher_creator*/
														> udp_session_creator_type; // udp session creator

typedef yggr::server::start_mode::server_active_start_mode<
															session_id_generator_type,
															start_data_generator_type,
															owner_generator_type,
															tcp_session_creator_type
														> tcp_server_start_mode; // tcp server start mode


typedef yggr::server::start_mode::server_active_start_mode<
															session_id_generator_type,
															start_data_generator_type,
															owner_generator_type,
															udp_session_creator_type
														> udp_server_start_mode; // udp server start mode

typedef yggr::network::session_helper::session_killer<tcp_session_type> tcp_session_killer_type; // tcp session killer

typedef yggr::network::session_helper::session_killer<udp_session_type> udp_session_killer_type; // udp session killer

typedef yggr::network::session_state::session_state_checker
		<
			yggr::network::session_state::session_state_def
			<
				yggr::network::session_state::tag_def::E_session_state_online
			>,
			yggr::network::heart::heart_checker
		> udp_session_state_checker_type;

typedef yggr::network::session_mgr<tcp_session_type,
									tcp_server_start_mode,
									yggr::network::session_state::default_session_state_checker_type,
									tcp_session_killer_type
									> tcp_session_mgr_type; // tcp session mgr


typedef yggr::network::session_mgr<udp_session_type,
									udp_server_start_mode,
									udp_session_state_checker_type,
									udp_session_killer_type
									> udp_session_mgr_type; // udp session mgr

BOOST_MPL_ASSERT((boost::is_same<udp_conn_type::service_type, tcp_conn_type::service_type>)); // check service type
typedef udp_conn_type::service_type network_service_type;


typedef yggr::network::service_handler<
											network_service_type,
											boost::asio::signal_set,
											yggr::thread::boost_thread_config_type,
											std::allocator,
											std::vector,
											yggr::network::balance_io_service_selector
										> service_handler_type;

typedef yggr::server::basic_svr_handler< yggr::server::server_config::server_tcp_config_type,
										service_handler_type,
										tcp_session_mgr_type
									> tcp_svr_handler_type;

typedef yggr::server::basic_svr_handler< yggr::server::server_config::server_udp_config_type,
										service_handler_type,
										udp_session_mgr_type,
										::yggr::network::default_options_setter_type,
										yggr::network::session_helper::udp_async_acceptor_handler
									> udp_svr_handler_type;

typedef yggr::network::network_mixer_handler //use network mixer handler
		<
			service_handler_type,
			adapter_mgr_type,
			boost::mutex,
			tcp_svr_handler_type,
			udp_svr_handler_type
		> network_handler_type;

typedef yggr::server::server<network_handler_type> svr_type;

typedef yggr::ptr_single<svr_type> svr_ptr_single_type;
typedef svr_ptr_single_type::obj_ptr_type svr_ptr_type;

typedef yggr::network::heart::heart_timer_task_mixer_pak
		<
			svr_ptr_type,
			udp_svr_handler_type::tag_type,
			udp_linker_type::protocol_tag_type
		> heart_timer_task_pak_type; // use heart_timer_task_mixer_pak

typedef yggr::ids::id64_type timer_task_id_type;
typedef yggr::time::timer_mgr<timer_task_id_type> timer_mgr_type;

typedef yggr::ptr_single<task_center_type> task_center_single;
typedef yggr::ptr_single<ctrl_center_type> ctrl_center_single;
typedef yggr::ptr_single<yggr::log::default_log_accesser> log_accesser_single;

#endif // __ASYNC_TCP_SVR_CONFIG_HPP__