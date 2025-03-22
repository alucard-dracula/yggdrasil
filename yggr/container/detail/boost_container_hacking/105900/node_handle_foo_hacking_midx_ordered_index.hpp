// node_handle_foo_hacking_midx_ordered_index.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105900_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105900_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_HPP__
#	error "this file is inner file, can't include it, please include <yggr/container/multi_index_ordered_index.hpp>"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_HPP__

#include <yggr/container/detail/node_handle_foo_hacking_midx_ordered_index_decl.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T>
struct make_augmented_interface_hacking<T, boost::multi_index::detail::null_augment_policy>
{
	typedef T type;
};

template<typename Arg1, typename Arg2, typename Arg3>
struct node_handle_foo_hacking_midx_ordered_unique
{
	typedef boost::multi_index::ordered_unique<Arg1, Arg2, Arg3> org_type;

	typedef typename org_type::index_args index_args;
	typedef typename org_type::tag_list_type tag_list_type;
	typedef typename org_type::key_from_value_type key_from_value_type;
	typedef typename org_type::compare_type compare_type;

	template<typename Super>
	struct node_class
	{
		typedef 
			boost::multi_index::detail::ordered_index_node
			<
				boost::multi_index::detail::null_augment_policy,
				Super
			> type;
	};

	template<typename SuperMeta>
	struct index_class
	{
		typedef 
			node_handle_foo_hacking_midx_ordered_index
			<
				key_from_value_type, 
				compare_type,
				SuperMeta,
				tag_list_type,
				boost::multi_index::detail::ordered_unique_tag,
				boost::multi_index::detail::null_augment_policy
			> type;
	};
};

template<typename Arg1, typename Arg2, typename Arg3>
struct node_handle_foo_hacking_midx_ordered_non_unique
{
	typedef boost::multi_index::ordered_non_unique<Arg1, Arg2, Arg3> org_type;

	typedef typename org_type::index_args index_args;
	typedef typename org_type::tag_list_type tag_list_type;
	typedef typename org_type::key_from_value_type key_from_value_type;
	typedef typename org_type::compare_type compare_type;

	template<typename Super>
	struct node_class
	{
		typedef 
			boost::multi_index::detail::ordered_index_node
			<
				boost::multi_index::detail::null_augment_policy,
				Super
			> type;
	};

	template<typename SuperMeta>
	struct index_class
	{
		typedef 
			node_handle_foo_hacking_midx_ordered_index
			<
				key_from_value_type,
				compare_type,
				SuperMeta,
				tag_list_type,
				boost::multi_index::detail::ordered_non_unique_tag,
				boost::multi_index::detail::null_augment_policy
			> type;
	};
};

} // namespace yggr
} // namespace container
} // namespace detail

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105900_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_HPP__
