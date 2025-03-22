//async_svr_test.cpp

#include "config.hpp"
#include "calculator.hpp"
#include "handler_reg.hpp"
#include "protocol_reg.hpp"
#include "init_server.hpp"

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

int main(int argc, char* argv[])
{
	yggr::u32 port = 9000;
	
	if(!(argc < 2) && (argv[1] == yggr::string("--input-port")))
	{
		std::cout << "please input port:" << std::endl;
		std::cin >> port;
	}

	log_accesser_single_type::init_ins("svr_nt_log");

	yggr::log::default_log_accesser::op_caller_type caller("svr_nt_log");
	log_accesser_single_type::get_ins()->register_msg(10053, caller);

	ctrl_center_single_type::init_ins();
	yggr::ptr_single<yggr::exception::exception>::init_ins();

	runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info(2));

	if(!shared_info_ptr)
	{
		return -1;
	}

	task_center_single_type::init_ins();

	if(!init_server(4, port))
	{
		return -1;
	}

	//svr_ptr_single_type::init_ins(
	//	4,
	//	tcp_svr_handler_type::init_type(
	//		port,
	//		*(task_center_single_type::get_ins()),
	//		tcp_svr_handler_type::link_handler_init_type() ) );

	svr_ptr_type psvr = svr_ptr_single_type::get_ins();

	boost::shared_ptr<Calculator> my_cal(new Calculator());
	reg_calculator(shared_info_ptr, my_cal);

	reg_network_protocol(psvr);

	ctrl_center_single_type::obj_ptr_type pctrl = ctrl_center_single_type::get_ins();

	if(pctrl)
	{
		reg_ctrl_center(pctrl, psvr, my_cal);
	}

	{
	    ctrl_center_single_type::obj_ptr_type tmp;
		pctrl.swap(tmp);
	}

	trd_mgr_type trd_mgr;

	trd_mgr
		.add_thread()
		.add_thread()
		.add_thread()
		.add_thread();

	try
	{
		trd_mgr.startup_all<work_runner_type>(*(task_center_single_type::get_ins()), shared_info_ptr);
		
		//psvr->run();
		run_server(psvr);
		std::cout << "started" << std::endl;

#	ifdef _DEBUG
		char cc = 0;
		std::cin >> cc;
#	else
		psvr->join();
		//trd_mgr.join_all();
		//yggr::ptr_single<yggr::exception::exception>::get_ins()->join(); // linux exit hang
#	endif //_DEBUG

	}
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
	}
	catch(const yggr::stl_exception& e)
	{
        std::cerr << e.what() << std::endl;
	}

	//shared_info_ptr->unregister_calculator(*my_cal);
	unreg_calculator(shared_info_ptr, my_cal);
	{
	    runner_shared_info_ptr_type tmp;
		shared_info_ptr.swap(tmp);
	}

	trd_mgr.stop_all();
	trd_mgr.join_all();
	psvr->stop();
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	{
	    svr_ptr_type tmp;
		psvr.swap(tmp);
	}
	ctrl_center_single_type::uninstall();
	svr_ptr_single_type::uninstall();
	task_center_single_type::uninstall();
	yggr::ptr_single<yggr::exception::exception>::uninstall();

	std::cout << "stop end" << std::endl;

	return 0;
}
