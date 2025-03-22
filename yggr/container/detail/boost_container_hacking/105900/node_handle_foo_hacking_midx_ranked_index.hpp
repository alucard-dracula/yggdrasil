// node_handle_foo_hacking_midx_ranked_index.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105900_NODE_HANDLE_FOO_HACKING_MIDX_RANKED_INDEX_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105900_NODE_HANDLE_FOO_HACKING_MIDX_RANKED_INDEX_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_DECL_HPP__
#	error "this file is inner file, can't include it!"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MIDX_ORDERED_INDEX_DECL_HPP__

#include <yggr/container/detail/node_handle_foo_hacking_midx_ranked_index_decl.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename OrderedIndexImpl>
class node_handle_foo_hacking_midx_ranked_index;

template<typename T>
struct make_augmented_interface_hacking<T, boost::multi_index::detail::rank_policy>
{
	typedef node_handle_foo_hacking_midx_ranked_index<T> type;
};

template<typename OrderedIndexImpl>
class node_handle_foo_hacking_midx_ranked_index
	: public OrderedIndexImpl
{
	typedef OrderedIndexImpl super;

protected:
	typedef typename super::index_node_type         index_node_type;
	typedef typename super::node_impl_pointer node_impl_pointer;

public:
	typedef typename super::ctor_args_list ctor_args_list;
	typedef typename super::allocator_type allocator_type;
	typedef typename super::iterator       iterator;

private:
	typedef node_handle_foo_hacking_midx_ranked_index this_type;

private:
	node_handle_foo_hacking_midx_ranked_index(void);
	node_handle_foo_hacking_midx_ranked_index(BOOST_RV_REF(this_type));
	node_handle_foo_hacking_midx_ranked_index(const this_type&);
	~node_handle_foo_hacking_midx_ranked_index(void);
	const this_type& operator=( const this_type& );
	const this_type& operator=( BOOST_RV_REF(this_type) );
};

template<typename OrderedIndexImpl>
struct node_handle_traits_impl_custom< boost::multi_index::detail::ranked_index<OrderedIndexImpl> >
{
public:
	typedef boost::multi_index::detail::ranked_index<OrderedIndexImpl> native_type;
	typedef typename index_hacking_cast<native_type>::type hacking_type;

public:
	typedef typename hacking_type::node_type node_type;
	typedef typename hacking_type::insert_return_type insert_return_type;
	typedef typename hacking_type::iterator hint_insert_return_type;
};


template<typename Arg1, typename Arg2, typename Arg3>
struct node_handle_foo_hacking_midx_ranked_unique
{
	typedef boost::multi_index::ranked_unique<Arg1, Arg2, Arg3> org_type;

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
				boost::multi_index::detail::rank_policy,
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
				boost::multi_index::detail::rank_policy
			> type;
	};
};

template<typename Arg1, typename Arg2, typename Arg3>
struct node_handle_foo_hacking_midx_ranked_non_unique
{
	typedef boost::multi_index::ranked_non_unique<Arg1, Arg2, Arg3> org_type;

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
				boost::multi_index::detail::rank_policy,
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
				boost::multi_index::detail::rank_policy
			> type;
	};
};

} // namespace yggr
} // namespace container
} // namespace detail

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105900_NODE_HANDLE_FOO_HACKING_MIDX_RANKED_INDEX_HPP__
