// node_handle_foo_hacking_set.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_SET_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_SET_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_HPP__
#	error "this file is inner file, can't include it, please include <yggr/container/set.hpp>"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_HPP__

#include <yggr/container/detail/node_handle_traits.hpp>
#include <yggr/container/detail/get_hint_insert_return.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_tree.hpp>
#include <yggr/container/detail/node_handle.hpp>
#include <yggr/container/detail/native_extractable.hpp>
#include <yggr/container/detail/native_node_insertable.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/container/set.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename Set>
class node_handle_foo_hacking_set;

template<typename MultiSet>
class node_handle_foo_hacking_multiset;

// node_handle_foo_hacking_set
template<template<typename _K, typename _C, typename _A> class Set,
			typename K, typename C, typename A>
class node_handle_foo_hacking_set< Set<K, C, A> >
{
private:
	typedef node_handle_foo_hacking_set this_type;

public:
	typedef Set<K, C, A> org_set_type;
	typedef node_handle_foo_hacking_set hacking_set_type;

public:
	// org
	typedef typename org_set_type::key_type key_type;
	typedef typename org_set_type::value_type value_type;

	typedef typename org_set_type::iterator iterator;
	typedef typename org_set_type::const_iterator const_iterator;

public:
	typedef 
		boost::container::container_detail::rbtree
		<
			K, 
			K,
            boost::container::container_detail::identity<K>,
			C, 
			A
		> org_tree_t;

	typedef node_handle_foo_hacking_tree<org_tree_t> hacking_tree_type;
	typedef hacking_tree_type base_type;

	typedef typename hacking_tree_type::Icont Icont;
	typedef typename hacking_tree_type::iiterator iiterator;
	typedef typename hacking_tree_type::NodeAlloc NodeAlloc;

	typedef typename hacking_tree_type::node_type node_type;
	typedef typename hacking_tree_type::insert_return_type insert_return_type;

protected:
	inline hacking_tree_type& cast_to_hacking_tree(void) YGGR_NOEXCEPT
	{
		return reinterpret_cast<hacking_tree_type&>(m_tree);
	}

	inline const hacking_tree_type& cast_to_hacking_tree(void) const YGGR_NOEXCEPT
	{
		return reinterpret_cast<const hacking_tree_type&>(m_tree);
	}

public:
	template<typename Key> inline
	node_type extract(BOOST_RV_REF(Key) k)
	{
		return cast_to_hacking_tree().extract(boost::move(k));
	}

	inline node_type extract(const key_type& k)
	{
		return cast_to_hacking_tree().extract(k);
	}

	inline node_type extract(const_iterator position)
	{
		return cast_to_hacking_tree().extract(position);
	}

	inline insert_return_type insert(BOOST_RV_REF(node_type) nh)
	{  
		return cast_to_hacking_tree().insert_unique_node(boost::move(nh));  
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{ 
		return
			get_hint_insert_return(
				cast_to_hacking_tree().insert_unique_node(hint, boost::move(nh)));  
	}

public:
   org_tree_t m_tree; 
};

template<typename T, typename C, typename A>
struct node_handle_traits_impl_custom< boost::container::set<T, C, A> >
{
	typedef typename 
		detail::node_handle_foo_hacking_set
		<
			boost::container::set<T, C, A> 
		>::node_type node_type;

	typedef typename 
		detail::node_handle_foo_hacking_set
		<
			boost::container::set<T, C, A> 
		>::insert_return_type insert_return_type;

	//typedef insert_return_type hint_insert_return_type;
	typedef typename 
		boost::container::set<T, C, A>::iterator hint_insert_return_type;
};

// node_handle_foo_hacking_multiset
template<template<typename _K, typename _C, typename _A> class MultiSet,
			typename K, typename C, typename A>
class node_handle_foo_hacking_multiset< MultiSet<K, C, A> >
{
private:
	typedef node_handle_foo_hacking_multiset this_type;

public:
	typedef MultiSet<K, C, A> org_multiset_type;
	typedef node_handle_foo_hacking_multiset hacking_multiset_type;

public:
	// org
	typedef typename org_multiset_type::key_type key_type;
	typedef typename org_multiset_type::value_type value_type;

	typedef typename org_multiset_type::iterator iterator;
	typedef typename org_multiset_type::const_iterator const_iterator;

public:
	typedef 
		boost::container::container_detail::rbtree
		<
			K, 
			K,
            boost::container::container_detail::identity<K>,
			C, 
			A
		> org_tree_t;

	typedef node_handle_foo_hacking_tree<org_tree_t> hacking_tree_type;
	typedef hacking_tree_type base_type;

	typedef typename hacking_tree_type::Icont Icont;
	typedef typename hacking_tree_type::iiterator iiterator;
	typedef typename hacking_tree_type::NodeAlloc NodeAlloc;

	typedef typename hacking_tree_type::node_type node_type;
	//multiset not has insert_return_type
	//typedef typename hacking_tree_type::insert_return_type insert_return_type;

protected:
	inline hacking_tree_type& cast_to_hacking_tree(void) YGGR_NOEXCEPT
	{
		return reinterpret_cast<hacking_tree_type&>(m_tree);
	}

	inline const hacking_tree_type& cast_to_hacking_tree(void) const YGGR_NOEXCEPT
	{
		return reinterpret_cast<const hacking_tree_type&>(m_tree);
	}

public:
	template<typename Key> inline
	node_type extract(BOOST_RV_REF(Key) k)
	{
		return cast_to_hacking_tree().extract(boost::move(k));
	}

	inline node_type extract(const key_type& k)
	{
		return cast_to_hacking_tree().extract(k);
	}

	inline node_type extract(const_iterator position)
	{
		return cast_to_hacking_tree().extract(position);
	}

	inline iterator insert(BOOST_RV_REF(node_type) nh)
	{  
		return cast_to_hacking_tree().insert_equal_node(boost::move(nh));  
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{ 
		return cast_to_hacking_tree().insert_equal_node(hint, boost::move(nh));  
	}

public:
   org_tree_t m_tree; 
};

template<typename T, typename C, typename A>
struct node_handle_traits_impl_custom< boost::container::multiset<T, C, A> >
{
	typedef typename 
		detail::node_handle_foo_hacking_multiset
		<
			boost::container::multiset<T, C, A> 
		>::node_type node_type;

	typedef typename boost::container::multiset<T, C, A>::iterator insert_return_type;

	typedef insert_return_type hint_insert_return_type;
};

} // namespace detail
} // namespace container
} // namespace yggr

#include <yggr/container/detail/node_handle_foo_hacking_set_foo_impl.hpp>

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_SET_HPP__
