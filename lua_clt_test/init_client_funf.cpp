//init_server.cpp

#include <cassert>
#include "init_client.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER


void stop_client(clt_ptr_type pclt)
{
	pclt->stop();
	pclt->clear();
	yggr::ptr_single<yggr::exception::exception>::get_ins()->stop();
	{
	    clt_ptr_type tmp;
		pclt.swap(tmp);
	}
}

void load_lua(void)
{
	script_mgr_single_type::init_ins();
	script_mgr_single_type::get_ins()->append_module(script_mgr_type::E_lib_all);
	script_mgr_single_type::get_ins()->insert("create_num", "create_cdt", script_mgr_type::file_code_type("create_cdt.lua"), 0);
}
