//matrix3d.hpp

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

#ifndef __YGGR_MATH_MATRIX3D_HPP__
#define __YGGR_MATH_MATRIX3D_HPP__

#include <yggr/math/vector3d_t.hpp>
#include <yggr/math/matrix3d_t.hpp>

namespace yggr
{
namespace math
{

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR> inline
vector3d<typename multiplies_operator_result<ValL, ValR>::type, BaseL>
	operator*(const vector3d<ValL, BaseL>& l, const matrix3d<ValR, Array, BaseR>& r)
{
	typedef vector3d<typename multiplies_operator_result<ValL, ValR>::type, BaseL> ret_type;

	return
		ret_type(
			l[0] * r[0][0] + l[1] * r[1][0] + l[2] * r[2][0] + l[3] * r[3][0],
			l[0] * r[0][1] + l[1] * r[1][1] + l[2] * r[2][1] + l[3] * r[3][1],
			l[0] * r[0][2] + l[1] * r[1][2] + l[2] * r[2][2] + l[3] * r[3][2],
			l[0] * r[0][3] + l[1] * r[1][3] + l[2] * r[2][3] + l[3] * r[3][3]);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseL,
			template<typename _Val, std::size_t> class BaseR > inline
vector3d<typename multiplies_operator_result<ValL, ValR>::type, BaseR>
	operator*(const matrix3d<ValL, Array, BaseL>& l, const vector3d<ValR, BaseR>& r)
{
	typedef vector2d<typename multiplies_operator_result<ValL, ValR>::type, BaseR> ret_type;

	return
		ret_type(
			l[0][0] * r[0] + l[0][1] * r[0] + l[0][2] * r[0] + l[0][3] * r[0],
			l[1][0] * r[1] + l[1][1] * r[1] + l[1][2] * r[1] + l[1][3] * r[1],
			l[2][0] * r[2] + l[2][1] * r[2] + l[2][2] * r[2] + l[2][3] * r[2],
			l[3][0] * r[3] + l[3][1] * r[3] + l[3][2] * r[3] + l[3][3] * r[3]);
}

template<typename ValL, typename ValR,
			template<typename _Val, std::size_t> class BaseL,
			template<typename _Val, std::size_t> class Array,
			template<typename _Val, std::size_t, std::size_t,
						template<typename __Val, std::size_t> class _Base> class BaseR > inline
vector3d<ValL, BaseL>&
	operator*=(vector3d<ValL, BaseL>& l, const matrix3d<ValR, Array, BaseR>& r)
{
	typedef vector3d<ValL, BaseL> ret_type;

	ret_type rst(l[0] * r[0][0] + l[1] * r[1][0] + l[2] * r[2][0] + l[3] * r[3][0],
					l[0] * r[0][1] + l[1] * r[1][1] + l[2] * r[2][1] + l[3] * r[3][1],
					l[0] * r[0][2] + l[1] * r[1][2] + l[2] * r[2][2] + l[3] * r[3][2],
					l[0] * r[0][3] + l[1] * r[1][3] + l[2] * r[2][3] + l[3] * r[3][3]);
	return l = rst;
}

} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_MATRIX3D_HPP__
