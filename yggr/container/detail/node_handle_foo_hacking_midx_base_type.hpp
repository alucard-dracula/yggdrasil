//node_handle_foo_hacking_midx_base_type.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_BASE_TYPE_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_BASE_TYPE_HPP__

#include <yggr/mplex/static_assert.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_midx_index_base.hpp>
#include <boost/multi_index/detail/base_type.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

// nth_layer_hacking
template<int N, typename Value, typename IndexSpecifierList, typename Allocator>
struct nth_layer_hacking;

// multi_index_base_hacking_type
template<typename Value, typename IndexSpecifierList, typename Allocator>
struct multi_index_base_hacking_type;

} // namespace detail
} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{

/* MPL machinery to construct a linear hierarchy of indices out of
	* a index list.
	*/

#if BOOST_WORKAROUND(BOOST_MSVC, <1310)

struct index_applier
{
	template<typename IndexSpecifierMeta, typename SuperMeta>
	struct apply
		:	boost::multi_index::detail
				::msvc_index_specifier<IndexSpecifierMeta::type>
					::template result_index_class<SuperMeta>
	{
	}; 
};

#else

struct index_applier
{
	template<typename IndexSpecifierMeta, typename SuperMeta>
	struct apply
	{
		typedef typename IndexSpecifierMeta::type index_specifier;
		typedef typename 
			index_specifier
				::BOOST_NESTED_TEMPLATE index_class<SuperMeta>::type type;
	}; 
};

#endif // BOOST_WORKAROUND(BOOST_MSVC,<1310)

template<int N, typename Value, typename IndexSpecifierList, typename Allocator>
struct nth_layer_hacking
{
	YGGR_STATIC_CONSTANT(int, length = boost::mpl::size<IndexSpecifierList>::value);

	typedef typename 
		boost::mpl::eval_if_c
		<
			N == length,
			boost::mpl::identity
			< 
				node_handle_foo_hacking_midx_index_base
				<
					Value,
					IndexSpecifierList,
					Allocator
				> 
			>,
			boost::mpl::apply2
			<
				index_applier,
				boost::mpl::at_c<IndexSpecifierList, N>,
				nth_layer_hacking<N+1, Value, IndexSpecifierList, Allocator>
			>
		>::type type;
};

template<typename Value, typename IndexSpecifierList, typename Allocator>
struct multi_index_base_hacking_type
	: nth_layer_hacking<0, Value, IndexSpecifierList, Allocator>
{
	BOOST_STATIC_ASSERT(boost::multi_index::detail::is_index_list<IndexSpecifierList>::value);
};


} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_BASE_TYPE_HPP__