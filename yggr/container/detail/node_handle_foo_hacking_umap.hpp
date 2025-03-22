// node_handle_foo_hacking_umap.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_UMAP_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_UMAP_HPP__

#include <yggr/container/detail/boost_container_hacking_path.hpp>

#include YGGR_PP_BOOST_CONTAINER_HACKING_LATEST_VERSION_PATH(node_handle_foo_hacking_umap)

namespace yggr
{
namespace container
{
namespace detail
{

// node_handle_foo_hacking_unordered_map

// umap_extract
template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMap,
			typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			native_extractable<UMap<K, T, H, P, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename UMap<K, T, H, P, A>::node_type
	>::type
	umap_node_extract(UMap<K, T, H, P, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMap,
			typename K, typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename 
	boost::enable_if
	<
		native_extractable<UMap<K, T, H, P, A> >,
		typename UMap<K, T, H, P, A>::node_type
	>::type
	umap_node_extract(UMap<K, T, H, P, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// umap_insert
template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMap,
			typename K, typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		native_node_insertable<UMap<K, T, H, P, A> >,
		typename UMap<K, T, H, P, A>::insert_return_type
	>::type
	umap_node_insert(UMap<K, T, H, P, A>& c, 
						BOOST_RV_REF_BEG
							typename UMap<K, T, H, P, A>::node_type
						BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMap,
			typename K, typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		native_node_insertable<UMap<K, T, H, P, A> >,
		typename UMap<K, T, H, P, A>::iterator
	>::type
	umap_node_insert(UMap<K, T, H, P, A>& c, 
						typename UMap<K, T, H, P, A>::const_iterator hint,
						BOOST_RV_REF_BEG
							typename UMap<K, T, H, P, A>::node_type
						BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

// umultimap_extract
template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMultiMap,
			typename K, typename T, typename H, typename P, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			native_extractable< UMultiMap<K, T, H, P, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename UMultiMap<K, T, H, P, A>::node_type
	>::type
	umultimap_node_extract(UMultiMap<K, T, H, P, A>& c, BOOST_RV_REF(Key) key)
{
	return c.extract(boost::move(key));
}

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMultiMap,
			typename K, typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename 
	boost::enable_if
	<
		native_extractable<UMultiMap<K, T, H, P, A> >,
		typename UMultiMap<K, T, H, P, A>::node_type
	>::type
	umultimap_node_extract(UMultiMap<K, T, H, P, A>& c, const KeyOrIter& key_or_iter)
{
	return c.extract(key_or_iter);
}

// umultimap_insert
template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMultiMap,
			typename K, typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		native_node_insertable<UMultiMap<K, T, H, P, A> >,
		typename UMultiMap<K, T, H, P, A>::iterator
	>::type
	umultimap_node_insert(UMultiMap<K, T, H, P, A>& c, 
							BOOST_RV_REF_BEG
								typename UMultiMap<K, T, H, P, A>::node_type
							BOOST_RV_REF_END node)
{
	return c.insert(boost::move(node));
}

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMultiMap,
			typename K, typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		native_node_insertable<UMultiMap<K, T, H, P, A> >,
		typename UMultiMap<K, T, H, P, A>::iterator
	>::type
	umultimap_node_insert(UMultiMap<K, T, H, P, A>& c, 
							typename UMultiMap<K, T, H, P, A>::const_iterator hint,
							BOOST_RV_REF_BEG
								typename UMultiMap<K, T, H, P, A>::node_type
							BOOST_RV_REF_END node)
{
	return c.insert(hint, boost::move(node));
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_UMAP_HPP__
