// boost_midx_container_opt_decl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_CONTAINER_OPT_DECL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_CONTAINER_OPT_DECL_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/container/multi_index_container.hpp>

#include <yggr/safe_container/detail/cast_midx_indexed_by.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Value,
			typename IndexSpecifierList,
			typename Allocator>
class boost_midx_container_opt;

template<typename T>
struct cast_to_midx_container_opt;

template<typename T>
struct cast_to_midx_container_org;

} // namespace detail
} // namespace safe_container
} // namespace yggr


namespace yggr
{
namespace safe_container
{
namespace detail
{

// cast to opt
template<typename T>
struct cast_to_midx_container_opt
{
	typedef T type;
};

template<typename V, typename I, typename A>
struct cast_to_midx_container_opt
		< 
			boost::multi_index::multi_index_container<V, I, A> 
		>
{
	typedef 
		boost_midx_container_opt
		<
			V,
			typename indexed_by_opt_cast<I>::type,
			A
		> type;
};

// cast to org
template<typename T>
struct cast_to_midx_container_org
{
	typedef T type;
};

template<typename V, typename I, typename A>
struct cast_to_midx_container_org
		< 
			boost_midx_container_opt<V, I, A>
		>
{
	typedef 
		boost::multi_index::multi_index_container
		<
			V,
			typename indexed_by_org_cast<I>::type,
			A
		> type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_CONTAINER_OPT_DECL_HPP__
