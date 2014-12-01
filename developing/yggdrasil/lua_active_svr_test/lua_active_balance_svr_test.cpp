//lua_active_svr_test.cpp

#error "the example is oldder"

#include <iostream>
#include <lua_calculator_impl.hpp>

#include <yggr/modules/modules_mgr.hpp>
#include <yggr/plugins/plugins_installer.hpp>


#ifdef _MSC_VER
#   ifdef _DEBUG
#	    include <vld.h>
#   endif //_DEBUG
#endif // _MSC_VER

typedef yggr::plugins::plugins_installer<yggr::modules::modules_mgr> plugins_install_type;

void plugin_install_handler(runner_shared_info_ptr_type ptr1, 
								exception_ptr_single_type::obj_ptr_type ptr2,
								svr_ptr_single_type::obj_ptr_type svr_ptr,
								Enter_Handler ptr3)
{
	if(!ptr3)
	{
		return;
	}
	ptr3(ptr1, ptr2, svr_ptr);
}

int main(int argc, char* argv[])
{
	typedef yggr::ptr_single<task_center_type> data_center_single;
	typedef yggr::ptr_single<ctrl_center_type> ctrl_center_single;
	typedef yggr::ptr_single<plugins_install_type> plugins_install_single;

	plugins_install_single::init_ins();
	ctrl_center_single::init_ins();

	yggr::ptr_single<yggr::exception::exception>::init_ins();

	runner_shared_info_ptr_type shared_info_ptr(work_runner_type::create_work_runner_shared_info());
	if(!shared_info_ptr)
	{
		return -1;
	}

	data_center_single::init_ins(shared_info_ptr);


	//svr_type svr(svr_type::init_type(9000, *(data_center_single::get_ins()), 6));
	svr_ptr_single_type::init_ins(svr_type::init_type(9000, *(data_center_single::get_ins()), 6));
	svr_type& svr = *(svr_ptr_single_type::get_ins());


#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
    std::string module_name = "lua_calculator.dll";
#elif defined(YGGR_AT_LINUX)
    std::string module_name = "lua_calculator.so";
#else
#   error "module name unknow !"
#endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

    std::cout << module_name << std::endl;

	plugins_install_single::obj_ptr_type pis_ptr(plugins_install_single::get_ins());

	if(pis_ptr)
	{
		pis_ptr->install<Enter_Handler>(module_name, HANDLER_NAME, boost::bind(&plugin_install_handler, shared_info_ptr,
																				exception_ptr_single_type::get_ins(),
																				svr_ptr_single_type::get_ins(), _1));
        //pis_ptr->install<Enter_Handler>(module_name, HANDLER_NAME, boost::bind(&plugin_install_handler, shared_info_ptr, _1));
	}

  /*  yggr::modules::modules_mgr mmgr;
    Enter_Handler peh = mmgr.get_entry<Enter_Handler>(module_name, HANDLER_NAME);
    std::cout << module_name << "," << HANDLER_NAME << std::endl;
	if(!peh)
	{
		std::cerr << "non foo" << std::endl;
	}
	else
	{
		peh(shared_info_ptr, yggr::ptr_single<yggr::exception::exception>::get_ins());
	}*/

	ctrl_center_single::get_ins()->register_controller(svr);

	svr.register_network_protocol<test_pak_type>();

	try
	{
		svr.run();

		std::cout << "started" << std::endl;

#	ifdef _DEBUG
		char cc = 0;
		std::cin >> cc;
		svr.stop();
		svr.join();
#	else
		svr.join();
		yggr::ptr_single<yggr::exception::exception>::get_ins()->join();
#	endif //_DEBUG

		svr_ptr_single_type::get_ins()->stop();
		yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();

		ctrl_center_single::uninstall();
		svr_ptr_single_type::uninstall();
		data_center_single::uninstall();


		shared_info_ptr->clear(); //waring this must clear cal register because dll will be free

        std::cout << "trd_mgr stop" << std::endl;
	}
	catch(const boost::system::system_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cout << "stop end" << std::endl;

//	if(pis_ptr)
//	{
//		pis_ptr->uninstall_all();
//	}

	plugins_install_single::uninstall();
	yggr::ptr_single<yggr::exception::exception>::uninstall();

	return 0;
}
