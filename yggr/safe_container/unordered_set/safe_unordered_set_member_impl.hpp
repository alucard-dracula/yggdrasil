//safe_unordered_set_member_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_UNORDERED_SET_SAFE_UNORDERED_SET_MEMBER_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_UNORDERED_SET_SAFE_UNORDERED_SET_MEMBER_IMPL_HPP__

#ifndef __YGGR_SAFE_CONTAINER_SAFE_UNORDERED_SET_HPP__
#	error "this file is impl file, can't be used alone"
#endif // __YGGR_SAFE_CONTAINER_SAFE_UNORDERED_SET_HPP__

#include <yggr/container/unordered_set.hpp>
#include <yggr/safe_container/detail/safe_construct_impl.hpp>
#include <yggr/safe_container/detail/merge_kv_helper.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

//constructor
#define YGGR_PP_SAFE_UNORDERED_SET_CONSTRUCTOR_IMPL() \
	YGGR_PP_SAFE_UKV_CONSTRUCTOR_IMPL(safe_unordered_set)

#define YGGR_PP_SAFE_UNORDERED_SET_ISOMERIC_SAME_CONSTRUCTOR_IMPL( __base_tpl__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_CONSTRUCTOR_IMPL( safe_unordered_set, 5, __base_tpl__ )

#define YGGR_PP_SAFE_UNORDERED_MULTISET_CONSTRUCTOR_IMPL() \
	YGGR_PP_SAFE_UKV_CONSTRUCTOR_IMPL(safe_unordered_multiset)

#define YGGR_PP_SAFE_UNORDERED_MULTISET_ISOMERIC_SAME_CONSTRUCTOR_IMPL( __base_tpl__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_CONSTRUCTOR_IMPL( safe_unordered_multiset, 5, __base_tpl__ )

//destructor
#define YGGR_PP_SAFE_UNORDERED_SET_DESTRUCTOR_IMPL() \
	YGGR_PP_SAFE_UKV_DESTRUCTOR_IMPL(safe_unordered_set)

#define YGGR_PP_SAFE_UNORDERED_MULTISET_DESTRUCTOR_IMPL() \
	YGGR_PP_SAFE_UKV_DESTRUCTOR_IMPL(safe_unordered_multiset)

// ukv unique
#define YGGR_PP_SAFE_USET_OPT_INSERT_NODE_FUNCTION_IMPL() \
	insert_return_type insert(BOOST_RV_REF(node_type) node) { \
		if(!node) { return insert_return_type(); } \
		std::size_t key_hash = !node? 0 : opt_type::s_hash(this_type::hash_function(), node.value()); \
		write_lock_type lk(_mutex); \
		try { return insert_return_type(opt_cast().insert(key_hash, boost::move(node))); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return insert_return_type(); } } \
	\
	template<typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
		base_insert_return_type base_iret; \
		std::size_t key_hash = !node? 0 : opt_type::s_hash(this_type::hash_function(), node.value()); \
		write_lock_type lk(_mutex); \
		if(node) { \
			try { base_iret = opt_cast().insert(key_hash, boost::move(node)); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())); } } \
		return ret_handler(_base, base_iret); } \
	\
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node) { \
		if(!node) { return false; } \
		bool old_node_empty = !node; \
		if(old_node_empty) { return false; } \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.value()); \
		write_lock_type lk(_mutex); \
		try { return opt_cast().insert(key_hash, iter_handler(_base), boost::move(node)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename IterHandler, typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.value()); \
		write_lock_type lk(_mutex); \
		if(node) { \
			try { ret.first = opt_cast().insert(key_hash, iter_handler(_base), boost::move(node)); \
					ret.second = ret.first != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } } \
		return ret_handler(_base, ret); }

// ukv_non_unique
#define YGGR_PP_SAFE_UMULTISET_OPT_INSERT_NODE_FUNCTION_IMPL() \
	bool insert(BOOST_RV_REF(node_type) node) { \
		if(!node) { return false; } \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.value()); \
		write_lock_type lk(_mutex); \
		try { return opt_cast().insert(key_hash, boost::move(node)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.value()); \
		write_lock_type lk(_mutex); \
		if(node) { \
		try { ret.first = opt_cast().insert(key_hash, boost::move(node)); \
				ret.second = ret.first != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } } \
		return ret_handler(_base, ret); } \
	\
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node) { \
		if(!node) { return false; } \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.value()); \
		write_lock_type lk(_mutex); \
		try { return opt_cast().insert(key_hash, iter_handler(_base), boost::move(node)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename IterHandler, typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.value()); \
		write_lock_type lk(_mutex); \
		if(node) { \
			try { ret.first = opt_cast().insert(key_hash, iter_handler(_base), boost::move(node)); \
					ret.second = ret.first != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } } \
		return ret_handler(_base, ret); }

namespace yggr
{
namespace safe_container
{
namespace tpl_conflict_patch
{

// gcc need this fix conflict

template<typename K, typename USetOrUMultiSet>
struct is_uset_or_umultiset_merge
	: public boost::mpl::false_
{
};

template<typename T,
			template<typename _T, typename _H, typename _P, typename _A> class USetOrUMultiSet,
			typename H, typename P, typename A>
struct is_uset_or_umultiset_merge< T, USetOrUMultiSet< T, H, P, A> >
	: public boost::mpl::true_
{
};

} // namespace tpl_conflict_patch
} // namespace safe_container
} // namespace yggr

// merge
//#define YGGR_PP_SAFE_USET_UNIQUE_MERGE_UNSAFE_USETS_FUNCTION_IMPL() \
//	template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMultiSet, \
//				typename H2, typename P2, typename A2> \
//	void merge(BOOST_RV_REF_BEG USetOrUMultiSet<key_type, H2, P2, A2> BOOST_RV_REF_END right) { \
//		typedef USetOrUMultiSet<key_type, H2, P2, A2> right_type; \
//		if(!static_cast<right_type&>(right).size()) { return; } \
//		write_lock_type lk(_mutex); \
//		container::merge(_base, boost::move(right)); } \
//	\
//	template<template<typename _T, typename _H, typename _P, typename _A> class USetOrUMultiSet, \
//				typename H2, typename P2, typename A2> \
//	void merge(USetOrUMultiSet<key_type, H2, P2, A2>& right) { \
//		if(!right.size()) { return; } \
//		write_lock_type lk(_mutex); \
//		container::merge(_base, right); }

#define YGGR_PP_SAFE_USET_UNIQUE_MERGE_UNSAFE_USETS_FUNCTION_IMPL() \
	template<typename USetOrUMultiSet> \
	typename boost::enable_if<tpl_conflict_patch::is_uset_or_umultiset_merge<key_type, USetOrUMultiSet>, void>::type \
		merge(BOOST_RV_REF(USetOrUMultiSet) right) { \
		typedef USetOrUMultiSet right_type; \
		if(!static_cast<right_type&>(right).size()) { return; } \
		write_lock_type lk(_mutex); \
		container::merge(_base, boost::move(right)); } \
	\
	template<typename USetOrUMultiSet> \
	typename boost::enable_if<tpl_conflict_patch::is_uset_or_umultiset_merge<key_type, USetOrUMultiSet>, void>::type \
		merge(USetOrUMultiSet& right) { \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		container::merge(_base, right); }

#define YGGR_PP_SAFE_USET_UNIQUE_MERGE_SAFE_USETS_FUNCTION_IMPL() \
	template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeUSet, \
				typename M2, typename H2, typename P2, typename A2> \
	void merge(BOOST_RV_REF_BEG \
					safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeUSet> \
				BOOST_RV_REF_END right) { \
		typedef safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeUSet> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		right_type& right_ref = static_cast<right_type&>(right); \
		if(!right_ref.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right_ref.using_handler(helper); } \
	\
	template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeSet, \
				typename M2, typename H2, typename P2, typename A2> \
	void merge(safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeSet>& right) { \
		typedef safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeSet> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right.using_handler(helper); } \
	\
	template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeMultiSet, \
				typename M2, typename H2, typename P2, typename A2> \
	void merge(BOOST_RV_REF_BEG \
					safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet> \
				BOOST_RV_REF_END right) { \
		typedef safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		right_type& right_ref = static_cast<right_type&>(right); \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right_ref.using_handler(helper); } \
	\
	template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeMultiSet, \
				typename M2, typename H2, typename P2, typename A2> \
	void merge(safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet>& right) { \
		typedef safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right.using_handler(helper); }

#define YGGR_PP_SAFE_UMULTISET_OPT_MERGE_FUNCTION_IMPL() \
		template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeUSet, \
					typename M2, typename H2, typename P2, typename A2> inline \
		void merge(BOOST_RV_REF_BEG \
						safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeUSet> \
					BOOST_RV_REF_END right) { \
			typedef safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeUSet> right_type; \
			typedef typename right_type::base_type right_base_type; \
			right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
			right.swap(rtmp); \
			this_type::merge(boost::move(rtmp)); } \
		\
		template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeUSet, \
					typename M2, typename H2, typename P2, typename A2> inline \
		void merge(safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeUSet>& right) { \
			typedef safe_unordered_set<key_type, M2, H2, P2, A2, UnsafeUSet> right_type; \
			typedef typename right_type::base_type right_base_type; \
			right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
			right.swap(rtmp); \
			this_type::merge(rtmp); } \
		\
		template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeMultiSet, \
					typename M2, typename H2, typename P2, typename A2> inline \
		void merge(BOOST_RV_REF_BEG \
						safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet> \
					BOOST_RV_REF_END right) { \
			typedef safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet> right_type; \
			typedef typename right_type::base_type right_base_type; \
			right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
			right.swap(rtmp); \
			this_type::merge(boost::move(rtmp)); } \
		\
		template<template<typename _T, typename _H, typename _P, typename _A> class UnsafeMultiSet, \
					typename M2, typename H2, typename P2, typename A2> inline \
		void merge(safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet>& right) { \
			typedef safe_unordered_multiset<key_type, M2, H2, P2, A2, UnsafeMultiSet> right_type; \
			typedef typename right_type::base_type right_base_type; \
			right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
			right.swap(rtmp); \
			this_type::merge(rtmp); }

#define YGGR_PP_SAFE_USET_MERGE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_USET_UNIQUE_MERGE_UNSAFE_USETS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_USET_UNIQUE_MERGE_SAFE_USETS_FUNCTION_IMPL()

#define YGGR_PP_SAFE_USET_HA_OPT_NON_UNIQUE_MERGE_FUNCTION_IMPL() \
	template<typename H2, typename P2> inline \
	void merge(BOOST_RV_REF_BEG \
					boost::unordered_set<key_type, H2, P2, allocator_type> \
				BOOST_RV_REF_END right) { \
		typedef boost::unordered_set<key_type, H2, P2, allocator_type> right_type; \
		this_type::merge(static_cast<right_type&>(right)); } \
	\
	template<typename H2, typename P2> inline \
	void merge(boost::unordered_set<key_type, H2, P2, allocator_type>& right) { \
		typedef boost::unordered_set<key_type, H2, P2, allocator_type> right_type; \
		if(!right.size()) { return; } \
		right_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::insert(rtmp.begin(), rtmp.end()); } \
	\
	template<typename H2, typename P2> inline \
	void merge(BOOST_RV_REF_BEG \
					boost::unordered_multiset<key_type, H2, P2, allocator_type> \
				BOOST_RV_REF_END right) { \
		typedef boost::unordered_multiset<key_type, H2, P2, allocator_type> right_type; \
		this_type::merge(static_cast<right_type&>(right)); } \
	\
	template<typename H2, typename P2> \
	void merge(boost::unordered_multiset<key_type, H2, P2, allocator_type>& right) { \
		typedef boost::unordered_multiset<key_type, H2, P2, allocator_type> right_type; \
		typedef typename detail::cast_to_umset_ha_opt<right_type>::type right_opt_type; \
		typedef typename right_opt_type::multi_destroy_holder_type right_multi_destroy_holder_type; \
		typedef typename right_opt_type::node_pointer right_pointer_type; \
		typedef container::vector<std::size_t> right_key_hash_vt_type; \
		typedef container::vector<right_pointer_type> right_pointer_vt_type; \
		typedef typename right_pointer_vt_type::iterator right_pointer_iter_type; \
		if(!right.size()) { return; } \
		right_multi_destroy_holder_type holder(right.size(), right.get_allocator()); \
		right_key_hash_vt_type right_key_hash_vt; \
		right_pointer_vt_type right_pointer_vt; \
		right_key_hash_vt.reserve(right.size()); \
		right_pointer_vt.reserve(right.size()); \
		(reinterpret_cast<right_opt_type&>(right)).clear(detail::allocator_destroy_holder_ref(holder)); \
		holder.copy_to(right_pointer_vt); \
		holder.non_free_clear(); \
		hasher hfoo = this_type::hash_function(); \
		opt_type::s_rehash_range(right_key_hash_vt, this_type::hash_function(), right_pointer_vt.begin(), right_pointer_vt.end()); \
		{ write_lock_type lk(_mutex); \
			opt_cast().insert(right_key_hash_vt.begin(), right_key_hash_vt.end(), right_pointer_vt.begin(), right_pointer_vt.end()); } } \
	\
	template<typename M2, typename H2, typename P2> inline \
	void merge(BOOST_RV_REF_BEG \
					safe_unordered_set<key_type, M2, H2, P2, allocator_type> \
				BOOST_RV_REF_END right) { \
		typedef safe_unordered_set<key_type, M2, H2, P2, allocator_type> right_type; \
		this_type::merge(static_cast<right_type&>(right)); } \
	\
	template<typename M2, typename H2, typename P2> inline \
	void merge(safe_unordered_set<key_type, M2, H2, P2, allocator_type>& right) { \
		typedef safe_unordered_set<key_type, M2, H2, P2, allocator_type> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(rtmp); } \
	\
	template<typename M2, typename H2, typename P2> inline \
	void merge(BOOST_RV_REF_BEG \
					safe_unordered_multiset<key_type, M2, H2, P2, allocator_type> \
				BOOST_RV_REF_END right) { \
		typedef safe_unordered_multiset<key_type, M2, H2, P2, allocator_type> right_type; \
		this_type::merge(static_cast<right_type&>(right)); } \
	\
	template<typename M2, typename H2, typename P2> inline \
	void merge(safe_unordered_multiset<key_type, M2, H2, P2, allocator_type>& right) { \
		typedef safe_unordered_multiset<key_type, M2, H2, P2, allocator_type> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(rtmp); }

#define YGGR_PP_SAFE_UMULTISET_MERGE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_USET_UNIQUE_MERGE_UNSAFE_USETS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UMULTISET_OPT_MERGE_FUNCTION_IMPL()

#define YGGR_PP_SAFE_UMULTISET_MERGE_HA_OPT_FUNCTION_IMPL() \
	YGGR_PP_SAFE_USET_UNIQUE_MERGE_UNSAFE_USETS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UMULTISET_OPT_MERGE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_USET_HA_OPT_NON_UNIQUE_MERGE_FUNCTION_IMPL()

#endif // __YGGR_SAFE_CONTAINER_UNORDERED_SET_SAFE_UNORDERED_SET_MEMBER_IMPL_HPP__
