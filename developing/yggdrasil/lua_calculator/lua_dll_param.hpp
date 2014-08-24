//dll_param.hpp

#ifndef __DLL_PARAM_HPP__
#define __DLL_PARAM_HPP__

#include <yggr/network/network_config/network_balance_tcp_config.hpp>
#include <yggr/network/network_packet.hpp>

#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/task_center/recver_handler_mgr.hpp>
#include <yggr/task_center/task.hpp>
#include <yggr/task_center/direct_data_center.hpp>

#include <yggr/thread/dynamic_work_runner.hpp>

#include <yggr/base/ctrl_center.hpp>
#include <yggr/base/system_code.hpp>
#include <boost/asio/error.hpp>

#include <yggr/network/connection.hpp>
#include <yggr/network/ex_linker.hpp>

#include <yggr/server/session.hpp>
#include <yggr/server/session_mgr.hpp>
#include <yggr/server/tcp_balance_svr_handler.hpp>
#include <yggr/server/server.hpp>

#include <yggr/adapter/base_send_adapter.hpp>
#include <yggr/adapter/base_recv_adapter.hpp>
#include <yggr/adapter/adapter_mgr.hpp>

#include <yggr/network/pak_back_id.hpp>
#include <yggr/base/exception.hpp>

#include <yggr/ids/base_ids_def.hpp>

typedef yggr::system_controller::ctrl_center ctrl_center_type;

typedef yggr::network::network_packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::network::network_packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;

typedef opacket_type::base_type opack;
typedef ipacket_type::base_type ipack;

typedef yggr::task_center::task<opack::data_cmp_type, yggr::ids::id64_type> task_type;
typedef yggr::task_center::direct_data_center<task_type, yggr::task_center::recver_handler_mgr> task_center_type;

typedef task_center_type::work_runner_type work_runner_type;
typedef task_center_type::init_type data_center_init_type;
typedef work_runner_type::shared_info_ptr_type runner_shared_info_ptr_type;

typedef yggr::network::network_packet::pak_back_id<yggr::ids::id64_type> test_pak_type;

typedef yggr::adapter::adapter_mgr<task_center_type, ipack, boost::any, opack,
									yggr::adapter::base_send_adapter,
									yggr::adapter::base_recv_adapter
									> adapter_mgr_type;

typedef yggr::network::ex_linker<yggr::network::network_config::balance_tcpv4_config> linker_type;

typedef yggr::network::connection<linker_type, opacket_type, ipacket_type> conn_type;

typedef yggr::server::session<conn_type, adapter_mgr_type> session_type;
typedef yggr::server::session_mgr<session_type> session_mgr_type;
typedef yggr::server::tcp_balance_svr_handler<test_pak_type, session_mgr_type> tcp_handler_type;
typedef yggr::server::server<tcp_handler_type> svr_type;

typedef yggr::ptr_single<svr_type> svr_ptr_single_type;
typedef yggr::ptr_single<yggr::exception::exception> exception_ptr_single_type;

#endif //__DLL_PARAM_HPP__
