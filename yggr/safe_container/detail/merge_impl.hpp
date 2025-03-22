//merge_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_MERGE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_MERGE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/function.hpp>

#define YGGR_PP_SAFE_CONTAINER_SEQ_MERGE_IMPL() \
	private: \
		template<typename Base> inline \
		static void prv_s_handler_merge_eins(Base& l, Base& r, bool need_sort_r) { \
			if(need_sort_r) { r.sort(); } \
			l.merge(r); } \
		\
		template<typename Base, typename Pred> inline \
		static void prv_s_handler_merge_zwei(Base& l, Base& r, \
												const utility::basic_args_holder_nv& any_cmp, \
												bool need_sort_r) { \
			typedef Pred pred_type; \
			const pred_type* pcmp = utility::args_holder_ref_wrap_static_cast<Pred const>(any_cmp); \
			assert(pcmp); \
			if(!pcmp) { return; } \
			if(need_sort_r) { r.sort(*pcmp); } \
			l.merge(r, *pcmp); } \
	public: \
		inline void merge(BOOST_RV_REF(base_type) right, \
							bool need_sort_self = true, \
							bool need_sort_other = true) { \
			base_type& right_ref = right; \
			this_type::merge(right_ref, need_sort_self, need_sort_other); } \
		\
		void merge(base_type& right, \
					bool need_sort_self = true, \
					bool need_sort_other = true) { \
			if(need_sort_other){ right.sort(); } \
			write_lock_type lk(_mutex); \
			if(need_sort_self) { _base.sort(); } \
			_base.merge(right); } \
		\
		template<typename Pred> inline \
		void merge(BOOST_RV_REF(base_type) right, Pred comp, \
					bool need_sort_self = true, bool need_sort_other = true) { \
			base_type& right_ref = right; \
			this_type::merge(right_ref, comp, need_sort_self, need_sort_other); } \
		\
		template<typename Pred> \
		void merge(base_type& right, Pred comp, \
					bool need_sort_self = true, \
					bool need_sort_other = true) { \
			if(need_sort_other) { right.sort(comp); } \
			write_lock_type lk(_mutex); \
			if(need_sort_other) { _base.sort(comp); } \
			_base.merge(right, comp); } \
		\
		inline void merge(BOOST_RV_REF(this_type) right, \
							bool need_sort_self = true, \
							bool need_sort_other = true) { \
			this_type& right_ref = right; \
			this_type::merge(right_ref, need_sort_self, need_sort_other); } \
		\
		void merge(this_type& right, \
					bool need_sort_self = true, \
					bool need_sort_other = true) { \
			write_lock_type lk(_mutex); \
			if(need_sort_self) { _base.sort(); } \
			right.using_handler( \
				boost::bind( \
					&this_type::prv_s_handler_merge_eins<base_type>, \
					boost::ref(_base), _1, need_sort_other)); } \
		\
		template<typename Pred> inline \
		void merge(BOOST_RV_REF(this_type) right, Pred comp, \
					bool need_sort_self = true, \
					bool need_sort_other = true) { \
			this_type& right_ref = right; \
			this_type::merge(right_ref, comp, need_sort_self, need_sort_other); } \
		\
		template<typename Pred> \
		void merge(this_type& right, Pred comp, \
					bool need_sort_self = true, \
					bool need_sort_other = true) { \
			typedef Pred cmp_type; \
			typedef utility::args_holder_nv_ref_wrap<cmp_type const> cmp_holder_type; \
			cmp_holder_type cmp_holder(comp); \
			write_lock_type lk(_mutex); \
			if(need_sort_self) { _base.sort(comp); } \
			right.using_handler( \
				boost::bind( \
					&this_type::prv_s_handler_merge_zwei<base_type, cmp_type>, \
					boost::ref(_base), _1, \
					boost::cref(utility::args_holder_trans(cmp_holder)), \
					need_sort_other)); }

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_MERGE_IMPL_HPP__
