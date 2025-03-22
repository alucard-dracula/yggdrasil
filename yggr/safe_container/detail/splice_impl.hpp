//splice_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_SPLICE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_SPLICE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/args_holder_nv.hpp>
#include <yggr/tuple/tuple.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <boost/function.hpp>

#define YGGR_PP_SAFE_CONTAINER_SPLICE_IMPL() \
	private: \
		template<typename Right, typename DstIterHandler, typename SrcEIterHandler> inline \
		static void prv_s_right_using_handler_splice(typename Right::base_type& lbase, Right& right, \
														const DstIterHandler& dst_iter_handler, \
														const SrcEIterHandler& src_eins_iter_handler ) { \
			typedef Right right_type; \
			typedef typename right_type::base_type right_base_type; \
			typedef DstIterHandler dst_iter_handler_type; \
			typedef SrcEIterHandler src_eins_handler_type; \
			typedef utility::args_holder_nv_ref_wrap<dst_iter_handler_type const> dst_iter_handler_holder_type; \
			typedef utility::args_holder_nv_ref_wrap<src_eins_handler_type const> src_eins_handler_holder_type; \
			dst_iter_handler_holder_type dst_iter_handler_holder(dst_iter_handler); \
			src_eins_handler_holder_type src_eins_handler_holder(src_eins_iter_handler); \
			right.using_handler( \
				boost::bind( \
					&this_type::prv_s_handler_splice2<right_base_type, dst_iter_handler_type, src_eins_handler_type>, \
					boost::ref(lbase), _1, \
					boost::cref(utility::args_holder_trans(dst_iter_handler_holder)), \
					boost::cref(utility::args_holder_trans(src_eins_handler_holder)))); } \
		\
		template<typename Right, typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> inline \
		static void prv_s_right_using_handler_splice(typename Right::base_type& lbase, Right& right, \
														const DstIterHandler& dst_iter_handler, \
														const SrcEIterHandler& src_eins_iter_handler, \
														const SrcZIterHandler& src_zwei_iter_handler ) { \
			typedef Right right_type; \
			typedef typename right_type::base_type right_base_type; \
			typedef DstIterHandler dst_iter_handler_type; \
			typedef SrcEIterHandler src_eins_handler_type; \
			typedef SrcZIterHandler src_zwei_handler_type; \
			typedef utility::args_holder_nv_ref_wrap<dst_iter_handler_type const> dst_iter_handler_holder_type; \
			typedef utility::args_holder_nv_ref_wrap<src_eins_handler_type const> src_eins_handler_holder_type; \
			typedef utility::args_holder_nv_ref_wrap<src_zwei_handler_type const> src_zwei_handler_holder_type; \
			dst_iter_handler_holder_type dst_iter_handler_holder(dst_iter_handler); \
			src_eins_handler_holder_type src_eins_handler_holder(src_eins_iter_handler); \
			src_zwei_handler_holder_type src_zwei_handler_holder(src_zwei_iter_handler); \
			right.using_handler( \
				boost::bind( \
					&this_type::prv_s_handler_splice3< \
						right_base_type, dst_iter_handler_type, src_eins_handler_type, src_zwei_handler_type>, \
					boost::ref(lbase), _1, \
					boost::cref(utility::args_holder_trans(dst_iter_handler_holder)), \
					boost::cref(utility::args_holder_trans(src_eins_handler_holder)), \
					boost::cref(utility::args_holder_trans(src_zwei_handler_holder)) ) ); } \
		\
		template<typename Base, typename DstIterHandler, typename SrcEIterHandler> inline \
		static \
		typename \
			boost::enable_if< \
				boost::is_same< \
					typename boost::remove_reference< \
						typename boost::remove_cv< \
							typename func::foo_t_info< SrcEIterHandler >::result_type \
						>::type \
					>::type, iterator >, \
				void >::type \
			prv_s_handler_splice2(Base& l, Base& r, \
									const utility::basic_args_holder_nv& any_dst_iter_handler, \
									const utility::basic_args_holder_nv& any_src_eiter_handler) { \
			typedef Base now_base_type; \
			typedef typename now_base_type::iterator now_base_iter_type; \
			BOOST_MPL_ASSERT((boost::is_same<now_base_iter_type, iterator>)); \
			typedef DstIterHandler dst_iter_handler_type; \
			typedef SrcEIterHandler src_eiter_handler_type; \
			const dst_iter_handler_type* pdst_iter_handler = \
				utility::args_holder_ref_wrap_static_cast<dst_iter_handler_type const>(any_dst_iter_handler); \
			const src_eiter_handler_type* psrc_eiter_handler = \
				utility::args_holder_ref_wrap_static_cast<src_eiter_handler_type const>(any_src_eiter_handler); \
			assert(pdst_iter_handler && psrc_eiter_handler); \
			if(!(pdst_iter_handler && psrc_eiter_handler)) { return; } \
			now_base_iter_type riter = (*psrc_eiter_handler)(r); \
			if(riter == r.end()) { return; } \
			l.splice( (*pdst_iter_handler)(l), r, riter ); } \
		\
		template<typename Base, typename DstIterHandler, typename SrcEIterHandler> inline \
		static \
		typename \
			boost::disable_if< \
				boost::is_same< \
					typename boost::remove_reference< \
						typename boost::remove_cv< \
							typename func::foo_t_info< SrcEIterHandler >::result_type \
						>::type \
					>::type, iterator >, \
				void >::type \
			prv_s_handler_splice2(Base& l, Base& r, \
									const utility::basic_args_holder_nv& any_dst_iter_handler, \
									const utility::basic_args_holder_nv& any_src_eiter_handler) { \
			typedef Base now_base_type; \
			typedef typename now_base_type::iterator now_base_iter_type; \
			BOOST_MPL_ASSERT((boost::is_same<now_base_iter_type, iterator>)); \
			typedef DstIterHandler dst_iter_handler_type; \
			typedef SrcEIterHandler src_eiter_handler_type; \
			const dst_iter_handler_type* pdst_iter_handler = \
				utility::args_holder_ref_wrap_static_cast<dst_iter_handler_type const >(any_dst_iter_handler); \
			const src_eiter_handler_type* psrc_eiter_handler = \
				utility::args_holder_ref_wrap_static_cast<src_eiter_handler_type const>(any_src_eiter_handler); \
			assert(pdst_iter_handler && psrc_eiter_handler); \
			if(!(pdst_iter_handler && psrc_eiter_handler)) { return; } \
			std::pair<now_base_iter_type, now_base_iter_type> riter_range = (*psrc_eiter_handler)(r); \
			l.splice( (*pdst_iter_handler)(l), r, riter_range.first, riter_range.second ); } \
		\
		template<typename Base, typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> inline \
		static void prv_s_handler_splice3(Base& l, Base& r, \
											const utility::basic_args_holder_nv& any_dst_iter_handler, \
											const utility::basic_args_holder_nv& any_src_eiter_handler, \
											const utility::basic_args_holder_nv& any_src_ziter_handler) { \
			typedef DstIterHandler dst_iter_handler_type; \
			typedef SrcEIterHandler src_eiter_handler_type; \
			typedef SrcZIterHandler src_ziter_handler_type; \
			const dst_iter_handler_type* pdst_iter_handler = \
				utility::args_holder_ref_wrap_static_cast<dst_iter_handler_type const>(any_dst_iter_handler); \
			const src_eiter_handler_type* psrc_eiter_handler = \
				utility::args_holder_ref_wrap_static_cast<src_eiter_handler_type const>(any_src_eiter_handler); \
			const src_eiter_handler_type* psrc_ziter_handler = \
				utility::args_holder_ref_wrap_static_cast<src_ziter_handler_type const>(any_src_ziter_handler); \
			assert(pdst_iter_handler && psrc_eiter_handler && psrc_ziter_handler); \
			if(!(pdst_iter_handler && psrc_eiter_handler && psrc_ziter_handler)) { return; } \
			l.splice( (*pdst_iter_handler)(l), r, (*psrc_eiter_handler)(r), (*psrc_ziter_handler)(r) ); } \
	\
	public: \
		inline void splice(BOOST_RV_REF(base_type) right) { \
			base_type& right_ref = right; \
			this_type::splice(right_ref); } \
		\
		void splice(base_type& right) { \
			write_lock_type lk(_mutex); \
			_base.splice(_base.end(), right); } \
		\
		inline void splice(BOOST_RV_REF(base_type) right, iterator i) { \
			base_type& right_ref = right; \
			this_type::splice(right_ref, i); } \
		\
		void splice(base_type& right, iterator i) { \
			write_lock_type lk(_mutex); \
			_base.splice(_base.end(), right, i); } \
		\
		inline void splice(BOOST_RV_REF(base_type) right, iterator first, iterator last) { \
			base_type& right_ref = right; \
			this_type::splice(right_ref, first, last); } \
		\
		void splice(base_type& right, iterator first, iterator last) { \
			write_lock_type lk(_mutex); \
			_base.splice(_base.end(), right, first, last); } \
		\
		template<typename IterHandler> inline \
		void splice(const IterHandler& iter_handler, BOOST_RV_REF(base_type) right) { \
			base_type& right_ref = right; \
			this_type::splice(iter_handler, right_ref); } \
		\
		template<typename IterHandler> \
		void splice(const IterHandler& iter_handler, base_type& right) { \
			write_lock_type lk(_mutex); \
			_base.splice(iter_handler(_base), right); } \
		\
		template<typename IterHandler> inline \
		void splice(const IterHandler& iter_handler, \
						BOOST_RV_REF(base_type) right, iterator i) { \
			base_type& right_ref = right; \
			this_type::splice(iter_handler, right_ref, i); } \
		\
		template<typename IterHandler> \
		void splice(const IterHandler& iter_handler, base_type& right, iterator i) { \
			write_lock_type lk(_mutex); \
			_base.splice(iter_handler(_base), right, i); } \
		\
		template<typename IterHandler> \
		void splice(const IterHandler& iter_handler, \
					BOOST_RV_REF(base_type) right, iterator first, iterator last) { \
			base_type& right_ref = right; \
			this_type::splice(iter_handler, right_ref, first, last); } \
		\
		template<typename IterHandler> \
		void splice(const IterHandler& iter_handler, \
					base_type& right, iterator first, iterator last) { \
			write_lock_type lk(_mutex); \
			_base.splice(iter_handler(_base), right, first, last); } \
		\
		inline void splice(BOOST_RV_REF(this_type) right) { \
			this_type& right_ref = right; \
			this_type::splice(right_ref); } \
		\
		void splice(this_type& right) { \
			typedef iterator (base_type::* iter_foo_type)(void); \
			write_lock_type lk(_mutex); \
			this_type::prv_s_right_using_handler_splice( \
				_base, right, \
				boost::bind(static_cast<iter_foo_type>(&base_type::end), _1), \
				boost::bind(static_cast<iter_foo_type>(&base_type::begin), _1), \
				boost::bind(static_cast<iter_foo_type>(&base_type::end), _1) ); } \
		\
		template<typename SrcEIterHandler> inline \
		void splice(BOOST_RV_REF(this_type) right, const SrcEIterHandler& src_eins_iter_handler) { \
			this_type& right_ref = right; \
			this_type::splice(right_ref, src_eins_iter_handler); } \
		\
		template<typename SrcEIterHandler> \
		void splice(this_type& right, const SrcEIterHandler& src_eins_iter_handler) { \
			typedef iterator (base_type::* iter_foo_type)(void); \
			write_lock_type lk(_mutex); \
			this_type::prv_s_right_using_handler_splice( \
				_base, right, \
				boost::bind(static_cast<iter_foo_type>(&base_type::end), _1), \
				src_eins_iter_handler ); } \
		\
		template<typename SrcEIterHandler, typename SrcZIterHandler> inline \
		void splice(BOOST_RV_REF(this_type) right, \
					const SrcEIterHandler& src_eins_iter_handler, \
					const SrcZIterHandler& src_zwei_iter_handler ) { \
			this_type& right_ref = right; \
			this_type::splice(right_ref, src_eins_iter_handler, src_zwei_iter_handler); } \
		\
		template<typename SrcEIterHandler, typename SrcZIterHandler> \
		void splice(this_type& right, \
					const SrcEIterHandler& src_eins_iter_handler, \
					const SrcZIterHandler& src_zwei_iter_handler ) { \
			typedef iterator (base_type::* iter_foo_type)(void); \
			write_lock_type lk(_mutex); \
			this_type::prv_s_right_using_handler_splice( \
				_base, right, \
				boost::bind(static_cast<iter_foo_type>(&base_type::end), _1), \
				src_eins_iter_handler, \
				src_zwei_iter_handler ); } \
		\
		template<typename IterHandler> inline \
		void splice(const IterHandler& iter_handler, BOOST_RV_REF(this_type) right) { \
			this_type& right_ref = right; \
			this_type::splice(iter_handler, right_ref); } \
		\
		template<typename IterHandler> \
		void splice(const IterHandler& iter_handler, this_type& right) { \
			typedef iterator (base_type::* iter_foo_type)(void); \
			write_lock_type lk(_mutex); \
			this_type::prv_s_right_using_handler_splice( \
				_base, right, \
				iter_handler, \
				boost::bind(static_cast<iter_foo_type>(&base_type::begin), _1), \
				boost::bind(static_cast<iter_foo_type>(&base_type::end), _1) ); } \
		\
		template<typename DstIterHandler, typename SrcEIterHandler> inline \
		void splice(const DstIterHandler& dst_iter_handler, \
					BOOST_RV_REF(this_type) right, \
					const SrcEIterHandler& src_eins_iter_handler) { \
			this_type& right_ref = right; \
			this_type::splice(dst_iter_handler, right_ref, src_eins_iter_handler); } \
		\
		template<typename DstIterHandler, typename SrcEIterHandler> \
		void splice(const DstIterHandler& dst_iter_handler, \
					this_type& right, \
					const SrcEIterHandler& src_eins_iter_handler) { \
			write_lock_type lk(_mutex); \
			this_type::prv_s_right_using_handler_splice( \
				_base, right, \
				dst_iter_handler, \
				src_eins_iter_handler ); } \
		\
		template<typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> inline \
		void splice(const DstIterHandler& dst_iter_handler, \
					BOOST_RV_REF(this_type) right, \
					const SrcEIterHandler& src_eins_iter_handler, \
					const SrcZIterHandler& src_zwei_iter_handler) { \
			this_type& right_ref = right; \
			this_type::splice(dst_iter_handler, right_ref, src_eins_iter_handler, src_zwei_iter_handler); } \
		\
		template<typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> \
		void splice(const DstIterHandler& dst_iter_handler, \
					this_type& right, \
					const SrcEIterHandler& src_eins_iter_handler, \
					const SrcZIterHandler& src_zwei_iter_handler) { \
			write_lock_type lk(_mutex); \
			this_type::prv_s_right_using_handler_splice( \
				_base, right, \
				dst_iter_handler, \
				src_eins_iter_handler, \
				src_zwei_iter_handler ); }

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_SPLICE_IMPL_HPP__
