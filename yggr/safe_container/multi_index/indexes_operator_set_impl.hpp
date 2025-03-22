//indexes_operator_set_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_OPERATOR_SET_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_OPERATOR_SET_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#   include <initializer_list>
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#	define YGGR_PP_SAFE_MIDX_INDEXES_OPERATOR_SET_IL_IMPL() \
		this_type& operator=(std::initializer_list<value_type> il) { \
			final_type tmp(boost::begin(il), boost::end(il), typename final_type::ctor_args_list(), this_type::get_allocator()); \
			base_type& tmp_base = tmp.template get<N>(); \
			write_lock_type lk(_mutex); \
			_base.swap(tmp_base); \
			return *this; }

#else

#	define YGGR_PP_SAFE_MIDX_INDEXES_OPERATOR_SET_IL_IMPL()

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

// operator_set
#define YGGR_PP_SAFE_MIDX_INDEXES_OPERATOR_SET_IMPL() \
	inline this_type& operator=(BOOST_RV_REF(base_type) right) { \
		base_type& right_chk = right; \
		if(&_base == &right_chk) { return *this; } \
		final_type tmp(this_type::get_allocator()); \
		base_type& tmp_base = tmp.template get<N>(); \
		copy_or_move_or_swap(tmp_base, boost::move(right)); \
		write_lock_type lk(_mutex); \
		_base.swap(tmp_base); \
		return *this; } \
	\
	this_type& operator=(const base_type& right) { \
		if(&_base == &right) { return *this; } \
		final_type tmp(this_type::get_allocator()); \
		base_type& tmp_base = tmp.template get<N>(); \
		tmp_base = right; \
		write_lock_type lk(_mutex); \
		_base.swap(tmp_base); \
		return *this; } \
	\
	inline this_type& operator=(BOOST_RV_REF(final_type) right) { \
		base_type& right_chk = right.template get<N>(); \
		if(&_base == &right_chk) { return *this; } \
		final_type tmp(this_type::get_allocator()); \
		base_type& tmp_base = tmp.template get<N>(); \
		copy_or_move_or_swap(tmp_base, boost::move(right_chk)); \
		write_lock_type lk(_mutex); \
		_base.swap(tmp_base); \
		return *this; } \
	\
	this_type& operator=(const final_type& right) { \
		const base_type& right_chk = right.template get<N>(); \
		if(&_base == &right_chk) { return *this; } \
		final_type tmp(this_type::get_allocator()); \
		base_type& tmp_base = tmp.template get<N>(); \
		tmp_base = right_chk; \
		write_lock_type lk(_mutex); \
		_base.swap(tmp_base); \
		return *this; } \
	\
	inline this_type& operator=(BOOST_RV_REF(this_type) right) { \
		if(this == rvref_to_pointer(boost::move(right))) { \
			return *this; } \
		this_type& right_ref = right; \
		write_lock_type lk(_mutex); \
		right_ref.swap(_base); \
		return *this; } \
	\
	this_type& operator=(const this_type& right) { \
		if(this == &right) { return *this; } \
		write_lock_type lk(_mutex); \
		right.copy_to_base(_base); \
		return *this; } \
	\
	template<typename Vx, typename Ix, typename Mx, typename Ax, \
				template<typename _V, typename _I, typename _A> class Midx> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_multi_index_container<Vx, Ix, Mx, Ax, Midx>::base_type, \
				final_type >, \
			this_type& >::type \
		operator=(BOOST_RV_REF_BEG safe_multi_index_container< Vx, Ix, Mx, Ax, Midx > BOOST_RV_REF_END right ) { \
		return this_type::operator=(boost::move(right.template get<N>())); } \
	\
	template<typename Vx, typename Ix, typename Mx, typename Ax, \
				template<typename _V, typename _I, typename _A> class Midx> \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_multi_index_container<Vx, Ix, Mx, Ax, Midx>::base_type, \
				final_type >, \
			this_type& >::type \
		operator=(const safe_multi_index_container<Vx, Ix, Mx, Ax, Midx>& right) { \
		return this_type::operator=(right.template get<N>()); } \
	\
	YGGR_PP_SAFE_MIDX_INDEXES_OPERATOR_SET_IL_IMPL()

#define YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_OPERATOR_SET_IMPL( __safe_tpl__ ) \
	template<typename M2> \
	this_type& operator=(BOOST_RV_REF_BEG \
								__safe_tpl__<N, final_type, M2> \
							BOOST_RV_REF_END right) { \
		typedef __safe_tpl__<N, final_type, M2> right_type; \
		right_type& right_ref = right; \
		write_lock_type lk(_mutex); \
		right_ref.swap(_base); \
		return *this; } \
	\
	template<typename M2> \
	this_type& operator=(const __safe_tpl__<N, final_type, M2>& right) { \
 		write_lock_type lk(_mutex); \
		right.copy_to_base(_base); \
		return *this; }

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_OPERATOR_SET_IMPL_HPP__
