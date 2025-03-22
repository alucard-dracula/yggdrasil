//array_t_maker.hpp

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

#ifndef __YGGR_CONTAINER_EX_ARRAY_REF_T_MAKER_HPP__
#define __YGGR_CONTAINER_EX_ARRAY_REF_T_MAKER_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/type_traits/remove_reference.hpp>

namespace yggr
{
namespace container_ex
{

template<typename T, std::size_t N>
struct array_reference_t_maker
{
	typedef T(&type)[N];
};

template<typename T, std::size_t N>
struct array_const_reference_t_maker
{
	typedef const T(&type)[N];
};

template<typename T, std::size_t N>
struct array_pointer_t_maker
{
	typedef T(*type)[N];
};

template<typename T, std::size_t N>
struct array_const_pointer_t_maker
{
	typedef const T(*type)[N];
};

template<typename T, std::size_t N>
struct array_t_maker
{
	typedef T type[N];
};

} // namespace container_ex

using container_ex::array_reference_t_maker;
using container_ex::array_const_reference_t_maker;

using container_ex::array_pointer_t_maker;
using container_ex::array_const_pointer_t_maker;

using container_ex::array_t_maker;

} // namespace yggr

#endif // __YGGR_CONTAINER_EX_ARRAY_REF_T_MAKER_HPP__
