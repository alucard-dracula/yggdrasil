// any_operator_mgr_basic_t_reg_logic_op_greater_equal.cpp

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

#include <yggr/any_val/any_operator_mgr_basic_t_reg.hpp>
#include <yggr/any_val/any_operator_mgr_reg_macro.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace any_val
{

/*static*/ 
bool any_operator_mgr_basic_t_reg::s_register_base_type_greater_equal(void)
{
	return
		this_type::prv_s_register_base_type_greater_equal_eins()
		&& this_type::prv_s_register_base_type_greater_equal_zwei();
}

/*static*/ 
bool any_operator_mgr_basic_t_reg::prv_s_register_base_type_greater_equal_eins(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

	// bool
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, bool, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, bool, f64);

	// char
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, char, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, char, f64);

	//wchar_t
#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, wchar_t, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, char);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, wchar_t);

#	if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, c8);
#	endif // YGGR_NO_CHAR8_T

#	if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, c16);
#	endif // YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, c32);
#	endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, wchar_t, f64);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

	// c8
#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, c8, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, char);

#	if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, wchar_t);
#	endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, c8);

#	if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, c16);
#	endif // YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, c32);
#	endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c8, f64);
#endif // YGGR_NO_CHAR8_T

	// c16
#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, c16, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, char);

#	if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, wchar_t);
#	endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#	if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, c8);
#	endif // YGGR_NO_CHAR8_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, c16);

#	if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, c32);
#	endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c16, f64);
#endif // YGGR_NO_CHAR16_T

	// c32
#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, c32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, char);

#	if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, wchar_t);
#	endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#	if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, c8);
#	endif // YGGR_NO_CHAR8_T

#	if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, c16);
#	endif // YGGR_NO_CHAR16_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, c32);

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, c32, f64);
#endif // YGGR_NO_CHAR32_T

	//s8
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, s8, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s8, f64);

	//u8
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, u8, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u8, f64);

	//s16
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, s16, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s16, f64);

	//u16
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, u16, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u16, f64);

	//s32
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, s32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s32, f64);

	//u32
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, u32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u32, f64);

	//s64
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, s64, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, s64, f64);

	//u64
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, u64, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, u64, f64);

	//f32
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, f32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f32, f64);

	//f64
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, greater_equal, f64, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, greater_equal, f64, f64);

	return bright;
}

} // namespace any_val
} // namespace yggr
