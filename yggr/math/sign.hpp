//sign.hpp

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

#ifndef __YGGR_MATH_SIGN_HPP__
#define __YGGR_MATH_SIGN_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/type_traits/types_checker.hpp>
#include <yggr/math/flaw_comparer.hpp>

namespace yggr
{
namespace math
{

namespace detail
{

template<typename Val, bool is_signed>
struct sign_result;

template<typename Val>
struct sign_result<Val, true>
{
	typedef Val value_type;

	template<typename Cmper> inline
	value_type operator()(const Cmper& cmper) const
	{
		return
			cmper == value_type()? 
				value_type() 
				: cmper < value_type()?
					value_type(-1) 
					: value_type(1);
	}
};

template<typename Val>
struct sign_result<Val, false>
{
	typedef Val value_type;

	template<typename Cmper> inline
	value_type operator()(const Cmper& cmper) const
	{
		return cmper == value_type()? value_type() : value_type(1);
	}
};

} // namespace detail


template<typename T> inline
typename upper_signed<T>::type sign(const T& x)
{
	typedef T val_type;
	typedef flaw_comparer<val_type> cmper_type;
	typedef detail::sign_result<val_type, signed_value_checker<val_type>::value> sign_result_type;

	cmper_type cmper(x);
	sign_result_type rst;

	return rst(cmper);
}

} // namespace math
} // namespace yggr

#endif //__YGGR_MATH_SIGN_HPP__