//operator_wrap.hpp

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

#ifndef __YGGR_OPERATOR_WRAP_HPP__
#define __YGGR_OPERATOR_WRAP_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/move/move.hpp>
#include <yggr/math/modulus.hpp>

#include <yggr/type_traits/operator_result.hpp>
#include <yggr/type_traits/native_t.hpp>

#include <yggr/mplex/pointer_to_reference_t.hpp>

#include <boost/type_traits/add_pointer.hpp>

namespace yggr
{
namespace any_val
{

struct operator_wrap
{
	template<typename L> inline
	static L& self(L& l)
	{
		return l;
	}

	template<typename L> inline
	static typename positive_operator_result<L>::type
		positive(const L& l)
	{
		return +l;
	}

	template<typename L> inline
	static typename negate_operator_result<L>::type
		negate(const L& l)
	{
		return -l;
	}

	// *ptr
	template<typename L> inline
	static typename mplex::pointer_to_reference_t<L>::type
		dereference(L& l)
	{
		return *l;
	}

	template<typename L> inline
	static typename mplex::pointer_to_value_t<L>::type
		dereference_readonly(L& l)
	{
		typedef typename mplex::pointer_to_value_t<L>::type ret_type;
		return l? *l : ret_type();
	}

	template<typename L> inline
	static typename boost::add_pointer<L>::type
		addressof(L& l)
	{
		return &l;
	}

	template<typename L> inline
	static std::size_t sizeof_(L& l)
	{
		return sizeof(l);
	}

	template<typename L> inline
	static L& head_increment(L& l)
	{
		return ++l;
	}

	template<typename L> inline
	static L tail_increment(L& l)
	{
		return l++;
	}

	template<typename L> inline
	static L& head_decrement(L& l)
	{
		return --l;
	}

	template<typename L> inline
	static L tail_decrement(L& l)
	{
		return l--;
	}

	template<typename L, typename R> inline
	static L& set(L& l, const R& r)
	{
		l = r;
		return l;
	}

	template<typename L, typename R> inline
	static L& set_move(L& l, R& r)
	{
		l = boost::move(r);
		return l;
	}

	template<typename L, typename R> inline
	static typename plus_operator_result<L, R>::type
		plus(const L& l, const R& r)
	{
		return l + r;
	}

	template<typename L, typename R> inline
	static L& plus_set(L& l, const R& r)
	{
		l += r;
		return l;
	}

	template<typename L, typename R> inline
	static typename minus_operator_result<L, R>::type
		minus(const L& l, const R& r)
	{
		return l - r;
	}

	template<typename L, typename R> inline
	static L& minus_set(L& l, const R& r)
	{
		l -= r;
		return l;
	}

	template<typename L, typename R> inline
	static typename multiplies_operator_result<L, R>::type
		multiplies(const L& l, const R& r)
	{
		return l * r;
	}

	template<typename L, typename R> inline
	static L& multiplies_set(L& l, const R& r)
	{
		l *= r;
		return l;
	}

	template<typename L, typename R> inline
	static typename divides_operator_result<L, R>::type
		divides(const L& l, const R& r)
	{
		return l / r;
	}

	template<typename L, typename R> inline
	static L& divides_set(L& l, const R& r)
	{
		l /= r;
		return l;
	}

	template<typename L, typename R> inline
	static typename modulus_operator_result<L, R>::type
		modulus(const L& l, const R& r)
	{
		return math::modulus(l, r);
	}

	template<typename L, typename R> inline
	static L& modulus_set(L& l, const R& r)
	{
		return math::modulus_set(l, r);
	}

	template<typename L, typename R> inline
	static bool logic_and(const L& l, const R& r)
	{
		return l && r;
	}

	template<typename L, typename R> inline
	static bool logic_or(const L& l, const R& r)
	{
		return l || r;
	}

	template<typename L> inline
	static bool logic_not(const L& l)
	{
		return !l;
	}

	template<typename L, typename R> inline
	static typename bitwise_and_operator_result<L, R>::type
		bitwise_and(const L& l, const R& r)
	{
		return l & r;
	}

	template<typename L, typename R> inline
	static L& bitwise_and_set(L& l, const R& r)
	{
		l &= r;
		return l;
	}

	template<typename L, typename R> inline
	static typename bitwise_or_operator_result<L, R>::type
		bitwise_or(const L& l, const R& r)
	{
		return l | r;
	}

	template<typename L, typename R> inline
	static L& bitwise_or_set(L& l, const R& r)
	{
		l |= r;
		return l;
	}

	template<typename L, typename R> inline
	static typename bitwise_xor_operator_result<L, R>::type
		bitwise_xor(const L& l, const R& r)
	{
		return l ^ r;
	}

	template<typename L, typename R> inline
	static L& bitwise_xor_set(L& l, const R& r)
	{
		l ^= r;
		return l;
	}

	template<typename L, typename R> inline
	static typename bitwise_left_shift_operator_result<L, R>::type
		bitwise_left_shift(const L& l, const R& r)
	{
		return l << r;
	}

	template<typename L, typename R> inline
	static L& bitwise_left_shift_set(L& l, const R& r)
	{
		l <<= r;
		return l;
	}

	template<typename L, typename R> inline
	static typename bitwise_right_shift_operator_result<L, R>::type
		bitwise_right_shift(const L& l, const R& r)
	{
		return l >> r;
	}

	template<typename L, typename R> inline
	static L& bitwise_right_shift_set(L& l, const R& r)
	{
		l >>= r;
		return l;
	}

	template<typename L> inline
	static typename bitwise_not_operator_result<L>::type
		bitwise_not(const L& l)
	{
		return ~(l);
	}

	template<typename L, typename R> inline
	static bool equal_to(const L& l, const R& r)
	{
		return l == r;
	}

	template<typename L, typename R> inline
	static bool not_equal_to(const L& l, const R& r)
	{
		return l != r;
	}

	template<typename L, typename R> inline
	static bool less(const L& l, const R& r)
	{
		return l < r;
	}

	template<typename L, typename R> inline
	static bool less_equal(const L& l, const R& r)
	{
		return l <= r;
	}

	template<typename L, typename R> inline
	static bool greater(const L& l, const R& r)
	{
		return l > r;
	}

	template<typename L, typename R> inline
	static bool greater_equal(const L& l, const R& r)
	{
		return l >= r;
	}

	// ---------------custom-------------------

	template<typename Ret, typename L> inline
	static Ret self_custom(L& l)
	{
		return l;
	}

	template<typename Ret, typename L> inline
	static Ret positive_custom(const L& l)
	{
		return +l;
	}

	template<typename Ret, typename L> inline
	static Ret negate_custom(const L& l)
	{
		return -l;
	}

	template<typename Ret, typename L> inline
	static Ret dereference_custom(L& l)
	{
		return *l;
	}

	template<typename Ret, typename L> inline
	static Ret dereference_readonly_custom(L& l)
	{
		return l? *l : Ret();
	}

	template<typename Ret, typename L> inline
	static Ret head_increment_custom(L& l)
	{
		return ++l;
	}

	template<typename Ret, typename L> inline
	static Ret head_decrement_custom(L& l)
	{
		return --l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret set_custom(L& l, const R& r)
	{
		l = r;
		return l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret set_move_custom(L& l, R& r)
	{
		l = boost::move(r);
		return l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret plus_set_custom(L& l, const R& r)
	{
		l += r;
		return l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret minus_set_custom(L& l, const R& r)
	{
		l -= r;
		return l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret multiplies_set_custom(L& l, const R& r)
	{
		l *= r;
		return l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret divides_set_custom(L& l, const R& r)
	{
		l /= r;
		return l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret modulus_set_custom(L& l, const R& r)
	{
		return math::modulus_set(l, r);
	}

	template<typename Ret, typename L, typename R> inline
	static Ret bitwise_and_set_custom(L& l, const R& r)
	{
		l &= r;
		return l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret bitwise_or_set_custom(L& l, const R& r)
	{
		l |= r;
		return l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret bitwise_xor_set_custom(L& l, const R& r)
	{
		l ^= r;
		return l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret bitwise_left_shift_set_custom(L& l, const R& r)
	{
		l <<= r;
		return l;
	}

	template<typename Ret, typename L, typename R> inline
	static Ret bitwise_right_shift_set_custom(L& l, const R& r)
	{
		l >>= r;
		return l;
	}

};

} // namespace any_val
} // namespace yggr

#endif // __YGGR_OPERATOR_WRAP_HPP__
