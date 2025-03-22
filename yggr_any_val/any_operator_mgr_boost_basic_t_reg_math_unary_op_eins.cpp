//any_operator_mgr_boost_basic_t_reg.cpp

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

#include <yggr/any_val/any_operator_mgr_boost_basic_t_reg.hpp>
#include <yggr/any_val/any_operator_mgr_reg_macro.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace any_val
{

/*static*/ 
bool any_operator_mgr_boost_basic_t_reg::s_register_base_type_tail_decrement(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

	// --don't support bool_type
	//bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, char);
#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_decrement, f64);

	return bright;
}

/*static*/ 
bool any_operator_mgr_boost_basic_t_reg::s_register_base_type_head_decrement(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

	// --don't support bool_type
	//bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_decrement, f64);

	return bright;
}

/*static*/ 
bool any_operator_mgr_boost_basic_t_reg::s_register_base_type_tail_increment(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

	// --don't support bool_type
	//bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, tail_increment, f64);

	return bright;
}

/*static*/ 
bool any_operator_mgr_boost_basic_t_reg::s_register_base_type_head_increment(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

	// --don't support bool_type
	//bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, bool);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, u8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, u16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, u32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, u64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_MUTABLE_UNARY_OPERATOR(pmgr, head_increment, f64);

	return bright;
}

} // namespace any_val
} // namespace yggr
