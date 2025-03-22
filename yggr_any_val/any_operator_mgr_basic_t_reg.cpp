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
bool any_operator_mgr_basic_t_reg::s_init(void)
{
	return !!(any_operator_mgr_single_type::init_ins());
}

/*static*/
bool any_operator_mgr_basic_t_reg::s_init(bool binit_base)
{
	return (any_operator_mgr_single_type::init_ins())
			&& (binit_base? s_support_base() : true);
}

/*static*/
bool any_operator_mgr_basic_t_reg::s_support_base(void)
{
	any_operator_mgr_single_type::obj_ptr_type pmgr
		= any_operator_mgr_single_type::get_ins();

	if(!pmgr)
	{
		return false;
	}

	bool bright = true;

#if !defined(YGGR_ANY_VAL_OP_INIT_NOT_SUPPORT_BASE_TYPE)
	bright = bright && s_register_base_type_posivite();
	bright = bright && s_register_base_type_negate();
	bright = bright && s_register_base_type_head_increment();
	bright = bright && s_register_base_type_tail_increment();
	bright = bright && s_register_base_type_head_decrement();
	bright = bright && s_register_base_type_tail_decrement();
	bright = bright && s_register_base_type_set();
	bright = bright && s_register_base_type_plus();
	bright = bright && s_register_base_type_plus_set();
	bright = bright && s_register_base_type_minus();
	bright = bright && s_register_base_type_minus_set();
	bright = bright && s_register_base_type_multiplies();
	bright = bright && s_register_base_type_multiplies_set();
	bright = bright && s_register_base_type_divides();
	bright = bright && s_register_base_type_divides_set();
	bright = bright && s_register_base_type_modulus();
	bright = bright && s_register_base_type_modulus_set();
	bright = bright && s_register_base_type_logic_and();
	bright = bright && s_register_base_type_logic_or();
	bright = bright && s_register_base_type_logic_not();
	bright = bright && s_register_base_type_bitwise_and();
	bright = bright && s_register_base_type_bitwise_and_set();
	bright = bright && s_register_base_type_bitwise_or();
	bright = bright && s_register_base_type_bitwise_or_set();
	bright = bright && s_register_base_type_bitwise_xor();
	bright = bright && s_register_base_type_bitwise_xor_set();
	bright = bright && s_register_base_type_bitwise_left_shift();
	bright = bright && s_register_base_type_bitwise_left_shift_set();
	bright = bright && s_register_base_type_bitwise_right_shift();
	bright = bright && s_register_base_type_bitwise_right_shift_set();
	bright = bright && s_register_base_type_bitwise_not();
	bright = bright && s_register_base_type_equal_to();
	bright = bright && s_register_base_type_not_equal_to();
	bright = bright && s_register_base_type_less();
	bright = bright && s_register_base_type_less_equal();
	bright = bright && s_register_base_type_greater();
	bright = bright && s_register_base_type_greater_equal();
#endif // YGGR_ANY_VAL_OP_INIT_NOT_SUPPORT_BASE_TYPE

	return bright;
}

} // namespace any_val
} // namespace yggr
