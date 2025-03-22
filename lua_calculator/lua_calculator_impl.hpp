//dll_cal_test_impl.hpp

#ifndef __DLL_CAL_TEST_IMPL_HPP__
#define __DLL_CAL_TEST_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/dll_make/dll_mark_define.hpp>
#include <yggr/dll_make/dll_imp_helper.hpp>
#include "lua_dll_param.hpp"
#include <yggr/base/ptr_single.hpp>
#include <yggr/exception/exception.hpp>

YGGR_DLL_MAKE_API_TYPE_2(void, Enter_Handler, runner_shared_info_ptr_type, single_container_type&);
YGGR_DLL_MAKE_API_TYPE_1(void, Leave_Handler, runner_shared_info_ptr_type);

#define ENTRY_NAME "reg_cal_enter"
#define LEAVE_NAME "unreg_cal_enter"

#endif //__DLL_CAL_TEST_IMPL_HPP__
