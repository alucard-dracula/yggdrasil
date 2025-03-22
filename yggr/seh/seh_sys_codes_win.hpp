//seh_sys_codes_win.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_SEH_SEH_SYS_CODES_WIN_HPP__
#define __YGGR_SEH_SEH_SYS_CODES_WIN_HPP__

#ifndef YGGR_SEH_INCLUDE
#	error "seh_sys_codes_win.hpp please include seh.hpp."
#endif //YGGR_SEH_INCLUDE

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace seh
{

class seh_sys_codes_win
{
public:
	YGGR_STATIC_CONSTANT(u32, E_thread_access_violation = EXCEPTION_ACCESS_VIOLATION);
	YGGR_STATIC_CONSTANT(u32, E_thread_array_bounds_exceded = EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
	YGGR_STATIC_CONSTANT(u32, E_break_point = EXCEPTION_BREAKPOINT);
	YGGR_STATIC_CONSTANT(u32, E_single_step = EXCEPTION_SINGLE_STEP);
	YGGR_STATIC_CONSTANT(u32, E_thread_datatype_misalignment = EXCEPTION_DATATYPE_MISALIGNMENT);
	YGGR_STATIC_CONSTANT(u32, E_flt_denormal_operand = EXCEPTION_FLT_DENORMAL_OPERAND);
	YGGR_STATIC_CONSTANT(u32, E_flt_divide_by_zero = EXCEPTION_FLT_DIVIDE_BY_ZERO);
	YGGR_STATIC_CONSTANT(u32, E_flt_inexact_result = EXCEPTION_FLT_INEXACT_RESULT);
	YGGR_STATIC_CONSTANT(u32, E_flt_overflow = EXCEPTION_FLT_OVERFLOW);
	YGGR_STATIC_CONSTANT(u32, E_flt_stack_check = EXCEPTION_FLT_STACK_CHECK);
	YGGR_STATIC_CONSTANT(u32, E_flt_underflow = EXCEPTION_FLT_UNDERFLOW);
	YGGR_STATIC_CONSTANT(u32, E_int_divide_by_zero = EXCEPTION_INT_DIVIDE_BY_ZERO);
	YGGR_STATIC_CONSTANT(u32, E_int_overflow = EXCEPTION_INT_OVERFLOW);
	YGGR_STATIC_CONSTANT(u32, E_thread_priv_instructon = EXCEPTION_PRIV_INSTRUCTION);
	YGGR_STATIC_CONSTANT(u32, E_thread_in_page_error = EXCEPTION_IN_PAGE_ERROR);
	YGGR_STATIC_CONSTANT(u32, E_thread_illegal_instruction = EXCEPTION_ILLEGAL_INSTRUCTION);
	YGGR_STATIC_CONSTANT(u32, E_thread_noncontinuable_exception = EXCEPTION_NONCONTINUABLE_EXCEPTION);
	YGGR_STATIC_CONSTANT(u32, E_thread_stack_overflow = EXCEPTION_STACK_OVERFLOW);
	YGGR_STATIC_CONSTANT(u32, E_handle_invalid_disposition = EXCEPTION_INVALID_DISPOSITION);
	YGGR_STATIC_CONSTANT(u32, E_guard_page = EXCEPTION_GUARD_PAGE);
	YGGR_STATIC_CONSTANT(u32, E_handle_invalid_handle = EXCEPTION_INVALID_HANDLE);
	//YGGR_STATIC_CONSTANT(u32, E_posible_deadlock = EXCEPTION_POSSIBLE_DEADLOCK); // undef
	YGGR_STATIC_CONSTANT(u32, E_cpp_exception = 0xe06d7363);

	YGGR_STATIC_CONSTANT(u32, E_unknown = 0xffffffff);
};

} // namespace seh
} // namespace yggr

#endif // __YGGR_SEH_SEH_SYS_CODES_WIN_HPP__
