//matrix2d.hpp

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

#ifndef __YGGR_MATH_MATRIX2D_HPP__
#define __YGGR_MATH_MATRIX2D_HPP__

#include <yggr/math/vector2d_t.hpp>
#include <yggr/math/matrix2d_t.hpp>

namespace yggr
{
namespace math
{
} // namespace math
} // namespace yggr

template<typename ValL, typename ValR,
		template<typename _Val, std::size_t> class Array,
		template<typename _Val, std::size_t, std::size_t,
					template<typename __Val, std::size_t> class _Base> class Base>
yggr::math::matrix2d<ValR, Array, Base> operator*(const ValL& l, const yggr::math::matrix2d<ValR, Array, Base>& r)
{
	typedef yggr::math::matrix2d<ValR, Array, Base> ret_type;
	ret_type tmp(ValR(0));

	for(yggr::size_type i = 0, isize = ret_type::E_ROW_LENGTH; i != isize; ++i)
	{
		for(yggr::size_type j = 0, jsize = ret_type::E_COL_LENGTH; j != jsize; ++j)
		{
			tmp[i][j] = l * r[i][j];
		}
	}

	return tmp;
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR	
		>
yggr::math::vector2d<ValL, BaseL> operator*(const yggr::math::vector2d<ValL, BaseL>& l,
												const yggr::math::matrix2d<ValR, Array, BaseR>& r)
{
	typedef yggr::math::vector2d<ValL, BaseL> ret_type;

	ret_type rst(l[0] * r[0][0] + l[1] * r[1][0] + l[2] * r[2][0],
							l[0] * r[0][1] + l[1] * r[1][1] + l[2] * r[2][1]);
	rst[2] = l[0] * r[0][2] + l[1] * r[1][2] + l[2] * r[2][2];
	return rst;
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR	
		>
yggr::math::vector2d<ValL, BaseL>& operator*=(yggr::math::vector2d<ValL, BaseL>& l,
												const yggr::math::matrix2d<ValR, Array, BaseR>& r)
{
	typedef yggr::math::vector2d<ValL, BaseL> ret_type;

	ret_type rst(l[0] * r[0][0] + l[1] * r[1][0] + l[2] * r[2][0],
							l[0] * r[0][1] + l[1] * r[1][1] + l[2] * r[2][1]);
	rst[2] = l[0] * r[0][2] + l[1] * r[1][2] + l[2] * r[2][2];
	l.swap(rst);
	return l;
}

#endif // __YGGR_MATH_MATRIX2D_HPP__
