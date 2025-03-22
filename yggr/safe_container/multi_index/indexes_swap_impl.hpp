//indexes_swap_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_SWAP_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_SWAP_IMPL_HPP__

#include <yggr/safe_container/detail/swap_impl.hpp>
#include <yggr/ppex/symbols.hpp>

#define YGGR_PP_SAFE_MIDX_INDEXES_SWAP_IMPL() \
	YGGR_PP_SAFE_CONTAINER_SWAP_IMPL() \
	\
	inline void swap(BOOST_RV_REF(final_type) right) { \
		final_type& right_ref = right; \
		this_type::swap(right_ref); } \
	\
	void swap(final_type& right) { \
		write_lock_type lk(_mutex); \
		_base.swap(right.template get<N>()); } \
	\
	template<typename Vx, typename Ix, typename Mx, typename Ax, \
				template<typename _V, typename _I, typename _A> class Midx> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_multi_index_container<Vx, Ix, Mx, Ax, Midx>::base_type, \
				final_type >, \
			void >::type \
		swap(BOOST_RV_REF_BEG safe_multi_index_container< Vx, Ix, Mx, Ax, Midx > BOOST_RV_REF_END right ) { \
		this_type::swap(boost::move(right.template get<N>())); } \
	\
	template<typename Vx, typename Ix, typename Mx, typename Ax, \
				template<typename _V, typename _I, typename _A> class Midx> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_multi_index_container<Vx, Ix, Mx, Ax, Midx>::base_type, \
				final_type >, \
			void >::type \
		swap(safe_multi_index_container<Vx, Ix, Mx, Ax, Midx>& right) { \
		this_type::swap(right.template get<N>()); }

#define YGGR_PP_SAFE_MIDX_INDEXES_ISOMERIC_SAME_SWAP_IMPL( __safe_tpl__ ) \
	template<typename M2> inline \
	void swap(BOOST_RV_REF_BEG \
					__safe_tpl__<N, final_type, M2> \
				BOOST_RV_REF_END right) { \
		typedef __safe_tpl__<N, final_type, M2> right_type; \
		right_type& right_ref = right; \
		this_type::swap(right_ref); } \
	\
	template<typename M2> \
	void swap( __safe_tpl__<N, final_type, M2>& right) { \
 		write_lock_type lk(_mutex); \
		right.swap(_base); }


namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_SWAP_IMPL_HPP__

