// node_handle_foo_hacking_midx_sequenced_index_decl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_DECL_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_DECL_HPP__

#include <yggr/base/yggrdef.h>

#if (BOOST_VERSION < 107400)

#include <yggr/container/detail/cast_midx_base_type.hpp>
#include <boost/multi_index/sequenced_index.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename SuperMeta, typename TagList>
class node_handle_foo_hacking_midx_sequenced_index;

template<typename TagList>
struct node_handle_foo_hacking_midx_sequenced;


template<typename T>
struct cast_to_midx_sequenced_index_hacking;

template<typename T>
struct cast_to_midx_sequenced_index_wrap_hacking;

template<typename T>
struct cast_to_midx_sequenced_index_native;

template<typename T>
struct cast_to_midx_sequenced_index_wrap_native;

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

// cast to opt
template<typename S, typename T>
struct cast_to_midx_sequenced_index_hacking
		< 
			boost::multi_index::detail::sequenced_index<S, T> 
		>
{
	typedef 
		node_handle_foo_hacking_midx_sequenced_index
		<
			typename nth_layer_hacking_cast<S>::type, 
			T
		> type;
};

template<typename TL>
struct cast_to_midx_sequenced_index_wrap_hacking
		<
			boost::multi_index::sequenced<TL>
		>
{
	typedef node_handle_foo_hacking_midx_sequenced<TL> type;
};

// cast to org
template<typename S, typename T>
struct cast_to_midx_sequenced_index_native
		<
			node_handle_foo_hacking_midx_sequenced_index<S, T>
		>
{
	typedef 
		boost::multi_index::detail::sequenced_index
		<
			typename nth_layer_native_cast<S>::type, 
			T
		> type;
};

template<typename TL>
struct cast_to_midx_sequenced_index_wrap_native
		<
			node_handle_foo_hacking_midx_sequenced<TL>
		>
{
	typedef boost::multi_index::sequenced<TL> type;
};

// index_hacking_cast
template<typename S, typename T>
struct index_hacking_cast< boost::multi_index::detail::sequenced_index<S, T> >
{
private:
	typedef boost::multi_index::detail::sequenced_index<S, T> src_type;
public:
	typedef typename cast_to_midx_sequenced_index_hacking<src_type>::type type;
};

// index_native_cast
template<typename S, typename T>
struct index_native_cast< node_handle_foo_hacking_midx_sequenced_index<S, T> >
{
private:
	typedef node_handle_foo_hacking_midx_sequenced_index<S, T> src_type;
public:
	typedef typename cast_to_midx_sequenced_index_native<src_type>::type type;
};

// wrap_opt_cast
template<typename TL>
struct index_wrap_hacking_cast< boost::multi_index::sequenced<TL> >
{
private:
	typedef boost::multi_index::sequenced<TL> src_type;
public:
	typedef typename cast_to_midx_sequenced_index_wrap_hacking<src_type>::type type;
};

// wrap_org_cast
template<typename TL>
struct index_wrap_native_cast< node_handle_foo_hacking_midx_sequenced<TL> >
{
private:
	typedef node_handle_foo_hacking_midx_sequenced<TL> src_type;
public:
	typedef typename cast_to_midx_sequenced_index_wrap_native<src_type>::type type;
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // (BOOST_VERSION < 107400)

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_DECL_HPP__
