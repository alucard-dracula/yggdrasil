// any_operator_mgr_basic_t_reg.cpp

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
bool any_operator_mgr_basic_t_reg::s_register_base_type_minus(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

	// bool
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, bool, f64);

	// char
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, char, f64);

	//wchar_t
#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, char);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, wchar_t);

#	if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, c8);
#	endif // YGGR_NO_CHAR8_T

#	if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, c16);
#	endif // YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, c32);
#	endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, wchar_t, f64);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

	// c8
#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, char);

#	if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, wchar_t);
#	endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, c8);

#	if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, c16);
#	endif // YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, c32);
#	endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c8, f64);
#endif // YGGR_NO_CHAR8_T

	// c16
#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, char);

#	if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, wchar_t);
#	endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#	if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, c8);
#	endif // YGGR_NO_CHAR8_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, c16);

#	if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, c32);
#	endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c16, f64);
#endif // YGGR_NO_CHAR16_T

	// c32
#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, char);

#	if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, wchar_t);
#	endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#	if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, c8);
#	endif // YGGR_NO_CHAR8_T

#	if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, c16);
#	endif // YGGR_NO_CHAR16_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, c32);

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, c32, f64);
#endif // YGGR_NO_CHAR32_T

	//s8
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s8, f64);

	//u8
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u8, f64);

	//s16
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s16, f64);

	//u16
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u16, f64);

	//s32
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s32, f64);

	//u32
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u32, f64);

	//s64
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, s64, f64);

	//u64
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, u64, f64);

	//f32
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f32, f64);

	//f64
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, minus, f64, f64);

	return bright;
}


} // namespace any_val
} // namespace yggr
