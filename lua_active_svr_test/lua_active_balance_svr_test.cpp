// lua_active_balance_svr_test.cpp

//#include <iostream>
//#include <lua_calculator_impl.hpp>
//
//#include <yggr/base/random.h>
//
//#include <yggr/modules/modules_mgr.hpp>
//#include <yggr/plugins/plugins_installer.hpp>


#include "plugin_setup.hpp"
#include "handler_reg.hpp"
#include "init_server.hpp"

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(modules)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

//typedef yggr::ids::inner_process_id inner_process_id_type;
//typedef yggr::ids::id_generator<inner_process_id_type> inner_process_id_gen_type;
//typedef yggr::ptr_single<inner_process_id_type> inner_process_id_single_type;
//
//typedef yggr::plugins::plugins_installer<yggr::modules::modules_mgr> plugins_install_type;
//typedef yggr::ptr_single<plugins_install_type> plugins_install_single_type;
//
//typedef yggr::modules::modules_accesser::error_type modules_error_type;

//void plugin_install_handler(runner_shared_info_ptr_type ptr1,
//								single_container_type& scont,
//								Enter_Handler ptr3)
//{
//	if(!ptr3)
//	{
//		return;
//	}
//
//	(*ptr3)(ptr1, scont);
//}
//
//void plugin_uninstall_handler(runner_shared_info_ptr_type ptr1,
//								Leave_Handler ptr3)
//{
//	if(!ptr3)
//	{
//		return;
//	}
//
//	(*ptr3)(ptr1);
//}

int main(int argc, char* argv[])
{

	// !!! MINGW will release the loaded DLL again after the process ends !!!
#if defined(__MINGW32__)
    {
        HMODULE hmod = LoadLibrary("lua_cal_svr_procotol.dll");
        if(!hmod)
        {
            std::cerr << "load lua_cal_svr_procotol.dll faied" << std::endl;
            return -1;
        }
    }

    {
        HMODULE hmod = LoadLibrary("lua_calculator.dll");
        if(!hmod)
        {
            std::cerr << "load lua_calculator.dll faied" << std::endl;
            return -1;
        }
    }
#endif // #if defined(__MINGW32__)

	yggr::random::init();
	{
		inner_process_id_gen_type gen;
		inner_process_id_single_type::init_ins(gen());
	}

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
    yggr::string module_name = "lua_calculator.dll";
#elif defined(YGGR_AT_LINUX)
    yggr::string module_name = "lua_calculator.so";
#elif defined(YGGR_AT_DARWIN)
	yggr::string module_name = "lua_calculator.dylib";
#else
#   error "module name unknown !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

	std::cout << module_name << std::endl;
	yggr::u32 port = 9000;

	if(!(argc < 2) && (argv[1] == yggr::string("--input-port")))
	{
		std::cout << "please input port:" << std::endl;
		std::cin >> port;
	}

	//yggr::log::yggr_exception_log_accesser_type::init_type log_init; //memd to BOOST_PP_LOCAL_MACRO
	yggr::log::default_log_accesser::init_type log_init;
	log_init.push_back("svr_nt_log");

	log_accesser_single_type::init_ins(log_init);

	yggr::log::default_log_accesser::op_caller_type caller("svr_nt_log");
	log_accesser_single_type::get_ins()->register_msg(10054, caller);

	ctrl_center_single_type::init_ins();
	//exception_single_type::init_ins();
	{
        exception_single_type::obj_ptr_type ptr = exception_single_type::init_ins();
        assert(ptr);
    }

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

	//::yggr::shared_ptr<Calculator> my_cal(new Calculator());
	//shared_info_ptr->register_calculator(*my_cal); //use this of in dll

	task_center_single_type::init_ins(shared_info_ptr, task_center_type::rst_saver_init_type());

	//svr_ptr_single_type::init_ins(4,
	//								tcp_svr_handler_type::init_type(port,
	//																*(task_center_single_type::get_ins()),
	//																tcp_svr_handler_type::link_handler_init_type()));

	if(!init_server(4, port))
	{
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
			//pctrl->register_controller(*psvr);
			reg_ctrl_center(pctrl, psvr);
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
		//exception_single_type::get_ins()->join(); // linux exit hang
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

	pis_ptr->uninstall<Leave_Handler>(module_name, LEAVE_NAME,
										boost::bind(&plugin_uninstall_handler,
														shared_info_ptr, _1));

	{
			plugins_install_single_type::obj_ptr_type tmp;
			pis_ptr.swap(tmp);
	}
	{
	    runner_shared_info_ptr_type tmp;
		shared_info_ptr.swap(tmp);
	}

	psvr->stop();
	{
	    svr_ptr_type tmp;
		psvr.swap(tmp);
	}

	//exception_single_type::get_ins()->stop(); //macOS free and set null early
	{
        exception_single_type::obj_ptr_type ptr = exception_single_type::get_ins();
        if(ptr) ptr->stop();
    }

	plugins_install_single_type::uninstall();
	inner_process_id_single_type::uninstall();
	ctrl_center_single_type::uninstall();
	task_center_single_type::uninstall();
	svr_ptr_single_type::uninstall();

	log_accesser_single_type::uninstall();

	std::cout << "stop end" << std::endl;



	exception_single_type::uninstall();
	return 0;
}
