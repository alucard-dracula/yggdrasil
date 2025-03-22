//const_var.hpp

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

#ifndef __YGGR_MATH_CONST_VAR_HPP__
#define __YGGR_MATH_CONST_VAR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>

#include <boost/type_traits/is_floating_point.hpp>

#define YGGR_PP_MATH_PI()			3.14159265358979323846
#define YGGR_PP_MATH_PI_2()			1.57079632679489661923
#define YGGR_PP_MATH_PI_4()			0.78539816339744830962
#define YGGR_PP_MATH_1_PI()			0.31830988618379067154
#define YGGR_PP_MATH_2_PI()			0.63661977236758134308

#define YGGR_PP_MATH_E()			2.7182818284590452354
#define YGGR_PP_MATH_LOG2E()		1.4426950408889634074
#define YGGR_PP_MATH_LOG10E()		0.43429448190325182765
#define YGGR_PP_MATH_LN2()			0.69314718055994530942
#define YGGR_PP_MATH_LN10()			2.30258509299404568402

#define YGGR_PP_MATH_SQRT2()		1.41421356237309504880
#define YGGR_PP_MATH_SQRT1_2()		0.70710678118654752440

namespace yggr
{
namespace math
{

struct const_var
{
	// pi
	inline static f64 pi(void)
	{
		return YGGR_PP_MATH_PI();
	}

	template<typename T> inline
	static T pi(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_PI());
	}

	inline static f64 pi_div_2(void)
	{
		return YGGR_PP_MATH_PI_2();
	}

	template<typename T> inline
	static T pi_div_2(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_PI_2());
	}

	inline static f64 pi_div_4(void)
	{
		return YGGR_PP_MATH_PI_4();
	}

	template<typename T> inline
	static T pi_div_4(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_PI_4());
	}

	inline static f64 eins_div_pi(void)
	{
		return YGGR_PP_MATH_1_PI();
	}

	template<typename T> inline
	static T eins_div_pi(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_1_PI());
	}

	inline static f64 zwei_div_pi(void)
	{
		return YGGR_PP_MATH_2_PI();
	}

	template<typename T> inline
	static T zwei_div_pi(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_2_PI());
	}

	// e
	inline static f64 e(void)
	{
		return YGGR_PP_MATH_E();
	}

	template<typename T> inline
	static T e(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_E());
	}

	inline static f64 log2e(void)
	{
		return YGGR_PP_MATH_LOG2E();
	}

	template<typename T> inline
	static T log2e(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_LOG2E());
	}

	inline static f64 log10e(void)
	{
		return YGGR_PP_MATH_LOG10E();
	}

	template<typename T> inline
	static T log10e(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_LOG10E());
	}

	inline static f64 ln2(void)
	{
		return YGGR_PP_MATH_LN2();
	}

	template<typename T> inline
	static T ln2(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_LN2());
	}

	inline static f64 ln10(void)
	{
		return YGGR_PP_MATH_LN10();
	}

	template<typename T> inline
	static T ln10(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_LN10());
	}

	//sqrt(2)

	inline static f64 sqrt2(void)
	{
		return YGGR_PP_MATH_SQRT2();
	}

	template<typename T> inline
	static T sqrt2(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_SQRT2());
	}

	inline static f64 sqrt_1_div_2(void)
	{
		return YGGR_PP_MATH_SQRT1_2();
	}

	template<typename T> inline
	static T sqrt_1_div_2(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_SQRT1_2());
	}
};

} // namespace math
} // namespace yggr

#undef YGGR_PP_MATH_PI
#undef YGGR_PP_MATH_E

#endif //__YGGR_MATH_CONST_VAR_HPP__
