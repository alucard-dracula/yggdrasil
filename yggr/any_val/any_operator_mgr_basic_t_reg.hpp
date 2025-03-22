//any_operator_mgr_basic_t_reg.hpp

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

#ifndef __YGGR_ANY_VAL_ANY_OPERATOR_MGR_BASIC_REG_HPP__
#define __YGGR_ANY_VAL_ANY_OPERATOR_MGR_BASIC_REG_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/ptr_single.hpp>
#include <yggr/any_val/any_operator_mgr.hpp>

namespace yggr
{
namespace any_val
{
struct any_operator_mgr_basic_t_reg
{
private:
	typedef any_operator_mgr_basic_t_reg this_type;

public:
	typedef any_operator_mgr< ::yggr::any> any_operator_mgr_type;
	typedef yggr::ptr_single<any_operator_mgr_type> any_operator_mgr_single_type;

public:
	static bool s_init(void);
	static bool s_init(bool binit_base);

	static bool s_support_base(void);

	static bool s_register_base_type_greater_equal(void);
	static bool s_register_base_type_greater(void);

	static bool s_register_base_type_less_equal(void);
	static bool s_register_base_type_less(void);

	static bool s_register_base_type_not_equal_to(void);
	static bool s_register_base_type_equal_to(void);

	static bool s_register_base_type_bitwise_not(void);

	static bool s_register_base_type_bitwise_right_shift_set(void);
	static bool s_register_base_type_bitwise_right_shift(void);

	static bool s_register_base_type_bitwise_left_shift_set(void);
	static bool s_register_base_type_bitwise_left_shift(void);

	static bool s_register_base_type_bitwise_xor_set(void);
	static bool s_register_base_type_bitwise_xor(void);

	static bool s_register_base_type_bitwise_or_set(void);
	static bool s_register_base_type_bitwise_or(void);

	static bool s_register_base_type_bitwise_and_set(void);
	static bool s_register_base_type_bitwise_and(void);

	static bool s_register_base_type_logic_not(void);
	static bool s_register_base_type_logic_or(void);
	static bool s_register_base_type_logic_and(void);

	static bool s_register_base_type_modulus_set(void);
	static bool s_register_base_type_modulus(void);

	static bool s_register_base_type_divides_set(void);
	static bool s_register_base_type_divides(void);

	static bool s_register_base_type_multiplies_set(void);
	static bool s_register_base_type_multiplies(void);

	static bool s_register_base_type_minus_set(void);
	static bool s_register_base_type_minus(void);

	static bool s_register_base_type_plus_set(void);
	static bool s_register_base_type_plus(void);

	static bool s_register_base_type_set(void);

	static bool s_register_base_type_tail_decrement(void);
	static bool s_register_base_type_head_decrement(void);

	static bool s_register_base_type_tail_increment(void);
	static bool s_register_base_type_head_increment(void);
	
	static bool s_register_base_type_posivite(void);
	static bool s_register_base_type_negate(void);

private:
	static bool prv_s_register_base_type_plus_set_eins(void);
	static bool prv_s_register_base_type_plus_set_zwei(void);

	static bool prv_s_register_base_type_plus_eins(void);
	static bool prv_s_register_base_type_plus_zwei(void);

	static bool prv_s_register_base_type_equal_to_eins(void);
	static bool prv_s_register_base_type_equal_to_zwei(void);

	static bool prv_s_register_base_type_not_equal_to_eins(void);
	static bool prv_s_register_base_type_not_equal_to_zwei(void);

	static bool prv_s_register_base_type_less_equal_eins(void);
	static bool prv_s_register_base_type_less_equal_zwei(void);

	static bool prv_s_register_base_type_less_eins(void);
	static bool prv_s_register_base_type_less_zwei(void);

	static bool prv_s_register_base_type_greater_equal_eins(void);
	static bool prv_s_register_base_type_greater_equal_zwei(void);

	static bool prv_s_register_base_type_greater_eins(void);
	static bool prv_s_register_base_type_greater_zwei(void);

	static bool prv_s_register_base_type_set_eins(void);
	static bool prv_s_register_base_type_set_zwei(void);

};

} // namespace any_val
} // namespace yggr

#endif // __YGGR_ANY_VAL_ANY_OPERATOR_MGR_BASIC_REG_HPP__