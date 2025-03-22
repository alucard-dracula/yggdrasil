// begin_rbegin_value_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BEGIN_RBEGIN_VALUE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BEGIN_RBEGIN_VALUE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/iterator_ex/iterator.hpp>

// seq begin rbegin value
#define YGGR_PP_SAFE_SEQ_BEGIN_VALUE_FUCTION_IMPL() \
	inline value_type begin_value(void) const { \
		return this_type::unsafe_get_front(); } \
	\
	inline bool get_begin_value(value_type& val) const { \
		return this_type::get_front(val); } \
	\
	inline bool set_begin_value(BOOST_RV_REF(value_type) val) { \
		return this_type::set_front(boost::move(val)); } \
	\
	inline bool set_begin_value(const value_type& val) { \
		return this_type::set_front(val); }

#define YGGR_PP_SAFE_SEQ_RBEGIN_VALUE_FUCTION_IMPL() \
	inline value_type rbegin_value(void) const { \
		return this_type::unsafe_get_back(); } \
	\
	inline bool get_rbegin_value(value_type& val) const { \
		return this_type::get_back(val); } \
	\
	inline bool set_rbegin_value(BOOST_RV_REF(value_type) val) { \
		return this_type::set_back(boost::move(val)); } \
	\
	inline bool set_rbegin_value(const value_type& val) { \
		return this_type::set_back(val); }

// unseq begin rbegin value
#define YGGR_PP_SAFE_UNSEQ_BEGIN_VALUE_FUNCTION_IMPL() \
	value_type begin_value(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return *(_base.begin()); } \
	\
	bool get_begin_value(value_type& val) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (val = *(_base.begin()), true); } \
	\
	bool set_begin_value(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		return (_base.size()) && (copy_or_move_or_swap(*(_base.begin()), boost::move(val)), true); } \
	\
	bool set_begin_value(const value_type& val) { \
		value_type tmp(val); \
		write_lock_type lk(_mutex); \
		return (_base.size()) && (copy_or_move_or_swap(*(_base.begin()), boost::move(tmp)), true); } 

#define YGGR_PP_SAFE_UNSEQ_RBEGIN_VALUE_FUNCTION_IMPL() \
	value_type rbegin_value(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return *(_base.rbegin()); } \
	\
	bool get_rbegin_value(value_type& val) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (val = *(_base.rbegin()), true); } \
	\
	bool set_rbegin_value(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		return (_base.size()) && (copy_or_move_or_swap(*(_base.rbegin()), boost::move(val)), true); } \
	\
	bool set_rbegin_value(const value_type& val) { \
		value_type tmp(val); \
		write_lock_type lk(_mutex); \
		return (_base.size()) && (copy_or_move_or_swap(*(_base.rbegin()), boost::move(tmp)), true); }

#define YGGR_PP_SAFE_UV_UNSEQ_BEGIN_VALUE_FUNCTION_IMPL() \
	value_type begin_value(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return *(_base.begin()); } \
	\
	bool get_begin_value(value_type& val) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (val = *(_base.begin()), true); }

// #define YGGR_PP_SAFE_UV_UNSEQ_RBEGIN_VALUE_FUNCTION_IMPL() // hash don't has this

// kv begin rbegin value
#define YGGR_PP_SAFE_KV_BEGIN_VALUE_FUNCTION_IMPL() \
	value_type begin_value(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return *(_base.begin()); } \
	\
	key_type begin_key(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return _base.begin()->first; } \
	\
	mapped_type begin_mapped(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return _base.begin()->second; } \
	\
	bool get_begin_value(std::pair<key_type, mapped_type>& val) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (val = *(_base.begin()), true); } \
	\
	bool get_begin_key(key_type& key) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (key = _base.begin()->first, true); } \
	\
	bool get_begin_mapped(mapped_type& mapped) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (mapped = _base.begin()->second, true); } \
	\
	bool set_begin_mapped(BOOST_RV_REF(mapped_type) mapped) { \
		write_lock_type lk(_mutex); \
		return (_base.size()) \
				&& (copy_or_move_or_swap(_base.begin()->second, boost::move(mapped)), true); } \
	\
	bool set_begin_mapped(const mapped_type& mapped) { \
		mapped_type tmp(mapped); \
		write_lock_type lk(_mutex); \
		return (_base.size()) \
				&& (copy_or_move_or_swap(_base.begin()->second, boost::move(tmp)), true); }

#define YGGR_PP_SAFE_KV_RBEGIN_VALUE_FUNCTION_IMPL() \
	value_type rbegin_value(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return *(_base.rbegin()); } \
	\
	key_type rbegin_key(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return _base.rbegin()->first; } \
	\
	mapped_type rbegin_mapped(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return _base.rbegin()->second; } \
	\
	bool get_rbegin_value(std::pair<key_type, mapped_type>& val) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (val = *(_base.rbegin()), true); } \
	\
	bool get_rbegin_key(key_type& key) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (key = _base.rbegin()->first, true); } \
	\
	bool get_rbegin_mapped(mapped_type& mapped) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (mapped = _base.rbegin()->second, true); } \
	\
	bool set_rbegin_mapped(BOOST_RV_REF(mapped_type) mapped) { \
		write_lock_type lk(_mutex); \
		return (_base.size()) \
				&& (copy_or_move_or_swap(_base.rbegin()->second, boost::move(mapped)), true); } \
	\
	bool set_rbegin_mapped(const mapped_type& mapped) { \
		mapped_type tmp(mapped); \
		write_lock_type lk(_mutex); \
		return (_base.size()) \
				&& (copy_or_move_or_swap(_base.rbegin()->second, boost::move(tmp)), true); }

// ukv begin rbegin value
#define YGGR_PP_SAFE_UKV_BEGIN_VALUE_FUNCTION_IMPL() \
	value_type begin_value(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return *(_base.begin()); } \
	\
	key_type begin_key(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return _base.begin()->first; } \
	\
	mapped_type begin_mapped(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return _base.begin()->second; } \
	\
	bool get_begin_value(std::pair<key_type, mapped_type>& val) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (val = *(_base.begin()), true); } \
	\
	bool get_begin_key(key_type& key) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (key = _base.begin()->first, true); } \
	\
	bool get_begin_mapped(mapped_type& mapped) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (mapped = _base.begin()->second, true); }

// #define YGGR_PP_SAFE_UKV_RBEGIN_VALUE_FUNCTION_IMPL() // hash don't has this

// midx
#define YGGR_PP_SAFE_MIDX_SEQ_BEGIN_VALUE_FUNCTION_IMPL() \
	inline value_type begin_value(void) const { \
		return this_type::front(); } \
	\
	inline bool get_begin_value(value_type& val) const { \
		return this_type::get_front(val); } 

#define YGGR_PP_SAFE_MIDX_SEQ_RBEGIN_VALUE_FUNCTION_IMPL() \
	inline value_type rbegin_value(void) const { \
		return this_type::back(); } \
	\
	inline bool get_rbegin_value(value_type& val) const { \
		return this_type::get_back(val); }

#define YGGR_PP_SAFE_MIDX_ORD_BEGIN_VALUE_FUNCTION_IMPL() \
	value_type begin_value(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return *(_base.cbegin()); } \
	\
	bool get_begin_value(value_type& val) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (val = *(_base.cbegin()), true); }

#define YGGR_PP_SAFE_MIDX_ORD_RBEGIN_VALUE_FUNCTION_IMPL() \
	value_type rbegin_value(void) const { \
		read_lock_type lk(_mutex); \
		if(!_base.size()) { \
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return *(_base.crbegin()); } \
	\
	bool get_rbegin_value(value_type& val) const { \
		read_lock_type lk(_mutex); \
		return (_base.size()) && (val = *(_base.crbegin()), true); }

#define YGGR_PP_SAFE_MIDX_HASH_BEGIN_VALUE_FUNCTION_IMPL YGGR_PP_SAFE_MIDX_ORD_BEGIN_VALUE_FUNCTION_IMPL

// #define YGGR_PP_SAFE_MIDX_HASH_RBEGIN_VALUE_FUNCTION_IMPL() // hash don't has this

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BEGIN_RBEGIN_VALUE_IMPL_HPP__
