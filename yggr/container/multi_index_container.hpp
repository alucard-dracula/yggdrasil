// multi_index_container.hpp

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

#ifndef __YGGR_CONTAINER_MULTI_INDEX_CONTAINER_HPP__
#define __YGGR_CONTAINER_MULTI_INDEX_CONTAINER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/typedef.hpp>

#include <yggr/mplex/typename_expand.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/multi_index_config.hpp>
#include <yggr/container/basic_function_adapter.hpp>

#include <yggr/container/node_handle_traits.hpp>

#include <yggr/container/detail/range_size_def.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_midx_container.hpp>

#include <boost/multi_index_container.hpp>

namespace yggr
{
namespace container
{

using boost::multi_index::multi_index_container;

} // namespace container

using container::multi_index_container;

} // namespace yggr

namespace yggr
{
namespace container
{

template<typename T>
struct cast_to_indexed_by;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	template<template<typename _EArg, typename ..._Args> class Src, 
				typename EArg, typename ...Args>
	struct cast_to_indexed_by< Src<EArg, Args...> >
	{
		typedef boost::multi_index::indexed_by<EArg, Args...> type;
	};

#else

#	define YGGR_PP_CAST_TO_INDEXED_BY_IMPL( __n__ ) \
		template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T), \
					template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename _T)> class Src > \
		struct cast_to_indexed_by< Src< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > > { \
			typedef boost::multi_index::indexed_by<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T)> type; };


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
			YGGR_PP_CAST_TO_INDEXED_BY_IMPL( __n__ )

#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE( )

#	undef YGGR_PP_CAST_TO_INDEXED_BY_IMP

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace container
} // namespace yggr

namespace boost
{
namespace multi_index
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, boost::multi_index::multi_index_container)

} // namespace multi_index
} // namespace boost

#endif // __YGGR_CONTAINER_MULTI_INDEX_CONTAINER_HPP__
