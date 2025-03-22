//get_element.hpp (utility)

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

#ifndef __YGGR_UTILITY_GET_ELEMENT_HPP__
#define __YGGR_UTILITY_GET_ELEMENT_HPP__

#include <yggr/ppex/typedef.hpp>

#include <yggr/utility/get_element_default.hpp>
#include <yggr/utility/pair.hpp>

namespace yggr
{
namespace utility
{

// -------------pair------------

//if T is pair return first
//------------------get_first_element-----------

template<typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE 
typename utility::detail::pair_getter<T1, T2, 0>::result_type& 
	get_first_element(std::pair<T1, T2>& val) YGGR_NOEXCEPT_OR_NOTHROW
{ 
	return yggr::get<0>(val);
}

template<typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE 
const typename utility::detail::pair_getter<T1, T2, 0>::result_type&
	get_first_element(const std::pair<T1, T2>& val) YGGR_NOEXCEPT_OR_NOTHROW
{ 
	return yggr::get<0>(val);
}
//if T is pair return second
// -----------------get_last_element------------
template<typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE 
typename utility::detail::pair_getter<T1, T2, 1>::result_type& 
	get_last_element(std::pair<T1, T2>& val) YGGR_NOEXCEPT_OR_NOTHROW
{ 
	return yggr::get<1>(val);
}

template<typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE 
const typename utility::detail::pair_getter<T1, T2, 1>::result_type&
	get_last_element(const std::pair<T1, T2>& val) YGGR_NOEXCEPT_OR_NOTHROW
{ 
	return yggr::get<1>(val);
}

} // namespace utility

using utility::get_first_element;
using utility::get_last_element;

} // namespace yggr

#endif // __YGGR_UTILITY_GET_ELEMENT_HPP__
