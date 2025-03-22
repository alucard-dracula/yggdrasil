// container_compare_t.hpp

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

#ifndef __YGGR_CONTAINER_EX_CONTAINER_COMPARE_T_HPP__
#define __YGGR_CONTAINER_EX_CONTAINER_COMPARE_T_HPP__

#include <yggr/container_ex/has_compare_t.hpp>
#include <utility>

namespace yggr
{
namespace container_ex
{
namespace detail
{

template<typename T, bool has_key_compare = has_key_compare_t<T>::value>
struct container_key_compare_t_detail;

template<typename T>
struct container_key_compare_t_detail<T, true>
{
	typedef typename T::key_compare type;
};

template<typename T>
struct container_key_compare_t_detail<T, false>
	: public mplex::null_type
{
};

template<typename T, bool has_key_compare = has_value_compare_t<T>::value>
struct container_value_compare_t_detail;

template<typename T>
struct container_value_compare_t_detail<T, true>
{
	typedef typename T::value_compare type;
};

template<typename T>
struct container_value_compare_t_detail<T, false>
	: public mplex::null_type
{
};

} // namespace detail

template<typename Container>
struct container_key_compare_t
	: public detail::container_key_compare_t_detail<Container>
		
{
};

template<typename Container>
struct container_value_compare_t
	: public detail::container_value_compare_t_detail<Container>
		
{
};

} // namespace container_ex

using container_ex::container_key_compare_t;
using container_ex::container_value_compare_t;

} // namespace yggr

#endif // __YGGR_CONTAINER_EX_CONTAINER_COMPARE_T_HPP__

