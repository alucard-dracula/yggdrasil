//mixer_svr_test.cpp

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
#include YGGR_PP_LINK_LIB(encryption_tool)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void out_online_size(yggr::u32 size)
{
	std::cout << "now_online_size = " << size << std::endl;
}

int main(int argc, char* argv[])
{
	yggr::u32 port = 9000;

	if(!(argc < 2) && (argv[1] == yggr::string("--input-port")))
	{
		std::cout << "please input port:" << std::endl;
		std::cin >> port;
	}

	log_accesser_single::init_ins("svr_nt_log");

	yggr::log::default_log_accesser::op_caller_type caller("svr_nt_log");
	log_accesser_single::get_ins()->register_msg(10053, caller);

	ctrl_center_single::init_ins();
	yggr::ptr_single<yggr::exception::exception>::init_ins();

	runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info());

	if(!shared_info_ptr)
	{
		return -1;
	}

	::yggr::shared_ptr<Calculator> my_cal(new Calculator());
	//shared_info_ptr->register_calculator(*my_cal);
	reg_calculator(shared_info_ptr, my_cal);

	task_center_single::init_ins(shared_info_ptr, task_center_type::rst_saver_init_type());

	{
	    runner_shared_info_ptr_type tmp;
		shared_info_ptr.swap(tmp);
	}

	svr_ptr_single_type::init_ins(4, *(task_center_single::get_ins()));

	svr_ptr_type psvr = svr_ptr_single_type::get_ins();

	if(!psvr)
	{
		return -1;
	}

	try
	{
		//psvr->append<tcp_svr_handler_type>(tcp_svr_handler_type::link_handler_init_type(), port);
		//psvr->append<udp_svr_handler_type>(udp_svr_handler_type::link_handler_init_type(6), port /*+ 100*/);
		append_server(psvr, port);
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
		return -1;
	}

	//psvr->register_network_protocol<test_pak_type>();
	reg_network_protocol(psvr);

	{
		ctrl_center_single::obj_ptr_type pctrl = ctrl_center_single::get_ins();
		if(pctrl)
		{
			//pctrl->register_controller(*psvr);
			//pctrl->register_dispatchers(*my_cal);
			reg_ctrl_center(pctrl, psvr, my_cal);
		}
	}

	timer_mgr_type timer_mgr;
	timer_mgr.add_task<heart_timer_task_pak_type>(psvr, yggr::time::time(10, 0));

	try
	{
		//psvr->run();
		run_server(psvr);
		timer_mgr.start();
		psvr->using_method<tcp_svr_handler_type>(
				boost::bind(&tcp_svr_handler_type::online_size, _1),
				boost::bind(&out_online_size, _1) );
		//psvr->append<udp_svr_handler_type>(udp_svr_handler_type::link_handler_init_type(6), port + 100); // test run after ok
		std::cout << "started" << std::endl;

#	ifdef _DEBUG
		char cc = 0;
		std::cin >> cc;
#	else
		psvr->join();
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

	timer_mgr.stop();
	timer_mgr.clear();
	psvr->stop();
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	{
	    svr_ptr_type tmp;
		psvr.swap(tmp);
	}
	ctrl_center_single::uninstall();
	svr_ptr_single_type::uninstall();
	task_center_single::uninstall();

	std::cout << "stop end" << std::endl;
	yggr::ptr_single<yggr::exception::exception>::uninstall();

	return 0;
}
