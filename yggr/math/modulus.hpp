//modulus.hpp

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

#ifndef __YGGR_MATH_MODULUS_HPP__
#define __YGGR_MATH_MODULUS_HPP__

#include <yggr/type_traits/operator_result.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <cmath>

namespace yggr
{
namespace math
{

template<typename Ret, bool isfloat = boost::is_floating_point<Ret>::value >
struct modulus_handler;

template<typename Ret>
struct modulus_handler<Ret, false>
{
	typedef Ret ret_type;

	template<typename L, typename R> inline
	ret_type op_mod(const L& l, const R& r) const
	{
		return l % r;
	}

	template<typename L, typename R> inline
	L& op_mod_set(L& l, const R& r) const
	{
		l %= r;
		return l;
	}
};

template<typename Ret>
struct modulus_handler<Ret, true>
{
	typedef Ret ret_type;

	template<typename L, typename R> inline
	ret_type op_mod(const L& l, const R& r) const
	{
		return std::fmod(ret_type(l), ret_type(r));
	}

	template<typename L, typename R> inline
	L& op_mod_set(L& l, const R& r) const
	{
		l = std::fmod(ret_type(l), ret_type(r));
		return l;
	}
};

template<typename L, typename R> inline
typename modulus_operator_result<L, R>::type 
	modulus(const L& l, const R& r)
{
	typedef typename modulus_operator_result<L, R>::type ret_type;
	typedef modulus_handler<ret_type> handler_type;
	handler_type handler;
	return handler.op_mod(l, r);
}

template<typename L, typename R> inline
L& modulus_set(L& l, const R& r)
{
	typedef typename modulus_operator_result<L, R>::type ret_type;
	typedef modulus_handler<ret_type> handler_type;
	handler_type handler;
	return handler.op_mod_set(l, r);
}

} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_MODULUS_HPP__
