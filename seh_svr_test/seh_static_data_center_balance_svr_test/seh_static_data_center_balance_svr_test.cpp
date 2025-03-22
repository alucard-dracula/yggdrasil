//seh_static_data_center_balance_svr_test.cpp

#include "config.hpp"

#if defined(YGGR_USE_SEH)

#include "calculator.hpp"
#include "handler_reg.hpp"
#include "protocol_reg.hpp"
#include "init_server.hpp"

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(seh)
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

#if defined(YGGR_USE_SEH)
        yggr::seh::seh_type::init_ins();

#   if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
        yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_thread_access_violation);
#   elif defined(YGGR_AT_LINUX) || defined(YGGR_AT_ANDROID)

#		if defined(YGGR_AT_ANDROID)
        yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGSEGV);
		yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGABRT);
#		else
		yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGSEGV);
#		endif // #		if defined(YGGR_AT_ANDROID)
#	elif defined(YGGR_AT_DARWIN)
		yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGSEGV);
		//yggr::seh::seh_type::s_register_code(yggr::seh::seh_type::seh_sys_codes_type::E_SIGABRT);
#   else
#     error "please add now platform code!!!"
#   endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
#endif // YGGR_USE_SEH

	std::cout << "tmp seh start" << std::endl;

	log_accesser_single::init_ins("svr_nt_log"); 

	yggr::log::default_log_accesser::op_caller_type caller("svr_nt_log"); 
	log_accesser_single::get_ins()->register_msg(10053, caller); 
	log_accesser_single::get_ins()->register_msg(10054, caller);


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

	try
	{
		svr_ptr_single_type::init_ins(
			4, 
			tcp_svr_handler_type::init_type(
				port,
				*(task_center_single::get_ins()),
				tcp_svr_handler_type::link_handler_init_type()));
	}
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
		return -1;
	}
	catch(const yggr::stl_exception& e)
	{
        std::cerr << e.what() << std::endl;
		return -1;
	}

	svr_ptr_type psvr = svr_ptr_single_type::get_ins();
	//psvr->register_network_protocol<test_pak_type>(); 
	////psvr->unregister_network_protocol<test_pak_type>();
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
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
	}
	catch(const yggr::stl_exception& e)
	{
        std::cerr << e.what() << std::endl;
	}
	
	shared_info_ptr->unregister_calculator(*my_cal); 
	{
		runner_shared_info_ptr_type tmp_ptr;
		shared_info_ptr.swap(tmp_ptr);
	}
	psvr->stop(); 
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop(); 
	{
		svr_ptr_type tmp_ptr;
		psvr.swap(tmp_ptr);
	}
	ctrl_center_single::uninstall();
	svr_ptr_single_type::uninstall(); 
	task_center_single::uninstall(); 

	std::cout << "stop end" << std::endl;

#if defined(YGGR_USE_SEH)
	yggr::seh::seh_type::uninstall();
#endif // YGGR_USE_SEH
	yggr::ptr_single<yggr::exception::exception>::uninstall(); 
	
	return 0;
}

#else

int main(int argc, char* argv[])
{
	std::cout << "now platform not support seh" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}

#endif // #if defined(YGGR_USE_SEH)