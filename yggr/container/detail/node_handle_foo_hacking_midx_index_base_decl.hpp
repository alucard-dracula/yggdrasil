// node_handle_foo_hacking_midx_index_base_decl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_INDEX_BASE_DECL_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_INDEX_BASE_DECL_HPP__

#include <yggr/container/detail/multi_index_index_base_insert_tag.hpp>

#if (BOOST_VERSION < 107400)

#include <yggr/container/multi_index_config.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template
<
	typename Value, 
	typename IndexSpecifierList, 
	typename Allocator
>
class node_handle_foo_hacking_midx_index_base;

/*
 * the nth_layer_cast is rebuild nth_layer
 * , so index_base not need cast
 */

//
//template<typename T>
//struct cast_to_midx_index_base_hacking;
//
//template<typename T>
//struct cast_to_midx_index_base_native;


} // namespace detail
} // namespace container
} // namespace yggr

#endif // BOOST_VERSION < 107400

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_INDEX_BASE_DECL_HPP__
