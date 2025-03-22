// byte_size_helper_def.hpp

/****************************************************************************
Copyright (c) 2014-2022 yggdrasil

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BYTE_SIZE_HELPER_DEF_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BYTE_SIZE_HELPER_DEF_HPP__

#include <yggr/bytes/detail/byte_size_detail_basic.hpp>

#ifdef _MSC_VER
// ----------------------- think you for UnitUniverse (QQ1020676476) fix This macro problem in the MSVC --------------------
#	define YGGR_PP_MAKE_SAFE_CONTAINER_BYTE_SIZE_HELPER( __n__ ) \
		namespace yggr { namespace bytes { namespace detail { \
		template< template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename _T ) \
								YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
								template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __n__, 1 ), typename __T ) > class __IC > class C \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __n__, 1 ), typename _T ) > class IC \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) > \
		struct byte_size_helper< C < YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) \
										YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
										IC> > \
			: public boost::mpl::true_ { \
		public: \
			typedef C < YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) \
										YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
										IC > type; \
			\
		private: \
			template<bool is_class, typename Nil = void> \
			struct inner_helper; \
			\
			template<typename Nil> \
			struct inner_helper<true, Nil> { \
				typedef inner_helper this_type; \
				u64 handler_accmulate(const typename type::base_type& cont) const { \
					typedef typename type::value_type cv_value_type; \
					typedef typename boost::remove_cv<cv_value_type>::type value_type; \
					typedef byte_size_helper<value_type> h_type; \
					h_type h; \
					return std::accumulate(boost::begin(cont), boost::end(cont), 0, \
											boost::bind(&acc_op::foo<value_type, h_type>, \
															_1, _2, boost::cref(h))); } \
				\
				u64 operator()(const type& val) const { \
					return val.using_handler(boost::bind(&this_type::handler_accmulate, this, _1)); } }; \
				\
			template<typename Nil> \
			struct inner_helper<false, Nil> { \
				inline u64 operator()(const type& val) const { \
					typedef typename type::value_type cv_value_type; \
					typedef typename boost::remove_cv<cv_value_type>::type value_type; \
					typedef byte_size_helper<value_type> h_type; \
					h_type h; \
					return val.size() * h(); } }; \
			\
		public: \
			u64 operator()(const type& val) const { \
				typedef typename type::value_type cv_value_type; \
				typedef typename boost::remove_cv<cv_value_type>::type value_type; \
				typedef boost::is_class<value_type> is_class_type; \
				typedef byte_size_helper<value_type> value_helper_type; \
				typedef inner_helper<(is_class_type::value && value_helper_type::value)> ih_type; \
				ih_type ih; \
				return ih(val);	} }; }}}

#else

#	define YGGR_PP_MAKE_SAFE_CONTAINER_BYTE_SIZE_HELPER( __n__ ) \
		namespace yggr { namespace bytes { namespace detail { \
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __n__, 1 ), typename _T ) > class IC \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename _T ) \
								YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
								template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __n__, 1 ), typename __T ) > class __IC > class C > \
		struct byte_size_helper< C < YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) \
										YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
										IC> > \
			: public boost::mpl::true_ { \
		public: \
			typedef C < YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) \
										YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
										IC > type; \
			\
		private: \
			template<bool is_class, typename Nil = void> \
			struct inner_helper; \
			\
			template<typename Nil> \
			struct inner_helper<true, Nil> { \
				typedef inner_helper this_type; \
				u64 handler_accmulate(const typename type::base_type& cont) const { \
					typedef typename type::value_type cv_value_type; \
					typedef typename boost::remove_cv<cv_value_type>::type value_type; \
					typedef byte_size_helper<value_type> h_type; \
					h_type h; \
					return std::accumulate(boost::begin(cont), boost::end(cont), 0, \
											boost::bind(&acc_op::foo<value_type, h_type>, \
															_1, _2, boost::cref(h))); } \
				\
				u64 operator()(const type& val) const { \
					return val.using_handler(boost::bind(&this_type::handler_accmulate, this, _1)); } }; \
				\
			template<typename Nil> \
			struct inner_helper<false, Nil> { \
				inline u64 operator()(const type& val) const { \
					typedef typename type::value_type cv_value_type; \
					typedef typename boost::remove_cv<cv_value_type>::type value_type; \
					typedef byte_size_helper<value_type> h_type; \
					h_type h; \
					return val.size() * h(); } }; \
			\
		public: \
			u64 operator()(const type& val) const { \
				typedef typename type::value_type cv_value_type; \
				typedef typename boost::remove_cv<cv_value_type>::type value_type; \
				typedef boost::is_class<value_type> is_class_type; \
				typedef byte_size_helper<value_type> value_helper_type; \
				typedef inner_helper<(is_class_type::value && value_helper_type::value)> ih_type; \
				ih_type ih; \
				return ih(val);	} }; }}}

#endif // _MSC_VER


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_MAKE_SAFE_CONTAINER_BYTE_SIZE_HELPER( __n__ )
#	define BOOST_PP_LOCAL_LIMITS ( 3, 8 )
#	include BOOST_PP_LOCAL_ITERATE(  )

#endif // __YGGR_SAFE_CONTAINER_DETAIL_BYTE_SIZE_HELPER_DEF_HPP__