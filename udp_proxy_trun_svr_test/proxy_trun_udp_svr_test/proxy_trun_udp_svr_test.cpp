// proxy_trun_udp_svr_test.cpp

#include "config.hpp"
#include "calculator.hpp"
#include "init_server.hpp"
#include "init_proxy.hpp"
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
	send_count_single_type::init_ins(0);
	now_state_single_type::init_ins(0);
	chg_state_single_type::init_ins(0);

	yggr::u32 port = 9000;
	
	if(!(argc < 2) && (argv[1] == yggr::string("--input-port")))
	{
		std::cout << "please input port:" << std::endl;
		std::cin >> port;
	}

	yggr::log::default_log_accesser::init_type log_init;
	log_init.push_back("svr_nt_log");

	log_accesser_single::init_ins(log_init);

	yggr::log::default_log_accesser::op_caller_type caller1("svr_nt_log");
	log_accesser_single::get_ins()->register_msg(10054, caller1);

	ctrl_center_single::init_ins();
	yggr::ptr_single<yggr::exception::exception>::init_ins();

	proxy_runner_shared_info_ptr_type proxy_shared_info_ptr(proxy_work_runner_type::create_work_runner_shared_info());

	if(!proxy_shared_info_ptr)
	{
		return -1;
	}

	//proxy_router_ptr_type prouter(new proxy_router_type());
	//proxy_args_builder_ptr_type pargs_build(new proxy_args_builder_type());
	//proxy_repeater_ptr_type prepeater(
	//	new proxy_repeater_type(proxy_shared_info_ptr, prouter, pargs_build));

	proxy_repeater_ptr_type prepeater = create_proxy_repeater(proxy_shared_info_ptr);

	//proxy_task_center_single::init_ins(proxy_shared_info_ptr, proxy_task_center_type::rst_saver_init_type());
	init_proxy(proxy_shared_info_ptr);

	//try
	//{
	//	svr_ptr_single_type::init_ins(4,
	//									udp_svr_handler_type::init_type(port,
	//																	*(proxy_task_center_single::get_ins()),
	//																	udp_svr_handler_type::link_handler_init_type(6)));
	//}
	//catch(const boost::system::error_code& e)
	//{
	//	std::cerr << e.message() << std::endl;
	//	return -1;
	//}
	//catch(const yggr::stl_exception& e)
	//{
	//	std::cerr << e.what() << std::endl;
	//	return -1;
	//}
	
	if(!init_server(4, port))
	{
		return -1;
	}
	
	svr_ptr_type psvr = svr_ptr_single_type::get_ins();

	::yggr::shared_ptr<proxy_msg_calculator> proxy_cal(new proxy_msg_calculator(prepeater, psvr->get_adapter_mgr_ptr()));

	//proxy_shared_info_ptr->register_calculator(*proxy_cal);
	reg_calculator(proxy_shared_info_ptr, proxy_cal);

	
	//register network protocol, some proxy_system's used network data
	//psvr->register_network_protocol_of_container<proxy_register_msg_type,
	//												net_ipak_type>(yggr::network::network_direct_def_type::E_recv_enable);

	//psvr->register_network_protocol_of_container<proxy_register_back_msg_type,
	//												net_ipak_type>(yggr::network::network_direct_def_type::E_send_enable);

	//psvr->register_network_protocol_of_container<proxy_mode_change_msg_type,
	//												net_ipak_type>(yggr::network::network_direct_def_type::E_recv_enable);

	//psvr->register_network_protocol_of_container<proxy_mode_change_back_msg_type,
	//												net_ipak_type>(yggr::network::network_direct_def_type::E_send_enable);

	//psvr->register_network_protocol_of_container<proxy_unregister_msg_type,
	//												net_ipak_type>(yggr::network::network_direct_def_type::E_recv_enable);

	//psvr->register_network_protocol_of_container<proxy_unregister_back_msg_type,
	//												net_ipak_type>(yggr::network::network_direct_def_type::E_send_enable);

	reg_network_protocol(psvr);


	{
		ctrl_center_single::obj_ptr_type pctrl = ctrl_center_single::get_ins();
		if(pctrl)
		{
			//pctrl->register_controller(*psvr);
			//pctrl->register_dispatchers(*proxy_cal);
			reg_ctrl_center(pctrl, psvr, proxy_cal);
		}
	}

	timer_mgr_type timer_mgr;
	timer_mgr.add_task<heart_timer_task_pak_type>(psvr, yggr::time::time(10, 0));

	try
	{
		//psvr->run();
		run_server(psvr);
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

	//proxy_shared_info_ptr->unregister_calculator(*proxy_cal);
	unreg_calculator(proxy_shared_info_ptr, proxy_cal);
	{
	    proxy_runner_shared_info_ptr_type tmp;
		proxy_shared_info_ptr.swap(tmp);
	}

	timer_mgr.stop();
	timer_mgr.clear();
	//psvr->stop(); // bigobj
	stop_server(psvr);
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	{
	    svr_ptr_type tmp;
		psvr.swap(tmp);
	}

	proxy_cal->clear();
	//prepeater->clear();
	
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	
	ctrl_center_single::uninstall();
	svr_ptr_single_type::uninstall();
	proxy_task_center_single::uninstall();

	std::cout << "stop end" << std::endl;
	yggr::ptr_single<yggr::exception::exception>::uninstall();

	return 0;
}
