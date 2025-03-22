// lua_calculator_detail.cpp : Defines the entry point for the DLL application.

#include "lua_calculator_detail.hpp"

#if defined(_MSC_VER)
#   include <vld.h>
#endif // defined(_MSC_VER) 

void lua_load(void)
{
	// lua load
	script_mgr_single_type::init_ins();
	script_mgr_single_type::get_ins()->append_module(script_mgr_type::E_lib_all);
	script_mgr_single_type::get_ins()->insert("cal_num", "cal", script_mgr_type::file_code_type("cal_num_test.lua"), 0);
}