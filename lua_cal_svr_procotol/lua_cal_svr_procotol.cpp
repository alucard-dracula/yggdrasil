//cal_svr_procotol.cpp

#include "lua_cal_svr_procotol_impl.hpp"

#include <yggr/charset/string.hpp>
#include <yggr/script/lua_base_environment.hpp>

#include <iostream>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/lua_include.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(script_lua)


#include <yggr/dll_make/dll_mark_define.hpp>

#include <yggr/dll_make/dll_make_begin.ipp>

YGGR_DLL_API int LUA_MODULE_INIT_FOO(lua_cal_svr_procotol)(lua_State*);

#include <yggr/dll_make/dll_make_end.ipp>



int LUA_MODULE_INIT_FOO(lua_cal_svr_procotol)(lua_State* L)
{
	std::cout << "added_cal_svr_procotol" << std::endl;
	luabind::open(L);

	luabind::module(L)
	[
	luabind::class_<cdt_procotol_type>("cdt_procotol_type")
		.def(luabind::constructor<>())
		.def(luabind::constructor<const cdt_procotol_type&>())
		.def(luabind::constructor<cdt_procotol_type::num_type, cdt_procotol_type::num_type>())
		.def(luabind::tostring(luabind::const_self))
		.def("get_num1", &cdt_procotol_type::get_num1)
		.def("set_num1", &cdt_procotol_type::set_num1)
		.def("get_num2", &cdt_procotol_type::get_num2)
		.def("set_num2", &cdt_procotol_type::set_num2),

	luabind::class_<rst_procotol_type>("rst_procotol_type")
		.def(luabind::constructor<>())
		.def(luabind::constructor<const rst_procotol_type&>())
		.def(luabind::constructor<rst_procotol_type::num_type>())
		.def(luabind::tostring(luabind::const_self))
		.def("get_rst", &rst_procotol_type::get_rst)
		.def("set_rst", &rst_procotol_type::set_rst)

	];

	return 0;
}
