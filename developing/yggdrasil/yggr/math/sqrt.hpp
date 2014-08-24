//sqrt.hpp

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

#ifndef __YGGR_MATH_SQRT_HPP__
#define __YGGR_MATH_SQRT_HPP__

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace math
{
template<typename T>
T sqrt(const T& t);

#ifdef YGGR_USE_FAST_MATH

inline f32 sqrt(const f32& x )
{
	f32 x_half = 0.5f * x;
	s32 tmp = 0x5F3759DF - ( *(s32*)&x >> 1 ); //initial guess
	f32 x_res = *(f32*)&tmp;
	
	x_res *= ( 1.5f - ( x_half * x_res * x_res ) );
	return x_res * x;
}

inline f64 sqrt(const f64 &x) 
{
	f64 x_half = 0.5 * x;
	s64 tmp = 0x5FE6EB50C7B537AAl - ( *(s64*)&x >> 1);//initial guess
	f64 x_res = * (f64*)&tmp;
	
	x_res *= ( 1.5 - ( x_half * x_res * x_res ) );
	return x_res * x;
}

#else

#include <complex>
inline f32 sqrt(const f32& x )
{
	return std::sqrt(x);
}

inline f64 sqrt(const f64 &x) 
{
	return std::sqrt(x);
}

#endif // YGGR_USE_FAST_MATH

} // namespace math
} // namespace yggr

#endif //__YGGR_MATH_SQRT_HPP__