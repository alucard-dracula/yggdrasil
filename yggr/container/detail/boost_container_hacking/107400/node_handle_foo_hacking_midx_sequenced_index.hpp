// node_handle_foo_hacking_midx_sequenced_index.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_107400_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_107400_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_HPP__
#	error "this file is inner file, can't include it, please include <yggr/container/multi_index_sequenced_index.hpp>"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_HPP__

#include <yggr/container/detail/node_handle_traits.hpp>
#include <boost/multi_index/sequenced_index.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename S, typename T>
struct node_handle_traits_impl_custom< boost::multi_index::detail::sequenced_index<S, T> >
{
private:
	typedef boost::multi_index::detail::sequenced_index<S, T> native_type;

public:
	typedef typename native_type::node_type node_type;
	typedef typename native_type::insert_return_type insert_return_type;
	typedef typename native_type::insert_return_type hint_insert_return_type;
};

} // namespace detail
} // namespace container
} // namespace yggr


#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_107400_NODE_HANDLE_FOO_HACKING_MIDX_SEQUENCED_INDEX_HPP__
