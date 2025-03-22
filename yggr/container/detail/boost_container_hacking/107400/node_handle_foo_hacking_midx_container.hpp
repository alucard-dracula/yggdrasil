// node_handle_foo_hacking_midx_container.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_107400_NODE_HANDLE_FOO_HACKING_MIDX_CONTAINER_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_107400_NODE_HANDLE_FOO_HACKING_MIDX_CONTAINER_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_CONTAINER_HPP__
#	error "this file is inner file, can't include it, please include <yggr/container/multi_index_container.hpp>"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_CONTAINER_HPP__

#include <yggr/container/detail/node_handle_traits.hpp>
#include <boost/multi_index_container.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename V, typename I, typename A>
struct node_handle_traits_impl_custom< boost::multi_index::multi_index_container<V, I, A> >
	: public 
		node_handle_traits
		<
			typename
				boost::multi_index::detail::multi_index_base_type
				<
					V, I, A
				>::type
		>
{
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_107400_NODE_HANDLE_FOO_HACKING_MIDX_CONTAINER_HPP__
