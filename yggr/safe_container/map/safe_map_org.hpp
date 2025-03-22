//safe_map_org.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MAP_SAFE_MAP_ORG_HPP__
#define __YGGR_SAFE_CONTAINER_MAP_SAFE_MAP_ORG_HPP__

#ifndef __YGGR_SAFE_CONTAINER_SAFE_MAP_HPP__
#	error "please include<yggr/safe_container/safe_map.hpp>"
#endif // __YGGR_SAFE_CONTAINER_SAFE_MAP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/mutex_def_helper.hpp>

#include <yggr/move/move.hpp>
#include <yggr/ppex/friend.hpp>

#include <yggr/container/map.hpp>
#include <yggr/safe_container/detail/swap_def.hpp>
#include <yggr/safe_container/detail/comparer_def.hpp>
#include <yggr/safe_container/detail/safe_insert_return_type_base.hpp>

#include <yggr/safe_container/detail/safe_element_reference.hpp>
#include <yggr/safe_container/detail/begin_rbegin_value_impl.hpp>
#include <yggr/safe_container/detail/operator_set_impl.hpp>
#include <yggr/safe_container/detail/swap_impl.hpp>
#include <yggr/safe_container/detail/safe_other_impl.hpp>
#include <yggr/safe_container/detail/erase_impl.hpp>
#include <yggr/safe_container/detail/insert_impl.hpp>
#include <yggr/safe_container/detail/emplace_impl.hpp>
#include <yggr/safe_container/detail/extract_node_impl.hpp>
#include <yggr/safe_container/detail/using_handler_impl.hpp>
#include <yggr/safe_container/detail/for_each_impl.hpp>
#include <yggr/safe_container/detail/bound_impl.hpp>
#include <yggr/safe_container/detail/equal_range_impl.hpp>
#include <yggr/safe_container/detail/find_impl.hpp>
#include <yggr/safe_container/detail/contains_impl.hpp>
#include <yggr/safe_container/detail/safe_gernel_impl.hpp>
#include <yggr/safe_container/detail/range_size_def.hpp>
#include <yggr/safe_container/safe_container_error.hpp>
#include <yggr/safe_container/map/safe_map_member_impl.hpp>

#include <yggr/safe_container/safe_map_fwd.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/range/functions.hpp>
#include <boost/function.hpp>

#include <iterator>
#include <algorithm>
#include <utility>

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! All safe containers can not guarantee the creation and destruction of multi-threaded safe,		!!!
!!! in different threads to create and destroy the safe container, please use swap to deal with,	!!!
!!! and																								!!!
!!!  it is forbidden to pass any "iterator reference pointer" inside the safe_container				!!!
!!!	 through the callback function to the outside of the safe_container and save or use it!			!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

namespace yggr
{
namespace safe_container
{

template<typename Key, typename Val,
			typename Mutex = void,
			typename Cmp = typename mplex::typename_expand_get<container::map<Key, Val>, 2>::type,
			typename Alloc = typename mplex::typename_expand_get<container::map<Key, Val>, 3>::type,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class Map = container::map
		>
class safe_map
{
public:
	typedef Alloc tpl_arg_allocator_type;

public:
	typedef Map<Key, Val, Cmp, Alloc> base_type;

	typedef typename base_type::allocator_type allocator_type;

	typedef typename base_type::key_type key_type;
	typedef typename base_type::mapped_type mapped_type;

	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference;

	typedef typename base_type::key_compare key_compare;
	typedef typename base_type::value_compare value_compare;

	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;

	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	typedef safe_container_error::error_type error_type;

	typedef typename
		container::node_handle_traits
		<
			base_type
		>::node_type node_type;

	typedef typename
		container::node_handle_traits
		<
			base_type
		>::insert_return_type base_insert_return_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_base_of<mplex::null_type, base_insert_return_type>,
			mplex::null_type,
			detail::safe_insert_return_type_base<node_type>
		>::type insert_return_type;

private:
	typedef safe_container_error::error_maker_type error_maker_type;

private:
	typedef thread::mutex_def_helper<Mutex> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

private:
	typedef safe_map this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	//safe_map(void);
	//explicit safe_map(const key_compare& cmp);
	//explicit safe_map(const allocator_type& alloc);
	//safe_map(const key_compare& cmp, const allocator_type& alloc);

	//template<class InputIterator>
	//safe_map(InputIterator first, InputIterator last);

	//template<class InputIterator>
	//safe_map(InputIterator first, InputIterator last, const key_compare& cmp);

	//template<class InputIterator>
	//safe_map(InputIterator first, InputIterator last, const allocator_type& alloc);

	//template<class InputIterator>
	//safe_map(InputIterator first, InputIterator last, const key_compare& cmp, const allocator_type& alloc);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//safe_map(std::initializer_list<value_type> l);

	//safe_map(std::initializer_list<value_type> l, const allocator_type& alloc);

	//safe_map(std::initializer_list<value_type> l,
	//			const key_compare& key_comp,
	//			const allocator_type& alloc);

//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//explicit safe_map(BOOST_RV_REF(base_type) right);
	//explicit safe_map(const base_type& right);

	//safe_map(BOOST_RV_REF(base_type) right, const allocator_type& alloc);
	//safe_map(const base_type& right, const allocator_type& alloc);

	//safe_map(BOOST_RV_REF(this_type) right);
	//safe_map(const this_type& right);

	//safe_map(BOOST_RV_REF(this_type) right, const allocator_type& alloc);
	//safe_map(const this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_MAP_CONSTRUCTOR_IMPL()

	//safe_map(BOOST_RV_REF(isomeric_same_base_type) right);
	//safe_map(const isomeric_same_base_type& right);

	//safe_map(BOOST_RV_REF(isomeric_same_base_type) right, const allocator_type& alloc);
	//safe_map(const isomeric_same_base_type& right, const allocator_type& alloc);

	//safe_map(BOOST_RV_REF(isomeric_same_this_type) right);
	//safe_map(const isomeric_same_this_type& right);

	//safe_map(BOOST_RV_REF(isomeric_same_this_type) right, const allocator_type& alloc);
	//safe_map(const isomeric_same_this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_MAP_ISOMERIC_SAME_CONSTRUCTOR_IMPL(Map)

	//~safe_map(void);
	YGGR_PP_SAFE_MAP_DESTRUCTOR_IMPL()

public:
	//inline this_type& operator=(BOOST_RV_REF(base_type) right);
	//this_type& operator=(const base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(this_type) right);
	//this_type& operator=(const this_type& right);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//this_type& operator=(std::initializer_list<value_type> il);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_KV_OPERATOR_SET_IMPL()

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_base_type) right);
	//this_type& operator=(const isomeric_same_base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_this_type) right);
	//this_type& operator=(const isomeric_same_this_type& right);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_OPERATOR_SET_IMPL(
		5, Map, safe_map, container::is_isomeric_same_map)

public:
	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	YGGR_PP_SAFE_CONTAINER_SWAP_IMPL()

	//inline void swap(BOOST_RV_REF(isomeric_same_base_type) right);
	//void swap(isomeric_same_base_type& right);

	//inline void swap(BOOST_RV_REF(isomeric_same_this_type) right);
	//void swap(isomeric_same_this_type& right);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_SWAP_IMPL(
		5, Map, safe_map, container::is_isomeric_same_map)

public:
	//operator base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL()

	//operator isomeric_same_base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_OPERATOR_BASE_IMPL(
		4, Map, container::is_isomeric_same_map)

public:
	// capacity:
	//bool empty(void) const;
	//size_type size(void) const;
	//size_type max_size(void) const;

	YGGR_PP_SAFE_KV_CAPACITY_IMPL()

	// element access:
//private:

	//class bridge_at;
	//class bridge_index;

	//friend class bridge_at;
	//friend class bridge_index;

//public:
	//detail::safe_element_reference<bridge_index> operator[](BOOST_RV_REF(key_type) key);
	//detail::safe_element_reference<bridge_index> operator[](const key_type& key);

	//detail::safe_element_reference<bridge_at> at(const key_type& key);
	//mapped_type at(const key_type& key) const;

	//// non-try-catch get and set
	//bool get_value(const key_type& key, mapped_type& val) const;

	//bool set_value(const key_type& key, BOOST_RV_REF(mapped_type) val);
	//bool set_value(const key_type& key, const mapped_type& val);

	//bool set_value(BOOST_RV_REF(value_type) val);
	//bool set_value(const value_type& val);

	//bool get_at(const key_type& key, mapped_type& val) const;

	//bool set_at(const key_type& key, BOOST_RV_REF(mapped_type) val);
	//bool set_at(const key_type& key, const mapped_type& val);

	//bool set_at(BOOST_RV_REF(value_type) val);
	//bool set_at(const value_type& val);

	YGGR_PP_SAFE_MAP_SUBSCRIPT_AND_AT_IMPL()

	// modifiers:
	// insert specially
	//bool insert(const key_type& key, BOOST_RV_REF(mapped_type) val);
	//bool insert(const key_type& key, const mapped_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	insert(const key_type& key, BOOST_RV_REF(mapped_type) val, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	insert(const key_type& key, const mapped_type& val, const Handler& handler);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler,
	//				const key_type& key, BOOST_RV_REF(mapped_type) val);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler,
	//				const key_type& key, const mapped_type& val);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//			const key_type& key, BOOST_RV_REF(mapped_type) val,
	//			const Handler& handler);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//			const key_type& key, const mapped_type& val,
	//			const Handler& handler);

	YGGR_PP_SAFE_KV_INSERT_FUNCTION_SMAP_SP_IMPL()

	//bool insert(BOOST_RV_REF(value_type) val);
	//bool insert(const value_type& val);

	//template<typename P>
	//bool insert(BOOST_RV_REF(P) val);

	//template<typename Handler>
	//typename Handler::result_type
	//	insert(BOOST_RV_REF(value_type) val, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	insert(const value_type& val, const Handler& handler);

	//template<typename Handler, typename P>
	//typename Handler::result_type
	//	insert(BOOST_RV_REF(P) val, const Handler& handler);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler,
	//				BOOST_RV_REF(value_type) val);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler,
	//				const value_type& val);

	//template<typename IterHandler, typename P>
	//bool insert(const IterHandler& iter_handler,
	//				BOOST_RV_REF(P) val);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//			BOOST_RV_REF(value_type) val,
	//			const Handler& handler);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//			const value_type& val,
	//			const Handler& handler);

	//template<typename IterHandler, typename P, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//			BOOST_RV_REF(P) val,
	//			const Handler& handler);

	//template<typename InputIter>
	//bool insert(InputIter start, InputIter last);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//bool insert(std::initializer_list<value_type> list);

//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	// insert_node
	//insert_return_type insert(BOOST_RV_REF(node_type) node);

	//template<typename RetHandler>
	//typename ::yggr::func::foo_t_info<RetHandler>::result_type
	//	insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node);

	//template<typename IterHandler, typename RetHandler>
	//typename ::yggr::func::foo_t_info<RetHandler>::result_type
	//	insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler);

	YGGR_PP_SAFE_KV_UNIQUE_INSERT_FUNCTION_IMPL()

	//template<typename K, typename M>
	//bool insert_or_assign(BOOST_RV_REF(K) k, BOOST_RV_REF(M) m);

	//template<typename K, typename M>
	//bool insert_or_assign(const K& k, BOOST_RV_REF(M) m);

	//template<typename K, typename M, typename RetHandler>
	//typename ::yggr::func::foo_t_info<RetHandler>::result_type
	//	insert_or_assign(BOOST_RV_REF(K) k, BOOST_RV_REF(M) m, const RetHandler& ret_handler);

	//template<typename K, typename M, typename RetHandler>
	//typename ::yggr::func::foo_t_info<RetHandler>::result_type
	//	insert_or_assign(const K& k, BOOST_RV_REF(M) m, const RetHandler& ret_handler);

	//template<typename IterHandler, typename K, typename M>
	//bool insert_or_assign(const IterHandler& iter_handler, BOOST_RV_REF(K) k, BOOST_RV_REF(M) m);

	//template<typename IterHandler, typename K, typename M>
	//bool insert_or_assign(const IterHandler& iter_handler, const K& k, BOOST_RV_REF(M) m);

	//template<typename IterHandler, typename K, typename M, typename RetHandler>
	//typename ::yggr::func::foo_t_info<RetHandler>::result_type
	//	insert_or_assign(const IterHandler& iter_handler, BOOST_RV_REF(K) k, BOOST_RV_REF(M) m, const RetHandler& ret_handler);

	//template<typename IterHandler, typename K, typename M, typename RetHandler>
	//typename ::yggr::func::foo_t_info<RetHandler>::result_type
	//	insert_or_assign(const IterHandler& iter_handler, const K& k, BOOST_RV_REF(M) m, const RetHandler& ret_handler);

	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL()

	//// erase(key)
	//size_type erase(const key_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	erase(const key_type& key, const Handler& handler);

	//// erase(iterator) or  erase(std::pair<iterator, iterator>)
	//template<typename IterHandler>
	//bool erase(const IterHandler&);

	//// erase(iterator, iterator)
	//// erase(iterator, iterator)
	//template<typename EHandler, typename ZHandler>
	//bool erase(const EHandler& eins_handler, const ZHandler& zwei_iter_handler);

	//// handler(erase(std::pair<iterator, iterator>))
	//template<typename EHandler, typename ZHandler>
	//typename Handler::result_type erase(const EHandler& iter_handler, const Handler& handler);

	//// handler(erase(iterator, iterator))
	//template<typename EIterHandler, typename ZIterHandler, typename Handler>
	//typename Handler::result_type erase(const EIterHandler&, const ZIterHandler&, const Handler&)

	YGGR_PP_SAFE_KV_ERASE_FUNCTION_SP_IMPL()
	YGGR_PP_SAFE_KV_ERASE_FUNCTION_IMPL()

	// erase_if
	//template<typename Pred>
	//size_type erase_if(const Pred& pred);

	YGGR_PP_SAFE_KV_ERASE_IF_MEMBER_FUNCTION_IMPL()

	void clear(void)
	{
		base_type tmp(this_type::key_comp(), this_type::get_allocator());
		{
			write_lock_type lk(_mutex);
			_base.swap(tmp);
		}
	}

	//// emplace
	//template<typename ...Args>
	//bool emplace(Args&&... args);

	//template<typename Handler, typename ...Args>
	//typename Handler::result_type
	//	emplace(const Handler& handler, Args&&... args);

	//// emplace_hint
	//template< typename IterHandler, typename ...Args>
	//bool emplace_hint (const IterHandler& iter_handler, Args&&... args);

	//template< typename IterHandler, typename Handler, typename ...Args>
	//typename Handler::result_type
	//	emplace_hint (const IterHandler& iter_handler,
	//					const Handler& handler,
	//					Args&&... args);

	//// try_emplace
	//template<typename ...Args>
	//bool try_emplace(tag_non_handler, BOOST_RV_REF(key_type) key, BOOST_FWD_REF(Args)... args);
	//
	//template<typename ...Args>
	//bool try_emplace(tag_non_handler, const key_type& key, BOOST_FWD_REF(Args)... args);
	//
	//template<typename Handler, typename ...Args>
	//typename ::yggr::func::foo_t_info<Handler>::result_type
	//	try_emplace(const Handler& handler, BOOST_RV_REF(key_type) key, BOOST_FWD_REF(Args)... args);
	//
	//template<typename Handler, typename ...Args>
	//typename ::yggr::func::foo_t_info<Handler>::result_type
	//	try_emplace(const Handler& handler, const key_type& key, BOOST_FWD_REF(Args)... args);
	//
	//template<typename IterHandler, typename ...Args>
	//bool try_emplace(const IterHandler& iter_handler, tag_non_handler,
	//					BOOST_RV_REF(key_type) key, BOOST_FWD_REF(Args)... args);
	//
	//template<typename IterHandler, typename ...Args>
	//bool try_emplace(const IterHandler& iter_handler, tag_non_handler,
	//					const key_type& key, BOOST_FWD_REF(Args)... args);
	//
	//template<typename IterHandler, typename Handler, typename ...Args>
	//typename ::yggr::func::foo_t_info<Handler>::result_type
	//	try_emplace(const IterHandler& iter_handler, const Handler& handler,
	//				BOOST_RV_REF(key_type) key, BOOST_FWD_REF(Args)... args);
	//
	//template<typename IterHandler, typename Handler, typename ...Args>
	//typename ::yggr::func::foo_t_info<Handler>::result_type
	//	try_emplace(const IterHandler& iter_handler, const Handler& handler,
	//				const key_type& key, BOOST_FWD_REF(Args)... args);

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	YGGR_PP_SAFE_KV_UNIQUE_EMPLACE_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_KV_EMPLACE_HINT_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_KV_UNIQUE_TRY_EMPLACE_FUNCTION_IMPL( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_KV_UNIQUE_EMPLACE_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_KV_EMPLACE_HINT_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_KV_UNIQUE_TRY_EMPLACE_FUNCTION_IMPL( __n__ )


#	define YGGR_PP_FOO_ARG_NAME() arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// extract_node
	//template<typename Key>
	//node_type extract(BOOST_RV_REF(Key) key);

	//template<typename Key>
	//node_type extract(const Key& key);

	//template<typename IterHandler>
	//node_type extract(const IterHandler& pos_handler);

	YGGR_PP_SAFE_CONTAINER_EXTRACT_NODE_IMPL()

	// merge
	// set multimap supported
	//template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMultiMap,
	//			typename C2, typename A2>
	//void merge(BOOST_RV_REF_BEG MapOrMultiMap<key_type, mapped_type, C2, A2> BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMultiMap,
	//			typename C2, typename A2>
	//void merge(MapOrMultiMap<key_type, mapped_type, C2, A2>& right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMultiMap,
	//			typename C2, typename A2, typename O2>
	//void merge(BOOST_RV_REF_BEG MapOrMultiMap<key_type, mapped_type, C2, A2, O2> BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMultiMap,
	//			typename C2, typename A2, typename O2>
	//void merge(MapOrMultiMap<key_type, mapped_type, C2, A2, O2>& right);

	//template<template<typename _K, typename _T, typename _C, typename _A> class UnSafeMap,
	//			typename M2, typename C2, typename A2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A> class UnSafeMap,
	//			typename M2, typename C2, typename A2>
	//void merge(safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap>& right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A> class UnsafeMultiMap,
	//			typename M2, typename C2, typename A2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiMap>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A> class UnsafeMultiMap,
	//			typename M2, typename C2, typename A2>
	//void merge(safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiMap>& right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnSafeMap,
	//			typename M2, typename C2, typename A2, typename O2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnSafeMap,
	//			typename M2, typename C2, typename A2, typename O2>
	//void merge(safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap>& right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnsafeMultiMap,
	//			typename M2, typename C2, typename A2, typename O2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiMap>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnsafeMultiMap,
	//			typename M2, typename C2, typename A2, typename O2>
	//void merge(safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiMap>& right);

	YGGR_PP_SAFE_MAP_MERGE_FUNCTION_IMPL()

	// observers:
	//key_compare key_comp(void) const;
	//value_compare value_comp(void) const;

	YGGR_PP_SAFE_KV_OBSERVERS_IMPL()

	// operations:
	// find
	// map specially
	bool find(const key_type& key, mapped_type& val) const
	{
		read_lock_type lk(_mutex);
		const_iterator iter = _base.find(key);
		if(iter == _base.end())
		{
			return false;
		}

		val = iter->second;
		return true;
	}

	//bool find(const key_type& key) const;

	//template<typename Handler>
	//typename Handler::result_type
	//	find(const key_type& key, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	find(const key_type& key, const Handler& handler) const;

	YGGR_SAFE_KV_FIND_IMPL()

	// contains
	//inline bool contains(const key_type& key) const;

	//template<typename Key> inline
	//bool contains(const Key& key) const;

	YGGR_SAFE_KV_CONTAINS_IMPL()

	//size_type count(const key_type& key) const;

	YGGR_PP_SAFE_KV_COUNT_IMPL()

	//// lower_bound
	//template<typename Handler>
	//typename Handler::result_type
	//	lower_bound(const value_type& val, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	lower_bound(const value_type& val, const Handler& handler) const;

	YGGR_PP_SAFE_KV_LOWER_BOUND_IMPL()

	// upper_bound
	//template<typename Handler>
	//typename Handler::result_type
	//	upper_bound(const value_type& val, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	upper_bound(const value_type& val, const Handler& handler) const;

	YGGR_PP_SAFE_KV_UPPER_BOUND_IMPL()

	// equal_range
	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler) const;

	YGGR_PP_SAVE_KV_EQUAL_RANGE_IMPL()

	// allocator:
	//allocator_type get_allocator(void) const;

	YGGR_PP_SAFE_CONTAINER_GET_ALLOCATOR_IMPL()

	// safe other
	//bool is_exists(const key_type& key) const;

	YGGR_PP_SAFE_KV_IS_EXISTS_IMPL()

	//size_type replace(const key_type& key, BOOST_RV_REF(mapped_type) val);
	//size_type replace(const key_type& key, const mapped_type& val);

	//size_type replace(BOOST_RV_REF(value_type) val);
	//size_type replace(const value_type& val);

	YGGR_PP_SAFE_MAP_RELACE_IMPL()

	//// both_bound
	//template<typename Handler>
	//typename Handler::result_type
	//	both_bound(const value_type& lval, const value_type& uval,
	//				const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	both_bound(const value_type& lval, const value_type& uval,
	//				const Handler& handler) const;

	YGGR_PP_SAFE_KV_RANGE_IMPL()

	//base_type& copy_to_base(base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL()

	//isomeric_same_base_type& copy_to_base(isomeric_same_base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COPY_TO_BASE_IMPL(
		4, Map, container::is_isomeric_same_map)

	//base_type load(void) const;
	//base_type load(const allocator_type& alloc) const;

	YGGR_PP_SAFE_CONTAINER_LOAD_IMPL()

	// isomeric_same_base_type load(void) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_LOAD_IMPL(container::is_isomeric_same_map)

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	YGGR_PP_SAFE_CONTAINER_STONE_IMPL()

	//void store(BOOST_RV_REF(isomeric_same_base_type) base);
	//void store(const isomeric_same_base_type& base);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_STONE_IMPL(
		4, Map, container::is_isomeric_same_map)

	//value_type begin_value(void) const;
	//key_type begin_key(void) const;
	//mapped_type begin_mapped(void) const;

	//bool get_begin_value(value_type& val) const;
	//bool get_begin_key(key& key) const;
	//bool get_begin_mapped(mapped_type& mapped) const;

	//bool set_begin_mapped(BOOST_RV_REF(mapped_type) mapped);
	//bool set_begin_mapped(const mapped_type& mapped);

	YGGR_PP_SAFE_KV_BEGIN_VALUE_FUNCTION_IMPL()

	//value_type rbegin_value(void) const;
	//key_type rbegin_key(void) const;
	//mapped_type rbegin_mapped(void) const;

	//bool get_rbegin_value(value_type& val) const;
	//bool get_rbegin_key(key& key) const;
	//bool get_rbegin_mapped(mapped_type& mapped) const;

	//bool set_rbegin_mapped(BOOST_RV_REF(mapped_type) mapped);
	//bool set_rbegin_mapped(const mapped_type& mapped);

	YGGR_PP_SAFE_KV_RBEGIN_VALUE_FUNCTION_IMPL()

	// use handler:
	//template<typename Handler>
	//typename Handler::result_type using_handler(const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type using_handler(const Handler& handler) const;

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type
	//	using_handler(const Handler& handler, const Return_Handler& ret_handler);

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type
	//	using_handler(const Handler& handler, const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_CONTAINER_USING_HANDLER_IMPL()

	// using_hook_handler
	//template<typename Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler);
	//
	//template<typename Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler) const;
	//
	//template<typename Handler, typename Return_Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler,
	//						const Return_Handler& ret_handler);
	//
	//template<typename Handler, typename Return_Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler,
	//						const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_CONTAINER_USING_HOOK_HANDLER_IMPL()

	// for_each_iterator
	//template<typename Handler>
	//void for_each_iterator(const Handler& handler);

	//template<typename Handler>
	//void for_each_iterator(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator(const Handler& handler,
	//							const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator(const Handler& handler,
	//							const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_ITERATOR_IMPL()

	//template<typename Handler>
	//void for_each_iterator_reverse(const Handler& handler);

	//template<typename Handler>
	//void for_each_iterator_reverse(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator_reverse(const Handler& handler,
	//									const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator_reverse(const Handler& handler,
	//									const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_ITERATOR_REVERSE_IMPL()

	// for_each
	//template<typename Handler>
	//void for_each(const Handler& handler);

	//template<typename Handler>
	//void for_each(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each(const Handler& handler,
	//				const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each(const Handler& handler,
	//				const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_IMPL()

	//template<typename Handler>
	//void for_each_reverse(const Handler& handler);

	//template<typename Handler>
	//void for_each_reverse(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_reverse(const Handler& handler,
	//							const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_reverse(const Handler& handler,
	//						const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_REVERSE_IMPL()

private:
	mutable mutex_type _mutex;
	base_type _base;
};

YGGR_PP_SAFE_CONTAINER_COMPARER_FULL(5, safe_map)

YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(5, safe_map, container::is_isomeric_same_map)

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_SUPPORT(5, safe_map)

} // namespace safe_container
} // namespace yggr

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(5, yggr::safe_container::safe_map)

namespace yggr
{
namespace safe_container
{

template<template<typename _K, typename _T, typename _C, typename _A> class Map,
			typename K, typename T, typename M, typename C, typename A, typename Pred > inline
typename safe_map<K, T, M, C, A, Map>::size_type
	erase_if(safe_map<K, T, M, C, A, Map>& cont, Pred pred)
{
	return cont.erase_if(pred);
}

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support swap-------------------------------------
namespace yggr
{
namespace safe_container
{
namespace swap_support
{
	YGGR_PP_SAFE_CONTAINER_SWAP(5, safe_map)

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_SWAP(
		5, safe_map, container::is_isomeric_same_map)

} // namespace swap_support

using swap_support::swap;

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace boost

//------------------------multimap-------------------------------------------------------

namespace yggr
{
namespace safe_container
{

template<typename Key, typename Val,
			typename Mutex = void,
			typename Cmp = typename mplex::typename_expand_get<container::multimap<Key, Val>, 2>::type,
			typename Alloc = typename mplex::typename_expand_get<container::multimap<Key, Val>, 3>::type,
			template<typename _Key, typename _Val, typename _Cmp, typename _Alloc> class MultiMap = container::multimap
		>
class safe_multimap
{
public:
	typedef Alloc tpl_arg_allocator_type;

public:
	typedef MultiMap<Key, Val, Cmp, Alloc> base_type;

	typedef typename base_type::allocator_type allocator_type;

	typedef typename base_type::key_type key_type;
	typedef typename base_type::mapped_type mapped_type;

	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference;

	typedef typename base_type::key_compare key_compare;
	typedef typename base_type::value_compare value_compare;

	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;

	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	typedef safe_container_error::error_type error_type;

	typedef typename
		container::node_handle_traits
		<
			base_type
		>::node_type node_type;

private:
	typedef safe_container_error::error_maker_type error_maker_type;

private:
	typedef thread::mutex_def_helper<Mutex> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

private:
	typedef safe_multimap this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	//safe_multimap(void);
	//explicit safe_multimap(const key_compare& cmp);
	//explicit safe_multimap(const allocator_type& alloc);
	//safe_multimap(const key_compare& cmp, const allocator_type& alloc);

	//template<class InputIterator>
	//safe_multimap(InputIterator first, InputIterator last);

	//template<class InputIterator>
	//safe_multimap(InputIterator first, InputIterator last, const key_compare& cmp);

	//template<class InputIterator>
	//safe_multimap(InputIterator first, InputIterator last, const allocator_type& alloc);

	//template<class InputIterator>
	//safe_multimap(InputIterator first, InputIterator last, const key_compare& cmp, const allocator_type& alloc);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//safe_multimap(std::initializer_list<value_type> l);

	//safe_multimap(std::initializer_list<value_type> l, const allocator_type& alloc);

	//safe_multimap(std::initializer_list<value_type> l,
	//			const key_compare& key_comp,
	//			const allocator_type& alloc);

//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//explicit safe_multimap(BOOST_RV_REF(base_type) right);
	//explicit safe_multimap(const base_type& right);

	//safe_multimap(BOOST_RV_REF(base_type) right, const allocator_type& alloc);
	//safe_multimap(const base_type& right, const allocator_type& alloc);

	//safe_multimap(BOOST_RV_REF(this_type) right);
	//safe_multimap(const this_type& right);

	//safe_multimap(BOOST_RV_REF(this_type) right, const allocator_type& alloc);
	//safe_multimap(const this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_MULTIMAP_CONSTRUCTOR_IMPL()

	//safe_multimap(BOOST_RV_REF(isomeric_same_base_type) right);
	//safe_multimap(const isomeric_same_base_type& right);

	//safe_multimap(BOOST_RV_REF(isomeric_same_base_type) right, const allocator_type& alloc);
	//safe_multimap(const isomeric_same_base_type& right, const allocator_type& alloc);

	//safe_multimap(BOOST_RV_REF(isomeric_same_this_type) right);
	//safe_multimap(const isomeric_same_this_type& right);

	//safe_multimap(BOOST_RV_REF(isomeric_same_this_type) right, const allocator_type& alloc);
	//safe_multimap(const isomeric_same_this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_MULTIMAP_ISOMERIC_SAME_CONSTRUCTOR_IMPL(MultiMap)

	//~safe_multimap(void);
	YGGR_PP_SAFE_MULTIMAP_DESTRUCTOR_IMPL()

public:
	//inline this_type& operator=(BOOST_RV_REF(base_type) right);
	//this_type& operator=(const base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(this_type) right);
	//this_type& operator=(const this_type& right);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//this_type& operator=(std::initializer_list<value_type> il);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_KV_OPERATOR_SET_IMPL()

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_base_type) right);
	//this_type& operator=(const isomeric_same_base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_this_type) right);
	//this_type& operator=(const isomeric_same_this_type& right);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_OPERATOR_SET_IMPL(
		5, MultiMap, safe_multimap, container::is_isomeric_same_multimap)

public:
	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	YGGR_PP_SAFE_CONTAINER_SWAP_IMPL()

	//inline void swap(BOOST_RV_REF(isomeric_same_base_type) right);
	//void swap(isomeric_same_base_type& right);

	//inline void swap(BOOST_RV_REF(isomeric_same_this_type) right);
	//void swap(isomeric_same_this_type& right);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_SWAP_IMPL(
		5, MultiMap, safe_multimap, container::is_isomeric_same_multimap)

public:
	//operator base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL()

	//operator isomeric_same_base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_OPERATOR_BASE_IMPL(
		4, MultiMap, container::is_isomeric_same_multimap)

public:
	// capacity:
	//bool empty(void) const;
	//size_type size(void) const;
	//size_type max_size(void) const;

	YGGR_PP_SAFE_KV_CAPACITY_IMPL()

	// modifiers:
	// insert specially
	//bool insert(const key_type& key, BOOST_RV_REF(mapped_type) val);
	//bool insert(const key_type& key, const mapped_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	insert(const key_type& key, BOOST_RV_REF(mapped_type) val, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	insert(const key_type& key, const mapped_type& val, const Handler& handler);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler,
	//				const key_type& key, BOOST_RV_REF(mapped_type) val);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler,
	//				const key_type& key, const mapped_type& val);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//			const key_type& key, BOOST_RV_REF(mapped_type) val,
	//			const Handler& handler);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//			const key_type& key, const mapped_type& val,
	//			const Handler& handler);

	YGGR_PP_SAFE_KV_INSERT_FUNCTION_MMAP_SP_IMPL()

	//bool insert(BOOST_RV_REF(value_type) val);

	//bool insert(const value_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	insert(BOOST_RV_REF(value_type) val, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	insert(const value_type& val, const Handler& handler);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler,
	//				BOOST_RV_REF(value_type) val);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler,
	//				const value_type& val);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//			BOOST_RV_REF(value_type) val,
	//			const Handler& handler);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//			const value_type& val,
	//			const Handler& handler);

	//template<typename InputIter>
	//bool insert(InputIter start, InputIter last);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//bool insert(std::initializer_list<value_type> list);

//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	// insert node
	//bool insert(BOOST_RV_REF(node_type) node);

	//template<typename RetHandler>
	//typename ::yggr::func::foo_t_info<RetHandler>::result_type
	//	insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node);

	//template<typename IterHandler, typename RetHandler>
	//typename ::yggr::func::foo_t_info<RetHandler>::result_type
	//	insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler);

	YGGR_PP_SAFE_KV_NON_UNIQUE_INSERT_FUNCTION_IMPL()

	//// erase(key)
	//size_type erase(const key_type& val);

	//// erase(iterator) or  erase(std::pair<iterator, iterator>)
	//template<typename IterHandler>
	//bool erase(const IterHandler&);

	//template<typename Handler>
	//typename Handler::result_type
	//	erase(const key_type& key, const Handler& handler);

	//// erase(iterator, iterator)
	//template<typename EHandler, typename ZHandler>
	//bool erase(const EHandler& eins_handler, const ZHandler& zwei_iter_handler);

	//// handler(erase(std::pair<iterator, iterator>))
	//template<typename EHandler, typename ZHandler>
	//typename Handler::result_type erase(const EHandler& iter_handler, const Handler& handler);

	//// handler(erase(iterator, iterator))
	//template<typename EIterHandler, typename ZIterHandler, typename Handler>
	//typename Handler::result_type erase(const EIterHandler&, const ZIterHandler&, const Handler&)

	YGGR_PP_SAFE_KV_ERASE_FUNCTION_SP_IMPL()
	YGGR_PP_SAFE_KV_ERASE_FUNCTION_IMPL()

	// erase_if
	//template<typename Pred>
	//size_type erase_if(const Pred& pred);

	YGGR_PP_SAFE_KV_ERASE_IF_MEMBER_FUNCTION_IMPL()

	void clear(void)
	{
		base_type tmp(this_type::key_comp(), this_type::get_allocator());
		{
			write_lock_type lk(_mutex);
			_base.swap(tmp);
		}
	}

	//// emplace
	//template<typename ...Args>
	//bool emplace(non_handler, Args&&... args);

	//template<typename Handler, typename ...Args>
	//typename Handler::result_type
	//	emplace(const Handler& handler, Args&&... args);

	//// emplace_hint
	//template< typename IterHandler, typename ...Args>
	//bool emplace_hint (const IterHandler& iter_handler, Args&&... args);

	//template< typename IterHandler, typename Handler, typename ...Args>
	//typename Handler::result_type
	//	emplace_hint (const IterHandler& iter_handler,
	//					const Handler& handler,
	//					Args&&... args);

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	YGGR_PP_SAFE_KV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_KV_EMPLACE_HINT_FUNCTION_IMPL( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_KV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_KV_EMPLACE_HINT_FUNCTION_IMPL( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// extract_node
	//template<typename Key>
	//node_type extract(BOOST_RV_REF(Key) key);

	//template<typename Key>
	//node_type extract(const Key& key);

	//template<typename IterHandler>
	//node_type extract(const IterHandler& pos_handler);

	YGGR_PP_SAFE_CONTAINER_EXTRACT_NODE_IMPL()

	// merge
	// set multimap supported
	//template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMultiMap,
	//			typename C2, typename A2>
	//void merge(BOOST_RV_REF_BEG MapOrMultiMap<key_type, mapped_type, C2, A2> BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMultiMap,
	//			typename C2, typename A2>
	//void merge(MapOrMultiMap<key_type, mapped_type, C2, A2>& right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMultiMap,
	//			typename C2, typename A2, typename O2>
	//void merge(BOOST_RV_REF_BEG MapOrMultiMap<key_type, mapped_type, C2, A2, O2> BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMultiMap,
	//			typename C2, typename A2, typename O2>
	//void merge(MapOrMultiMap<key_type, mapped_type, C2, A2, O2>& right);

	//template<template<typename _K, typename _T, typename _C, typename _A> class UnSafeMap,
	//			typename M2, typename C2, typename A2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A> class UnSafeMap,
	//			typename M2, typename C2, typename A2>
	//void merge(safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap>& right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A> class UnsafeMultiMap,
	//			typename M2, typename C2, typename A2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiMap>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A> class UnsafeMultiMap,
	//			typename M2, typename C2, typename A2>
	//void merge(safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiMap>& right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnSafeMap,
	//			typename M2, typename C2, typename A2, typename O2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnSafeMap,
	//			typename M2, typename C2, typename A2, typename O2>
	//void merge(safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap>& right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnsafeMultiMap,
	//			typename M2, typename C2, typename A2, typename O2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiMap>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnsafeMultiMap,
	//			typename M2, typename C2, typename A2, typename O2>
	//void merge(safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiMap>& right);

	YGGR_PP_SAFE_MULTIMAP_MERGE_FUNCTION_IMPL()

	// observers:
	//key_compare key_comp(void) const;
	//value_compare value_comp(void) const;

	YGGR_PP_SAFE_KV_OBSERVERS_IMPL()

	// operations:
	//bool find(const key_type& key) const;

	//template<typename Handler>
	//typename Handler::result_type
	//	find(const key_type& key, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	find(const key_type& key, const Handler& handler) const;

	YGGR_SAFE_KV_FIND_IMPL()

	// contains
	//inline bool contains(const key_type& key) const;

	//template<typename Key> inline
	//bool contains(const Key& key) const;

	YGGR_SAFE_KV_CONTAINS_IMPL()

	//size_type count(const key_type& key) const;

	YGGR_PP_SAFE_KV_COUNT_IMPL()

	// lower_bound
	//template<typename Handler>
	//typename Handler::result_type
	//	lower_bound(const value_type& val, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	lower_bound(const value_type& val, const Handler& handler) const;

	YGGR_PP_SAFE_KV_LOWER_BOUND_IMPL()

	// upper_bound
	//template<typename Handler>
	//typename Handler::result_type
	//	upper_bound(const value_type& val, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	upper_bound(const value_type& val, const Handler& handler) const;

	YGGR_PP_SAFE_KV_UPPER_BOUND_IMPL()

	// equal_range
	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler) const;

	YGGR_PP_SAVE_KV_EQUAL_RANGE_IMPL()

	// allocator:
	//allocator_type get_allocator(void) const;

	YGGR_PP_SAFE_CONTAINER_GET_ALLOCATOR_IMPL()

	// safe other
	//bool is_exists(const key_type& key) const;

	YGGR_PP_SAFE_KV_IS_EXISTS_IMPL()

	// both_bound
	//template<typename Handler>
	//typename Handler::result_type
	//	both_bound(const value_type& lval, const value_type& uval,
	//				const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	both_bound(const value_type& lval, const value_type& uval,
	//				const Handler& handler) const;

	YGGR_PP_SAFE_KV_RANGE_IMPL()

	//base_type& copy_to_base(base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL()

	//isomeric_same_base_type& copy_to_base(isomeric_same_base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COPY_TO_BASE_IMPL(
		4, MultiMap, container::is_isomeric_same_multimap)

	//base_type load(void) const;
	//base_type load(const allocator_type& alloc) const;

	YGGR_PP_SAFE_CONTAINER_LOAD_IMPL()

	// isomeric_same_base_type load(void) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_LOAD_IMPL(container::is_isomeric_same_multimap)

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	YGGR_PP_SAFE_CONTAINER_STONE_IMPL()

	//void store(BOOST_RV_REF(isomeric_same_base_type) base);
	//void store(const isomeric_same_base_type& base);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_STONE_IMPL(
		4, MultiMap, container::is_isomeric_same_multimap)

	//value_type begin_value(void) const;
	//key_type begin_key(void) const;
	//mapped_type begin_mapped(void) const;

	//bool get_begin_value(value_type& val) const;
	//bool get_begin_key(key& key) const;
	//bool get_begin_mapped(mapped_type& mapped) const;

	//bool set_begin_mapped(BOOST_RV_REF(mapped_type) mapped);
	//bool set_begin_mapped(const mapped_type& mapped);

	YGGR_PP_SAFE_KV_BEGIN_VALUE_FUNCTION_IMPL()

	//value_type rbegin_value(void) const;
	//key_type rbegin_key(void) const;
	//mapped_type rbegin_mapped(void) const;

	//bool get_rbegin_value(value_type& val) const;
	//bool get_rbegin_key(key& key) const;
	//bool get_rbegin_mapped(mapped_type& mapped) const;

	//bool set_rbegin_mapped(BOOST_RV_REF(mapped_type) mapped);
	//bool set_rbegin_mapped(const mapped_type& mapped);

	YGGR_PP_SAFE_KV_RBEGIN_VALUE_FUNCTION_IMPL()

	// use handler:
	//template<typename Handler>
	//typename Handler::result_type using_handler(const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type using_handler(const Handler& handler) const;

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type
	//	using_handler(const Handler& handler, const Return_Handler& ret_handler);

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type
	//	using_handler(const Handler& handler, const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_CONTAINER_USING_HANDLER_IMPL()

	// using_hook_handler
	//template<typename Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler);
	//
	//template<typename Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler) const;
	//
	//template<typename Handler, typename Return_Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler,
	//						const Return_Handler& ret_handler);
	//
	//template<typename Handler, typename Return_Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler,
	//						const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_CONTAINER_USING_HOOK_HANDLER_IMPL()

	// for_each_iterator
	//template<typename Handler>
	//void for_each_iterator(const Handler& handler);

	//template<typename Handler>
	//void for_each_iterator(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator(const Handler& handler,
	//							const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator(const Handler& handler,
	//							const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_ITERATOR_IMPL()

	//template<typename Handler>
	//void for_each_iterator_reverse(const Handler& handler);

	//template<typename Handler>
	//void for_each_iterator_reverse(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator_reverse(const Handler& handler,
	//									const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator_reverse(const Handler& handler,
	//									const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_ITERATOR_REVERSE_IMPL()

	// for_each
	//template<typename Handler>
	//void for_each(const Handler& handler);

	//template<typename Handler>
	//void for_each(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each(const Handler& handler,
	//				const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each(const Handler& handler,
	//				const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_IMPL()

	//template<typename Handler>
	//void for_each_reverse(const Handler& handler);

	//template<typename Handler>
	//void for_each_reverse(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_reverse(const Handler& handler,
	//							const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_reverse(const Handler& handler,
	//						const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_REVERSE_IMPL()

private:
	mutable mutex_type _mutex;
	base_type _base;
};

YGGR_PP_SAFE_CONTAINER_COMPARER_FULL(5, safe_multimap)

YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(5, safe_multimap, container::is_isomeric_same_multimap)

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_SUPPORT(5, safe_multimap)

} // namespace safe_container
} // namespace yggr

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(5, yggr::safe_container::safe_multimap)

namespace yggr
{
namespace safe_container
{

template<template<typename _K, typename _T, typename _C, typename _A> class MultiMap,
			typename K, typename T, typename M, typename C, typename A, typename Pred > inline
typename safe_multimap<K, T, M, C, A, MultiMap>::size_type
	erase_if(safe_multimap<K, T, M, C, A, MultiMap>& cont, Pred pred)
{
	return cont.erase_if(pred);
}

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support swap-------------------------------------
namespace yggr
{
namespace safe_container
{
namespace swap_support
{
	YGGR_PP_SAFE_CONTAINER_SWAP(5, safe_multimap)

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_SWAP(
		5, safe_multimap, container::is_isomeric_same_multimap)

} // namespace swap_support

using swap_support::swap;

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace boost

//--------------------------------------------------support std::swap-------------------------------------

#endif // __YGGR_SAFE_CONTAINER_MAP_SAFE_MAP_ORG_HPP__
