// node_handle_foo_hacking_midx_hashed_index_decl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_HASHED_INDEX_DECL_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_HASHED_INDEX_DECL_HPP__

#include <yggr/base/yggrdef.h>

#if (BOOST_VERSION < 107400)

#include <yggr/container/detail/cast_midx_base_type.hpp>
#include <boost/multi_index/hashed_index.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template
<
	typename KeyFromValue, typename Hash, typename Pred,
	typename SuperMeta, typename TagList, typename Category
>
class node_handle_foo_hacking_midx_hashed_index;

template<typename Arg1,
			typename Arg2 = boost::mpl::na,
			typename Arg3 = boost::mpl::na,
			typename Arg4 = boost::mpl::na >
struct node_handle_foo_hacking_midx_hashed_non_unique;

template<typename Arg1,
			typename Arg2 = boost::mpl::na,
			typename Arg3 = boost::mpl::na,
			typename Arg4 = boost::mpl::na >
struct node_handle_foo_hacking_midx_hashed_unique;

// cast decl
template<typename T>
struct cast_to_midx_hashed_index_hacking;

template<typename T>
struct cast_to_midx_hashed_index_wrap_hacking;

template<typename T>
struct cast_to_midx_hashed_index_native;

template<typename T>
struct cast_to_midx_hashed_index_wrap_native;

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
template<typename K, typename H, typename P, 
			typename S, typename T, typename C>
struct cast_to_midx_hashed_index_hacking
		< 
			boost::multi_index::detail::hashed_index<K, H, P, S, T, C> 
		>
{
	typedef 
		node_handle_foo_hacking_midx_hashed_index
		<
			K, H, P, 
			typename nth_layer_hacking_cast<S>::type, 
			T, C
		> type;
};

template<typename A1, typename A2, typename A3, typename A4>
struct cast_to_midx_hashed_index_wrap_hacking
		<
			boost::multi_index::hashed_unique<A1, A2, A3, A4>
		>
{
	typedef node_handle_foo_hacking_midx_hashed_unique<A1, A2, A3, A4> type;
};

template<typename A1, typename A2, typename A3, typename A4>
struct cast_to_midx_hashed_index_wrap_hacking
		<
			boost::multi_index::hashed_non_unique<A1, A2, A3, A4>
		>
{
	typedef node_handle_foo_hacking_midx_hashed_non_unique<A1, A2, A3, A4> type;
};

// cast to native
template<typename K, typename H, typename P, 
			typename S, typename T, typename C>
struct cast_to_midx_hashed_index_native
		<
			node_handle_foo_hacking_midx_hashed_index<K, H, P, S, T, C>
		>
{
	typedef 
		boost::multi_index::detail::hashed_index
		<
			K, H, P, 
			typename nth_layer_native_cast<S>::type, 
			T, C
		> type;
};

template<typename A1, typename A2, typename A3, typename A4>
struct cast_to_midx_hashed_index_wrap_native
		<
			node_handle_foo_hacking_midx_hashed_unique<A1, A2, A3, A4>
		>
{
	typedef boost::multi_index::hashed_unique<A1, A2, A3, A4> type;
};

template<typename A1, typename A2, typename A3, typename A4>
struct cast_to_midx_hashed_index_wrap_native
		<
			node_handle_foo_hacking_midx_hashed_non_unique<A1, A2, A3, A4>
		>
{
	typedef boost::multi_index::hashed_non_unique<A1, A2, A3, A4> type;
};

// index_hacking_cast
template<typename K, typename H, typename P, 
			typename S, typename T, typename C >
struct index_hacking_cast< boost::multi_index::detail::hashed_index<K, H, P, S, T, C> >
{
private:
	typedef boost::multi_index::detail::hashed_index<K, H, P, S, T, C> src_type;
public:
	typedef typename cast_to_midx_hashed_index_hacking<src_type>::type type;
};

// index_native_cast
template<typename K, typename H, typename P, 
			typename S, typename T, typename C >
struct index_native_cast< node_handle_foo_hacking_midx_hashed_index<K, H, P, S, T, C> >
{
private:
	typedef node_handle_foo_hacking_midx_hashed_index<K, H, P, S, T, C> src_type;
public:
	typedef typename cast_to_midx_hashed_index_native<src_type>::type type;
};

// wrap_hacking_cast
template<typename T1, typename T2, typename T3, typename T4>
struct index_wrap_hacking_cast<boost::multi_index::hashed_non_unique<T1, T2, T3, T4> >
{
private:
	typedef boost::multi_index::hashed_non_unique<T1, T2, T3, T4> src_type;
public:
	typedef typename 
		cast_to_midx_hashed_index_wrap_hacking<src_type>::type type;
};

template<typename T1, typename T2, typename T3, typename T4>
struct index_wrap_hacking_cast<boost::multi_index::hashed_unique<T1, T2, T3, T4> >
{
private:
	typedef boost::multi_index::hashed_unique<T1, T2, T3, T4> src_type;
public:
	typedef typename 
		cast_to_midx_hashed_index_wrap_hacking<src_type>::type type;
};

// wrap_native_cast
template<typename T1, typename T2, typename T3, typename T4>
struct index_wrap_native_cast< node_handle_foo_hacking_midx_hashed_non_unique<T1, T2, T3, T4> >
{
private:
	typedef node_handle_foo_hacking_midx_hashed_non_unique<T1, T2, T3, T4> src_type;
public:
	typedef typename 
		cast_to_midx_hashed_index_wrap_native<src_type>::type type;
};

template<typename T1, typename T2, typename T3, typename T4>
struct index_wrap_native_cast<node_handle_foo_hacking_midx_hashed_unique<T1, T2, T3, T4> >
{
private:
	typedef node_handle_foo_hacking_midx_hashed_unique<T1, T2, T3, T4> src_type;
public:
	typedef typename 
		cast_to_midx_hashed_index_wrap_native<src_type>::type type;
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // BOOST_VERION < 107400

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_HASHED_INDEX_DECL_HPP__
