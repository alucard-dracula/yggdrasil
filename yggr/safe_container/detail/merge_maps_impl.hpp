//merge_maps_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_MERGE_MAPS_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_MERGE_MAPS_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container/map.hpp>
#include <yggr/container/options_map.hpp>
#include <yggr/safe_container/detail/merge_kv_helper.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

namespace yggr
{
namespace safe_container
{
namespace tpl_conflict_patch
{

// gcc need this fix conflict
template<typename K, typename T, typename MapOrMultiMap>
struct is_map_or_multimap_merge
	: public boost::mpl::false_
{
};

template<typename K, typename T,
			template<typename _K, typename _T, typename _C, typename _A> class MapOrMultiMap,
			typename C, typename A>
struct is_map_or_multimap_merge<K, T, MapOrMultiMap<K, T, C, A> >
	: public boost::mpl::true_
{
};

template<typename K, typename T, 
			template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMultiMap,
			typename C, typename A, typename O>
struct is_map_or_multimap_merge< K, T, MapOrMultiMap<K, T, C, A, O> >
	: public boost::mpl::true_
{
};

} // namespace tpl_conflict_patch
} // namespace safe_container
} // namespace yggr

// merge
//#define YGGR_PP_SAFE_MAP_UNIQUE_MERGE_UNSAFE_MAPS_FUNCTION_IMPL() \
//	template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMultiMap, \
//				typename C2, typename A2> \
//	void merge(BOOST_RV_REF_BEG MapOrMultiMap<key_type, mapped_type, C2, A2> BOOST_RV_REF_END right) { \
//		typedef MapOrMultiMap<key_type, mapped_type, C2, A2> right_type; \
//		if(!static_cast<right_type&>(right).size()) { return; } \
//		write_lock_type lk(_mutex); \
//		container::merge(_base, boost::move(right)); } \
//	\
//	template<template<typename _K, typename _T, typename _C, typename _A> class MapOrMultiMap, \
//				typename C2, typename A2> \
//	void merge(MapOrMultiMap<key_type, mapped_type, C2, A2>& right) { \
//		if(!right.size()) { return; } \
//		write_lock_type lk(_mutex); \
//		container::merge(_base, right); } \
//	\
//	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMultiMap, \
//				typename C2, typename A2, typename O2> \
//	void merge(BOOST_RV_REF_BEG MapOrMultiMap<key_type, mapped_type, C2, A2, O2> BOOST_RV_REF_END right) { \
//		typedef MapOrMultiMap<key_type, mapped_type, C2, A2, O2> right_type; \
//		if(!static_cast<right_type&>(right).size()) { return; } \
//		write_lock_type lk(_mutex); \
//		container::merge(_base, boost::move(right)); } \
//	\
//	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class MapOrMultiMap, \
//				typename C2, typename A2, typename O2> \
//	void merge(MapOrMultiMap<key_type, mapped_type, C2, A2, O2>& right) { \
//		if(!right.size()) { return; } \
//		write_lock_type lk(_mutex); \
//		container::merge(_base, right); }

#define YGGR_PP_SAFE_MAP_UNIQUE_MERGE_UNSAFE_MAPS_FUNCTION_IMPL() \
	template<typename MapOrMultiMap> \
	typename boost::enable_if<tpl_conflict_patch::is_map_or_multimap_merge<key_type, mapped_type, MapOrMultiMap>, void>::type \
		merge(BOOST_RV_REF(MapOrMultiMap) right) { \
		typedef MapOrMultiMap right_type; \
		if(!static_cast<right_type&>(right).size()) { return; } \
		write_lock_type lk(_mutex); \
		container::merge(_base, boost::move(right)); } \
	\
	template<typename MapOrMultiMap> \
	typename boost::enable_if<tpl_conflict_patch::is_map_or_multimap_merge<key_type, mapped_type, MapOrMultiMap>, void>::type \
		merge(MapOrMultiMap& right) { \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		container::merge(_base, right); }

#define YGGR_PP_SAFE_MAP_UNIQUE_MERGE_SAFE_MAPS_FUNCTION_IMPL() \
	template<template<typename _K, typename _T, typename _C, typename _A> class UnSafeMap, \
				typename M2, typename C2, typename A2> \
	void merge(BOOST_RV_REF_BEG \
					safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap> \
				BOOST_RV_REF_END right) { \
		typedef safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		right_type& right_ref = static_cast<right_type&>(right); \
		if(!right_ref.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right_ref.using_handler(helper); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A> class UnSafeMap, \
				typename M2, typename C2, typename A2> \
	void merge(safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap>& right) { \
		typedef safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right.using_handler(helper); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A> class UnsafeMultiMap, \
				typename M2, typename C2, typename A2> \
	void merge(BOOST_RV_REF_BEG \
					safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiMap> \
				BOOST_RV_REF_END right) { \
		typedef safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		right_type& right_ref = static_cast<right_type&>(right); \
		if(!right_ref.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right_ref.using_handler(helper); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A> class UnsafeMultiMap, \
				typename M2, typename C2, typename A2> \
	void merge(safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiMap>& right) { \
		typedef safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right.using_handler(helper); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnSafeMap, \
				typename M2, typename C2, typename A2, typename O2> \
	void merge(BOOST_RV_REF_BEG \
					safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap> \
				BOOST_RV_REF_END right) { \
		typedef safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		right_type& right_ref = static_cast<right_type&>(right); \
		if(!right_ref.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right_ref.using_handler(helper); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnSafeMap, \
				typename M2, typename C2, typename A2, typename O2> \
	void merge(safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap>& right) { \
		typedef safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right.using_handler(helper); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnsafeMultiMap, \
				typename M2, typename C2, typename A2, typename O2> \
	void merge(BOOST_RV_REF_BEG \
					safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiMap> \
				BOOST_RV_REF_END right) { \
		typedef safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		right_type& right_ref = static_cast<right_type&>(right); \
		if(!right_ref.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right_ref.using_handler(helper); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnsafeMultiMap, \
				typename M2, typename C2, typename A2, typename O2> \
	void merge(safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiMap>& right) { \
		typedef safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right.using_handler(helper); }

#define YGGR_PP_SAFE_MAP_MERGE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MAP_UNIQUE_MERGE_UNSAFE_MAPS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MAP_UNIQUE_MERGE_SAFE_MAPS_FUNCTION_IMPL()

#define YGGR_PP_SAFE_MAP_NON_UNIQUE_MERGE_SAFE_MAPS_FUNCTION_IMPL() \
	template<template<typename _K, typename _T, typename _C, typename _A> class UnSafeMap, \
				typename M2, typename C2, typename A2> inline \
	void merge(BOOST_RV_REF_BEG \
					safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap> \
				BOOST_RV_REF_END right) { \
		typedef safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(right.key_comp(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(boost::move(rtmp)); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A> class UnSafeMap, \
				typename M2, typename C2, typename A2> inline \
	void merge(safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap>& right) { \
		typedef safe_map<key_type, mapped_type, M2, C2, A2, UnSafeMap> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(right.key_comp(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(rtmp); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A> class UnsafeMultiSet, \
				typename M2, typename C2, typename A2> inline \
	void merge(BOOST_RV_REF_BEG \
					safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiSet> \
				BOOST_RV_REF_END right) { \
		typedef safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiSet> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(right.key_comp(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(boost::move(rtmp)); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A> class UnsafeMultiSet, \
				typename M2, typename C2, typename A2> inline \
	void merge(safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiSet>& right) { \
		typedef safe_multimap<key_type, mapped_type, M2, C2, A2, UnsafeMultiSet> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(right.key_comp(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(rtmp); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnSafeMap, \
				typename M2, typename C2, typename A2, typename O2> inline \
	void merge(BOOST_RV_REF_BEG \
					safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap> \
				BOOST_RV_REF_END right) { \
		typedef safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(right.key_comp(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(boost::move(rtmp)); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnSafeMap, \
				typename M2, typename C2, typename A2, typename O2> inline \
	void merge(safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap>& right) { \
		typedef safe_options_map<key_type, mapped_type, M2, C2, A2, O2, UnSafeMap> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(right.key_comp(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(rtmp); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnsafeMultiSet, \
				typename M2, typename C2, typename A2, typename O2> inline \
	void merge(BOOST_RV_REF_BEG \
					safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiSet> \
				BOOST_RV_REF_END right) { \
		typedef safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiSet> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(right.key_comp(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(boost::move(rtmp)); } \
	\
	template<template<typename _K, typename _T, typename _C, typename _A, typename _O> class UnsafeMultiSet, \
				typename M2, typename C2, typename A2, typename O2> inline \
	void merge(safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiSet>& right) { \
		typedef safe_options_multimap<key_type, mapped_type, M2, C2, A2, O2, UnsafeMultiSet> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(right.key_comp(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(rtmp); }

#define YGGR_PP_SAFE_MAP_NON_UNIQUE_OPT_MERGE_FUNCTION_IMPL() \
	template<typename C2, typename A2> inline \
	void merge(BOOST_RV_REF_BEG \
				container::map<key_type, mapped_type, C2, A2> \
				BOOST_RV_REF_END right) { \
		typedef container::map<key_type, mapped_type, C2, A2> right_type; \
		this_type::merge(static_cast<right_type&>(right)); } \
	\
	template<typename C2> \
	void merge(container::map<key_type, mapped_type, C2, allocator_type>& right) { \
		typedef container::map<key_type, mapped_type, C2, allocator_type> right_type; \
		typedef typename detail::cast_to_map_opt<right_type>::type right_opt_type; \
		typedef typename right_opt_type::multi_destroy_holder_type right_multi_destroy_holder_type; \
		typedef typename right_multi_destroy_holder_type::pointer right_pointer_type; \
		typedef container::vector<right_pointer_type> right_pointer_vt_type; \
		if(!right.size()) { return; } \
		right_multi_destroy_holder_type holder(right.size(), right.get_allocator()); \
		right_pointer_vt_type right_pointer_vt; \
		right_pointer_vt.reserve(right.size()); \
		(reinterpret_cast<right_opt_type&>(right)).clear(detail::allocator_destroy_holder_ref(holder)); \
		holder.copy_to(right_pointer_vt); \
		{ write_lock_type lk(_mutex); \
		opt_cast().insert(boost::begin(right_pointer_vt), boost::end(right_pointer_vt)); } \
		holder.non_free_clear(); } \
	\
	template<typename C2, typename A2> inline \
	void merge(BOOST_RV_REF_BEG \
					container::multimap<key_type, mapped_type, C2, A2> \
				BOOST_RV_REF_END right) { \
		typedef container::multimap<key_type, mapped_type, C2, A2> right_type; \
		this_type::merge(static_cast<right_type&>(right)); } \
	\
	template<typename C2> \
	void merge(container::multimap<key_type, mapped_type, C2, allocator_type>& right) { \
		typedef container::multimap<key_type, mapped_type, C2, allocator_type> right_type; \
		typedef typename detail::cast_to_multimap_opt<right_type>::type right_opt_type; \
		typedef typename right_opt_type::multi_destroy_holder_type right_multi_destroy_holder_type; \
		typedef typename right_multi_destroy_holder_type::pointer right_pointer_type; \
		typedef container::vector<right_pointer_type> right_pointer_vt_type; \
		if(!right.size()) { return; } \
		right_multi_destroy_holder_type holder(right.size(), right.get_allocator()); \
		right_pointer_vt_type right_pointer_vt; \
		right_pointer_vt.reserve(right.size()); \
		(reinterpret_cast<right_opt_type&>(right)).clear(detail::allocator_destroy_holder_ref(holder)); \
		holder.copy_to(right_pointer_vt); \
		{write_lock_type lk(_mutex); \
		opt_cast().insert(boost::begin(right_pointer_vt), boost::end(right_pointer_vt)); } \
		holder.non_free_clear(); } \
	\
	template<typename C2, typename A2, typename O2> inline \
	void merge(BOOST_RV_REF_BEG \
					container::options_map<key_type, mapped_type, C2, A2, O2> \
				BOOST_RV_REF_END right) { \
		typedef container::options_map<key_type, mapped_type, C2, A2, O2> right_type; \
		typedef typename right_type::base_type right_base_type; \
		this_type::merge(static_cast<right_base_type&>(right)); } \
	\
	template<typename C2, typename A2, typename O2> inline \
	void merge(container::options_map<key_type, mapped_type, C2, A2, O2>& right) { \
		typedef container::options_map<key_type, mapped_type, C2, A2, O2> right_type; \
		typedef typename right_type::base_type right_base_type; \
		this_type::merge(static_cast<right_base_type&>(right)); } \
	\
	template<typename C2, typename A2, typename O2> inline \
	void merge(BOOST_RV_REF_BEG \
					container::options_multimap<key_type, mapped_type, C2, A2> \
				BOOST_RV_REF_END right) { \
		typedef container::options_multimap<key_type, mapped_type, C2, A2, O2> right_type; \
		typedef typename right_type::base_type right_base_type; \
		this_type::merge(static_cast<right_base_type&>(right)); } \
	\
	template<typename C2, typename A2, typename O2> inline \
	void merge(container::options_multimap<key_type, mapped_type, C2, A2>& right) { \
		typedef container::options_multimap<key_type, mapped_type, C2, A2, O2> right_type; \
		typedef typename right_type::base_type right_base_type; \
		this_type::merge(static_cast<right_base_type&>(right)); }

#if BOOST_VERSION < 105600

#	define YGGR_PP_SAFE_MAP_NON_UNIQUE_OPT_MERGE_BOOST_SP_FUNCTION_IMPL()

#else

#	define YGGR_PP_SAFE_MAP_NON_UNIQUE_OPT_MERGE_BOOST_SP_FUNCTION_IMPL() \
		template<typename C2, typename A2, typename O2> inline \
		void merge(BOOST_RV_REF_BEG \
					boost::container::map<key_type, mapped_type, C2, A2, O2> \
					BOOST_RV_REF_END right) { \
			typedef boost::container::map<key_type, mapped_type, C2, A2, O2> right_type; \
			this_type::merge(static_cast<right_type&>(right)); } \
		\
		template<typename C2, typename O2> \
		void merge(boost::container::map<key_type, mapped_type, C2, allocator_type, O2>& right) { \
			typedef boost::container::map<key_type, mapped_type, C2, allocator_type, O2> right_type; \
			typedef typename detail::cast_to_map_opt<right_type>::type right_opt_type; \
			typedef typename right_opt_type::multi_destroy_holder_type right_multi_destroy_holder_type; \
			typedef typename right_multi_destroy_holder_type::pointer right_pointer_type; \
			typedef container::vector<right_pointer_type> right_pointer_vt_type; \
			if(!right.size()) { return; } \
			right_multi_destroy_holder_type holder(right.size(), right.get_allocator()); \
			right_pointer_vt_type right_pointer_vt; \
			right_pointer_vt.reserve(right.size()); \
			(reinterpret_cast<right_opt_type&>(right)).clear(detail::allocator_destroy_holder_ref(holder)); \
			holder.copy_to(right_pointer_vt); \
			{ write_lock_type lk(_mutex); \
			opt_cast().insert(boost::begin(right_pointer_vt), boost::end(right_pointer_vt)); } \
			holder.non_free_clear(); } \
		\
		template<typename C2, typename O2> inline \
		void merge(BOOST_RV_REF_BEG \
						boost::container::multimap<key_type, mapped_type, C2, allocator_type, O2> \
					BOOST_RV_REF_END right) { \
			typedef boost::container::multimap<key_type, mapped_type, C2, allocator_type> right_type; \
			this_type::merge(static_cast<right_type&>(right)); } \
		\
		template<typename C2, typename O2> \
		void merge(boost::container::multimap<key_type, mapped_type, C2, allocator_type, O2>& right) { \
			typedef boost::container::multimap<key_type, mapped_type, C2, allocator_type, O2> right_type; \
			typedef typename detail::cast_to_multimap_opt<right_type>::type right_opt_type; \
			typedef typename right_opt_type::multi_destroy_holder_type right_multi_destroy_holder_type; \
			typedef typename right_multi_destroy_holder_type::pointer right_pointer_type; \
			typedef container::vector<right_pointer_type> right_pointer_vt_type; \
			if(!right.size()) { return; } \
			right_multi_destroy_holder_type holder(right.size(), right.get_allocator()); \
			right_pointer_vt_type right_pointer_vt; \
			right_pointer_vt.reserve(right.size()); \
			(reinterpret_cast<right_opt_type&>(right)).clear(detail::allocator_destroy_holder_ref(holder)); \
			holder.copy_to(right_pointer_vt); \
			{ write_lock_type lk(_mutex); \
			opt_cast().insert(boost::begin(right_pointer_vt), boost::end(right_pointer_vt)); } \
			holder.non_free_clear(); }

#endif // BOOST_VERSION < 105600

#define YGGR_PP_SAFE_MULTIMAP_MERGE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MAP_UNIQUE_MERGE_UNSAFE_MAPS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MAP_NON_UNIQUE_MERGE_SAFE_MAPS_FUNCTION_IMPL()

#define YGGR_PP_SAFE_MULTIMAP_OPT_MERGE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MAP_UNIQUE_MERGE_UNSAFE_MAPS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MAP_NON_UNIQUE_MERGE_SAFE_MAPS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MAP_NON_UNIQUE_OPT_MERGE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_MAP_NON_UNIQUE_OPT_MERGE_BOOST_SP_FUNCTION_IMPL()


namespace yggr
{
namespace safe_container
{
namespace detail
{

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_MERGE_MAPS_IMPL_HPP__
