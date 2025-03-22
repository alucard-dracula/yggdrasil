// lua_calculator_detail.cpp : Defines the entry point for the DLL application.

#include "lua_calculator_detail.hpp"

#if defined(_MSC_VER)
#   include <vld.h>
#endif // defined(_MSC_VER) 

void register_network_protocol(void)
{
	svr_ptr_single_type::get_ins()->register_network_protocol<test_pak_type>();
	svr_ptr_single_type::get_ins()->register_network_protocol<cdt_procotol_type>();
	svr_ptr_single_type::get_ins()->register_network_protocol<rst_procotol_type>();
}