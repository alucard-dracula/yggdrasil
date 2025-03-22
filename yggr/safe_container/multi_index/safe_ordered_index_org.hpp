//safe_ordered_index_org.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_SAFE_ORDERED_INDEX_ORG_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_SAFE_ORDERED_INDEX_ORG_HPP__

#ifndef __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_ORDERED_INDEX_HPP__
#	error "please include<yggr/safe_container/safe_multi_index_ordered_index.hpp>"
#endif // __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_ORDERED_INDEX_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/thread/mutex_def_helper.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/any_val/operator_wrap.hpp>

#include <yggr/container/detail/node_handle_midx.hpp>
#include <yggr/container/node_handle_traits.hpp>
#include <yggr/container/multi_index_ordered_index.hpp>

#include <yggr/safe_container/multi_index/safe_ordered_index_decl.hpp>
#include <yggr/safe_container/multi_index/indexes_comparer_def.hpp>

#include <yggr/safe_container/multi_index/indexes_constructor_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_genel_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_swap_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_operator_set_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_insert_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_extract_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_erase_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_emplace_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_replace_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_modify_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_modify_key_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_find_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_count_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_bound_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_equal_range_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_using_handler_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_safe_other_impl.hpp>
#include <yggr/safe_container/multi_index/indexes_range_size_def.hpp>

#include <yggr/safe_container/multi_index/indexes_swap_def.hpp>

#include <yggr/safe_container/detail/for_each_impl.hpp>
#include <yggr/safe_container/detail/begin_rbegin_value_impl.hpp>
#include <yggr/safe_container/detail/safe_insert_return_type_base.hpp>

#include <yggr/safe_container/safe_container_error.hpp>

#include <yggr/safe_container/multi_index/safe_multi_index_container_decl.hpp>
#include <yggr/safe_container/multi_index/safe_ordered_index_gernel_impl.hpp>

#include <boost/multi_index_container.hpp>

#include <boost/mpl/pair.hpp>
#include <boost/mpl/int.hpp>
#include <boost/fusion/include/pair.hpp>

#include <cassert>

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{

template
<
	int N,
	typename Multi_Index_Container,
	typename Mutex
>
class safe_ordered_index
{
public:
	typedef Multi_Index_Container final_type;

	typedef typename 
		boost::multi_index::nth_index
		<
			final_type,
			N
		>::type base_type;

private:
	typedef thread::mutex_def_helper<Mutex> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

private:

	BOOST_MPL_ASSERT((boost::mpl::bool_< (N >= 0) >));

public:
	typedef typename base_type::key_type						key_type;
	typedef typename base_type::value_type						value_type;
	typedef typename base_type::key_from_value					key_from_value;
	typedef typename base_type::key_compare						key_compare;
	typedef typename base_type::value_compare					value_compare;
	typedef typename base_type::ctor_args						ctor_args;
	typedef typename base_type::allocator_type					allocator_type;
	typedef typename base_type::pointer							pointer;
	typedef typename base_type::const_pointer					const_pointer;
	typedef typename base_type::reference						reference;
	typedef typename base_type::const_reference					const_reference;
	typedef typename base_type::size_type						size_type;
	typedef typename base_type::difference_type					difference_type;

	typedef typename base_type::iterator						iterator;
	typedef typename base_type::const_iterator					const_iterator;
	typedef typename base_type::reverse_iterator				reverse_iterator;
	typedef typename base_type::const_reverse_iterator			const_reverse_iterator;
	typedef typename base_type::tag_list						tag_list;

	typedef boost::mpl::int_<N> nth_index_id_type;
	typedef boost::mpl::pair<nth_index_id_type, tag_list> index_id_type; 

	typedef safe_container_error::error_type error_type;

protected:
	typedef yggr::container::node_handle_traits<base_type> node_handle_traits_type;

public:
	typedef typename node_handle_traits_type::node_type				node_type;
	typedef typename node_handle_traits_type::insert_return_type	base_insert_return_type;
	typedef detail::safe_insert_return_type_base<node_type>			insert_return_type;

private:
	typedef safe_container_error::error_maker_type error_maker_type;

private:
	typedef safe_ordered_index this_type;

private:
	// disable default constructor
	safe_ordered_index(void);

public:
	//safe_ordered_index(mutex_type& mutex, base_type& base);
	//safe_ordered_index(const this_type& right);

	YGGR_PP_SAFE_MIDX_INDEXES_CONSTRUCTOR_IMPL(safe_ordered_index)

	//~safe_ordered_index(void);
	YGGR_PP_SAFE_MIDX_INDEXES_DESTRUCTOR_IMPL(safe_ordered_index)

public:
	//// operator=
	//inline this_type& operator=(BOOST_RV_REF(base_type) right);
	//this_type& operator=(const base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(final_type) right);
	//this_type& operator=(const final_type& right);

	//inline this_type& operator=(BOOST_RV_REF(this_type) right);
	//this_type& operator=(const this_type& right);

	//this_type& operator=(BOOST_RV_REF(safe_multi_index_containe_this) right);
	//this_type& operator=(const safe_multi_index_containe_this& right);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//this_type& operator=(std::initializer_list<value_type> list);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_MIDX_INDEXES_OPERATOR_SET_IMPL()

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_this_type) right);
	//this_type& operator=(const isomeric_same_this_type& right);

	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_OPERATOR_SET_IMPL(safe_ordered_index)

public:
	//inline void swap(BOOST_RV_REF(final_type) right);
	//void swap(final_type& right);

	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	//void swap(BOOST_RV_REF(safe_multi_index_containe_this) right);
	//void swap(safe_multi_index_containe_this& right);

	YGGR_PP_SAFE_MIDX_INDEXES_SWAP_IMPL()

	//inline void swap(BOOST_RV_REF(isomeric_same_this_type) right);
	//void swap(isomeric_same_this_type& right);

	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_SWAP_IMPL(safe_ordered_index)

public:
	//operator final_type(void) const;

	YGGR_PP_SAFE_MIDX_INDEXES_OPERATOR_FINAL_IMPL()

	//// iterators:
	//template<typename ValueHandler, typename Handler> 
	//typename Handler::result_type 
	//	iterator_to(const ValueHandler& val_handler, const Handler& handler);
	
	//template<typename ValueHandler, typename Handler> 
	//typename Handler::result_type 
	//	iterator_to(const ValueHandler& val_handler, const Handler& handler) const;

	YGGR_PP_SAFE_MIDX_INDEXES_ITERATOR_TO_IMPL()

	// size and capacity:

	//bool empty(void) const;
	//size_type size(void) const;
	//size_type max_size(void) const;

	YGGR_PP_SAFE_MIDX_INDEXES_CAPACITY_IMPL()

	//modifiers:

	//bool insert(BOOST_RV_REF(value_type) val);
	//bool insert(const value_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	insert(BOOST_RV_REF(value_type) val);

	//template<typename Handler>
	//typename Handler::result_type
	//	insert(const value_type& val);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val);
	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, const value_type& val);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val, const Handler& handler);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler, const value_type& val, const Handler& handler);

	//template<typename InputIter>
	//bool insert(InputIter first, InputIter last);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//bool insert(std::initializer_list<value_type> il);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//insert_return_type insert(BOOST_RV_REF(node_type) nh);

	//template<typename Handler>
	//typename ::yggr::func::foo_t_info<Handler>::result_type
	//	insert(BOOST_RV_REF(node_type) nh, const Handler& handler);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh);

	//template<typename IterHandler, typename Handler>
	//typename ::yggr::func::foo_t_info<Handler>::result_type
	//	insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) nh, const Handler& handler);

	YGGR_PP_SAFE_MIDX_KV_INDEXES_INSERT_IMPL()

	// extract
	//node_type extract(const key_type& key);

	//template<typename IterHandler>
	//node_type extract(const IterHandler& iter_handler);

	YGGR_PP_SAFE_MIDX_INDEXES_KV_EXTRACT_FUNCTION_IMPL()

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
	//template<typename EHandler, typename Handler>
	//typename Handler::result_type erase(const EHandler& iter_handler, const Handler& handler);

	//// handler(erase(iterator, iterator))
	//template<typename EIterHandler, typename ZIterHandler, typename Handler>
	//typename Handler::result_type erase(const EIterHandler&, const ZIterHandler&, const Handler&)

	YGGR_PP_SAFE_MIDX_KV_INDEXES_ERASE_FUNCTION_SP_IMPL()
	YGGR_PP_SAFE_MIDX_INDEXES_ERASE_FUNCTION_IMPL()

	//void clear(void);

	YGGR_PP_SAFE_MIDX_INDEXES_CLEAR_IMPL()

	// replace
	//template<typename IterHandler>
	//bool replace(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val);

	//template<typename IterHandler>
	//bool replace(const IterHandler& iter_handler, const value_type& val);

	YGGR_PP_SAFE_MIDX_INDEXES_REPLACE_IMPL()

	// find_and_replace
	//template<typename CKey>
	//bool find_and_replace(const CKey& key, BOOST_RV_REF(value_type) val);

	//template<typename CKey>
	//bool find_and_replace(const CKey& key, const value_type& val);

	//template<typename CKey, typename CPred>
	//bool find_and_replace(const CKey& key, const CPred& pred, 
	//						BOOST_RV_REF(value_type) val);

	//template<typename CKey, typename CPred>
	//bool find_and_replace(const CKey& key, const CPred& pred, 
	//						const value_type& val);

	YGGR_PP_SAFE_MIDX_KV_INDEXES_FIND_AND_REPLACE_IMPL()

	//// modify
	//template<typename IterHandler, typename Modifier> 
	//bool modify(const IterHandler& iter_handler, Modifier mod);

	//template<typename IterHandler, typename Modifier, typename Rollback>
	//bool modify(const IterHandler& iter_handler, Modifier mod, Rollback back);

	//template<typename IterHandler, typename KeyInfoHandler, typename ModifyHandler>
	//bool modify(const IterHandler& iter_handler,
	//				optimization_modifier<KeyInfoHandler, ModifyHandler> mod);

	//template<typename IterHandler, typename Modifier, typename Rollback>
	//bool modify(const IterHandler& iter_handler,
	//				optimization_modifier<KeyInfoHandler, ModifyHandler> mod,
	//				Rollback back);

	YGGR_PP_SAFE_MIDX_INDEXES_MODIFY_IMPL()

	//// find_and_modify
	//template<typename CKey, typename Modifier> 
	//bool find_and_modify(const CKey& key, Modifier mod);

	//template<typename CKey, typename Modifier, typename Rollback> 
	//bool find_and_modify(const CKey& key, Modifier mod, Rollback back);

	//template<typename CKey, typename KeyInfoHandler, typename ModifyHandler> 
	//bool find_and_modify(const CKey& key, 
	//						optimization_modifier<KeyInfoHandler, ModifyHandler> mod);
	
	//template<typename CKey, 
	//			typename KeyInfoHandler, typename ModifyHandler, 
	//			typename Rollback> 
	//bool find_and_modify(const CKey& key, 
	//						optimization_modifier<KeyInfoHandler, ModifyHandler> mod,
	//						Rollback back);

	//template<typename CKey, typename CPred,
	//			typename Modifier> 
	//bool find_and_modify(const CKey& key, const CPred& pred,
	//						Modifier mod);

	//template<typename CKey, typename CPred,
	//			typename Modifier, typename Rollback> 
	//bool find_and_modify(const CKey& key, const CPred& pred, 
	//						Modifier mod, Rollback back);

	//template<typename CKey, typename CPred, 
	//			typename KeyInfoHandler, typename ModifyHandler> 
	//bool find_and_modify(const CKey& key, const CPred& pred, 
	//						optimization_modifier<KeyInfoHandler, ModifyHandler> mod);

	//template<typename CKey, typename CPred, 
	//			typename KeyInfoHandler, typename ModifyHandler, 
	//			typename Rollback> 
	//bool find_and_modify(const CKey& key, const CPred& pred, 
	//						optimization_modifier<KeyInfoHandler, ModifyHandler> mod, 
	//						Rollback back);

	YGGR_PP_SAFE_MIDX_KV_INDEXES_FIND_AND_MODIFY_IMPL()
  
	////modify_key
	//template<typename IterHandler, typename Modifier> 
	//bool modify_key(const IterHandler& iter_handler, Modifier mod);

	//template<typename IterHandler, typename Modifier, typename Rollback>
	//bool modify_key(const IterHandler& iter_handler, Modifier mod, Rollback back);

	//template<typename IterHandler, 
	//			typename KeyInfoHandler, typename ModifyHandler> 
	//bool modify_key(const IterHandler& iter_handler, 
	//				optimization_modifier<KeyInfoHandler, ModifyHandler> mod);

	//template<typename IterHandler, 
	//			typename KeyInfoHandler, typename ModifyHandler, 
	//			typename Rollback> 
	//bool modify_key(const IterHandler& iter_handler, 
	//					optimization_modifier<KeyInfoHandler, ModifyHandler> mod, 
	//					Rollback back);

	YGGR_PP_SAFE_MIDX_INDEXES_MODIFY_KEY_IMPL()

	//// find_and_modfy_key
	//template<typename CKey, typename Modifier> 
	//bool find_and_modify_key(const CKey& key, Modifier mod);
	
	//template<typename CKey, typename Modifier, typename Rollback> 
	//bool find_and_modify_key(const CKey& key, Modifier mod, Rollback back);

	//template<typename CKey, typename KeyInfoHandler, typename ModifyHandler> 
	//bool find_and_modify_key(const CKey& key, 
	//							optimization_modifier<KeyInfoHandler, ModifyHandler> mod);

	//template<typename CKey, 
	//			typename KeyInfoHandler, typename ModifyHandler, 
	//			typename Rollback> 
	//bool find_and_modify_key(const CKey& key, 
	//							optimization_modifier<KeyInfoHandler, ModifyHandler> mod, 
	//							Rollback back);

	//template<typename CKey, typename CHash, typename CEq, 
	//			typename Modifier> 
	//bool find_and_modify_key(const CKey& key, const CHash& hash, const CEq& eq, 
	//							Modifier mod);
	
	//template<typename CKey, typename CHash, typename CEq, 
	//			typename Modifier, typename Rollback> 
	//bool find_and_modify_key(const CKey& key, const CHash& hash, const CEq& eq, 
	//							Modifier mod, Rollback back);

	//template<typename CKey, typename CHash, typename CEq, 
	//			typename KeyInfoHandler, typename ModifyHandler> 
	//bool find_and_modify_key(const CKey& key, const CHash& hash, const CEq& eq, 
	//							optimization_modifier<KeyInfoHandler, ModifyHandler> mod);

	//template<typename CKey, typename CHash, typename CEq, 
	//			typename KeyInfoHandler, typename ModifyHandler, typename Rollback> 
	//bool find_and_modify_key(const CKey& key, const CHash& hash, const CEq& eq, 
	//							optimization_modifier<KeyInfoHandler, ModifyHandler> mod, 
	//							Rollback back);

	//template<typename CKey, typename KeyInfoHandler, typename ModifyHandler> 
	//bool find_and_modify_key(const CKey& key, 
	//							optimization_modifier<KeyInfoHandler, ModifyHandler> mod);
	
	//template<typename CKey, typename KeyInfoHandler, typename ModifyHandler, typename Rollback> 
	//bool find_and_modify_key(const CKey& key, 
	//							optimization_modifier<KeyInfoHandler, ModifyHandler> mod, 
	//							Rollback back);

	//template<typename CKey, typename CPred, 
	//			typename KeyInfoHandler, typename ModifyHandler> 
	//bool find_and_modify_key(const CKey& key, const CPred& pred, 
	//							optimization_modifier<KeyInfoHandler, ModifyHandler> mod);
	
	//template<typename CKey, typename CPred, 
	//			typename KeyInfoHandler, typename ModifyHandler, 
	//			typename Rollback> 
	//bool find_and_modify_key(const CKey& key, const CPred& pred, 
	//							optimization_modifier<KeyInfoHandler, ModifyHandler> mod, 
	//							Rollback back);

	YGGR_PP_SAFE_MIDX_KV_INDEXES_FIND_AND_MODIFY_KEY_IMPL()

	////observers:
	//key_from_value key_extractor(void) const;

	//key_compare key_comp(void) const;

	//value_compare value_comp(void) const;

	YGGR_PP_SAFE_MIDX_ORDERED_INDEX_OBSERVERS_IMPL()

	// lookup:

	// find
	//template<typename CompatibleKey>
	//bool find(const CompatibleKey& key) const;

	//template<typename CompatibleKey, typename Handler>
	//typename Handler::result_type
	//	find(const CompatibleKey& key, const Handler& handler);

	//template<typename CompatibleKey, typename Handler>
	//typename Handler::result_type
	//	find(const CompatibleKey& key, const Handler& handler) const;

	//template<typename CompatibleKey,
	//			typename CompatiblePred >
	//bool find(const CompatibleKey& key, 
	//			const CompatiblePred& pred) const;

	//template<typename CompatibleKey,
	//			typename CompatiblePred,
	//			typename Handler>
	//typename Handler::result_type
	//	find(const CompatibleKey& key, 
	//			const CompatiblePred& pred,
	//			const Handler& handler);

	//template<typename CompatibleKey,
	//			typename CompatiblePred,
	//			typename Handler>
	//typename Handler::result_type
	//	find(const CompatibleKey& key, 
	//			const CompatiblePred& pred,
	//			const Handler& handler) const;

	YGGR_SAFE_MIDX_KV_INDEXES_FIND_IMPL()

	// get_value
	//template<typename CompatibleKey>
	//bool get_value(const CompatibleKey& key, value_type& val) const;

	//template<typename CompatibleKey,
	//			typename CompatiblePred >
	//bool get_value(const CompatibleKey& key, 
	//				const CompatiblePred& pred,
	//				value_type& val) const;

	YGGR_SAFE_MIDX_KV_INDEXES_GET_VALUE_IMPL()

	// count
	//template<typename CompatibleKey>
	//size_type count(const CompatibleKey& key) const;

	//template<typename CompatibleKey, 
	//			typename CompatiblePred >
	//size_type count(const CompatibleKey& key,
	//					const CompatiblePred& pred) const;

	YGGR_PP_SAFE_MIDX_KV_INDEXES_COUNT_IMPL()

	// equal_range
	//template<typename CompatibleKey, typename Handler>
	//typename Handler::result_type
	//	equal_range(const CompatibleKey& key, const Handler& handler);

	//template<typename CompatibleKey, typename Handler>
	//typename Handler::result_type
	//	equal_range(const CompatibleKey& key, const Handler& handler) const;

	//template<typename CompatibleKey, 
	//			typename CompatiblePred,
	//			typename Handler >
	//typename Handler::result_type
	//	equal_range(const CompatibleKey& key, 
	//					const CompatiblePred& pred,
	//					const Handler& handler);

	//template<typename CompatibleKey, 
	//			typename CompatiblePred,
	//			typename Handler >
	//typename Handler::result_type
	//	equal_range(const CompatibleKey& key, 
	//					const CompatiblePred& pred,
	//					const Handler& handler ) const;

	YGGR_PP_SAFE_MIDX_KV_INDEXES_EQUAL_RANGE_IMPL()

	// lower_bound
	//template<typename CKey, typename Handler>
	//typename Handler::result_type
	//	lower_bound(const CKey& x, const Handler& handler);

	//template<typename CKey, typename Handler>
	//typename Handler::result_type
	//	lower_bound(const CKey& x, const Handler& handler) const;

	//template<typename CKey, typename CPred, typename Handler>
	//typename Handler::result_type
	//	lower_bound(const CKey& x, const CPred& pred, const Handler& handler);

	//template<typename CKey, typename CPred, typename Handler>
	//typename Handler::result_type
	//	lower_bound(const CKey& x, const CPred& pred, const Handler& handler) const;

	YGGR_PP_SAFE_MIDX_KV_INDEXES_LOWER_BOUND_IMPL()

	//// upper_bound
	//template<typename CKey, typename Handler>
	//typename Handler::result_type
	//	upper_bound(const CKey& x, const Handler& handler);

	//template<typename CKey, typename Handler>
	//typename Handler::result_type
	//	upper_bound(const CKey& x, const Handler& handler) const;

	//template<typename CKey, typename CPred, typename Handler>
	//typename Handler::result_type
	//	upper_bound(const CKey& x, const CPred& pred, const Handler& handler);

	//template<typename CKey, typename CPred, typename Handler>
	//typename Handler::result_type
	//	upper_bound(const CKey& x, const CPred& pred, const Handler& handler) const;

	YGGR_PP_SAFE_MIDX_KV_INDEXES_UPPER_BOUND_IMPL()

	//range

	//template<typename LowerBounder, typename UpperBounder, typename Handler>
	//typename Handler::result_type
	//	range(const LowerBounder& lower, const UpperBounder& upper, const Handler& handler);

	//template<typename LowerBounder, typename UpperBounder, typename Handler>
	//typename Handler::result_type
	//	range(const LowerBounder& lower, const UpperBounder& upper, const Handler& handler) const;

	YGGR_PP_SAFE_MIDX_KV_INDEXES_RANGE_IMPL()

	//template<typename ...Args>
	//bool emplace(tag_non_handler, Args&&... args);

	//template<typename Handler, typename ...Args>
	//typename Handler::result_type
	//	emplace(const Handler& handler, Args&&... args);

	//template< typename IterHandler, typename ...Args> 
	//bool emplace_hint(const IterHandler& iter_handler, tag_non_handler, Args&&... args);

	//template< typename IterHandler, typename Handler, typename ...Args>
	//typename Handler::result_type
	//	emplace_hint(const IterHandler& iter_handler, const Handler& handler, Args&&... args);

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
	
	YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_IMPL( _ )
	YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_HINT_IMPL( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_IMPL( __n__ ) \
		YGGR_PP_SAFE_MIDX_KV_INDEXES_EMPLACE_HINT_IMPL( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// allocator
	//allocator_type get_allocator(void) const;

	YGGR_PP_SAFE_CONTAINER_GET_ALLOCATOR_IMPL()

	// safe_other
	//bool is_exists(const key_type& key) const;

	//template<typename CKey, typename CHash, typename CEq>
	//bool is_exists(const CKey& key, const CHash& hash, const CEq& eq) const;

	YGGR_PP_SAFE_MIDX_KV_INDEXES_IS_EXISTS_IMPL()

	//base_type& copy_to_base(base_type& out) const;
	//final_type& copy_to_base(final_type& out) const;

	YGGR_PP_SAFE_MIDX_INDEXES_COPY_TO_BASE_IMPL()

	//final_type load(void) const;

	YGGR_PP_SAFE_MIDX_INDEXES_LOAD_IMPL()

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	//void store(BOOST_RV_REF(final_type) final);
	//void store(const final_type& final);

	YGGR_PP_SAFE_MIDX_INDEXES_STONE_IMPL()

	//value_type begin_value(void) const;
	//bool get_begin_value(value_type& val) const;

	YGGR_PP_SAFE_MIDX_ORD_BEGIN_VALUE_FUNCTION_IMPL()

	//value_type rbegin_value(void) const;
	//bool get_rbegin_value(value_type& val) const;

	YGGR_PP_SAFE_MIDX_ORD_RBEGIN_VALUE_FUNCTION_IMPL()

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

	//template<typename Handler>
	//typename Handler::result_type using_handler_final(const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type using_handler_final(const Handler& handler) const;

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type 
	//	using_handler_final(const Handler& handler, const Return_Handler& ret_handler);

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type 
	//	using_handler_final(const Handler& handler, const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_MIDX_INDEXES_USING_HANDLER_FINAL_IMPL()

	// using_hook_handler_final
	//template<typename Handler> inline
	//typename Handler::result_type 
	//	using_hook_handler_final(const Handler& handler);
	//
	//template<typename Handler> inline 
	//typename Handler::result_type 
	//	using_hook_handler_final(const Handler& handler) const;

	//template<typename Handler, typename Return_Handler> inline
	//typename Handler::result_type 
	//	using_hook_handler_final(const Handler& handler, const Return_Handler& ret_handler);
	//
	//template<typename Handler, typename Return_Handler> inline 
	//typename Handler::result_type 
	//	using_hook_handler_final(const Handler& handler, const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_MIDX_INDEXES_USING_HOOK_HANDLER_FINAL_IMPL()

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
	/*mutable*/ mutex_type& _mutex;
	final_type& _final;
	base_type& _base;
};

YGGR_PP_SAFE_MIDX_INDEXES_COMPARER_FULL(safe_ordered_index)

YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_COMPARER_FULL(safe_ordered_index)

YGGR_PP_SAFE_MIDX_INDEXES_RANGE_SIZE_SUPPORT(safe_ordered_index)

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

YGGR_PP_SAFE_MIDX_INDEXES_RANGE_SIZE_CHECK_PASS_SUPPORT(yggr::safe_container::multi_index_detail::safe_ordered_index)


//--------------------------------------------------support swap-------------------------------------
namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
namespace swap_support
{
	YGGR_PP_SAFE_MIDX_INDEXES_SWAP(safe_ordered_index)

	YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_SWAP(safe_ordered_index)

} // namespace swap_support

using swap_support::swap;

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	using ::yggr::safe_container::multi_index_detail::swap_support::swap;
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	using ::yggr::safe_container::multi_index_detail::swap_support::swap;
} // namespace boost

#endif // __YGGR_SAFE_CONTAINER_SAFE_MULTI_INDEX_SAFE_ORDERED_INDEX_ORG_HPP__
