// node_handle_foo_hacking_midx_ordered_index_decl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_DECL_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_DECL_HPP__

#include <yggr/base/yggrdef.h>

#if (BOOST_VERSION < 107400)

#include <yggr/container/detail/node_handle_foo_hacking_midx_ordered_index_basic_decl.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename Arg1,
			typename Arg2 = boost::mpl::na,
			typename Arg3 = boost::mpl::na>
struct node_handle_foo_hacking_midx_ordered_unique;

template<typename Arg1,
			typename Arg2 = boost::mpl::na,
			typename Arg3 = boost::mpl::na >
struct node_handle_foo_hacking_midx_ordered_non_unique;

template<typename T>
struct cast_to_midx_ordered_index_wrap_hacking;

template<typename T>
struct cast_to_midx_ordered_index_wrap_native;

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

template<typename A1, typename A2, typename A3>
struct cast_to_midx_ordered_index_wrap_hacking
		<
			boost::multi_index::ordered_unique<A1, A2, A3>
		>
{
	typedef node_handle_foo_hacking_midx_ordered_unique<A1, A2, A3> type;
};

template<typename A1, typename A2, typename A3>
struct cast_to_midx_ordered_index_wrap_hacking
		<
			boost::multi_index::ordered_non_unique<A1, A2, A3>
		>
{
	typedef node_handle_foo_hacking_midx_ordered_non_unique<A1, A2, A3> type;
};

// cast to native

template<typename A1, typename A2, typename A3>
struct cast_to_midx_ordered_index_wrap_native
		<
			node_handle_foo_hacking_midx_ordered_unique<A1, A2, A3>
		>
{
	typedef boost::multi_index::ordered_unique<A1, A2, A3> type;
};

template<typename A1, typename A2, typename A3>
struct cast_to_midx_ordered_index_wrap_native
		<
			node_handle_foo_hacking_midx_ordered_non_unique<A1, A2, A3>
		>
{
	typedef boost::multi_index::ordered_non_unique<A1, A2, A3> type;
};

// wrap_hacking_cast
template<typename A1, typename A2, typename A3>
struct index_wrap_hacking_cast<boost::multi_index::ordered_non_unique<A1, A2, A3> >
{
private:
	typedef boost::multi_index::ordered_non_unique<A1, A2, A3> src_type;
public:
	typedef typename cast_to_midx_ordered_index_wrap_hacking<src_type>::type type;
};

template<typename A1, typename A2, typename A3>
struct index_wrap_hacking_cast<boost::multi_index::ordered_unique<A1, A2, A3> >
{
private:
	typedef boost::multi_index::ordered_unique<A1, A2, A3> src_type;
public:
	typedef typename cast_to_midx_ordered_index_wrap_hacking<src_type>::type type;
};

// wrap_native_cast
template<typename A1, typename A2, typename A3>
struct index_wrap_native_cast< node_handle_foo_hacking_midx_ordered_non_unique<A1, A2, A3> >
{
private:
	typedef node_handle_foo_hacking_midx_ordered_non_unique<A1, A2, A3> src_type;
public:
	typedef typename cast_to_midx_ordered_index_wrap_native<src_type>::type type;
};

template<typename A1, typename A2, typename A3>
struct index_wrap_native_cast<node_handle_foo_hacking_midx_ordered_unique<A1, A2, A3> >
{
private:
	typedef node_handle_foo_hacking_midx_ordered_unique<A1, A2, A3> src_type;
public:
	typedef typename cast_to_midx_ordered_index_wrap_native<src_type>::type type;
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // BOOST_VERSION < 107400

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_DECL_HPP__
