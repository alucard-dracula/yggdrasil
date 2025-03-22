//config.hpp

#ifndef __STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_SVR_CONFIG_HPP__
#define __STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_SVR_CONFIG_HPP__

#include <yggr/base/yggrdef.h>
#include <iostream>

#if defined(_MSC_VER) || defined(YGGR_AT_LINUX) \
	|| defined(YGGR_AT_ANDROID) \
	|| defined(YGGR_AT_DARWIN)

#if !defined(YGGR_USE_SEH)
#	define YGGR_USE_SEH
#endif // YGGR_USE_SEH

#if defined(_DEBUG) && !defined(YGGR_INCLUDE_SEH_DEBUG)
#	define YGGR_INCLUDE_SEH_DEBUG
#endif // #defined(_DEBUG) && !defined(YGGR_INCLUDE_SEH_DEBUG)

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
#include <yggr/network/session_helper/session_killer.hpp>
#include <yggr/network/service_handler.hpp>
#include <yggr/network/network_handler.hpp>

#include <yggr/network/session_state/session_state_fixer.hpp>
#include <yggr/network/session_state/session_state_checker.hpp>

#include <yggr/server/server_config/server_tcp_config.hpp>
#include <yggr/server/start_mode/server_active_start_mode.hpp>
#include <yggr/server/basic_svr_handler.hpp>
#include <yggr/server/server.hpp>

#include <yggr/log/default_log_accesser.hpp>
#include <yggr/seh/seh.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_set.hpp>

#include <cassert>



typedef yggr::system_controller::ctrl_center ctrl_center_type; //��������������� �������������������������쳣�¼��ģ�

typedef yggr::network::start_data::pak_back_id<yggr::ids::id64_type> test_pak_type; // ��������Խӵĵ�һ�����ݰ����ͣ������Ⲣ�����������echo��

typedef yggr::network::network_info<yggr::ids::id64_type> network_info_type; // ����������Ϣ���ͣ�������Ϣ������ʶ���ݰ����ݰ������ĸ��û��ģ�
typedef yggr::network::support::network_data_info_def<yggr::u32, yggr::u16> data_info_def_type; // �����������ݵ�������Ϣ�����ṹ��U32�����ݰ�ID, u16�ǰ汾��

typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_oarchive_partner,
				data_info_def_type,
				network_info_type
			> net_opak_type; // �����������������

typedef yggr::network::network_packet<
				yggr::archive::archive_partner::network_iarchive_partner,
				data_info_def_type,
				network_info_type
			> net_ipak_type; // �����������������

typedef yggr::ids::id_generator<yggr::ids::uuid,
								yggr::ids::parse_uuid_genner::lagged_fibonacci607_type> task_id_gen_type; // ��������ID���������ͣ�������UUID����������

typedef yggr::task_center::task<yggr::ids::uuid, network_info_type, 
									yggr::task_center::default_task_data_info_type> task_type; // ����������������

typedef yggr::task_center::support::task_data_info_parser<yggr::task_center::default_task_data_info_type> task_data_info_parser_type;

YGGR_PP_THREAD_ACTRION_HELPER_MAKE_REG_ID_PARSER(action_reg_string_parser, 
													yggr::string, 
													task_data_info_parser_type,
													class_name) // �����������ݵ�ע��ID����������

typedef yggr::thread::action_helper::action_reg_string_parser action_reg_id_parser_type; // �����������ݵ�ע��ID����������

typedef yggr::thread::action_helper::action_id_parser<action_reg_id_parser_type, 
														task_type::class_name_getter> action_id_parser_type; // �����������ݵĽ��������ͣ�����ע��ID���� �� �ַ�������

typedef yggr::thread::static_work_runner<task_type, action_id_parser_type> work_runner_type; // ���幤�������� ��YGGR����� ÿ���߳������������ɹ�������������
																							// �����������������ȡ���� Ȼ�����ע��ĺ�������������

typedef work_runner_type::shared_info_ptr_type runner_shared_info_ptr_type; // ���幤���߹�����Ϣ����

typedef yggr::task_center::work_runner_saver<yggr::task_center::type_traits::mark_saver_condition, 
												work_runner_type> cdt_saver_type; // �����������ݵ������洢����YGGR��Ϊ���еļ��㶼��һ������������Ĺ��̣�

typedef yggr::task_center::recver_handler_mgr<yggr::task_center::type_traits::mark_saver_result, task_type> rst_saver_type; // �����������ݵĽ���洢������

typedef yggr::task_center::task_creator<task_id_gen_type, task_type> task_creator_type; // �������񴴽�������

typedef yggr::task_center::task_center<task_creator_type, cdt_saver_type, rst_saver_type> task_center_type; // ����������������
																											//��YGGR�����е����񶼷ŵ���������ͳһ������

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
		> adapter_id_parser_def_type; // ������������ID��������


typedef yggr::adapter::adapter_mgr<task_center_type, 
									net_ipak_type, 
									net_opak_type, 
									adapter_id_parser_def_type, 
									yggr::network::adapter_helper::io_converter> adapter_mgr_type; // ��������������
																									//���������������ǽ���������ת������������, Ȼ������������ģ�

typedef yggr::network::ex_linker<yggr::network::network_config::balance_tcpv4_config> linker_type; //��������ʵ�����������ͣ�������TCP IP V4��ʽ��

typedef yggr::network::connection<linker_type, net_opak_type, net_ipak_type> conn_type; // ��������������

YGGR_PP_REGISTER_SESSION_ID_CREATOR_BEGIN(test_pak_type, test_pak, const yggr::ids::id64_type& ) // ����ỰID�Ĵ���������
	test_pak.id()
YGGR_PP_REGISTER_SESSION_ID_CREATOR_END()

typedef yggr::network::packets_support::packets_checker<network_info_type> packets_checker_type; // ������������
YGGR_PP_REGISTER_SESSION_CHECKER_CREATOR_BEGIN(test_pak_type, test_pak, packets_checker_type) // ����Ự����������
	test_pak.id()
YGGR_PP_REGISTER_SESSION_CHECKER_CREATOR_END()

typedef yggr::network::packets_support::packets_crypher<void> packets_crypher_type; // �������ݰ����������ͣ�������ʹ�õ��� �����ܵļ��������ͣ�
YGGR_PP_REGISTER_SESSION_CRYPHER_CREATOR_BEGIN(test_pak_type, test_pak, packets_crypher_type) // ����Ự����������
YGGR_PP_REGISTER_SESSION_CRYPHER_CREATOR_END()

typedef yggr::network::session_helper::first_action first_action_type; //����Ự������ĵ�һ����������

typedef yggr::network::session<yggr::network::type_traits::tag_server,
								conn_type, 
								adapter_mgr_type, 
								packets_checker_type,
								packets_crypher_type,
								first_action_type,
								yggr::network::session_state::default_session_state_fixer_type
							> session_type; // ����Ự����

typedef yggr::ids::pointer_id_generator<yggr::ids::id64_type> session_id_generator_type; // ����ỰID���������ͣ�������ֱ��ʹ�ûỰ�Ķ����ַ��Ϊ�ỰID��
typedef yggr::network::start_data::start_data_generator<test_pak_type> start_data_generator_type; // ���忪ʼ���Ĵ���������
typedef yggr::network::session_helper::owner_generator<network_info_type> owner_generator_type; // ����ӵ���ߴ���������
typedef yggr::network::session_helper::session_creator<test_pak_type, conn_type/*,
														yggr::network::session_helper::session_checker_creator,
														yggr::network::session_helper::session_crypher_creator*/
														> session_creator_type; // ����Ự����������


typedef yggr::server::start_mode::server_active_start_mode<
															session_id_generator_type, 
															start_data_generator_type,
															owner_generator_type,
															session_creator_type
														> server_start_mode; // �����������ʼģʽ
																			//����ʼģʽ�������ӳɹ���ĵ�һ������,  
																			//   ����ʹ�÷��������������ģʽ, 
																			//  ��ǰYGGR ��û��֧�ֿͻ�������������ģʽ��


typedef yggr::network::session_helper::session_killer<session_type> session_killer_type; // ����Ự����������
																							//������������ڵ������쳣ʱ,  �Ự������ʲô������
																							// ��killer�����ٵ�����Ự��
typedef yggr::network::session_mgr<session_type, 
									server_start_mode,
									yggr::network::session_state::default_session_state_checker_type,
									session_killer_type
									> session_mgr_type; // ����Ự����������

typedef yggr::network::service_handler<
											session_mgr_type::service_type,
											boost::asio::signal_set,
											yggr::thread::boost_thread_config_type,
											std::allocator,
											std::vector,
											yggr::network::balance_io_service_selector
										> service_handler_type; // �������ִ��������

typedef yggr::server::basic_svr_handler< yggr::server::server_config::server_tcp_config_type,
										service_handler_type,
										session_mgr_type
									> tcp_svr_handler_type; // ����TCP ������ִ��������

typedef yggr::network::network_handler<tcp_svr_handler_type> network_handler_type; // ��������ִ��������

typedef yggr::server::server<network_handler_type> svr_type; // �������������

typedef yggr::ptr_single<svr_type> svr_ptr_single_type; // �����������������
typedef svr_ptr_single_type::obj_ptr_type svr_ptr_type; // ���������ָ������

typedef yggr::ptr_single<task_center_type> task_center_single; //�����������ĵĵ�������
typedef yggr::ptr_single<ctrl_center_type> ctrl_center_single; // ����������ĵĵ�������
typedef yggr::ptr_single<yggr::log::default_log_accesser> log_accesser_single; // ������־�������ĵ�������

#endif // #if defined(_MSC_VER) || defined(YGGR_AT_LINUX)

#endif // __STATIC_TASK_CENTER_BALANCE_ASYNC_TCP_SVR_CONFIG_HPP__