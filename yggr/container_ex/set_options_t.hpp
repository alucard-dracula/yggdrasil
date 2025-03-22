// set_options_t.hpp

/****************************************************************************
Copyright (c) 2014-2022 yggdrasil

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

#ifndef __YGGR_CONTAINER_EX_SET_OPTIONS_T_HPP__
#define __YGGR_CONTAINER_EX_SET_OPTIONS_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>
#include <yggr/mplex/typename_expand.hpp>
#include <yggr/container/get_set_options.hpp>

namespace yggr
{
namespace container_ex
{
namespace detail
{

template<typename T, std::size_t tpl_arg_size = mplex::typename_expand_size<T>::value>
struct set_options_t_detail
	: public mplex::null_type
{
};

template<typename T>
struct set_options_t_detail<T, 4>
{
public:
	typedef typename 
		container::detail::get_tree_options
		<
			typename mplex::typename_expand_get<T, 3>::type
		>::type type;
};


} // namespace detail

template<typename T>
struct set_options_t
	: public detail::set_options_t_detail<T>
{
};

template<typename T>
struct multiset_options_t
	: public detail::set_options_t_detail<T>
{
};


} // namespace container_ex

using container_ex::set_options_t;
using container_ex::multiset_options_t;

} // namespace yggr

#endif // __YGGR_CONTAINER_EX_SET_OPTIONS_T_HPP__