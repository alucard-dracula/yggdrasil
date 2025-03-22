//front_back_at_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_FRONT_BACK_AT_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_FRONT_BACK_AT_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

// get set value
#define YGGR_PP_SAFE_SEQ_VALUE_IMPL() \
	bool get_value(size_type idx, value_type& val) const { \
		read_lock_type lk(_mutex); \
		if(idx < _base.size()) { \
			val = _base.at(idx); \
			return true; } \
		return false; } \
	\
	bool set_value(size_type idx, BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		if(idx < _base.size()) { \
			copy_or_move_or_swap(_base.at(idx), boost::move(val)); \
			return true; } \
		return false; } \
	\
	bool set_value(size_type idx, const value_type& val) { \
		value_type tmp(val); \
		write_lock_type lk(_mutex); \
		if(idx < _base.size()) { \
			copy_or_move_or_swap(_base.at(idx), boost::move(tmp)); \
			return true; } \
		return false; }

// get set at
#define YGGR_PP_SAFE_SEQ_AT_IMPL() \
	YGGR_PP_SAFE_SEQ_VALUE_IMPL() \
	inline bool get_at(size_type idx, value_type& val) const { \
		return this_type::get_value(idx, val); } \
	\
	inline bool set_at(size_type idx, BOOST_RV_REF(value_type) val) { \
		return this_type::set_value(idx, boost::move(val)); } \
	\
	inline bool set_at(size_type idx, const value_type& val) { \
		return this_type::set_value(idx, val); }

// get set front
#define YGGR_PP_SAFE_SEQ_FRONT_IMPL() \
	bool get_front(value_type& val) const { \
		read_lock_type lk(_mutex); \
		return (!_base.empty()) && (val = _base.front(), true); } \
	\
	bool set_front(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) \
				&& (copy_or_move_or_swap(_base.front(), boost::move(val)), true); } \
	\
	bool set_front(const value_type& val) { \
		value_type tmp(val); \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) \
				&& (copy_or_move_or_swap(_base.front(), boost::move(tmp)), true); }

//get set back
#define YGGR_PP_SAFE_SEQ_BACK_IMPL() \
	bool get_back(value_type& val) const { \
		read_lock_type lk(_mutex); \
		return (!_base.empty()) && (val = _base.back(), true); } \
	\
	bool set_back(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) \
				&& (copy_or_move_or_swap(_base.back(), boost::move(val)), true); } \
	\
	bool set_back(const value_type& val) { \
		value_type tmp(val); \
		write_lock_type lk(_mutex); \
		return (!_base.empty()) \
				&& (copy_or_move_or_swap(_base.back(), boost::move(tmp)), true); }

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_FRONT_BACK_AT_IMPL_HPP__
