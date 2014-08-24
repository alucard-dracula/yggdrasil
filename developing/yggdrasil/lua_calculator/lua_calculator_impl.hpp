//dll_cal_test_impl.hpp

#ifndef __DLL_CAL_TEST_IMPL_HPP__
#define __DLL_CAL_TEST_IMPL_HPP__

#include <lua_dll_param.hpp>
#include <yggr/dll_make/dll_mark_define.hpp>
#include <yggr/dll_make/dll_imp_helper.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/base/exception.hpp>

YGGR_DLL_MAKE_API_TYPE_3(void, Enter_Handler,
							runner_shared_info_ptr_type,
							exception_ptr_single_type::obj_ptr_type,
							svr_ptr_single_type::obj_ptr_type);

#define HANDLER_NAME "reg_cal_enter"

#endif //__DLL_CAL_TEST_IMPL_HPP__
