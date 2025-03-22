//dll_cal_test.h

#ifndef __DLL_CAL_TEST_H__
#define __DLL_CAL_TEST_H__

#include <yggr/dll_make/dll_mark_define.hpp>
#include <yggr/dll_make/dll_def_helper.hpp>
#include <dll_param.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/exception/exception.hpp>

#	include <yggr/dll_make/dll_make_begin.ipp>

	YGGR_DLL_MAKE_API_2(void, reg_cal_enter, runner_shared_info_ptr_type, single_container_type&);
	YGGR_DLL_MAKE_API_1(void, unreg_cal_enter, runner_shared_info_ptr_type);

#	include <yggr/dll_make/dll_make_end.ipp>

#endif //__DLL_CAL_TEST_H__
