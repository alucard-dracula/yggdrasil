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

#ifndef __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_USET_HPP__
#define __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_USET_HPP__

#include <yggr/container/detail/boost_container_hacking_path.hpp>

#include YGGR_PP_BOOST_CONTAINER_HACKING_LATEST_VERSION_PATH(node_handle_foo_hacking_uset)

namespace yggr
{
namespace container
{
namespace detail
{

// node_handle_foo_hacking_unordered_set

// uset_extract
template<template<typename _T, typename _H, typename _P, typename _A> class USet,
			typename T, typename H, typename P, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			native_extractable<USet<T, H, P, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename USet<T, H, P, A>::node_type
	>::type
	uset_node_extract(USet<T, H, P, A>& s, BOOST_RV_REF(Key) key)
{
	return s.extract(boost::move(key));
}

template<template<typename _T, typename _H, typename _P, typename _A> class USet,
			typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename 
	boost::enable_if
	<
		native_extractable<USet<T, H, P, A> >,
		typename USet<T, H, P, A>::node_type
	>::type
	uset_node_extract(USet<T, H, P, A>& s, const KeyOrIter& key_or_iter)
{
	return s.extract(key_or_iter);
}

// uset_insert
template<template<typename _T, typename _H, typename _P, typename _A> class USet,
			typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		native_node_insertable<USet<T, H, P, A> >,
		typename USet<T, H, P, A>::insert_return_type
	>::type
	uset_node_insert(USet<T, H, P, A>& s, 
						BOOST_RV_REF_BEG
							typename USet<T, H, P, A>::node_type
						BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<template<typename _T, typename _H, typename _P, typename _A> class USet,
			typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		native_node_insertable<USet<T, H, P, A> >,
		typename USet<T, H, P, A>::iterator
	>::type
	uset_node_insert(USet<T, H, P, A>& s, 
						typename USet<T, H, P, A>::const_iterator hint,
						BOOST_RV_REF_BEG
							typename USet<T, H, P, A>::node_type
						BOOST_RV_REF_END node)
{
	return s.insert(hint, boost::move(node));
}

//node_handle_foo_hacking_unordered_multiset

// umultiset_extract
template<template<typename _T, typename _H, typename _P, typename _A> class UMultiSet,
			typename T, typename H, typename P, typename A, typename Key> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			native_extractable<UMultiSet<T, H, P, A> >,
			boost::mpl::not_< iterator_ex::is_iterator<Key> >
		>,
		typename UMultiSet<T, H, P, A>::node_type
	>::type
	umultiset_node_extract(UMultiSet<T, H, P, A>& s, BOOST_RV_REF(Key) key)
{
	return s.extract(boost::move(key));
}

template<template<typename _T, typename _H, typename _P, typename _A> class UMultiSet,
			typename T, typename H, typename P, typename A, typename KeyOrIter> inline
typename 
	boost::enable_if
	<
		native_extractable<UMultiSet<T, H, P, A> >,
		typename UMultiSet<T, H, P, A>::node_type
	>::type
	umultiset_node_extract(UMultiSet<T, H, P, A>& s, const KeyOrIter& key_or_iter)
{
	return s.extract(key_or_iter);
}

// umultiset_insert
template<template<typename _T, typename _H, typename _P, typename _A> class UMultiSet,
			typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		native_node_insertable<UMultiSet<T, H, P, A> >,
		typename UMultiSet<T, H, P, A>::iterator
	>::type
	umultiset_node_insert(UMultiSet<T, H, P, A>& s, 
							BOOST_RV_REF_BEG
								typename UMultiSet<T, H, P, A>::node_type
							BOOST_RV_REF_END node)
{
	return s.insert(boost::move(node));
}

template<template<typename _T, typename _H, typename _P, typename _A> class UMultiSet,
			typename T, typename H, typename P, typename A> inline
typename 
	boost::enable_if
	<
		native_node_insertable<UMultiSet<T, H, P, A> >,
		typename UMultiSet<T, H, P, A>::iterator
	>::type
	umultiset_node_insert(UMultiSet<T, H, P, A>& s, 
							typename UMultiSet<T, H, P, A>::const_iterator hint,
							BOOST_RV_REF_BEG
								typename UMultiSet<T, H, P, A>::node_type
							BOOST_RV_REF_END node)
{
	return s.insert(hint, boost::move(node));
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_NODE_HANDLE_FOO_HACKING_USET_HPP__
