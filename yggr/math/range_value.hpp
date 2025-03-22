//range_value.hpp

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

#ifndef __YGGR_MATH_RANGE_VALUE_HPP__
#define __YGGR_MATH_RANGE_VALUE_HPP__

#include <utility>

namespace yggr
{
namespace math
{

template<typename T> inline
T range_value(const T& val, const T& min, const T& max)
{
	return val < min? min : (val > max? max : val);
}

template<typename T,
			template<typename _T1, typename _T2> class Pair > inline
T range_value(const T& val, const Pair<T, T>& range)
{
	return val < range.first? range.first : (val > range.second? range.second : val);
}

} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_RANGE_VALUE_HPP__
