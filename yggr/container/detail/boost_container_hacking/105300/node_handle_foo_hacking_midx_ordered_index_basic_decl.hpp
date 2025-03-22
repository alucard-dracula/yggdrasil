// node_handle_foo_hacking_midx_ordered_index_basic_decl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_BASIC_DECL_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_BASIC_DECL_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_BASIC_DECL_HPP__
#	error "this file is inner file, can't include it!"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_BASIC_DECL_HPP__

#include <yggr/container/detail/cast_midx_base_type.hpp>

#include <boost/multi_index/ordered_index.hpp>

#ifdef YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX
#	error "boost version panic"
#endif // YGGR_BOOST_MULTI_INDEX_ARGS6_VER_ORDERED_INDEX

namespace yggr
{
namespace container
{
namespace detail
{

template
<
	typename KeyFromValue, typename Compare,
	typename SuperMeta, typename TagList, typename Category
>
class node_handle_foo_hacking_midx_ordered_index;

template<typename T>
struct cast_to_midx_ordered_index_hacking;

template<typename T>
struct cast_to_midx_ordered_index_native;

} // namespace detail
} // namespace container
} // namespace yggr

// cast
namespace yggr
{
namespace container
{
namespace detail
{

// cast to hacking

template<typename K, typename P, 
			typename S, typename T, typename C>
struct cast_to_midx_ordered_index_hacking
		< 
			boost::multi_index::detail::ordered_index<K, P, S, T, C> 
		>
{
	typedef 
		node_handle_foo_hacking_midx_ordered_index
		<
			K, P, 
			typename nth_layer_hacking_cast<S>::type, 
			T, C
		> type;
};

// cast to native

template<typename K, typename P, 
			typename S, typename T, typename C>
struct cast_to_midx_ordered_index_native
		<
			node_handle_foo_hacking_midx_ordered_index<K, P, S, T, C>
		>
{
	typedef 
		boost::multi_index::detail::ordered_index
		<
			K, P, 
			typename nth_layer_native_cast<S>::type, 
			T, C
		> type;
};


template<typename K, typename P, 
			typename S, typename T, typename C >
struct index_hacking_cast<boost::multi_index::detail::ordered_index<K, P, S, T, C> >
{
private:
	typedef boost::multi_index::detail::ordered_index<K, P, S, T, C> src_type;
public:
	typedef typename cast_to_midx_ordered_index_hacking<src_type>::type type;
};

// index_native_cast
template<typename K, typename P, 
			typename S, typename T, typename C >
struct index_native_cast< node_handle_foo_hacking_midx_ordered_index<K, P, S, T, C> >
{
private:
	typedef node_handle_foo_hacking_midx_ordered_index<K, P, S, T, C> src_type;
public:
	typedef typename cast_to_midx_ordered_index_native<src_type>::type type;
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_BASIC_DECL_HPP__
