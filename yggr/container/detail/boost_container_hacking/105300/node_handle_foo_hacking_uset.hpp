// node_handle_foo_hacking_uset.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_USET_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_USET_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_USET_HPP__
#	error "this file is inner file, can't include it, please include <yggr/container/unordered_set.hpp>"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_USET_HPP__

#include <yggr/move/move.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>

#include <yggr/container/detail/node_handle_unordered_set.hpp>
#include <yggr/container/detail/insert_return_type_base.hpp>
#include <yggr/container/detail/node_handle_foo_hacking_table.hpp>
#include <yggr/container/detail/native_extractable.hpp>
#include <yggr/container/detail/native_node_insertable.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/unordered_set.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

template<typename USet>
class node_handle_foo_hacking_unordered_set;

template<typename UMultiSet>
class node_handle_foo_hacking_unordered_multiset;

// node_handle_foo_hacking_unordered_set
template <template<typename _T, typename _H, typename _P, typename _A> class USet,
			typename T, typename H, typename P, typename A>
class node_handle_foo_hacking_unordered_set<USet<T, H, P, A> >
{
private:
	typedef node_handle_foo_hacking_unordered_set this_type;

public:
	typedef USet<T, H, P, A> org_uset_type;
	typedef node_handle_foo_hacking_unordered_set hacking_uset_type;

public:
	typedef typename org_uset_type::key_type key_type;
	typedef typename org_uset_type::value_type value_type;

	typedef typename org_uset_type::hasher hasher;
	typedef typename org_uset_type::key_equal key_equal;
	typedef typename org_uset_type::allocator_type allocator_type;

	typedef typename org_uset_type::iterator iterator;
	typedef typename org_uset_type::const_iterator const_iterator;


public:
    typedef boost::unordered::detail::set<A, T, H, P> types;
    typedef typename types::table org_table;
	typedef typename types::node node;

	typedef org_table org_table_impl_type;
	typedef node_handle_foo_hacking_table_impl<org_table_impl_type> hacking_table_impl_type;

	typedef typename hacking_table_impl_type::node_pointer node_pointer;


public:
	typedef node_handle_uset<node, value_type, allocator_type> node_type;
	typedef 
		insert_return_type_base
		<
			iterator,
			node_type
		> insert_return_type;

public:
	YGGR_CONSTEXPR_OR_INLINE 
	hacking_table_impl_type& hacking_table_impl_cast(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<hacking_table_impl_type&>(table_);
	}

	YGGR_CONSTEXPR_OR_INLINE 
	const hacking_table_impl_type& hacking_table_impl_cast(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<const hacking_table_impl_type&>(table_);
	}

public:
	// extract
    inline node_type extract(const_iterator position)
    {
        return 
			node_type(
				hacking_table_impl_cast().extract_by_iterator(position), 
				table_.node_alloc());
    }

    inline node_type extract(const key_type& k)
    {
        return 
			node_type(
				hacking_table_impl_cast().extract_by_key(k), 
				table_.node_alloc());
    }

	//insert
	inline insert_return_type insert(BOOST_RV_REF(node_type) np)
	{
		insert_return_type result;
		hacking_table_impl_cast().move_insert_node_type(static_cast<node_type&>(np), result);
		return YGGR_MOVE_RET(insert_return_type, result);
	}

    inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) np)
    {
        return 
			hacking_table_impl_cast().move_insert_node_type_with_hint(
				hint, static_cast<node_type&>(np));
    }

public:
    org_table table_;
};

template<typename T, typename H, typename P, typename A>
struct node_handle_traits_impl_custom< boost::unordered_set<T, H, P, A> >
{
	typedef typename 
		detail::node_handle_foo_hacking_unordered_set
		<
			boost::unordered_set<T, H, P, A>
		>::node_type node_type;

	typedef typename 
		detail::node_handle_foo_hacking_unordered_set
		<
			boost::unordered_set<T, H, P, A>
		>::insert_return_type insert_return_type;

	typedef typename
		boost::unordered_set<T, H, P, A>::iterator hint_insert_return_type;
};

// uset_node_extract
template<template<typename _T, typename _H, typename _P, typename _A> class USet,
			typename T, typename H, typename P, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_< native_extractable<USet<T, H, P, A> > >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename node_handle_foo_hacking_unordered_set<USet<T, H, P, A> >::node_type
	>::type
	uset_node_extract(USet<T, H, P, A>& s, BOOST_RV_REF(Key) key)
{
	typedef Key key_type;
	typedef node_handle_foo_hacking_unordered_set<USet<T, H, P, A> > hacking_uset_type;
	const key_type& arg_key = key;
	return (reinterpret_cast<hacking_uset_type&>(s)).extract(arg_key);
}

template<template<typename _T, typename _H, typename _P, typename _A> class USet,
			typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename 
	boost::disable_if
	<
		native_extractable<USet<T, H, P, A> >,
		typename node_handle_foo_hacking_unordered_set<USet<T, H, P, A> >::node_type
	>::type
	uset_node_extract(USet<T, H, P, A>& s, const KeyOrIter& key_or_iter)
{
	typedef node_handle_foo_hacking_unordered_set<USet<T, H, P, A> > hacking_uset_type;
	return (reinterpret_cast<hacking_uset_type&>(s)).extract(key_or_iter);
}

// uset_insert
template<template<typename _T, typename _H, typename _P, typename _A> class USet,
			typename T, typename H, typename P, typename A> inline
typename 
	boost::disable_if
	<
		native_node_insertable<USet<T, H, P, A> >,
		typename 
			node_handle_foo_hacking_unordered_set
			<
				USet<T, H, P, A> 
			>::insert_return_type
	>::type
	uset_node_insert(USet<T, H, P, A>& s, 
						BOOST_RV_REF_BEG
							typename 
								node_handle_foo_hacking_unordered_set
								< 
									USet<T, H, P, A> 
								>::node_type
						BOOST_RV_REF_END node)
{
	typedef 
		node_handle_foo_hacking_unordered_set
		<
			USet<T, H, P, A> 
		> hacking_uset_type;
	return (reinterpret_cast<hacking_uset_type&>(s)).insert(boost::move(node));
}

template<template<typename _T, typename _H, typename _P, typename _A> class USet,
			typename T, typename H, typename P, typename A> inline
typename 
	boost::disable_if
	<
		native_node_insertable<USet<T, H, P, A> >,
		typename USet<T, H, P, A>::iterator
	>::type
	uset_node_insert(USet<T, H, P, A>& s, 
						typename USet<T, H, P, A>::const_iterator hint,
						BOOST_RV_REF_BEG
							typename 
								node_handle_foo_hacking_unordered_set
								< 
									USet<T, H, P, A> 
								>::node_type
						BOOST_RV_REF_END node)
{
	typedef 
		node_handle_foo_hacking_unordered_set
		<
			USet<T, H, P, A> 
		> hacking_uset_type;

	return (reinterpret_cast<hacking_uset_type&>(s)).insert(hint, boost::move(node));
}

//node_handle_foo_hacking_unordered_multiset

template <template<typename _T, typename _H, typename _P, typename _A> class UMultiSet,
			typename T, typename H, typename P, typename A>
class node_handle_foo_hacking_unordered_multiset<UMultiSet<T, H, P, A> >
{
private:
	typedef node_handle_foo_hacking_unordered_multiset this_type;

public:
	typedef UMultiSet<T, H, P, A> org_umultiset_type;
	typedef node_handle_foo_hacking_unordered_multiset hacking_umultiset_type;

public:
	typedef typename org_umultiset_type::key_type key_type;
	typedef typename org_umultiset_type::value_type value_type;

	typedef typename org_umultiset_type::hasher hasher;
	typedef typename org_umultiset_type::key_equal key_equal;
	typedef typename org_umultiset_type::allocator_type allocator_type;

	typedef typename org_umultiset_type::const_iterator const_iterator;
	typedef typename org_umultiset_type::iterator iterator;

public:
	typedef boost::unordered::detail::multiset<A, T, H, P> types;
	typedef typename types::table org_table;
	typedef typename types::node node;

	typedef org_table org_table_impl_type;
	typedef node_handle_foo_hacking_grouped_table_impl<org_table_impl_type> hacking_table_impl_type;

	typedef typename hacking_table_impl_type::node_pointer node_pointer;

public:
	typedef node_handle_uset<node, value_type, allocator_type> node_type;

public:
	YGGR_CONSTEXPR_OR_INLINE 
	hacking_table_impl_type& hacking_table_impl_cast(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<hacking_table_impl_type&>(table_);
	}

	YGGR_CONSTEXPR_OR_INLINE 
	const hacking_table_impl_type& hacking_table_impl_cast(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return reinterpret_cast<const hacking_table_impl_type&>(table_);
	}

public:
	// extract
    inline node_type extract(const_iterator position)
    {
        return 
			node_type(
				hacking_table_impl_cast().extract_by_iterator(position), 
				table_.node_alloc());
    }

    inline node_type extract(const key_type& k)
    {
        return 
			node_type(
				hacking_table_impl_cast().extract_by_key(k), 
				table_.node_alloc());
    }

	//insert
	inline iterator insert(BOOST_RV_REF(node_type) np)
	{
		return hacking_table_impl_cast().move_insert_node_type(static_cast<node_type&>(np));
	}

    inline iterator insert(const_iterator hint, BOOST_RV_REF(node_type) np)
    {
        return 
			hacking_table_impl_cast().move_insert_node_type_with_hint(
				hint, static_cast<node_type&>(np));
    }


public:
	org_table table_;
};

template<typename T, typename H, typename P, typename A>
struct node_handle_traits_impl_custom< boost::unordered_multiset<T, H, P, A> >
{
	typedef typename 
		detail::node_handle_foo_hacking_unordered_multiset
		<
			boost::unordered_multiset<T, H, P, A>
		>::node_type node_type;

	typedef typename 
		boost::unordered_multiset<T, H, P, A>::iterator insert_return_type;

	typedef insert_return_type hint_insert_return_type;
};


// umultiset_extract
template<template<typename _T, typename _H, typename _P, typename _A> class UMultiSet,
			typename T, typename H, typename P, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_< native_extractable<UMultiSet<T, H, P, A> > >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename node_handle_foo_hacking_unordered_multiset<UMultiSet<T, H, P, A> >::node_type
	>::type
	umultiset_node_extract(UMultiSet<T, H, P, A>& s, BOOST_RV_REF(Key) key)
{
	typedef Key key_type;
	typedef node_handle_foo_hacking_unordered_multiset<UMultiSet<T, H, P, A> > hacking_umultiset_type;
	const key_type& arg_key = key;
	return (reinterpret_cast<hacking_umultiset_type&>(s)).extract(arg_key);
}

template<template<typename _T, typename _H, typename _P, typename _A> class UMultiSet,
			typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename 
	boost::disable_if
	<
		native_extractable<UMultiSet<T, H, P, A> >,
		typename node_handle_foo_hacking_unordered_multiset<UMultiSet<T, H, P, A> >::node_type
	>::type
	umultiset_node_extract(UMultiSet<T, H, P, A>& s, const KeyOrIter& key_or_iter)
{
	typedef node_handle_foo_hacking_unordered_multiset<UMultiSet<T, H, P, A> > hacking_umultiset_type;
	return (reinterpret_cast<hacking_umultiset_type&>(s)).extract(key_or_iter);
}

// umultiset_insert
template<template<typename _T, typename _H, typename _P, typename _A> class UMultiSet,
			typename T, typename H, typename P, typename A> inline
typename 
	boost::disable_if
	<
		native_node_insertable<UMultiSet<T, H, P, A> >,
		typename UMultiSet<T, H, P, A>::iterator
	>::type
	umultiset_node_insert(UMultiSet<T, H, P, A>& s, 
							BOOST_RV_REF_BEG
								typename 
									node_handle_foo_hacking_unordered_multiset
									< 
										UMultiSet<T, H, P, A> 
									>::node_type
							BOOST_RV_REF_END node)
{
	typedef 
		node_handle_foo_hacking_unordered_multiset
		<
			UMultiSet<T, H, P, A> 
		> hacking_umultiset_type;
	return (reinterpret_cast<hacking_umultiset_type&>(s)).insert(boost::move(node));
}

template<template<typename _T, typename _H, typename _P, typename _A> class UMultiSet,
			typename T, typename H, typename P, typename A> inline
typename 
	boost::disable_if
	<
		native_node_insertable<UMultiSet<T, H, P, A> >,
		typename UMultiSet<T, H, P, A>::iterator
	>::type
	umultiset_node_insert(UMultiSet<T, H, P, A>& s, 
							typename UMultiSet<T, H, P, A>::const_iterator hint,
							BOOST_RV_REF_BEG
								typename 
									node_handle_foo_hacking_unordered_multiset
									< 
										UMultiSet<T, H, P, A> 
									>::node_type
							BOOST_RV_REF_END node)
{
	typedef 
		node_handle_foo_hacking_unordered_multiset
		<
			UMultiSet<T, H, P, A> 
		> hacking_umultiset_type;

	return (reinterpret_cast<hacking_umultiset_type&>(s)).insert(hint, boost::move(node));
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_105300_NODE_HANDLE_FOO_HACKING_USET_HPP__
