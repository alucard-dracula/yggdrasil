//lua_clt_test.cpp

#include "config.hpp"
#include "calculator.hpp"
#include "handler_reg.hpp"
#include "protocol_reg.hpp"
#include "init_client.hpp"

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(script_lua)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{

#if defined(__MINGW32__)
    HMODULE hmod = LoadLibrary("lua_cal_svr_procotol.dll");
    if(!hmod)
    {
        std::cerr << "load lua_cal_svr_procotol.dll faied" << std::endl;
        return -1;
    }
#endif // #if defined(__MINGW32__)

	int send_count = 10;

	std::cout << "please input num" << std::endl;
	std::cin >> send_count;

	send_count_single_type::init_ins(send_count);

	//yggr::log::default_log_accesser::init_type log_init; //memd to BOOST_PP_LOCAL_MACRO
	//log_init.push_back("clt_nt_log_eins");
	//log_init.push_back("clt_nt_log_zwei");

	//log_accesser_single_type::init_ins(log_init);

	//yggr::log::default_log_accesser::op_caller_type dc1("clt_nt_log_eins");
	//yggr::log::default_log_accesser::op_caller_type dc2("clt_nt_log_zwei");
	//log_accesser_single_type::get_ins()->register_msg(boost::asio::error::connection_refused, dc1);
	//log_accesser_single_type::get_ins()->register_msg(boost::asio::error::not_connected, dc2);

	init_log();

	ctrl_center_single_type::init_ins();
	yggr::ptr_single<yggr::exception::exception>::init_ins();

	runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info());

	if(!shared_info_ptr)
	{
		return -1;
	}

	::yggr::shared_ptr<Calculator> my_cal(new Calculator());
	reg_calculator(shared_info_ptr, my_cal);

	task_center_single_type::init_ins(shared_info_ptr, task_center_type::rst_saver_init_type());

	//clt_ptr_single_type::init_ins(1,
	//								tcp_clt_handler_type::init_type(*(task_center_single_type::get_ins()),
	//																	tcp_clt_handler_type::link_handler_init_type()));
	init_client(1);

	clt_ptr_type pclt = clt_ptr_single_type::get_ins();
	reg_network_protocol(pclt);

	ctrl_center_single_type::obj_ptr_type pctrl = ctrl_center_single_type::get_ins();

	if(pctrl)
	{
		reg_ctrl_center(pctrl, pclt, my_cal);
	}

	// lua script_mgr
//	script_mgr_single_type::init_ins();
//	script_mgr_single_type::get_ins()->append_module(script_mgr_type::E_lib_all);
//	script_mgr_single_type::get_ins()->insert("create_num", "create_cdt", script_mgr_type::file_code_type("create_cdt.lua"), 0);
	load_lua();

	std::string ip("127.0.0.1");
	std::string port("9000");
	try
	{
		//pclt->run();
		//pclt->op_handler().connect(ip, port);
		run_client(pclt, ip, port);
		std::cout << "started" << std::endl;

#	ifdef _DEBUG
		char cc = 0;
		std::cin >> cc;
#	else
		pclt->join();
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

	//shared_info_ptr->unregister_calculator(*my_cal);
	unreg_calculator(shared_info_ptr, my_cal);
	{
	    runner_shared_info_ptr_type tmp;
		shared_info_ptr.swap(tmp);
	}

	//pclt->stop();
	//pclt->clear();
//	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
//	{
//	    clt_ptr_type tmp;
//		pclt.swap(tmp);
//	}

	stop_client(pclt);

	ctrl_center_single_type::uninstall();
	clt_ptr_single_type::uninstall();
	task_center_single_type::uninstall();
	script_mgr_single_type::uninstall();

	yggr::ptr_single<yggr::exception::exception>::uninstall();
	send_count_single_type::uninstall();

	std::cout << "stop end" << std::endl;

	return 0;
}

