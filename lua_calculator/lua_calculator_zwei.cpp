// lua_calculator_zwei.cpp

#include "lua_calculator_detail.hpp"

#if defined(_MSC_VER)
#   include <vld.h>
#endif // defined(_MSC_VER) 

YGGR_DLL_API void YGGR_MODULE_CALL_T() unreg_cal_enter(runner_shared_info_ptr_type ptr)
{
	if(!ptr)
	{
		return;
	}

	svr_ptr_single_type::get_ins()->unregister_network_protocol<test_pak_type>();
	svr_ptr_single_type::get_ins()->unregister_network_protocol<cdt_procotol_type>();
	svr_ptr_single_type::get_ins()->unregister_network_protocol<rst_procotol_type>();

	ptr->unregister_calculator(*cal_sig_type::get_ins());
	ctrl_center_single_type::get_ins()->unregister_dispatchers(*cal_sig_type::get_ins());

	cal_sig_type::get_ins()->join();
	cal_sig_type::uninstall();

	inner_process_id_single_type::uninstall();
	single_container_type::uninstall();

	// lua uninstall
	script_mgr_single_type::uninstall();
}
