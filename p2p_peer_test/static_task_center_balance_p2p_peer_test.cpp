//static_task_center_balance_p2p_peer_test.cpp

#include "p2p_peer_def.hpp"
#include "p2p_peer_calculator_def.hpp"
#include "user_calculator.hpp"
#include "init_peer.hpp"
#include "register_network_protocol.hpp"
#include "register_ctrl_center.hpp"
#include "register_p2p_calculator.hpp"
#include "register_user_calculator.hpp"

//static yggr::u32 send_count = 0;

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
	//std::cout << "please input num" << std::endl;
	//std::cin >> send_count;

	boost::array<std::string, 2> log_init = {"clt_nt_log_eins", "clt_nt_log_zwei"};

	log_accesser_single_type::init_ins(log_init);

	yggr::log::default_log_accesser::op_caller_type caller1("clt_nt_log_eins");
	yggr::log::default_log_accesser::op_caller_type caller2("clt_nt_log_zwei");
	log_accesser_single_type::get_ins()->register_msg(boost::asio::error::connection_refused, caller1);
	log_accesser_single_type::get_ins()->register_msg(boost::asio::error::not_connected, caller2);

	ctrl_center_single_type::init_ins();
	yggr::ptr_single<yggr::exception::exception>
		::init_ins()->bind_log_accesser(log_accesser_single_type::get_ins());

	runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info());

	if(!shared_info_ptr)
	{
		return -1;
	}

	::yggr::shared_ptr<user_calculator> my_cal(new user_calculator());
	::yggr::shared_ptr<p2p_peer_calculator_type> p2p_cal(new p2p_peer_calculator_type());

	register_user_calcuator(shared_info_ptr, my_cal);
	register_p2p_calculator(shared_info_ptr, p2p_cal);

	task_center_single_type::init_ins(shared_info_ptr, task_center_type::rst_saver_init_type());

	//clt_ptr_single_type::init_ins(
	//	1,
	//	p2p_peer_handler_type::init_type(
	//		*(task_center_single_type::get_ins()),
	//		p2p_peer_handler_type::link_handler_init_type(1)));

	init_peer(1);

	clt_ptr_type pclt = clt_ptr_single_type::get_ins();

	ctrl_center_single_type::obj_ptr_type pctrl = ctrl_center_single_type::get_ins();

	register_network_protocol(pclt);

	if(pctrl)
	{
		//pctrl->register_controller(*pclt); // error debug it
		register_ctrl_center(pctrl, pclt);
	}

#if 0
	std::string ip("192.168.1.101");
#else
	std::string ip("127.0.0.1");
#endif // 1 | 0
	std::string port("9000");
	try
	{
		//pclt->run();
		//pclt->op_handler().connect(ip, port);
		run_peer(pclt, ip, port);
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
	unregister_user_calcuator(shared_info_ptr, my_cal);
	unregister_p2p_calculator(shared_info_ptr, p2p_cal);
	
	{
	    runner_shared_info_ptr_type tmp;
		shared_info_ptr.swap(tmp);
	}

	pclt->stop();
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	{
	    clt_ptr_type tmp;
		pclt.swap(tmp);
	}
	ctrl_center_single_type::uninstall();
	clt_ptr_single_type::uninstall();
	task_center_single_type::uninstall();

	std::cout << "stop end" << std::endl;
	yggr::ptr_single<yggr::exception::exception>::uninstall();

	return 0;
}

