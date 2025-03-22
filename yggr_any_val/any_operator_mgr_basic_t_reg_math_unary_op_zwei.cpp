//any_operator_mgr_basic_t_reg.cpp

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
bool any_operator_mgr_basic_t_reg::s_register_base_type_posivite(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, positive, f64);

	return bright;
}

/*static*/ 
bool any_operator_mgr_basic_t_reg::s_register_base_type_negate(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, char);

#if !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, wchar_t);
#endif // !(defined(YGGR_NO_CWCHAR) || defined(YGGR_AT_ANDROID))

#if !defined(YGGR_NO_CHAR8_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, c8);
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, c16);
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, c32);
#endif // YGGR_NO_CHAR32_T

	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, s8);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, s16);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, s32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, s64);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, f32);
	bright = bright && YGGR_PP_ANY_VAL_REGISTER_UNARY_OPERATOR(pmgr, negate, f64);

	return bright;
}

} // namespace any_val
} // namespace yggr
