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
bool any_operator_mgr_basic_t_reg::s_register_base_type_divides(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

	// bool
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, bool, f64);

	// char
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, char, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, char, f64);

	//wchar_t
#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, wchar_t, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, char);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, wchar_t);

#	if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, c8);
#	endif // YGGR_NO_CHAR8_T

#	if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, c16);
#	endif // YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, c32);
#	endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, wchar_t, f64);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

	// c8
#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, c8, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, char);

#	if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, wchar_t);
#	endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, c8);

#	if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, c16);
#	endif // YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, c32);
#	endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c8, f64);
#endif // YGGR_NO_CHAR8_T

	// c16
#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, c16, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, char);

#	if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, wchar_t);
#	endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#	if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, c8);
#	endif // YGGR_NO_CHAR8_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, c16);

#	if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, c32);
#	endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c16, f64);
#endif // YGGR_NO_CHAR16_T

	// c32
#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, c32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, char);

#	if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, wchar_t);
#	endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#	if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, c8);
#	endif // YGGR_NO_CHAR8_T

#	if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, c16);
#	endif // YGGR_NO_CHAR16_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, c32);

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, c32, f64);
#endif // YGGR_NO_CHAR32_T

	//s8
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, s8, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s8, f64);

	//u8
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, u8, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u8, f64);

	//s16
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, s16, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s16, f64);

	//u16
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, u16, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u16, f64);

	//s32
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, s32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s32, f64);

	//u32
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, u32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u32, f64);

	//s64
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, s64, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, s64, f64);

	//u64
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, u64, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, u64, f64);

	//f32
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, f32, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f32, f64);

	//f64
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR_UNSAFE(pmgr, divides, f64, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_BINARY_OPERATOR(pmgr, divides, f64, f64);

	return bright;
}

} // namespace any_val
} // namespace yggr
