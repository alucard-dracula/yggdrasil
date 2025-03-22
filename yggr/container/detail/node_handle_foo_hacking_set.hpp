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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_HPP__

#include <yggr/container/detail/boost_container_hacking_path.hpp>

#include YGGR_PP_BOOST_CONTAINER_HACKING_LATEST_VERSION_PATH(node_handle_foo_hacking_set)

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
		typename Set<K, C, A>::node_type
	>::type
	set_node_extract(Set<K, C, A>& s, BOOST_RV_REF(Key) key)
{
	return s.extract(boost::move(key));
}

template<
	template<typename _K, typename _C, typename _A> class Set,
	typename K, typename C, typename A, typename KeyOrIter> inline
typename
	boost::enable_if
	<
		native_extractable< Set<K, C, A> >,
		typename Set<K, C, A>::node_type
	>::type
	set_node_extract(Set<K, C, A>& s, const KeyOrIter& key_or_iter)
{
	return s.extract(key_or_iter);
}

template<
	template<typename _K, typename _C, typename _A> class Set,
	typename K, typename C, typename A> inline
typename
	boost::enable_if
	<
		native_node_insertable< Set<K, C, A> >,
		typename Set<K, C, A>::insert_return_type
	>::type
	set_node_insert(Set<K, C, A>& s, 
					BOOST_RV_REF_BEG 
						typename Set<K, C, A>::node_type 
					BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<
	template<typename _K, typename _C, typename _A> class Set,
	typename K, typename C, typename A> inline
typename
	boost::enable_if
	<
		native_node_insertable< Set<K, C, A> >,
		typename Set<K, C, A>::iterator
	>::type
	set_node_insert(Set<K, C, A>& s, 
					typename Set<K, C, A>::const_iterator hint,
					BOOST_RV_REF_BEG 
						typename Set<K, C, A>::node_type 
					BOOST_RV_REF_END node)
{
	return get_hint_insert_return(s.insert(hint, boost::move(node)));
}

template<
	template<typename _K, typename _C, typename _A> class MultiSet,
	typename K, typename C, typename A, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			native_extractable< MultiSet<K, C, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename MultiSet<K, C, A>::node_type
	>::type
	multiset_node_extract(MultiSet<K, C, A>& s, BOOST_RV_REF(Key) key)
{
	return s.extract(boost::move(key));
}

template<
	template<typename _K, typename _C, typename _A> class MultiSet,
	typename K, typename C, typename A, typename KeyOrIter> inline
typename
	boost::enable_if
	<
		native_extractable< MultiSet<K, C, A> >,
		typename MultiSet<K, C, A>::node_type
	>::type
	multiset_node_extract(MultiSet<K, C, A>& s, const KeyOrIter& key_or_iter)
{
	return s.extract(key_or_iter);
}

template<
	template<typename _K, typename _C, typename _A> class MultiSet,
	typename K, typename C, typename A> inline
typename
	boost::enable_if
	<
		native_node_insertable< MultiSet<K, C, A> >,
		typename MultiSet<K, C, A>::iterator
	>::type
	multiset_node_insert(MultiSet<K, C, A>& s, 
							BOOST_RV_REF_BEG 
								typename MultiSet<K, C, A>::node_type 
							BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<
	template<typename _K, typename _C, typename _A> class MultiSet,
	typename K, typename C, typename A> inline
typename
	boost::enable_if
	<
		native_node_insertable< MultiSet<K, C, A> >,
		typename MultiSet<K, C, A>::iterator
	>::type
	multiset_node_insert(MultiSet<K, C, A>& s, 
							typename MultiSet<K, C, A>::const_iterator hint,
							BOOST_RV_REF_BEG 
								typename MultiSet<K, C, A>::node_type 
							BOOST_RV_REF_END node)
{
	return s.insert(hint, boost::move(node));
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_SET_HPP__
