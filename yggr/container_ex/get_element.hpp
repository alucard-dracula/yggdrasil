//get_element.hpp (container_ex)

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

#ifndef __YGGR_CONTAINER_EX_GET_ELEMENT_HPP__
#define __YGGR_CONTAINER_EX_GET_ELEMENT_HPP__

#include <yggr/ppex/typedef.hpp>

#include <yggr/utility/get_element_default.hpp>
#include <yggr/container/array.hpp>

namespace yggr
{
namespace container_ex
{

// -------------array------------

// std::array
#if !defined(YGGR_NO_CXX11_HDR_ARRAY)

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
T& get_first_element(std::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr[0];
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const T& get_first_element(const std::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr[0];
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
T& get_last_element(std::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr[N - 1];
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const T& get_last_element(const std::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr[N -1];
}

#endif // YGGR_NO_CXX11_HDR_ARRAY

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
T& get_first_element(boost::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr[0];
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const T& get_first_element(const boost::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr[0];
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
T& get_last_element(boost::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr[N - 1];
}

template<typename T, std::size_t N> YGGR_CONSTEXPR_OR_INLINE
const T& get_last_element(const boost::array<T, N>& arr) YGGR_NOEXCEPT_OR_NOTHROW
{
	return arr[N -1];
}

} // namespace container_ex

using container_ex::get_first_element;
using container_ex::get_last_element;

} // namespace yggr

#endif // __YGGR_CONTAINER_EX_GET_ELEMENT_HPP__
