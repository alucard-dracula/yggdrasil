//safe_unordered_map_org.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_UNORDERED_MAP_ORG_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_UNORDERED_MAP_ORG_HPP__

#ifndef __YGGR_SAFE_CONTAINER_SAFE_UNORDERED_MAP_HPP__
#	error "please include<yggr/safe_container/safe_unordered_map.hpp>"
#endif // __YGGR_SAFE_CONTAINER_SAFE_UNORDERED_MAP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/move/move.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/mutex_def_helper.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/unordered_map.hpp>
#include <yggr/safe_container/detail/swap_def.hpp>
#include <yggr/safe_container/detail/comparer_def.hpp>
#include <yggr/safe_container/detail/safe_insert_return_type_base.hpp>

#include <yggr/safe_container/detail/safe_element_reference.hpp>
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
#include <yggr/safe_container/detail/range_size_def.hpp>
#include <yggr/safe_container/detail/safe_unordered_gernel_impl.hpp>
#include <yggr/safe_container/safe_container_error.hpp>

#include <yggr/safe_container/safe_container_error.hpp>
#include <yggr/safe_container/unordered_map/safe_unordered_map_member_impl.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/range/functions.hpp>
#include <boost/function.hpp>
#include <boost/mpl/if.hpp>
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

template < typename Key, 
			typename Val,
			typename Mutex,
			typename HashFoo,
			typename Cmp,
			typename Alloc,
			template<typename _Key, typename _Val, typename _HashFoo, typename _Cmp, typename _Alloc> class Unordered_Map
		>
class safe_unordered_map;

template < typename Key, 
			typename Val,
			typename Mutex,
			typename HashFoo,
			typename Cmp,
			typename Alloc,
			template<typename _Key, typename _Val, typename _HashFoo, typename _Cmp, typename _Alloc> class Unordered_MultiMap
		>
class safe_unordered_multimap;

} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{

template < typename Key, typename Val,
			typename Mutex = void,
			typename HashFoo = typename mplex::typename_expand_get<boost::unordered_map<Key, Val>, 2>::type,
			typename Cmp = typename mplex::typename_expand_get<boost::unordered_map<Key, Val>, 3>::type,
			typename Alloc = typename mplex::typename_expand_get<boost::unordered_map<Key, Val>, 4>::type,
			template<typename _Key, typename _Val, typename _HashFoo, typename _Cmp, typename _Alloc>
				class Unordered_Map = container::unordered_map
		>
class safe_unordered_map
{
public:
	typedef Alloc tpl_arg_allocator_type;

public:
	typedef Unordered_Map<Key, Val, HashFoo, Cmp, Alloc> base_type;

	typedef typename base_type::key_type key_type;
	typedef typename base_type::mapped_type mapped_type;
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

	typedef typename base_type::allocator_type allocator_type;

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
	typedef safe_unordered_map this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	//safe_unordered_map(void);
	//explicit safe_unordered_map(const allocator_type& alloc);
	//safe_unordered_map(const key_equal& cmp, const allocator_type& alloc);
	//safe_unordered_map(const hasher& hash_foo, const key_equal& cmp, const allocator_type& alloc);
	//safe_unordered_map(size_type n, const hasher& hash_foo, const key_equal& cmp, const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_map(InputIt start, InputIt last);

	//template<typename InputIt>
	//safe_unordered_map(InputIt start, InputIt last,
	//					const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_map(InputIt start, InputIt last,
	//				const key_equal& cmp,
	//				const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_map(InputIt start, InputIt last,
	//				const hasher& hash_foo,
	//				const key_equal& cmp,
	//				const allocator_type& alloc);

	//template <class InputIt>
	//safe_unordered_map(InputIt start, InputIt last,
	//					size_type n,
	//					const hasher& hash_foo,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//safe_unordered_map(std::initializer_list<value_type> l);

	//safe_unordered_map(std::initializer_list<value_type> l,
	//					const allocator_type& alloc);

	//safe_unordered_map(std::initializer_list<value_type> l,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

	//safe_unordered_map(std::initializer_list<value_type> l,
	//						const hasher& hash_foo,
	//						const key_equal& cmp,
	//						const allocator_type& alloc);

	//safe_unordered_map(std::initializer_list<value_type> l,
	//					size_type n,
	//					const hasher& hash_foo,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST


	//safe_unordered_map(BOOST_RV_REF(base_type) right);
	//safe_unordered_map(const base_type& right);

	//safe_unordered_map(BOOST_RV_REF(base_type) right, const allocator_type& alloc)
	//safe_unordered_map(const base_type& right, const allocator_type& alloc);

	//safe_unordered_map(BOOST_RV_REF(this_type) right);
	//safe_unordered_map(const this_type& right);

	//safe_unordered_map(BOOST_RV_REF(this_type) right, const allocator_type& alloc);
	//safe_unordered_map(const this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_UNORDERED_MAP_CONSTRUCTOR_IMPL()

	//safe_unordered_map(BOOST_RV_REF(isomeric_same_this_type) right);
	//safe_unordered_map(const isomeric_same_this_type& right);

	//safe_unordered_map(BOOST_RV_REF(isomeric_same_this_type) right, const allocator_type& alloc);
	//safe_unordered_map(const isomeric_same_this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_UNORDERED_MAP_ISOMERIC_SAME_CONSTRUCTOR_IMPL(Unordered_Map)

	//~safe_unordered_map(void);

	YGGR_PP_SAFE_UNORDERED_MAP_DESTRUCTOR_IMPL()

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

	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_OPERATOR_SET_IMPL(6, Unordered_Map, safe_unordered_map)

public:
	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	YGGR_PP_SAFE_CONTAINER_SWAP_IMPL()

	//inline void swap(BOOST_RV_REF(isomeric_same_this_type) right);
	//void swap(isomeric_same_this_type& right);

	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_SWAP_IMPL(6, Unordered_Map, safe_unordered_map)

public:
	//operator base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL()

public:
	// capacity:
	//bool empty(void) const;
	//size_type size(void) const;
	//size_type max_size(void) const;

	YGGR_PP_SAFE_UKV_CAPACITY_IMPL()

	// element access
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

	YGGR_PP_SAFE_UNORDERED_MAP_SUBSCRIPT_AND_AT_IMPL()

	// element lookup

	// find
	// map specially
	bool find(const key_type& key, mapped_type& val) const
	{
		read_lock_type lk(_mutex);
		const_iterator iter = _base.find(key);
		return iter == _base.end()? false : (val = iter->second, true);
	}

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

	YGGR_SAFE_UKV_FIND_IMPL(unordered_map)

	// contains
	//inline bool contains(const key_type& key) const;

	//template<typename Key> inline
	//bool contains(const Key& key) const;

	YGGR_SAFE_UKV_CONTAINS_IMPL()

	//size_type count(const key_type& key) const;

	YGGR_PP_SAFE_UKV_COUNT_IMPL()

	// equal_range
	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler) const;

	YGGR_PP_SAVE_UKV_EQUAL_RANGE_IMPL()

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

	YGGR_PP_SAFE_UKV_INSERT_FUNCTION_SMAP_SP_IMPL()

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

	YGGR_PP_SAFE_UKV_UNIQUE_INSERT_FUNCTION_IMPL()

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

	YGGR_PP_SAFE_UKV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL()

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

	YGGR_PP_SAFE_KV_ERASE_FUNCTION_SP_IMPL()
	YGGR_PP_SAFE_KV_ERASE_FUNCTION_IMPL()

	// erase_if
	//template<typename Pred>
	//size_type erase_if(const Pred& pred);

	YGGR_PP_SAFE_UKV_ERASE_IF_MEMBER_FUNCTION_IMPL()

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
	
	YGGR_PP_SAFE_UKV_UNIQUE_EMPLACE_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_UKV_EMPLACE_HINT_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_UKV_UNIQUE_TRY_EMPLACE_FUNCTION_IMPL( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_UKV_UNIQUE_EMPLACE_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_UKV_EMPLACE_HINT_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_UKV_UNIQUE_TRY_EMPLACE_FUNCTION_IMPL( __n__ )

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

	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMultiMap, 
	//			typename H2, typename P2, typename A2> 
	//void merge(BOOST_RV_REF_BEG UMapOrUMultiMap<key_type, mapped_type, H2, P2, A2> BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMultiMap, 
	//			typename H2, typename P2, typename A2> 
	//void merge(UMapOrUMultiMap<key_type, mapped_type, H2, P2, A2>& right);

	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeUMap, 
	//			typename M2, typename H2, typename P2, typename A2> 
	//void merge(BOOST_RV_REF_BEG 
	//				safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeUMap> 
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeUMap, 
	//			typename M2, typename H2, typename P2, typename A2> 
	//void merge(safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeUMap>& right);
	//
	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeMultiMap, 
	//			typename M2, typename H2, typename P2, typename A2> 
	//void merge(BOOST_RV_REF_BEG 
	//				safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap> 
	//			BOOST_RV_REF_END right);

	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeMultiMap,
	//			typename M2, typename H2, typename P2, typename A2>
	//void merge(safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap>& right);

	YGGR_PP_SAFE_UMAP_MERGE_FUNCTION_IMPL()

	// buckets:
	//size_type bucket_count(void) const;
	//size_type max_bucket_count(void) const;
	//size_type bucket_size(size_type idx) const;

	YGGR_PP_SAFE_UKV_BUCKETS_IMPL()

	//size_type bucket(const key_type& key) const;

	YGGR_PP_SAFE_UKV_BUCKET_FUNCTION_IMPL()

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

	//size_type replace(const key_type& key, BOOST_RV_REF(mapped_type) val);
	//size_type replace(const key_type& key, const mapped_type& val);

	//size_type replace(BOOST_RV_REF(value_type) val);
	//size_type replace(const value_type& val);

	YGGR_PP_SAFE_MAP_RELACE_IMPL()

	//base_type& copy_to_base(base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL()

	//base_type load(void) const;
	//base_type load(const allocator_type& alloc) const;

	YGGR_PP_SAFE_CONTAINER_LOAD_IMPL()

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	YGGR_PP_SAFE_CONTAINER_STONE_IMPL()

	//value_type begin_value(void) const;
	//key_type begin_key(void) const;
	//mapped_type begin_mapped(void) const;
	//bool get_begin_value(std::pair<key_type, mapped_type>& val) const;
	//bool get_begin_key(key_type& key) const;
	//bool get_begin_mapped(mapped_type& mapped) const;

	YGGR_PP_SAFE_UKV_BEGIN_VALUE_FUNCTION_IMPL()

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

YGGR_PP_SAFE_CONTAINER_EQUALTO_AND_NOTEQUALTO(6, safe_unordered_map)

YGGR_PP_SAFE_UKV_ISOMERIC_SAME_EQUALTO_AND_NOTEQUALTO(6, safe_unordered_map)

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_SUPPORT(6, safe_unordered_map)

} // namespace safe_container
} // namespace yggr

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(6, yggr::safe_container::safe_unordered_map)

namespace yggr
{
namespace safe_container
{

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMap,
			typename K, typename T, typename M, typename H, typename P, typename A, typename Pred > inline
typename safe_unordered_map<K, T, M, H, P, A, UMap>::size_type
	erase_if(safe_unordered_map<K, T, M, H, P, A, UMap>& cont, Pred pred)
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
	YGGR_PP_SAFE_CONTAINER_SWAP(6, safe_unordered_map)

	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_SWAP(6, safe_unordered_map)

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

//----------------------------safe_unordered_multimap------------------------------------

namespace yggr
{
namespace safe_container
{

template < typename Key, typename Val,
			typename Mutex = void,
			typename HashFoo = typename mplex::typename_expand_get<boost::unordered_multimap<Key, Val>, 2>::type,
			typename Cmp = typename mplex::typename_expand_get<boost::unordered_multimap<Key, Val>, 3>::type,
			typename Alloc = typename mplex::typename_expand_get<boost::unordered_multimap<Key, Val>, 4>::type,
			template<typename _Key, typename _Val, typename _HashFoo, typename _Cmp, typename _Alloc>
				class Unordered_MultiMap = container::unordered_multimap
		>
class safe_unordered_multimap
{
public:
	typedef Alloc tpl_arg_allocator_type;

public:
	typedef Unordered_MultiMap<Key, Val, HashFoo, Cmp, Alloc> base_type;

	typedef typename base_type::key_type key_type;
	typedef typename base_type::mapped_type mapped_type;
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

	typedef typename base_type::allocator_type allocator_type;

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
	typedef safe_unordered_multimap this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	//safe_unordered_multimap(void);
	//explicit safe_unordered_multimap(const allocator_type& alloc);
	//safe_unordered_multimap(const key_equal& cmp, const allocator_type& alloc);
	//safe_unordered_multimap(const hasher& hash_foo, const key_equal& cmp, const allocator_type& alloc);
	//safe_unordered_multimap(size_type n, const hasher& hash_foo, const key_equal& cmp, const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_multimap(InputIt start, InputIt last);

	//template<typename InputIt>
	//safe_unordered_multimap(InputIt start, InputIt last,
	//					const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_multimap(InputIt start, InputIt last,
	//				const key_equal& cmp,
	//				const allocator_type& alloc);

	//template<typename InputIt>
	//safe_unordered_multimap(InputIt start, InputIt last,
	//				const hasher& hash_foo,
	//				const key_equal& cmp,
	//				const allocator_type& alloc);

	//template <class InputIt>
	//safe_unordered_multimap(InputIt start, InputIt last,
	//					size_type n,
	//					const hasher& hash_foo,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

	//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//safe_unordered_multimap(std::initializer_list<value_type> l);

	//safe_unordered_multimap(std::initializer_list<value_type> l,
	//					const allocator_type& alloc);

	//safe_unordered_multimap(std::initializer_list<value_type> l,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

	//safe_unordered_multimap(std::initializer_list<value_type> l,
	//						const hasher& hash_foo,
	//						const key_equal& cmp,
	//						const allocator_type& alloc);

	//safe_unordered_multimap(std::initializer_list<value_type> l,
	//					size_type n,
	//					const hasher& hash_foo,
	//					const key_equal& cmp,
	//					const allocator_type& alloc);

//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST


	//safe_unordered_multimap(BOOST_RV_REF(base_type) right);
	//safe_unordered_multimap(const base_type& right);

	//safe_unordered_multimap(BOOST_RV_REF(base_type) right, const allocator_type& alloc)
	//safe_unordered_multimap(const base_type& right, const allocator_type& alloc);

	//safe_unordered_multimap(BOOST_RV_REF(this_type) right);
	//safe_unordered_multimap(const this_type& right);

	//safe_unordered_multimap(BOOST_RV_REF(this_type) right, const allocator_type& alloc);
	//safe_unordered_multimap(const this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_UNORDERED_MULTIMAP_CONSTRUCTOR_IMPL()

	//safe_unordered_multimap(BOOST_RV_REF(isomeric_same_this_type) right);
	//safe_unordered_multimap(const isomeric_same_this_type& right);

	//safe_unordered_multimap(BOOST_RV_REF(isomeric_same_this_type) right, const allocator_type& alloc);
	//safe_unordered_multimap(const isomeric_same_this_type& right, const allocator_type& alloc);

	YGGR_PP_SAFE_UNORDERED_MULTIMAP_ISOMERIC_SAME_CONSTRUCTOR_IMPL(Unordered_MultiMap)

	//~safe_unordered_multimap(void);

	YGGR_PP_SAFE_UNORDERED_MULTIMAP_DESTRUCTOR_IMPL()

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

	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_OPERATOR_SET_IMPL(6, Unordered_MultiMap, safe_unordered_multimap)

public:
	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	YGGR_PP_SAFE_CONTAINER_SWAP_IMPL()

	//inline void swap(BOOST_RV_REF(isomeric_same_this_type) right);
	//void swap(isomeric_same_this_type& right);

	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_SWAP_IMPL(6, Unordered_MultiMap, safe_unordered_multimap)

public:
	//operator base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL()

public:
	// capacity:
	//bool empty(void) const;
	//size_type size(void) const;
	//size_type max_size(void) const;

	YGGR_PP_SAFE_UKV_CAPACITY_IMPL()

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

	YGGR_SAFE_UKV_FIND_IMPL(unordered_multimap)

	// contains
	//inline bool contains(const key_type& key) const;

	//template<typename Key> inline
	//bool contains(const Key& key) const;

	YGGR_SAFE_UKV_CONTAINS_IMPL()

	//size_type count(const key_type& key) const;

	YGGR_PP_SAFE_UKV_COUNT_IMPL()

	// equal_range
	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type
	//	equal_range(const key_type& key, const Handler& handler) const;

	YGGR_PP_SAVE_UKV_EQUAL_RANGE_IMPL()

	// modifiers
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

	YGGR_PP_SAFE_UKV_INSERT_FUNCTION_MMAP_SP_IMPL()

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

	YGGR_PP_SAFE_UKV_NON_UNIQUE_INSERT_FUNCTION_IMPL()

	//// erase(key)
	//size_type erase(const key_type& val);

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

	YGGR_PP_SAFE_UKV_ERASE_FUNCTION_SP_IMPL()
	YGGR_PP_SAFE_UKV_ERASE_FUNCTION_IMPL()

	// erase_if
	//template<typename Pred>
	//size_type erase_if(const Pred& pred);

	YGGR_PP_SAFE_UKV_ERASE_IF_MEMBER_FUNCTION_IMPL()

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
	
	YGGR_PP_SAFE_UKV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_UKV_EMPLACE_HINT_FUNCTION_IMPL( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_UKV_NON_UNIQUE_EMPLACE_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_UKV_EMPLACE_HINT_FUNCTION_IMPL( __n__ )

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

	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMultiMap, 
	//			typename H2, typename P2, typename A2> 
	//void merge(BOOST_RV_REF_BEG UMapOrUMultiMap<key_type, mapped_type, H2, P2, A2> BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMultiMap, 
	//			typename H2, typename P2, typename A2> 
	//void merge(UMapOrUMultiMap<key_type, mapped_type, H2, P2, A2>& right);

	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeUMap, 
	//			typename M2, typename H2, typename P2, typename A2> 
	//void merge(BOOST_RV_REF_BEG 
	//				safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeUMap> 
	//			BOOST_RV_REF_END right);
	//
	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeUMap, 
	//			typename M2, typename H2, typename P2, typename A2> 
	//void merge(safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeUMap>& right);
	//
	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeMultiMap, 
	//			typename M2, typename H2, typename P2, typename A2> 
	//void merge(BOOST_RV_REF_BEG 
	//				safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap> 
	//			BOOST_RV_REF_END right);

	//template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeMultiMap,
	//			typename M2, typename H2, typename P2, typename A2>
	//void merge(safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap>& right);

	YGGR_PP_SAFE_UMULTIMAP_MERGE_FUNCTION_IMPL()

	// buckets:
	//size_type bucket_count(void) const;
	//size_type max_bucket_count(void) const;
	//size_type bucket_size(size_type idx) const;

	YGGR_PP_SAFE_UKV_BUCKETS_IMPL()

	//size_type bucket(const key_type& key) const;

	YGGR_PP_SAFE_UKV_BUCKET_FUNCTION_IMPL()

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

	//base_type& copy_to_base(base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL()

	//base_type load(void) const;
	//base_type load(const allocator_type& alloc) const;

	YGGR_PP_SAFE_CONTAINER_LOAD_IMPL()

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	YGGR_PP_SAFE_CONTAINER_STONE_IMPL()

	//value_type begin_value(void) const;
	//key_type begin_key(void) const;
	//mapped_type begin_mapped(void) const;
	//bool get_begin_value(std::pair<key_type, mapped_type>& val) const;
	//bool get_begin_key(key_type& key) const;
	//bool get_begin_mapped(mapped_type& mapped) const;

	YGGR_PP_SAFE_UKV_BEGIN_VALUE_FUNCTION_IMPL()

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

YGGR_PP_SAFE_CONTAINER_EQUALTO_AND_NOTEQUALTO(6, safe_unordered_multimap)

YGGR_PP_SAFE_UKV_ISOMERIC_SAME_EQUALTO_AND_NOTEQUALTO(6, safe_unordered_multimap)

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_SUPPORT(6, safe_unordered_multimap)

} // namespace safe_container
} //namespace yggr

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(6, yggr::safe_container::safe_unordered_multimap)

	namespace yggr
{
namespace safe_container
{

template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMultiMap,
			typename K, typename T, typename M, typename H, typename P, typename A, typename Pred > inline
typename safe_unordered_multimap<K, T, M, H, P, A, UMultiMap>::size_type
	erase_if(safe_unordered_multimap<K, T, M, H, P, A, UMultiMap>& cont, Pred pred)
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
	YGGR_PP_SAFE_CONTAINER_SWAP(6, safe_unordered_multimap)

	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_SWAP(6, safe_unordered_multimap)

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

#endif //__YGGR_SAFE_CONTAINER_SAFE_UNORDERED_MAP_HPP__
