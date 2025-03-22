//proxy_udp_svr_test.cpp

#include "config.hpp"
#include "calculator.hpp"
#include "init_server.hpp"
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

//----------------------------------------------------------------------------------

void out_input_menu(void)
{
	/*
	E_proxy_mode_monopolize,
		E_proxy_mode_source_hash,
		E_proxy_mode_blanace,
		E_proxy_mode_all,
	*/
	std::cout << "proxy_state:\n"
				<< "1: mode_monopolize\n"
				<< "2: mode_source_hash\n"
				<< "3: mode_blance\n"
				<< "4: mode_all" << std::endl;
}

int main(int argc, char* argv[])
{
	send_count_single_type::init_ins(0);
	now_state_single_type::init_ins(0);
	chg_state_single_type::init_ins(0);

	out_input_menu();
	std::cout << "please input now_state and chg_state" << std::endl;
	std::cin >> *now_state_single_type::get_ins() >> *chg_state_single_type::get_ins();

	yggr::log::default_log_accesser::init_type log_init;
	log_init.push_back("clt_nt_log_eins");
	log_init.push_back("clt_nt_log_zwei");

	log_accesser_single::init_ins(log_init); 

	yggr::log::default_log_accesser::op_caller_type caller1("clt_nt_log_eins");
	yggr::log::default_log_accesser::op_caller_type caller2("clt_nt_log_zwei");
	log_accesser_single::get_ins()->register_msg(boost::asio::error::connection_refused, caller1);
	log_accesser_single::get_ins()->register_msg(boost::asio::error::not_connected, caller2);

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

	/*{
		shared_info_ptr.swap(runner_shared_info_ptr_type());
	}*/

	clt_ptr_single_type::init_ins(1,
									udp_clt_handler_type::init_type(*(task_center_single::get_ins()),
																		udp_clt_handler_type::link_handler_init_type(1)));

	clt_ptr_type pclt = clt_ptr_single_type::get_ins();

	ctrl_center_single::obj_ptr_type pctrl = ctrl_center_single::get_ins();

	// register network protocol
	//pclt->register_network_protocol<test_pak_type>();
	//pclt->register_network_protocol<proxy_register_msg_type>(yggr::network::network_direct_def_type::E_send_enable);
	//pclt->register_network_protocol<proxy_register_back_msg_type>(yggr::network::network_direct_def_type::E_recv_enable);
	//pclt->register_network_protocol<proxy_mode_change_msg_type>(yggr::network::network_direct_def_type::E_send_enable);
	//pclt->register_network_protocol<proxy_mode_change_back_msg_type>(yggr::network::network_direct_def_type::E_recv_enable);
	//pclt->register_network_protocol<proxy_unregister_msg_type>(yggr::network::network_direct_def_type::E_send_enable);
	//pclt->register_network_protocol<proxy_unregister_back_msg_type>(yggr::network::network_direct_def_type::E_recv_enable);
	reg_network_protocol(pclt);

	if(pctrl)
	{
		//pctrl->register_controller(*pclt);
		////pctrl->register_dispatchers(*my_cal);
		reg_ctrl_center(pctrl, pclt, my_cal);
	}

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
		//yggr::ptr_single<yggr::exception::exception>::get_ins()->join();  // linux exit hang
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

	pclt->stop();
	{
	    clt_ptr_type tmp;
		pclt.swap(tmp);
	}
	
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	
	ctrl_center_single::uninstall();
	clt_ptr_single_type::uninstall();
	task_center_single::uninstall();

	std::cout << "stop end" << std::endl;
	yggr::ptr_single<yggr::exception::exception>::uninstall();

	return 0;
}

