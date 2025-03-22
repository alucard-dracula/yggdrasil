//vector2d.hpp

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

#ifndef __YGGR_MATH_VECTOR2D_HPP__
#define __YGGR_MATH_VECTOR2D_HPP__

#include <yggr/math/vector2d_t.hpp>
#include <yggr/math/operator_dot.hpp>
#include <yggr/math/operator_cross.hpp>

namespace yggr
{
namespace math
{

// dot
template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename multiplies_operator_result<ValL, ValR>::type
	operator*(const vector2d<ValL, BaseL>& l, const doter<vector2d<ValR, BaseR> >& r)
{
	return l.dot(r.get());
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename multiplies_operator_result<ValL, ValR>::type
	operator*(const doter< vector2d<ValL, BaseL> >& l, const vector2d<ValR, BaseR>& r)
{
	return l.get().dot(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename multiplies_operator_result<ValL, ValR>::type
	operator*(const doter< vector2d<ValL, BaseL> >& l, const doter<vector2d<ValR, BaseR> >& r)
{
	return l.get().dot(r.get());
}

// cross
template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename multiplies_operator_result<ValL, ValR>::type
	operator*(const vector2d<ValL, BaseL>& l, const crosser<vector2d<ValR, BaseR> >& r)
{
	return l.cross(r.get());
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename multiplies_operator_result<ValL, ValR>::type
	operator*(const crosser< vector2d<ValL, BaseL> >& l, const vector2d<ValR, BaseR>& r)
{
	return l.get().cross(r);
}

template<typename ValL, typename ValR,
			template <typename _Val, std::size_t> class BaseL,
			template <typename _Val, std::size_t> class BaseR> inline
typename multiplies_operator_result<ValL, ValR>::type
	operator*(const crosser< vector2d<ValL, BaseL> >& l, const crosser<vector2d<ValR, BaseR> >& r)
{
	return l.get().cross(r.get());
}

} // namespace math
} // namespace yggr


#endif //__YGGR_MATH_VECTOR2D_HPP__
