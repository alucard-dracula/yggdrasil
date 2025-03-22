// set_regularisation.hpp

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

#ifndef __YGGR_CONTAINER_EX_SET_REGULARISATION_HPP__
#define __YGGR_CONTAINER_EX_SET_REGULARISATION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/container_ex/allocator_t.hpp>
#include <yggr/container_ex/container_compare_t.hpp>
#include <yggr/container_ex/set_options_t.hpp>

namespace yggr
{
namespace container_ex
{
namespace detail
{

template<typename T>
struct set_regularisation_detail
	: public mplex::null_type
{
};

template<template<typename _T, typename _C, typename _A> class SetOrMSet,
			typename T, typename C, typename A>
struct set_regularisation_detail< SetOrMSet<T, C, A> >
{
private:
	typedef SetOrMSet<T, C, A> arg_type;

public:
	typedef 
		SetOrMSet
		<
			typename range_ex::range_value_ex<arg_type>::type,
			typename ::yggr::container_ex::container_key_compare_t<arg_type>::type,
			typename ::yggr::container_ex::allocator_t<arg_type>::type
		> type;
};

template<template<typename _T, typename _C, typename _A, typename _O> class SetOrMSet,
			typename T, typename C, typename A, typename O>
struct set_regularisation_detail< SetOrMSet<T, C, A, O> >
{
private:
	typedef SetOrMSet<T, C, A, O> arg_type;

public:
	typedef 
		SetOrMSet
		<
			typename range_ex::range_value_ex<arg_type>::type,
			typename ::yggr::container_ex::container_key_compare_t<arg_type>::type,
			typename ::yggr::container_ex::allocator_t<arg_type>::type,
			typename ::yggr::container_ex::set_options_t<arg_type>::type
		> type;
};

} // namespace detail

template<typename T>
struct set_regularisation
	: public detail::set_regularisation_detail<T>
{
};

} // namespace container_ex

using container_ex::set_regularisation;

} // namespace yggr

#endif // __YGGR_CONTAINER_EX_SET_REGULARISATION_HPP__