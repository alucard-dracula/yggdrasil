// proxy_trun_tcp_svr_test.cpp

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
	data_info_type* pinfo = 0;

	yggr::u32 port = 9000;
	std::cout << "please input port:" << std::endl;
	std::cin >> port;

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


	proxy_router_ptr_type prouter(new proxy_router_type());
	proxy_args_builder_ptr_type pargs_build(new proxy_args_builder_type());
	proxy_repeater_ptr_type prepeater(
		new proxy_repeater_type(proxy_shared_info_ptr, prouter, pargs_build));

	proxy_task_center_single::init_ins(proxy_shared_info_ptr, proxy_task_center_type::rst_saver_init_type());

	svr_ptr_single_type::init_ins(4,
									tcp_svr_handler_type::init_type(port,
																	*(proxy_task_center_single::get_ins()),
																	tcp_svr_handler_type::link_handler_init_type()));
	svr_ptr_type psvr = svr_ptr_single_type::get_ins();

	::yggr::shared_ptr<proxy_msg_calculator> proxy_cal(new proxy_msg_calculator(prepeater, psvr->get_adapter_mgr_ptr()));
	//proxy_shared_info_ptr->register_calculator(*proxy_cal);
	reg_calculator(proxy_shared_info_ptr, proxy_cal);

	{
	    proxy_runner_shared_info_ptr_type tmp;
		proxy_shared_info_ptr.swap(tmp);
	}

	//psvr->register_network_protocol_of_container<test_pak_type, net_ipak_type>(); // proxy turn data type registered from proxy_svr

	////register network protocol, some proxy_system's used network data
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
			//pctrl->unregister_dispatchers(*proxy_cal);
			//pctrl->register_dispatchers(*proxy_cal);
			reg_ctrl_center(pctrl, psvr, proxy_cal);
		}
	}

	try
	{
		//psvr->run();
		run_server(psvr);
		std::cout << "started" << std::endl;

#	ifdef _DEBUG
		char cc = 0;
		std::cin >> cc;
#	else
		psvr->join();
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

	psvr->stop();
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	{
	    svr_ptr_type tmp;
		psvr.swap(tmp);
	}

	proxy_cal->clear();
	// do not forget to deal with this clear, otherwise it will cause a memory leak when exiting. 
	// currently invoked in proxy_cal->clear()
	//prepeater->clear(); 
	
	ctrl_center_single::uninstall();
	svr_ptr_single_type::uninstall();
	proxy_task_center_single::uninstall();

	std::cout << "stop end" << std::endl;
	yggr::ptr_single<yggr::exception::exception>::uninstall();

	return 0;
}
