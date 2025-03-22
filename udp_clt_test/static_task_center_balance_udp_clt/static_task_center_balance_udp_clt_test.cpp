//static_task_center_balance_udp_clt_test.cpp

#include <iostream>
#include <cassert>

#include "config.hpp"
#include "calculator.hpp"
#include "init_client.hpp"
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
	std::cout << "please input num" << std::endl;
	std::cin >> Calculator::send_count();

	boost::array<std::string, 2> log_init = {"clt_nt_log_eins", "clt_nt_log_zwei"};

	log_accesser_single_type::init_ins(log_init);

	yggr::log::default_log_accesser::op_caller_type caller1("clt_nt_log_eins");
	yggr::log::default_log_accesser::op_caller_type caller2("clt_nt_log_zwei");
	log_accesser_single_type::get_ins()->register_msg(boost::asio::error::connection_refused, caller1);
	log_accesser_single_type::get_ins()->register_msg(boost::asio::error::not_connected, caller2);

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

	if(!init_client(1, *(task_center_single_type::get_ins()), 1))
	{
		return -1;
	}
	

	clt_ptr_type pclt = clt_ptr_single_type::get_ins();

	reg_network_protocol(pclt);

	ctrl_center_single_type::obj_ptr_type pctrl = ctrl_center_single_type::get_ins();
	
	if(pctrl)
	{
		reg_ctrl_center(pctrl, pclt, my_cal);
	}

	std::string ip("127.0.0.1");
	//std::string ip("192.168.1.103");
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

	pclt->stop();
	
	{
	    clt_ptr_type tmp;
		pclt.swap(tmp);
	}
	
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	
	ctrl_center_single_type::uninstall();
	clt_ptr_single_type::uninstall();
	task_center_single_type::uninstall();

	std::cout << "stop end" << std::endl;
	exception_single_type::uninstall();

	return 0;
}

