//config.hpp

#ifndef __SVR_TEST_SERVICE_HANDLER_SARED_WRAP_TEST_CONFIG_HPP__
#define __SVR_TEST_SERVICE_HANDLER_SARED_WRAP_TEST_CONFIG_HPP__

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
#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/network/network_config/network_balance_tcp_config.hpp>
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
#include <yggr/network/network_config/network_tcp_config.hpp>
#include <yggr/network/connection.hpp>
#include <yggr/network/ex_linker.hpp>

#include <yggr/network/session.hpp>
#include <yggr/network/session_mgr.hpp>
#include <yggr/network/session_helper/session_killer.hpp>
#include <yggr/network/service_handler.hpp>
#include <yggr/network/service_handler_shared_wrap.hpp>
#include <yggr/network/network_handler.hpp>

#include <yggr/network/session_state/session_state_fixer.hpp>
#include <yggr/network/session_state/session_state_checker.hpp>

#include <yggr/server/server_config/server_tcp_config.hpp>
#include <yggr/server/start_mode/server_active_start_mode.hpp>
#include <yggr/server/basic_svr_handler.hpp>
#include <yggr/server/server.hpp>

#include <yggr/log/default_log_accesser.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_set.hpp>

#include <iostream>
#include <cassert>

typedef yggr::system_controller::ctrl_center ctrl_center_type; //定义控制中心类型 （控制中心是用来处理各种异常事件的）

typedef yggr::network::start_data::pak_back_id<yggr::ids::id64_type> test_pak_type; // 定义网络对接的第一个数据包类型（此例题并用这个包测试echo）

typedef yggr::network::network_info<yggr::ids::id64_type> network_info_type; // 定义网络信息类型（网络信息用来标识数据包数据包属于哪个用户的）
typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type; // 定义网络数据的类型信息描述结构（U32是数据包ID, u16是版本）

typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type,
				network_info_type
			> net_opak_type; // 定义网络输出包类型

typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type,
				network_info_type
			> net_ipak_type; // 定义网络输入包类型

typedef yggr::ids::id_generator<yggr::ids::uuid,
								yggr::ids::parse_uuid_genner::lagged_fibonacci607_type> task_id_gen_type; // 定义任务ID生成器类型（这里是UUID的生成器）

typedef yggr::task_center::task<yggr::ids::uuid, network_info_type,
									yggr::task_center::default_task_data_info_type> task_type; // 定义任务中心类型

typedef yggr::task_center::support::task_data_info_parser<yggr::task_center::default_task_data_info_type> task_data_info_parser_type;

YGGR_PP_THREAD_ACTRION_HELPER_MAKE_REG_ID_PARSER(action_reg_string_parser,
													yggr::string,
													task_data_info_parser_type,
													class_name) // 生成任务数据的注册ID解析器类型

typedef yggr::thread::action_helper::action_reg_string_parser action_reg_id_parser_type; // 定义任务数据的注册ID解析器类型

typedef yggr::thread::action_helper::action_id_parser<action_reg_id_parser_type,
														task_type::class_name_getter> action_id_parser_type; // 定义任务数据的解析器类型（包括注册ID解析 和 分发解析）

typedef yggr::thread::static_work_runner<task_type, action_id_parser_type> work_runner_type; // 定义工作者类型 （YGGR的设计 每个线程能做的事情由工作者来处理）
																							// 工作者做的事情就是取数据 然后根据注册的函数来处理数据

typedef work_runner_type::shared_info_ptr_type runner_shared_info_ptr_type; // 定义工作者共享信息类型

typedef yggr::task_center::work_runner_saver<yggr::task_center::type_traits::mark_saver_condition,
												work_runner_type> cdt_saver_type; // 定义任务数据的条件存储器（YGGR认为所有的计算都是一个条件到结果的过程）


typedef yggr::task_center::recver_handler_mgr<yggr::task_center::type_traits::mark_saver_result, task_type> rst_saver_type; // 定义任务数据的结果存储器类型

typedef yggr::task_center::task_creator<task_id_gen_type, task_type> task_creator_type; // 定义任务创建器类型

typedef yggr::task_center::task_center<task_creator_type, cdt_saver_type, rst_saver_type> task_center_type; // 定义任务中心类型
																											//（YGGR将所有的任务都放到任务中心统一管理）

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
		> adapter_id_parser_def_type; // 定义适配器的ID解析类型


typedef yggr::adapter::adapter_mgr<task_center_type,
									net_ipak_type,
									net_opak_type,
									adapter_id_parser_def_type,
									yggr::network::adapter_helper::io_converter> adapter_mgr_type; // 定义适配器类型
																									//（适配器的作用是将网络数据转换成任务数据, 然后放入任务中心）

typedef yggr::network::ex_linker<yggr::network::network_config::balance_tcpv4_config> linker_type; //定义网络实体连接器类型（这里是TCP IP V4方式）

typedef yggr::network::connection<linker_type, net_opak_type, net_ipak_type> conn_type; // 定义网络连接器

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

typedef yggr::network::session_helper::first_action first_action_type; //定义会话创建后的第一个操作类型

typedef yggr::network::session<yggr::network::type_traits::tag_server,
								conn_type,
								adapter_mgr_type,
								packets_checker_type,
								packets_crypher_type,
								first_action_type,
								yggr::network::session_state::default_session_state_fixer_type
							> session_type; // 定义会话类型

typedef yggr::ids::pointer_id_generator<yggr::ids::id64_type> session_id_generator_type; // 定义会话ID生成器类型（该例题直接使用会话的对象地址作为会话ID）
typedef yggr::network::start_data::start_data_generator<test_pak_type> start_data_generator_type; // 定义开始包的创建器类型
typedef yggr::network::session_helper::owner_generator<network_info_type> owner_generator_type; // 定义拥有者创建器类型
typedef yggr::network::session_helper::session_creator<test_pak_type, conn_type/*,
														yggr::network::session_helper::session_id_creator,
														yggr::network::session_helper::session_checker_creator,
														yggr::network::session_helper::session_crypher_creator*/
														> session_creator_type; // 定义会话创建器类型


typedef yggr::server::start_mode::server_active_start_mode<
															session_id_generator_type,
															start_data_generator_type,
															owner_generator_type,
															session_creator_type
														> server_start_mode; // 定义服务器开始模式
																			//（开始模式代表连接成功后的第一个动作, 
																			//   这里使用服务端主动发包的模式, 
																			//  当前YGGR 还没有支持客户端主动发包的模式）


typedef yggr::network::session_helper::session_killer<session_type> session_killer_type; // 定义会话销毁器类型
																							//（这个类型用于当网络异常时,  会话管理做什么操作）
																							// （killer代表毁掉这个会话）
typedef yggr::network::session_mgr<session_type,
									server_start_mode,
									yggr::network::session_state::default_session_state_checker_type,
									session_killer_type
									> session_mgr_type; // 定义会话管理器类型

typedef yggr::network::service_handler<
											session_mgr_type::service_type,
											boost::asio::signal_set,
											yggr::thread::boost_thread_config_type,
											std::allocator,
											std::vector,
											yggr::network::balance_io_service_selector
										> service_handler_type; // 定义服务执行器类型

typedef yggr::network::service_handler_shared_wrap<service_handler_type> service_handler_shared_wrap_type;
typedef service_handler_shared_wrap_type::service_handler_ptr_type service_handler_ptr_type;

typedef yggr::server::basic_svr_handler< yggr::server::server_config::server_tcp_config_type,
										service_handler_shared_wrap_type, //use wrap type
										session_mgr_type
									> tcp_svr_handler_type; // 定义TCP 服务器执行器类型

typedef yggr::network::network_handler<tcp_svr_handler_type> network_handler_type; // 定义网络执行器类型

typedef yggr::server::server<network_handler_type> svr_type; // 定义服务器类型

typedef yggr::ptr_single<svr_type> svr_ptr_single_type; // 定义服务器单例类型
typedef svr_ptr_single_type::obj_ptr_type svr_ptr_type; // 定义服务器指针类型

typedef yggr::network::network_direct_def_type network_direct_def_type; // 数据流的流向类型

typedef yggr::ptr_single<task_center_type> task_center_single_type; //定义任务中心的单例类型
typedef yggr::ptr_single<ctrl_center_type> ctrl_center_single_type; // 定义控制中心的单例类型
typedef yggr::ptr_single<yggr::log::default_log_accesser> log_accesser_single_type; // 定义日志访问器的单例类型

#endif // __SVR_TEST_SERVICE_HANDLER_SARED_WRAP_TEST_CONFIG_HPP__