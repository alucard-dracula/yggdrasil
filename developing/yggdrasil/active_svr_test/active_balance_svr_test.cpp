//active_svr_test.cpp

#include <iostream>

#include <dll_cal_test_imp.hpp>

#include <yggr/base/random.h>

#include <yggr/modules/modules_mgr.hpp>
#include <yggr/plugins/plugins_installer.hpp>

#ifdef _MSC_VER
#   ifdef _DEBUG
#	    include <vld.h>
#   endif //_DEBUG
#endif //_MSC_VER

typedef yggr::ids::inner_process_id inner_process_id_type;
typedef yggr::ids::id_generator<inner_process_id_type> inner_process_id_gen_type;
typedef yggr::ptr_single<inner_process_id_type> inner_process_id_single_type;

typedef yggr::plugins::plugins_installer<yggr::modules::modules_mgr> plugins_install_type;
typedef yggr::ptr_single<plugins_install_type> plugins_install_single_type;

typedef yggr::modules::modules_accesser::error_type modules_error_type;

void plugin_install_handler(runner_shared_info_ptr_type ptr1,
								single_container_type& scont,
								Enter_Handler ptr3)
{
	if(!ptr3)
	{
		return;
	}

	(*ptr3)(ptr1, scont);
}

void plugin_uninstall_handler(runner_shared_info_ptr_type ptr1,
								Leave_Handler ptr3)
{
	if(!ptr3)
	{
		return;
	}

	(*ptr3)(ptr1);
}

int main(int argc, char* argv[])
{
	typedef yggr::ptr_single<task_center_type> task_center_single_type;

	yggr::random::init();
	{
		inner_process_id_gen_type gen;
		inner_process_id_single_type::init_ins(gen());
	}

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
    std::string module_name = "dll_cal_test.dll";
#elif defined(YGGR_AT_LINUX)
    std::string module_name = "dll_cal_test.so";
#else
#   error "module name unknow !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

	std::cout << module_name << std::endl;
	yggr::u32 port = 9000;
	std::cout << "please input port:" << std::endl;
	std::cin >> port;

	yggr::log::yggr_exception_log_accesser_type::init_type log_init; //memd to BOOST_PP_LOCAL_MACRO
	log_init.push_back("svr_nt_log");

	log_accesser_single_type::init_ins(log_init);

	yggr::log::yggr_exception_log_accesser_type::data_creator_type dc1("svr_nt_log");
	log_accesser_single_type::get_ins()->register_msg(10054, dc1);

	ctrl_center_single_type::init_ins();
	exception_single_type::init_ins();

	runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info());

	if(!shared_info_ptr)
	{
		return -1;
	}

	plugins_install_single_type::init_ins();

	plugins_install_single_type::obj_ptr_type pis_ptr(plugins_install_single_type::get_ins());

	if(!pis_ptr)
	{
		return -1;
	}

	//boost::shared_ptr<Calculator> my_cal(new Calculator());
	//shared_info_ptr->register_calculator(*my_cal); //use this of in dll

	task_center_single_type::init_ins(shared_info_ptr, task_center_type::rst_saver_init_type());

	try
	{
		svr_ptr_single_type::init_ins(4,
										tcp_svr_handler_type::init_type(port,
																		*(task_center_single_type::get_ins()),
																		tcp_svr_handler_type::link_handler_init_type()));
	}
	catch(const compatibility::stl_exception& e)
	{
        std::cerr << e.what() << std::endl;
		return -1;
	}
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
		return -1;
	}

	svr_ptr_type psvr = svr_ptr_single_type::get_ins();
	//psvr->register_network_protocol<test_pak_type>(); // use this of in dll

	try
	{
		single_container_type scont;

		pis_ptr->install<Enter_Handler>(module_name, ENTRY_NAME,
										boost::bind(&plugin_install_handler,
														shared_info_ptr, boost::ref(scont), _1));
	}
	catch(const modules_error_type& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	{
		ctrl_center_single_type::obj_ptr_type pctrl = ctrl_center_single_type::get_ins();
		if(pctrl)
		{
			pctrl->register_controller(*psvr);
		}
	}


	try
	{
		psvr->run();
		std::cout << "started" << std::endl;

#	ifdef _DEBUG
		char cc = 0;
		std::cin >> cc;
#	else
		psvr->join();
		exception_single_type::get_ins()->join();
#	endif //_DEBUG

	}
	catch(const compatibility::stl_exception& e)
	{
        std::cerr << e.what() << std::endl;
	}
	catch(const boost::system::error_code& e)
	{
		std::cerr << e.message() << std::endl;
	}

	pis_ptr->uninstall<Leave_Handler>(module_name, LEAVE_NAME,
										boost::bind(&plugin_uninstall_handler,
														shared_info_ptr, _1));
	{
	    runner_shared_info_ptr_type tmp;
		shared_info_ptr.swap(tmp);
	}

	psvr->stop();
	exception_single_type::get_ins()->stop();
	{
	    svr_ptr_type tmp;
		psvr.swap(tmp);
	}
	plugins_install_single_type::uninstall();
	ctrl_center_single_type::uninstall();
	svr_ptr_single_type::uninstall();
	task_center_single_type::uninstall();

	std::cout << "stop end" << std::endl;
	exception_single_type::uninstall();

	plugins_install_single_type::uninstall();
	inner_process_id_single_type::uninstall();

	return 0;
}
