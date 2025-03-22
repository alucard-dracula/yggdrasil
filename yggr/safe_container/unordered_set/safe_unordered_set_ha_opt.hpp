//safe_unordered_set.hpp

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

// optimization hash and allocator
#ifndef __YGGR_SAFE_CONTAINER_UNORDERED_SET_SAFE_UNORDERED_SET_HA_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_UNORDERED_SET_SAFE_UNORDERED_SET_HA_OPT_HPP__

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#ifndef __YGGR_SAFE_CONTAINER_SAFE_UNORDERED_SET_HPP__
#	error "please include<yggr/safe_container/safe_unordered_set.hpp>"
#endif // __YGGR_SAFE_CONTAINER_SAFE_UNORDERED_SET_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/mutex_def_helper.hpp>

#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/unordered_set.hpp>
#include <yggr/safe_container/detail/boost_uset_ha_opt.hpp>
#include <yggr/safe_container/detail/boost_umset_ha_opt.hpp>
#include <yggr/safe_container/detail/swap_def.hpp>
#include <yggr/safe_container/detail/comparer_def.hpp>
#include <yggr/safe_container/detail/safe_insert_return_type_base.hpp>

#include <yggr/safe_container/detail/operator_set_impl.hpp>
#include <yggr/safe_container/detail/begin_rbegin_value_impl.hpp>
#include <yggr/safe_container/detail/swap_impl.hpp>
#include <yggr/safe_container/detail/insert_impl.hpp>
#include <yggr/safe_container/detail/emplace_impl.hpp>
#include <yggr/safe_container/detail/extract_node_impl.hpp>
#include <yggr/safe_container/detail/find_impl.hpp>
#include <yggr/safe_container/detail/contains_impl.hpp>
#include <yggr/safe_container/detail/erase_impl.hpp>
#include <yggr/safe_container/detail/equal_range_impl.hpp>
#include <yggr/safe_container/detail/using_handler_impl.hpp>
#include <yggr/safe_container/detail/for_each_impl.hpp>
#include <yggr/safe_container/detail/safe_other_impl.hpp>
#include <yggr/safe_container/detail/safe_unordered_gernel_impl.hpp>
#include <yggr/safe_container/detail/create_node_impl.hpp>
#include <yggr/safe_container/safe_container_error.hpp>
#include <yggr/safe_container/unordered_set/safe_unordered_set_member_impl.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/range/functions.hpp>
#include <boost/function.hpp>

#include <boost/type_traits/is_base_of.hpp>

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

template< typename Val,
			typename Mutex,
			typename HashFoo,
			typename Cmp,
			typename Alloc>
class safe_unordered_set< Val, Mutex, HashFoo, Cmp, Alloc, ::boost::unordered_set >
{
public:
	typedef Alloc tpl_arg_allocator_type;

public:
	typedef ::boost::unordered_set<Val, HashFoo, Cmp, Alloc > base_type;

	typedef typename base_type::allocator_type allocator_type;

	typedef typename base_type::key_type key_type;
	typedef typename base_type::value_type value_type;

	typedef typename base_type::hasher hasher;
	typedef typename base_type::key_equal key_equal;

	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference;

	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;

	typedef typename base_type::local_iterator local_iterator;
	typedef typename base_type::const_local_iterator const_local_iterator;

	typedef safe_container_error::error_type error_type;

	typedef typename
		container::node_handle_traits
		<
			base_type
		>::node_type node_type;

	BOOST_MPL_ASSERT_NOT((boost::is_base_of<mplex::null_type, node_type>));

	typedef typename
		container::node_handle_traits
		<
			base_type
		>::insert_return_type base_insert_return_type;

	typedef detail::safe_insert_return_type_base<node_type> insert_return_type;

private:
	typedef safe_container_error::error_maker_type error_maker_type;

private:
	typedef thread::mutex_def_helper<Mutex> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

private:
	typedef typename detail::cast_to_uset_ha_opt<base_type>::type opt_type;
	typedef typename opt_type::node_ptr_type node_ptr_type;
	typedef typename opt_type::destroy_holder_type destroy_holder_type;
	typedef typename opt_type::multi_destroy_holder_type multi_destroy_holder_type;

private:
	typedef safe_unordered_set this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	inline opt_type& opt_cast(void)
	{
		return reinterpret_cast<opt_type&>(_base);
	}

	inline const opt_type& opt_cast(void) const
	{
		return reinterpret_cast<const opt_type&>(_base);
	}

public:
	//safe_unordered_set(void);
	//explicit safe_unordered_set(const allocator_type& alloc);
	//explicit safe_unordered_set(const key_equal& cmp, const allocator_type& alloc);
	//explicit safe_unordered_set(const hasher& hash_foo, const key_equal& cmp, const allocator_type& alloc);
	//safe_unordered_set(size_type n, const hasher& hash_foo, const key_equal& cmp, const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_set(InputIt start, InputIt last);

	//template<typename InputIt>
	//safe_unordered_set(InputIt start, InputIt last,
	//				const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_set(InputIt start, InputIt last,
	//				const key_equal& cmp,
	//				const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_set(InputIt start, InputIt last,
	//				const hasher& hash_foo,
	//				const key_equal& cmp,
	//				const allocator_type& alloc);

	//template <class InputIt>
	//safe_unordered_set(InputIt start, InputIt last,
	//					size_type n,
	//					const hasher& hash_foo,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//safe_unordered_set(std::initializer_list<value_type> l);

	//safe_unordered_set(std::initializer_list<value_type> l,
	//					const allocator_type& alloc);

	//safe_unordered_set(std::initializer_list<value_type> l,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

	//safe_unordered_set(std::initializer_list<value_type> l,
	//					const hasher& hash_foo,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

	//safe_unordered_set(std::initializer_list<value_type> l,
	//					size_type n,
	//					const hasher& hash_foo,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//explicit safe_unordered_set(BOOST_RV_REF(base_type) right);
	//explicit safe_unordered_set(const base_type& right);

	//safe_unordered_set(BOOST_RV_REF(base_type) right, const allocator_type& alloc);
	//safe_unordered_set(const base_type& right, const allocator_type& alloc);

	//safe_unordered_set(BOOST_RV_REF(this_type) right);
	//safe_unordered_set(const this_type& right);

	//safe_unordered_set(BOOST_RV_REF(this_type) right, const allocator_type& alloc);
	//safe_unordered_set(const this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_UNORDERED_SET_CONSTRUCTOR_IMPL()

	//safe_unordered_set(BOOST_RV_REF(isomeric_same_this_type) right);
	//safe_unordered_set(const isomeric_same_this_type& right);

	//safe_unordered_set(BOOST_RV_REF(isomeric_same_this_type) right, const allocator_type& alloc);
	//safe_unordered_set(const isomeric_same_this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_UNORDERED_SET_ISOMERIC_SAME_CONSTRUCTOR_IMPL(::boost::unordered_set)

	//~safe_unordered_set(void);

	YGGR_PP_SAFE_UNORDERED_SET_DESTRUCTOR_IMPL()

public:
	//inline this_type& operator=(BOOST_RV_REF(base_type) right);
	//this_type& operator=(const base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(this_type) right);
	//this_type& operator=(const this_type& right);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//this_type& operator=(std::initializer_list<value_type> il);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_UKV_OPERATOR_SET_IMPL()

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_this_type) right);
	//this_type& operator=(const isomeric_same_this_type& right);

	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_OPERATOR_SET_IMPL(5, ::boost::unordered_set, safe_unordered_set)

public:
	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	YGGR_PP_SAFE_CONTAINER_SWAP_IMPL()

	//inline void swap(BOOST_RV_REF(isomeric_same_this_type) right);
	//void swap(isomeric_same_this_type& right);

	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_SWAP_IMPL(5, ::boost::unordered_set, safe_unordered_set)

public:
	//operator base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL()

public:
	// capacity:
	//bool empty(void) const;
	//size_type size(void) const;
	//size_type max_size(void) const;

	YGGR_PP_SAFE_UKV_CAPACITY_IMPL()

private:
	// create_node
#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

	YGGR_PP_SAFE_CONTAINER_HA_OPT_CREATE_NODE( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_CONTAINER_HA_OPT_CREATE_NODE( __n__ )

#	define YGGR_PP_FOO_ARG_NAME( ) arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // BOOST_CONTAINER_PERFECT_FORWARDING

public:
	// element lookup:
	//bool find(const key_type& key) const;

	//template<typename Handler>
	//typename Handler::result_type
	//	find(const key_type& key, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	find(const key_type& key, const Handler& handler) const;

	//template<typename CKey, typename CHash, typename CEq>
	//bool find(const CKey& key, const CHash& hash, const CEq& eq) const;
	//
	//template<typename CKey, typename CHash, typename CEq, typename Handler>
	//typename Handler::result_type
	//	find(const CKey& key, const CHash& hash, const CEq& eq, const Handler& handler);
	//
	//template<typename CKey, typename CHash, typename CEq, typename Handler>
	//typename Handler::result_type
	//	find(const CKey& key, const CHash& hash, const CEq& eq, const Handler& handler) const;

	YGGR_PP_SAFE_UKV_OPT_FIND_IMPL()

	// contains
	//inline bool contains(const key_type& key) const;

	//template<typename Key> inline
	//bool contains(const Key& key) const;

	YGGR_SAFE_UKV_CONTAINS_IMPL()

	//size_type count(const key_type& key) const;

	YGGR_PP_SAFE_UKV_OPT_COUNT_IMPL()

	// equal_range
	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler) const;

	YGGR_PP_SAVE_UKV_OPT_EQUAL_RANGE_IMPL()

	// modifiers:
	//insert
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

	YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_INSERT_FUNCTION_IMPL()

	// insert node
	//insert_return_type insert(BOOST_RV_REF(node_type) node);

	//template<typename RetHandler>
	//typename ::yggr::func::foo_t_info<RetHandler>::result_type
	//	insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node);

	//template<typename IterHandler, typename RetHandler>
	//typename ::yggr::func::foo_t_info<RetHandler>::result_type
	//	insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler);

	YGGR_PP_SAFE_USET_OPT_INSERT_NODE_FUNCTION_IMPL()

	//// erase(key)
	//size_type erase(const key_type& key);

	//template<typename Handler>
	//typename Handler::result_type
	//	erase(const key_type& key, const Handler& handler);

	//// erase(iterator) or  erase(std::pair<iterator, iterator>)
	//template<typename IterHandler>
	//bool erase(const IterHandler&);

	//// erase(iterator, iterator)
	//template<typename EHandler, typename ZHandler>
	//bool erase(const EHandler& eins_handler, const ZHandler& zwei_iter_handler);

	//// handler(erase(std::pair<iterator, iterator>))
	//template<typename EHandler, typename ZHandler>
	//typename Handler::result_type erase(const EHandler& iter_handler, const Handler& handler);

	//// handler(erase(iterator, iterator))
	//template<typename EIterHandler, typename ZIterHandler, typename Handler>
	//typename Handler::result_type erase(const EIterHandler&, const ZIterHandler&, const Handler&)

	YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_ERASE_FUNCTION_IMPL()

	// erase_if
	//template<typename Pred>
	//size_type erase_if(const Pred& pred);

	YGGR_PP_SAFE_UKV_ERASE_IF_OPT_MEMBER_FUNCTION_IMPL()

	void clear(void)
	{
		base_type tmp(0,
						this_type::hash_function(),
						this_type::key_eq(),
						this_type::get_allocator());
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

	YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_HINT_FUNCTION_IMPL( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_HINT_FUNCTION_IMPL( __n__ )

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

	YGGR_PP_SAFE_UKV_EXTRACT_NODE_OPT_IMPL()

	// merge
	//template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMultiSet,
	//			typename H2, typename P2, typename A2>
	//void merge(BOOST_RV_REF_BEG USetOrUMultiSet<key_type, H2, P2, A2> BOOST_RV_REF_END right);
	//
	//template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMultiSet,
	//			typename H2, typename P2, typename A2>
	//void merge(USetOrUMultiSet<key_type, H2, P2, A2>& right);
	//
	//template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeUSet,
	//			typename M2, typename H2, typename P2, typename A2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeUSet>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeSet,
	//			typename M2, typename H2, typename P2, typename A2>
	//void merge(safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeSet>& right);
	//
	//template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeMultiSet,
	//			typename M2, typename H2, typename P2, typename A2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeMultiSet,
	//			typename M2, typename H2, typename P2, typename A2>
	//void merge(safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet>& right);

	YGGR_PP_SAFE_USET_MERGE_FUNCTION_IMPL()

	// buckets:
	//size_type bucket_count(void) const;
	//size_type max_bucket_count(void) const;
	//size_type bucket_size(size_type idx) const;

	YGGR_PP_SAFE_UKV_BUCKETS_IMPL()

	//size_type bucket(const key_type& key) const

	YGGR_PP_SAFE_UKV_OPT_BUCKET_FUNCTION_IMPL()

	// hash policy:
	//f32 load_factor(void) const;
	//f32 max_load_factor(void) const;
	//void max_load_factor(f32 z);
	//void rehash(size_type n);
	//void reserve(size_type n);

	YGGR_PP_SAFE_UKV_HASH_POLICY_IMPL()

	// observers:
	//hasher hash_function(void) const;
	//key_equal key_eq(void) const;

	YGGR_PP_SAFE_UKV_OBSERVERS_IMPL()

	// allocator:
	//allocator_type get_allocator(void) const;

	YGGR_PP_SAFE_CONTAINER_GET_ALLOCATOR_IMPL()

	// safe other
	//bool is_exists(const key_type& key) const;

	//template<typename CKey, typename CHash, typename CEq>
	//bool is_exists(const CKey& key, const CHash& hash, const CEq& eq) const;

	YGGR_PP_SAFE_UKV_IS_EXISTS_IMPL()

	size_type replace(const key_type& key, BOOST_RV_REF(value_type) val)
	{
		if(key == opt_type::s_get_key(val))
		{
			return 1;
		}

		node_ptr_type pnode = node_ptr_type();
		allocator_type alloc = this_type::get_allocator();
		destroy_holder_type holder_erase(alloc);
		destroy_holder_type holder_ins(alloc);

		hasher hash = this_type::hash_function();
		std::size_t key_hash_old = opt_type::s_hash(hash, key);
		std::size_t key_hash_new = opt_type::s_hash(hash, val);

		{
			write_lock_type lk(_mutex);
			opt_type& opt = opt_cast();
			if(0 == opt.erase(detail::allocator_destroy_holder_ref(holder_erase), key_hash_old, key))
			{
				return 0;
			}

			pnode = holder_erase.template get_pointer<node_ptr_type>(0);
			assert(pnode);
			copy_or_move_or_swap(pnode->value(), boost::move(val));
			//pnode->hash_ = key_hash_new;
			opt_type::s_set_node_hash(pnode, key_hash_new);
			opt.insert(detail::allocator_destroy_holder_ref(holder_ins), key_hash_new, pnode);
		}

		holder_erase.non_free_clear();
		assert(holder_erase.size() == 0);
		return 1;
	}

	size_type replace(const key_type& key, const value_type& val)
	{
		if(key == opt_type::s_get_key(val))
		{
			return 1;
		}

		node_ptr_type pnode = node_ptr_type();
		allocator_type alloc = this_type::get_allocator();
		destroy_holder_type holder_erase(alloc);
		destroy_holder_type holder_ins(alloc);

		hasher hash = this_type::hash_function();
		std::size_t key_hash_old = opt_type::s_hash(hash, key);
		std::size_t key_hash_new = opt_type::s_hash(hash, val);

		{
			write_lock_type lk(_mutex);
			opt_type& opt = opt_cast();
			if(0 == opt.erase(detail::allocator_destroy_holder_ref(holder_erase), key_hash_old, key))
			{
				return 0;
			}

			pnode = holder_erase.template get_pointer<node_ptr_type>(0);
			assert(pnode);
			pnode->value() = val;
			//pnode->hash_ = key_hash_new;
			opt_type::s_set_node_hash(pnode, key_hash_new);
			opt.insert(detail::allocator_destroy_holder_ref(holder_ins), key_hash_new, pnode);
		}

		holder_erase.non_free_clear();
		assert(holder_erase.size() == 0);
		return 1;
	}

	//base_type& copy_to_base(base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL()

	//base_type load(void) const;
	//base_type load(const allocator_type& alloc) const;

	YGGR_PP_SAFE_CONTAINER_LOAD_IMPL()

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	YGGR_PP_SAFE_CONTAINER_STONE_IMPL()

	//value_type begin_value(void) const;
	//bool get_begin_value(value_type& val) const;

	YGGR_PP_SAFE_UV_UNSEQ_BEGIN_VALUE_FUNCTION_IMPL()

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
	//void for_each_local_iterator(size_type n, const Handler& handler);
	//
	//template<typename Handler>
	//void for_each_local_iterator(size_type n, const Handler& handler) const;
	//
	//template<typename Handler, typename Filter_Handler>
	//void for_each_local_iterator(size_type n,
	//						const Handler& handler,
	//						const Filter_Handler& filter_handler);
	//
	//template<typename Handler, typename Filter_Handler>
	//void for_each_local_iterator(size_type n,
	//								const Handler& handler,
	//								const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_UKV_FOR_EACH_LOCAL_ITERATOR_IMPL()

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
	//void for_each_local(size_type n, const Handler& handler);
	//
	//template<typename Handler>
	//void for_each_local(size_type n, const Handler& handler) const;
	//
	//template<typename Handler, typename Filter_Handler>
	//void for_each_local(size_type n, const Handler& handler, const Filter_Handler& filter_handler);
	//
	//template<typename Handler, typename Filter_Handler>
	//void for_each_local(size_type n, const Handler& handler,
	//						const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_UKV_FOR_EACH_LOCAL_IMPL()

private:
	mutable mutex_type _mutex;
	base_type _base;
};

} // namespace safe_container
} // namespace yggr

//----------------------hash_multiset---------------------------------
namespace yggr
{
namespace safe_container
{
template < typename Val,
			typename Mutex,
			typename HashFoo,
			typename Cmp,
			typename Alloc >
class safe_unordered_multiset<Val, Mutex, HashFoo, Cmp, Alloc, ::boost::unordered_multiset>
{
public:
	typedef Alloc tpl_arg_allocator_type;

public:
	typedef ::boost::unordered_multiset<Val, HashFoo, Cmp, Alloc > base_type;

	typedef typename base_type::allocator_type allocator_type;

	typedef typename base_type::key_type key_type;
	typedef typename base_type::value_type value_type;

	typedef typename base_type::hasher hasher;
	typedef typename base_type::key_equal key_equal;

	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference;

	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;

	typedef typename base_type::local_iterator local_iterator;
	typedef typename base_type::const_local_iterator const_local_iterator;

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
	typedef typename detail::cast_to_umset_ha_opt<base_type>::type opt_type;
	typedef typename opt_type::node_ptr_type node_ptr_type;
	typedef typename opt_type::destroy_holder_type destroy_holder_type;
	typedef typename opt_type::multi_destroy_holder_type multi_destroy_holder_type;

private:
	typedef safe_unordered_multiset this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	inline opt_type& opt_cast(void)
	{
		return reinterpret_cast<opt_type&>(_base);
	}

	inline const opt_type& opt_cast(void) const
	{
		return reinterpret_cast<const opt_type&>(_base);
	}

public:
	//safe_unordered_multiset(void);
	//explicit safe_unordered_multiset(const allocator_type& alloc);
	//explicit safe_unordered_multiset(const key_equal& cmp, const allocator_type& alloc);
	//explicit safe_unordered_multiset(const hasher& hash_foo, const key_equal& cmp, const allocator_type& alloc);
	//safe_unordered_multiset(size_type n, const hasher& hash_foo, const key_equal& cmp, const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_multiset(InputIt start, InputIt last);

	//template<typename InputIt>
	//safe_unordered_multiset(InputIt start, InputIt last,
	//				const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_multiset(InputIt start, InputIt last,
	//				const key_equal& cmp,
	//				const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_multiset(InputIt start, InputIt last,
	//				const hasher& hash_foo,
	//				const key_equal& cmp,
	//				const allocator_type& alloc);

	//template <class InputIt>
	//safe_unordered_multiset(InputIt start, InputIt last,
	//					size_type n,
	//					const hasher& hash_foo,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//safe_unordered_multiset(std::initializer_list<value_type> l);

	//safe_unordered_multiset(std::initializer_list<value_type> l,
	//					const allocator_type& alloc);

	//safe_unordered_multiset(std::initializer_list<value_type> l,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

	//safe_unordered_multiset(std::initializer_list<value_type> l,
	//					const hasher& hash_foo,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

	//safe_unordered_multiset(std::initializer_list<value_type> l,
	//					size_type n,
	//					const hasher& hash_foo,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//explicit safe_unordered_multiset(BOOST_RV_REF(base_type) right);
	//explicit safe_unordered_multiset(const base_type& right);

	//safe_unordered_multiset(BOOST_RV_REF(base_type) right, const allocator_type& alloc);
	//safe_unordered_multiset(const base_type& right, const allocator_type& alloc);

	//safe_unordered_multiset(BOOST_RV_REF(this_type) right);
	//safe_unordered_multiset(const this_type& right);

	//safe_unordered_multiset(BOOST_RV_REF(this_type) right, const allocator_type& alloc);
	//safe_unordered_multiset(const this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_UNORDERED_MULTISET_CONSTRUCTOR_IMPL()

	//safe_unordered_multiset(BOOST_RV_REF(isomeric_same_this_type) right);
	//safe_unordered_multiset(const isomeric_same_this_type& right);

	//safe_unordered_multiset(BOOST_RV_REF(isomeric_same_this_type) right, const allocator_type& alloc);
	//safe_unordered_multiset(const isomeric_same_this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_UNORDERED_MULTISET_ISOMERIC_SAME_CONSTRUCTOR_IMPL(::boost::unordered_multiset)

	//~safe_unordered_multiset(void);

	YGGR_PP_SAFE_UNORDERED_MULTISET_DESTRUCTOR_IMPL()

public:
	//inline this_type& operator=(BOOST_RV_REF(base_type) right);
	//this_type& operator=(const base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(this_type) right);
	//this_type& operator=(const this_type& right);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//this_type& operator=(std::initializer_list<value_type> il);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_UKV_OPERATOR_SET_IMPL()

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_this_type) right);
	//this_type& operator=(const isomeric_same_this_type& right);

	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_OPERATOR_SET_IMPL(5, ::boost::unordered_multiset, safe_unordered_multiset)

public:
	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	YGGR_PP_SAFE_CONTAINER_SWAP_IMPL()

	//inline void swap(BOOST_RV_REF(isomeric_same_this_type) right);
	//void swap(isomeric_same_this_type& right);

	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_SWAP_IMPL(5, ::boost::unordered_multiset, safe_unordered_multiset)

public:
	//operator base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL()

public:
	// capacity:
	//bool empty(void) const;
	//size_type size(void) const;
	//size_type max_size(void) const;

	YGGR_PP_SAFE_UKV_CAPACITY_IMPL()

private:
	// create_node
#ifdef BOOST_CONTAINER_PERFECT_FORWARDING

	YGGR_PP_SAFE_CONTAINER_HA_OPT_CREATE_NODE( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_CONTAINER_HA_OPT_CREATE_NODE( __n__ )

#	define YGGR_PP_FOO_ARG_NAME( ) arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // BOOST_CONTAINER_PERFECT_FORWARDING

public:
	// element lookup:
	//bool find(const key_type& key) const;

	//template<typename Handler>
	//typename Handler::result_type
	//	find(const key_type& key, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	find(const key_type& key, const Handler& handler) const;

	//template<typename CKey, typename CHash, typename CEq>
	//bool find(const CKey& key, const CHash& hash, const CEq& eq) const;
	//
	//template<typename CKey, typename CHash, typename CEq, typename Handler>
	//typename Handler::result_type
	//	find(const CKey& key, const CHash& hash, const CEq& eq, const Handler& handler);
	//
	//template<typename CKey, typename CHash, typename CEq, typename Handler>
	//typename Handler::result_type
	//	find(const CKey& key, const CHash& hash, const CEq& eq, const Handler& handler) const;

	YGGR_PP_SAFE_UKV_OPT_FIND_IMPL()

	// contains
	//inline bool contains(const key_type& key) const;

	//template<typename Key> inline
	//bool contains(const Key& key) const;

	YGGR_SAFE_UKV_CONTAINS_IMPL()

	//size_type count(const key_type& key) const;

	YGGR_PP_SAFE_UKV_OPT_COUNT_IMPL()

	// equal_range
	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler) const;

	YGGR_PP_SAVE_UKV_OPT_EQUAL_RANGE_IMPL()

	// modifiers:
	//insert
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

	YGGR_PP_SAFE_UKV_HA_OPT_NON_UNIQUE_INSERT_FUNCTION_IMPL()

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

	YGGR_PP_SAFE_UMULTISET_OPT_INSERT_NODE_FUNCTION_IMPL()

	//// erase(key)
	//size_type erase(const key_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	erase(const key_type& key, const Handler& handler);

	//// erase(iterator) or  erase(std::pair<iterator, iterator>)
	//template<typename IterHandler>
	//bool erase(const IterHandler&);

	//// erase(iterator, iterator)
	//template<typename EHandler, typename ZHandler>
	//bool erase(const EHandler& eins_handler, const ZHandler& zwei_iter_handler);

	//// handler(erase(std::pair<iterator, iterator>))
	//template<typename EHandler, typename ZHandler>
	//typename Handler::result_type erase(const EHandler& iter_handler, const Handler& handler);

	//// handler(erase(iterator, iterator))
	//template<typename EIterHandler, typename ZIterHandler, typename Handler>
	//typename Handler::result_type erase(const EIterHandler&, const ZIterHandler&, const Handler&)

	YGGR_PP_SAFE_UKV_HA_OPT_NON_UNIQUE_ERASE_FUNCTION_IMPL()

	// erase_if
	//template<typename Pred>
	//size_type erase_if(const Pred& pred);

	YGGR_PP_SAFE_UKV_ERASE_IF_OPT_MEMBER_FUNCTION_IMPL()

	void clear(void)
	{
		base_type tmp(0,
						this_type::hash_function(),
						this_type::key_eq(),
						this_type::get_allocator());
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

	//YGGR_PP_SAFE_KV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( _ )

	//// emplace_hint
	//template< typename IterHandler, typename ...Args>
	//bool emplace_hint (const IterHandler& iter_handler, Args&&... args);

	//template< typename IterHandler, typename Handler, typename ...Args>
	//typename Handler::result_type
	//	emplace_hint (const IterHandler& iter_handler,
	//					const Handler& handler,
	//					Args&&... args);

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_HINT_FUNCTION_IMPL( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_UKV_HA_OPT_EMPLACE_HINT_FUNCTION_IMPL( __n__ )

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

	YGGR_PP_SAFE_UKV_EXTRACT_NODE_OPT_IMPL()

	// merge
	//template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMultiSet,
	//			typename H2, typename P2, typename A2>
	//void merge(BOOST_RV_REF_BEG USetOrUMultiSet<key_type, H2, P2, A2> BOOST_RV_REF_END right);
	//
	//template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMultiSet,
	//			typename H2, typename P2, typename A2>
	//void merge(USetOrUMultiSet<key_type, H2, P2, A2>& right);
	//
	//template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeUSet,
	//			typename M2, typename H2, typename P2, typename A2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeUSet>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeSet,
	//			typename M2, typename H2, typename P2, typename A2>
	//void merge(safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeSet>& right);
	//
	//template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeMultiSet,
	//			typename M2, typename H2, typename P2, typename A2>
	//void merge(BOOST_RV_REF_BEG
	//				safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet>
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeMultiSet,
	//			typename M2, typename H2, typename P2, typename A2>
	//void merge(safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet>& right);

	YGGR_PP_SAFE_UMULTISET_MERGE_HA_OPT_FUNCTION_IMPL()

	// buckets:
	//size_type bucket_count(void) const;
	//size_type max_bucket_count(void) const;
	//size_type bucket_size(size_type idx) const;

	YGGR_PP_SAFE_UKV_BUCKETS_IMPL()

	//size_type bucket(const key_type& key) const;

	YGGR_PP_SAFE_UKV_OPT_BUCKET_FUNCTION_IMPL()

	// hash policy:
	//f32 load_factor(void) const;
	//f32 max_load_factor(void) const;
	//void max_load_factor(f32 z);
	//void rehash(size_type n);
	//void reserve(size_type n);

	YGGR_PP_SAFE_UKV_HASH_POLICY_IMPL()

	// observers:
	//hasher hash_function(void) const;
	//key_equal key_eq(void) const;

	YGGR_PP_SAFE_UKV_OBSERVERS_IMPL()

	// allocator:
	//allocator_type get_allocator(void) const;

	YGGR_PP_SAFE_CONTAINER_GET_ALLOCATOR_IMPL()

	// safe other
	//bool is_exists(const key_type& key) const;

	// safe other
	//bool is_exists(const key_type& key) const;

	//template<typename CKey, typename CHash, typename CEq>
	//bool is_exists(const CKey& key, const CHash& hash, const CEq& eq) const;

	YGGR_PP_SAFE_UKV_IS_EXISTS_IMPL()

	size_type replace(const key_type& key, const value_type& val)
	{
		typedef container::vector<node_ptr_type> node_ptr_vt_type;
		if(key == val)
		{
			return this_type::count(key);
		}

		//size_type size = 0;
		node_ptr_vt_type node_ptr_vt;
		multi_destroy_holder_type holder_erase(this_type::get_allocator());

		hasher hash = this_type::hash_function();
		std::size_t key_hash_old = opt_type::s_hash(hash, key);
		std::size_t key_hash_new = opt_type::s_hash(hash, val);

		{
			write_lock_type lk(_mutex);
			opt_type& opt = opt_cast();
			if(0 == opt.erase(detail::allocator_destroy_holder_ref(holder_erase), key_hash_old, key))
			{
				return 0;
			}

			holder_erase.copy_to(node_ptr_vt);

			for(typename node_ptr_vt_type::iterator i = node_ptr_vt.begin(), isize = node_ptr_vt.end();
					i != isize; ++i)
			{
				(*i)->value() = val;
				//(*i)->hash_ = key_hash_new;
				opt_type::s_set_node_hash((*i), key_hash_new);
			}
			opt.insert(key_hash_new, node_ptr_vt.begin(), node_ptr_vt.end());
		}

		holder_erase.non_free_clear();
		return node_ptr_vt.size();
	}

	//base_type& copy_to_base(base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL()

	//base_type load(void) const;
	//base_type load(const allocator_type& alloc) const;

	YGGR_PP_SAFE_CONTAINER_LOAD_IMPL()

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	YGGR_PP_SAFE_CONTAINER_STONE_IMPL()

	//value_type begin_value(void) const;
	//bool get_begin_value(value_type& val) const;

	YGGR_PP_SAFE_UV_UNSEQ_BEGIN_VALUE_FUNCTION_IMPL()

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
	//void for_each_local_iterator(size_type n, const Handler& handler);
	//
	//template<typename Handler>
	//void for_each_local_iterator(size_type n, const Handler& handler) const;
	//
	//template<typename Handler, typename Filter_Handler>
	//void for_each_local_iterator(size_type n,
	//						const Handler& handler,
	//						const Filter_Handler& filter_handler);
	//
	//template<typename Handler, typename Filter_Handler>
	//void for_each_local_iterator(size_type n,
	//								const Handler& handler,
	//								const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_UKV_FOR_EACH_LOCAL_ITERATOR_IMPL()

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
	//void for_each_local(size_type n, const Handler& handler);
	//
	//template<typename Handler>
	//void for_each_local(size_type n, const Handler& handler) const;
	//
	//template<typename Handler, typename Filter_Handler>
	//void for_each_local(size_type n, const Handler& handler, const Filter_Handler& filter_handler);
	//
	//template<typename Handler, typename Filter_Handler>
	//void for_each_local(size_type n, const Handler& handler,
	//						const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_UKV_FOR_EACH_LOCAL_IMPL()

private:
	mutable mutex_type _mutex;
	base_type _base;
};

} // namespace safe_container
} //namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif //__YGGR_SAFE_CONTAINER_UNORDERED_SET_SAFE_UNORDERED_SET_HA_OPT_HPP__
