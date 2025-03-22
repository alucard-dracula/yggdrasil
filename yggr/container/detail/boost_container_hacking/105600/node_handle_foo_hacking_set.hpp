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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105600_NODE_HANDLE_FOO_HACKING_SET_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105600_NODE_HANDLE_FOO_HACKING_SET_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_HPP__
#	error "this file is inner file, can't include it, please include <yggr/container/set.hpp>"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_HPP__

#include <yggr/iterator_ex/is_iterator.hpp>

#include <yggr/container/detail/node_handle_traits.hpp>
#include <yggr/container/detail/get_hint_insert_return.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_tree.hpp>
#include <yggr/container/detail/node_handle.hpp>
#include <yggr/container/detail/native_extractable.hpp>
#include <yggr/container/detail/native_node_insertable.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
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
template<template<typename _K, typename _C, typename _A, typename _SO> class Set,
			typename K, typename C, typename A, typename SO>
class node_handle_foo_hacking_set< Set<K, C, A, SO> >
	: protected 
		node_handle_foo_hacking_tree
		<
			boost::container::container_detail::tree
			<
				K, 
				K, 
				boost::container::container_detail::identity<K>, 
				C, 
				A, 
				SO
			>
		>
{
private:
	typedef node_handle_foo_hacking_set this_type;

public:
	typedef Set<K, C, A, SO> org_set_type;
	typedef node_handle_foo_hacking_set hacking_set_type;

public:
	// org
	typedef typename org_set_type::key_type key_type;
	typedef typename org_set_type::value_type value_type;

	typedef typename org_set_type::iterator iterator;
	typedef typename org_set_type::const_iterator const_iterator;

public:
	typedef 
		boost::container::container_detail::tree
		<
			K, 
			K, 
			boost::container::container_detail::identity<K>, 
			C, 
			A, 
			SO
		> org_tree_t;

	typedef node_handle_foo_hacking_tree<org_tree_t> hacking_tree_type;
	typedef hacking_tree_type base_type;

	typedef typename hacking_tree_type::Icont Icont;
	typedef typename hacking_tree_type::iiterator iiterator;
	typedef typename hacking_tree_type::NodeAlloc NodeAlloc;

	typedef typename hacking_tree_type::node_type node_type;
	typedef typename hacking_tree_type::insert_return_type insert_return_type;

public:
	using base_type::extract;

	//YGGR_CONTAINER_FORCEINLINE 
	inline insert_return_type insert(BOOST_RV_REF(node_type) nh)
	{  
		return base_type::insert_unique_node(boost::move(nh));  
	}

	//YGGR_CONTAINER_FORCEINLINE 
	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{ 
		return get_hint_insert_return(base_type::insert_unique_node(hint, boost::move(nh)));  
	}
};

template<typename T, typename C, typename A, typename SO>
struct node_handle_traits_impl_custom< boost::container::set<T, C, A, SO> >
{
	typedef typename 
		detail::node_handle_foo_hacking_set
		<
			boost::container::set<T, C, A, SO> 
		>::node_type node_type;

	typedef typename 
		detail::node_handle_foo_hacking_set
		<
			boost::container::set<T, C, A, SO> 
		>::insert_return_type insert_return_type;

	typedef typename 
		boost::container::set<T, C, A, SO>::iterator hint_insert_return_type;
};

template<
	template<typename _K, typename _C, typename _A, typename _SO> class Set,
	typename K, typename C, typename A, typename SO, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			native_extractable< Set<K, C, A, SO> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename Set<K, C, A, SO>::node_type
	>::type
	set_node_extract(Set<K, C, A, SO>& s, BOOST_RV_REF(Key) key)
{
	return s.extract(boost::move(key));
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class Set,
	typename K, typename C, typename A, typename SO, typename KeyOrIter> inline
typename
	boost::enable_if
	<
		native_extractable< Set<K, C, A, SO> >,
		typename Set<K, C, A, SO>::node_type
	>::type
	set_node_extract(Set<K, C, A, SO>& s, const KeyOrIter& key_or_iter)
{
	return s.extract(key_or_iter);
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class Set,
	typename K, typename C, typename A, typename SO, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_< native_extractable< Set<K, C, A, SO> > >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename node_handle_foo_hacking_set<Set<K, C, A, SO> >::node_type
	>::type
	set_node_extract(Set<K, C, A, SO>& s, BOOST_RV_REF(Key) key)
{
	typedef node_handle_foo_hacking_set< Set<K, C, A, SO> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).extract(boost::move(key));
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class Set,
	typename K, typename C, typename A, typename SO, typename KeyOrIter> inline
typename
	boost::disable_if
	<
		native_extractable< Set<K, C, A, SO> >,
		typename node_handle_foo_hacking_set<Set<K, C, A, SO> >::node_type
	>::type
	set_node_extract(Set<K, C, A, SO>& s, const KeyOrIter& key_or_iter)
{
	typedef node_handle_foo_hacking_set< Set<K, C, A, SO> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).extract(key_or_iter);
}

// insert
template<
	template<typename _K, typename _C, typename _A, typename _SO> class Set,
	typename K, typename C, typename A, typename SO> inline
typename
	boost::enable_if
	<
		native_node_insertable< Set<K, C, A, SO> >,
		typename Set<K, C, A, SO>::insert_return_type
	>::type
	set_node_insert(Set<K, C, A, SO>& s, 
					BOOST_RV_REF_BEG typename Set<K, C, A, SO>::node_type BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class Set,
	typename K, typename C, typename A, typename SO> inline
typename
	boost::enable_if
	<
		native_node_insertable< Set<K, C, A, SO> >,
		typename Set<K, C, A, SO>::iterator
	>::type
	set_node_insert(Set<K, C, A, SO>& s, 
					typename Set<K, C, A, SO>::const_iterator hint,
					BOOST_RV_REF_BEG typename Set<K, C, A, SO>::node_type BOOST_RV_REF_END node)
{
	return get_hint_insert_return(s.insert(hint, boost::move(node)));
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class Set,
	typename K, typename C, typename A, typename SO> inline
typename
	boost::disable_if
	<
		native_node_insertable< Set<K, C, A, SO> >,
		typename node_handle_foo_hacking_set<Set<K, C, A, SO> >::insert_return_type
	>::type
	set_node_insert(Set<K, C, A, SO>& s, 
					BOOST_RV_REF_BEG 
						typename node_handle_foo_hacking_set<Set<K, C, A, SO> >::node_type
					BOOST_RV_REF_END node)
{
	typedef node_handle_foo_hacking_set< Set<K, C, A, SO> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).insert(boost::move(node));
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class Set,
	typename K, typename C, typename A, typename SO> inline
typename
	boost::disable_if
	<
		native_node_insertable< Set<K, C, A, SO> >,
		typename Set<K, C, A, SO>::iterator
	>::type
	set_node_insert(Set<K, C, A, SO>& s, 
					typename Set<K, C, A, SO>::const_iterator hint,
					BOOST_RV_REF_BEG 
						typename node_handle_foo_hacking_set<Set<K, C, A, SO> >::node_type
					BOOST_RV_REF_END node)
{
	typedef node_handle_foo_hacking_set< Set<K, C, A, SO> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).insert(hint, boost::move(node));
}

// node_handle_foo_hacking_multiset
template<template<typename _K, typename _C, typename _A, typename _SO> class MultiSet,
			typename K, typename C, typename A, typename SO>
class node_handle_foo_hacking_multiset< MultiSet<K, C, A, SO> >
	: protected 
		node_handle_foo_hacking_tree
		<
			boost::container::container_detail::tree
			<
				K, 
				K, 
				boost::container::container_detail::identity<K>, 
				C, 
				A, 
				SO
			>
		>
{
private:
	typedef node_handle_foo_hacking_multiset this_type;

public:
	typedef MultiSet<K, C, A, SO> org_multiset_type;
	typedef node_handle_foo_hacking_multiset hacking_multiset_type;

public:
	// org
	typedef typename org_multiset_type::key_type key_type;
	typedef typename org_multiset_type::value_type value_type;

	typedef typename org_multiset_type::iterator iterator;
	typedef typename org_multiset_type::const_iterator const_iterator;


public:
	typedef 
		boost::container::container_detail::tree
		<
			K, 
			K, 
			boost::container::container_detail::identity<K>, 
			C, 
			A, 
			SO
		> org_tree_t;

	typedef node_handle_foo_hacking_tree<org_tree_t> hacking_tree_type;
	typedef hacking_tree_type base_type;

	typedef typename hacking_tree_type::Icont Icont;
	typedef typename hacking_tree_type::iiterator iiterator;
	typedef typename hacking_tree_type::NodeAlloc NodeAlloc;

	typedef typename hacking_tree_type::node_type node_type;

public:
	using base_type::extract;

	inline iterator insert(BOOST_RV_REF(node_type) nh)
	{  
		return base_type::insert_equal_node(boost::move(nh));  
	}

	inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) nh)
	{ 
		return base_type::insert_equal_node(hint, boost::move(nh));  
	}
};

template<typename T, typename C, typename A, typename SO>
struct node_handle_traits_impl_custom< boost::container::multiset<T, C, A, SO> >
{
	typedef typename 
		detail::node_handle_foo_hacking_multiset
		<
			boost::container::multiset<T, C, A, SO> 
		>::node_type node_type;

	typedef typename boost::container::multiset<T, C, A, SO>::iterator insert_return_type;

	typedef insert_return_type hint_insert_return_type;
};

template<
	template<typename _K, typename _C, typename _A, typename _SO> class MultiSet,
	typename K, typename C, typename A, typename SO, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			native_extractable< MultiSet<K, C, A, SO> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename MultiSet<K, C, A, SO>::node_type
	>::type
	multiset_node_extract(MultiSet<K, C, A, SO>& s, BOOST_RV_REF(Key) key)
{
	return s.extract(boost::move(key));
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class MultiSet,
	typename K, typename C, typename A, typename SO, typename KeyOrIter> inline
typename
	boost::enable_if
	<
		native_extractable< MultiSet<K, C, A, SO> >,
		typename MultiSet<K, C, A, SO>::node_type
	>::type
	multiset_node_extract(MultiSet<K, C, A, SO>& s, const KeyOrIter& key_or_iter)
{
	return s.extract(key_or_iter);
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class MultiSet,
	typename K, typename C, typename A, typename SO, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_< native_extractable< MultiSet<K, C, A, SO> > >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename node_handle_foo_hacking_multiset<MultiSet<K, C, A, SO> >::node_type
	>::type
	multiset_node_extract(MultiSet<K, C, A, SO>& s, BOOST_RV_REF(Key) key)
{
	typedef Key key_type;
	typedef node_handle_foo_hacking_multiset< MultiSet<K, C, A, SO> > hacked_set_type;
	const key_type& key_arg = key;
	return reinterpret_cast<hacked_set_type&>(s).extract(key_arg);
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class MultiSet,
	typename K, typename C, typename A, typename SO, typename KeyOrIter> inline
typename
	boost::disable_if
	<
		native_extractable< MultiSet<K, C, A, SO> >,
		typename node_handle_foo_hacking_multiset<MultiSet<K, C, A, SO> >::node_type
	>::type
	multiset_node_extract(MultiSet<K, C, A, SO>& s, const KeyOrIter& key_or_iter)
{
	typedef node_handle_foo_hacking_multiset< MultiSet<K, C, A, SO> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).extract(key_or_iter);
}

// insert

template<
	template<typename _K, typename _C, typename _A, typename _SO> class MultiSet,
	typename K, typename C, typename A, typename SO> inline
typename
	boost::enable_if
	<
		native_node_insertable< MultiSet<K, C, A, SO> >,
		typename MultiSet<K, C, A, SO>::iterator
	>::type
	multiset_node_insert(MultiSet<K, C, A, SO>& s, 
							BOOST_RV_REF_BEG 
								typename MultiSet<K, C, A, SO>::node_type 
							BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class MultiSet,
	typename K, typename C, typename A, typename SO> inline
typename
	boost::enable_if
	<
		native_node_insertable< MultiSet<K, C, A, SO> >,
		typename MultiSet<K, C, A, SO>::iterator
	>::type
	multiset_node_insert(MultiSet<K, C, A, SO>& s, 
							typename MultiSet<K, C, A, SO>::const_iterator hint,
							BOOST_RV_REF_BEG 
								typename MultiSet<K, C, A, SO>::node_type 
							BOOST_RV_REF_END node)
{
	return s.insert(hint, boost::move(node));
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class MultiSet,
	typename K, typename C, typename A, typename SO> inline
typename
	boost::disable_if
	<
		native_node_insertable< MultiSet<K, C, A, SO> >,
		typename MultiSet<K, C, A, SO>::iterator
	>::type
	multiset_node_insert(MultiSet<K, C, A, SO>& s, 
							BOOST_RV_REF_BEG 
								typename node_handle_foo_hacking_multiset<MultiSet<K, C, A, SO> >::node_type
							BOOST_RV_REF_END node)
{
	typedef node_handle_foo_hacking_multiset< MultiSet<K, C, A, SO> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).insert(boost::move(node));
}

template<
	template<typename _K, typename _C, typename _A, typename _SO> class MultiSet,
	typename K, typename C, typename A, typename SO> inline
typename
	boost::disable_if
	<
		native_node_insertable< MultiSet<K, C, A, SO> >,
		typename MultiSet<K, C, A, SO>::iterator
	>::type
	multiset_node_insert(MultiSet<K, C, A, SO>& s, 
							typename MultiSet<K, C, A, SO>::const_iterator hint,
							BOOST_RV_REF_BEG 
								typename node_handle_foo_hacking_multiset<MultiSet<K, C, A, SO> >::node_type
							BOOST_RV_REF_END node)
{
	typedef node_handle_foo_hacking_multiset< MultiSet<K, C, A, SO> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).insert(hint, boost::move(node));
}

} // namespace detail
} // namespace container
} // namespace yggr

#include <yggr/container/detail/node_handle_foo_hacking_set_foo_impl.hpp>

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105600_NODE_HANDLE_FOO_HACKING_SET_HPP__
