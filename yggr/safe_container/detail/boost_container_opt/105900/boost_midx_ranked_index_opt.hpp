// boost_midx_ranked_index_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105900_BOOST_MIDX_RANKED_INDEX_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105900_BOOST_MIDX_RANKED_INDEX_OPT_HPP__

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_RANKED_INDEX_OPT_HPP__
#	error "this file is inner file, can't include it, please include <yggr/safe_container/safe_multi_index_ranked_index.hpp>"
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_RANKED_INDEX_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/yggrdef.h>

#include <yggr/safe_container/detail/boost_midx_ranked_index_opt_decl.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename OrderedIndexImpl>
class boost_midx_ranked_index_opt;

template<typename T>
struct make_augmented_interface_opt<T, boost::multi_index::detail::rank_policy>
{
	typedef boost_midx_ranked_index_opt<T> type;
};

template<typename OrderedIndexImpl>
class boost_midx_ranked_index_opt
	: public OrderedIndexImpl
{
	typedef OrderedIndexImpl super;

protected:
	typedef typename super::index_node_type         index_node_type;
	typedef typename super::node_impl_pointer		node_impl_pointer;

public:
	typedef typename super::ctor_args_list ctor_args_list;
	typedef typename super::allocator_type allocator_type;
	typedef typename super::iterator       iterator;

private:
	typedef boost_midx_ranked_index_opt this_type;

private:
	boost_midx_ranked_index_opt(void);
	boost_midx_ranked_index_opt(BOOST_RV_REF(boost_midx_ranked_index_opt));
	boost_midx_ranked_index_opt(const boost_midx_ranked_index_opt&);
	~boost_midx_ranked_index_opt(void);
	const boost_midx_ranked_index_opt& operator=( const boost_midx_ranked_index_opt& );
	const boost_midx_ranked_index_opt& operator=( BOOST_RV_REF(boost_midx_ranked_index_opt) );
};

template<typename Arg1, typename Arg2, typename Arg3>
struct boost_midx_ranked_unique_opt
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
			boost_midx_ordered_index_opt
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
struct boost_midx_ranked_non_unique_opt
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
			boost_midx_ordered_index_opt
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
} // namespace safe_container
} // namespace detail

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_CONTAINER_OPT_105900_BOOST_MIDX_RANKED_INDEX_OPT_HPP__
