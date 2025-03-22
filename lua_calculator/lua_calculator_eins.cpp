// lua_calculator_eins.cpp

#include "lua_calculator_detail.hpp"

#if defined(_MSC_VER)
#   include <vld.h>
#endif // defined(_MSC_VER)

YGGR_DLL_API void YGGR_MODULE_CALL_T() reg_cal_enter(runner_shared_info_ptr_type ptr, single_container_type& singles)
{
	inner_process_id_gen_type gen;
	inner_process_id_single_type::init_ins(gen());
	if(!ptr || !singles.init_ins())
	{
		return;
	}

	cal_sig_type::init_ins();
	ctrl_center_single_type::get_ins()->register_dispatchers(*cal_sig_type::get_ins());

	// lua load
	//script_mgr_single_type::init_ins();
	//script_mgr_single_type::get_ins()->append_module(script_mgr_type::E_lib_all);
	//script_mgr_single_type::get_ins()->insert("cal_num", "cal", script_mgr_type::file_code_type("cal_num_test.lua"), 0);
	lua_load();

	//svr_ptr_single_type::get_ins()->register_network_protocol<test_pak_type>();
	//svr_ptr_single_type::get_ins()->register_network_protocol<cdt_procotol_type>();
	//svr_ptr_single_type::get_ins()->register_network_protocol<rst_procotol_type>();
	register_network_protocol();

	//ptr->register_calculator(*cal_sig_type::get_ins());
	register_calculator(ptr);
}
