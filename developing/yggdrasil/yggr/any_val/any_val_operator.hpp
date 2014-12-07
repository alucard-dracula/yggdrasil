//any_val_operator.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_ANY_VAL_ANY_VAL_OPERATOR_HPP__
#define __YGGR_ANY_VAL_ANY_VAL_OPERATOR_HPP__

#include <yggr/any_val/any.hpp>

namespace yggr
{
namespace any_val
{

template<typename Type_Op> //this template plese extern from base_any_type_operator.hpp
class any_val_operator : public Type_Op
{
public:
	typedef Type_Op base_type;
	typedef typename base_type::any_type any_type;
	typedef any_val_operator<base_type> this_type;

public:

	template<typename L>
	static const L negate(const L& l)
	{
		return base_type::negate(l);
	}

	template<typename L>
	static L& befor_plus_plus(L& l)
	{
		return base_type::befor_plus_plus(l);
	}

	template<typename L>
	static const L after_plus_plus(L& l)
	{
		return base_type::after_plus_plus(l);
	}

	template<typename L>
	static L& befor_minus_minus(L& l)
	{
		return base_type::befor_minus_minus(l);
	}

	template<typename L>
	static const L after_minus_minus(L& l)
	{
		return base_type::after_minus_minus(l);
	}

	template<typename L, typename R>
	static void set(L& l, const R& r)
	{
		base_type::set(l, r);
	}

	static void set(any_type& l, const any_type& r)
	{
		base_type::set(l, r);
	}

	template<typename L>
	static void set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::set(tl, r);
		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::set(l, r);
	}

	template<typename L, typename R>
	static const L plus(const L& l, const R& r)
	{
		return base_type::plus(l, r);
	}

	static const any_type plus(const any_type& l, const any_type& r)
	{
		return base_type::plus(l, r);
	}

	template<typename L>
	static const L plus(const L& l, const any_type& r)
	{
		any_type tl(l);
		any_type rst = base_type::plus(tl, r);
		L *p = yggr::any_cast<L>(&rst);

		if(!p)
		{
			return L();
		}

		return *p;
	}

	template<typename R>
	static const any_type plus(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::plus(l, tr);
	}

	template<typename L, typename R>
	static void plus_set(L& l, const R& r)
	{
		base_type::plus_set(l, r);
	}

	static void plus_set(any_type& l, const any_type& r)
	{
		base_type::plus_set(l, r);
	}

	template<typename L>
	static void plus_set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::plus_set(tl, r);
		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void plus_set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::plus_set(l, tr);
	}

	template<typename L, typename R>
	static const L minus(const L& l, const R& r)
	{
		return base_type::minus(l, r);
	}

	static const any_type minus(const any_type& l, const any_type& r)
	{
		return base_type::minus(l, r);
	}

	template<typename L>
	static const L minus(const L& l, const any_type& r)
	{
		any_type tl(l);
		any_type rst = base_type::minus(tl, r);

		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return L();
		}

		return *p;
	}

	template<typename R>
	static const any_type minus(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::minus(l, tr);
	}

	template<typename L, typename R>
	static void minus_set(L& l, const R& r)
	{
		base_type::minus_set(l, r);
	}

	static void minus_set(any_type& l, const any_type& r)
	{
		base_type::minus_set(l, r);
	}

	template<typename L>
	static void minus_set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::minus_set(tl, r);
		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void minus_set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::minus_set(l, tr);
	}

	template<typename L, typename R>
	static const L multiplies(const L& l, const R& r)
	{
		return base_type::multiplies(l, r);
	}

	static const any_type multiplies(const any_type& l, const any_type& r)
	{
		return base_type::multiplies(l, r);
	}

	template<typename L>
	static const L multiplies(const L& l, const any_type& r)
	{
		any_type tl(l);
		any_type rst = base_type::multiplies(tl, r);

		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return L();
		}

		return *p;
	}

	template<typename R>
	static const any_type multiplies(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::multiplies(l, tr);
	}

	template<typename L, typename R>
	static void multiplies_set(L& l, const R& r)
	{
		base_type::multiplies_set(l, r);
	}

	static void multiplies_set(any_type& l, const any_type& r)
	{
		base_type::multiplies_set(l, r);
	}

	template<typename L>
	static void multiplies_set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::multiplies_set(tl, r);
		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void multiplies_set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::multiplies_set(l, tr);
	}

	template<typename L, typename R>
	static const L divides(const L& l, const R& r)
	{
		return base_type::divides(l, r);
	}

	static const any_type divides(const any_type& l, const any_type& r)
	{
		return base_type::divides(l, r);
	}

	template<typename L>
	static const L divides(const L& l, const any_type& r)
	{
		any_type tl(l);
		any_type rst = base_type::divides(tl, r);

		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return L();
		}

		return *p;
	}

	template<typename R>
	static const any_type divides(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::divides(l, tr);
	}

	template<typename L, typename R>
	static void divides_set(L& l, const R& r)
	{
		base_type::divides_set(l, r);
	}

	static void divides_set(any_type& l, const any_type& r)
	{
		base_type::divides_set(l, r);
	}


	template<typename L>
	static void divides_set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::divides_set(tl, r);
		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void divides_set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::divides_set(l, tr);
	}

	template<typename L, typename R>
	static const L modulus(const L& l, const R& r)
	{
		return base_type::modulus(l, r);
	}

	static const any_type modulus(const any_type& l, const any_type& r)
	{
		return base_type::modulus(l, r);
	}

	template<typename L>
	static const L modulus(const L& l, const any_type& r)
	{
		any_type tl(l);
		any_type rst = base_type::modulus(tl, r);

		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return L();
		}

		return *p;
	}

	template<typename R>
	static const any_type modulus(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::modulus(l, tr);
	}

	template<typename L, typename R>
	static void modulus_set(L& l, const R& r)
	{
		base_type::modulus_set(l, r);
	}

	static void modulus_set(any_type& l, const any_type& r)
	{
		base_type::modulus_set(l, r);
	}

	template<typename L>
	static void modulus_set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::modulus_set(tl, r);
		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void modulus_set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::modulus_set(l, tr);
	}

	template<typename L, typename R>
	static bool logic_and(const L& l, const R& r)
	{
		return base_type::logic_and(l, r);
	}

	static bool logic_and(const any_type& l, const any_type& r)
	{
		return base_type::logic_and(l, r);
	}

	template<typename L>
	static bool logic_and(const L& l, const any_type& r)
	{
		any_type tl(l);
		return base_type::logic_and(tl, r);
	}

	template<typename R>
	static bool logic_and(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::logic_and(l, tr);
	}

	template<typename L, typename R>
	static bool logic_or(const L& l, const R& r)
	{
		return base_type::logic_or(l, r);
	}

	static bool logic_or(const any_type& l, const any_type& r)
	{
		return base_type::logic_or(l, r);
	}

	template<typename L>
	static bool logic_or(const L& l, const any_type& r)
	{
		any_type tl(l);
		return base_type::logic_or(tl, r);
	}

	template<typename R>
	static bool logic_or(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::logic_or(l, tr);
	}

	template<typename L>
	static bool logic_not(const L& l)
	{
		return base_type::logic_not(l);
	}

	template<typename L, typename R>
	static const L bit_and(const L& l, const R& r)
	{
		return base_type::bit_and(l, r);
	}

	static const any_type bit_and(const any_type& l, const any_type& r)
	{
		return base_type::bit_and(l, r);
	}

	template<typename L>
	static const L bit_and(const L& l, const any_type& r)
	{
		any_type tl(l);
		any_type rst = base_type::bit_and(tl, r);

		L *p = yggr::any_cast<L>(&rst);

		if(!p)
		{
			return L();
		}

		return *p;
	}

	template<typename R>
	static const any_type bit_and(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::bit_and(l, tr);
	}

	template<typename L, typename R>
	static void bit_and_set(L& l, const R& r)
	{
		base_type::bit_and_set(l, r);
	}

	static void bit_and_set(any_type& l, const any_type& r)
	{
		base_type::bit_and_set(l, r);
	}


	template<typename L>
	static void bit_and_set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::bit_and_set(tl, r);

		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void bit_and_set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::bit_and_set(l, tr);
	}

	template<typename L, typename R>
	static const L bit_or(const L& l, const R& r)
	{
		return base_type::bit_or(l, r);
	}

	static const any_type bit_or(const any_type& l, const any_type& r)
	{
		return base_type::bit_or(l, r);
	}

	template<typename L>
	static const L bit_or(const L& l, const any_type& r)
	{
		any_type tl(l);
		any_type rst = base_type::bit_or(tl, r);

		L *p = yggr::any_cast<L>(&rst);

		if(!p)
		{
			return L();
		}

		return *p;
	}

	template<typename R>
	static const any_type bit_or(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::bit_or(l, tr);
	}

	template<typename L, typename R>
	static void bit_or_set(L& l, const R& r)
	{
		base_type::bit_or_set(l, r);
	}

	static void bit_or_set(any_type& l, const any_type& r)
	{
		base_type::bit_or_set(l, r);
	}

	template<typename L>
	static void bit_or_set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::bit_or_set(tl, r);

		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void bit_or_set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::bit_or_set(l, tr);
	}

	template<typename L, typename R>
	static const L bit_xor(const L& l, const R& r)
	{
		return base_type::bit_xor(l, r);
	}

	static const any_type bit_xor(const any_type& l, const any_type& r)
	{
		return base_type::bit_xor(l, r);
	}

	template<typename L>
	static const L bit_xor(const L& l, const any_type& r)
	{
		any_type tl(l);
		any_type rst = base_type::bit_xor(tl, r);

		L *p = yggr::any_cast<L>(&rst);

		if(!p)
		{
			return L();
		}

		return *p;
	}

	template<typename R>
	static const any_type bit_xor(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::bit_xor(l, tr);
	}

	template<typename L, typename R>
	static void bit_xor_set(L& l, const R& r)
	{
		base_type::bit_xor_set(l, r);
	}

	static void bit_xor_set(any_type& l, const any_type& r)
	{
		base_type::bit_xor_set(l, r);
	}

	template<typename L>
	static void bit_xor_set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::bit_xor_set(tl, r);

		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void bit_xor_set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::bit_xor_set(l, tr);
	}

	template<typename L, typename R>
	static const L bit_left_trans(const L& l, const R& r)
	{
		return base_type::bit_left_trans(l, r);
	}

	static const any_type bit_left_trans(const any_type& l, const any_type& r)
	{
		return base_type::bit_left_trans(l, r);
	}

	template<typename L>
	static const L bit_left_trans(const L& l, const any_type& r)
	{
		any_type tl(l);
		any_type rst = base_type::bit_left_trans(tl, r);

		L *p = yggr::any_cast<L>(&rst);

		if(!p)
		{
			return L();
		}

		return *p;
	}

	template<typename R>
	static const any_type bit_left_trans(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::bit_left_trans(l, tr);
	}

	template<typename L, typename R>
	static void bit_left_trans_set(L& l, const R& r)
	{
		base_type::bit_left_trans_set(l, r);
	}

	static void bit_left_trans_set(any_type& l, const any_type& r)
	{
		base_type::bit_left_trans_set(l, r);
	}

	template<typename L>
	static void bit_left_trans_set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::bit_left_trans_set(tl, r);

		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void bit_left_trans_set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::bit_left_trans_set(l, tr);
	}


	template<typename L, typename R>
	static const L bit_right_trans(const L& l, const R& r)
	{
		return base_type::bit_right_trans(l, r);
	}

	static const any_type bit_right_trans(const any_type& l, const any_type& r)
	{
		return base_type::bit_right_trans(l, r);
	}

	template<typename L>
	static const L bit_right_trans(const L& l, const any_type& r)
	{
		any_type tl(l);
		any_type rst = base_type::bit_right_trans(tl, r);

		L *p = yggr::any_cast<L>(&rst);

		if(!p)
		{
			return L();
		}

		return *p;
	}

	template<typename R>
	static const any_type bit_right_trans(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::bit_right_trans(l, tr);
	}

	template<typename L, typename R>
	static void bit_right_trans_set(L& l, const R& r)
	{
		base_type::bit_right_trans_set(l, r);
	}

	static void bit_right_trans_set(any_type& l, const any_type& r)
	{
		base_type::bit_right_trans_set(l, r);
	}

	template<typename L>
	static void bit_right_trans_set(L& l, const any_type& r)
	{
		any_type tl(l);
		base_type::bit_right_trans_set(tl, r);

		L *p = yggr::any_cast<L>(&tl);

		if(!p)
		{
			return;
		}

		l = *p;
	}

	template<typename R>
	static void bit_right_trans_set(any_type& l, const R& r)
	{
		any_type tr(r);
		base_type::bit_right_trans_set(l, tr);
	}


	template<typename L, typename R>
	static const L bit_not(const L& l)
	{
		return base_type::bit_not(l);
	}

	template<typename L, typename R>
	static bool equal_to(const L& l, const R& r)
	{
		return base_type::equal_to(l, r);
	}

	static bool equal_to(const any_type& l, const any_type& r)
	{
		return base_type::equal_to(l, r);
	}

	template<typename L>
	static bool equal_to(const L& l, const any_type& r)
	{
		any_type tl(l);
		return base_type::equal_to(tl, r);
	}

	template<typename R>
	static bool equal_to(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::equal_tp(l, tr);
	}

	template<typename L, typename R>
	static bool not_equal_to(const L& l, const R& r)
	{
		return base_type::not_equal_to(l, r);
	}

	static bool not_equal_to(const any_type& l, const any_type& r)
	{
		return base_type::not_equal_to(l, r);
	}

	template<typename L>
	static bool not_equal_to(const L& l, const any_type& r)
	{
		any_type tl(l);
		return base_type::not_equal_to(tl, r);
	}

	template<typename R>
	static bool not_equal_to(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::not_equal_to(l, tr);
	}

	template<typename L, typename R>
	static bool less(const L& l, const R& r)
	{
		return base_type::less(l, r);
	}

	static bool less(const any_type& l, const any_type& r)
	{
		return base_type::less(l, r);
	}

	template<typename L>
	static bool less(const L& l, const any_type& r)
	{
		any_type tl(l);
		return base_type::less(tl, r);
	}

	template<typename R>
	static bool less(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::less(l, tr);
	}

	template<typename L, typename R>
	static bool less_equal(const L& l, const R& r)
	{
		return base_type::less_equal(l, r);
	}

	static bool less_equal(const any_type& l, const any_type& r)
	{
		return base_type::less_equal(l, r);
	}

	template<typename L>
	static bool less_equal(const L& l, const any_type& r)
	{
		any_type tl(l);
		return base_type::less_equal(tl, r);
	}

	template<typename R>
	static bool less_equal(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::less_equal(l, tr);
	}

	template<typename L, typename R>
	static bool greater(const L& l, const R& r)
	{
		return base_type::greater(l, r);
	}

	static bool greater(const any_type& l, const any_type& r)
	{
		return base_type::greater(l, r);
	}

	template<typename L>
	static bool greater(const L& l, const any_type& r)
	{
		any_type tl(l);
		return base_type::greater(tl, r);
	}

	template<typename R>
	static bool greater(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::greater(l, tr);
	}

	template<typename L, typename R>
	static bool greater_equal(const L& l, const R& r)
	{
		return base_type::greater_equal(l, r);
	}

	static bool greater_equal(const any_type& l, const any_type& r)
	{
		return base_type::greater_equal(l, r);
	}

	template<typename L>
	static bool greater_equal(const L& l, const any_type& r)
	{
		any_type tl(l);
		return base_type::greater_equal(tl, r);
	}

	template<typename R>
	static bool greater_equal(const any_type& l, const R& r)
	{
		any_type tr(r);
		return base_type::greater_equal(l, tr);
	}
};

} // namespace any_params
} // namespace yggr

#endif //__YGGR_ANY_VAL_ANY_VAL_OPERATOR_HPP__
