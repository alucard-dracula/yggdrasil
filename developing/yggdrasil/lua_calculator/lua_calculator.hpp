//lua_calculator.hpp

#ifndef __LUA_CALCULATOR_HPP__
#define __LUA_CALCULATOR_HPP__

#include <yggr/dll_make/dll_mark_define.hpp>
#include <yggr/dll_make/dll_def_helper.hpp>
#include <lua_dll_param.hpp>
#include <yggr/base/ptr_single.hpp>

#	include <yggr/dll_make/dll_make_begin.ipp>

	YGGR_DLL_MAKE_API_3(void, reg_cal_enter,
							runner_shared_info_ptr_type,
							exception_ptr_single_type::obj_ptr_type,
							svr_ptr_single_type::obj_ptr_type);

#	include <yggr/dll_make/dll_make_end.ipp>

#endif //__LUA_CALCULATOR_HPP__
