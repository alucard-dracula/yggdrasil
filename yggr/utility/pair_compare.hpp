//pair_compare.hpp

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

#ifndef __YGGR_UTILITY_PAIR_COMPARE_HPP__
#define __YGGR_UTILITY_PAIR_COMPARE_HPP__

#include <yggr/base/yggrdef.h>
#include <utility>

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_11) \
    || ( defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20) )

namespace std
{

#if !(defined(__clang__) && (__clang__))

template<typename TL1, typename TL2, typename TR1, typename TR2> inline //YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator==(const std::pair<TL1, TL2>& l, const std::pair<TR1, TR2>& r)
{
	return l.first == r.first && l.second == r.second;
}

template<typename TL1, typename TL2, typename TR1, typename TR2> inline //YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator!=(const std::pair<TL1, TL2>& l, const std::pair<TR1, TR2>& r)
{
	return !(l.first == r.first && l.second == r.second);
}

template<typename TL1, typename TL2, typename TR1, typename TR2> inline //YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator<(const std::pair<TL1, TL2>& l, const std::pair<TR1, TR2>& r)
{
	return  (l.first == r.first && l.second < r.second) || (l.first < r.first);
}

template<typename TL1, typename TL2, typename TR1, typename TR2> inline //YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator<=(const std::pair<TL1, TL2>& l, const std::pair<TR1, TR2>& r)
{
	return !operator<(r, l);
}

template<typename TL1, typename TL2, typename TR1, typename TR2> inline //YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator>(const std::pair<TL1, TL2>& l, const std::pair<TR1, TR2>& r)
{
	return operator<(r, l);
}

template<typename TL1, typename TL2, typename TR1, typename TR2> inline //YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator>=(const std::pair<TL1, TL2>& l, const std::pair<TR1, TR2>& r)
{
	return !operator<(l, r);
}

#endif // #if !(defined(__clang__) && (__clang__))

} // namespace std

#endif // #if(YGGR_CPP_VERSION < YGGR_CPP_VER_11) || ( defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20) )

#endif // __YGGR_UTILITY_PAIR_COMPARE_HPP__
