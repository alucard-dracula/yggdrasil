// node_handle_foo_hacking_map.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MAP_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MAP_HPP__

#include <yggr/container/detail/boost_container_hacking_path.hpp>

#include YGGR_PP_BOOST_CONTAINER_HACKING_LATEST_VERSION_PATH(node_handle_foo_hacking_map)

namespace yggr
{
namespace container
{
namespace detail
{

// map
// extract
template<
	template<typename _K, typename _T, typename _C, typename _A> class Map,
	typename K, typename T, typename C, typename A, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			native_extractable< Map<K, T, C, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename Map<K, T, C, A>::node_type
	>::type
	map_node_extract(Map<K, T, C, A>& s, BOOST_RV_REF(Key) key)
{
	return s.extract(boost::move(key));
}

template<
	template<typename _K, typename _T, typename _C, typename _A> class Map,
	typename K, typename T, typename C, typename A, typename KeyOrIter> inline
typename
	boost::enable_if
	<
		native_extractable< Map<K, T, C, A> >,
		typename Map<K, T, C, A>::node_type
	>::type
	map_node_extract(Map<K, T, C, A>& s, const KeyOrIter& key_or_iter)
{
	return s.extract(key_or_iter);
}

// insert

template<
	template<typename _K, typename _T, typename _C, typename _A> class Map,
	typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		native_node_insertable< Map<K, T, C, A> >,
		typename Map<K, T, C, A>::insert_return_type
	>::type
	map_node_insert(Map<K, T, C, A>& s, 
					BOOST_RV_REF_BEG 
						typename Map<K, T, C, A>::node_type 
					BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<
	template<typename _K, typename _T, typename _C, typename _A> class Map,
	typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		native_node_insertable< Map<K, T, C, A> >,
		typename Map<K, T, C, A>::iterator
	>::type
	map_node_insert(Map<K, T, C, A>& s, 
					typename Map<K, T, C, A>::const_iterator hint,
					BOOST_RV_REF_BEG 
						typename Map<K, T, C, A>::node_type 
					BOOST_RV_REF_END node)
{
	return get_hint_insert_return(s.insert(hint, boost::move(node)));
}

// multimap
// extract
template<
	template<typename _K, typename _T, typename _C, typename _A> class MultiMap,
	typename K, typename T, typename C, typename A, typename Key> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			native_extractable< MultiMap<K, T, C, A> >,
			boost::mpl::and_< iterator_ex::is_iterator<Key> >
		>,
		typename MultiMap<K, T, C, A>::node_type
	>::type
	multimap_node_extract(MultiMap<K, T, C, A>& s, BOOST_RV_REF(Key) key)
{
	return s.extract(boost::move(key));
}

template<
	template<typename _K, typename _T, typename _C, typename _A> class MultiMap,
	typename K, typename T, typename C, typename A, typename KeyOrIter> inline
typename
	boost::enable_if
	<
		native_extractable< MultiMap<K, T, C, A> >,
		typename MultiMap<K, T, C, A>::node_type
	>::type
	multimap_node_extract(MultiMap<K, T, C, A>& s, const KeyOrIter& key_or_iter)
{
	return s.extract(key_or_iter);
}

// insert

template<
	template<typename _K, typename _T, typename _C, typename _A> class MultiMap,
	typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		native_node_insertable< MultiMap<K, T, C, A> >,
		typename MultiMap<K, T, C, A>::iterator
	>::type
	multimap_node_insert(MultiMap<K, T, C, A>& s, 
							BOOST_RV_REF_BEG 
								typename MultiMap<K, T, C, A>::node_type 
							BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<
	template<typename _K, typename _T, typename _C, typename _A> class MultiMap,
	typename K, typename T, typename C, typename A> inline
typename
	boost::enable_if
	<
		native_node_insertable< MultiMap<K, T, C, A> >,
		typename MultiMap<K, T, C, A>::iterator
	>::type
	multimap_node_insert(MultiMap<K, T, C, A>& s, 
							typename MultiMap<K, T, C, A>::const_iterator hint,
							BOOST_RV_REF_BEG 
								typename MultiMap<K, T, C, A>::node_type 
							BOOST_RV_REF_END node)
{
	return s.insert(hint, boost::move(node));
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_MAP_HPP__
