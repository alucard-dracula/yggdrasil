//sqrt.hpp

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

#ifndef __YGGR_MATH_SQRT_HPP__
#define __YGGR_MATH_SQRT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/upper_types.hpp>

#include <complex>

namespace yggr
{
namespace math
{

inline f32 fast_sqrt(const f32& x)
{
	f32 x_half = 0.5f * x;
	s32 tmp = 0x5F3759DF - ( (*reinterpret_cast<const s32*>(&x)) >> 1 ); //initial guess
	f32 x_res = *(reinterpret_cast<f32*>(&tmp));
	
	x_res *= ( 1.5f - ( x_half * x_res * x_res ) );
#ifdef YGGR_FAST_MATH_ITERATE_TWICE
	x_res *= ( 1.5f - ( x_half * x_res * x_res ) );
#endif // YGGR_FAST_MATH_ITERATE_TWICE
	return x_res * x;
}

inline f64 fast_sqrt(const f64& x) 
{
	f64 x_half = 0.5 * x;
	//s64 tmp = 0x5FE6EB50C7B537AAl - ( (*reinterpret_cast<const s64*>(&x)) >> 1);//initial guess
	s64 tmp = 0x5FE6EB50C7AA19F9l - ( (*reinterpret_cast<const s64*>(&x)) >> 1);//initial guess
	f64 x_res = *(reinterpret_cast<f64*>(&tmp));
	
	x_res *= ( 1.5 - ( x_half * x_res * x_res ) );
#ifdef YGGR_FAST_MATH_ITERATE_TWICE
	x_res *= ( 1.5 - ( x_half * x_res * x_res ) );
#endif // YGGR_FAST_MATH_ITERATE_TWICE
	return x_res * x;
}

inline f32 fast_rsqrt(const f32& x)
{
	f32 x_half = 0.5f * x;
	s32 tmp = 0x5F3759DF - ( (*reinterpret_cast<const s32*>(&x)) >> 1 ); //initial guess
	f32 x_res = *(reinterpret_cast<f32*>(&tmp));
	
	x_res *= ( 1.5f - ( x_half * x_res * x_res ) );
#ifdef YGGR_FAST_MATH_ITERATE_TWICE
	x_res *= ( 1.5f - ( x_half * x_res * x_res ) );
#endif // YGGR_FAST_MATH_ITERATE_TWICE
	return x_res;
}

inline f64 fast_rsqrt(const f64& x) 
{
	f64 x_half = 0.5 * x;
	//s64 tmp = 0x5FE6EB50C7B537AAl - ( (*reinterpret_cast<const s64*>(&x)) >> 1);//initial guess
	s64 tmp = 0x5FE6EB50C7AA19F9l - ( (*reinterpret_cast<const s64*>(&x)) >> 1);//initial guess
	f64 x_res = *(reinterpret_cast<f64*>(&tmp));
	
	x_res *= ( 1.5 - ( x_half * x_res * x_res ) );
#ifdef YGGR_FAST_MATH_ITERATE_TWICE
	x_res *= ( 1.5 - ( x_half * x_res * x_res ) );
#endif // YGGR_FAST_MATH_ITERATE_TWICE
	return x_res;
}

#ifdef YGGR_USE_FAST_MATH

inline f32 sqrt(const f32& x)
{
	return fast_sqrt(x);
}

inline f64 sqrt(const f64& x) 
{
	return fast_sqrt(x);
}

inline f32 rsqrt(const f32& x)
{
	return fast_rsqrt(x);
}

inline f64 rsqrt(const f64& x) 
{
	return fast_rsqrt(x);
}

#else

inline f32 sqrt(const f32& x)
{
	return std::sqrt(x);
}

inline f64 sqrt(const f64& x) 
{
	return std::sqrt(x);
}

inline f32 rsqrt(const f32& x)
{
	return 1.0f / std::sqrt(x);
}

inline f64 rsqrt(const f64& x) 
{
	return 1.0 / std::sqrt(x);
}

#endif // YGGR_USE_FAST_MATH

template<typename T> inline
std::complex<T> sqrt(const std::complex<T>& x)
{
	return std::sqrt(x);
}

template<typename T> inline
typename upper_float<T>::type sqrt(const T& x)
{
	typedef typename upper_float<T>::type ret_type;
	return sqrt(ret_type(x));
}

template<typename T> inline
std::complex<T> rsqrt(const std::complex<T>& x)
{
	typedef std::complex<T> value_type;
	return value_type(T(1)) / std::sqrt(x);
}

template<typename T> inline
typename upper_float<T>::type rsqrt(const T& x)
{
	typedef typename upper_float<T>::type ret_type;
	return ret_type(1) / sqrt(ret_type(x));
}

} // namespace math
} // namespace yggr

#endif //__YGGR_MATH_SQRT_HPP__