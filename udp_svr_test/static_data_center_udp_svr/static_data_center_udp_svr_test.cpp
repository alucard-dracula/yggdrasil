//static_data_center_udp_svr_test.cpp

#include <iostream>
#include <cassert>

#include "config.hpp"
#include "init_server.hpp"
#include "calculator.hpp"
#include "handler_reg.hpp"
#include "protocol_reg.hpp"

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(encryption_tool)
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
	log_accesser_single_type::get_ins()->register_msg(10054, caller);

	ctrl_center_single_type::init_ins();
	exception_single_type::init_ins()->bind_log_accesser(log_accesser_single_type::get_ins());

	runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info());

	if(!shared_info_ptr)
	{
		return -1;
	}

	::yggr::shared_ptr<Calculator> my_cal(new Calculator());

	reg_calculator(shared_info_ptr, my_cal);

	task_center_single_type::init_ins(shared_info_ptr, task_center_type::rst_saver_init_type());

	if(!init_server(4, port, *(task_center_single_type::get_ins()), 6))
	{
		return -1;
	}

	svr_ptr_type psvr = svr_ptr_single_type::get_ins();
	reg_network_protocol(psvr);

	{
		ctrl_center_single_type::obj_ptr_type pctrl = ctrl_center_single_type::get_ins();
		if(pctrl)
		{
			reg_ctrl_center(pctrl, psvr, my_cal);
		}
	}

	timer_mgr_type timer_mgr;
	reg_timer(timer_mgr, psvr);

	try
	{
		//psvr->run(); // bigobj
		run_server();
		timer_mgr.start();
		std::cout << "started" << std::endl;

#	ifdef _DEBUG
		char cc = 0;
		std::cin >> cc;
#	else
		psvr->join();
		//yggr::ptr_single<yggr::exception::exception>::get_ins()->join();  // linux exit hang
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

	unreg_calculator(shared_info_ptr, my_cal);

	{
	    runner_shared_info_ptr_type tmp;
		shared_info_ptr.swap(tmp);
	}

	timer_mgr.stop();
	timer_mgr.clear();
	psvr->stop();
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	{
	    svr_ptr_type tmp;
		psvr.swap(tmp);
	}
	ctrl_center_single_type::uninstall();
	svr_ptr_single_type::uninstall();
	task_center_single_type::uninstall();

	std::cout << "stop end" << std::endl;
	exception_single_type::uninstall();

	return 0;
}
