//safe_other_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_SAFE_OTHER_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_SAFE_OTHER_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>

#include <yggr/container/reserve.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#define YGGR_PP_SAFE_CONTAINER_LOAD_IMPL() \
	base_type load(void) const { \
		read_lock_type lk(_mutex); \
		return _base; } \
	\
	base_type load(const allocator_type& alloc) const { \
		read_lock_type lk(_mutex); \
		return base_type(_base, alloc); } \
	\
	template<typename Cont> \
	typename boost::enable_if< boost::is_same<Cont, base_type>, Cont >::type \
		load(void) const { \
		read_lock_type lk(_mutex); \
		return _base; } \
	\
	template<typename Cont> \
	typename boost::enable_if< boost::is_same<Cont, base_type>, Cont >::type \
		load(const allocator_type& alloc) const { \
		typedef Cont ret_type; \
		read_lock_type lk(_mutex); \
		return ret_type(_base, alloc); }

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_LOAD_IMPL( __is_isomeric_same__ ) \
	template<typename Cont> \
	typename boost::enable_if< __is_isomeric_same__<Cont, base_type>, Cont>::type \
		load(void) const { \
		typedef Cont ret_type; \
		allocator_type alloc = this_type::get_allocator(); \
		read_lock_type lk(_mutex); \
		return ret_type(reinterpret_cast<const ret_type&>(_base), alloc); } \
	\
	template<typename Cont> \
	typename boost::enable_if< __is_isomeric_same__<Cont, base_type>, Cont>::type \
		load(const allocator_type& alloc) const { \
		typedef Cont ret_type; \
		read_lock_type lk(_mutex); \
		return ret_type(reinterpret_cast<const ret_type&>(_base), alloc); }


#define YGGR_PP_SAFE_CONTAINER_STONE_IMPL() \
	inline void store(BOOST_RV_REF(base_type) base) { \
		this_type::operator=(boost::move(base)); } \
	\
	inline void store(const base_type& base) { \
		this_type::operator=(base); }

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_STONE_IMPL( \
			__type_count__, __base_tpl__, __is_isomeric_same__) \
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >, \
				base_type >, \
			void >::type \
		store(BOOST_RV_REF_BEG \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) > \
				BOOST_RV_REF_END base) { \
		typedef __base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) > right_type; \
		right_type& right_ref = base; \
		this_type::store(boost::move(reinterpret_cast<base_type&>(right_ref))); } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >, \
				base_type >, \
			void >::type \
		store(const __base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >& right) { \
		this_type::store(reinterpret_cast<const base_type&>(right)); }


#define YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL() \
	operator base_type(void) const { \
		read_lock_type lk(_mutex); \
		return _base; }

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_OPERATOR_BASE_IMPL( \
			__type_count__, __base_tpl__, __is_isomeric_same__ ) \
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) inline \
	operator \
		__base_tpl__ \
		BOOST_PP_EXPR_IF( __type_count__, < ) \
			YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT )  \
		BOOST_PP_EXPR_IF( __type_count__, > ) (void) const { \
		typedef \
			__base_tpl__ \
				BOOST_PP_EXPR_IF( __type_count__, < ) \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT )  \
				BOOST_PP_EXPR_IF( __type_count__, > ) ret_type; \
		BOOST_MPL_ASSERT(( __is_isomeric_same__<base_type, ret_type> )); \
		read_lock_type lk(_mutex); \
		return reinterpret_cast<const ret_type&>(_base); }

// random_access copy
#define YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_RA_IMPL() \
	base_type& copy_to_base(base_type& out, size_type s = 0) const { \
		if(boost::addressof(_base) == boost::addressof(out)) { return out; } \
		{ read_lock_type lk(_mutex); \
		if(s >= _base.size()) { return out; } \
		const_iterator siter = boost::begin(_base); \
		std::advance(siter, s); \
		out.clear(); \
		std::copy(siter, boost::end(_base), \
					std::inserter(out, boost::end(out))); } \
		return out; } \
	\
	base_type& copy_to_base(base_type& out, std::size_t s, std::size_t e) const { \
		if(boost::addressof(_base) == boost::addressof(out)) { return out; } \
		{ read_lock_type lk(_mutex); \
		size_type now_size = _base.size(); \
		if(s >= now_size) { return out; } \
		e = e >= now_size? now_size : e; \
		const_iterator siter, eiter; \
		siter = eiter = boost::begin(_base); \
		std::advance(siter, s); \
		std::advance(eiter, e); \
		out.clear(); \
		std::copy(siter, eiter, std::inserter(out, boost::end(out))); } \
		return out; }

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COPY_TO_BASE_RA_IMPL( \
			__type_count__, __base_tpl__, __is_isomeric_same__ ) \
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >, \
				base_type >, \
			__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >& >::type \
		copy_to_base( __base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >& out, \
						size_type s = 0) const { \
		this_type::copy_to_base(reinterpret_cast<base_type&>(out), s); \
		return out; } \
	\
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >, \
				base_type >, \
			__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >& >::type \
		copy_to_base( __base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >& out, \
						std::size_t s, std::size_t e) const { \
		this_type::copy_to_base(reinterpret_cast<base_type&>(out), s, e); \
		return out; }

#define YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL() \
	base_type& copy_to_base(base_type& out) const { \
		if(boost::addressof(_base) == boost::addressof(out)) { return out; } \
		{read_lock_type lk(_mutex); \
		out = _base; } \
		return out; }

#define YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COPY_TO_BASE_IMPL( \
			__type_count__, __base_tpl__, __is_isomeric_same__ ) \
	BOOST_PP_EXPR_IF( __type_count__, template< ) \
		YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename IMCT ) \
	BOOST_PP_EXPR_IF( __type_count__, > ) inline \
	typename \
		boost::enable_if< \
			__is_isomeric_same__< \
				__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >, \
				base_type >, \
			__base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >& >::type \
		copy_to_base( __base_tpl__< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, IMCT ) >& out ) const { \
		this_type::copy_to_base(reinterpret_cast<base_type&>(out)); \
		return out; }

#define YGGR_PP_SAFE_KV_IS_EXISTS_IMPL() \
	inline bool is_exists(const key_type& key) const { \
		return this_type::find(key); }

#define YGGR_PP_SAFE_UKV_IS_EXISTS_IMPL() \
	YGGR_PP_SAFE_KV_IS_EXISTS_IMPL() \
	\
	template<typename CKey, typename CHash, typename CEq> inline \
	bool is_exists(const CKey& key, const CHash& hash, const CEq& eq) const { \
		return this_type::find(key, hash, eq); }

#define YGGR_PP_SAFE_MAP_RELACE_IMPL() \
	inline size_type replace(const key_type& key, BOOST_RV_REF(mapped_type) val) { \
		value_type tmp(key, mapped_type()); \
		copy_or_move_or_swap(tmp.second, boost::move(val)); \
		return this_type::replace(boost::move(tmp)); } \
	\
	inline size_type replace(const key_type& key, const mapped_type& val) { \
		value_type tmp(key, val); \
		return this_type::replace(boost::move(tmp)); } \
	\
	size_type replace(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(val.first); \
		if(iter == _base.end()) { return 0; } \
		copy_or_move_or_swap(iter->second, boost::move(val.second)); \
		return 1; } \
	\
	size_type replace(const value_type& val) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(val.first); \
		if(iter == _base.end()) { return 0; } \
		iter->second = val.second; \
		return 1; }

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_SAFE_OTHER_IMPL_HPP__
