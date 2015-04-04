//non_any_type_operator.hpp

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

#ifndef __YGGR_ANY_VAL_NON_ANY_TYPE_OPERATOR_HPP__
#define __YGGR_ANY_VAL_NON_ANY_TYPE_OPERATOR_HPP__

#include <functional>
#include <cassert>
#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/any_val/any.hpp>
#include <yggr/any_val/any_set.hpp> // ANY_SET
#include <yggr/nonable/noncreateable.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning( disable : 4018)
#	pragma warning( disable : 4244)
#	pragma warning( disable : 4800)
#	pragma warning( disable : 4804)
#	pragma warning( disable : 4805)
#	pragma warning( disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace any_val
{

class non_any_type_operator : private nonable::noncreateable
{
public:

	typedef yggr::any any_type;
	typedef non_any_type_operator this_type;
	
public:
	template<typename L, typename Handler>
	static const L math_op(const L& l, const Handler& handler)
	{
		return handler(l);
	}

	template<typename L, typename R, typename Handler>
	static const L math_op(const L& l, const R& r, const Handler& handler)
	{
		any_type ta(handler(l, r));
		return ta;
	}

	template<typename L, typename R, typename Handler>
	static void set_op(L& l, const R& r, const Handler& handler)
	{
		handler(l, r);
	}

	template<typename L, typename Handler>
	static bool logic_op(const L& l, const Handler& handler)
	{
		return handler(l);
	}

	template<typename L, typename R, typename Handler>
	static bool logic_op(const L& l, const R& r, const Handler& handler)
	{
		return handler(l, r);
	}

//public:
protected:
	template<typename L>
	static const L negate(const L& l)
	{
		assert(!this_type::is_any<L>());
		return -l;
	}

	static const any_type negate(const any_type& l)
	{
		assert(false);
		return l;
	}

	template<typename L>
	static L& befor_plus_plus(L& l)
	{
		assert(!this_type::is_any<L>());
		return ++l;
	}

	static any_type& befor_plus_plus(any_type& l)
	{
		assert(false);
		return l;
	}

	template<typename L>
	static const L after_plus_plus(L& l)
	{
		assert(!is_any<L>());
		L rst(l);
		++l;
		return rst;
	}

	static const any_type after_plus_plus(any_type& l)
	{
		assert(false);
		return l;
	}

	template<typename L>
	static L& befor_minus_minus(L& l)
	{
		assert(!this_type::is_any<L>());
		return --l;
	}

	static any_type& befor_minus_minus(any_type& l)
	{
		assert(false);
		return l;
	}

	template<typename L>
	static const L after_minus_minus(L& l)
	{
		assert(!is_any<L>());
		L rst = l;
		--l;
		return rst;
	}

	static const any_type after_minus_minus(any_type& l)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static void set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l = r;
	}

	static void set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L, typename R>
	static const L plus(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l + r;
		return rst;
	}

	static const any_type plus(const any_type& l, const any_type& r)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static const void plus_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l += r;
	}

	static const void plus_set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L, typename R>
	static const L minus(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst(l - r);
		return rst;
	}

	static const any_type minus(const any_type& l, const any_type& r)
	{
		assert(false);
		return l;
	}


	template<typename L, typename R>
	static void minus_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l -= r;
	}

	static void minus_set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L, typename R>
	static const L multiplies(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst(l * r);
		return rst;
	}

	static const any_type multiplies(const any_type& l, const any_type& r)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static void multiplies_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l *= r;
	}

	static void multiplies_set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L, typename R>
	static const L divides(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		if(!r)
		{
			return L();
		}
		
		L rst(l / r);
		return rst;
	}

	static const any_type divides(const any_type& l, const any_type& r)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static void divides_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));

		if(!r)
		{
			return;
		}

		l /= r;
	}

	static void divides_set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L, typename R>
	static const L modulus(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));

		if(!r)
		{
			return L();
		}

		L rst = (l % r);
		return rst;
	}

	static const any_type modulus(const any_type& l, const any_type& r)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static void modulus_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));

		if(!r)
		{
			return;
		}

		l %= r;
	}

	static void modulus_set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L, typename R>
	static bool logic_and(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l && r;
	}

	static bool logic_and(const any_type& l, const any_type& r)
	{
		assert(false);
		return false;
	}

	template<typename L, typename R>
	static bool logic_or(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));

		return l || r;
	}

	static bool logic_or(const any_type& l, const any_type& r)
	{
		assert(false);
		return false;
	}

	template<typename L>
	static bool logic_not(const L& l)
	{
		assert(!(this_type::is_any<L>()));
		return !l;
	}

	static bool logic_not(const any_type& l)
	{
		assert(false);
		return false;
	}

	template<typename L, typename R>
	static const L bit_and(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l & r;
		return rst;
	}

	static const any_type bit_and(const any_type& l, const any_type& r)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static void bit_and_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l &= r;
	}

	static void bit_and_set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L, typename R>
	static const L bit_or(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l | r;
		return rst;
	}

	static const any_type bit_or(const any_type& l, const any_type& r)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static void bit_or_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l |= r;
	}

	static void bit_or_set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L, typename R>
	static const L bit_xor(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l ^ r;
		return rst;
	}

	static const any_type bit_xor(const any_type& l, const any_type& r)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static void bit_xor_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l ^= r;
	}

	static void bit_xor_set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L, typename R>
	static const L bit_left_trans(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l << r;
		return rst;
	}

	static any_type bit_left_trans(const any_type& l, const any_type& r)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static void bit_left_trans_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l <<= r;
	}

	static void bit_left_trans_set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L, typename R>
	static const L bit_right_trans(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l >> r;
		return rst;
	}

	static const any_type bit_right_trans(const any_type& l, const any_type& r)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static void bit_right_trans_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l >>= r;
	}

	static void bit_right_trans_set(any_type& l, const any_type& r)
	{
		assert(false);
	}

	template<typename L>
	static L bit_not(const L& l)
	{
		assert(!(this_type::is_any<L>()));
		L rst = ~l;
		return rst;
	}

	static const any_type bit_not(const any_type& l)
	{
		assert(false);
		return l;
	}

	template<typename L, typename R>
	static bool equal_to(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l == r;
	}

	static bool equal_to(const any_type& l, const any_type& r)
	{
		assert(false);
		return false;
	}

	template<typename L, typename R>
	static bool not_equal_to(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l != r;
	}

	static bool not_equal_to(const any_type& l, const any_type& r)
	{
		assert(false);
		return false;
	}

	template<typename L, typename R>
	static bool less(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l < r;
	}
	
	static bool less(const any_type& l, const any_type& r)
	{
		assert(false);
		return false;
	}

	template<typename L, typename R>
	static bool less_equal(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l <= r;
	}

	static bool less_equal(const any_type& l, const any_type& r)
	{
		assert(false);
		return false;
	}

	template<typename L, typename R>
	static bool greater(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l > r;
	}

	static bool greater(const any_type& l, const any_type& r)
	{
		assert(false);
		return false;
	}

	template<typename L, typename R>
	static bool greater_equal(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l >= r;
	}

	static bool greater_equal(const any_type& l, const any_type& r)
	{
		assert(false);
		return false;
	}
	
protected:

	template<typename L>
	static bool is_any(void)
	{
		typedef L ltype;
		return typeid(ltype) == typeid(any_type);
	}

	template<typename L, typename R>
	static bool is_have_any(void) 
	{
		typedef L ltype;
		typedef R rtype;

		return typeid(ltype) == typeid(any_type) || typeid(rtype) == typeid(any_type);
	}

	template<typename L, typename R>
	static bool is_all_any(void)
	{
		typedef L ltype;
		typedef R rtype;

		return typeid(ltype) == typeid(any_type) && typeid(rtype) == typeid(any_type);
	}

};

} // namespace any_params
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif //__YGGR_ANY_VAL_NON_ANY_TYPE_OPERATOR_HPP__