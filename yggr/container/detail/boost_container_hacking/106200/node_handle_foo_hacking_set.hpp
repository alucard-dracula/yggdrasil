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

#ifndef __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_106200_NODE_HANDLE_FOO_HACKING_SET_HPP__
#define __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_106200_NODE_HANDLE_FOO_HACKING_SET_HPP__

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_HPP__
#	error "this file is inner file, can't include it, please include <yggr/container/set.hpp>"
#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_HPP__

#include <yggr/iterator_ex/is_iterator.hpp>

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

// set
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

// multiset
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

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_BOOST_CONTAINER_HACKING_106200_NODE_HANDLE_FOO_HACKING_SET_HPP__
