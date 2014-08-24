//sign.hpp

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

#ifndef __YGGR_MATH_SIGN_HPP__
#define __YGGR_MATH_SIGN_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/type_traits/types_checker.hpp>
#include <yggr/math/value_miss_comparer.hpp>

namespace yggr
{
namespace math
{

template<typename T>
typename upper_signed<T>::value_type sign(const T& x)
{
	typedef T val_type;
	typedef value_miss_comparer<val_type> cmper_type;

	cmper_type cmper(x);

	if(!signed_checker<val_type>())
	{
		return cmper == val_type()? val_type() : val_type(1);
	}

	return cmper == val_type()
			? val_type() 
			: cmper < val_type()
				? val_type(-1) 
				: val_type(1);
}

} // namespace math
} // namespace yggr

#endif //__YGGR_MATH_SIGN_HPP__