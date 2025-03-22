// node_handle_foo_hacking_set_foo_impl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_FOO_IMPL_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_FOO_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#if (BOOST_VERSION < 105700)

#include <yggr/iterator_ex/is_iterator.hpp>

#include <yggr/container/detail/native_extractable.hpp>
#include <yggr/container/detail/native_node_insertable.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/typeof/typeof.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

// set
// extract

template<
	template<typename _K, typename _C, typename _A> class Set,
	typename K, typename C, typename A, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			native_extractable< Set<K, C, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename node_handle_foo_hacking_set<Set<K, C, A> >::node_type
	>::type
	set_node_extract(Set<K, C, A>& s, BOOST_RV_REF(Key) key)
{
	typedef Key key_type;
	typedef node_handle_foo_hacking_set< Set<K, C, A> > hacked_set_type;
	const key_type& key_arg = key;
	return reinterpret_cast<hacked_set_type&>(s).extract(key_arg);
}

template<
	template<typename _K, typename _C, typename _A> class Set,
	typename K, typename C, typename A, typename KeyOrIter> inline
typename
	boost::disable_if
	<
		native_extractable< Set<K, C, A> >,
		typename node_handle_foo_hacking_set<Set<K, C, A> >::node_type
	>::type
	set_node_extract(Set<K, C, A>& s, const KeyOrIter& key_or_iter)
{
	typedef node_handle_foo_hacking_set< Set<K, C, A> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).extract(key_or_iter);
}

// insert
template<
	template<typename _K, typename _C, typename _A> class Set,
	typename K, typename C, typename A> inline
typename
	boost::disable_if
	<
		native_node_insertable< Set<K, C, A> >,
		typename node_handle_foo_hacking_set<Set<K, C, A> >::insert_return_type
	>::type
	set_node_insert(Set<K, C, A>& s, 
					BOOST_RV_REF_BEG 
						typename node_handle_foo_hacking_set<Set<K, C, A> >::node_type
					BOOST_RV_REF_END node)
{
	typedef node_handle_foo_hacking_set< Set<K, C, A> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).insert(boost::move(node));
}

template<
	template<typename _K, typename _C, typename _A> class Set,
	typename K, typename C, typename A> inline
typename
	boost::disable_if
	<
		native_node_insertable< Set<K, C, A> >,
		typename Set<K, C, A>::iterator
	>::type
	set_node_insert(Set<K, C, A>& s, 
					typename Set<K, C, A>::const_iterator hint,
					BOOST_RV_REF_BEG 
						typename node_handle_foo_hacking_set<Set<K, C, A> >::node_type
					BOOST_RV_REF_END node)
{
	typedef node_handle_foo_hacking_set< Set<K, C, A> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).insert(hint, boost::move(node));
}

// multiset
// extract
template<
	template<typename _K, typename _C, typename _A> class MultiSet,
	typename K, typename C, typename A, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_< native_extractable< MultiSet<K, C, A> > >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename node_handle_foo_hacking_multiset<MultiSet<K, C, A> >::node_type
	>::type
	multiset_node_extract(MultiSet<K, C, A>& s, BOOST_RV_REF(Key) key)
{
	typedef Key key_type;
	typedef node_handle_foo_hacking_multiset< MultiSet<K, C, A> > hacked_set_type;
	const key_type& key_arg = key;
	return reinterpret_cast<hacked_set_type&>(s).extract(key_arg);
}

template<
	template<typename _K, typename _C, typename _A> class MultiSet,
	typename K, typename C, typename A, typename KeyOrIter> inline
typename
	boost::disable_if
	<
		native_extractable< MultiSet<K, C, A> >,
		typename node_handle_foo_hacking_multiset<MultiSet<K, C, A> >::node_type
	>::type
	multiset_node_extract(MultiSet<K, C, A>& s, const KeyOrIter& key_or_iter)
{
	typedef node_handle_foo_hacking_multiset< MultiSet<K, C, A> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).extract(key_or_iter);
}

// insert
template<
	template<typename _K, typename _C, typename _A> class MultiSet,
	typename K, typename C, typename A> inline
typename
	boost::disable_if
	<
		native_node_insertable< MultiSet<K, C, A> >,
		typename MultiSet<K, C, A>::iterator
	>::type
	multiset_node_insert(MultiSet<K, C, A>& s, 
							BOOST_RV_REF_BEG 
								typename node_handle_foo_hacking_multiset<MultiSet<K, C, A> >::node_type
							BOOST_RV_REF_END node)
{
	typedef node_handle_foo_hacking_multiset< MultiSet<K, C, A> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).insert(boost::move(node));
}

template<
	template<typename _K, typename _C, typename _A> class MultiSet,
	typename K, typename C, typename A> inline
typename
	boost::disable_if
	<
		native_node_insertable< MultiSet<K, C, A> >,
		typename MultiSet<K, C, A>::iterator
	>::type
	multiset_node_insert(MultiSet<K, C, A>& s, 
							typename MultiSet<K, C, A>::const_iterator hint,
							BOOST_RV_REF_BEG 
								typename node_handle_foo_hacking_multiset<MultiSet<K, C, A> >::node_type
							BOOST_RV_REF_END node)
{
	typedef node_handle_foo_hacking_multiset< MultiSet<K, C, A> > hacked_set_type;
	return reinterpret_cast<hacked_set_type&>(s).insert(hint, boost::move(node));
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // (BOOST_VERSION < 105700)

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_FOO_IMPL_HPP__
