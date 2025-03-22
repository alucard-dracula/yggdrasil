// map_regularisation.hpp

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

#ifndef __YGGR_CONTAINER_EX_MAP_REGULARISATION_HPP__
#define __YGGR_CONTAINER_EX_MAP_REGULARISATION_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/container_ex/allocator_t.hpp>
#include <yggr/container_ex/container_compare_t.hpp>
#include <yggr/container_ex/container_mapped_value_t.hpp>
#include <yggr/container_ex/container_key_t.hpp>
#include <yggr/container_ex/map_options_t.hpp>

namespace yggr
{
namespace container_ex
{
namespace detail
{

template<typename T>
struct map_regularisation_detail
	: public mplex::null_type
{
};

template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMMap,
			typename K, typename T, typename C, typename A>
struct map_regularisation_detail< MapOrMMap<K, T, C, A> >
{
private:
	typedef MapOrMMap<K, T, C, A> arg_type;

public:
	typedef 
		MapOrMMap
		<
			typename ::yggr::container_ex::container_key_t<arg_type>::type,
			typename ::yggr::container_ex::container_mapped_value_t<arg_type>::type,
			typename ::yggr::container_ex::container_key_compare_t<arg_type>::type,
			typename ::yggr::container_ex::allocator_t<arg_type>::type
		> type;
};

template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMMap,
			typename K, typename T, typename C, typename A, typename O>
struct map_regularisation_detail< MapOrMMap<K, T, C, A, O> >
{
private:
	typedef MapOrMMap<K, T, C, A, O> arg_type;

public:
	typedef 
		MapOrMMap
		<
			typename ::yggr::container_ex::container_key_t<arg_type>::type,
			typename ::yggr::container_ex::container_mapped_value_t<arg_type>::type,
			typename ::yggr::container_ex::container_key_compare_t<arg_type>::type,
			typename ::yggr::container_ex::allocator_t<arg_type>::type,
			typename ::yggr::container_ex::map_options_t<arg_type>::type
		> type;
};

} // namespace detail

template<typename T>
struct map_regularisation
	: public detail::map_regularisation_detail<T>
{
};

} // namespace container_ex

using container_ex::map_regularisation;

} // namespace yggr

#endif // __YGGR_CONTAINER_EX_MAP_REGULARISATION_HPP__