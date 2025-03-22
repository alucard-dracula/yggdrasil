// node_handle_foo_hacking_midx_index_base.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLER_FOO_HACKING_MIDX_BASE_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLER_FOO_HACKING_MIDX_BASE_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLER_FOO_HACKING_MIDX_BASE_HPP__
#	error "this file is inner file, can't include it!"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLER_FOO_HACKING_MIDX_BASE_HPP__

#include <yggr/container/detail/cast_midx_indexed_by.hpp>

#include <yggr/container/detail/node_handle_foo_hacking_midx_container_decl.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_midx_index_base_decl.hpp>
#include <yggr/container/detail/node_handle_midx.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename Value, typename IndexSpecifierList, typename Allocator>
class node_handle_foo_hacking_midx_index_base
{
public:
	typedef
		boost::multi_index::detail::index_base
		<
			Value,
			typename indexed_by_native_cast<IndexSpecifierList>::type,
			Allocator
		>															org_type;

protected:
	typedef
		boost::multi_index::detail::index_node_base
		<
			Value,
			Allocator
		>															index_node_type;

	typedef typename
		boost::multi_index::detail::multi_index_node_type
		<
			Value,
			IndexSpecifierList,
			Allocator
		>::type														final_node_type;

	typedef
		node_handle_foo_hacking_midx_container
		<
			Value,
			IndexSpecifierList,
			Allocator
		>															final_type;

	typedef boost::tuples::null_type								ctor_args_list;
	typedef typename
		boost::detail::allocator::rebind_to
		<
			Allocator,
			typename Allocator::value_type
		>::type														final_allocator_type;

	typedef 
		node_handle_midx
		<
			final_node_type,
			final_allocator_type
		>															final_node_handle_type;

	typedef boost::mpl::vector0<>									index_type_list;
	typedef boost::mpl::vector0<>									iterator_type_list;
	typedef boost::mpl::vector0<>									const_iterator_type_list;

	typedef
		boost::multi_index::detail::copy_map
		<
			final_node_type,
			final_allocator_type
		>															copy_map_type;

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
	typedef
		boost::multi_index::detail::index_saver
		<
			index_node_type,
			final_allocator_type
		>										                     index_saver_type;

	typedef
		boost::multi_index::detail::index_loader
		<
			index_node_type,
			final_node_type,
			final_allocator_type
		>										                    index_loader_type;
#endif // BOOST_MULTI_INDEX_DISABLE_SERIALIZATION

protected:
	typedef typename boost::call_traits<Value>::param_type	value_param_type;

private:
	typedef node_handle_foo_hacking_midx_index_base this_type;

private:
	node_handle_foo_hacking_midx_index_base(void);
	node_handle_foo_hacking_midx_index_base(BOOST_RV_REF(this_type));
	node_handle_foo_hacking_midx_index_base(const this_type&);
	~node_handle_foo_hacking_midx_index_base(void);
	const this_type& operator=( const this_type& );
	const this_type& operator=( BOOST_RV_REF(this_type) );


protected:
	inline void extract_(index_node_type*)
	{
	}

	template<typename Tag> inline 
	typename 
		boost::enable_if
		<
			boost::is_base_of<emplaced_tag, Tag>,
			index_node_type* 
		>::type
		insert_(value_param_type v, index_node_type* x, Tag)
	{
		return x;
	}

	template<typename Tag> inline 
	typename 
		boost::enable_if
		<
			boost::is_base_of<emplaced_tag, Tag>,
			index_node_type* 
		>::type
		insert_(value_param_type v, index_node_type*, index_node_type* x, Tag)
	{
		return x;
	}

	template<typename Tag> inline 
	typename 
		boost::disable_if
		<
			boost::is_base_of<emplaced_tag, Tag>,
			index_node_type* 
		>::type
		insert_(value_param_type v, index_node_type* x, Tag)
	{
		boost::detail::allocator::construct(::boost::addressof(x->value()), v);
		return x;
	}

	template<typename Tag> inline 
	typename 
		boost::disable_if
		<
			boost::is_base_of<emplaced_tag, Tag>,
			index_node_type* 
		>::type
		insert_(value_param_type v, index_node_type*, index_node_type* x, Tag)
	{
		boost::detail::allocator::construct(::boost::addressof(x->value()), v);
		return x;
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
	/* invariant stuff */

	inline bool invariant_(void) const { return true; }

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

	/* access to backbone memfuns of Final class */

	inline final_type& final(void)
	{
		return *static_cast<final_type*>(this);
	}

	inline const final_type& final(void) const
	{
		return *static_cast<const final_type*>(this);
	}

	inline final_node_type* final_header() const
	{
		return final().header();
	}

	// final_extract_
	inline final_node_handle_type final_extract_(final_node_type* x)
	{
		return final().extract_(x);
	}

	inline std::pair<final_node_type*, bool> 
		final_insert_nh_(final_node_handle_type& nh)
    {
		return final().insert_nh_(nh);
	}

	inline std::pair<final_node_type*, bool> 
		final_insert_nh_(final_node_handle_type& nh, final_node_type* position)
    {
		return final().insert_nh_(nh, position);
	}

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)

	inline void final_check_invariant_(void) const
	{ 
		final().check_invariant_(); 
	}

#endif // BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING

};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLER_FOO_HACKING_MIDX_BASE_HPP__
