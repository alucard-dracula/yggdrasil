//lua_clt_test.cpp

#include <boost/system/system_error.hpp>
#include <iostream>

#include <yggr/network/network_packet.hpp>
#include <yggr/network/network_config/network_tcp_config.hpp>

#include <yggr/archive/net_oarchive.hpp>
#include <yggr/archive/net_iarchive.hpp>
#include <yggr/network/connection.hpp>

#include <yggr/network/tcp_linker.hpp>

#include <yggr/client/basic_clt_handler.hpp>
#include <yggr/client/client.hpp>

#include <yggr/archive/network_archive_partner.hpp>

#include <yggr/safe_container/safe_queue.hpp>

#include <yggr/task_center/recver_handler_mgr.hpp>
#include <yggr/task_center/task.hpp>
#include <yggr/task_center/auto_recv_data_center.hpp>
#include <yggr/task_center/val_saver.hpp>

#include <yggr/adapter/base_send_adapter.hpp>
#include <yggr/adapter/base_recv_adapter.hpp>
#include <yggr/adapter/adapter_mgr.hpp>

#include <yggr/network/pak_back_id.hpp>

#include <yggr/thread/dynamic_work_runner.hpp>
#include <yggr/thread/thread_mgr.hpp>
#include <yggr/thread/work_thread.hpp>
#include <yggr/thread/boost_thread_config.hpp>

#include <yggr/base/ctrl_center.hpp>
#include <boost/asio/error.hpp>

#include <signal.h>

#include <yggr/log/yggr_exception_log_accesser.hpp>

#include <boost/exception/exception.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <yggr/ids/base_ids_def.hpp>

#include <yggr/script/lua_script.hpp>
#include <yggr/script/lua_environment.hpp>
#include <yggr/script/script_mgr.hpp>

#include <cal_svr_procotol_impl.hpp>
#include <yggr/base/error_make.hpp>

#include <set>

#ifdef _MSC_VER
#   ifdef _DEBUG
#	    include <vld.h>
#   endif //_DEBUG
#endif // _MSC_VER

typedef yggr::system_controller::ctrl_center ctrl_center_type;

typedef yggr::network::network_packet<yggr::archive::archive_partner::network_oarchive_partner> opacket_type;
typedef yggr::network::network_packet<yggr::archive::archive_partner::network_iarchive_partner> ipacket_type;


typedef yggr::network::tcp_linker<yggr::network::network_config::tcpv4_config> linker_type;

typedef yggr::network::connection<linker_type, opacket_type, ipacket_type, true, false> conn_type; // strand_connect_test
//typedef yggr::network::connection<linker_type, opacket_type, ipacket_type, false, false> conn_type; // non_strand_connect_test
//typedef yggr::network::connection<linker_type, opacket_type, ipacket_type, false, true> conn_type; // sync_connect_test

typedef opacket_type::base_type opack;
typedef ipacket_type::base_type ipack;

typedef yggr::network::network_packet::pak_back_id<yggr::ids::id64_type> test_pak_type;

typedef yggr::task_center::task<opack::data_cmp_type,yggr::ids::id64_type> task_type;
typedef yggr::safe_container::safe_queue<task_type> task_container_type;
typedef yggr::task_center::val_saver<task_container_type> val_saver_type;

typedef yggr::task_center::auto_recv_data_center<val_saver_type, yggr::task_center::recver_handler_mgr> task_center_type;

typedef yggr::adapter::adapter_mgr<task_center_type, ipack, boost::any, opack,
									yggr::adapter::base_send_adapter,
									yggr::adapter::base_recv_adapter
									> adapter_mgr_type;


typedef yggr::client::basic_clt_handler<test_pak_type, adapter_mgr_type, conn_type>  tcp_clt_handler_type;
typedef yggr::client::client<tcp_clt_handler_type> clt_type;

typedef yggr::ptr_single<clt_type> clt_ptr_single_type;

typedef yggr::thread::dynamic_work_runner<task_center_type> work_runner_type;
typedef work_runner_type::shared_info_ptr_type runner_shared_info_ptr_type;

typedef yggr::thread::work_thread<yggr::thread::boost_thread_config_type> work_thread_type;
typedef yggr::thread::thread_mgr<work_thread_type> trd_mgr_type;

static yggr::u32 send_count = 0;

typedef yggr::script::script_mgr<std::string, yggr::script::lua::lua_script,
									yggr::script::lua::lua_environment> script_mgr_type;

typedef yggr::ptr_single<script_mgr_type> script_mgr_single_type;

struct Calculator : public boost::enable_shared_from_this<Calculator>
{
public:
	ERROR_MAKER_BEGIN("Calculator")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
			ERROR_CODE_DEF(E_code_test)
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_code_test, "E_code_test")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	typedef yggr::u64 user_id_type;

	typedef std::multiset<user_id_type> user_id_container_type;


#include <yggr/support/runner_condition_and_result.ipp>


	template<typename Runner, typename Action_Table, typename Recv_Handler>
	void register_cal_object(Action_Table& at, const Recv_Handler& handler)
	{
		typedef Runner runner_type;
		typedef Action_Table action_table_type;
		typedef Recv_Handler recv_handler_type;


		at.template register_calculator<rst_procotol_type, Recv_Handler>(handler,
															boost::bind(&Calculator::cal_rst_procotol_type<user_id_type, runner_type, recv_handler_type>,
																			shared_from_this(), _1, _2, _3, _4));

		at.template register_calculator<test_pak_type, Recv_Handler>(handler,
															boost::bind(&Calculator::cal_test_pak_type<user_id_type, runner_type, recv_handler_type>,
																			shared_from_this(), _1, _2, _3, _4));
	}


	template<typename User_ID, typename Runner, typename Handler>
	void cal_test_pak_type(const User_ID& uid, const test_pak_type& cdt, Runner* prunner, const Handler& handler)
	{
		std::cout << "cdt = " << cdt.id << std::endl;
		cdt_procotol_type rst;
		script_mgr_single_type::get_ins()->execute_rule("create_num", rst);

		user_id_container_type uids;
		uids.insert(uid);
		handler(prunner, error(0), static_cast<yggr::u32>(E_CAL_RESULT), rst.data_info(), uids, rst);
		return;
	}

	template<typename User_ID, typename Runner, typename Handler>
	void cal_rst_procotol_type(const User_ID& uid, const rst_procotol_type& cdt, Runner* prunner, const Handler& handler)
	{
		static int count = 0;
		static int i = 0;
		if(i != send_count)
		{
			++i;
			std::cout << "-------------calculate " << i << "-----------------------"<< std::endl;
			std::cout << "rst = " << cdt.rst << std::endl;

			cdt_procotol_type rst;
			script_mgr_single_type::get_ins()->execute_rule("create_num", rst);

			user_id_container_type uids;
			uids.insert(uid);
			handler(prunner, error(0), static_cast<yggr::u32>(E_CAL_RESULT), rst.data_info(), uids, rst);
		}

		return;
	}
};

//void sign_print(int i)
//{
//    //std::cout << i << std::endl;
//    yggr::ptr_single<yggr::exception::exception>::obj_ptr_type pe = yggr::ptr_single<yggr::exception::exception>::get_ins();
//    if(pe)
//    {
//        //pe->post(i);
//        std::cout << i << std::endl;
//    }
//}

int main(int argc, char* argv[])
{
//    for(int i = 1; i != 138; ++i)
//    {
//        signal(i, sign_print);
//    }
//    //signal();
	typedef yggr::ptr_single<task_center_type> data_center_single;
	typedef yggr::ptr_single<ctrl_center_type> ctrl_center_single;
	typedef yggr::ptr_single<yggr::log::yggr_exception_log_accesser_type> log_acc_single;


	ctrl_center_single::init_ins();
	yggr::log::yggr_exception_log_accesser_type::init_type initlog;
	log_acc_single::init_ins(initlog);
	log_acc_single::obj_ptr_type plog = log_acc_single::get_ins();

	yggr::ptr_single<yggr::exception::exception>::init_ins();


	initlog.push_back("ccc.txt");
	initlog.push_back("ddd.txt");


	if(plog)
	{
		plog->register_msg(10061, initlog.front());
		plog->register_msg(10054, initlog.back());
	}

	std::cout << "please input num" << std::endl;
	std::cin >> send_count;

	//ctrl_center_single::init_ins();

	yggr::ptr_single<yggr::exception::exception>::obj_ptr_type pe = yggr::ptr_single<yggr::exception::exception>::get_ins();

    trd_mgr_type trd_mgr;

	data_center_single::init_ins();

	script_mgr_single_type::init_ins();
	script_mgr_single_type::get_ins()->append_module(script_mgr_type::E_lib_all);
	script_mgr_single_type::get_ins()->insert("create_num", "create_cdt", script_mgr_type::file_code_type("create_cdt.lua"));

	try
	{
		std::string str_socket("127.0.0.1 : 9000");
		//std::cin >> ip;
		//clt_type::init_type clt_init(str_socket);

		clt_ptr_single_type::init_ins((clt_type::init_type(str_socket, *(data_center_single::get_ins()))));
		clt_type& clt = *(clt_ptr_single_type::get_ins());

		runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info(2));

		boost::shared_ptr<Calculator> my_cal(new Calculator());
		shared_info_ptr->register_calculator(*my_cal);

		ctrl_center_single::get_ins()->register_controller(clt);

		clt.register_network_protocol<test_pak_type>();
		clt.register_network_protocol<cdt_procotol_type>();
		clt.register_network_protocol<rst_procotol_type>();


		trd_mgr
			.add_thread()
            .add_thread()
			.add_thread()
			.add_thread();

		trd_mgr.startup_all<work_runner_type>(work_runner_type::init_type(*(data_center_single::get_ins()), shared_info_ptr));

		clt.run();

		char cc = 0;
		std::cin >> cc;

		trd_mgr.stop_all();
		clt.stop();
		yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();

		ctrl_center_single::uninstall();
		clt_ptr_single_type::uninstall();
		data_center_single::uninstall();

	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const boost::system::system_error& e)
	{
		std::cerr << e.what() << std::endl;
	}

	yggr::ptr_single<yggr::exception::exception>::uninstall();
	log_acc_single::uninstall();

	return 0;
}
