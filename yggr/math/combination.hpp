// combination.hpp

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

#ifndef __YGGR_MATH_COMBINATION_HPP__
#define __YGGR_MATH_COMBINATION_HPP__

#include <yggr/math/pascals_triangle.hpp>

namespace yggr
{
namespace math
{

template<typename T> inline
typename boost::enable_if<boost::is_class<T>, T>::type
	combination(const T& n, const T& m)
{
	typedef detail::pascals_triangle_detail<T> cal_type;
	cal_type cal;
	return cal(n, m);
}

template<typename T> inline
typename boost::disable_if<boost::is_class<T>, T>::type
	combination(T n, T m)
{
	typedef detail::pascals_triangle_detail<T> cal_type;
	cal_type cal;
	return cal(n, m);
}

template<std::size_t N, std::size_t M>
struct combination_t
	: public detail::pascals_triangle_t<N, M>
{
};

} // namespace math
} // namesapce yggr

#endif // __YGGR_MATH_COMBINATION_HPP__
