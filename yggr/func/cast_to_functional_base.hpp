// cast_to_functional_base.hpp

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

#ifndef __YGGR_FUNC_CAST_TO_FUNCTONAL_BASE_HPP__
#define __YGGR_FUNC_CAST_TO_FUNCTONAL_BASE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>
#include <yggr/func/functional_args_size.hpp>

#include <boost/mpl/if.hpp>

namespace yggr
{
namespace func
{

namespace detail
{

template<typename T, std::size_t N, bool is_functional_chk>
struct make_base_functional_if
	: public mplex::null_type
{
};

#if YGGR_CPP_VERSION < YGGR_CPP_VER_17

template<typename T>
struct make_base_functional_if<T, 1, true>
	: public make_base_functional<T, 1, !!(::yggr::is_base_of_unary_function<T>::value)>
{
};

template<typename T>
struct make_base_functional_if<T, 2, true>
	: public make_base_functional<T, 2, !!(::yggr::is_base_of_binary_function<T>::value)>
{
};

#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17

} // namespace detail

template<typename T>
struct cast_to_functional_base
	: public
		detail::make_base_functional_if
		<
			T, 
			functional_args_size<T>::type::value,
			is_functional<T>::type::value
		>
{
};

} // namespace func
} // namespace yggr

#endif // __YGGR_FUNC_CAST_TO_FUNCTONAL_BASE_HPP__

