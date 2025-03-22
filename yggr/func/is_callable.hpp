//is_callable.hpp

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

#ifndef __YGGR_FUNC_IS_CALLABLE_HPP__
#define __YGGR_FUNC_IS_CALLABLE_HPP__

#include <yggr/type_traits/has_operator_function.hpp>
#include <boost/type_traits/is_pointer.hpp>

namespace yggr
{
namespace func
{
namespace detail
{

template<typename F, bool is_pointer = boost::is_pointer<F>::value >
struct is_callable_exclude_function_pointer_detail;

template<typename F>
struct is_callable_exclude_function_pointer_detail<F, true>
	: public boost::mpl::false_
{
};

template<typename F>
struct is_callable_exclude_function_pointer_detail<F, false>
	: public has_operator_function<F>
{
};

} // namespace detail

template<typename F>
struct is_callable
	: public has_operator_function<F>::type
{
};

template<typename F>
struct is_callable_exclude_function_pointer
	: public detail::is_callable_exclude_function_pointer_detail<F>
{
};

} // namespace func

// !!! is_callable not support class member function, because class member function need object !!!

using func::is_callable;
using func::is_callable_exclude_function_pointer;

} // namespace yggr

#endif // __YGGR_FUNC_IS_CALLABLE_HPP__
